#include "syscfg.h"
#include "sysctrl.h"
#include "device_audio.h"
#include "usb_audio_ctrl.h"
#include "card_reader_ctrl.h"
#include "playctrl.h"
#include "device.h"
#include "rtc_ctrl.h"
#include "breakpoint.h"
#include "linein_ctrl.h"
#include "sleep.h"
#include "radio_app_interface.h"
#include "user_interface.h"
#include "debug.h"
#include "sys_on_off.h"
#include "radio_ctrl.h"
#include "display.h"
#include "key.h"
#include "spi_fs.h"
#include "otp_play.h"
#include "touch_key.h"
#include "pt231x.h"
#include "bluetooth_ctrl.h"
#include "slave_ctrl.h"
#include "bt.h"
#include "npca110x.h"


#ifdef FUNC_QUICK_RESPONSE_EN 
BOOL IsSwitchMode;
BYTE CurMode;
#endif
#ifdef FUNC_NPCA110X_EN
extern BOOL isDefaultBass;
#endif

#ifdef FUNC_BT_CHAN_AUTO
typedef enum _CHANNEL_TYPE{
	CHANNEL_TYPE_NONE 	= 0,
	CHANNEL_TYPE_SD		= 1,
	CHANNEL_TYPE_FM		= 2,
	CHANNEL_TYPE_BT		= 3,

}CHANNEL_TYPE;

BYTE ChannelType = CHANNEL_TYPE_NONE;

#endif


BOOL isDealSetBass = FALSE;
static 	USHORT i = 0;

#ifdef   FUNC_USB_CHARGE_EN
//��ǰϵͳ���Ƿ�������Apple�豸��־
BOOL     AppleDeviceFlag = FALSE;	
//gMassStorInvalidVid = 0x05ACʱ����Ipod�豸��Ϊ����豸
WORD	 gMassStorInvalidVid = 0x05AC;
#endif

//#define FUNC_PAUSE_DURING_DEVTRY	//�豸�����л�ʱֹͣ���ţ���ʾLOD.

extern BOOL gDeviceError;
extern SYS_MODE gSysMode;	// define in library.
extern BOOL IsCardLinkFlag; //for audio
extern BOOL gIsUsbRemove;//for usb Reconnect in mode switch 
extern BOOL IsUsbLinkFlag; 

#define PLAY_WATCH_TIME				10000
extern VOID SongPlayTimePause(VOID);
extern VOID SongPlayTimeResume(VOID);
extern TIMER gPlayWatchTimer;
//extern TIMER VolumeUpTimer;
extern TIMER CardDctTimer;
extern VOID BassLed_CallBak(VOID);
#define SET_BASS_TIME	1	
TIMER SetBassTimer;

//#ifdef FUNC_USB_MSC_SN_EN
//BYTE gUDiskSN[MAX_USB_SN_LEN];
//#endif

#ifdef	FUNC_DEVICE_FORCEPAUSE_EN
BOOL gDevicePlayForcePauseFlag = FALSE;	//ǿ����ͣ�豸����״̬
#endif

#ifdef	FUNC_HEADPHONE_DETECT_EN
static TIMER sHeadPhoneCheckTimer;
#endif

#ifdef FUNC_POWER_ON_AUDIO_EN
BOOL gUSBChargePowerOnFlag = TRUE;	//�ػ�״̬��USB���������ϵͳ������־
#endif

#ifdef FUNC_STANDBY_EN
SYS_MODE gSystemExecState;
#endif

#ifdef FUNC_SNOOZE_EN
#define SNOOZE_TIME		300000	// 5mins

static TIMER sSnoozeTimer;
static BOOL sSnoozeOnFlag = FALSE;
static BOOL sAlarmPowerOnFlag = FALSE;
#endif

#ifdef FUNC_BEEP_SOUND_EN 	
extern 	TIMER gPushKeyBeepTimer;
#endif


////////////////////////////////////////////////////
///////////////////////////////////////////////////
//ǿ�ƽ���ĳ��ģʽ
//����ԭģʽ�˳�ʱ���ƺ�����
static VOID GotoMode(BYTE Mode)
{
#ifdef FUNC_TOUCH_KEY_EN
	TouchKeyScanInit();
#endif

	switch(gSys.SystemMode)		//ԭģʽ
	{
#ifdef FUNC_USB_EN
		case SYS_MODE_USB:		//�뿪USBģʽ
			//ֹͣ����
			PlayCtrlEnd();
			break;
#endif

#ifdef FUNC_CARD_EN
		case SYS_MODE_SD:		//�뿪SDģʽ
			//ֹͣ����
			PlayCtrlEnd();
			break;
#endif

#ifdef FUNC_SPI_PLAY_EN
		case SYS_MODE_SPIDEV:		//�뿪SPIģʽ
			//ֹͣ����
			PlayCtrlEnd();
			break;
#endif

#ifdef FUNC_RADIO_EN
		case SYS_MODE_RADIO: 		//�뿪RADIOģʽ
			if(Mode == SYS_MODE_RADIO)
			{
				break;
			}	
			RadioCtrlDeinit();				
//#if (FM_IO_TYPE == DAC_OUT_LR)
//			DBG(("LeaveRad,DacInit\n"));
//			InDacInit(FALSE);
//#endif
			SysClkDivClr();	
			InDacSetVmid(FALSE);
			break;
#endif

#ifdef FUNC_LINEIN_EN
		case SYS_MODE_LINEIN:	//�뿪line-inģʽ
			if(Mode == SYS_MODE_LINEIN)
			{
				break;
			}
			LineinIODeinit();
			//SysClkDivClr();	
			break;
#endif
#ifdef FUNC_BLUETOOTH_CSR_EN
		case SYS_MODE_BLUETOOTH:	
			if(Mode == SYS_MODE_BLUETOOTH)
			{
				break;
			}
			BluetothCtrlEnd();
			//SysClkDivClr();	
			break;
#endif
#if 0//def FUNC_BLUETOOTH_EN
		case SYS_MODE_BLUETOOTH:
			if (Mode == SYS_MODE_BLUETOOTH)
			{
				break;
			}
			BluetothCtrlEnd();
			//SysClkDivClr();
			break;
#endif

		case SYS_MODE_SLEEP:	//�뿪SLEEPģʽ
			
			break;

#ifdef FUNC_RTC_EN
		case SYS_MODE_RTC:		//�뿪RTCģʽ

#ifdef FUNC_POWER_AMPLIFIER_EN
			if (Mode == SYS_MODE_RTC)
			{
				break;
			}
			if (Mode != SYS_MODE_RADIO)
			{
				ABPowerAmplifierOn();
			}
#endif	

#ifdef FUNC_USB_VOLUP_EN
			if (Mode == SYS_MODE_RTC)
			{
				break;
			}
			if (Mode != SYS_MODE_USB)
			{
				USB_VOLUP_Off();
			}
#endif		
			break;
#endif
			
#if (defined(FUNC_AUDIO_EN) || defined(FUNC_READER_EN) || defined(FUNC_AUDIO_READER_EN))
		case SYS_MODE_AUDIO:	//�뿪USB����ģʽ
		case SYS_MODE_READER:		//�뿪USB������ģʽ
		case SYS_MODE_AUDIO_READER:	//�뿪USBһ��ͨģʽ
			DeviceAudioCtrlEnd();
			DeviceStorCtrlEnd();
			break;
#endif

		default:
			break;
	}
	
#ifdef FUNC_STANDBY_EN
	if(Mode != SYS_MODE_STANDBY)
	{
		gSystemExecState = Mode;
	}
#endif

	gSys.SystemMode = Mode;
	gSysMode = Mode;		//�����ڵײ㣬Ϊ�˱���ײ�ֱ������gSys.SystemMode����(��ֹӦ�ò�����޸�gSys�ṹ��Ӱ�쵽�ײ�)��
	MuteOn(TRUE, TRUE);	//�ر�DAC��LineIn��FMInͨ�������
	gSys.MuteFg = FALSE;
	
	MessageInit();

#ifdef FUN_SYSTEM_POWEROFF_WAIT_TIME
	TimeOutSet(&gSysTimeOffState.DisTimer, 0);	
#endif

#ifdef FUNC_DISP_EN
	DispMute();	//��ȷ�ָ�Mute��ʾ״̬�������Mute״̬���л��豸��ɵ���ʾ����
	DispDev();
#endif
}  


#ifdef FUNC_STANDBY_EN
//�����л���Standbyģʽ
static BOOL TryGotoModeStandby(VOID)
{	
	DBG(("TryGotoModeStandby()\n"));

	ModuleClkEn(MASK_ALL_CLOCK);
	SysClkDivClr();	
	
	GotoMode(SYS_MODE_STANDBY);	
	
	InDacChannelSel(DAC_CH_NONE);	
	InDacPowerDown(FALSE);	

#ifdef FUNC_POWER_AMPLIFIER_EN
	ABPowerAmplifierOff();
#endif

#ifdef FUNC_SNOOZE_EN
	sAlarmPowerOnFlag = FALSE;
#endif

	return TRUE;
}
#endif


//�����л���IDLEģʽ
static BOOL TryGotoModeIdle(VOID)
{
	DBG(("TryGotoModeIdle()\n"));
	GotoMode(SYS_MODE_NONE);

	InDacChannelSel(DAC_CH_NONE);

#ifdef FUNC_POWER_AMPLIFIER_EN
	ABPowerAmplifierOff();
#endif

	return TRUE;
}


#ifdef FUNC_CARD_EN
//�����л���SD��ģʽ
static BOOL TryGotoModeSD(VOID)
{
	BOOL TempFlag = FALSE;

#ifdef FUNC_BT_CHAN_AUTO
	ChannelType = CHANNEL_TYPE_SD;
#endif
		
	DBG1(("TryGotoModeSD()\n"));
	
#ifdef FUNC_POWER_ON_AUDIO_EN
	if(gUSBChargePowerOnFlag == TRUE)
	{
		return FALSE;
	}
#endif
	
	if(IsCardLinkFlag == TRUE)
	{
		MuteOn(TRUE, TRUE); //���SD��USB֮��ֱ���л���ͣһ���ٳ�֮ǰ�豸������������
		TempFlag = TRUE;
	}

	//�ȴ�SD����U���豸����
	if(!IsCardLink())
	{
		
		DBG1(("IsCardLink() FALSE!\n"));
		IsCardLinkFlag = FALSE;
		return FALSE;
	}
	DBG1(("IsCardLink() OK!\n"));

	

   	ModuleClkEn(MASK_ALL_CLOCK);
	SysClkDivClr();	

	gPlayCtrl.State = PLAY_STATE_STOP;
	
#ifdef FUNC_DISP_EN
	DispLoad();
#endif
	if(TempFlag == FALSE)
	{
		MuteOn(TRUE, TRUE); //���SD��USB֮��ֱ���л���ͣһ���ٳ�֮ǰ�豸������������
	}

	//WaitMs(50);

//	if(!CardInit())	
	{
		//WaitMs(50);
		if(!CardInit())	
		{
			DBG1(("CardInit() error!\n"));
			return FALSE;
		}
		
	}
	DBG1(("Card OK!\n"));

	GotoMode(SYS_MODE_SD);
#ifdef AU6210K_NR_D_8_CSRBT
	NPCA110X_ADC_Input_CH_Select(INPUT_CH2);
#endif
	InDacChannelSel(DAC_CH_DAC);
	
#ifdef FUNC_SPI_KEY_SOUND_EN	
	SPI_PlaySelectNum(SPIPLAY_SONG_PLAY_TF, 0);
#endif
	
	DBG1(("FSInit(DEV_ID_SD)\n"));
	if(!FSInit(DEV_ID_SD))
	{
		DBG1(("FSInit() error!\n"));
		return FALSE;
	}

	DBG1(("*******SD MODE********\n"));
	PlayCtrlInit();	 
#ifdef FUNC_TOUCH_KEY_EN
	TR1 = 1;
#endif
	return TRUE;
}
#endif


#ifdef FUNC_USB_EN
//�����л���USBģʽ
static BOOL TryGotoModeUSB(VOID)
{	
	BOOL TempFlag = FALSE;
	
	DBG(("TryGotoModeUSB()\n"));
	
#ifdef FUNC_POWER_ON_AUDIO_EN
	if(gUSBChargePowerOnFlag == TRUE)
	{
		return FALSE;
	}
#endif	
	
	if(IsUsbLinkFlag == TRUE)
	{
		MuteOn(TRUE, TRUE); //���SD��USB֮��ֱ���л���ͣһ���ٳ�֮ǰ�豸������������
		TempFlag = TRUE;
	}
	
	//U�̵������Ӽ��
	if(!IsUSBLink())
	{
		DBG(("IsUSBLink() FALSE!\n"));
		return FALSE;
	}
	DBG(("IsUSBLink() OK!\n"));

	if(TempFlag == FALSE)
	{
		MuteOn(TRUE, TRUE); //���SD��USB֮��ֱ���л���ͣһ���ٳ�֮ǰ�豸������������
	}	
	
	ModuleClkEn(MASK_ALL_CLOCK);	
	SysClkDivClr();	

#ifdef	FUNC_USB_CHARGE_EN
	//��ֹTryGotoModeUSB()��������ε���ʱ������AppleDevice�����ʾ���
	if(AppleDeviceFlag)
	{
		return FALSE;
	}
#endif

#ifdef FUNC_DISP_EN
	DispLoad();
#endif

//	DBG(("IsUSBLink() OK!\n"));
	if(!HostStorInit())	
	{
		DBG(("HostStorInit() error!\n"));
#ifdef	FUNC_USB_CHARGE_EN
		//if(IsInvalidUsbDevice())
		if(gLibUsbInfo.VID == 0x05AC)
		{
			extern BYTE UsbSetConfiguration(BYTE configuration);
			AppleDeviceFlag = TRUE;							
			UsbSetConfiguration(1);
			DBG(("USB��翪ʼ!\n"));
		}
#endif
		return FALSE;
	}
	DBG(("USB OK!\n"));

	GotoMode(SYS_MODE_USB);
	InDacChannelSel(DAC_CH_DAC);
	
#if 0//def FUNC_SPI_KEY_SOUND_EN	
	SPI_PlaySelectNum(SPIPLAY_SONG_HAVE_CALL, 0);
#endif

	DBG(("FSInit(DEV_ID_USB)\n"));
	if(!FSInit(DEV_ID_USB))
	{
		DBG(("FSInit() error!\n"));
		return FALSE;
	}

	DBG(("*******USB MODE********\n"));
	PlayCtrlInit();
	
#ifdef FUNC_TOUCH_KEY_EN
	TR1 = 1;
#endif
	return TRUE;
}
#endif


#ifdef FUNC_RADIO_EN
//�����л���RADIOģʽ
static BOOL TryGotoModeRADIO(VOID)
{		
	DBG(("TryGotoModeRADIO()\n"));
#ifdef FUNC_BT_CHAN_AUTO
	ChannelType = CHANNEL_TYPE_FM;
#endif



#ifdef FUNC_POWER_ON_AUDIO_EN
	if(gUSBChargePowerOnFlag == TRUE)
	{
		return FALSE;
	}
#endif

	//U�̵������Ӽ��  
	if(!IsRadioLink())
	{
		DBG(("IsRadioLink() FALSE!\n"));
		return FALSE;
	}
	DBG(("IsRadioLink() OK!\n"));
	
	GotoMode(SYS_MODE_RADIO);
	
#ifdef FUNC_SPI_KEY_SOUND_EN	
	SPI_PlaySelectNum(SPIPLAY_SONG_PLAY_FM, 0);
#endif
 
	RadioPowerOn();
	InDacSetVmid(TRUE);

	//FM set
#ifdef FM_DAC_OUT_D3D4
	//���Ҫ��FM��Ƶ��D3��D4�����������Ҫ����������	
	SetGpioRegBit(GPIO_FMD_OE, 0x04);
	ClrGpioRegBit(GPIO_FMD_IE, 0x04);
	SetGpioRegBit(GPIO_FMD_OUT, 0x04);

	ClrGpioRegBit(GPIO_D_PU, 0x18);
	SetGpioRegBit(GPIO_D_PD, 0x18);
	SetGpioRegBit(GPIO_FMA_EN, 0x03);	//Ƭ��FMģ�����Ƶ�ź����ӵ�D3��D4

	//��ѡ����Ƶ�ź����뵽оƬ�ڲ���Ȼ��� DAC�����ʽ
	//��Line inͨ�����뵽оƬ�ڲ�
	InDacChannelSel(DAC_CH_LINEIN); 

	//��E0 E1���뵽оƬ�ڲ�
	//SET_E0_ANALOG_IN();	//��E0E1����Ϊģ��˿�
	//SET_E1_ANALOG_IN();		
	//InDacChannelSel(DAC_CH_E0E1);		

	RadioCtrlInit();

#else
	//Ĭ��ѡ��Ƭ��FM��Ƶͨ·
	InDacChannelSel(DAC_CH_ONCHIP_FM);
#endif

//FM 	
#ifdef FUNC_PWM_EN
	ModuleClkDis(MASK_ALL_CLOCK & (~MSK_PWM_CLOCK));
#else
	ModuleClkDis(MASK_ALL_CLOCK);
#endif
// �л���FM��ʱ�ӷ�Ƶ����
// O20����ǰоƬ��һ����������ʱ��Դ��Ĭ��������ΪRC clk��
#ifndef	FUNC_SLAVE_UART_EN
	SysClkDivSet(CLK_DIV_RATE);
#endif
//#if (FM_IO_TYPE != FM_ONCHIP_IP)	
//	RadioCtrlInit();
//#endif

	RadioEnterInit();
#ifdef FUNC_TOUCH_KEY_EN
	TR1 = 1;
#endif
	return TRUE;
}
#endif


#ifdef FUNC_LINEIN_EN	  
//�����л���line-inģʽ
static BOOL TryGotoModeLINEIN(VOID)
{
	DBG(("TryGotoModeLINEIN()\n"));
	
#ifdef FUNC_POWER_ON_AUDIO_EN
	if(gUSBChargePowerOnFlag == TRUE)
	{
		return FALSE;
	}
#endif	

	//U�̵������Ӽ��
	if(!IsLineInLink())
	{
		DBG(("IsLineInLink() FALSE!\n"));
		return FALSE;
	}
   	ModuleClkEn(MASK_ALL_CLOCK);
	SysClkDivClr();	
	DBG(("IsLineInLink() OK!\n"));

	GotoMode(SYS_MODE_LINEIN);

#ifdef FUNC_SPI_KEY_SOUND_EN	
	SPI_PlaySelectNum(SPIPLAY_SONG_PLAY_AUX, 0);
#endif	
#ifdef AU6210K_NR_D_8_CSRBT
	NPCA110X_ADC_Input_CH_Select(INPUT_CH0);
	InDacChannelSel(LINEIN_IO_TYPE);
#else


	//Ĭ��LINE_INģʽ����Դѡ��Ƭ��LINE_INͨ·
	//LINE_INģʽ����ԴҲ����ѡ��E0��E1ͨ·, ����Ҫ����������	
/*#if (LINEIN_IO_TYPE == DAC_CH_E0_L)
	SET_E0_ANALOG_IN();	//��E0����Ϊģ��˿�
#elif (LINEIN_IO_TYPE == DAC_CH_E1_R)
	SET_E1_ANALOG_IN();	//��E1����Ϊģ��˿�
#elif (LINEIN_IO_TYPE == DAC_CH_E0E1)
*/
#if (LINEIN_IO_TYPE == DAC_CH_E0E1)
	SET_E0_ANALOG_IN();	//��E0E1����Ϊģ��˿�
	SET_E1_ANALOG_IN();	
#endif

	InDacChannelSel(LINEIN_IO_TYPE);


#endif	
	LineInCtrlInit();

#ifdef FUNC_TOUCH_KEY_EN
	TR1 = 1;
#endif
	return TRUE;
}
#endif

#ifdef FUNC_BLUETOOTH_CSR_EN	  
//�����л���line-inģʽ
static BOOL TryGotoBluetooth(VOID)
{
	DBG(("TryGotoModeLINEIN()\n"));

	
#ifdef FUNC_BT_CHAN_AUTO
	ChannelType = CHANNEL_TYPE_BT;
#endif
	
#ifdef FUNC_POWER_ON_AUDIO_EN
	if(gUSBChargePowerOnFlag == TRUE)
	{
		return FALSE;
	}
#endif	

   	ModuleClkEn(MASK_ALL_CLOCK);
	SysClkDivClr();	
	DBG1(("IsLineInLink() OK!\n"));

	GotoMode(SYS_MODE_BLUETOOTH);

#ifdef FUNC_SPI_KEY_SOUND_EN	
	SPI_PlaySelectNum(SPIPLAY_SONG_PLAY_BT, 0);
#endif	
#ifndef FUNC_PT231X_EN
	//Ĭ��LINE_INģʽ����Դѡ��Ƭ��LINE_INͨ·
	//LINE_INģʽ����ԴҲ����ѡ��E0��E1ͨ·, ����Ҫ����������	
/*#if (BLUETOOTH_CJ_IO_TYPE == DAC_CH_E0_L)
	SET_E0_ANALOG_IN();	//��E0����Ϊģ��˿�
#elif (BLUETOOTH_CJ_IO_TYPE == DAC_CH_E1_R)
	SET_E1_ANALOG_IN();	//��E1����Ϊģ��˿�
#elif (BLUETOOTH_CJ_IO_TYPE == DAC_CH_E0E1)
	SET_E0_ANALOG_IN();	//��E0E1����Ϊģ��˿�
	SET_E1_ANALOG_IN();	
#endif*/

	//InDacChannelSel(BLUETOOTH_CJ_IO_TYPE);
#endif	
	BluetoothCtrlInit();

#ifdef FUNC_TOUCH_KEY_EN
	TR1 = 1;
#endif
	return TRUE;
}
#endif
#if 0//def FUNC_BLUETOOTH_EN
static BOOL TryGotoBluetooth()
{
	DBG(("TryGotoModeBLUETOOTH()\n"));
#ifdef BLUETOOTH_ALWAYS_OPEN
	if (!gBTConnected)//����δ������ʱ������������״̬
	{
		DBG(("Bluetooth disconnected, don't entry bluetooth mode.\n"));
		return FALSE;
	}
#endif
	
	MuteOn(TRUE, TRUE);//�Ƚ���������ȥPOPO����BluetoothCtrlInit()�л��ٴ���������
	ModuleClkEn(MASK_ALL_CLOCK);
	SysClkDivClr();//����ϵͳ״̬
	GotoMode(SYS_MODE_BLUETOOTH);	//����BLUETOOTHģʽ

#ifdef RTC_SHOWTIME_ANYMODE
	DispTimeOnOff(FALSE);
#endif

#if defined(AU6210K_JLH_JH82B)
	InDacChannelSel(DAC_CH_NONE);
#endif

#if defined(AU6210K_NR_D_8_CSRBT)
	InDacChannelSel(BLUETOOTH_CJ_IO_TYPE);
#endif

	return BluetoothCtrlInit();
}
#endif


////�����л���RTCģʽ
//static BOOL TryGotoModeRTC(VOID)
//{
//	DBG(("TryGotoModeRTC()\n"));
//	GotoMode(SYS_MODE_RTC);
//
//	InDacChannelSel(DAC_CH_NONE);
//#ifdef FUNC_POWER_AMPLIFIER_EN
	//ABPowerAmplifierOff();
//#endif
//	return TRUE;
//}


#ifdef FUNC_AUDIO_EN
//�����л���AUDIOģʽ
static BOOL TryGotoModeAUDIO(VOID)
{
#ifdef FUNC_POWER_ON_AUDIO_EN	
	static TIMER Timer;
#endif

	DBG(("TryGotoModeAUDIO()\n"));	
	
#ifdef FUNC_POWER_ON_AUDIO_EN
	//����PC����ֱ�ӽ���Audio ģʽ
	if(gUSBChargePowerOnFlag == TRUE)
	{
		TimeOutSet(&Timer, 1500);	//����ͻ�ϣ������PC������Ҫֱ�ӽ���Audioģʽ,��ʱ��������1.4s
		while(!IsTimeOut(&Timer))
		{
			if(IsPcLink())
			{
				break;
			}
		}			
	}
	gUSBChargePowerOnFlag = FALSE;
#endif
	
	//PC���Ӽ��
	IsPcInsert();	//Ϊ��PC���ӿ�������¼��PC ״̬
	
	if(!IsPcLink())
	{
		DBG(("Audio,IsPcLink() FALSE!\n"));
		return FALSE;
	}

#ifdef FUNC_DISP_EN	
	DispAudio();
#endif	

	MuteOn(TRUE, TRUE);	//������� PC�����л��ٳ�֮ǰ�豸������������

	ModuleClkEn(MASK_ALL_CLOCK);		
	SysClkDivClr();

	GotoMode(SYS_MODE_AUDIO);
	InDacChannelSel(DAC_CH_DAC);
	
	DeviceAudioSetMode(USB_DEVICE_AUDIO);
	DeviceAudioCtrlInit();
#ifdef FUNC_TOUCH_KEY_EN
	TR1 = 1;
#endif

//	DBG(("Usb audio mode ok!\n"));
	return TRUE;
}
#endif


#ifdef FUNC_READER_EN
//�����л���READERģʽ
static BOOL TryGotoModeREADER(VOID)
{	
	DBG(("TryGotoModeREADER()\n"));
	//PC���Ӽ��
	if(!IsPcLink())
	{
		DBG(("Reader,IsPcLink() FALSE!\n"));
		return FALSE;
	}

	MuteOn(TRUE, TRUE);	//������� PC�����л��ٳ�֮ǰ�豸������������

#ifdef FUNC_DISP_EN
	DispLoad();
#endif

	ModuleClkEn(MASK_ALL_CLOCK);		
	SysClkDivClr();

	GotoMode(SYS_MODE_READER);
	InDacChannelSel(DAC_CH_DAC);

	DeviceAudioSetMode(USB_DEVICE_READER);
#ifdef FUNC_CARD_EN 
	DeviceStorCtrlInit();
#endif
#ifdef FUNC_TOUCH_KEY_EN
	TR1 = 1;
#endif
//	DBG(("Usb reader mode ok!\n"));
	return TRUE;
}
#endif


#ifdef FUNC_AUDIO_READER_EN
//�����л���AUDIO_READERģʽ
static BOOL TryGotoModeAUDIO_READER(VOID)
{	
#ifdef FUNC_POWER_ON_AUDIO_EN	
	static TIMER Timer;
#endif

	DBG(("TryGotoModeAUDIO_READER()\n"));	
	
#ifdef FUNC_POWER_ON_AUDIO_EN
	//����PC����ֱ�ӽ���Audio ģʽ
	if(gUSBChargePowerOnFlag == TRUE)
	{
		TimeOutSet(&Timer, 1500);	//����ͻ�ϣ������PC������Ҫֱ�ӽ���Audioģʽ,��ʱ��������1.4s
		while(!IsTimeOut(&Timer))
		{
			if(IsPcLink())
			{
				break;
			}
		}			
	}
	gUSBChargePowerOnFlag = FALSE;
#endif

	//PC���Ӽ��
	IsPcInsert();	//Ϊ��PC���ӿ�������¼��PC ״̬

	if(!IsPcLink())
	{
		DBG(("Audio,IsPcLink() FALSE!\n"));
		return FALSE;
	}	

#ifdef FUNC_DISP_EN	
	DispAudio();
#endif	

	MuteOn(TRUE, TRUE);	//������� PC�����л��ٳ�֮ǰ�豸������������
	
	ModuleClkEn(MASK_ALL_CLOCK);		
	SysClkDivClr();

	GotoMode(SYS_MODE_AUDIO_READER);
	InDacChannelSel(DAC_CH_DAC);
	
	DeviceAudioSetMode(USB_DEVICE_AUDIO_READER);
	DeviceAudioCtrlInit();
#ifdef FUNC_CARD_EN 
	DeviceStorCtrlInit();
#endif
#ifdef FUNC_TOUCH_KEY_EN
	TR1 = 1;
#endif
//	DBG(("Usb audio reader mode ok!\n"));
	return TRUE;
}
#endif


#ifdef	FUNC_SPI_UPDATE_EN
extern BYTE gIsMVFFileFind;
#endif
#ifdef FUNC_SPI_PLAY_EN
//�����л���spi-devģʽ
static BOOL TryGotoModeSpiDev()
{
	DBG(("->TryGotoSPIDev!\n"));

	if(!IsSpiFlashHandleGot())
	{
		DBG(("No SPI Dev!\n"));
		return FALSE;
	}
#ifdef FUNC_DISP_EN
	DispLoad();
#endif

	ModuleClkEn(MASK_ALL_CLOCK);		
	SysClkDivClr();

	GotoMode(SYS_MODE_SPIDEV);
	InDacChannelSel(DAC_CH_DAC);

#ifdef	FUNC_SPI_UPDATE_EN
	gIsMVFFileFind = 0;
#endif

	FSInit(DEV_ID_SPI);
	PlayCtrlInit();
	gSongInfo.SongType = SONG_TYPE_MP3; 
	gPlayCtrl.File.FileType = FILE_TYPE_MP3;
	gPlayCtrl.FileNum = 1;
	DBG(("<-TryGotoSPIDev!\n"));
#ifdef FUNC_TOUCH_KEY_EN
	TR1 = 1;
#endif	
	return TRUE;
}
#endif


#ifdef FUNC_OTP_PLAY_EN
static BOOL TryGotoModeOtpDev(VOID)
{
	DBG((">>TryGotoModeOtpDev()\n"));
#ifdef FUNC_DISP_EN
	DispLoad();
#endif

	ModuleClkEn(MASK_ALL_CLOCK);		
	SysClkDivClr();
		
	GotoMode(SYS_MODE_OTPDEV);
	InDacChannelSel(DAC_CH_DAC);

	FSInit(DEV_ID_OTP);
	PlayCtrlInit();
	gSongInfo.SongType = SONG_TYPE_MP3; 
	gPlayCtrl.File.FileType=FILE_TYPE_MP3;
	gPlayCtrl.FileNum = 1;
#ifdef FUNC_TOUCH_KEY_EN
	TR1 = 1;
#endif
	return TRUE;
}


//����ѡ���OTP����
static VOID OTP_PlaySelectNum(BYTE PlayNum)
{	
	BYTE SystemMode, Volume;

	SystemMode = gSys.SystemMode;	
	Volume = gSys.Volume;	
	
	DBG(("<-OTPSongPlayStart()\n"));	
	PlayNum = 1;	
	gSys.Volume = VOLUME_MAX;
#ifdef AU6210K_NR_D_8_CSRBT
	NPCA110X_DAC1_Set_Volume_and_Mute(gSys.Volume);
	InDacSetVolume(gVolArr[32], gVolArr[32]);
#else
	InDacSetVolume(gVolArr[gSys.Volume], gVolArr[gSys.Volume]);
#endif	
	TryGotoModeOtpDev();
	UnMute();	
	while(1)
	{    
		FeedWatchDog();	
		if(FileEOF(&gPlayCtrl.File) && IsFifoEmpty())
		{
			DBG(("Song play end!\n"));
			//�ȴ�PCM����ȫ����ȡ��
			break;
		}
		if(!SongPlayDo())
		{
			DBG(("PlayStatePlayOp,!SongPlayDo()\n"));			
			break;
		}    	
	}
	DBG(("<-OTPSongPlayStop()\n"));
	PlayCtrlEnd();	
	InDacChannelSel(DAC_CH_NONE);
	gSys.SystemMode = SystemMode;
	gSys.Volume = Volume;
}
#endif


//ϵͳ�ϵ翪��������
VOID SystemPowerOnSound(VOID)
{	
#ifdef	FUNC_OTP_KEY_SOUND_EN
	OTP_PlaySelNum(OTPPLAY_NUM_POWERON, 0);
#endif

#ifdef FUNC_SPI_FLASH_POWERON_SOUND
	SPI_PlaySelectNum(SPIPLAY_SONG_POWER_ON_SOUND1, 0);
#endif
}


//�����л���ָ���豸
static BOOL TryGotoMode(BYTE Mode)
{	
#ifdef	FUNC_QUICK_RESPONSE_EN
	//�����ǿ���л�ģʽ
	if(IsSwitchMode)
	{
		DBG(("300\n"));
		return TRUE;
	}

	CurMode = Mode;
#endif

	switch(Mode)
	{
#ifdef FUNC_SLAVE_UART_EN
		case SYS_MODE_NONE:
			return TryGotoModeIdle();
#endif		
	
#ifdef FUNC_USB_EN
		case SYS_MODE_USB:
			return TryGotoModeUSB();
#endif

#ifdef FUNC_CARD_EN
		case SYS_MODE_SD:
			return TryGotoModeSD();
#endif

#ifdef FUNC_LINEIN_EN
		case SYS_MODE_LINEIN:
			return TryGotoModeLINEIN();
#endif
#ifdef FUNC_BLUETOOTH_CSR_EN
		case SYS_MODE_BLUETOOTH:
			return TryGotoBluetooth();
#endif
#ifdef FUNC_BLUETOOTH_EN
		case SYS_MODE_BLUETOOTH:
			return TryGotoBluetooth();
#endif


#ifdef FUNC_RADIO_EN
		case SYS_MODE_RADIO:
			DBG(("TryGotoModeRADIO\n"));
			return TryGotoModeRADIO();
#endif

#ifdef FUNC_AUDIO_EN
		case SYS_MODE_AUDIO:
			DBG(("TryGotoModeAUDIO\n"));
			return TryGotoModeAUDIO();
#endif

#ifdef FUNC_READER_EN
		case SYS_MODE_READER:
			DBG(("TryGotoModeREADER\n"));
			return TryGotoModeREADER();
#endif

#ifdef FUNC_AUDIO_READER_EN
		case SYS_MODE_AUDIO_READER:
			DBG(("TryGotoModeAUDIO_READER\n"));
			return TryGotoModeAUDIO_READER();
#endif

#ifdef FUNC_SPI_PLAY_EN
		case SYS_MODE_SPIDEV:
			DBG(("TryGotoModeOtpDevSPI\n"));
			return TryGotoModeSpiDev();
			break;
#endif

#ifdef FUNC_OTP_PLAY_EN
		case SYS_MODE_OTPDEV:
			DBG(("TryGotoModeOtpDev001\n"));
			return TryGotoModeOtpDev();
			break;
#endif

#ifdef FUNC_STANDBY_EN
		case SYS_MODE_STANDBY:
			return TryGotoModeStandby();
			break;
#endif

		default:
#ifdef FUNC_USB_EN
			return TryGotoModeUSB();
#elif defined(FUNC_CARD_EN)
			return TryGotoModeSD();
#else
			return FALSE;
#endif
	}
}


//�����л���Modeģʽ��������ɹ����������һ��ģʽ��
static VOID TryGotoNextMode(BYTE Mode)
{
	BYTE i;
	BYTE ModeIndex;

	//�����������豸�л�ʱ��˳�򣬸�˳����ѭ����
	static BYTE ModeOrder[] = 
	{
#ifdef FUNC_BLUETOOTH_EN
		SYS_MODE_BLUETOOTH, 
#endif
	

#ifdef FUNC_BLUETOOTH_CSR_EN
		SYS_MODE_BLUETOOTH, 
#endif

#ifdef FUNC_USB_EN
		SYS_MODE_USB,
#endif	

#ifdef FUNC_CARD_EN
		SYS_MODE_SD,
#endif


#ifdef FUNC_LINEIN_EN
		SYS_MODE_LINEIN,
#endif	

#ifdef FUNC_RADIO_EN
		SYS_MODE_RADIO,
#endif

 
#ifdef FUNC_AUDIO_EN
		SYS_MODE_AUDIO,
#endif

#ifdef FUNC_READER_EN
		SYS_MODE_READER,
#endif

#ifdef FUNC_AUDIO_READER_EN
		SYS_MODE_AUDIO_READER,
#endif

#ifdef FUNC_SPI_PLAY_EN
		SYS_MODE_SPIDEV,
#endif

#ifdef FUNC_OTP_PLAY_EN
		SYS_MODE_OTPDEV,
#endif
	};

	for(ModeIndex = 0; ModeIndex < sizeof(ModeOrder); ModeIndex++)
	{
		if(Mode == ModeOrder[ModeIndex])
		{
		 	break;
		}
	}

	for(i = 0; i < sizeof(ModeOrder); i++)
	{
		FeedWatchDog();
		ModeIndex++;
		if(ModeIndex >= sizeof(ModeOrder))
		{
			ModeIndex = 0;
		} 		
		
#ifdef AU6210K_XLX_ALD800
		if(ModeOrder[ModeIndex] == SYS_MODE_LINEIN)
		{
			if(TryGotoMode(SYS_MODE_BLUETOOTH))
			{
				return;
			}
		}
			

#endif
		if(TryGotoMode(ModeOrder[ModeIndex]))
		{
			return;
		}
	}

	TryGotoModeIdle();
}


//�����л���Modeģʽ��������ɹ����������һ��ģʽ��
static VOID TryGotoValidMode(BYTE Mode)
{
	if(!TryGotoMode(Mode))
	{
		TryGotoNextMode(Mode);
	}
}


//�����л���Modeģʽ��������ɹ��������ԭ����ģʽ��
static VOID TryGotoValidModeReturn(BYTE Mode)
{
	BYTE OldMode = gSys.SystemMode;

#ifdef	FUNC_DEVICE_FORCEPAUSE_EN
	gDevicePlayForcePauseFlag = FALSE;
#endif

	if(!TryGotoMode(Mode))
	{
		if(!TryGotoMode(OldMode))
		{
			TryGotoNextMode(SYS_MODE_USB);
		}
	}
}


VOID DevCtrlInit(VOID)
{
	static TIMER Timer;

//	TimeOutSet(&VolumeUpTimer, 0);
	TimeOutSet(&gPlayWatchTimer, 0);
	
#ifdef	FUNC_HEADPHONE_DETECT_EN
	TimeOutSet(&sHeadPhoneCheckTimer, 0);
#endif
	
#ifdef FUNC_BEEP_SOUND_EN
	TimeOutSet(&gPushKeyBeepTimer, 0);
#endif	

#ifdef FUNC_SNOOZE_EN
	TimeOutSet(&sSnoozeTimer, 0);
#endif	

#ifdef FUNC_LED_LINEIN_MULTIPLE_EN
	TimeOutSet(&gLineInCheckWaitTimer, 0);
#endif

#ifndef FUNC_BREAK_POINT_EN
	gSys.SystemMode = SYS_MODE_NONE;
	gSys.Volume = VOLUME_INIT;
	gPlayCtrl.Eq = DECD_EQ_NORMAL;
	gPlayCtrl.RepeatMode = REPEAT_ALL;
#ifdef FUNC_FOLDER_EN
	gPlayCtrl.FolderEnable = TRUE;
#else
	gPlayCtrl.FolderEnable = FALSE;
#endif
#endif

#ifdef FUNC_QUICK_RESPONSE_EN
	IsSwitchMode = FALSE;
#endif

#ifdef FUNC_USB_EN
	DBG(("InitUsb()\n"));
	HostInit();
#endif

#ifdef FUNC_CARD_EN
#ifdef CARD_BUS_A3A4A5_EN
	SET_CARD_TO_A3A4A5();
#endif
#ifdef	CARD_BUS_A0E2E3_EN
	SET_CARD_TO_A0E2E3();
#endif
#ifdef	CARD_BUS_D5D6D7_EN
	SET_CARD_TO_D5D6D7();
#endif
	DBG(("CardOpen()\n"));
	CardOpen();
	TimeOutSet(&CardDctTimer, 0);
#endif

#ifdef FUNC_RADIO_EN
#ifdef FUNC_SLAVE_UART_EN
       gRadioCtrl.RadioUpperBound = RADIO_UPPER_BOUND;
       gRadioCtrl.RadioLowerBound = RADIO_LOWER_BOUND;
	gRadioCtrl.RadioFreqBase = gRadioCtrl.RadioLowerBound;   
	gRadioCtrl.RadioSeekStep = RADIO_SEEK_STEP_100KHZ;
#endif

	RadioInit();
	RadioPowerDown();
#endif

#ifdef FUNC_POWER_AMPLIFIER_EN
		ABPowerAmplifierOn();	//������ģʽ��ѡ��AB �๦��
#endif	


	//�ȴ�300���룬��һ�μ�������豸������״̬
	//����ϵͳ��λ���һ�εĺ���Ȳ�
	TimeOutSet(&Timer, 500);
	while(!IsTimeOut(&Timer))
	{	
#ifdef FUNC_HEADPHONE_ADC_DETECT_EN
		KeyEventGet();	
#endif
	
#ifdef FUNC_USB_EN
		IsUsbInsert();
#endif
#ifdef AU6210K_NR_D_8_CSRBT
#ifdef FUNC_LINEIN_EN
		IsLineInInsert();
#endif
#ifdef FUNC_CARD_EN
		IsCardInsert();
#endif

#else
#ifdef FUNC_CARD_EN
		IsCardInsert();
#endif

#ifdef FUNC_LINEIN_EN
		IsLineInInsert();
#endif
#endif
	}

#ifdef	FUNC_HEADPHONE_DETECT_EN
	IsHeadPhonInsert();
#endif

#ifdef OPTION_CHARGER_DETECT
	IsChargerInsert();
	
	if(IsChargerLinkFlag == TRUE)  
	{
#ifdef	FUNC_DEVICE_FORCEPAUSE_EN
		gDevicePlayForcePauseFlag = TRUE;
#endif
	}	
	else
	{			
#ifdef FUNC_POWER_ON_AUDIO_EN
		gUSBChargePowerOnFlag = FALSE;					
#endif
	}
#endif	 

#if (defined(FUNC_OTP_KEY_SOUND_EN) || defined(FUNC_SPI_FLASH_POWERON_SOUND))
	SystemPowerOnSound();	//ϵͳ�������ֲ���
#endif

#if (defined(FUNC_AUDIO_EN) || defined(FUNC_AUDIO_READER_EN))
//����PC����ֱ�ӽ���Audio ģʽ
#ifdef FUNC_POWER_ON_AUDIO_EN
#ifdef FUNC_AUDIO_EN
	gSys.SystemMode = SYS_MODE_AUDIO;
#else
	gSys.SystemMode = SYS_MODE_AUDIO_READER;
#endif
#endif
#endif

#ifdef FUNC_STANDBY_EN
	gSys.SystemMode = SYS_MODE_STANDBY;
#endif
#ifdef FUNC_PT231X_EN
       gSys.Volume =VOLUME_INIT;
       PT2313Init();
#endif

#if 0//defined(AU6210K_NR_D_9X_XJ_HTS)
	if (IsUsbLinkFlag == TRUE)
	{
		gSys.SystemMode = SYS_MODE_USB;
	}
	else if (IsCardLinkFlag == TRUE)
	{
		gSys.SystemMode = SYS_MODE_SD;
	}
	else
	{
		gSys.SystemMode = SYS_MODE_BLUETOOTH;
	}
#endif

#if defined(AU6210K_NR_D_8_CSRBT)|| defined(AU6210K_ZB_BT007_CSR)
#ifdef FUNC_LINEIN_EN
	if (IsLineInLinkFlag == TRUE)
	{
		gSys.SystemMode = SYS_MODE_LINEIN;
	}
	else 
#endif

#ifdef FUNC_CARD_EN
	if(IsCardLinkFlag == TRUE)
	{
		gSys.SystemMode = SYS_MODE_SD;
	}
	else
#endif
	{
		gSys.SystemMode = SYS_MODE_BLUETOOTH;
	}
#endif


#if 0// defined(AU6210K_HXX_B002)
	if (IsCardLinkFlag == TRUE)
	{
		gSys.SystemMode = SYS_MODE_SD;
	}
	else
	{
		gSys.SystemMode = SYS_MODE_BLUETOOTH;
	}
#endif

#if defined(AU6210K_JLH_JH82B)
		gSys.SystemMode = SYS_MODE_BLUETOOTH;
#endif
	if(!TryGotoMode(gSys.SystemMode))
	{
#ifndef FUNC_QUICK_RESPONSE_EN
#ifdef FUNC_BLUETOOTH_EN
		TryGotoValidMode(SYS_MODE_BLUETOOTH);
#else
#ifdef AU6210K_NR_D_8_CSRBT
		TryGotoValidMode(SYS_MODE_SD);
#else
		TryGotoValidMode(SYS_MODE_BLUETOOTH);
#endif
#endif
#else
		//����ϵ���Ϣ�е�ģʽʱʧ�ܣ����Խ����һ����Чģʽ
		if(!IsSwitchMode)
		{
			TryGotoValidMode(SYS_MODE_USB);
		}

		//��ģʽ�����������л�ģʽ
		while(IsSwitchMode)
		{
			IsSwitchMode = FALSE;
			DBG(("CurMode: %d\n", (WORD)CurMode));
			TryGotoNextMode(CurMode);
		}
#endif
	}
}

#ifdef FUNC_BT_CHAN_AUTO
BOOL DacMute_Save = TRUE;

static VOID ChannelSwitchBT_and_Unmute()
{
	InDacChannelSel(BLUETOOTH_CH_TYPE);
	SetBluetoothVolume(gSys.Volume);
	
	ChannelType = CHANNEL_TYPE_BT;
	InDacMuteDis();
	IsDacMute = FALSE;
}

static VOID DacUnmute()
{
	if(DacMute_Save)
		return;
	InDacMuteDis();
	IsDacMute = FALSE;
}

static VOID ChannelSwitch_and_Unmute()
{

	if(gSys.SystemMode == SYS_MODE_SD)
	{
		//if(ChannelType == CHANNEL_TYPE_SD)
		//	return;
		//else if(ChannelType == CHANNEL_TYPE_BT)
		{
			InDacChannelSel(DAC_CH_DAC);
			PlayVol2Decd();
			ChannelType = CHANNEL_TYPE_SD;
			DacUnmute();
		}
	}
	else if(gSys.SystemMode == SYS_MODE_RADIO)
	{
		//if(ChannelType == CHANNEL_TYPE_FM)
		//	return;
		//else if(ChannelType == CHANNEL_TYPE_BT)
		{
			InDacChannelSel(DAC_CH_ONCHIP_FM);
			RadioSetVolume(gSys.Volume);
			ChannelType = CHANNEL_TYPE_FM;
			DacUnmute();
		}
	}

	
}

#endif

extern BOOL gBtCallIsHigh_Flag;
// Device state control.
VOID DevStateCtrl(VOID)
{
	static MESSAGE Event;	//Ϊ���Eventֵ���޸�bug����������Ϊstatic����.	
	BYTE TempMode = gSys.SystemMode;

	Event = MessageGet(MSG_FIFO_DEV_CTRL);	 


#ifdef FUNC_BT_CHAN_AUTO
	if(IsBtCallComing())
	{
		if(gSys.SystemMode != SYS_MODE_BLUETOOTH)
		{
			if(IsDacMute)
				DacMute_Save = TRUE;
			else
				DacMute_Save = FALSE;
			MuteOn(TRUE, TRUE);
			ChannelSwitchBT_and_Unmute();
		}
	}

	 
	if(IsBtCallEnd())
	{
		
			ChannelSwitch_and_Unmute();
		
	}
#endif

	
//	DBG(("DevStateCtrl\nevent:%bx\n", Event));
//DBG1(("vol = %x\n",gSys.Volume));
#if 0//def AU6210K_NR_D_8_CSRBT
#if defined(FUNC_NPCA110X_EN) 
	if(Event == MSG_DEFAULT)
	{
		DBG1(("msg_default\n"));
		if(!isDealSetBass)
		{			
			isDealSetBass = TRUE;
			TimeOutSet(&SetBassTimer, 0);

			if(isDefaultBass)
			{
				isDefaultBass = FALSE;
			}
			else
			{
				isDefaultBass = TRUE;
			}
		}
	}

	
	if(IsTimeOut(&SetBassTimer) && isDealSetBass)
	{

		TimeOutSet(&SetBassTimer, SET_BASS_TIME);
		if(isDefaultBass)
		{
			NPCA110X_NormalBass(i,BassLed_CallBak);
		}
		else
		{
			NPCA110X_SetBass(i,BassLed_CallBak);
		}
		i++;
				DBG1(("msg_default deal i = %x\n",i));

	}
	else
	{
		i = 0;
	}
#endif
#endif

#ifdef FUNC_QUICK_RESPONSE_EN
	IsSwitchMode = FALSE;
#endif

#ifdef FUNC_STANDBY_EN
	//����״̬��ֻ��Ӧ�����˳���Ϣ
	if(gSys.SystemMode == SYS_MODE_STANDBY)
	{
		if(Event == MSG_POWER)
		{
			DBG(("EXIT STANDBY!\n"));
			InDacInit(FALSE);			
			if(!TryGotoMode(gSystemExecState))
			{
				TryGotoValidMode(SYS_MODE_USB);
			}	
		    #ifdef FUNC_POWER_AMPLIFIER_EN
		       if(gSystemExecState != SYS_MODE_RADIO)
			      ABPowerAmplifierOn();
            #endif
			
			#ifdef FUNC_USB_VOLUP_EN
			    if (Mode != SYS_MODE_USB)
			   {
				  USB_VOLUP_Off();
			   }
            #endif		
			
		}
		return;
	}
#endif

#ifdef POWER_SAVING_MODE_OPTION
	//���ϵͳ���йرչ���(POWERDOWN��SLEEP)���յ�ϵͳ�ر���Ϣ����ִ��ϵͳ�رղ���
	if((Event == MSG_SYS_OFF) || (Event == MSG_POWER))
	{	
#ifdef FUNC_STANDBY_EN
		TryGotoValidMode(SYS_MODE_STANDBY);
#else
		SystemOff();
#endif
	}
#endif

#ifdef FUNC_BT_CHAN_AUTO
	IsBtCallComing();
#endif


#ifdef FUNC_USB_EN
	//����Ȳ����
	//DBG(("gIsUsbReconnect%bx\n",gIsUsbReconnect));
	if(IsUsbInsert() || ((gIsUsbRemove == TRUE) && (IsUSBLink())))			//��U���²���
	{		
		IsUsbLinkFlag = TRUE;	//usb ���ٲ�ι�����һ���߼�©��
		if((gFsInfo.DevID == DEV_ID_OTP) && (gSys.SystemMode == SYS_MODE_OTPDEV))
		{
			gPlayCtrl.State = PLAY_STATE_STOP;
		}				
		DBG(("IsUsbInsert()\n"));
		if((gIsUsbRemove == TRUE) && IsUSBLink())
		{
			DBG(("**Ins**\n"));
		}
#ifdef	FUNC_USB_CHARGE_EN
		AppleDeviceFlag = FALSE;    
#endif
		gIsUsbRemove = FALSE;
		if(!TryGotoMode(SYS_MODE_USB))
		{
			TryGotoValidMode(TempMode);
		}			
	}
#endif
	//DBG1(("IsDacMute = %x\n",IsDacMute));

#ifdef FUNC_CARD_EN
	//�п��²��룬���ҵ�ǰ���Ǵ��ڶ�������һ��ͨģʽ����������Ȳ�����
	if(IsCardInsert()		
	&& (gSys.SystemMode != SYS_MODE_READER)
	&& (gSys.SystemMode != SYS_MODE_AUDIO_READER))		
	{	
		if((gFsInfo.DevID == DEV_ID_OTP) && (gSys.SystemMode == SYS_MODE_OTPDEV))
		{
			gPlayCtrl.State = PLAY_STATE_STOP;
		}				
		DBG(("IsCardInsert()\n"));
		MuteOn(TRUE,TRUE);
		WaitMs(150);//���init sd card error����
		TryGotoValidModeReturn(SYS_MODE_SD);
	}
#endif

#ifdef FUNC_LINEIN_EN
	if(IsLineInInsert())	//��line-in�²���
	{
		DBG(("IsLineInInsert()\n"));
		TryGotoValidModeReturn(SYS_MODE_LINEIN);
	}
#endif

#if ((defined FUNC_AUDIO_EN) || (defined FUNC_READER_EN) || (defined FUNC_AUDIO_READER_EN))
	if(IsPcInsert()) 		//��PC�²���
	{
		DBG(("IsPcInsert()\n"));

#if (defined FUNC_AUDIO_EN)
		TryGotoValidModeReturn(SYS_MODE_AUDIO);
#elif (defined FUNC_READER_EN)
		TryGotoValidModeReturn(SYS_MODE_READER);
#elif (defined FUNC_AUDIO_READER_EN)
		TryGotoValidModeReturn(SYS_MODE_AUDIO_READER);
#endif
	}
#endif

#ifdef	FUNC_DEVICE_FORCEPAUSE_EN
	if(IsChargerInsert())	//��PC�������²���
	{
		gDevicePlayForcePauseFlag = TRUE;
#ifdef	FUNC_RADIO_EN
		if((gSys.SystemMode == SYS_MODE_RADIO) && (gRadioCtrl.State != RADIO_IDLE))
		{
			gRadioCtrl.State = RADIO_IDLE;	
		}
#endif
		MuteOn(TRUE, TRUE);
	}
	else if(gDevicePlayForcePauseFlag == TRUE)
	{
		if(!IsInCharge())
		{
			gDevicePlayForcePauseFlag = FALSE;
			if((gSys.SystemMode == SYS_MODE_SD) || (gSys.SystemMode == SYS_MODE_USB)) 
			{
				MessageSend(MSG_FIFO_KEY, MSG_PLAY_PAUSE);
			}	
#ifdef	FUNC_RADIO_EN
			else if(gSys.SystemMode == SYS_MODE_RADIO)
			{
				UnMute();
			}
#endif
		}
	}
#endif

#ifdef	FUNC_HEADPHONE_DETECT_EN
	if(IsTimeOut(&sHeadPhoneCheckTimer))
	{		
#ifdef FUNC_HEADPHONE_ADC_DETECT_EN		
		TimeOutSet(&sHeadPhoneCheckTimer, 0);
		if(IsHeadphoneLinkFlag == TRUE)
#else		
		TimeOutSet(&sHeadPhoneCheckTimer, 100);
		if(IsHeadPhonInsert())  //�ж�������
#endif
		{
#ifdef FUNC_EXMUTE_EN
			ExMuteOn(); 	
#endif			
			//DBG(("HeadPhone insert\n"));
		}
		else if((IsHeadphoneLinkFlag == FALSE) && (IsDacMute == FALSE))
		{
#ifdef FUNC_EXMUTE_EN
			ExMuteOff();
#endif			
			//DBG(("HeadPhone no insert\n"));
		}
	}
#endif

	//�豸�γ����
	//cunrrent device is removed.
	switch(gSys.SystemMode)
	{
#ifdef FUNC_USB_EN
		case SYS_MODE_USB:			//current system mode is USB		
			if(gDeviceError)		//usb error
			{
				DBG(("gDeviceError\n"));
				gDeviceError = 0;
				//���USB�����ִ���������᳢�����½���USBģʽ
				if(!TryGotoModeUSB())
				{
#if 0
					if (!TryGotoModeSD())
					{
						TryGotoValidMode(SYS_MODE_LINEIN);
					}
#else
					TryGotoValidMode(SYS_MODE_SD);
#endif
				}
			}
			else if(!IsUSBLink())		//usb removed
			{
				DBG(("usb removed!\n"));
#ifdef FUNC_USB_CHARGE_EN			
				AppleDeviceFlag = FALSE;
#endif

#if 0
				if (!TryGotoModeSD())
				{
					TryGotoValidMode(SYS_MODE_LINEIN);
				}
#else
				TryGotoValidMode(SYS_MODE_SD);
#endif
			}	
			break;
#endif

#ifdef FUNC_CARD_EN
		case SYS_MODE_SD:			//current system mode is SD
			if(gDeviceError)		//sd-card error
			{
				DBG(("gDeviceError\n"));
				//���SD�����ִ���������᳢�����½���SDģʽ
				if(!TryGotoModeSD())
				{
#if defined(AU6210K_JLH_JH82B) || defined(AU6210K_NR_D_8_CSRBT)
					if (!TryGotoModeLINEIN())
					{
						TryGotoValidMode(SYS_MODE_BLUETOOTH);
					}
#elif defined(AU6210K_HXX_B002)
					TryGotoValidMode(SYS_MODE_BLUETOOTH);
#else
					TryGotoValidMode(SYS_MODE_USB);
#endif
				}
			}
			else if(!IsCardLink())		//sd-card removed
			{
				DBG(("card removed!\n"));
#if defined(AU6210K_JLH_JH82B) || defined(AU6210K_NR_D_8_CSRBT)
				if (!TryGotoModeLINEIN())
				{
					TryGotoValidMode(SYS_MODE_BLUETOOTH);
				}
#elif defined(AU6210K_HXX_B002)
				TryGotoValidMode(SYS_MODE_BLUETOOTH);
#else
				TryGotoValidMode(SYS_MODE_USB);
#endif
			}
			break;
#endif

#ifdef FUNC_LINEIN_EN
		case SYS_MODE_LINEIN:		//current system mode is LINEIN
			if(!IsLineInLink())		//line-in removed
			{
				DBG(("line-in removed!\n"));
				TryGotoValidMode(SYS_MODE_BLUETOOTH);
			}
			break;
#endif

#if (defined(FUNC_AUDIO_EN) || defined(FUNC_READER_EN) || defined(FUNC_AUDIO_READER_EN))
		case SYS_MODE_AUDIO:		//current system mode is AUDIO
		case SYS_MODE_READER:		//current system mode is READER
		case SYS_MODE_AUDIO_READER:	//current system mode is AUDIO_READER
			//DBG(("DevStateCtrl->SYS_MODE_AUDIO\n"));
			if(!IsPcLink())			//pc removed
			{
				DBG(("pc removed 0!\n"));
				WaitMs(400);
				if(IsPcInsert())
				{
					DBG(("pc !rem"));
					break;
				}
				DBG(("pc removed!\n"));
#ifdef AU6210K_ZB_BT007_CSR
				TryGotoValidMode(SYS_MODE_SD);
#else
				TryGotoValidMode(SYS_MODE_USB);
#endif
			}
			break;
#endif

#ifdef	FUNC_RADIO_EN
		case SYS_MODE_RADIO:
			break;
#endif

		case SYS_MODE_NONE: 
#ifdef FUN_SYSTEM_POWEROFF_WAIT_TIME
			SystemOffTimeMsgPro(Event);
#endif	
			break;

		default:
			break;
	}

	//�����л����
	
	if((Event == MSG_MODE_SW) 
#ifdef FUNC_QUICK_RESPONSE_EN
	|| IsSwitchMode
#endif	
	)
	{
		DBG(("DevStateCtrl,MODE\n"));
		if((gFsInfo.DevID == DEV_ID_OTP) && (gSys.SystemMode == SYS_MODE_OTPDEV))
		{
			gPlayCtrl.State = PLAY_STATE_STOP;
		}
		
#ifdef	FUNC_DEVICE_FORCEPAUSE_EN
		gDevicePlayForcePauseFlag = FALSE;
#endif

#ifdef FUNC_BEEP_SOUND_EN	
		PushKeyBeep(1);
#endif		

#ifndef FUNC_QUICK_RESPONSE_EN
		TryGotoNextMode(gSys.SystemMode);
#else
		IsSwitchMode = TRUE;
		while(IsSwitchMode)
		{
			IsSwitchMode = FALSE;
			DBG(("CurMode: %d\n", (WORD)CurMode));
			TryGotoNextMode(CurMode);
		}
#endif
	} 
	
#if defined(AU6210K_DEMO)
#ifdef FUNC_USB_EN
	if((Event == MSG_MODE_USB) && (gSys.SystemMode != SYS_MODE_USB))
	{
		if(IsUsbLinkFlag == TRUE)
		{
			//DBG(("DevStateCtrl,USB MODE\n"));
			TryGotoValidModeReturn(SYS_MODE_USB);
		}
		else
		{
			//DBG(("DevStateCtrl,NO USB\n"));
		}
	}
#endif

#ifdef FUNC_CARD_EN
	if((Event == MSG_MODE_SD) && (gSys.SystemMode != SYS_MODE_SD))
	{
		if(IsCardLinkFlag == TRUE)
		{
			//DBG(("DevStateCtrl,SD MODE\n"));
			TryGotoValidModeReturn(SYS_MODE_SD);
		}
		else
		{
			//DBG(("DevStateCtrl,NO CARD\n"));
		}
	}	
#endif

#ifdef FUNC_RADIO_EN
	if((Event == MSG_MODE_FM) && (gSys.SystemMode != SYS_MODE_RADIO))
	{
		if(Radio_Name != RADIO_NONE)
		{
			//DBG(("DevStateCtrl,FM MODE\n"));
			TryGotoValidModeReturn(SYS_MODE_RADIO);
		}
		else
		{
			//DBG(("DevStateCtrl, NO FM or FM Error\n"));
		}
	}
#endif

#ifdef FUNC_LINEIN_EN
	if((Event == MSG_MODE_AUX) && (gSys.SystemMode != SYS_MODE_LINEIN)) 
	{
		if(IsLineInLink())
		{
			//DBG(("DevStateCtrl,AUX MODE\n"));
			TryGotoValidModeReturn(SYS_MODE_LINEIN);
		}
		else
		{
			//DBG(("DevStateCtrl, NO AUX\n"));
		}
	}	
#endif
#endif

#ifdef FUNC_SNOOZE_EN
	//SNOOZE �������
	if((sAlarmPowerOnFlag == TRUE) && (Event == MSG_SNOOZE))
	{
		TryGotoValidMode(SYS_MODE_STANDBY);
		sSnoozeOnFlag = TRUE;
	}
#endif
}


#ifdef FUNC_STANDBY_EN
VOID StandbyStateCtrl(VOID)
{	
#ifdef AU6210K_BOOMBOX_DEMO
#ifdef FUNC_ALARM_EN
	if(gAlarm1OnFlag == TRUE)
	{
		gAlarm1OnFlag = FALSE;
		sAlarmPowerOnFlag = TRUE;
		gSys.Volume = gAlarm1Volume;
		gSys.SystemMode = gAlarm1Source;
		InDacInit(FALSE);
		TryGotoValidMode(gSys.SystemMode);		 
	}
	else if(gAlarm2OnFlag == TRUE)
	{
		gAlarm2OnFlag = FALSE;
		sAlarmPowerOnFlag = TRUE;
		gSys.Volume = gAlarm2Volume;
		gSys.SystemMode = gAlarm2Source;
		InDacInit(FALSE);
		TryGotoValidMode(gSys.SystemMode);
	}	
#endif
#endif

#ifdef FUNC_SNOOZE_EN
	if((sSnoozeOnFlag == TRUE) && (IsTimeOut(&sSnoozeTimer)))
	{
		sSnoozeOnFlag = FALSE;
		TryGotoValidMode(gSystemExecState);
	}
#endif
}
#endif


