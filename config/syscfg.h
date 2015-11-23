#ifndef __SYSCFG_H__
#define __SYSCFG_H__


/*
**********************************************************
*					INCLUDE
* ˵����ͷ�ļ������б������޸�
**********************************************************
*/
#include <reg51.h>
#include <string.h>	
#include <stdlib.h>

#include "type.h"
#include "utility.h"
#include "sysctrl.h"
#include "timer.h"
#include "rst.h"
#include "host.h"
#include "card.h"
#include "dac.h"
#include "gpio.h"
#include "int.h"
#include "clk.h"
#include "sleep.h"
#include "adc.h"
#include "fsinfo.h"
#include "fat.h"
#include "dir.h"
#include "file.h"
#include "folder.h"
#include "partition.h"
#include "presearch.h"
#include "message.h"
#include "decd.h"
#include "mp3.h"
#include "storage_if.h"
#include "clk_if.h"
#include "spi_play.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////			
//�û���Ŀ����
////////////////////////////////////////////////////////////////////////////////////////////////////////////			
//#define AU6210K_DEMO
//#define AU6210K_JLH_JH82B			//���廪
//#define AU6210K_HXX_B002			//����оB002
//#define AU6210K_NR_D_9X			//����D-9Xϵ��
//#define AU6210K_NR_D_9X_XJ_HTS  //����D-9Xϵ�и���������·

//#define AU6210K_NR_D_8_CSRBT     //ID=581
//#define AU6210K_LK_SJ_CSRBT     //����������
#define AU6210K_ZB_BT007_CSR    //�ǲ�bt007 key 
#define AU6210K_XLX_ALD800  // ����оADL800:oem�ǲ�bt007 <FM+LINE+CSRBT>

#ifdef AU6210K_ZB_BT007_CSR
//#define AU6210K_ZB_BT007_IR_IC_IS_334M_CSR  //�ƶ���Դ�İ���    //���ǲ�bt007 ���в��ֹ��ܲ�ͬ: ir ic  //168
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////			
// ���Ź�����
////////////////////////////////////////////////////////////////////////////////////////////////////////////			
#define	FUNC_WATCHDOG_EN  			//���Ź����ܿ��ƺ꣬��Ĭ�ϱ��ֿ�����


////////////////////////////////////////////////////////////////////////////////////////////////////////////			
// ϵͳ֧���豸����ģʽ����
////////////////////////////////////////////////////////////////////////////////////////////////////////////	
#if defined(AU6210K_JLH_JH82B) || \
	defined(AU6210K_HXX_B002) || \
	defined(AU6210K_NR_D_8_CSRBT) || \
	defined(AU6210K_LK_SJ_CSRBT) || \
	defined(AU6210K_ZB_BT007_CSR)
#else
//#define FUNC_USB_EN					//USB HOST����(������U�̲���)
#endif

#if defined(AU6210K_XLX_ALD800) 
#else
//#define FUNC_CARD_EN				//CARD HOST����(������SD/TF������),������������ü�����
#endif

#define FUNC_LINEIN_EN				

#if defined(AU6210K_NR_D_8_CSRBT)||defined(AU6210K_ZB_BT007_CSR)
#else//no use radio
#define FUNC_RADIO_EN				//Radio����,���ͺ�оƬ�ڲ����ɸ�����FMģ��
#endif

#if defined(AU6210K_XLX_ALD800)//ald800 use radio
#define FUNC_RADIO_EN	
#endif

#ifdef AU6210K_ZB_BT007_CSR
//#define		BT_BtPOWEROFF_TIME	(DWORD)(600000)///10min 5000//5s///5000//5s use debug
//#define FUNC_READER_EN      		//������USB������ģʽ
#endif
//#define FUNC_AUDIO_READER_EN		//һ��ͨģʽ��PCͬʱʶ���USB������USB������	
//#define FUNC_AUDIO_EN				//������USB����ģʽ	
//#define FUNC_STANDBY_EN		//֧�ִ���ģʽ��Ĭ�ϲ�����
//#define FUNC_BLUETOOTH_EN

#define FUNC_BLUETOOTH_CSR_EN

#ifdef FUNC_BLUETOOTH_CSR_EN
#define CSR_IO_CTRL
//#define FUNC_AUTO_BTSTANDBY     //һ��ʱ��������û�����Զ��ص�����
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////			
// ����OTP ���ŵ�ģʽ
// FUNC_OTP_KEY_SOUND_EN : ����OTP ���������ܵĹرպͿ���  
// FUNC_OTP_PLAY_EN������OTP ����ģʽ�ĹرպͿ���
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#define FUNC_OTP_EN
#ifdef FUNC_OTP_EN
	#define FUNC_OTP_KEY_SOUND_EN	//�򿪴˺궨�壬֧��OTP ���Ű���������
	//#define FUNC_OTP_PLAY_EN			//�򿪴˺궨�壬��OTP����һ��MP3�豸����
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////			
// ����SPI ���ŵ�ģʽ
// SDK�н�ʵ����SST /GD SPI FLASH�Ĵ��룬����������Ҫ�ͻ��Լ����
//FUNC_SPI_KEY_SOUND_EN : ����SPI �������ĹرպͿ���
// FUNC_SPI_PLAY_EN��  ����SPI PLAY�ĹرպͿ���
// FUNC_SPI_UPDATE_EN���Ƿ��USB/SD������MVF��SPI FLASH
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define	FUNC_SPI_FLASH_EN
#ifdef FUNC_SPI_FLASH_EN
	#define FUNC_SPI_KEY_SOUND_EN	//�򿪴˺궨�壬֧��SPI ���Ű�����
	//#define FUNC_SPI_PLAY_EN		    // �򿪴˺궨�壬��SPI Flash����һ��MP3�豸
	#define FUNC_SPI_UPDATE_EN			// Update SPI via U-Disk/SD-Card(place *.MVF under root-diretory).
	// Define SPI flash CS port and pin.
	#define	SPI_FLASH_CS_PORT		GPIO_B_IN
	#define	SPI_FLASH_CS_PIN		(1 << 3)
	// Define the customer ID in SPI flash device
	// Caution: the ID must equal to the value in flash memory of products
	#define	SPI_DEV_CUSTOMER_ID		0x55AA01FE		
	#define	SPI_SECTOR_SIZE			512

#ifdef FUNC_SPI_UPDATE_EN
	//#define FUNC_SPI_UPDATE_AUTO_EN		//�򿪴˺궨�壬֧��SPI Flash�Զ�������������Ҫ�ֶ���������
#endif

#ifdef FUNC_SPI_KEY_SOUND_EN
	#define FUNC_SPI_FLASH_POWERON_SOUND		//�򿪸ú궨�壬֧��SPI Flash��������
#endif
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////			
// Ipod��繦��(ע:ϵͳʱ�Ӳ��ܷ�Ƶ���ܶ�Ipod���)
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#define FUNC_USB_CHARGE_EN


////////////////////////////////////////////////////////////////////////////////////////////////////////////			
// ������������Ӧ�����ܣ�Ӧ�ô�����user_interface.c�ļ���QuickResponse()����
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#define FUNC_QUICK_RESPONSE_EN


////////////////////////////////////////////////////////////////////////////////////////////////////////////			
// ����RTC��ģʽ
// FUNC_RTC_EN��  ����RTC�ĹرպͿ���
// FUNC_ALARM_EN���Ƿ������ӹ���
// FUNC_LUNAR_EN���Ƿ���ũ������
////////////////////////////////////////////////////////////////////////////////////////////////////////////			
//#define FUNC_RTC_EN								//RTC����
#ifdef FUNC_RTC_EN								//RTC��ģ��
	#define FUNC_ALARM_EN						//�Ƿ������ӹ���
	#define FUNC_LUNAR_EN						//�Ƿ���ũ������
	
#ifdef FUNC_ALARM_EN
	//#define FUNC_SNOOZE_EN						//�򿪸ú궨�壬֧��SNOOZE ����
#endif
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////			
// ��ʾģ�鶨�壬����ʵ��ѡ���֧��LED,LCD,����LED��
// FUNC_DISP_EN���Ƿ�����ʾ����
// FUNC_SEG_LCD_EN, FUNC_SEG_LED_EN,	FUNC_SINGLE_LED_EN������Ҫѡ��һ����������������ϸ����Ϣ��segPanel.h
// ���⻹��Ҫ����gDisplayMode����Ϊ��Ҫ��ģʽ,��ʾģʽ��������.
// DISP_DEV_SLED	-> ����LED��
// DISP_DEV_LED1888	-> ��λ�����LED��
// DISP_DEV_LED8888	-> ��λ����LED��
// DISP_DEV_LED57	-> 5*7����LED��
// DISP_DEV_LCD58	-> 5*8����LCD��
//ע����ʾ���ƺ���gDisplayMode�����Ķ�Ӧ��ϵ������Ҫѡ��LED57��ʾ�豸������Ҫ����FUNC_SEG_LED_EN�겢��gDisplayMode��ֵΪDISP_DEV_LED57��
////////////////////////////////////////////////////////////////////////////////////////////////////////////			
#define FUNC_DISP_EN					//��ʾ����
#ifdef FUNC_DISP_EN						//��ʾ��ģ��
//#define FUNC_SEG_LCD_EN				//����LCD��
//#define FUNC_SEG_LED_EN				//����LED��
#if 1
#define FUNC_SINGLE_LED_EN			//����LED��
#endif
#if defined(AU6210K_JLH_JH82B) || defined(AU6210K_NR_D_8_CSRBT)
#define FUNC_SINGLE_LED_HIGHON_EN //����LED�Ƿ��Ǹ���
#endif
#endif

//�������LED ��ѡ��
#ifdef	FUNC_SEG_LED_EN
#define FUNC_7PIN_SEG_LED_EN		//7ֻ�ŵĶ���LED ��
//#define FUNC_6PIN_SEG_LED_EN		//6ֻ�ŵĶ���LED ��
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////			
//7ֻ�ŵĶ���LED ������������GPIO����ʾ��(��AU6210HD Demoʹ�õ�LEDΪ��)
////////////////////////////////////////////////////////////////////////////////////////////////////////////			
#if (defined(FUNC_7PIN_SEG_LED_EN))
//#define FUNC_LED_ADC1_KEY_MULTIPLE_EN	//�򿪸ú궨�壬֧�ֶ���LED��ADC1 KEY�˿ڸ��ù���
//#define FUNC_LED_ADC2_KEY_MULTIPLE_EN	//�򿪸ú궨�壬֧�ֶ���LED��ADC2 KEY�˿ڸ��ù���
//#define FUNC_LED_LINEIN_MULTIPLE_EN		//�򿪸ú궨�壬֧�ֶ���LED��Line in���˿ڸ��ù���
//#define FUNC_TIMER1_REFRESH_LED_EN	//�򿪸ú궨�壬֧��Timer1 ˢ�¶���LED��ʾ(����LED������˸����)
#endif

#ifdef FUNC_LED_ADC1_KEY_MULTIPLE_EN
#define LED_ADC1_KEY_MULTIPLE_PORT_PU			GPIO_D_PU
#define LED_ADC1_KEY_MULTIPLE_PORT_PD			GPIO_D_PD
#define LED_ADC1_KEY_MULTIPLE_PORT_IN			GPIO_D_IN
#define LED_ADC1_KEY_MULTIPLE_PORT_IE			GPIO_D_IE
#define LED_ADC1_KEY_MULTIPLE_PORT_OE			GPIO_D_OE
#define LED_ADC1_KEY_MULTIPLE_PORT_OUT			GPIO_D_OUT
#define LED_ADC1_KEY_MULTIPLE_BIT				(1 << 7)
#endif
	
#ifdef FUNC_LED_ADC2_KEY_MULTIPLE_EN
#define LED_ADC2_KEY_MULTIPLE_PORT_PU			GPIO_B_PU
#define LED_ADC2_KEY_MULTIPLE_PORT_PD			GPIO_B_PD
#define LED_ADC2_KEY_MULTIPLE_PORT_IN			GPIO_B_IN
#define LED_ADC2_KEY_MULTIPLE_PORT_IE			GPIO_B_IE
#define LED_ADC2_KEY_MULTIPLE_PORT_OE			GPIO_B_OE
#define LED_ADC2_KEY_MULTIPLE_PORT_OUT			GPIO_B_OUT
#define LED_ADC2_KEY_MULTIPLE_BIT				(1 << 6)
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////			
// �ϵ�ϵ��������
// ��Ҫ���õĺ꣺
//     FUNC_BREAK_POINT_EN: �Ƿ����ϵ�ϵ繦��
//     FUNC_RESTORE_VOLUME�������Ƿ�ǿ�ƻָ�Ϊ���������
////////////////////////////////////////////////////////////////////////////////////////////////////////////			
#define FUNC_BREAK_POINT_EN				//�ϵ㡢�ϵ���书��
#ifdef FUNC_BREAK_POINT_EN//�ϵ㡢�ϵ������ģʽ
	//#define FUNC_RESTORE_VOLUME			//��������ģʽѡ��
	#define FUNC_RESTORE_DEVICE_EEPROM   	0
	#define FUNC_RESTORE_DEVICE_NVM         1
#if 0	
	#define FUNC_RESTORE_DEVICE_SELECE    	FUNC_RESTORE_DEVICE_EEPROM		//FUNC_RESTORE_DEVICE_EEPROM
#else
	#define FUNC_RESTORE_DEVICE_SELECE    	FUNC_RESTORE_DEVICE_NVM
#endif
	
//MP3����ʱ��������ѡ�񣬴˹��ܽ�����ֻ��AC����ͻȻ��������¼���ʱ��
#if (FUNC_RESTORE_DEVICE_SELECE == FUNC_RESTORE_DEVICE_EEPROM)
	//#define FUNC_SAVE_PLAYTIME_EN 	//Ĭ�Ϲرգ�֧�ִ˹�����ҪӲ�����Ӵ����
#endif
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////			
// ˵�������ڵ���ʹ�ܣ�ʹ��ʱUART�ڿ��Դ�ӡ��debug��Ϣ
//(ע��: UART_USE_A1A2֧�ֵ����������4800)
////////////////////////////////////////////////////////////////////////////////////////////////////////////			
//#define	FUNC_UARTDBG_EN		      		//���ڵ��Թ��ܺ꣬ע��UART�˿ڷ��䣬��ֹ��ͻ��
#ifdef FUNC_UARTDBG_EN
	#define	UART_PORT		UART_USE_TXD		//UARTʹ�õĶ˿ڣ�UART_USE_A1A2 or UART_USE_TXD
	#define	UART_BAUDRATE	BAUDRATE_57600		//UART�����ʣ�1200, 2400, 4800, 38400, 57600, 115200 
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////			
// ˵�������ڴӻ�ʹ�ܣ�ʹ��ʱUART�ڿ�����MCU����ͨ��
//(ע��: UART_USE_A1A2֧�ֵ����������4800)
////////////////////////////////////////////////////////////////////////////////////////////////////////////			

//#define	FUNC_SLAVE_UART_EN
////////////////////////////////////////////////////////////////////////////////////////////////////////////			
// ˵����������������
////////////////////////////////////////////////////////////////////////////////////////////////////////////			
#define FUNC_ADC_KEY_EN					//ADC��������

#if defined(AU6210K_ZB_BT007_CSR) && !defined(AU6210K_ZB_BT007_IR_IC_IS_334M_CSR)//007
#define FUNC_ADC_KEY_EN					//ADC��������
#endif
#if defined(AU6210K_HXX_B002) || defined(AU6210K_ZB_BT007_IR_IC_IS_334M_CSR)
#define FUNC_IR_KEY_EN				    //IR��������
#endif
//#define FUNC_GPIO_KEY_EN				//GPIO ɨ�谴������
//#define FUNC_CODING_KEY_EN			//��ת���뿪��
//#define FUNC_TOUCH_KEY_EN
//ADC�����˿�ѡ��
#ifdef FUNC_ADC_KEY_EN							
//SDK�е�������Ĭ��ʹ��2��ADCͨ������������ÿ��ͨ��֧��11������
#if (defined(FUNC_LED_ADC1_KEY_MULTIPLE_EN) || defined(FUNC_LED_ADC2_KEY_MULTIPLE_EN))
#define ADC_KEY_PORT_CH1	ADC_CHANNEL_D7	//ADC����ʹ�õĶ˿ڣ�B6, D7, E0, E1�����ֻʹ��һ��ͨ������������
#define ADC_KEY_PORT_CH2	ADC_CHANNEL_B6	//ADC����ʹ�õĶ˿ڣ�B6, D7, E0, E1�����ֻʹ��һ��ͨ������ע�͵�����
#elif 1//(defined(AU6210K_DEMO))
#define	ADC_KEY_PORT_CH1	ADC_CHANNEL_D7
#else
#define ADC_KEY_PORT_CH1	ADC_CHANNEL_E0	//ADC����ʹ�õĶ˿ڣ�B6, D7, E0, E1�����ֻʹ��һ��ͨ������������
#define ADC_KEY_PORT_CH2	ADC_CHANNEL_E1	//ADC����ʹ�õĶ˿ڣ�B6, D7, E0, E1�����ֻʹ��һ��ͨ������ע�͵�����
#endif
#endif

//IR ʹ�õĶ˿�ѡ��
#ifdef FUNC_IR_KEY_EN	
#define	IR_KEY_PORT			IR_INPUT_GPIOB6	//IRʹ�õĶ˿ڣ�IR_INPUT_GPIOB6 or IR_INPUT_GPIOE0
#endif

#if !defined(AU6210K_NR_D_8_CSRBT)
#define FUNC_NUMBER_KEY_EN		//�򿪸ú궨�壬֧�����ְ�������
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////			
// ���������������
////////////////////////////////////////////////////////////////////////////////////////////////////////////			
//#define	FUNC_LONG_NAME_EN		//���ļ�������														 
//#define	FUNC_FAST_PLAY_EN		//��������˹���
//#define	FUNC_AUTO_PLAY_EN		//�Զ����Ź���
//#define FUNC_FOLDER_EN			//�ļ��й���
//#define FUNC_HIDDEN_FOLD_EN 	//���������ļ����еĸ���
//#define FUNC_RECYCLED_FOLD_EN	//���Ż���վ�ļ����еĸ���
//#define FUNC_OPEN_FILE_DELAY_EN		//�Ƿ����ӳ���ӦPRE��NEXT���������ΰ�PRE��NEXTʱ���Կ�����Ӧ
//#define FUNC_DEVICE_FORCEPAUSE_EN	//�򿪸ú꿪�أ�֧�ֲ�������ʱǿ����ͣ��ǰ�豸�Ĳ���

//���ܲ��Ź���
//#define FUNC_ENCRYPT_DECODE_EN
#ifdef 	FUNC_ENCRYPT_DECODE_EN
    #define ENCRYPT_EXT  "HMR" //�ͻ����Զ����׺��	  
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////			
// FM,LINEIN��Ƶ���ͨ��ѡ������
// FM��LINEINģʽ���ã��û���Ҫȷ��ģʽ������ʵ�ʵ�·���
// LINEIN_IO_TYPE��ѡ��LINEIN��Ƶ����ţ���DAC_CH_E0_L��DAC_CH_E1_R��DAC_CH_E0E1��DAC_CH_LINEINѡһ
////////////////////////////////////////////////////////////////////////////////////////////////////////////			
#ifdef FUNC_RADIO_EN	
#define RADIO_MVFM_EN	//Radio����,���ͺ�оƬ�ڲ����ɸ�����FMģ��

//#define FM_DAC_OUT_D3D4		//�򿪸ú궨�壬֧��FMģ����Ƶ�źŴ�D3��D4ֱ�����
#endif

#ifdef FUNC_LINEIN_EN
#if defined(AU6210K_NR_D_8_CSRBT)
#define LINEIN_IO_TYPE		DAC_CH_NONE
#elif defined(AU6210K_LK_SJ_CSRBT)||defined(AU6210K_ZB_BT007_CSR)
#define LINEIN_IO_TYPE		DAC_CH_E0E1
#else
#define LINEIN_IO_TYPE		DAC_CH_LINEIN
#endif
#endif

#ifdef FUNC_BLUETOOTH_CSR_EN
#if defined(AU6210K_NR_D_8_CSRBT)
#define BLUETOOTH_CH_TYPE		DAC_CH_NONE
#elif defined(AU6210K_LK_SJ_CSRBT)||defined(AU6210K_ZB_BT007_CSR)
#define BLUETOOTH_CH_TYPE		DAC_CH_LINEIN
#else
#define BLUETOOTH_CH_TYPE		DAC_CH_E0E1
#endif

#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////			
//����������������
////////////////////////////////////////////////////////////////////////////////////////////////////////////			
#ifdef	FUNC_RADIO_EN
#if defined(FUNC_SINGLE_LED_EN)
#define FUNC_RADIO_AUTOSEEK_EN	// �򿪸ú궨�壬֧��FM Ӳ���Զ���̨����,����������̨ʱ�䣬���ǲ�֧��Ƶ����ʾ
#endif
//#define FUNC_RADIO_STEPOVER_EN		// �򿪸ú궨�壬 ֧��FM �ֶ�������̨����
//#define FUNC_RADIO_SEMO_AUTOSEEK_EN 	// �򿪸ú궨�壬֧��FM  ���Զ���̨����
#define FUNC_RADIO_ESD_AUTO_RESUME	// �򿪸ú궨�壬֧��FM  ��ESD�������Զ��ָ�����
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////			
//������������
////////////////////////////////////////////////////////////////////////////////////////////////////////////			
#if (defined(FUNC_AUDIO_EN) || defined(FUNC_AUDIO_READER_EN))
#define FUNC_POWER_ON_AUDIO_EN	//�򿪸ú궨�壬֧������PC ����ֱ�ӽ�������ģʽ,Ĭ�Ϲر�
#endif
//#define FUNC_DACOUT_DAPLESS_EN	// �򿪸ú궨��,֧�� DAC����������ֱ����������
//#define FUNC_BEEP_SOUND_EN	//�򿪸ú궨�壬֧�ְ�������������
//#define FUNC_PWM_EN		//�򿪸ú궨�壬֧��PWM����


////////////////////////////////////////////////////////////////////////////////////////////////////////////			
// ϵͳ���������
////////////////////////////////////////////////////////////////////////////////////////////////////////////			
// Maximum system clock divid value is not big than 16
#define		CLK_DIV_RATE_MAX	16
// usually recommand values are 2, 4, 6, 8
#if defined(FUNC_SINGLE_LED_EN)
#define		CLK_DIV_RATE		8
#else
#define		CLK_DIV_RATE		4
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////			
// ������ܼ���(�����ڴ����ϵͳ)�Ĺ��ܺ��ѡ���
// ���ܼ��Ӱ�������ص�ѹ��⼰�͵�ѹ���ϵͳ��Ϊ�Լ����ָʾ��
// ��ص�ѹ��⣬��ָLDOIN����˵ĵ�ѹ��⹦��(���ϵͳһ�㶼�ǵ��ֱ�Ӹ�LDOIN�ܽŹ���)
// �ù��ܺ�򿪺�Ĭ�ϰ�����ص�ѹ��⹦�ܣ��йص�ص�ѹ���������ɶ�������������power_monitor.c�ļ�
////////////////////////////////////////////////////////////////////////////////////////////////////////////			
//#if defined(AU6210K_NR_D_9X_XJ_HTS)
//#define FUNC_POWER_MONITOR_EN
//#endif

#ifdef FUNC_POWER_MONITOR_EN
//����豸�����⹦��
//#define	OPTION_CHARGER_DETECT		//�򿪸ú궨�壬֧��GPIO������豸���빦��

//�����˿�����
#ifdef OPTION_CHARGER_DETECT
	#define CHARGE_DETECT_PORT_PU			GPIO_E_PU
	#define CHARGE_DETECT_PORT_PD			GPIO_E_PD
	#define CHARGE_DETECT_PORT_IN			GPIO_E_IN
	#define CHARGE_DETECT_PORT_IE			GPIO_E_IE
	#define CHARGE_DETECT_PORT_OE			GPIO_E_OE
	#define CHARGE_DETECT_BIT				(1 << 0)
#endif
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////			
// ϵͳʡ��ģʽ����
// 
// ��Ҫ���õĺ꣺
//     POWER_SAVING_MODE_OPTION��ϵͳʡ��ģʽѡ�񣬲�������ȡ��ϵͳʡ�繦�ܣ���ѡ����Ϊ
//		->POWER_SAVING_MODE_SLEEP��ϵͳʡ��ʱ����SLEEPģʽ(Ƭ��LDOû�йر�,���Ľϸ�)
//		->POWER_SAVING_MODE_POWERDOWN��ϵͳʡ��ʱ����POWERDOWNģʽ(Ƭ��LDO�ر�,���ĵ�)
// ע�����
//	  ��ѡ��POWERDOWNģʽʱ��ϵͳ����ֻ�����ַ�ʽ
//		->POWER_KEY���ѣ�
//		->LOW POWER RTC����(RTC_MODE==LOWPOWER_RTC,����WAKEUP_IN_PWRDWN_BY_LP_RTC�꿪���ù���)��
//     ��ѡ��Sleepģʽʱ��ϵͳ���ѷ�ʽ��
//		->POWER_KEY���ѣ�
//		->GPIO����(�������ж�IO) ��
//		->NORMAL RTC(RTC_MODE==NORMAL_RTC)���ѣ�
//		->IR���ѡ�
//	  �����ҪRTC���ѹ��ܣ���Ҫ��ȷ����RTC_MODE��POWER_SAVING_MODE_OPTION��
////////////////////////////////////////////////////////////////////////////////////////////////////////////			

//����POWER_KEY�Ŀ�����ʽ��2ѡ1��ֻ�ܶ���1��
#define PWR_KEY_DISABLE			0
#define PWR_KEY_PUSH_BUTTON		1		//��ť����(����)
#define PWR_KEY_SLIDE_SWITCH	2		//��������(Ӳ����)

#define POWER_SAVING_MODE_SLEEP			0
#define POWER_SAVING_MODE_POWERDOWN		1
#define POWER_SAVING_MODE_OPTION		POWER_SAVING_MODE_POWERDOWN////POWER_SAVING_MODE_POWERDOWN	//

#if (POWER_SAVING_MODE_OPTION == POWER_SAVING_MODE_POWERDOWN)	//����ϵͳʡ��ģʽΪPOWERDOWN
	//POWERDOWN��ʽ: ϵͳ�ر�ʱ�ر�Ƭ��LDO��ϵͳ����ʱƬ��LDO���ϵ�
	//�÷�ʽҪ��оƬ��POWER_KEY���ţ�����ʵ�ּ��͹���(powerdown)�±�����Ҫ��Ϣ��Ҫ��
	//ѡ��÷�ʽ��Ĭ��ϵͳ����POWER_KEY����(����ѡ���POWER KEY��оƬ�ͺ�)
	#if defined(AU6210K_ZB_BT007_CSR) || defined(AU6210K_LK_SJ_CSRBT)
		#ifdef AU6210K_XLX_ALD800
		#define PWR_KEY_MODE	 PWR_KEY_PUSH_BUTTON
		#else
	 	#define PWR_KEY_MODE	 PWR_KEY_SLIDE_SWITCH	//PWR_KEY_PUSH_BUTTON
		#endif
	#else
     #define PWR_KEY_MODE	 PWR_KEY_PUSH_BUTTON
	#endif

#if	(PWR_KEY_MODE == PWR_KEY_PUSH_BUTTON)
		//���ѡ�����أ����Զ���POWER KEY���ػ�����ʱ��
		//�ڿ����׶Σ���ָ�û����밴סPOWER_KEY����[���ػ�����ʱ��]��ϵͳ�Ż������ʾ�������У�����оƬ�ٴν���powerdown
		//�ڹػ��׶Σ��û����밴סPOWER_KEY����[���ػ�����ʱ��]��ϵͳ�Ż�ر���ʾ�ȣ�Ȼ�����powerdown.
	#define TIME_POWER_ON_HOLD		2500	//��λms
	#define TIME_POWER_OFF_HOLD		2500

	//����POWER_KEY���Ի���оƬ����Ƭ������Դ�⣬���ɶ���LP_RTC������Դ��Ϊ��������Դ��RTC���ѽ�����PWR_KEY_PUSH_BUTTONģʽ��Ӧ��
	#define	WAKEUP_IN_PWRDWN_BY_LP_RTC

	//��ʱ�ػ����ܺ���ʾʹ��
	//#define FUN_SYSTEM_POWEROFF_WAIT_TIME
#endif
#else
	//#define PWR_KEY_MODE	PWR_KEY_DISABLE
#endif

#if (POWER_SAVING_MODE_OPTION == POWER_SAVING_MODE_SLEEP)	//����ϵͳʡ��ģʽΪSLEEP
	//SLEEP��ʽ: ϵͳ�رղ������ߵķ�ʽ���÷�ʽ��Ƭ������Դһֱ���磬���ı�POWERDOWN��ʽ��
	//�÷�ʽ�£�ϵͳ�������ɾ߱����ѹ��ܵ�IO��оƬģ�鴥��

	//������ЩIO����оƬģ����Ի���ϵͳ
	//WIS��WAKEUP_IN_SLEEP����д
	#define		WIS_FLG_IR	 //����IR ���Ѻ꣬IR IOѡ����IR_KEY_PORT����
	#define 	PWR_KEY_MODE	PWR_KEY_SLIDE_SWITCH
#endif		


////////////////////////////////////////////////////////////////////////////////////////////////////////////			
//˵����SD����������ѡ��
////////////////////////////////////////////////////////////////////////////////////////////////////////////			
#ifdef	FUNC_CARD_EN	
#define CARD_BUS_A3A4A5_EN
//#define CARD_BUS_A0E2E3_EN
//#define CARD_BUS_D5D6D7_EN
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////			
// SD������Ͱγ����ķ�ʽ������
////////////////////////////////////////////////////////////////////////////////////////////////////////////			
#ifdef	FUNC_CARD_EN
#if defined(AU6210K_NR_D_8_CSRBT) || defined(AU6210K_ZB_BT007_CSR)
#else
#define SD_DETECT_HW_SW				//���忨�ļ�ⷽʽΪ��Ӳ����ϼ�ⷽʽ(�Ƽ�)
#endif

#ifdef SD_DETECT_HW_SW
//���忨����Ӳ������pin��
//��GPIO��Դ����ʱ��Ҳ���Զ���һ��������GPIO����ΪӲ�����ţ��Ӷ���ʡһ������
#if defined(AU6210K_JLH_JH82B) || defined(AU6210K_ZB_BT007_CSR)
#else
#define SD_DETECT_PIN_USE_A4	//ʹ��A4����Ϊ����ʱ�򿪴˺궨��(���Խ�ԼGPIO��Դ)
//#define SD_DETECT_PIN_USE_E3	//ʹ��E3����Ϊ����ʱ�򿪴˺궨��(���Խ�ԼGPIO��Դ)
//#define SD_DETECT_PIN_USE_D6	//ʹ��D6����Ϊ����ʱ�򿪴˺궨��(���Խ�ԼGPIO��Դ)
//#define SD_DETECT_PIN_USE_A1	//ʹ��A1����Ϊ����ʱ�򿪴˺궨��(���Խ�ԼGPIO��Դ)

#endif
#if defined(SD_DETECT_PIN_USE_A4)
	#define	CARD_DETECT_PORT_IN			GPIO_A_IN
	#define	CARD_DETECT_PORT_OE			GPIO_A_OE
	#define	CARD_DETECT_PORT_DS			GPIO_A_DS
	#define	CARD_DETECT_PORT_IE			GPIO_A_IE
	#define	CARD_DETECT_PORT_PU			GPIO_A_PU
	#define	CARD_DETECT_PORT_PD			GPIO_A_PD
	#define	CARD_DETECT_BIT				(1 << 4)
#elif defined(SD_DETECT_PIN_USE_A1)
	#define	CARD_DETECT_PORT_IN			GPIO_A_IN
	#define	CARD_DETECT_PORT_OE			GPIO_A_OE
	#define	CARD_DETECT_PORT_DS			GPIO_A_DS
	#define	CARD_DETECT_PORT_IE			GPIO_A_IE
	#define	CARD_DETECT_PORT_PU			GPIO_A_PU
	#define	CARD_DETECT_PORT_PD			GPIO_A_PD
	#define	CARD_DETECT_BIT				(1 << 1)
#elif defined(SD_DETECT_PIN_USE_E3)
	#define	CARD_DETECT_PORT_IN			GPIO_E_IN
	#define	CARD_DETECT_PORT_OE			GPIO_E_OE
	#define	CARD_DETECT_PORT_DS			GPIO_E_DS
	#define	CARD_DETECT_PORT_IE			GPIO_E_IE
	#define	CARD_DETECT_PORT_PU			GPIO_E_PU
	#define	CARD_DETECT_PORT_PD			GPIO_E_PD
	#define	CARD_DETECT_BIT				(1 << 3)
#elif defined(SD_DETECT_PIN_USE_D6)
	#define	CARD_DETECT_PORT_IN			GPIO_D_IN
	#define	CARD_DETECT_PORT_OE			GPIO_D_OE
	#define	CARD_DETECT_PORT_DS			GPIO_D_DS
	#define	CARD_DETECT_PORT_IE			GPIO_D_IE
	#define	CARD_DETECT_PORT_PU			GPIO_D_PU
	#define	CARD_DETECT_PORT_PD			GPIO_D_PD
	#define	CARD_DETECT_BIT				(1 << 6)
#elif defined(AU6210K_JLH_JH82B)
	#define	CARD_DETECT_PORT_IN			GPIO_E_IN
	#define	CARD_DETECT_PORT_OE			GPIO_E_OE
	#define	CARD_DETECT_PORT_DS			GPIO_E_DS
	#define	CARD_DETECT_PORT_IE			GPIO_E_IE
	#define	CARD_DETECT_PORT_PU			GPIO_E_PU
	#define	CARD_DETECT_PORT_PD			GPIO_E_PD
	#define	CARD_DETECT_BIT				(1 << 0)
#else
	#define	CARD_DETECT_PORT_IN			GPIO_A_IN
	#define	CARD_DETECT_PORT_OE			GPIO_A_OE
	#define	CARD_DETECT_PORT_DS			GPIO_A_DS
	#define	CARD_DETECT_PORT_IE			GPIO_A_IE
	#define	CARD_DETECT_PORT_PU			GPIO_A_PU
	#define	CARD_DETECT_PORT_PD			GPIO_A_PD
	#define	CARD_DETECT_BIT				(1 << 2)
#endif
#else
	   //���û�ж��忨�ļ�ⷽʽΪ��Ӳ����Ϸ�ʽ��ϵͳ�򰴴������ⷽʽ�����Ĳ�����
	   //�������ⷽʽ�£�ϵͳ�ķ�����Ż���!
#endif
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////			
// LINEIN�����������(line in pin configure)
// LINEIN_DETECT_PORT_IN ~ LINEIN_DETECT_BIT��LINEIN����Ͱγ��ļ��ţ�����E2
////////////////////////////////////////////////////////////////////////////////////////////////////////////			
#ifdef FUNC_LINEIN_EN
#if defined(AU6210K_JLH_JH82B)
	#define LINEIN_DETECT_PORT_IN		GPIO_D_IN
	#define LINEIN_DETECT_PORT_OE		GPIO_D_OE
	#define LINEIN_DETECT_PORT_IE		GPIO_D_IE
	#define LINEIN_DETECT_PORT_PU		GPIO_D_PU
	#define LINEIN_DETECT_PORT_PD		GPIO_D_PD	
	#define LINEIN_DETECT_BIT			(1 << 4)
#elif 1//defined(AU6210K_CSR_BT) || defined(AU6210K_NR_D_8_CSRBT)
	#define	LINEIN_DETECT_PORT_IN		GPIO_B_IN
	#define	LINEIN_DETECT_PORT_OE		GPIO_B_OE
	#define LINEIN_DETECT_PORT_IE		GPIO_B_IE
	#define	LINEIN_DETECT_PORT_PU		GPIO_B_PU
	#define	LINEIN_DETECT_PORT_PD		GPIO_B_PD
	#define LINEIN_DETECT_BIT			(1 << 4)
#else
	#define	LINEIN_DETECT_PORT_IN		GPIO_E_IN
	#define	LINEIN_DETECT_PORT_OE		GPIO_E_OE
	#define LINEIN_DETECT_PORT_IE		GPIO_E_IE
	#define	LINEIN_DETECT_PORT_PU		GPIO_E_PU
	#define	LINEIN_DETECT_PORT_PD		GPIO_E_PD	
#if !(defined(AU6210K_DEMO))
	//#define LINEIN_DETECT_BIT			(1 << 2)
#endif
#endif
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////			
// ��ת����IO����(coding key pin configure)
// CODING_KEY_A_BIT ~ CODING_KEY_B_BIT����ת���ص�A,B�������ã�����D7,D3
////////////////////////////////////////////////////////////////////////////////////////////////////////////			
#ifdef FUNC_CODING_KEY_EN	
	#define CODING_KEY_A_PORT_IN	GPIO_D_IN	
	#define CODING_KEY_A_PORT_IE	GPIO_D_IE
	#define CODING_KEY_A_PORT_OE	GPIO_D_OE			
	#define CODING_KEY_A_PORT_PU	GPIO_D_PU 
	#define CODING_KEY_A_PORT_PD	GPIO_D_PD	
	#define CODING_KEY_A_PORT_DS	GPIO_D_DS	
	#define	CODING_KEY_A_BIT		(1 << 7) 
	
	#define	CODING_KEY_B_PORT_IN	GPIO_D_IN 	
	#define CODING_KEY_B_PORT_IE	GPIO_D_IE
	#define	CODING_KEY_B_PORT_OE	GPIO_D_OE 			
	#define	CODING_KEY_B_PORT_PU	GPIO_D_PU 
	#define	CODING_KEY_B_PORT_PD	GPIO_D_PD 		
	#define CODING_KEY_B_PORT_DS	GPIO_D_DS	
	#define	CODING_KEY_B_BIT		(1 << 3)

	//�жϲ�������
	#define CODING_KEY_A_PORT_PTRIG		GPIO_D_SEP_PTRIG	
	#define CODING_KEY_A_PORT_INTC		GPIO_D_SEP_INTC	
	#define CODING_KEY_A_PORT_INTE		GPIO_D_SEP_INTE	
	#define	CODING_KEY_A_PORT_INTS		GPIO_D_SEP_INTS
#endif 


////////////////////////////////////////////////////////////////////////////////////////////////////////////			
// �ⲿMute��·IO����(external mute pin configure)
// FUNC_EXMUTE_EN���Ƿ�֧��IO��������ʹ��
// EXMUTE_PORT_IE ~ EXMUTE_PORT_OUT���ⲿMute�������ã�����A6
////////////////////////////////////////////////////////////////////////////////////////////////////////////			
#define FUNC_EXMUTE_EN

#ifdef FUNC_EXMUTE_EN
#if 0
	#define EXMUTE_PORT_IE				GPIO_B_IE
	#define EXMUTE_PORT_OE				GPIO_B_OE
	#define EXMUTE_PORT_PU				GPIO_B_PU
	#define EXMUTE_PORT_PD				GPIO_B_PD
	#define EXMUTE_PORT_BIT				(1 << 6)   //B6
	#define EXMUTE_PORT_OUT				GPIO_B_OUT
#elif defined(AU6210K_JLH_JH82B)
	#define EXMUTE_PORT_IE				GPIO_D_IE
	#define EXMUTE_PORT_OE				GPIO_D_OE
	#define EXMUTE_PORT_PU				GPIO_D_PU
	#define EXMUTE_PORT_PD				GPIO_D_PD
	#define EXMUTE_PORT_BIT				(1 << 6)   //D6
	#define EXMUTE_PORT_OUT				GPIO_D_OUT
#elif 	defined(AU6210K_NR_D_9X_XJ_HTS) ||\
		defined(AU6210K_NR_D_8_CSRBT) || \
		defined(AU6210K_LK_SJ_CSRBT) ||\
		defined(AU6210K_ZB_BT007_CSR)
	#define EXMUTE_PORT_IE				GPIO_D_IE
	#define EXMUTE_PORT_OE				GPIO_D_OE
	#define EXMUTE_PORT_PU				GPIO_D_PU
	#define EXMUTE_PORT_PD				GPIO_D_PD
	#define EXMUTE_PORT_BIT				(1 << 4)   //D6
	#define EXMUTE_PORT_OUT				GPIO_D_OUT
#elif defined(AU6210K_HXX_B002)
	#define EXMUTE_PORT_IE				GPIO_A_IE
	#define EXMUTE_PORT_OE				GPIO_A_OE
	#define EXMUTE_PORT_PU				GPIO_A_PU
	#define EXMUTE_PORT_PD				GPIO_A_PD
	#define EXMUTE_PORT_BIT				(1 << 1)   //A1
	#define EXMUTE_PORT_OUT				GPIO_A_OUT
#else
	#define EXMUTE_PORT_IE				GPIO_A_IE
	#define EXMUTE_PORT_OE				GPIO_A_OE
	#define EXMUTE_PORT_PU				GPIO_A_PU
	#define EXMUTE_PORT_PD				GPIO_A_PD
	#define EXMUTE_PORT_BIT 			(1 << 2)   //A6
	#define EXMUTE_PORT_OUT 			GPIO_A_OUT
#endif
	#if defined(AU6210K_NR_D_9X) || defined(AU6210K_NR_D_8_CSRBT) || defined(AU6210K_ZB_BT007_CSR)
        #define ExMuteOn()	do{ \
							SetGpioRegBit(EXMUTE_PORT_PU, EXMUTE_PORT_BIT); \
							ClrGpioRegBit(EXMUTE_PORT_PD, EXMUTE_PORT_BIT); \
							ClrGpioRegBit(EXMUTE_PORT_IE, EXMUTE_PORT_BIT); \	
							SetGpioRegBit(EXMUTE_PORT_OE, EXMUTE_PORT_BIT); \
							ClrGpioRegBit(EXMUTE_PORT_OUT, EXMUTE_PORT_BIT); \
						}while(0)
	
	    #define ExMuteOff()	do{ \
							SetGpioRegBit(EXMUTE_PORT_PU, EXMUTE_PORT_BIT); \
							ClrGpioRegBit(EXMUTE_PORT_PD, EXMUTE_PORT_BIT); \
							ClrGpioRegBit(EXMUTE_PORT_IE, EXMUTE_PORT_BIT); \	
							SetGpioRegBit(EXMUTE_PORT_OE, EXMUTE_PORT_BIT); \
							SetGpioRegBit(EXMUTE_PORT_OUT, EXMUTE_PORT_BIT); \
						}while(0)
	#else	//��mute
	    #define ExMuteOn()	do{ \
							SetGpioRegBit(EXMUTE_PORT_PU, EXMUTE_PORT_BIT); \
							ClrGpioRegBit(EXMUTE_PORT_PD, EXMUTE_PORT_BIT); \
							ClrGpioRegBit(EXMUTE_PORT_IE, EXMUTE_PORT_BIT); \	
							SetGpioRegBit(EXMUTE_PORT_OE, EXMUTE_PORT_BIT); \
							SetGpioRegBit(EXMUTE_PORT_OUT, EXMUTE_PORT_BIT); \
						}while(0)
	
	    #define ExMuteOff()	do{ \
							SetGpioRegBit(EXMUTE_PORT_PU, EXMUTE_PORT_BIT); \
							ClrGpioRegBit(EXMUTE_PORT_PD, EXMUTE_PORT_BIT); \
							ClrGpioRegBit(EXMUTE_PORT_IE, EXMUTE_PORT_BIT); \	
							SetGpioRegBit(EXMUTE_PORT_OE, EXMUTE_PORT_BIT); \
							ClrGpioRegBit(EXMUTE_PORT_OUT, EXMUTE_PORT_BIT); \
						}while(0)
    #endif
//	#define IsExMuteOn() (!(GetGpioReg(EXMUTE_PORT_OUT) & EXMUTE_PORT_BIT)) 										
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////			
// ˵����Headphone�����������(Headphones in pin configure)
//FUNC_HEADPHONE_DETECT_EN���Ƿ�֧��HEADPHONE����Ͱγ����ʹ��
//HEADPHONE_DETECT_PORT_IN ~ HEADPHONE_DETECT_BIT��HEADPHONE����Ͱγ��ļ��ţ�����A7
////////////////////////////////////////////////////////////////////////////////////////////////////////////	
#if defined(AU6210K_LK_SJ_CSRBT)
#else
//#define FUNC_HEADPHONE_DETECT_EN
#endif

#ifdef FUNC_HEADPHONE_DETECT_EN
//	#define 03FUNC_HEADPHONE_ADC_DETECT_EN	//����ADC �ڼ���������״̬�򿪴˺궨��

#if	0
#define FUNC_HEADPHONE_SINGLE_LED_EN		//���������뵥LED�ƿڸ���ʱ���򿪸ú�
#endif

#ifdef FUNC_HEADPHONE_ADC_DETECT_EN
	#define HEADPHONE_ADC_PORT_CH	ADC_CHANNEL_B6	//ѡ��������ʹ�õ�ADC �˿�
//	#define HEADPHONE_ADC_PU_EN		//�򿪸ú궨��, ADC����ѡ������������Ʒ�ʽ,����ѡ���������跽ʽ
#elif 1//defined(AU6210K_CSR_BT)
	#define HEADPHONE_DETECT_PORT_IN 		GPIO_E_IN
	#define HEADPHONE_DETECT_PORT_OE 		GPIO_E_OE
	#define HEADPHONE_DETECT_PORT_DS 		GPIO_E_DS
	#define HEADPHONE_DETECT_PORT_IE 		GPIO_E_IE
	#define HEADPHONE_DETECT_PORT_PU 		GPIO_E_PU
	#define HEADPHONE_DETECT_PORT_PD 		GPIO_E_PD
	#define HEADPHONE_DETECT_BIT 			(1 << 2)
#else
	#define HEADPHONE_DETECT_PORT_IN 		GPIO_A_IN
	#define HEADPHONE_DETECT_PORT_OE 		GPIO_A_OE
	#define HEADPHONE_DETECT_PORT_DS 		GPIO_A_DS
	#define HEADPHONE_DETECT_PORT_IE 		GPIO_A_IE
	#define HEADPHONE_DETECT_PORT_PU 		GPIO_A_PU
	#define HEADPHONE_DETECT_PORT_PD 		GPIO_A_PD
	#define HEADPHONE_DETECT_BIT 			(1 << 2)
#endif
#endif

#if defined(AU6210K_NR_D_9X_XJ_HTS)
#define FUNC_USB_VOLUP_EN	//usb��ѹ
#endif

#ifdef FUNC_USB_VOLUP_EN
	#define USB_VOLUP_PORT_IE				GPIO_D_IE
	#define USB_VOLUP_PORT_OE				GPIO_D_OE
	#define USB_VOLUP_PORT_PU				GPIO_D_PU
	#define USB_VOLUP_PORT_PD				GPIO_D_PD
	//#define USB_VOLUP_PORT_BIT			(1 << 2)   //A0
	#define USB_VOLUP_PORT_OUT			GPIO_D_OUT
	#define USB_VOLUP_On()	do{ \
										SetGpioRegBit(POWER_AMPLIFIER_PORT_PU, POWER_AMPLIFIER_PORT_BIT); \
										ClrGpioRegBit(POWER_AMPLIFIER_PORT_PD, POWER_AMPLIFIER_PORT_BIT); \
										ClrGpioRegBit(POWER_AMPLIFIER_PORT_IE, POWER_AMPLIFIER_PORT_BIT); \	
										SetGpioRegBit(POWER_AMPLIFIER_PORT_OE, POWER_AMPLIFIER_PORT_BIT); \
										SetGpioRegBit(POWER_AMPLIFIER_PORT_OUT, POWER_AMPLIFIER_PORT_BIT); \
									}while(0)
	
	#define USB_VOLUP_Off()	do{ \
										SetGpioRegBit(POWER_AMPLIFIER_PORT_PU, POWER_AMPLIFIER_PORT_BIT); \
										ClrGpioRegBit(POWER_AMPLIFIER_PORT_PD, POWER_AMPLIFIER_PORT_BIT); \
										ClrGpioRegBit(POWER_AMPLIFIER_PORT_IE, POWER_AMPLIFIER_PORT_BIT); \	
										SetGpioRegBit(POWER_AMPLIFIER_PORT_OE, POWER_AMPLIFIER_PORT_BIT); \
										ClrGpioRegBit(POWER_AMPLIFIER_PORT_OUT, POWER_AMPLIFIER_PORT_BIT); \
									}while(0)
#endif


#if  defined(AU6210K_ZB_BT007_CSR) // || !defined(AU6210K_NR_D_8_CSRBT) || defined(AU6210K_LK_SJ_CSRBT) 
#define FUNC_POWER_AMPLIFIER_EN	//AB��D�๦��ѡ��
#endif

#ifdef FUNC_POWER_AMPLIFIER_EN
#if 1//defined(AU6210K_NR_D_9X_XJ_HTS)||defined(AU6210K_ZB_BT007_CSR)
    #define POWER_AMPLIFIER_PORT_IE				GPIO_D_IE
	#define POWER_AMPLIFIER_PORT_OE				GPIO_D_OE
	#define POWER_AMPLIFIER_PORT_PU				GPIO_D_PU
	#define POWER_AMPLIFIER_PORT_PD				GPIO_D_PD
	#define POWER_AMPLIFIER_PORT_BIT			(1 << 3)   //
	#define POWER_AMPLIFIER_PORT_OUT			GPIO_D_OUT
#else
	#define POWER_AMPLIFIER_PORT_IE				GPIO_E_IE
	#define POWER_AMPLIFIER_PORT_OE				GPIO_E_OE
	#define POWER_AMPLIFIER_PORT_PU				GPIO_E_PU
	#define POWER_AMPLIFIER_PORT_PD				GPIO_E_PD
	#define POWER_AMPLIFIER_PORT_BIT			(1 << 1)   //A0
	#define POWER_AMPLIFIER_PORT_OUT			GPIO_E_OUT
#endif	
	#define ABPowerAmplifierOn()	do{ \
										SetGpioRegBit(POWER_AMPLIFIER_PORT_PU, POWER_AMPLIFIER_PORT_BIT); \
										ClrGpioRegBit(POWER_AMPLIFIER_PORT_PD, POWER_AMPLIFIER_PORT_BIT); \
										ClrGpioRegBit(POWER_AMPLIFIER_PORT_IE, POWER_AMPLIFIER_PORT_BIT); \	
										SetGpioRegBit(POWER_AMPLIFIER_PORT_OE, POWER_AMPLIFIER_PORT_BIT); \
										SetGpioRegBit(POWER_AMPLIFIER_PORT_OUT, POWER_AMPLIFIER_PORT_BIT); \
									}while(0)
	
	#define ABPowerAmplifierOff()	do{ \
										SetGpioRegBit(POWER_AMPLIFIER_PORT_PU, POWER_AMPLIFIER_PORT_BIT); \
										ClrGpioRegBit(POWER_AMPLIFIER_PORT_PD, POWER_AMPLIFIER_PORT_BIT); \
										ClrGpioRegBit(POWER_AMPLIFIER_PORT_IE, POWER_AMPLIFIER_PORT_BIT); \	
										SetGpioRegBit(POWER_AMPLIFIER_PORT_OE, POWER_AMPLIFIER_PORT_BIT); \
										ClrGpioRegBit(POWER_AMPLIFIER_PORT_OUT, POWER_AMPLIFIER_PORT_BIT); \
									}while(0)
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////			
// I2C IO����(IIC pin configure)
////////////////////////////////////////////////////////////////////////////////////////////////////////////			
// I2C SDA and CLK pin define


//#define I2C_FUNC

#define SDIO_TO_OTHER_GPIO	0	// I2C����ʹ�ö���GPIO
#define SDIO_TO_A3A4A5	1	// I2C������SD������A3A4A5����
#define SDIO_TO_A0E2E3	2	// I2C������SD������A0E2E3����
#define SDIO_TO_D5D6D7	3	// I2C������SD������D5D6D7����

#define I2C_PIN_CONFIGURE	SDIO_TO_OTHER_GPIO	

#if (I2C_PIN_CONFIGURE == SDIO_TO_A3A4A5)
	//SCL: GPIO_A[4]
	#define PORT_OUT_SCL		GPIO_A_OUT
	#define PORT_IN_SCL			GPIO_A_IN
	#define PORT_IE_SCL			GPIO_A_IE
	#define PORT_OE_SCL			GPIO_A_OE
	#define PORT_DS_SCL			GPIO_A_DS
	#define PORT_PU_SCL			GPIO_A_PU
	#define PORT_PD_SCL			GPIO_A_PD
	#define	MASK_BIT_SCL 		(1 << 4)		
	
	//SDA: GPIO_A[3]
	#define PORT_OUT_SDA		GPIO_A_OUT
	#define PORT_IN_SDA			GPIO_A_IN
	#define PORT_IE_SDA			GPIO_A_IE
	#define PORT_OE_SDA			GPIO_A_OE
	#define PORT_DS_SDA			GPIO_A_DS
	#define PORT_PU_SDA			GPIO_A_PU
	#define PORT_PD_SDA			GPIO_A_PD
	#define	MASK_BIT_SDA 		(1 << 3)

#elif (I2C_PIN_CONFIGURE == SDIO_TO_A0E2E3)

#elif (I2C_PIN_CONFIGURE == SDIO_TO_D5D6D7)

#else

#ifdef AU6210K_NR_D_8_CSRBT //pt231x
//SCL: GPIO_B[2]
	#define PORT_OUT_SCL		GPIO_A_OUT
	#define PORT_IN_SCL			GPIO_A_IN
	#define PORT_IE_SCL			GPIO_A_IE
	#define PORT_OE_SCL			GPIO_A_OE
	#define PORT_DS_SCL			GPIO_A_DS
	#define PORT_PU_SCL			GPIO_A_PU
	#define PORT_PD_SCL			GPIO_A_PD
	#define	MASK_BIT_SCL 		(1 << 6)		
	
	//SDA: GPIO_B[3]
	#define PORT_OUT_SDA		GPIO_A_OUT
	#define PORT_IN_SDA			GPIO_A_IN
	#define PORT_IE_SDA			GPIO_A_IE
	#define PORT_OE_SDA			GPIO_A_OE
	#define PORT_DS_SDA			GPIO_A_DS
	#define PORT_PU_SDA			GPIO_A_PU
	#define PORT_PD_SDA			GPIO_A_PD
	#define	MASK_BIT_SDA 		(1 << 7)
#else
	//SCL: GPIO_B[2]
	
	#define PORT_OUT_SCL		GPIO_B_OUT
	#define PORT_IN_SCL			GPIO_B_IN
	#define PORT_IE_SCL			GPIO_B_IE
	#define PORT_OE_SCL			GPIO_B_OE
	#define PORT_DS_SCL			GPIO_B_DS
	#define PORT_PU_SCL			GPIO_B_PU
	#define PORT_PD_SCL			GPIO_B_PD
	#define	MASK_BIT_SCL 		(1 << 2)		
	
	//SDA: GPIO_B[3]
	#define PORT_OUT_SDA		GPIO_B_OUT
	#define PORT_IN_SDA			GPIO_B_IN
	#define PORT_IE_SDA			GPIO_B_IE
	#define PORT_OE_SDA			GPIO_B_OE
	#define PORT_DS_SDA			GPIO_B_DS
	#define PORT_PU_SDA			GPIO_B_PU
	#define PORT_PD_SDA			GPIO_B_PD
	#define	MASK_BIT_SDA 		(1 << 3)
#endif	
#endif	

#ifdef AU6210K_NR_D_8_CSRBT //D8��ЧICѡ��
#define FUNC_NPCA110X_EN
//#define FUNC_PT231X_EN
#endif

#ifdef FUNC_PT231X_EN
#define FUNC_PT231X_IIC     //PT231X��iicͨѶ
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////			
// Define system volume range.
////////////////////////////////////////////////////////////////////////////////////////////////////////////			
#if defined(AU6210K_DEMO)||	defined(AU6210K_LK_SJ_CSRBT) || defined(AU6210K_ZB_BT007_CSR)
#define	VOLUME_MAX		32	
#define	VOLUME_INIT		25	
#elif defined(AU6210K_NR_D_8_CSRBT) 
#define	VOLUME_MAX		42	
#define	VOLUME_INIT		38	
#elif defined(AU6210K_NR_D_9X_XJ_HTS) 
#define	VOLUME_MAX		16		
#define	VOLUME_INIT		16		
#endif
#define	VOLUME_MIN		0


////////////////////////////////////////////////////////////////////////////////////////////////////////////			
// �豸ģʽ�б������޸�
// ����ͻ���Ҫ�����µĹ���ģʽ���������SYS_MODE_OTPDEV֮��
////////////////////////////////////////////////////////////////////////////////////////////////////////////			
typedef	enum _SYS_MODE
{
	SYS_MODE_NONE,
	SYS_MODE_USB,
	SYS_MODE_SD,
	SYS_MODE_LINEIN,
	SYS_MODE_RADIO,
	SYS_MODE_RTC,
	SYS_MODE_AUDIO,			//������USB����ģʽ
	SYS_MODE_READER, 		//������USB������ģʽ
	SYS_MODE_AUDIO_READER,	//һ��ͨģʽ��PCͬʱʶ���USB������USB������
	SYS_MODE_SLEEP,
	SYS_MODE_STANDBY,
	SYS_MODE_SPIDEV,
	SYS_MODE_BLUETOOTH,
	SYS_MODE_OTPDEV

} SYS_MODE;

#endif
