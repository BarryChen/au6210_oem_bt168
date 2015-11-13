#include <string.h>
#include "syscfg.h"
#include "utility.h"
#include "sysctrl.h"
#include "radio_ctrl.h"
#include "mv_fm.h"
#include "radio_app_interface.h"
#include "message.h"
#include "key.h"
#include "gpio.h"
#include "display.h"
#include "24cxx.h"
#include "breakpoint.h"
#include "user_interface.h"
#include "seg_panel.h"
#include "sys_on_off.h"
#include "rtc_ctrl.h"
#include "debug.h"
#include "otp_play.h"
#include "slave_ctrl.h"
#include "pt231x.h"

#ifdef FUNC_RADIO_EN

RADIO_CTRL	gRadioCtrl;
BYTE RadioDisFlag;
static BYTE RadioDisCount = 0;

#ifndef FUNC_BREAK_POINT_EN
RADIO_DATA	  gRadioData2Store;
#endif

#ifdef FUNC_RADIO_ESD_AUTO_RESUME
static TIMER RadioStateCheckTimer;
static BYTE RadioStateCheckCount;
WORD gRadioCurrFreqBack;
#endif

//����FM���ͺ�
RADIO_NAME 	Radio_Name = RADIO_NONE;
#ifdef	FUNC_NUMBER_KEY_EN
extern WORD	gRecvNum;
extern BOOL NumKeyRadioFreqErrorFlag;
extern BOOL NumKeyErrorDispFlag;
#endif

#ifdef AU6210K_BOOMBOX_DEMO
BOOL gRadioMonoEnableFlag = FALSE;	
#endif

#ifdef	FUNC_DEVICE_FORCEPAUSE_EN
extern BOOL gDevicePlayForcePauseFlag;
#endif


#ifdef __RADIOCTRL_DEBUG__
VOID RadioShowDebug(BYTE Index)
{
	BYTE i = 0;

	switch(Index)
	{
		case RadioShowPowerOn:
			DBG(("Read Data Form EEPROM\n"));
			break;	
					
		case RadioShowCancelSearch:
			DBG(("��̨��;�˳�\n"));	
			break;
	
		case RadioShowSearchEndOverflow:
			DBG(("�ѵ�̨��������MAX_RADIO_STATION_NUM\n"));
			break;
	
		case RadioShowSearchEndBufferEnough:
			DBG(("0<�ѵ�̨����<buffer����\n"));
			break;
	
		case RadioShowSearchUpDown:
			DBG(("SCANUP,SCANDOWN:find a channel\n"));
			break;
	
		case RadioShowSaveCurFreToCurChannle:
			DBG(("��ǰƵ�ʱ��浽��ǰͨ��\n"));
			break;
	
		case RadioShowSaveFre:
			DBG(("��ǰƵ�ʱ��浽ĳ��Ƶ��\n"));
			break;
	
		case RadioShowCurStationChange:
			DBG(("ͨ���л�\n"));
			break;
	
		case RadioShowFreChange:	
			DBG(("��ǰƵ�ʸı�\n"));
			break;
	}
	
	DBG(("Ƶ��760~874(start form 1):%2BX\n", gRadioData.Area1StationSum));	
	DBG(("Ƶ��875~1080(start form 1):%2BX\n", (gRadioData.StationSum - gRadioData.Area1StationSum)));				
	DBG(("��ǰƵ��(start form 0):%2BX\n", gRadioData.CurrStation));
	
	DBG(("Ƶ���б�(760~874):\n"));
	for(i = 0; i < gRadioData.Area1StationSum; i++)
	{
		DBG(("Ƶ��%2BX  %u\n", (i + 1), (gRadioData.Stations[i] + RADIO_FREQ_BASE_760)));
	}	

	DBG(("Ƶ���б�(875~1080):\n"));	
	for(i = gRadioData.Area1StationSum; i < gRadioData.StationSum; i++)
	{
		DBG(("Ƶ��%2BX  %u\n", (i + 1), (gRadioData.Stations[i] + RADIO_FREQ_BASE_875)));
	}	
}
#endif


//����FMǰ��ʼ������
//�����ڲ�FMģ��ʱ���ײ��������ͨ��(DAC_CH_ONCHIP_FM��DAC_CH_ONCHIP_FM_L��DAC_CH_ONCHIP_FM_R)�����е��ô˺���
VOID RadioCtrlInit(VOID)
{
	DBG((">>RadioCtrlInit()\n"));
#ifndef FUNC_SPI_KEY_SOUND_EN		
	RadioSetFreq();
	RadioMute(FALSE);
	RadioSetVolume(gSys.Volume);	
#endif
}


//�뿪FMǰ��ʼ������
//�����ڲ�FMģ��ʱ���ײ��������ͨ��(DAC_CH_ONCHIP_FM��DAC_CH_ONCHIP_FM_L��DAC_CH_ONCHIP_FM_R)�����е��ô˺���
VOID RadioCtrlDeinit(VOID)
{
	CancelSearch();
	MuteOn(TRUE, TRUE);
	RadioMute(TRUE);
	RadioPowerDown();
#ifdef FUNC_POWER_AMPLIFIER_EN
	ABPowerAmplifierOn();	//������ģʽ��ѡ��AB �๦��
#endif	
}


//����FM ���ʼ������
VOID RadioEnterInit(VOID)
{	
#ifdef FUNC_SPI_KEY_SOUND_EN
	RadioSetFreq();
	RadioMute(FALSE);
	RadioSetVolume(gSys.Volume);	
#endif

	gRadioCtrl.State = RADIO_IDLE;
	UnMute();	

#ifdef FUNC_POWER_AMPLIFIER_EN
	ABPowerAmplifierOff();	//��FMģʽ��ѡ��D �๦��
#endif
	
#ifdef FUNC_BREAK_POINT_EN
	BP_SaveInfo(&gBreakPointInfo.PowerMemory.SystemMode,sizeof(gBreakPointInfo.PowerMemory.SystemMode));
#endif

#ifdef FUNC_RADIO_ESD_AUTO_RESUME
	RadioStateCheckCount = 0;
	TimeOutSet(&RadioStateCheckTimer, 250);
#endif
}


//FMͨ�ù��ܣ��;����FM�ͺ�û�й�ϵ
//������̨ʱ����;ֹͣ��̨
VOID CancelSearch(VOID)
{
	if((gRadioCtrl.State > RADIO_IDLE) && (gRadioCtrl.SeekMode != SEEK_NONE))
	{
		RadioDisFlag = RadioDisCurFreNum;
		
		if(gRadioData.StationSum > 0)
		{
			gRadioData.CurrStation = 0;
			if(gRadioData.Area1StationSum > 0)
			{
				gRadioData.CurrFreq = gRadioData.Stations[gRadioData.CurrStation] + RADIO_FREQ_BASE_760;
			}
			else
			{
				gRadioData.CurrFreq = gRadioData.Stations[gRadioData.CurrStation] + RADIO_FREQ_BASE_875;
			}
			RadioSetFreq();	
			RadioShowDebug(RadioShowCancelSearch);
			RadioWriteData();			
		}
		else
		{
			DBG(("Error:empty\n"));
		}
		RadioMute(FALSE);		
		UnMute();
		gRadioCtrl.State = RADIO_IDLE;
#ifdef FUNC_SINGLE_LED_EN  
		DispDev();
#endif
	}
}


VOID RadioArrayInsert(VOID)
{
#ifdef	FUNC_NUMBER_KEY_EN
	BYTE i;

	if(gRecvNum > MAX_RADIO_STATION_NUM)
	{
		return;
	}
	
	if(gRadioData.CurrFreq >= RADIO_FREQ_BASE_875)
	{
		if(gRecvNum > gRadioData.StationSum)//׷�ӵ�����
		{		
			gRadioData.CurrStation = gRadioData.StationSum; 						
			gRadioData.StationSum++;						
		}
		else//�滻
		{
			gRadioData.CurrStation = gRecvNum - 1;
		}
		gRadioData.Stations[gRadioData.CurrStation] = (gRadioData.CurrFreq - RADIO_FREQ_BASE_875);
	}
	else //760~874
	{
		if(gRecvNum > gRadioData.Area1StationSum)//׷��
		{
			for(i = gRadioData.StationSum; i > gRadioData.Area1StationSum; i--)
			{
				gRadioData.Stations[i] = gRadioData.Stations[i-1];
			}
							
			gRadioData.CurrStation = gRadioData.Area1StationSum;
			gRadioData.Area1StationSum++;
			gRadioData.StationSum++;
		}
		else//�滻
		{
			gRadioData.CurrStation = gRecvNum - 1;			
		}
		gRadioData.Stations[gRadioData.CurrStation] = (gRadioData.CurrFreq - RADIO_FREQ_BASE_760);
	}
#endif
}


static VOID RadioChannelChange(VOID)
{
	if((gRadioData.CurrStation + 1) <= gRadioData.Area1StationSum)
	{
		gRadioData.CurrFreq = gRadioData.Stations[gRadioData.CurrStation] + RADIO_FREQ_BASE_760;
		DBG(("CH 760+:%u\n", (gRadioData.Stations[gRadioData.CurrStation] + RADIO_FREQ_BASE_760)));
	}
	else
	{
		gRadioData.CurrFreq = gRadioData.Stations[gRadioData.CurrStation] + RADIO_FREQ_BASE_875;
		DBG(("CH 875+:%u\n", (gRadioData.Stations[gRadioData.CurrStation] + RADIO_FREQ_BASE_875)));
	}

	RadioSetFreq();
	RadioMute(FALSE);				
	RadioWriteData();
	RadioShowDebug(RadioShowCurStationChange);
}

	
//��ȡ��Ч��Ԥ��̨��
#ifdef AU6210K_BOOMBOX_DEMO
static BOOL RadioGetValidPresetNum(BYTE CurrStationNum, BYTE Direction)
{
	BYTE Temp;

	if(Direction == UP)
	{
		for(Temp = CurrStationNum + 1; Temp < (MAX_RADIO_STATION_NUM / 2); Temp++)
		{
			if(gRadioData.Stations[Temp + (MAX_RADIO_STATION_NUM / 2)])
			{
				gRadioData.CurrStation = Temp;
				return TRUE;
			}
		}
		for(Temp = 0; Temp < CurrStationNum; Temp++)
		{
			if(gRadioData.Stations[Temp + (MAX_RADIO_STATION_NUM / 2)])
			{
				gRadioData.CurrStation = Temp;
				return TRUE;
			}
		}	
	}
	else 
	{
		for(Temp = CurrStationNum; Temp > 0; Temp--)
		{
			if(gRadioData.Stations[Temp + (MAX_RADIO_STATION_NUM / 2)] && (Temp != CurrStationNum))
			{
				gRadioData.CurrStation = Temp;
				return TRUE;
			}
		}
		for(Temp = ((MAX_RADIO_STATION_NUM / 2) - 1); Temp > CurrStationNum; Temp--)
		{
			if(gRadioData.Stations[Temp + (MAX_RADIO_STATION_NUM / 2)])
			{
				gRadioData.CurrStation = Temp;
				return TRUE;
			}
		}	
	}
	
	if(gRadioData.StationSum == 1)	//ֻ��һ��Ԥ��̨
	{
		return TRUE;
	}
	
	return FALSE;	//û��Ԥ��̨����
}
#endif

	
//������Ӧ
VOID RadioEventProcess(VOID)
{
	MESSAGE Event = MessageGet(MSG_FIFO_KEY);
	
	switch(Event)
	{
		case MSG_POWER:
		case MSG_MODE_SW:
			//DBG(("Exit FM, Event:%bx\n", Event));
			CancelSearch();
			MessageSend(MSG_FIFO_DEV_CTRL, Event);
			break;
			
		case MSG_SCAN://�����Զ�ȫƵ����̨
		case MSG_INTRO:
		case MSG_PLAY_1CP:
#if !defined(AU6210K_HXX_B002)			
		case MSG_PLAY_PAUSE:
#endif			
#ifdef FUNC_DEVICE_FORCEPAUSE_EN
			if(gDevicePlayForcePauseFlag == TRUE)
			{
				gDevicePlayForcePauseFlag = FALSE;
				UnMute();
				break;
			}
#endif
			DBG(("SCAN\n"));
#ifdef FUNC_BEEP_SOUND_EN	
			PushKeyBeep(1);
#endif
			if(gRadioCtrl.State == RADIO_IDLE)
			{
				gRadioCtrl.State = RADIO_READ;
				
				gRadioCtrl.SeekMode = SEEK_AUTOSCAN;
#ifdef FUNC_SLAVE_UART_EN
                            gRadioData.CurrFreq = gRadioCtrl.RadioLowerBound;
#else
				gRadioData.CurrFreq = RADIO_LOWER_BOUND;
#endif
				gRadioData.CurrStation = 0;	
				gRadioData.Area1StationSum = 0;
				gRadioData.StationSum = 0;					
				memset(&gRadioData.Stations, 0, sizeof(gRadioData.Stations));
				
				MuteOn(TRUE, TRUE);				
				RadioSetFreq();		
#ifdef FUNC_SLAVE_UART_EN
#ifdef	FUNC_RADIO_AUTOSEEK_EN
				MVFM_AutoSeekConfig(gRadioData.CurrFreq, gRadioCtrl.RadioUpperBound, gRadioCtrl.RadioSeekStep, 1, 1);
#else
				MVFM_AutoSeekConfig(gRadioData.CurrFreq, gRadioCtrl.RadioUpperBound, gRadioCtrl.RadioSeekStep, 0, 1);
#endif

#else
#ifdef	FUNC_RADIO_AUTOSEEK_EN
				MVFM_AutoSeekConfig(gRadioData.CurrFreq, RADIO_UPPER_BOUND, RADIO_SEEK_STEP, 1, 1);
#else
				MVFM_AutoSeekConfig(gRadioData.CurrFreq, RADIO_UPPER_BOUND, RADIO_SEEK_STEP, 0, 1);
#endif
#endif				
				RadioMute(TRUE);
				TimeOutSet(&gRadioCtrl.SeekPreviewTimer, RadioSeekTimeValue);
			}
			else
			{
				CancelSearch();
			}
			break;

		case MSG_SAVE_STATION:  //���浱ǰ��Ƶ�ʵ���ǰ��Ƶ��
			if(gRadioCtrl.State == RADIO_IDLE)
			{		
				if(gRadioData.StationSum < MAX_RADIO_STATION_NUM)
				{		
					if((gRadioData.CurrFreq < RADIO_FREQ_BASE_875) && ((gRadioData.CurrStation + 1) <= gRadioData.Area1StationSum))
					{
						gRadioData.Stations[gRadioData.CurrStation] = (gRadioData.CurrFreq - RADIO_FREQ_BASE_760);
					}
					else if((gRadioData.CurrFreq > RADIO_FREQ_BASE_875) && ((gRadioData.CurrStation + 1) > gRadioData.Area1StationSum))
					{
						gRadioData.Stations[gRadioData.CurrStation] = (gRadioData.CurrFreq - RADIO_FREQ_BASE_875);
					}				
					RadioShowDebug(RadioShowSaveCurFreToCurChannle);
					RadioWriteData();
					RadioDisFlag = RadioDisCurFreBlink;
					RadioDisCount = 6;
					TimeOutSet(&gRadioCtrl.SeekPreviewTimer,RadioBlinkTime);
#ifdef FUNC_SLAVE_UART_EN
					SlaveRadioRetCurrentStatus();
#endif

				}
			}
			break;
		
		case MSG_NEXT://��һ��̨
		case MSG_PLAY_1:
#ifdef	FUNC_DEVICE_FORCEPAUSE_EN
			if(gDevicePlayForcePauseFlag == TRUE) 
			{
				break;
			}					
#endif

#ifdef AU6210K_BOOMBOX_DEMO
			if(gRadioCtrl.State == RADIO_PROG)
			{
				if(gRadioData.CurrStation < (MAX_RADIO_STATION_NUM / 2))
				{
					gRadioData.CurrStation++;
				}
				else
				{
					gRadioData.CurrStation = 0;
				}
				break;
			}
			
			if(gRadioCtrl.State == RADIO_IDLE)
			{
				if(RadioGetValidPresetNum(gRadioData.CurrStation, UP) == TRUE)
				{
					RadioDisFlag = RadioDisCurChChange;
					TimeOutSet(&gRadioCtrl.SeekPreviewTimer, RadioPreVimeTimerValue);
					RadioChannelChange();
					UnMute();
					gSys.MuteFg = FALSE;
				}
				break;
			}
#endif

			if((gRadioData.StationSum > 0) && (gRadioCtrl.State == RADIO_IDLE))
			{
#ifdef FUNC_BEEP_SOUND_EN	
				PushKeyBeep(1);
#endif
				if(gRadioData.CurrStation < (gRadioData.StationSum - 1))
				{
					gRadioData.CurrStation++;
				}
				else
				{
					gRadioData.CurrStation = 0;
				}
				RadioDisFlag = RadioDisCurChChange;				
				RadioChannelChange();
				UnMute();
				TimeOutSet(&gRadioCtrl.SeekPreviewTimer, RadioPreVimeTimerValue);
				gSys.MuteFg = FALSE;
#ifdef FUNC_SLAVE_UART_EN
				SlaveRadioRetCurrentStatus();
#endif
			}
			break;
			
		case MSG_PRE://��һ��̨
		case MSG_EQ_CH_SUB:
#ifdef	FUNC_DEVICE_FORCEPAUSE_EN
			if(gDevicePlayForcePauseFlag == TRUE) 
			{
				break;
			}					
#endif

#ifdef AU6210K_BOOMBOX_DEMO
			if(gRadioCtrl.State == RADIO_PROG)
			{
				if(gRadioData.CurrStation < (MAX_RADIO_STATION_NUM / 2))
				{
					gRadioData.CurrStation++;
				}
				else
				{
					gRadioData.CurrStation = 0;
				}
				break;
			}
			
			if(gRadioCtrl.State == RADIO_IDLE)
			{
				if(RadioGetValidPresetNum(gRadioData.CurrStation, DOWN) == TRUE)
				{
					RadioDisFlag = RadioDisCurChChange;
					TimeOutSet(&gRadioCtrl.SeekPreviewTimer, RadioPreVimeTimerValue);
					RadioChannelChange();
					UnMute();
					gSys.MuteFg = FALSE;
					break;
				}
			}
#endif

			if((gRadioData.StationSum >  0) && (gRadioCtrl.State == RADIO_IDLE)) 
			{
#ifdef FUNC_BEEP_SOUND_EN	
				PushKeyBeep(1);
#endif
				if(gRadioData.CurrStation > 0)
				{
					gRadioData.CurrStation--;
				}
				else
				{
					gRadioData.CurrStation = gRadioData.StationSum - 1;
				}
				RadioDisFlag = RadioDisCurChChange;				
				RadioChannelChange();
				UnMute();
				TimeOutSet(&gRadioCtrl.SeekPreviewTimer, RadioPreVimeTimerValue);
				gSys.MuteFg = FALSE;
#ifdef FUNC_SLAVE_UART_EN
				SlaveRadioRetCurrentStatus();
#endif
			}
			break;
			
#ifdef	FUNC_NUMBER_KEY_EN
		case MSG_RECV_NUM://ѡ̨(������ֵ����Ч̨����)������Ƶ��(������ֵ����ЧƵ�ʷ�Χ��)	
			if(gRadioCtrl.State == RADIO_IDLE)//SeanFu
			{
				if((gRecvNum > 0) && (gRecvNum <= gRadioData.StationSum))
				{
					//DBG(("RecvNum�Ϸ�̨�ţ�%d\n", gRecvNum));				
					gRadioData.CurrStation = gRecvNum - 1;
					if(gRecvNum <= gRadioData.Area1StationSum)
					{
						gRadioData.CurrFreq = (gRadioData.Stations[gRadioData.CurrStation] + RADIO_FREQ_BASE_760);
					}
					else
					{
						gRadioData.CurrFreq = (gRadioData.Stations[gRadioData.CurrStation] + RADIO_FREQ_BASE_875);
					}	
					RadioWriteData();
					RadioShowDebug(RadioShowCurStationChange);
				}
#ifdef FUNC_SLAVE_UART_EN
                            else if((gRecvNum >= gRadioCtrl.RadioLowerBound) && (gRecvNum <= gRadioCtrl.RadioUpperBound))
#else
				else if((gRecvNum >= RADIO_LOWER_BOUND) && (gRecvNum <= RADIO_UPPER_BOUND))
#endif
				{
					//DBG(("RecvNum�Ϸ� Ƶ�ʣ�%d\n", gRecvNum));
					gRadioData.CurrFreq = gRecvNum;				
					RadioWriteData();
					RadioShowDebug(RadioShowSaveFre);
				}
				else 
				{
					//DBG(("RecvNum�Ƿ���%d\n", gRecvNum));	
					gRecvNum = 0;						
					NumKeyRadioFreqErrorFlag = TRUE;
					NumKeyErrorDispFlag = TRUE;					
#ifdef FUNC_DISP_EN						
					TimeOutSet(&DispTmr, NORMAL_INTERVAL);	//�������Err��ʾ����ʱ��
#endif
					return;
				}			
				RadioSetFreq();
				RadioMute(FALSE);
				gRecvNum = 0;					
#ifdef FUNC_DISP_EN						
				TimeOutSet(&DispTmr, 0);	//�˳���ֵ��ʾ
#endif
			}
			break;

		case MSG_NUM_SAVE_STAITON:	//����ǰƵ�ʱ�������
			if(gRadioCtrl.State == RADIO_IDLE)
			{				
				//DBG(("RecvNum %d \n", gRecvNum));
				RadioArrayInsert();
				gRecvNum = 0;
				RadioWriteData();
				RadioShowDebug(RadioShowSaveFre);
				RadioDisFlag = RadioDisCurFreBlink;
				RadioDisCount = 4;
				TimeOutSet(&gRadioCtrl.SeekPreviewTimer, RadioBlinkTime);
			}
			break;
#endif
			
#ifdef	FUNC_RADIO_STEPOVER_EN		
		case MSG_NEXT1:
		case MSG_FREQ_UP:
		case MSG_FREQUP_10TRK://ѭ������΢��Ƶ��(+100KHZ)	
			if(gRadioCtrl.State == RADIO_IDLE)
			{
#ifdef FUNC_SLAVE_UART_EN
                            if(++gRadioData.CurrFreq > gRadioCtrl.RadioUpperBound)
				{
					gRadioData.CurrFreq = gRadioCtrl.RadioLowerBound;
				}
#else
				if(++gRadioData.CurrFreq > RADIO_UPPER_BOUND)
				{
					gRadioData.CurrFreq = RADIO_LOWER_BOUND;
				}
#endif
				RadioSetFreq();
				RadioMute(FALSE);		
				RadioWriteData();
				RadioShowDebug(RadioShowFreChange);
			}
			break;
		
		case MSG_PREV1:
		case MSG_FREQ_DN:
		case MSG_FREQDN_10TRK://ѭ���ݼ�΢��Ƶ��(-100KHZ)		
			if(gRadioCtrl.State == RADIO_IDLE)
			{
#ifdef FUNC_SLAVE_UART_EN
                            if(--gRadioData.CurrFreq < gRadioCtrl.RadioLowerBound)
				{
					gRadioData.CurrFreq = gRadioCtrl.RadioUpperBound;
				}	
#else
				if(--gRadioData.CurrFreq < RADIO_LOWER_BOUND)
				{
					gRadioData.CurrFreq = RADIO_UPPER_BOUND;
				}	
#endif
				RadioSetFreq();
				RadioMute(FALSE);		
				RadioWriteData();
				RadioShowDebug(RadioShowFreChange);
			}
			break;
#endif
		
#ifdef	FUNC_RADIO_SEMO_AUTOSEEK_EN		
		case MSG_FF_START:
		case MSG_10TRACK_ADD://������ʽ��̨���ѵ���һ����ֹͣ
			if(gRadioCtrl.State == RADIO_IDLE)
			{
				gRadioCtrl.SeekMode = SEEK_SCANUP;
#ifdef FUNC_SLAVE_UART_EN
                            if(gRadioData.CurrFreq >= gRadioCtrl.RadioUpperBound)
				{	
					gRadioData.CurrFreq = gRadioCtrl.RadioLowerBound;
				}
				else
				{
					gRadioData.CurrFreq++;
				}
#else
				if(gRadioData.CurrFreq >= RADIO_UPPER_BOUND)
				{	
					gRadioData.CurrFreq = RADIO_LOWER_BOUND;
				}
				else
				{
					gRadioData.CurrFreq++;
				}
#endif				
				MuteOn(TRUE, TRUE);	
				RadioSetFreq();						
				gRadioCtrl.State = RADIO_READ;
#ifdef FUNC_SLAVE_UART_EN
#ifdef	FUNC_RADIO_AUTOSEEK_EN
				MVFM_AutoSeekConfig(gRadioData.CurrFreq, gRadioCtrl.RadioUpperBound, gRadioCtrl.RadioSeekStep, 1, 1);
#else
				MVFM_AutoSeekConfig(gRadioData.CurrFreq, gRadioCtrl.RadioUpperBound, gRadioCtrl.RadioSeekStep, 0, 1);
#endif
#else
#ifdef	FUNC_RADIO_AUTOSEEK_EN
				MVFM_AutoSeekConfig(gRadioData.CurrFreq, RADIO_UPPER_BOUND, RADIO_SEEK_STEP, 1, 1);
#else
				MVFM_AutoSeekConfig(gRadioData.CurrFreq, RADIO_UPPER_BOUND, RADIO_SEEK_STEP, 0, 1);
#endif
#endif
				RadioMute(TRUE);
				TimeOutSet(&gRadioCtrl.SeekPreviewTimer, RadioSeekTimeValue);
					
			}
			break;
		
		case MSG_FB_START:
		case MSG_10TRACK_SUB://�ݼ���ʽ��̨���ѵ���һ����ֹͣ
			if(gRadioCtrl.State == RADIO_IDLE)
			{
				gRadioCtrl.SeekMode = SEEK_SCANDOWN;
#ifdef FUNC_SLAVE_UART_EN
                            if(gRadioData.CurrFreq <= gRadioCtrl.RadioLowerBound)
				{
					gRadioData.CurrFreq = gRadioCtrl.RadioUpperBound;
				}
				else
				{
					gRadioData.CurrFreq--;
				}
#else				
				if(gRadioData.CurrFreq <= RADIO_LOWER_BOUND)
				{
					gRadioData.CurrFreq = RADIO_UPPER_BOUND;
				}
				else
				{
					gRadioData.CurrFreq--;
				}
#endif				
				MuteOn(TRUE, TRUE);	
				RadioSetFreq();						
				gRadioCtrl.State = RADIO_READ;
#ifdef FUNC_SLAVE_UART_EN
#ifdef	FUNC_RADIO_AUTOSEEK_EN
				MVFM_AutoSeekConfig(gRadioData.CurrFreq, gRadioCtrl.RadioLowerBound, gRadioCtrl.RadioSeekStep, 1, 1);
#else
				MVFM_AutoSeekConfig(gRadioData.CurrFreq, gRadioCtrl.RadioLowerBound, gRadioCtrl.RadioSeekStep, 0, 1);
#endif
#else
#ifdef	FUNC_RADIO_AUTOSEEK_EN
				MVFM_AutoSeekConfig(gRadioData.CurrFreq, RADIO_LOWER_BOUND, RADIO_SEEK_STEP, 1, 1);
#else
				MVFM_AutoSeekConfig(gRadioData.CurrFreq, RADIO_LOWER_BOUND, RADIO_SEEK_STEP, 0, 1);
#endif
#endif
				RadioMute(TRUE);
				TimeOutSet(&gRadioCtrl.SeekPreviewTimer, RadioSeekTimeValue);
					
			}
			break;
#endif

		case MSG_VOL_ADD://������					
#if defined(AU6210K_NR_D_9X)||defined(AU6210K_NR_D_9X_XJ_HTS)
			if (gSys.Volume >= VOLUME_MAX)
			{
				SPI_PlaySelectNum(SPIPLAY_SONG_MAX_VOLUME, 0);
				InDacChannelSel(DAC_CH_ONCHIP_FM);
				RadioSetFreq();
				UnMute();
				SysClkDivSet(CLK_DIV_RATE);
			}
#endif

#ifdef FUNC_POWER_AMPLIFIER_EN
			ABPowerAmplifierOff();			
#endif

#ifdef FUNC_OTP_KEY_SOUND_EN
			OTP_PlaySelNum(OTPPLAY_NUM_VOLUP, 0);
			InDacChannelSel(DAC_CH_ONCHIP_FM);
			RadioSetFreq();
			UnMute();
			SysClkDivSet(CLK_DIV_RATE);
#endif
#ifdef FUNC_PT231X_EN
            VolType = VOL_MAIN;
            PT2313VolAdd();
#else
			VolumeAdjust(UP);
#endif
			break;
		
		case MSG_VOL_SUB://������			
#if defined(AU6210K_NR_D_9X)||defined(AU6210K_NR_D_9X_XJ_HTS)
			if (gSys.Volume <= VOLUME_MIN)
			{
				SPI_PlaySelectNum(SPIPLAY_SONG_MIN_VOLUME, 0);
				InDacChannelSel(DAC_CH_ONCHIP_FM);
				RadioSetFreq();
				SysClkDivSet(CLK_DIV_RATE);
			}
#endif

#ifdef FUNC_POWER_AMPLIFIER_EN
			ABPowerAmplifierOff();			
#endif

#ifdef FUNC_OTP_KEY_SOUND_EN
			OTP_PlaySelNum(OTPPLAY_NUM_VOLDN, 0);
			InDacChannelSel(DAC_CH_ONCHIP_FM);
			RadioSetFreq();
			UnMute();
			SysClkDivSet(CLK_DIV_RATE);
#endif
#ifdef FUNC_PT231X_EN
                    VolType = VOL_MAIN;
                    PT2313VolSub();
#else
			VolumeAdjust(DOWN);
#endif
			break;
#ifdef FUNC_PT231X_EN
              case MSG_TREBUP:
			VolType = VOL_TREB;
                     PT2313VolAdd();  	
			break;  	
		case MSG_TREBDN:
			VolType = VOL_TREB;
                     PT2313VolSub();  	
			break;  	
		case MSG_BASSUP:
			VolType = VOL_BASS;
                     PT2313VolAdd();  	
			break;  	
		case MSG_BASSDN:
			VolType = VOL_BASS;
                     PT2313VolSub();  	
			break;  	
		case MSG_DEFAULT:
			PT2313TrebBassSetDefault();
			break;  	
#endif
#if defined(AU6210K_HXX_B002)
		case MSG_PLAY_PAUSE:
#endif
		case MSG_MUTE:
			if(gRadioCtrl.State == RADIO_IDLE)
			{			
#if 0//def FUNC_SPI_KEY_SOUND_EN
				if(gSys.MuteFg)
				{
					SPI_PlaySelectNum(SPIPLAY_SONG_UNMUTE, 0);
				}
				else
				{	
					SPI_PlaySelectNum(SPIPLAY_SONG_MUTE, 0);
				}				
				InDacChannelSel(DAC_CH_ONCHIP_FM);
				if(gSys.MuteFg)
				{
					RadioSetFreq();
					gSys.MuteFg = TRUE;
				}
				SysClkDivSet(CLK_DIV_RATE);
#endif
				MuteStatusChange();			
			}
			break;	
		
#if (defined(AU6210K_BOOMBOX_DEMO))
		case MSG_CLOCK:			
			if(gRadioCtrl.State == RADIO_IDLE)
			{
				gRadioCtrl.State = RADIO_PROG;
				RadioDisFlag = RadioDisCurProgramNum;
				TimeOutSet(&gRadioCtrl.SeekPreviewTimer, RadioProgramWaitTime);
				break;
			}	

			if(gRadioCtrl.State == RADIO_PROG)
			{		
				if(gRadioData.CurrFreq < RADIO_FREQ_BASE_875) 
				{
					gRadioData.Stations[gRadioData.CurrStation] = (gRadioData.CurrFreq - RADIO_FREQ_BASE_760);
				}
				else if(gRadioData.CurrFreq > RADIO_FREQ_BASE_875)
				{
					gRadioData.Stations[gRadioData.CurrStation] = (gRadioData.CurrFreq - RADIO_FREQ_BASE_875);
				}				
				gRadioData.Stations[gRadioData.CurrStation + (MAX_RADIO_STATION_NUM / 2)] = gRadioData.CurrStation + 1;
				gRadioData.StationSum = (MAX_RADIO_STATION_NUM / 2);		
					
				RadioShowDebug(RadioShowSaveCurFreToCurChannle);
#ifdef FUNC_BREAK_POINT_EN
				RadioWriteData();
#endif
				RadioDisFlag = RadioDisCurFreNum;
				gRadioCtrl.State = RADIO_IDLE;
			}
			break;

		case MSG_FM_STERO_SW:
			if(gRadioCtrl.State == RADIO_IDLE)
			{
				gRadioMonoEnableFlag ^= 1;
				RadioMonoModeSet(gRadioMonoEnableFlag);			
			}
			break;
#endif

		default:	
			break;
	}//switch(Event)

#ifdef FUN_SYSTEM_POWEROFF_WAIT_TIME
	SystemOffTimeMsgPro(Event);
#endif
}


#ifdef FUNC_DISP_EN	
#ifdef	FUNC_NUMBER_KEY_EN
//��������Ƶ�㱣����˸��ʾ
VOID RadioDisBlink(VOID)
{
	switch(gDisplayMode)
	{
#ifdef FUNC_SINGLE_LED_EN	
		case DISP_DEV_SLED:
			//add code
			break;
#endif

#ifdef FUNC_SEG_LED_EN
		case DISP_DEV_LED1888:		
		case DISP_DEV_LED8888:		
		case DISP_DEV_LED57:
			if(RadioDisCount % 2)
			{	
				DispString("    ");		
				DispIcon(ICON_DP_FM_MHZ, LIGHT_OFF);					
			}
			else
			{
				Num2Char(gRadioData.CurrFreq, FALSE);
				DispIcon(ICON_DP_FM_MHZ, LIGHT_ON);	
			}				
			break;
#endif

#ifdef FUNC_SEG_LCD_EN
		case DISP_DEV_LCD58:
			//add code
			break;
#endif
		
		default:
			break;			
	}
}
#endif
#endif


// Seek tuning will search up or down for a valid channel.
VOID RadioStationSearch(VOID)
{
	BYTE SearchState; 
		
	switch(gRadioCtrl.State)
	{	
		case RADIO_SEEK:	
			if(IsTimeOut(&gRadioCtrl.SeekPreviewTimer))
			{	
				MuteOn(TRUE, TRUE);	//���FM�ź�Դ��DACֱ�����ʱ���Զ���̨�����л��нϴ������
				RadioMute(TRUE);
				RadioDisFlag = RadioDisCurFreNum;

#ifdef	FUNC_RADIO_AUTOSEEK_EN
				if((gRadioCtrl.SeekMode == SEEK_AUTOSCAN) ||(gRadioCtrl.SeekMode == SEEK_SCANUP))
				{
#ifdef FUNC_SLAVE_UART_EN
                                   MVFM_AutoSeekConfig(gRadioData.CurrFreq, gRadioCtrl.RadioUpperBound, gRadioCtrl.RadioSeekStep, 1, 0);
#else
					MVFM_AutoSeekConfig(gRadioData.CurrFreq, RADIO_UPPER_BOUND, RADIO_SEEK_STEP, 1, 0);
#endif
				}
				else
				{
#ifdef FUNC_SLAVE_UART_EN
                                   MVFM_AutoSeekConfig(gRadioData.CurrFreq, gRadioCtrl.RadioLowerBound, gRadioCtrl.RadioSeekStep, 1, 0);
#else				
					MVFM_AutoSeekConfig(gRadioData.CurrFreq, RADIO_LOWER_BOUND, RADIO_SEEK_STEP, 1, 0);
#endif
				}
				DBG(("Auto Seek Start Freq %d\n", gRadioData.CurrFreq));	
#else
				if((gRadioCtrl.SeekMode == SEEK_AUTOSCAN) ||(gRadioCtrl.SeekMode == SEEK_SCANUP))
				{
					gRadioData.CurrFreq++;
				}
				else
				{
					gRadioData.CurrFreq--;
				}
				//DBG(("Search  Freq %d\n",gRadioData.CurrFreq));					
				//RadioSearchSet(gRadioData.CurrFreq);
#ifdef FUNC_SLAVE_UART_EN
                            MVFM_AutoSeekConfig(gRadioData.CurrFreq, gRadioCtrl.RadioUpperBound, gRadioCtrl.RadioSeekStep, 0, 0);
                            gRadioCtrl.RadioSeekResult =0;
#else								
				MVFM_AutoSeekConfig(gRadioData.CurrFreq, RADIO_UPPER_BOUND, RADIO_SEEK_STEP, 0, 0);
#endif
#endif
				gRadioCtrl.State = RADIO_READ;
				TimeOutSet(&gRadioCtrl.SeekPreviewTimer, RadioSeekTimeValue);
				
#ifdef FUNC_RADIO_ESD_AUTO_RESUME
				gRadioCurrFreqBack = gRadioData.CurrFreq;
#endif
#ifdef FUNC_SLAVE_UART_EN
				SlaveRadioRetCurrentStatus();
#endif
			}
			break;
						
		case RADIO_READ:
			if(IsTimeOut(&gRadioCtrl.SeekPreviewTimer))
			{
#ifdef	FUNC_RADIO_AUTOSEEK_EN
				if(SearchState = RadioRSSIRead(TRUE)) 
#else
				if(SearchState = RadioRSSIRead(FALSE)) 
#endif
				{	
#ifdef FUNC_SLAVE_UART_EN
                                   gRadioCtrl.RadioSeekResult =1;
#endif
#ifdef	FUNC_RADIO_AUTOSEEK_EN
#ifdef FUNC_DISP_EN
					DispResume();
#endif
#endif
					if(SearchState == 3) 
					{
						//DBG(("Search Freq %d\n",gRadioData.CurrFreq));						
						TimeOutSet(&gRadioCtrl.SeekPreviewTimer, RadioSeekTimeValue);
						break;
					}
					
					if(gRadioCtrl.SeekMode == SEEK_AUTOSCAN)
					{
						if(gRadioData.StationSum < MAX_RADIO_STATION_NUM)
						{
							if(gRadioData.CurrFreq < RADIO_FREQ_BASE_875)
							{
								gRadioData.Stations[gRadioData.StationSum] = (gRadioData.CurrFreq - RADIO_FREQ_BASE_760);
								gRadioData.Area1StationSum++;
							}
							else
							{
								//DBG(("~~~~~~~~~~~~~%u\n",gRadioData.CurrFreq));							
								gRadioData.Stations[gRadioData.StationSum] = (gRadioData.CurrFreq - RADIO_FREQ_BASE_875);
							}						
							gRadioData.StationSum++;

//#if (defined(FUNC_SINGLE_LED_EN))	//����LED��ʾ��Ҫ��̨Ԥ������
#ifdef RADIO_SEEK_WAIT_EN
							RadioMute(FALSE);
							UnMute();
#endif
//#endif
							RadioDisFlag = RadioDisCurChAutoscan;
							TimeOutSet(&gRadioCtrl.SeekPreviewTimer, RadioPreVimeTimerValue);
#ifdef FUNC_SINGLE_LED_EN  
							DispDev();
#endif
#ifdef FUNC_SLAVE_UART_EN
							SlaveRadioRetCurrentStatus();
#endif
						}
						else
						{					
							//DBG(("FM Buffer Over\n"));
							gRadioData.CurrStation = 0;
							if(gRadioData.Area1StationSum > 0)
							{
								gRadioData.CurrFreq = gRadioData.Stations[gRadioData.CurrStation] + RADIO_FREQ_BASE_760;
							}
							else
							{
								gRadioData.CurrFreq = gRadioData.Stations[gRadioData.CurrStation] + RADIO_FREQ_BASE_875;
							}
							RadioMute(FALSE);
							UnMute();
							gRadioCtrl.State = RADIO_IDLE;																			
							RadioWriteData();							
							RadioShowDebug(RadioShowSearchEndOverflow);
#ifdef FUNC_SINGLE_LED_EN 
							DispDev();
#endif
#ifdef FUNC_SLAVE_UART_EN
							SlaveRadioRetCurrentStatus();
#endif
							return;								
						}
					}
					else
					{
						gRadioCtrl.State = RADIO_IDLE;						
						RadioMute(FALSE);
						UnMute();
						RadioWriteData();						
						RadioShowDebug(RadioShowSearchUpDown);						
#ifdef FUNC_SINGLE_LED_EN 
						DispDev();
#endif
#ifdef FUNC_SLAVE_UART_EN
						SlaveRadioRetCurrentStatus();
#endif
						return;
					}
				}
#ifdef FUNC_SLAVE_UART_EN
                            if(((gRadioCtrl.SeekMode == SEEK_SCANDOWN) && (gRadioData.CurrFreq <= gRadioCtrl.RadioLowerBound))
				|| (((gRadioCtrl.SeekMode == SEEK_SCANUP) || (gRadioCtrl.SeekMode == SEEK_AUTOSCAN)) && (gRadioData.CurrFreq >= gRadioCtrl.RadioUpperBound)))
#else
				if(((gRadioCtrl.SeekMode == SEEK_SCANDOWN) && (gRadioData.CurrFreq <= RADIO_LOWER_BOUND))
				|| (((gRadioCtrl.SeekMode == SEEK_SCANUP) || (gRadioCtrl.SeekMode == SEEK_AUTOSCAN)) && (gRadioData.CurrFreq >= RADIO_UPPER_BOUND)))
#endif
				{ 
					gRadioCtrl.State = RADIO_IDLE;
					if((gRadioData.StationSum > 0) && (gRadioCtrl.SeekMode == SEEK_AUTOSCAN))
					{						
#ifdef	FUNC_RADIO_AUTOSEEK_EN
#ifdef FUNC_DISP_EN
						DispResume();
#endif
#endif
						gRadioData.CurrStation = 0;

						if(gRadioData.Area1StationSum > 0)
						{
							gRadioData.CurrFreq = gRadioData.Stations[gRadioData.CurrStation] + RADIO_FREQ_BASE_760;
						}
						else
						{
							gRadioData.CurrFreq = gRadioData.Stations[gRadioData.CurrStation] + RADIO_FREQ_BASE_875;
						}
						
						RadioSetFreq();							
						RadioWriteData();						
						RadioShowDebug(RadioShowSearchEndBufferEnough);	
					}
					else
					{
						DBG(("Error:empty\n"));
						RadioSetFreq();
					}
					
					RadioMute(FALSE);	
					UnMute();
#ifdef FUNC_SINGLE_LED_EN 
					DispDev();
#endif
#ifdef FUNC_SLAVE_UART_EN
					SlaveRadioRetCurrentStatus();
#endif
				}
				else
				{
					gRadioCtrl.State = RADIO_SEEK;
					
#ifdef	FUNC_RADIO_AUTOSEEK_EN
					if(gRadioCtrl.SeekMode == SEEK_SCANDOWN)
					{
						gRadioData.CurrFreq--;						
					}
					else
					{
						gRadioData.CurrFreq++;
					}					
#endif
				}		
			}		
			break;
			
		case RADIO_IDLE:
			if(IsTimeOut(&gRadioCtrl.SeekPreviewTimer))
			{	
				if(RadioDisFlag == RadioDisCurChChange)
				{
#ifdef AU6210K_BOOMBOX_DEMO
					RadioDisFlag = RadioDisCurPresetCh;
#else
					RadioDisFlag = RadioDisCurFreNum;
#endif
				}
				else if(RadioDisFlag == RadioDisCurChAutoscan)
				{
					RadioDisFlag = RadioDisCurFreNum;
				}				
#ifdef	FUNC_NUMBER_KEY_EN
				else if(RadioDisFlag == RadioDisCurFreBlink)
				{		
#ifdef FUNC_DISP_EN		
					RadioDisBlink();
#endif
					if(RadioDisCount > 0)
					{
						RadioDisCount--;
					}
					else
					{
						RadioDisFlag = RadioDisCurFreNum;
					}
					TimeOutSet(&gRadioCtrl.SeekPreviewTimer, RadioBlinkTime);				
				}
#endif
			}
			break;
			
#ifdef AU6210K_BOOMBOX_DEMO
		case RADIO_PROG:
			if(IsTimeOut(&gRadioCtrl.SeekPreviewTimer))
			{
				gRadioCtrl.State = RADIO_IDLE;	
				RadioDisFlag = RadioDisCurFreNum;
			}
			break;
#endif
			
		default:
			break;	
	}
}


//������Ӧ
//״̬����
VOID RadioStateCtrl(VOID)
{	
	RadioEventProcess();
	RadioStationSearch();

#ifdef FUNC_RADIO_ESD_AUTO_RESUME
	if(gRadioCtrl.State == RADIO_IDLE)
	{
		if(IsTimeOut(&RadioStateCheckTimer))
		{		
			if((MVFM_ReadReg(0x00) & 0x20) || (MVFM_GetCh() != gRadioCurrFreqBack))
			{
				RadioStateCheckCount++;
				if(RadioStateCheckCount > 2)	
				{
					RadioSetFreq();	
					RadioMute(FALSE);	
				}
			}
			else
			{
				RadioStateCheckCount = 0;
			}
			TimeOutSet(&RadioStateCheckTimer, 250);
		}			
	}
#endif
}

#endif
