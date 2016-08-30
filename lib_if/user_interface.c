#include "syscfg.h"
#include "display.h"
#include "radio_app_interface.h"
#include "user_interface.h"
#include "playctrl.h"
#include "breakpoint.h"
#include "debug.h"
#include "power.h"
#include "device.h"
#include "spi_play.h"
#include "touchkey_ctrl.h"
#include "pt231x.h"
#include "bt.h"
#include "npca110x.h"
#include "power_monitor.h"



BOOL IsDacMute = FALSE;
BOOL KeyInLedFlashOn = 0;
WORD KeyInLedFlashHoldTmr = 0;
WORD KeyInLedFlashIntTmr = 0;
#ifdef FUNC_BEEP_SOUND_EN 	
BOOL gPushKeyNeedBeepFlag = FALSE;	// ��Ҫ����������������־
BOOL gPushKeyBeepStartFlag = FALSE;	//����������������־
TIMER gPushKeyBeepTimer;	//��������������һ�ε�ʱ�䳤�ȶ�ʱ
#ifdef FUNC_RADIO_EN
extern BYTE CODE gFmInGain[];
#endif
#ifdef FUNC_LINEIN_EN
extern BYTE CODE gLineInGain[];
#endif
#endif
extern VOID BluetothCtrlEnd(VOID);

extern BOOL GetBtMuteFlag();

#ifdef FUNC_SAVE_PLAYTIME_EN
BOOL gSavePlayTimeEnFlag = FALSE;
#endif

#ifdef	FUNC_DEVICE_FORCEPAUSE_EN
extern BOOL gDevicePlayForcePauseFlag;
#endif

extern VOID PlayVol2Decd(VOID);
extern VOID SetLineInVolume(BYTE Vol);
extern VOID DeviceAudioSetVolume(VOID);
extern VOID SetBluetoothVolume(BYTE Vol);

extern BYTE gRunningDev;
extern BOOL ROM_QuickResponse(BYTE Dev);
BOOL IsSupportSpecialDevice = FALSE;	//֧�ֲ�����ɫ�豸��MBR 512�ֽ���������ֽڲ���0x55AA��Ĭ��ΪFLASE����֧�������豸��

BOOL getDacMute()
{
	return IsDacMute;
}
// ѡ��DAC ���ⲿ����Mute״̬
// Param: 
//		DacMuteEn: 1--ѡ��Dac Mute,  0--ѡ����������Ϊ0
//		ExMuteEn: �ⲿ����Mute ʹ��ѡ��
VOID MuteOn(BOOL DacMuteEn, BOOL ExMuteEn)
{
#ifdef FUNC_EXMUTE_EN
	if(ExMuteEn == TRUE)
	{
		ExMuteOn(); 	
	}
#endif
#if defined(FUNC_PT231X_EN) && !defined(AU6210K_NR_D_8_CSRBT)
      PT2313MuteOnOff(1);
#endif
	if(DacMuteEn == TRUE)
	{	
#ifdef AU6210K_NR_D_8_CSRBT
		NPCA110X_SetOutMute(TRUE);
#else
#endif
		InDacMuteEn();	

		//���������´�����LineIn Mute״̬��©�����⣬���ǻ������΢��POP.
#ifdef FUNC_LINEIN_EN
		if(gSys.SystemMode == SYS_MODE_LINEIN)
		{
			DBG(("InDacChannelSel(DAC_CH_NONE),Mute\n"));
			InDacChannelSel(DAC_CH_NONE);
		}
#endif


#if defined(FUNC_BLUETOOTH_CSR_EN)||defined(AU6210K_NR_D_8_CSRBT)
		if(gSys.SystemMode == SYS_MODE_BLUETOOTH)
		{
			DBG(("btInDacChannelSel(DAC_CH_NONE),Mute\n"));
			InDacChannelSel(DAC_CH_NONE);
		}
#endif


#if defined(AU6210K_JLH_JH82B)
#else
#ifdef FUNC_BLUETOOTH_EN
		if (gSys.SystemMode == SYS_MODE_BLUETOOTH)
		{
			InDacChannelSel(DAC_CH_NONE);

		}
#endif
#endif

	}
	else
	{
#ifdef AU6210K_NR_D_8_CSRBT
		NPCA110X_DAC1_Set_Volume_and_Mute(0);
		NPCA110X_SetOutMute(TRUE);
#else
		InDacSetVolume(0, 0);
#endif
	}	

#ifdef	FUNC_RADIO_EN
	if(gSys.SystemMode == SYS_MODE_RADIO)
	{
		RadioMute(TRUE);
	}
#endif	

	IsDacMute = TRUE;
}


// ���Mute״̬
VOID UnMute(VOID)
{
#ifdef	FUNC_DEVICE_FORCEPAUSE_EN
	if(gDevicePlayForcePauseFlag == TRUE)
	{
		return;
	}
#endif

	DBG1(("unmute.....  \n"));
	if(gSys.Volume == 0)
	{
		return;
	}	

	if(IsDacMute == TRUE)
	{		
#ifdef	FUNC_RADIO_EN
		if(gSys.SystemMode == SYS_MODE_RADIO)
		{
			RadioMute(FALSE);
			RadioSetVolume(gSys.Volume);
		}
#endif	

		//���MuteOn��������InDacChannelSel��������Ҫ��UnMute������������ͨLINEINͨ��.
#ifdef FUNC_LINEIN_EN
		if(gSys.SystemMode == SYS_MODE_LINEIN)
		{
			DBG(("InDacChannelSel(DAC_CH_LINEIN),UnMute,gSys.Volume:%bd\n", gSys.Volume));

#ifdef AU6210K_NR_D_8_CSRBT
			NPCA110X_ADC_Input_CH_Select(INPUT_CH0);
			NPCA110X_DAC1_Set_Volume_and_Mute(gSys.Volume);
#else
			InDacChannelSel(LINEIN_IO_TYPE);
			SetLineInVolume(gSys.Volume);
#endif			
		}
#endif	

#ifdef FUNC_BLUETOOTH_CSR_EN
		if(gSys.SystemMode == SYS_MODE_BLUETOOTH)
		{
#ifdef AU6210K_NR_D_8_CSRBT
			NPCA110X_ADC_Input_CH_Select(INPUT_CH1);
			NPCA110X_DAC1_Set_Volume_and_Mute(gSys.Volume);
#else
		
			DBG(("InDacChannelSel(DAC_CH_LINEIN),UnMute,gSys.Volume:%bd\n", gSys.Volume));
			InDacChannelSel(BLUETOOTH_CH_TYPE);
			SetBluetoothVolume(gSys.Volume);
		
#endif			
		}
#endif	

#if defined(AU6210K_JLH_JH82B)
		SetBluetoothVolume(gSys.Volume);
#else
#ifdef FUNC_BLUETOOTH_EN
		if (gSys.SystemMode == SYS_MODE_BLUETOOTH)
		{
			DBG(("bluetooth unmte\n"));
			SET_E0_ANALOG_IN();	//��E0E1����Ϊģ��˿�
			SET_E1_ANALOG_IN();	
			InDacChannelSel(DAC_CH_E0E1); //������E0E1ͨ��
			//SetBluetoothVolume(gSys.Volume);
		}
#endif
#endif

	}
#ifdef AU6210K_NR_D_8_CSRBT
	NPCA110X_SetOutMute(FALSE);
#endif
	InDacMuteDis();

#ifdef FUNC_EXMUTE_EN
	if(gSys.Volume > 0)
	{
#ifdef	FUNC_HEADPHONE_DETECT_EN
		if(IsHeadphoneLinkFlag == FALSE)
#endif
		{
			ExMuteOff();
		}	
	}		
#endif
#if 0// defiend(FUNC_PT231X_EN) && !defiend(AU6210K_NR_D_8_CSRBT)
      PT2313MuteOnOff(0);
#endif

	IsDacMute = FALSE;
}


// �ı�Mute״̬
VOID MuteStatusChange(VOID)
{	
	DBG((">>MuteStatusChange()\n"));
	if(gSys.MuteFg)
	{
		gSys.MuteFg = FALSE;		
		UnMute();	
		DBG1(("4433\n"));
	}
	else
	{	
		gSys.MuteFg = TRUE;
		MuteOn(TRUE, TRUE);       
	}
#ifdef FUNC_DISP_EN
   	DispMute();
#endif	
	DBG(("<<MuteStatusChange()\n"));
}


// ����ϵͳ����
VOID SetVolume(VOID)
{
	DBG((">>SetVolume() %d\n",(WORD)gSys.Volume));
	
	switch(gSys.SystemMode)
	{
#if (defined(FUNC_USB_EN) || defined(FUNC_CARD_EN))
#ifdef FUNC_USB_EN
		case SYS_MODE_USB:
#endif
#ifdef FUNC_CARD_EN
		case SYS_MODE_SD:
#endif
#ifdef	FUNC_OTP_PLAY_EN
		case SYS_MODE_OTPDEV:
#endif
			PlayVol2Decd();
			break;
#endif

#ifdef FUNC_LINEIN_EN
		case SYS_MODE_LINEIN:
			SetLineInVolume(gSys.Volume);
			break;
#endif

#ifdef FUNC_BLUETOOTH_CSR_EN
		case SYS_MODE_BLUETOOTH:
			SetBluetoothVolume(gSys.Volume);
			break;
#endif

#if 0//def FUNC_BLUETOOTH_EN
		case SYS_MODE_BLUETOOTH:
			SetBluetoothVolume(gSys.Volume);
			break;
#endif


#ifdef FUNC_RADIO_EN
		case SYS_MODE_RADIO:
			RadioSetVolume(gSys.Volume);
			break;
#endif

#ifdef FUNC_AUDIO_EN
		case SYS_MODE_AUDIO:
			DeviceAudioSetVolume();
			break;
#endif

#ifdef FUNC_AUDIO_READER_EN
		case SYS_MODE_AUDIO_READER:
			DeviceAudioSetVolume();
			break;
#endif

		default:
			return;//break;
	}

#ifdef FUNC_BREAK_POINT_EN
	BP_SaveInfo(&gBreakPointInfo.PowerMemory.Volume,sizeof(gBreakPointInfo.PowerMemory.Volume));
#endif

}


// ����ϵͳ������ͬʱ���Mute״̬
VOID SetVolumeWithMute(VOID)
{
#ifdef AU6210K_NR_D_8_CSRBT
	DBG((">>NPCA110X_DAC1_Set_Volume_and_Mute() %d\n",(WORD)gSys.Volume));
#if (defined(FUNC_USB_EN) || defined(FUNC_CARD_EN))
	if(gSys.SystemMode == SYS_MODE_SD)
		SetVolume();
	else
#endif
	{
		NPCA110X_DAC1_Set_Volume_and_Mute(gSys.Volume);
#ifdef FUNC_BREAK_POINT_EN
		BP_SaveInfo(&gBreakPointInfo.PowerMemory.Volume,sizeof(gBreakPointInfo.PowerMemory.Volume));
#endif
		
	}
	DBG(("gSys.Volume = %x\n",gSys.Volume));	
DBG(("gBreakPointInfo.PowerMemory.Volume = %x\n",gBreakPointInfo.PowerMemory.Volume));	


#else
	SetVolume();
#endif

	if((gSys.Volume == 0) && (IsDacMute == FALSE))
	{
		gSys.MuteFg = FALSE;		
		MuteOn(TRUE, TRUE);		
	}
	if((IsDacMute == TRUE) && (gSys.Volume > 0))
	{
		gSys.MuteFg = TRUE;
	}
	if(gSys.MuteFg == TRUE)
	{
		gSys.MuteFg = FALSE;
		DBG1(("rrrrrr\n"));
		UnMute();	
	}

#ifdef FUNC_DISP_EN
#ifndef FUNC_SINGLE_LED_EN
	DispMute();	  //��ȷ�ָ�Mute��ʾ״̬�������Mute״̬������������ɵ���ʾ����
#endif	
#endif
}

#ifdef FUNC_MIN_MAX_VOLUME_LED
extern BOOL min_max_volume_flag;
extern BOOL led_volume_display_end;
extern VOID SetLedVolumeDis();
#endif

// ����ϵͳ������ͬʱ���Mute״̬����ʾ����
VOID SetVolumeWithDisplay(VOID)
{
	SetVolumeWithMute();
#ifdef FUNC_DISP_EN
	DispVolume(TRUE);
#endif
}


//����ϵͳ����
VOID VolumeAdjust(BYTE Direction)
{	
#ifdef	FUNC_DEVICE_FORCEPAUSE_EN
	if(gDevicePlayForcePauseFlag == TRUE) 
	{
		return;
	}					
#endif
	
	if(Direction == UP)
	{
		DBG(("Vol+\n"));			
		if(gSys.Volume < VOLUME_MAX)
		{
			gSys.Volume++;	
		}	
	}
	else
	{
		DBG(("Vol-\n"));			
		if(gSys.Volume > VOLUME_MIN)
		{
			gSys.Volume--;
		}	
	}
#ifdef FUNC_MIN_MAX_VOLUME_LED
	if (gSys.Volume >= VOLUME_MAX ||
		gSys.Volume <= VOLUME_MIN)// && led_volume_display_end)
	{
		min_max_volume_flag = TRUE;
		//SetLedVolumeDis();
	}
#endif

	
#if 0//AU6210K_ZB_BT007_CSR
		{
			if(gSys.Volume == VOLUME_MAX)	
			{
				SPI_PlaySelectNum(SPIPLAY_SONG_MAX_VOLUME, 1);			

	#ifdef FUNC_EXMUTE_EN
				if(gSys.SystemMode == SYS_MODE_BLUETOOTH && !GetBtMuteFlag())
				{
					DBG1(("123\m"));
					UnMute();
				}
	#endif
			}
	
			if(gSys.Volume == VOLUME_MIN)	
			{
				SPI_PlaySelectNum(SPIPLAY_SONG_MIN_VOLUME, 1);	
	#ifdef FUNC_EXMUTE_EN

				if(gSys.SystemMode == SYS_MODE_BLUETOOTH && !GetBtMuteFlag())
				{
					DBG1(("345\m"));
					UnMute();
				}
	#endif
			}
		}
#endif	
	SetVolumeWithDisplay();	
			
#ifdef FUNC_BEEP_SOUND_EN	
	if(gSys.Volume < VOLUME_MAX)	
	{
		PushKeyBeep(1);
	}
	else
	{
		PushKeyBeep(2);
	}
#endif	
}


#ifndef FUNC_RADIO_EN
//����DAC���������л���ã���Ҫ��FUNC_RADIO_EN��ر�ʱ�������º���
VOID RadioCtrlInit(VOID)
{
	//DBG(("RadioCtrlInit,00\n"));
}


VOID RadioCtrlDeinit(VOID)
{
	//DBG(("RadioCtrlDeinit,00\n"));
}


BOOL RadioMute(BOOL MuteFlag)
{
	//DBG(("RadioMute,00\n"));
	return TRUE;
}
#endif


#ifdef FUNC_BEEP_SOUND_EN
//  ��������������
// Param:
//		ContinueNum: ����������������������
VOID PushKeyBeep(BYTE ContinueNum)
{		
	if(gPushKeyNeedBeepFlag == TRUE)		
	{
#ifdef	FUNC_RADIO_EN
		if(gSys.SystemMode == SYS_MODE_RADIO)  
		{
			SysClkDivClr();
		}
#endif
		
#ifdef FUNC_EXMUTE_EN
		if((gSys.Volume > 0) && (IsDacMute == TRUE))
		{		
#ifdef	FUNC_HEADPHONE_DETECT_EN
			if(IsHeadphoneLinkFlag == FALSE)
#endif
			{
				ExMuteOff();
			}
		}		
#endif
		
		while(ContinueNum)
		{		
			ContinueNum--;
			//����GPE0(FM_L)��Ϊ�ź�����ͨ��
			baGPIOCtrl[GPIO_E_OE] &= ~0x01;
			baGPIOCtrl[GPIO_E_IE] &= ~0x01;
			baGPIOCtrl[GPIO_E_PU] &= ~0x01;
			baGPIOCtrl[GPIO_E_PD] |= 0x01;		
			DBG(("InDacExFmConnect(DAC_CH_E0_L)\n"));
			InDacExFmConnect(DAC_CH_E0_L);	
#ifdef	FUNC_LINEIN_EN
			if(gSys.SystemMode == SYS_MODE_LINEIN) 
			{
				InDacLineInGain(gLineInGain[gSys.Volume], gLineInGain[gSys.Volume]);					
			}
			else
#endif
			{
#ifdef	FUNC_RADIO_EN
				InDacLineInGain(gFmInGain[gSys.Volume], gFmInGain[gSys.Volume]);
#else
				InDacLineInGain(gLineInGain[gSys.Volume], gLineInGain[gSys.Volume]);	
#endif
			}
			WaitUs(10);
			baGPIOCtrl[GPIO_E_OE] |= 0x01;  //����E0Ϊ���
			gPushKeyBeepStartFlag = TRUE;
			
			TimeOutSet(&gPushKeyBeepTimer, 20);
			while(!IsTimeOut(&gPushKeyBeepTimer))
			{			
				if((gSys.SystemMode == SYS_MODE_USB) || (gSys.SystemMode == SYS_MODE_SD)) 
				{
					SongPlayDo();
				}	
			}
			gPushKeyBeepStartFlag = FALSE;		
			
			DBG(("InDacExFmDisConnect()\n"));
			InDacExFmDisConnect();		
			baGPIOCtrl[GPIO_E_OE] &= ~0x01;
			baGPIOCtrl[GPIO_E_IE] &= ~0x01;
			baGPIOCtrl[GPIO_E_PU] &= ~0x01;
			baGPIOCtrl[GPIO_E_PD] &= ~0x01;				

			if(ContinueNum)
			{
				TimeOutSet(&gPushKeyBeepTimer, 100);
				while(!IsTimeOut(&gPushKeyBeepTimer))
				{			
					if((gSys.SystemMode == SYS_MODE_USB) || (gSys.SystemMode == SYS_MODE_SD)) 
					{
						SongPlayDo();
					}	
				}		
			}
		}
		
#ifdef FUNC_EXMUTE_EN
		if((gSys.Volume > 0) && (IsDacMute == TRUE))
		{		
#ifdef	FUNC_HEADPHONE_DETECT_EN
			if(IsHeadphoneLinkFlag == FALSE)
#endif
			{
				ExMuteOn();
			}
		}		
#endif
		
#ifdef	FUNC_RADIO_EN
		if(gSys.SystemMode == SYS_MODE_RADIO)  
		{
			SysClkDivSet(CLK_DIV_RATE);
		}
#endif
		gPushKeyNeedBeepFlag = FALSE;
	}
}
#endif


//----------------------------------------------------------
// ����û���Ҫ���˿��ļ��С����ܣ�Ҫ��Ӧ�ô�������дFsInfoInit()����
#ifdef FUNC_FOLDER_EN
#define	MAX_FOLDER_NUM_SAVE 	1024			//save foler 1-2048 empty flag
BYTE gFolderEmpty[MAX_FOLDER_NUM_SAVE / 8];
extern WORD gMaxFoldBitMapNum;
extern BYTE XDATA* gpFoldEmpty;


// �˿��ļ��г�ʼ��
VOID FsInfoInit(VOID)
{

	gpFoldEmpty = &gFolderEmpty;
	gMaxFoldBitMapNum = MAX_FOLDER_NUM_SAVE;
	memset(gpFoldEmpty, 0xFF, sizeof(gFolderEmpty));
	DBG(("FsInfoInit,gpFoldEmpty:0x%lx, gMaxFoldBitMapNum:%u\n", (DWORD)gpFoldEmpty, gMaxFoldBitMapNum));	
}
#endif


//----------------------------------------------------------
// ����û���Ҫʵ�֡�����������Ӧ�����ܣ�Ҫ��Ӧ�ô�������дQuickResponse()����
#ifdef FUNC_QUICK_RESPONSE_EN
extern BOOL IsSwitchMode;
// �����Ҫ�����˳�����TRUE.
BOOL QuickResponse(VOID)
{
	// To do...	
	if(MessageGet(MSG_FIFO_DEV_CTRL) == MSG_MODE_SW)
	{
		IsSwitchMode = TRUE;  
		MessageInit();
		return TRUE;
	}

	return FALSE;
}
#endif

#if defined(FUNC_MIN_MAX_VOLUME_LED_WITH_TONE)
extern BOOL type_flag;
#endif
extern BOOL BTisMute();
//----------------------------------------------------------
// ����û���Ҫ��1msϵͳʱ����������Ҫ��Ӧ�ô�������дTimerTick1ms()����
extern BOOL gIsNvmOnUse;
VOID TimerTick1ms(VOID)
{
#if	(PWR_KEY_MODE == PWR_KEY_SLIDE_SWITCH)
	extern BOOL gIsPwrkeyPadOn;

	if((gIsPwrkeyPadOn == FALSE) && (gIsNvmOnUse == FALSE))
	{
		//����ִ��InDacChannelSel��InDacPowerDown֮�������
#ifdef FUNC_DISP_EN
		DispDeInit();
#endif	
#ifdef FUNC_EXMUTE_EN		
		ExMuteOn(); 
#endif
		if(gSys.SystemMode == SYS_MODE_BLUETOOTH)
			BluetothCtrlEnd();
		
		DBG1(("22222GoToPDMode\n"));
		//InDacMuteEn();	//�ر���Ϊ�� ����ػ�ʱ��̫������
		GoToPDMode();		//ִ��Ӳ���ص�ϵͳ�رն���
	}
#endif

#ifdef FUNC_SEG_LED_EN 
#if !defined(FUNC_TIMER1_REFRESH_LED_EN)
	LedFlushDisp();
#endif
#endif

#ifdef FUNC_SEG_LCD_EN 
	LcdFlushDisp();
#endif
#if 0
      	if(KeyInLedFlashHoldTmr)
	{
		KeyInLedFlashHoldTmr--;
		if(KeyInLedFlashIntTmr)
		{
			KeyInLedFlashIntTmr--;				
		}
		else
		{
			KeyInLedFlashIntTmr =100;
			if(KeyInLedFlashOn)
			{
				KeyInLedFlashOn =0;
				baGPIOCtrl[GPIO_B_OE]|=0x80;
				baGPIOCtrl[GPIO_B_OUT]|=0x80;
			}
			else
			{
				KeyInLedFlashOn =1;
				baGPIOCtrl[GPIO_B_OE]|=0x80;
				baGPIOCtrl[GPIO_B_OUT]&=(~0x80);
			}
		}
	}
#endif
#if 1
	if((gSys.SystemMode == SYS_MODE_BLUETOOTH) && (!type_flag))
	{
		if(btIO_Red_Led_is_High())
			setMCU_Red__Led_Port();
		else
			clrMCU_Red__Led_Port();

		if(btIO_Blue_Led_is_High())
			setMCU_Blue__Led_Port();
		else
			if(GetPwrDisp() != PWR_MNT_DISP_EMPTY_V)
				clrMCU_Blue__Led_Port();
			else
				setMCU_Blue__Led_Port();				

#ifdef FUNC_EXMUTE_EN
		if(BTisMute())		
			//mute		
			ExMuteOn();	 
		else
			//unmute
			ExMuteOff();
#endif

		
	}
#endif

	// To do...
}


// ����ͻ���Ҫ�����ض�VID/PID��USB�豸������ͨ��������ʵ��
BOOL USB_IgnoreDev(VOID)
{
	if(gLibUsbInfo.VID == 0x05AC)	//����ƻ����˾�����豸
	{
		DBG(("Apple device!\n"));
		return TRUE;
	}

	return FALSE;
}


//----------------------------------------------------------
// �����֧��USB Audioģʽ��ϵͳ�����PC����״̬������ǿU�̼����ԡ�
BOOL IsAudDetectDisable(VOID)
{
#if (defined(FUNC_AUDIO_EN) || defined(FUNC_READER_EN) || defined(FUNC_AUDIO_READER_EN))
	return FALSE;
#else
	return TRUE;
#endif
}

