#include "syscfg.h"
#include "sysctrl.h"
#include "usb_audio_ctrl.h"
#include "card_reader_ctrl.h"
#include "devctrl.h"
#include "playctrl.h"
#include "device.h"
#include "rtc_ctrl.h"
#include "breakpoint.h"
#include "linein_ctrl.h"
#include "interrupt.h"
#include "power_key.h"
#include "key.h"
#include "i2c.h"
#include "sleep.h"
#include "sys_on_off.h"
#include "power_monitor.h"
#include "host_stor_scsi.h"
#include "host_hcd.h"
#include "chip.h"
#include "uart.h"
#include "radio_app_interface.h"
#include "radio_ctrl.h"
#include "debug.h"
#include "sleep.h"
#include "gpio.h"
#include "display.h"
#include "ir.h"
#include "user_interface.h"
#include "pwm.h"
#include "spi_flash.h"
#include "spi_fs.h"
#include "touch_key.h"
#include "bluetooth_ctrl.h"
#include "slave_ctrl.h"
#include "bt.h"
#include "npca110x.h"

#ifdef FUNC_EXDAC_EN
extern VOID UseAsSlaveDecd(VOID);
extern VOID UseAsSlaveDAC(VOID);
extern VOID UseAsMasterDac(VOID);
#endif
#ifdef FUNC_NPCA110X_EN
extern BOOL isDefaultBass;
#endif

SYSTEM	gSys;



BYTE GetBPSize(VOID)
{
#ifdef FUNC_BREAK_POINT_EN
	return sizeof(BP_INFO);
#else
	return 0;
#endif
}


//IP_X.0 for INT0, 
//IP_X.1 for T0, 
//IP_X.2 for INT1, 
//IP_X.3 for T1, 
//IP_X.4 for SP
sfr IP_X = 0xA9;

// System tick (1ms) initilization.
VOID SysTickInit(VOID)
{
	gSysTick = 0;

	//����Ϊ8λ�Զ���װ��ģʽ
	ET0 = 0; 	
	TR0 = 0; 	
	TMOD &= 0xf0; 	
	TMOD |= 0x02;										//using Timer0 as tick base	
	TimerCountSet(1);	
	PT0 = 1;											//high priority
	ET0 = 1;											//enable Timer0 int
	TR0 = 1;   
	IP_X |= 0x02;
//	EA = 1;											//run Timer0
}


// Oriole Series Chip example main function, add you code.
VOID main()
{
#ifdef FUNC_SAVE_PLAYTIME_EN
	BYTE TempVal;
#endif

	ChipInit();

#ifdef FUNC_BLUETOOTH_EN
//���������������ڵĵ�ƽ���Ա�֮������������
	BT_NEXT_L();
	BT_PREV_L();
	BT_PLAY_L();
	BT_POWER_L();
#endif


	ModuleClkEn(MASK_ALL_CLOCK);
	
#ifndef FUNC_WATCHDOG_EN
	WatchDogDis();
#else
	WatchDogEn();
	FeedWatchDog();
#endif
	
#if (defined(FUNC_USB_EN) || defined (FUNC_AUDIO_EN) \ 
	|| defined(FUNC_READER_EN) || defined (FUNC_AUDIO_READER_EN))
	IsUsbIOUseToGpio = FALSE;
#endif
	
	ClockSwitchToDPLL(); //add for O20 
	SysTickInit();	
#if (defined(FUNC_TIMER1_REFRESH_LED_EN) || defined(FUNC_TOUCH_KEY_EN))
	Timer1Init();
#endif
    EA = 1;
	ExInt1Init();

#ifdef FUNC_UARTDBG_EN
	I51UartOpen(UART_BAUDRATE, UART_PORT);
#endif
/*#ifdef FUNC_SLAVE_UART_EN
	SlaveCtrlInit();
#endif*/

#ifndef need_change_flag  //no use
	baGPIOCtrl[GPIO_A_IE] |= 0x40;//A2
	baGPIOCtrl[GPIO_A_OE] &= ~0x40;
	baGPIOCtrl[GPIO_A_PU] |= 0x40;
	baGPIOCtrl[GPIO_A_PD] |= 0x40; 
	baGPIOCtrl[GPIO_A_OUT] &= ~0x40; //A2
	WaitMs(2);
#endif

	DBG1(("******* Welcome to use MVsilicon's chip! *******\n"));

	KeyScanInit();

	MessageInit();

#if ((POWER_SAVING_MODE_OPTION == POWER_SAVING_MODE_SLEEP) \ 
	|| (POWER_SAVING_MODE_OPTION == POWER_SAVING_MODE_POWERDOWN)) 
	SystemOn();
#endif

#ifndef need_change_flag  //no use
	baGPIOCtrl[GPIO_A_IE] &= ~0x40;//A2
	baGPIOCtrl[GPIO_A_OE] |= 0x40;
	baGPIOCtrl[GPIO_A_PU] |= 0x40;
	baGPIOCtrl[GPIO_A_PD] |= 0x40; 
	baGPIOCtrl[GPIO_A_OUT] |= 0x40; //A2
	WaitMs(2);
#endif

#ifdef FUN_SYSTEM_POWEROFF_WAIT_TIME
	SystemOffTimeInit();
#endif

	//ʾ����A2����Ϊ��ͨGPIO���������
	SetA2IcsReg(A2_ICS_FOR_GPIO, A2_ICS_CLOSE);  //����A2����Ϊ��ͨGPIO
//	ClrGpioRegBit(GPIO_A_PD, (1 << 2));
//	ClrGpioRegBit(GPIO_A_PU, (1 << 2));
//	SetGpioRegBit(GPIO_A_IE, (1 << 2));
//	ClrGpioRegBit(GPIO_A_OE, (1 << 2));	

#ifdef FUNC_PWM_EN
	//PWMʾ����A0��A1����ΪPWM���
	PwmConfigChannel(PWM1_OUTPUT_GPIOA0, PWM_CLK_12MHZ, 1200, 600);	//���10KHz ��50%ռ�ձȵ�PWM�ź�
	PwmEnableChannel(PWM1_OUTPUT_GPIOA0);
	PwmConfigChannel(PWM2_OUTPUT_GPIOA1, PWM_CLK_46875HZ, 2, 1);	//���23.435KHz ��50%ռ�ձȵ�PWM�ź�
	PwmEnableChannel(PWM2_OUTPUT_GPIOA1);
#endif

#ifdef FUNC_EXMUTE_EN
	ExMuteOn(); 	
#endif	


#ifdef FUNC_DISP_EN

	DispInit(FALSE);
#endif
#ifdef FUNC_NPCA110X_EN
	I2C_Init();
	WaitMs(2);
	DBG(("initttttttt\n"));
	NPCA110X_Init();
	DBG(("ini22222\n"));
	isDefaultBass = TRUE;
#endif

	//������ܿ��ƹ��ܴ򿪣��˴���������1�ε͵�ѹ���ͳ���豸�������
#ifdef FUNC_POWER_MONITOR_EN
	PowerMonitorInit();
#endif	

#ifdef AU6210K_NR_D_8_CSRBT
	InDacSetVolume(gVolArr[28], gVolArr[28]);
#endif
	
#ifdef FUNC_EXDAC_EN
	InDacInit(FALSE);
	InDacChannelSel(DAC_CH_DAC);
	UseAsSlaveDecd();
	//�����ⲿDAC������
	//SetExDacSampleRate(SampleRate);
	gSys.Volume = 28;
	PlayVol2Decd();
	InDacMuteDis();
//	while(1);
#else
	InDacInit(FALSE);
#ifdef FUNC_DACOUT_DAPLESS_EN
	InDacVCOMSet(TRUE);	//�����ֱ����������
#endif
#endif

#ifdef FUNC_BREAK_POINT_EN
#if (FUNC_RESTORE_DEVICE_SELECE == FUNC_RESTORE_DEVICE_EEPROM)
#ifdef FUNC_SAVE_PLAYTIME_EN
	//��ȡLDOIN �ڲ�ADCֵ,�Դ��ж�AC���绹�ǵ�ع���
	if(AdcReadOneChannel(0, ADC_VREF_AVDD33) > 43)	//���LDOIN��ѹ����4.7v
	{
		gSavePlayTimeEnFlag = TRUE;	//AC������Ҫ������䲥��ʱ��
		DBG(("gSavePlayTimeEnFlag = 1;\n"));	
	}
	else
	{		
		gSavePlayTimeEnFlag = FALSE;	//��ع��粻��Ҫ����
		DBG(("gSavePlayTimeEnFlag = 0;\n"));	
	}
#endif
	
	I2C_Init();
#endif
	BP_LoadInfo();
#endif

#ifdef FUNC_RTC_EN
	RtcCtrlInit();	
#endif
	
#ifdef FUNC_SPI_FLASH_EN 
	SpiFlashInit();
#if (defined(FUNC_SPI_PLAY_EN) || defined(FUNC_SPI_KEY_SOUND_EN))
	SpiFlashFsInit();
#endif
#endif

	DevCtrlInit();	

	while(1)
	{
		FeedWatchDog();
#ifdef FUNC_POWER_MONITOR_EN
		//ִ�е�ص�ѹ��⡢����豸�����⼰�丽������ʾ��ϵͳ�رմ����
		PowerMonitor(); 
#endif

#ifdef FUNC_SAVE_PLAYTIME_EN
		if((gPlayCtrl.State == PLAY_STATE_PLAY) && (gSavePlayTimeEnFlag == TRUE))
		{
			TempVal = AdcReadOneChannel(0, ADC_VREF_AVDD33);
			//DBG(("Vin: %d\n",  (WORD)TempVal));
	
			//LDOIN��ѹ����4.7v�ж�ΪAC����
			if((TempVal < 43) && (TempVal > 34))
			{
				BP_UpdatePlayTime();
			}
		}
#endif
#if 0
		SlaveStateCtrl();
#else	
		KeyEventGet();
#endif
		DevStateCtrl();
		switch(gSys.SystemMode)
		{
			case SYS_MODE_USB:
			case SYS_MODE_SD:
#ifdef FUNC_SPI_PLAY_EN
			case SYS_MODE_SPIDEV:
#endif
#ifdef FUNC_OTP_PLAY_EN
			case SYS_MODE_OTPDEV:
#endif
				PlayStateCtrl();
				break;

#ifdef FUNC_RADIO_EN
			case SYS_MODE_RADIO:
				RadioStateCtrl();
				break;
#endif

#ifdef FUNC_BLUETOOTH_CSR_EN
			case SYS_MODE_BLUETOOTH:
				BluetoothStateCtrl();
				break;
#endif
#ifdef FUNC_LINEIN_EN
			case SYS_MODE_LINEIN:
				LineInStateCtrl();
				break;
#endif

#if 0//def FUNC_BLUETOOTH_EN
			case SYS_MODE_BLUETOOTH:
				BluetoothStateCtrl();
				break;
#endif


#ifdef FUNC_AUDIO_EN
			case SYS_MODE_AUDIO:
				DeviceAudioStateCtrl();
				break;
#endif

#ifdef FUNC_READER_EN
			case SYS_MODE_READER:
				DeviceStorStateCtrl();
				break;
#endif

#ifdef FUNC_AUDIO_READER_EN
			case SYS_MODE_AUDIO_READER:
				DeviceAudioStateCtrl();
				DeviceStorStateCtrl();
				break;
#endif

#ifdef FUNC_STANDBY_EN
			case SYS_MODE_STANDBY:
				StandbyStateCtrl();
				break;

#endif

			default:
				break;		
		}	

#ifdef FUNC_RTC_EN
#ifndef FUNC_SLAVE_UART_EN
		RtcStateCtrl();
#endif
#endif

#ifdef FUNC_DISP_EN
		Display();
#endif

#ifdef FUN_SYSTEM_POWEROFF_WAIT_TIME
		SystemOffTimeWait();
#endif
	}
}


