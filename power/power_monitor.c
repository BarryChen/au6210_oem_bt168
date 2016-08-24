#include "syscfg.h"
#include "message.h"
#include "debug.h"
#include "key.h"
#include "sysctrl.h"
#include "adc.h"
#include "timer.h"
#include "utility.h"
#include "power.h"
#include "device.h"
#include "display.h"
#include "Sys_on_off.h"
#include "Nvm.h"
#include "power_monitor.h"
#include "sled_disp.h"

#ifdef FUNC_DISP_EN
#include "seg_panel.h"
#include "Lcd_seg.h"
#endif
extern VOID UnMute(VOID);
extern BOOL GetBtMuteFlag();

#ifdef FUNC_POWER_MONITOR_EN

#define LDOIN_SAMPLE_COUNT			100		//��ȡLDOIN����ʱ����ƽ���Ĳ�������
#define LDOIN_SAMPLE_PERIOD			50		//��ȡLDOIN����ʱ��ȡ����ֵ�ļ��(ms)
#define LOW_POWEROFF_TIME			10000		//�͵���ػ��������ʱ��(ms)
#define LOW_POWER_SONG_TIME			30000		//


//���¶��岻ͬ�ĵ�ѹ����¼��Ĵ�����ѹ(��λmV)���û���������ϵͳ��ص��ص�������


#define LDOIN_VOLTAGE_FULL			4200
#define LDOIN_VOLTAGE_HIGH			3600
#define LDOIN_VOLTAGE_LOW			3400
#define LDOIN_VOLTAGE_OFF			3200	//���ڴ˵�ѹֵ����ػ�powerdown״̬

//��ѹ���ʱ��ͬ����ʾ����


//��Ҫ���ڵ�ѹ״̬��ʾ�ı���
static TIMER BlinkTimer;
static PWR_MNT_DISP PwrMntDisp;
static BOOL LowPwr_Flag = TRUE;



//���ڵ�ѹ���ı���
TIMER LowPowerSongTimer;

TIMER PowerMonitorTimer;
TIMER PowerOffTimer;
DWORD LdoinSampleSum = 0; 
WORD  LdoinSampleCnt = LDOIN_SAMPLE_COUNT;
DWORD LdoinLevelAverage = 0;		//��ǰLDOIN��ѹƽ��ֵ

extern BOOL SongPlayDo(VOID);		
extern BOOL SPI_PlaySelectNum(BYTE SpiPlayNum, BOOL PlayResumeFlag);

PWR_MNT_DISP GetPwrDisp(void)
{
	return PwrMntDisp;
}
VOID PowerMonitorDisp(VOID)
{
#if (defined(FUNC_SEG_LED_EN) || defined(FUNC_SEG_LCD_EN))
	static BOOL IsToShow = FALSE;

	switch(PwrMntDisp)
	{
		case PWR_MNT_DISP_CHARGE:
			//��˸���ICON,��ʾ���ڳ��
		    if(IsTimeOut(&BlinkTimer))
			{
				TimeOutSet(&BlinkTimer, 500);
				if(IsToShow)
				{
					//DBG(("Charge 111\n"));						
					DispIcon(ICON_BATFUL, TRUE);
					DispIcon(ICON_BATHAF, FALSE);
	                IsToShow = FALSE;
				}
				else
				{
					//DBG(("Charge 000\n"));						
					DispIcon(ICON_BATFUL, FALSE);
					DispIcon(ICON_BATHAF, TRUE);
					IsToShow = TRUE;
				}
			}
			break;

		case PWR_MNT_DISP_HIGH_V:
			//DBG(("BAT FULL\n"));				
			DispIcon(ICON_BATFUL, TRUE);
			DispIcon(ICON_BATHAF, FALSE);
			//������ʾ����������������ʾ����
			break;
			
		case PWR_MNT_DISP_MID_V:
			//DBG(("BAT HALF\n"));				
			DispIcon(ICON_BATFUL, FALSE);
			DispIcon(ICON_BATHAF, TRUE);
			//������ʾ2��������������ʾ����
			break;

		case PWR_MNT_DISP_LOW_V:
			DispIcon(ICON_BATFUL, FALSE);
			DispIcon(ICON_BATHAF, TRUE);
			//������ʾ1��������������ʾ����
			break;
			
		case PWR_MNT_DISP_EMPTY_V:
			//DBG(("BAT EMPTY\n"));				
			DispIcon(ICON_BATFUL, FALSE);

			if(IsTimeOut(&BlinkTimer))
			{
				TimeOutSet(&BlinkTimer, 300);
				if(IsToShow)
				{
					DispIcon(ICON_BATHAF, TRUE);
				}
				else
				{
					DispIcon(ICON_BATHAF, FALSE);
				}
				IsToShow = !IsToShow;
			}
			//������ʾ0��������������ʾ����
			break;
			
		case PWR_MNT_DISP_SYS_OFF:
			//DBG(("BAT OFF\n"));
			ClearScreen();			//�����ʾ				
			DispString(" LO ");
			break;
			
		default:
			break;
	}
#endif	
}


//���LDOIN�ĵ�ѹֵ��ִ�ж�Ӧ����Ĵ���
//PowerOnInitFlag: TRUE--��һ���ϵ�ִ�е�Դ��ؼ��
static VOID PowerLdoinLevelMonitor(BOOL PowerOnInitFlag)
{	
	BOOL PowerOffFlag = FALSE;
		
	if(LdoinSampleCnt > 0)
	{
#ifdef AU6210K_NR_D_8_CSRBT
		LdoinSampleSum +=  (AdcReadOneChannel(ADC_CHANNEL_B6, ADC_VREF_AVDD33)
			+ AdcReadOneChannel(ADC_CHANNEL_B6, ADC_VREF_AVDD33) 
		 	+ AdcReadOneChannel(ADC_CHANNEL_B6, ADC_VREF_AVDD33) + 2) / 3;	
#else
		LdoinSampleSum += GetLdoinVoltage();			
#endif
		LdoinSampleCnt--;
	}

	//������LDOIN_SAMPLE_COUNT��������LDOINƽ��ֵ
	if(LdoinSampleCnt == 0)
	{
		LdoinLevelAverage = LdoinSampleSum / LDOIN_SAMPLE_COUNT;

		DBG(("LODin 5V Volt: %lu", (DWORD)LdoinLevelAverage));

		//Ϊ�������LDOIN����ʼ������
		LdoinSampleCnt = LDOIN_SAMPLE_COUNT;
		LdoinSampleSum = 0;

#ifdef	OPTION_CHARGER_DETECT
		if(IsInCharge())		//������Ѿ�����Ĵ���
		{		
			return;
		}
#endif
		
		if(LdoinLevelAverage > LDOIN_VOLTAGE_HIGH)	  
		{
			//������ʾ�������������PowerMonitorDisp�������ʾ����
			//PowerMonitorDisp(PWR_MNT_DISP_HIGH_V);
			PwrMntDisp = PWR_MNT_DISP_HIGH_V;
			//DBG(("bat full\n"));
		}
		/*
		else if(LdoinLevelAverage > LDOIN_VOLTAGE_MID)
		{
			//������ʾ2�����������PowerMonitorDisp�������ʾ����
			//PowerMonitorDisp(PWR_MNT_DISP_MID_V);
			PwrMntDisp = PWR_MNT_DISP_MID_V;
		}
		//*/
		else if(LdoinLevelAverage > LDOIN_VOLTAGE_LOW)
		{
			//������ʾ1�����������PowerMonitorDisp�������ʾ����
			//PowerMonitorDisp(PWR_MNT_DISP_LOW_V);
			PwrMntDisp = PWR_MNT_DISP_LOW_V;
			//DBG(("bat LOW\n"));
		}
		else if(LdoinLevelAverage > LDOIN_VOLTAGE_OFF)
		{
			//������ʾ0�����������PowerMonitorDisp�������ʾ����
			//PowerMonitorDisp(PWR_MNT_DISP_EMPTY_V);
			PwrMntDisp = PWR_MNT_DISP_EMPTY_V;
		}

		if((LdoinLevelAverage > LDOIN_VOLTAGE_OFF) && (LdoinLevelAverage < LDOIN_VOLTAGE_LOW))
			PwrMntDisp = PWR_MNT_DISP_EMPTY_V;
	
		#if defined(AU6210K_ZB_BT007_CSR)
		if(PowerOnInitFlag == TRUE) 
		{		
			if(LdoinLevelAverage <= LDOIN_VOLTAGE_OFF)
			{
				PowerOffFlag = TRUE;
			}			
		}
		
        #endif
		if((PowerOnInitFlag == FALSE) && (LdoinLevelAverage <= LDOIN_VOLTAGE_OFF))
		{
			PowerOffFlag = TRUE;
		}
		
		if(PowerOffFlag == TRUE)
		{
			//���ڹػ���ѹ������ػ�����
			//����������ʾ�͵�ѹ��ʾ��Ȼ��ִ�йػ�����
			PwrMntDisp = PWR_MNT_DISP_SYS_OFF;
			PowerMonitorDisp();			
			FeedWatchDog();
			//ֹͣ�����������̣���������ʾ����DAC���ع��ŵ�Դ�ȶ���
			DBG(("PowerMonitor, PD\n"));	
			SystemOff();
		}		
	}
	PowerMonitorDisp();
}


//���ܼ��ӳ�ʼ��
//ʵ��ϵͳ���������еĵ͵�ѹ��⴦���Լ���ʼ������豸������IO��
VOID PowerMonitorInit(VOID)
{
	TimeOutSet(&PowerMonitorTimer, 0);	
	TimeOutSet(&BlinkTimer, 0);	
	TimeOutSet(&LowPowerSongTimer, 0);	

#ifdef OPTION_CHARGER_DETECT
	//���ϵͳ����ʱ������豸�Ѿ����룬�Ͳ�ִ��������ε͵�ѹ���ʹ������
	if(!IsInCharge())
#endif	
	{
		//ϵͳ���������еĵ͵�ѹ���
		//����ʱ��ѹ��⣬���С�ڿ�����ѹ���������豸���Ͳ������̣�ֱ�ӹػ�
		//������Ϊʱ50ms�������ж�Ӧ�Ĵ���
		while(LdoinSampleCnt)
		{
			LdoinSampleCnt--;
#ifdef AU6210K_NR_D_8_CSRBT
			LdoinSampleSum +=  (AdcReadOneChannel(ADC_CHANNEL_B6, ADC_VREF_AVDD33)
				+ AdcReadOneChannel(ADC_CHANNEL_B6, ADC_VREF_AVDD33) 
			 	+ AdcReadOneChannel(ADC_CHANNEL_B6, ADC_VREF_AVDD33) + 2) / 3;	
#else
			LdoinSampleSum += GetLdoinVoltage();			
#endif			
			WaitMs(5);
		}		
		//Ϊ��ߵ�LDOIN����ʼ������
		PowerLdoinLevelMonitor(TRUE);
	}
}


//ϵͳ��Դ״̬��غʹ���
//ϵͳ���������LDOIN���ڿ�����ѹ������ϵͳ���������м��LDOIN
VOID PowerMonitor(VOID)
{
	if(IsTimeOut(&PowerMonitorTimer))
	{
		TimeOutSet(&PowerMonitorTimer, LDOIN_SAMPLE_PERIOD);

#ifdef OPTION_CHARGER_DETECT
		if(IsInCharge())		//������Ѿ�����Ĵ���
		{
			if(LdoinLevelAverage >= LDOIN_VOLTAGE_FULL) 
			{
				//PowerMonitorDisp(PWR_MNT_DISP_HIGH_V);		//��ʾ���״̬
				PwrMntDisp = PWR_MNT_DISP_HIGH_V;
				//DBG(("charger full\n"));
			}
			else
			{
				//PowerMonitorDisp(PWR_MNT_DISP_CHARGE);		//��ʾ���״̬
				PwrMntDisp = PWR_MNT_DISP_CHARGE;
				//DBG(("charger.....\n"));
			}
		}
#endif		

#if defined(AU6210K_MINI503) || defined(AU6210K_AT_BT809)  //�͵���ʾ��
		
				if(PwrMntDisp == PWR_MNT_DISP_EMPTY_V)
				{
					if(IsTimeOut(&LowPowerSongTimer))
					{
						TimeOutSet(&LowPowerSongTimer, LOW_POWER_SONG_TIME);
#ifdef FUNC_SPI_KEY_SOUND_EN						
						SPI_PlaySelectNum(SPIPLAY_SONG_LOW_PWR,0);
#endif
						WaitMs(100);
						UnMute();
						DBG1(("low power song!!"));
						
					}
				}
				else
				{
					TimeOutSet(&LowPowerSongTimer, 0);	
				}
#endif			

		//û�в�����LDOIN_SAMPLE_COUNT��������������
		PowerLdoinLevelMonitor(FALSE);
	}
}


#endif
