#include "syscfg.h"
#include "key.h"
#include "utility.h"
#include "user_interface.h"
#include "power.h"
#include "chip.h"
#include "ir.h"
#include "radio_ctrl.h"
#include "debug.h"
#include "display.h"
#include "bluetooth_ctrl.h"
#include "npca110x.h"
#include "bt.h"

BYTE XDATA gWakeupTrigFlg _at_ 0x06D8; //��Ҫ���׸Ķ���ַ


//ϵͳ����������ϵͳ��SLEEP״̬�»����������ߴ�POWER DOWN״̬����Ƭ������Դ�ϵ緽ʽ��������ش���
//һ������£�2��������ʽ���û���Ʒ�в���ͬʱ����
VOID SystemOn(VOID)
{
	TIMER	WaitTimer;

	DBG1(("*******SystemOn************\n"));
	

#if (POWER_SAVING_MODE_OPTION == POWER_SAVING_MODE_SLEEP)
	
	//��ô�SLEEP״̬���ѵĴ���Դ��־
	DBG1((">>WakeUpOp(), gWakeupTrigFlg:%bx\n", gWakeupTrigFlg));

	//ϵͳ������IR���ѹ��ܣ�IR����ͷ������GPIOB6/GPIOE0��IR�źŻ���ϵͳ�����жϼ�ֵ
#ifdef  WIS_FLG_IR
	if((gWakeupTrigFlg == WIS_FLG_GPB6) || (gWakeupTrigFlg == WIS_FLG_GPE0))
	{		
		DWORD IrKeyData;
		

		TimeOutSet(&WaitTimer, 3500);
		//����ڹ涨ʱ����δ����Ԥ����IR��ֵ�������˯��
		while(!IsTimeOut(&WaitTimer))	
		{
#ifdef FUNC_WATCHDOG_EN
			FeedWatchDog();
#endif
			IrKeyData = IrKeyDataVal();
			DBG1(("IrKeyData = : %08LX\n", IrKeyData)); 

			//�����⵽Ԥ����IR��ֵ����ʼ����ϵͳ
			if( (IrKeyData == 0x5AA57F80) ||
				(IrKeyData == 0x5FA07F80) ||
				(IrKeyData == 0x5EA17F80) ||
				(IrKeyData == 0x5DA27F80) ||
				(IrKeyData == 0x5CA37F80) ||
				(IrKeyData == 0x59A67F80) ||
				(IrKeyData == 0x5BA47F80) )
			{
				MessageInit();
				DBG1(("WAKE UP FROM SLEEP BY IR!\n"));
				baGPIOCtrl[GPIO_A_IE] &= ~0x40;//A2
				return;
			}
			else
			{
			 	DBG1(("ERROR IR KEY!\n"));
				break;
			}
		}
		
		DBG1(("TIMEOUT!\n"));
#ifdef IR_KEY_PORT
		if(IR_KEY_PORT == IR_INPUT_GPIOB6)
		{
			SetWakeupSrcInSleep(WIS_GPB6_LOW);
			DBG1(("*GPIO_B_IN:%bx\n", GetGpioReg(GPIO_B_IN)));
			while((GetGpioReg(GPIO_B_IN) & 0x40) != 0x40)
			{
				DBG1(("��ȷ������Sleep״̬ǰWakeUp������������(GPIO_C_IN:%bx(GPIOC3 shoule be 0))\n", GetGpioReg(GPIO_B_IN)));
				WaitMs(10);
#ifdef FUNC_WATCHDOG_EN
				FeedWatchDog();
#endif
			}
		}
		else if(IR_KEY_PORT == IR_INPUT_GPIOE0)
		{
			SetWakeupSrcInSleep(WIS_GPE0_LOW);
			DBG(("*GPIO_E_IN:%bx\n", GetGpioReg(GPIO_E_IN)));
			while((GetGpioReg(GPIO_E_IN) & 0x01) != 0x01)
			{
				DBG1(("��ȷ������Sleep״̬ǰWakeUp������������(GPIO_C_IN:%bx(GPIOC3 shoule be 0))\n", GetGpioReg(GPIO_B_IN)));
				WaitMs(10);
#ifdef FUNC_WATCHDOG_EN
			FeedWatchDog();
#endif
			}
		}
#endif

		GoToSleepMode(SLEEP_MOD_OTP_PD);

		DBG1(("<<WakeUpOp()\n")); 
		return;
	}

	
#endif
	

#elif (POWER_SAVING_MODE_OPTION == POWER_SAVING_MODE_POWERDOWN)
#if	(PWR_KEY_MODE == PWR_KEY_PUSH_BUTTON)
	DBG(("\nPWR_KEY_PUSH_BUTTON\n\n"));	
	SetChip8SRstMode(CRM_IN_PUSH_BUTTON);

#elif(PWR_KEY_MODE == PWR_KEY_SLIDE_SWITCH)
	DBG(("\nPWR_KEY_SLIDE_SWITCH\n\n"));	
	SetChip8SRstMode(CRM_IN_SLIDE_SWITCH);
#endif

	gWakeupTrigFlg = GetWakeupFlgFromPD();			//��ô�powerdown״̬����Ƭ������Դ�ϵ�ϵͳ�����Ĵ���Դ��־
	DBG(("gWakeupTrigFlg:%bx\n", gWakeupTrigFlg));

	if(WIP_SRC_PWR_KEY & gWakeupTrigFlg)
	{
		DBG(("POWER KEY SYS ON\n"));
	}
	else if(WIP_SRC_LP_RTC & gWakeupTrigFlg)
	{
		DBG(("LP_RTC SYS ON\n"));
	}

	//���POWER_KEYʹ������(push button)�����Ҷ�����POWER KEY���ػ�����ʱ��
	//���磬Ҫ��סPOWER_KEY����2���ϵͳ�Ż������ʾ�������У�����ϵͳ�ٴιرգ��û�����о���ϵͳ���й�
	//POWER_KEY���������˹涨ʱ����ж�����power_key.c����ɵġ�
#if	((PWR_KEY_MODE == PWR_KEY_PUSH_BUTTON) && defined(TIME_POWER_ON_HOLD))	
	if(WIP_SRC_PWR_KEY & gWakeupTrigFlg)		//������POWER_KEY����ϵͳʱ���ż�鿪������ʱ�䣬��WTD��λ��ϵͳ������������Ҫ
	{
		TimeOutSet(&WaitTimer, (TIME_POWER_ON_HOLD * 2));
		
		while(!IsTimeOut(&WaitTimer))
		{
#ifdef FUNC_WATCHDOG_EN
			FeedWatchDog();
#endif
			KeyEventGet();	

			if(MessageGet(MSG_FIFO_KEY) == MSG_SYS_ON)		//����ﵽ��������ʱ�䣬��ϵͳ��������
			{			
				MessageInit();
				DBG(("POWER_KEY Hold time,MSG_SYS_ON\n"));
				return;
			}
		}
		
		DBG(("TIMEOUT!\n"));
		//����ڹ涨ʱ����û���յ���power_key.c���͵�MSG_SYS_ON��Ϣ����Ҫ�ٴ�ϵͳ�رա�
		GoToPDMode();
	}

#elif(PWR_KEY_MODE == PWR_KEY_SLIDE_SWITCH)
	DBG(("\nPWR_KEY_MODE == PWR_KEY_SLIDE_SWITCH\n\n"));	
#endif		
		
#endif
}

#ifdef AU6210K_AT_BT809
extern BOOL power_off_flag;
#endif

//ϵͳ�رգ�����ϵͳ��SLEEP��ʽ�رջ�����Ƭ������Դ�µ�POWER DOWN��ʽ�رյ���ش���
VOID SystemOff(VOID)
{
#ifdef FUNC_MIN_MAX_VOLUME_LED_WITH_TONE
	BYTE times=0;
	BOOL vol_disp_flag = TRUE;
	TIMER		Disp_vol;

	TimeOutSet(&Disp_vol, 200);
	ClrGpioRegBit(GPIO_E_OUT, (1 << 2));
	ClrGpioRegBit(GPIO_A_OUT, (1 << 0));
#endif
	BT_POWER_L();  //mini503 ������Դ���ܵ�Դ���ر仯
#ifdef FUNC_MIN_MAX_VOLUME_LED_WITH_TONE
	while(1)
	{
#ifdef FUNC_WATCHDOG_EN
		FeedWatchDog();
#endif
		power_off_flag = TRUE;
	
		if(times > 10)
			break;
		if(IsTimeOut(&Disp_vol) )
		{
			times++;
			TimeOutSet(&Disp_vol, 200);
			if(vol_disp_flag)
			{
				vol_disp_flag = FALSE;
				SetGpioRegBit(GPIO_E_OUT, (1 << 2));
				SetGpioRegBit(GPIO_A_OUT, (1 << 0));
			}
			else
			{
				vol_disp_flag = TRUE;
				ClrGpioRegBit(GPIO_E_OUT, (1 << 2));
				ClrGpioRegBit(GPIO_A_OUT, (1 << 0));
			}
		}

	}
	WaitMs(1000);
#endif

	
#if (POWER_SAVING_MODE_OPTION == POWER_SAVING_MODE_SLEEP)
	//���ߣ�����SLEEPģʽ
	DBG(("SLEEP!\n"));
#ifdef FUNC_DISP_EN
	DispDeInit();
#endif

#ifdef FUNC_RADIO_EN
	if(gSys.SystemMode == SYS_MODE_RADIO)
	{
		RadioCtrlDeinit();
	}
#endif
#ifdef AU6210K_MINI503
		SPI_PlaySelectNum(SPIPLAY_SONG_POWEROFF,0);
	//	SLedLightOp(LED_POWER, FALSE);	
		WaitMs(1000);
#endif

	MuteOn(FALSE, TRUE);
	//NPCA110X_SetOutMute(TRUE);
	InDacChannelSel(DAC_CH_NONE);
	InDacPowerDown(FALSE);

	//���⻽��Դѡ��ʱ��Ҫע��ѡ���GPIO�˿�
	SetWakeupSrcInSleep(WIS_GPB6_LOW);
	//SetWakeupSrcInSleep(WIS_GPD7_HIGH);
	//SetWakeupSrcInSleep(WIS_GPE0_LOW);
	//SetWakeupSrcInSleep(WIS_GPE1_HIGH);
	//SetWakeupSrcInSleep(WIS_RTC_ALARM);
	//SetWakeupSrcInSleep(WIS_PWR_KEY); 
	GoToSleepMode(SLEEP_MOD_OTP_PD);
	//GoToSleepMode(SLEEP_MOD_NORMAL);
	return;

#elif (POWER_SAVING_MODE_OPTION == POWER_SAVING_MODE_POWERDOWN)


#ifdef AU6210K_MINI503
		ClrGpioRegBit(GPIO_B_OUT, (1 << 5));
		SPI_PlaySelectNum(SPIPLAY_SONG_POWEROFF,0);
	//	SLedLightOp(LED_POWER, FALSE);	
		WaitMs(1000);
		SetGpioRegBit(GPIO_B_OUT, (1 << 5));
#endif

#if(PWR_KEY_MODE == PWR_KEY_PUSH_BUTTON) //POWER_KEYΪ��ť���أ����أ�
	DBG(("\nSYSTEM OFF BY PUSH BUTTON!\n\n"));

#ifdef	FUNC_ALARM_EN
	SetWakeupLpRtcInPD(); // RTC���ӻ���ʹ��
#endif

#elif(PWR_KEY_MODE == PWR_KEY_SLIDE_SWITCH)
		DBG(("\nSYSTEM OFF BY SLID SWITCH!\n\n"));	
#endif
	
#ifdef FUNC_DISP_EN
	DispDeInit();
#endif

	MuteOn(FALSE, TRUE);
//ִ��InDacMuteEn��InDacChannelSel��InDacPowerDown ��800ms���ң���ɹػ����ܿ��ٰ�������
//�����ⲿ�ִ������ܵ��¿��ٿ�������POP��
//	InDacChannelSel(DAC_CH_NONE);
//	InDacPowerDown(FALSE);
	
	GoToPDMode();	
#endif
}


#ifdef FUN_SYSTEM_POWEROFF_WAIT_TIME
SystemTimingOff gSysTimeOffState;


VOID SystemOffTimeInit(VOID)
{
	gSysTimeOffState.ProState = SYS_OFF_TIMEING_IDLE;
	gSysTimeOffState.EnState = FALSE;
	gSysTimeOffState.SleepState = SLEEP_00;
}


static VOID SystemOffSetTimer(VOID)
{
	DWORD SysOffWaitTime;

	switch(gSysTimeOffState.SleepState)
	{	
		case SLEEP_15:
			SysOffWaitTime = 900000;
			break;
			
		case SLEEP_30:
			SysOffWaitTime = 1800000;
			break;

		case SLEEP_45:
			SysOffWaitTime = 2700000;
			break;
			
		case SLEEP_60:
			SysOffWaitTime = 3600000;
			break;
			
		case SLEEP_75:
			SysOffWaitTime = 4500000;
			break;	
			
		case SLEEP_90:
			SysOffWaitTime = 5400000;
			break;

		case SLEEP_105:
			SysOffWaitTime = 6300000;
			break;
			
		case SLEEP_120:
			SysOffWaitTime = 7200000;
			break;
			
		default:
			break;
	}
	
	TimeOutSet(&gSysTimeOffState.AutoOutTimer, SysOffWaitTime);	
	DBG(("Start....                                          %2BX\n",gSysTimeOffState.SleepState));
}


VOID SystemOffTimeEnDis(BYTE Type)
{
	TimeOutSet(&gSysTimeOffState.DisTimer, 3000);
		
	if(SYSOFF_TIME_SET_VALUE == Type)
	{
		if(gSysTimeOffState.EnState == FALSE)
		{
			gSysTimeOffState.EnState = TRUE;				
			gSysTimeOffState.ProState = SYS_OFF_TIMEING_SET_MODE_VALID;	
			SystemOffSetTimer();
		}
		else
		{				
			gSysTimeOffState.EnState = FALSE;		
			gSysTimeOffState.ProState = SYS_OFF_TIMEING_SET_DIS;
		}	
	}
	else if(SYSOFF_TIME_READ_ONLY == Type)
	{		
		if((gSysTimeOffState.ProState == SYS_OFF_TIMEING_WAIT_POWEROFF) || (gSysTimeOffState.ProState == SYS_OFF_TIMEING_SET_MODE_VALID))			
		{
			gSysTimeOffState.ProState = SYS_OFF_TIMEING_READ_ONLY_TIMING;//�ص���ʱ�ػ������ö�ʱ�ػ�״̬
		}	
		else
		{
			gSysTimeOffState.ProState = SYS_OFF_TIMEING_READ_ONLY_IDLE;//�ص�����״̬		
		}
	}
}


VOID SystemOffTimeSet(VOID)
{				
	TimeOutSet(&gSysTimeOffState.DisTimer, 3000);	

	gSysTimeOffState.SleepState++;
	if(gSysTimeOffState.SleepState >= SLEEP_MAX)
	{
		gSysTimeOffState.SleepState = SLEEP_00;
	}	
	gSysTimeOffState.ProState = SYS_OFF_TIMEING_SET_MODE_VALID;	
	
	if(gSysTimeOffState.SleepState != SLEEP_00)
	{
		gSysTimeOffState.EnState = TRUE;
		SystemOffSetTimer();
	}
	else
	{
		gSysTimeOffState.EnState = FALSE;
	}	

//	if(gSysTimeOffState.EnState == TRUE)
//	{
//		gSysTimeOffState.SleepState++;
//		if(gSysTimeOffState.SleepState >= SLEEP_MAX)
//		{
//			gSysTimeOffState.SleepState = SLEEP_00;
//		}	
//		SystemOffSetTimer();
//		gSysTimeOffState.ProState = SYS_OFF_TIMEING_SET_MODE_VALID;	
//	}
//	else
//	{
//		gSysTimeOffState.ProState  = SYS_OFF_TIMEING_SET_MODE_INVALID;
//	}
}


//����кʹ˺��������÷�(Ƕ�뵽����ģʽ)�ĺ���λ����󣬴˺�����Ҫ��default����ֹ���غ������õ���Ϣ
VOID SystemOffTimeMsgPro(BYTE Msg)
{	
	if(Msg != MSG_NONE)
	{
		TimeOutSet(&gSysTimeOffState.DisTimer, 0);	
	}
	
	switch(Msg)
	{
		case MSG_SLEEP:
			SystemOffTimeSet();
			break;		
			
//		case MSG_SYS_TIMING_EN:
//			SystemOffTimeEnDis(SYSOFF_TIME_SET_VALUE);
//			break;

//		case MSG_SYS_TIMING_SET:
//			SystemOffTimeSet();
//			break;

//		case MSG_SYS_TIMING_READ:	
//			SystemOffTimeEnDis(SYSOFF_TIME_READ_ONLY);
//			break;		

		default:
			break;
	}
}


VOID SystemOffTimeWait(VOID)
{
	switch(gSysTimeOffState.ProState)
	{
		case SYS_OFF_TIMEING_IDLE:
			break;

		case SYS_OFF_TIMEING_READ_ONLY_IDLE:
		case SYS_OFF_TIMEING_READ_ONLY_TIMING:
		case SYS_OFF_TIMEING_SET_DIS:
		case SYS_OFF_TIMEING_SET_MODE_VALID:
		case SYS_OFF_TIMEING_SET_MODE_INVALID:
			if(IsTimeOut(&gSysTimeOffState.DisTimer))
			{
				if((gSysTimeOffState.ProState == SYS_OFF_TIMEING_SET_MODE_VALID) || (gSysTimeOffState.ProState == SYS_OFF_TIMEING_READ_ONLY_TIMING))
				{
					gSysTimeOffState.ProState = SYS_OFF_TIMEING_WAIT_POWEROFF;
				}
				else
				{
					gSysTimeOffState.ProState = SYS_OFF_TIMEING_IDLE;
				}
			}
			break;

		case SYS_OFF_TIMEING_WAIT_POWEROFF:
			if(IsTimeOut(&gSysTimeOffState.AutoOutTimer))
			{
				if(gSysTimeOffState.EnState == TRUE)
				{
					DBG(("SYSTEM OFF BY TimeOut!\n"));
					SystemOff();
				}
			}
			break;
			
		default:
			break;					
	}
}

#endif


