#include <string.h>
#include <stdio.h>

#include "type.h"										//include project header files
#include "utility.h"
#include "syscfg.h"

#include "sysctrl.h"
#include "timer.h"
#include "display.h"
#include "seg_panel.h"
#include "playctrl.h"
#include "breakpoint.h"
#include "seg_led_disp.h"
#include "debug.h"


#ifdef FUNC_SEG_LED_EN

extern BOOL IsUSBLink(VOID);
extern BOOL IsCardLink(VOID);

#ifdef FUNC_POWER_ON_AUDIO_EN
extern BOOL gUSBChargePowerOnFlag;	
#endif

extern WORD DispHoldTime;

#define LED57_COM_NUM	5
#define LED57_SEG_NUM	7


#ifdef FUNC_SEG_LED_EN
BYTE CODE Led57IconSet[ICON_NUM] = {ICON57_PLAY, ICON57_PAUSE, ICON57_COLON, ICON57_USB, ICON57_SD, ICON57_FM_MHZ, ICON57_MP3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,0xFF,0xFF,0xFF};
BYTE CODE Led48IconSet[ICON_NUM] = {0xFF, 0xFF, ICON48_COLON, 0xFF, 0xFF, ICON48_FM_MHZ, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
#endif

BOOL gLedDispRefreshFlag = FALSE;

// 显示刷新.
// Call by display refresh ISR.
VOID LedFlushDisp(VOID)
{
#if (defined(FUNC_7PIN_SEG_LED_EN) || defined(FUNC_6PIN_SEG_LED_EN))
	static BYTE ScanPinNum = 0;

	if(gLedDispRefreshFlag == FALSE)
	{
		return;
	}	
	
	ScanPinNum++;
	
	if(ScanPinNum >= (MAX_LED_PIN_NUM + 1))
	{
		ScanPinNum = 1;
	}

	LedAllPinGpioInput();
//	SetA2IcsReg(A2_ICS_FOR_GPIO, A2_ICS_CLOSE); 

	// LED 各个Pin 脚依次扫描
#if (defined(AU6210HD_7PIN_LED_DEMO)) 
//7只脚段码LED屏驱动扫描示例
	switch(ScanPinNum)
	{
		case 1:
			if(gDispBuff[0] & SEG_A) 
			{			
				LED_PIN2_IN_ON;
			}
			
			if(gDispBuff[0] & SEG_B)
			{
				LED_PIN3_IN_ON;		
			}
			
			if(gDispBuff[0] & SEG_E) 
			{
				LED_PIN4_IN_ON;
			}
			
			if(gDispBuff[4] & SEG_A) 
			{
				LED_PIN6_IN_ON;
			}	//PLAY
			
			if(gDispBuff[4] & SEG_D) 
			{
				LED_PIN5_IN_ON;
			}	//SD		
			
			if(gDispBuff[3] & SEG_C) 
			{
				LED_PIN7_IN_ON;
			}  		
			LED_PIN1_OUT_HIGH;
			break;

		case 2:
			if(gDispBuff[0] & SEG_F) 
			{ 
				LED_PIN1_IN_ON;	
			}
			
			if(gDispBuff[1] & SEG_A) 
			{
				LED_PIN3_IN_ON;		
			}
			
			if(gDispBuff[1] & SEG_B) 
			{
				LED_PIN4_IN_ON;		
			}
			
			if(gDispBuff[1] & SEG_E) 
			{
				LED_PIN5_IN_ON;		
			}  
			
			if(gDispBuff[1] & SEG_D) 
			{
				LED_PIN6_IN_ON;		
			} 		
			
			if(gDispBuff[3] & SEG_G) 
			{
				LED_PIN7_IN_ON;		
			}  		
			LED_PIN2_OUT_HIGH;	
			break;

		case 3:
			if(gDispBuff[0] & SEG_G) 
			{
				LED_PIN1_IN_ON;		
			}
			
			if(gDispBuff[1] & SEG_F) 
			{
				LED_PIN2_IN_ON;
			}
			
			if(gDispBuff[4] & SEG_E) 
			{
				LED_PIN4_IN_ON;			
			}  //K5 DOT
			
			if(gDispBuff[2] & SEG_B) 
			{
				LED_PIN5_IN_ON;		
			} 
			
			if(gDispBuff[4] & SEG_B) 
			{
				LED_PIN6_IN_ON;		
			}  //K2 PAUSE	
			
			if(gDispBuff[4] & SEG_G) 
			{
				LED_PIN7_IN_ON;		
			}  //K7 MP3	
			LED_PIN3_OUT_HIGH;			  
			break;

		case 4:
			if(gDispBuff[0] & SEG_C) 
			{
				LED_PIN1_IN_ON;		 	
			}
			
			if(gDispBuff[1] & SEG_G) 
			{
				LED_PIN2_IN_ON;		
			}
			
			if(gDispBuff[2] & SEG_F) 
			{
				LED_PIN3_IN_ON;		
			}
			
			if(gDispBuff[2] & SEG_C) 
			{
				LED_PIN5_IN_ON;		
			} 
			
			if(gDispBuff[3] & SEG_E) 
			{
				LED_PIN6_IN_ON;		
			}  	
			
			if(gDispBuff[3] & SEG_B) 
			{
				LED_PIN7_IN_ON;		
			}  	
			LED_PIN4_OUT_HIGH;		
			break;

		case 5:
			if(gDispBuff[0] & SEG_D)
			{
				LED_PIN1_IN_ON;		
			}
			
			if(gDispBuff[1] & SEG_C) 
			{
				LED_PIN2_IN_ON;		
			}
			
			if(gDispBuff[2] & SEG_G) 
			{
				LED_PIN3_IN_ON;		
			}
			
			if(gDispBuff[2] & SEG_A) 
			{
				LED_PIN4_IN_ON;
			}  
			LED_PIN5_OUT_HIGH;		
			break;

		case 6:
			if(gDispBuff[2] & SEG_D) 
			{  
				LED_PIN1_IN_ON;		
			}
			
			if(gDispBuff[4] & SEG_C) 
			{
				LED_PIN2_IN_ON;		
			}  //K3 USB
			
			if(gDispBuff[2] & SEG_E) 
			{
				LED_PIN3_IN_ON;		
			}
			
			if(gDispBuff[3] & SEG_D) 
			{
				LED_PIN4_IN_ON;		
			}  
			LED_PIN6_OUT_HIGH;		
			break;

		case 7:
			if(gDispBuff[3] & SEG_F) 
			{
				LED_PIN1_IN_ON;					
			} 
			
			if(gDispBuff[4] & SEG_F) 
			{
				LED_PIN3_IN_ON;					
			} //K6 FM
			
			if(gDispBuff[3] & SEG_A) 
			{
				LED_PIN4_IN_ON;		
			}
			LED_PIN7_OUT_HIGH;	
			break;

		default:
			break;	
	}

#elif 1
/*gDispBuff[0] = 0xFF;
gDispBuff[1] = 0xFF;
gDispBuff[2] = 0xFF;
gDispBuff[3] = 0xFF;
gDispBuff[4] = 0xFF;*/
	switch(ScanPinNum)
	{
		case 1:
			if(gDispBuff[1] & SEG_A) 
			{			
				LED_PIN2_IN_ON;
			}
			
			if(gDispBuff[1] & SEG_B)
			{
				LED_PIN3_IN_ON;		
			}
			
			if(gDispBuff[1] & SEG_C) 
			{
				LED_PIN4_IN_ON;
			}
			
			if(gDispBuff[1] & SEG_D) 
			{
				LED_PIN5_IN_ON;
			}
			
			if(gDispBuff[1] & SEG_E) 
			{
				LED_PIN6_IN_ON;
			}

			if(gDispBuff[1] & SEG_F) 
			{
				LED_PIN7_IN_ON;
			}

			LED_PIN1_OUT_HIGH;
			break;

		case 2:
			if(gDispBuff[0] & SEG_A) 
			{ 
				LED_PIN1_IN_ON;	
			}
			
			if(gDispBuff[1] & SEG_G) 
			{
				LED_PIN3_IN_ON;		
			}
			
			if(gDispBuff[4] & SEG_E) 
			{
				LED_PIN4_IN_ON;		
			}
			
			if(gDispBuff[4] & SEG_F) 
			{
				LED_PIN6_IN_ON;		
			}  
			
			if(gDispBuff[4] & SEG_G) 
			{
				LED_PIN7_IN_ON;		
			} 	

			LED_PIN2_OUT_HIGH;	
			break;

		case 3:
			if(gDispBuff[0] & SEG_B) 
			{
				LED_PIN1_IN_ON;		
			}
			
			if(gDispBuff[0] & SEG_G) 
			{
				LED_PIN2_IN_ON;
			}
			
			if(gDispBuff[3] & SEG_A) 
			{
				LED_PIN4_IN_ON;			
			}
			
			if(gDispBuff[3] & SEG_B) 
			{
				LED_PIN5_IN_ON;		
			} 
			
			if(gDispBuff[3] & SEG_C) 
			{
				LED_PIN6_IN_ON;		
			}
			
			if(gDispBuff[3] & SEG_D) 
			{
				LED_PIN7_IN_ON;		
			}

			LED_PIN3_OUT_HIGH;			  
			break;

		case 4:
			if(gDispBuff[0] & SEG_C) 
			{
				LED_PIN1_IN_ON;		 	
			}
			
			if(gDispBuff[2] & SEG_A) 
			{
				LED_PIN3_IN_ON;		
			}
			
			if(gDispBuff[3] & SEG_E) 
			{
				LED_PIN5_IN_ON;		
			} 
			
			if(gDispBuff[3] & SEG_F) 
			{
				LED_PIN6_IN_ON;		
			} 
			
			if(gDispBuff[3] & SEG_G) 
			{
				LED_PIN7_IN_ON;		
			}  	

			if(gDispBuff[4] & SEG_A) 
			{
				LED_PIN2_IN_ON;		
			}
			
			LED_PIN4_OUT_HIGH;		
			break;

		case 5:
			if(gDispBuff[0] & SEG_D)
			{
				LED_PIN1_IN_ON;		
			}
			
			if(gDispBuff[2] & SEG_B) 
			{
				LED_PIN3_IN_ON;		
			}
			
			if(gDispBuff[2] & SEG_E) 
			{
				LED_PIN4_IN_ON;		
			}
			
			if(gDispBuff[4] & SEG_B) 
			{
				LED_PIN2_IN_ON;
			}

			LED_PIN5_OUT_HIGH;		
			break;

		case 6:
			if(gDispBuff[0] & SEG_E) 
			{  
				LED_PIN1_IN_ON;		
			} 
			
			if(gDispBuff[2] & SEG_C) 
			{
				LED_PIN3_IN_ON;		
			}
			
			if(gDispBuff[2] & SEG_F) 
			{
				LED_PIN4_IN_ON;		
			} 
			
			if(gDispBuff[4] & SEG_C) 
			{
				LED_PIN2_IN_ON;		
			}  
#if 0
			if (gDispBuff[5] & SEG_A)
			{
				LED_PIN7_IN_ON;
			} //利用PIN6->PIN7的空位来点亮额外的二极管，用来显示播放状态，相应有COM6,对应图标ICON57_PLAY
#endif			
			LED_PIN6_OUT_HIGH;		
			break;

		case 7:			
			if(gDispBuff[0] & SEG_F) 
			{
				LED_PIN1_IN_ON;					
			}

			if(gDispBuff[2] & SEG_G) 
			{
				LED_PIN4_IN_ON;
			}

			if(gDispBuff[4] & SEG_D) 
			{
				LED_PIN2_IN_ON;
			}

			if(gDispBuff[2] & SEG_D) 
			{
				LED_PIN3_IN_ON;
			}
			
			LED_PIN7_OUT_HIGH;	
			break;

		default:
			break;	
	}		
#endif
	
#if 0//defined(AU6210K_6PIN_LED_DEMO)
//6只脚段码LED屏驱动扫描示例
	switch(ScanPinNum)
	{
		case 1:
			if(gDispBuff[2] & SEG_A) 
			{			
				LED_PIN2_IN_ON;
			}
			
			if(gDispBuff[2] & SEG_B)
			{
				LED_PIN3_IN_ON;		
			}
			
			if(gDispBuff[2] & SEG_C) 
			{
				LED_PIN4_IN_ON;
			}
			
			if(gDispBuff[2] & SEG_D) 
			{
				LED_PIN5_IN_ON;
			}  
			
			if(gDispBuff[2] & SEG_E) 
			{
				LED_PIN6_IN_ON;
			} 	
			LED_PIN1_OUT_HIGH;
			break;

		case 2:
			if(gDispBuff[1] & SEG_A) 
			{ 
				LED_PIN1_IN_ON;	
			}
			
			if(gDispBuff[3] & SEG_A) 
			{
				LED_PIN3_IN_ON;		
			}
			
			if(gDispBuff[3] & SEG_B) 
			{
				LED_PIN4_IN_ON;		
			}
			
			if(gDispBuff[3] & SEG_C) 
			{
				LED_PIN5_IN_ON;		
			} 
			
			if(gDispBuff[3] & SEG_D) 
			{
				LED_PIN6_IN_ON;		
			} 		
			LED_PIN2_OUT_HIGH;	
			break;

		case 3:
			if(gDispBuff[1] & SEG_B) 
			{
				LED_PIN1_IN_ON;		
			}
			
			if(gDispBuff[1] & SEG_F) 
			{
				LED_PIN2_IN_ON;
			}
			
			if(gDispBuff[3] & SEG_E)
			{
				LED_PIN4_IN_ON;			
			} 
			
			if(gDispBuff[3] & SEG_F) 
			{
				LED_PIN5_IN_ON;		
			} 
			
			if(gDispBuff[3] & SEG_G) 
			{
				LED_PIN6_IN_ON;		
			}  
			LED_PIN3_OUT_HIGH;		
			break;


		case 4:			
			if(gDispBuff[1] & SEG_C) 
			{
				LED_PIN1_IN_ON;		
			}
			
			if(gDispBuff[1] & SEG_G) 
			{
				LED_PIN2_IN_ON;		
			}
			
			if(gDispBuff[0] & SEG_B) 
			{
				LED_PIN3_IN_ON;		 	
			}
			LED_PIN4_OUT_HIGH;		
			break;

		case 5:			
			if(gDispBuff[1] & SEG_D) 
			{
				LED_PIN1_IN_ON;		
			}
			
			if(gDispBuff[2] & SEG_F) 
			{
				LED_PIN2_IN_ON;		
			}
			
			if(gDispBuff[0] & SEG_C)
			{
				LED_PIN3_IN_ON;		
			}
			LED_PIN5_OUT_HIGH;	
			break;

		case 6:
			if(gDispBuff[1] & SEG_E) 
			{
				LED_PIN1_IN_ON;		
			}  
			
			if(gDispBuff[2] & SEG_G) 
			{
				LED_PIN2_IN_ON;		
			}
			
			if(gDispBuff[4] & SEG_E) 
			{  
				LED_PIN3_IN_ON;		
			}
			LED_PIN6_OUT_HIGH;	
			break;	

		default:
			break;
	}
#endif

#else

	static BYTE DigiSw = 0;
	
	if(gLedDispRefreshFlag == FALSE)
	{
		return;
	}
	
	if((gDisplayMode < DISP_DEV_LCD58) && (gDisplayMode >= DISP_DEV_LED1888))
	{
		baGPIOCtrl[LED_COM_PORT_OUT] &= (~MASK_LED_COM);
		baGPIOCtrl[LED_SEG_PORT_1P7MA] = gDispBuff[DigiSw];	
		//baGPIOCtrl[LED_SEG_PORT_2P4MA] = gDispBuff[DigiSw];
#if (defined(AU6210K_DEMO))
		if(DigiSw > 2)
		{
			baGPIOCtrl[LED_COM_PORT_OUT] |= (LED_COM_STARTBIT << (DigiSw + 2));	
		}
		else
#endif
		{
			baGPIOCtrl[LED_COM_PORT_OUT] |= (LED_COM_STARTBIT << DigiSw);		
		}
		
		if(gDisplayMode == DISP_DEV_LED57)
		{
			if(++DigiSw >= MAX_COM_NUM)
			{
				DigiSw = 0;
			}
		}	
		else
		{
			if(++DigiSw >= 4)
			{
				DigiSw = 0;
			}
		}
	}
#endif
}


VOID DispLedIcon(ICON_IDX Icon, ICON_STATU Light)
{
	BYTE IconVal;
	BYTE IconPos;

	if(gDisplayMode == DISP_DEV_LED57)
	{
		IconVal = Led57IconSet[Icon];
	}
	else
	{
		IconVal = Led48IconSet[Icon];
	}

	if(IconVal == 0xFF)
	{
		return;
	}
	IconPos = IconVal / 8;
	IconVal = (1 << (IconVal % 8));

	if(Light == LIGHT_ON)
	{
		gDispBuff[IconPos] |= (DISP_TYPE)IconVal;
	}
	else if(Light == LIGHT_OFF)
	{
		gDispBuff[IconPos] &= (DISP_TYPE)(~IconVal);
	}
}	


VOID LedDispDevSymbol(VOID)
{
	if((gDisplayMode == DISP_DEV_LED8888) || (gDisplayMode == DISP_DEV_LED1888))
	{
		DispHoldTime = DEV_SYMBOL_INTERVAL;
		DispIcon(ICON_DP_FM_MHZ, LIGHT_OFF);
		DispIcon(ICON_COLON, LIGHT_OFF);
		switch(gSys.SystemMode)
		{
			case SYS_MODE_NONE:
#ifndef FUNC_RTC_EN				
				//DBG(("NOD3"));
				DispString(" NOD");
#else
				DispRtc();
#endif
				break;

#ifdef FUNC_USB_EN
			case SYS_MODE_USB:
				DispString(" USB");
				break;
#endif

#ifdef FUNC_CARD_EN
			case SYS_MODE_SD:
				DispString(" SD ");
				break;
#endif
		
#ifdef FUNC_LINEIN_EN
			case SYS_MODE_LINEIN:
				DispString(" AUX");
				break;
#endif

#ifdef FUNC_RADIO_EN
			case SYS_MODE_RADIO:
				DispString(" RAD");
				break;
#endif

#ifdef FUNC_AUDIO_EN
			case SYS_MODE_AUDIO:
				DispString(" PC ");
				break;
#endif

#ifdef FUNC_READER_EN
			case SYS_MODE_READER:
				DispString(" REA");
				break;
#endif

#ifdef FUNC_AUDIO_READER_EN
			case SYS_MODE_AUDIO_READER:
				DispString(" PC ");
				break;
#endif

#ifdef FUNC_RTC_EN
			case SYS_MODE_RTC:
				DispString(" RTC");
				break;
#endif
		
			default:
				DispHoldTime = 0;
				break;
		}
	}
	else if(gDisplayMode == DISP_DEV_LED57)	//ICON_MP3
	{
		DispHoldTime = DEV_SYMBOL_INTERVAL;
		switch(gSys.SystemMode)
		{
			case SYS_MODE_NONE:
#ifndef FUNC_RTC_EN		
				DispString(" NOD");
				DispIcon(ICON_USB, LIGHT_OFF);
				DispIcon(ICON_SD, LIGHT_OFF);
				DispIcon(ICON_DP_FM_MHZ, LIGHT_OFF);
				DispIcon(ICON_MP3, LIGHT_OFF);
				DispIcon(ICON_PLAY, LIGHT_OFF);
				DispIcon(ICON_PAUSE, LIGHT_OFF);
				DispIcon(ICON_COLON, LIGHT_OFF);
#else
				DispRtc();
#endif
				break;

#ifdef FUNC_USB_EN
			case SYS_MODE_USB:
				DispIcon(ICON_USB, LIGHT_ON);
				DispIcon(ICON_SD, LIGHT_OFF);
				DispIcon(ICON_DP_FM_MHZ, LIGHT_OFF);
				DispIcon(ICON_COLON, LIGHT_OFF);
				DispHoldTime = 0;
				break;
#endif

#ifdef FUNC_CARD_EN
			case SYS_MODE_SD:
				DispIcon(ICON_USB, LIGHT_OFF);
				DispIcon(ICON_SD, LIGHT_ON);
				DispIcon(ICON_DP_FM_MHZ, LIGHT_OFF);
				DispIcon(ICON_COLON, LIGHT_OFF);
				DispHoldTime = 0;
				break;
#endif
		
#ifdef FUNC_LINEIN_EN
			case SYS_MODE_LINEIN:
				DispString(" AUX");
				DispIcon(ICON_USB, LIGHT_OFF);
				DispIcon(ICON_SD, LIGHT_OFF);
				DispIcon(ICON_DP_FM_MHZ, LIGHT_OFF);
				DispIcon(ICON_MP3, LIGHT_OFF);
				DispIcon(ICON_PLAY, LIGHT_OFF);
				DispIcon(ICON_PAUSE, LIGHT_OFF);
				DispIcon(ICON_COLON, LIGHT_OFF);
				break;
#endif

#ifdef FUNC_BLUETOOTH_EN
			case SYS_MODE_BLUETOOTH:
				DispIcon(ICON_USB, LIGHT_OFF);
				DispIcon(ICON_SD, LIGHT_OFF);
				DispIcon(ICON_DP_FM_MHZ, LIGHT_OFF);

#ifdef RTC_SHOWTIME_ANYMODE
				//DispIcon(ICON_COLON, LIGHT_OFF);
#else
				DispString("BLUE");
#endif
				break;
#endif


#ifdef FUNC_RADIO_EN
			case SYS_MODE_RADIO:				
				DispIcon(ICON_USB, LIGHT_OFF);
				DispIcon(ICON_SD, LIGHT_OFF);
				DispIcon(ICON_DP_FM_MHZ, LIGHT_ON);
				DispIcon(ICON_MP3, LIGHT_OFF);
				DispIcon(ICON_PLAY, LIGHT_OFF);
				DispIcon(ICON_PAUSE, LIGHT_OFF);
				DispIcon(ICON_COLON, LIGHT_OFF);	
				if(gRadioData.CurrFreq == 0)
				{
					gRadioData.CurrFreq = RADIO_LOWER_BOUND;
				}
				Num2Char(gRadioData.CurrFreq, FALSE);
				DispHoldTime = 0;
				break;
#endif

#if (defined(FUNC_AUDIO_EN) || defined(SYS_MODE_AUDIO_READER))
			case SYS_MODE_AUDIO:
				DispString(" PC ");
				DispIcon(ICON_USB, LIGHT_OFF);
				DispIcon(ICON_SD, LIGHT_OFF);
				DispIcon(ICON_DP_FM_MHZ, LIGHT_OFF);
				DispIcon(ICON_MP3, LIGHT_OFF);
				DispIcon(ICON_PLAY, LIGHT_OFF);
				DispIcon(ICON_PAUSE, LIGHT_OFF);
				DispIcon(ICON_COLON, LIGHT_OFF);
				break;
#endif

#ifdef FUNC_RTC_EN
			case SYS_MODE_RTC:
				DispString("TIME");
				DispIcon(ICON_USB, LIGHT_OFF);
				DispIcon(ICON_SD, LIGHT_OFF);
				DispIcon(ICON_DP_FM_MHZ, LIGHT_OFF);
				DispIcon(ICON_MP3, LIGHT_OFF);
				DispIcon(ICON_PLAY, LIGHT_OFF);
				DispIcon(ICON_PAUSE, LIGHT_OFF);
				DispIcon(ICON_COLON, LIGHT_OFF);
				break;
#endif

#ifdef FUNC_STANDBY_EN
			case SYS_MODE_STANDBY:	
				DispIcon(ICON_USB, LIGHT_OFF);
				DispIcon(ICON_SD, LIGHT_OFF);
				DispIcon(ICON_DP_FM_MHZ, LIGHT_OFF);
				DispIcon(ICON_MP3, LIGHT_OFF);
				DispIcon(ICON_PLAY, LIGHT_OFF);
				DispIcon(ICON_PAUSE, LIGHT_OFF);
				DispIcon(ICON_COLON, LIGHT_OFF);
#ifndef FUNC_RTC_EN
				DispString(" CLO"); 
#else
				DispRtc();
#endif				
				break;
#endif
		
			default:
				DispIcon(ICON_USB, LIGHT_OFF);
				DispIcon(ICON_SD, LIGHT_OFF);
				DispIcon(ICON_DP_FM_MHZ, LIGHT_OFF);
				DispIcon(ICON_MP3, LIGHT_OFF);
				DispIcon(ICON_PLAY, LIGHT_OFF);
				DispIcon(ICON_PAUSE, LIGHT_OFF);
				DispIcon(ICON_COLON, LIGHT_OFF);
				DispHoldTime = 0;
				break;
		}
	}
}


VOID LedDispRepeat(VOID)
{
	if(gPlayCtrl.RepeatMode == REPEAT_ALL)
	{
		DispString(" ALL");
	}
	else if(gPlayCtrl.RepeatMode == REPEAT_ONE)
	{
		DispString(" ONE");
	}
	else if(gPlayCtrl.RepeatMode == REPEAT_RANDOM)
	{
		DispString(" RAN");
	}
	else if(gPlayCtrl.RepeatMode == REPEAT_INTRO)
	{
		DispString("INTO");
	}
	else if(gPlayCtrl.RepeatMode == REPEAT_FOLDER)
	{
		DispString("FLOD");
	}
	DispIcon(ICON_COLON, LIGHT_OFF);
	DispIcon(ICON_DP_FM_MHZ, LIGHT_OFF);	
}


VOID LedDispPlayState(VOID)
{
#if (defined(FUNC_USB_EN) || defined(FUNC_CARD_EN))
	if(gDisplayMode == DISP_DEV_LED57)
	{
		if(gPlayCtrl.State == PLAY_STATE_PAUSE)
		{
			//DBG(("LED57||\n"));
			DispPlayTime(DIS_PLAYTIME_PAUSE);
			DispIcon(ICON_PLAY, LIGHT_OFF);
			DispIcon(ICON_PAUSE, LIGHT_ON);
			DispIcon(ICON_COLON, LIGHT_ON);
		}
		else if((gPlayCtrl.State == PLAY_STATE_PLAY) || (gPlayCtrl.State == PLAY_STATE_FF) || (gPlayCtrl.State == PLAY_STATE_FB))
		{
			DispIcon(ICON_PLAY, LIGHT_ON);
			DispIcon(ICON_PAUSE, LIGHT_OFF);
			DispIcon(ICON_COLON, LIGHT_ON);
		}
		else // if((gPlayCtrl.State == PLAY_STATE_STOP) || (gPlayCtrl.State == PLAY_STATE_IDLE) || (gPlayCtrl.State == PLAY_STATE_BROWSE))
		{
			DispIcon(ICON_PLAY, LIGHT_OFF);
			DispIcon(ICON_PAUSE, LIGHT_OFF);
			DispIcon(ICON_COLON, LIGHT_OFF);
			if(gPlayCtrl.State == PLAY_STATE_STOP)
			{
				//DBG(("LED57 STOP"));
#ifdef FUNC_USB_EN
				if((gSys.SystemMode == SYS_MODE_USB) && IsUSBLink())
				{
					DispString("STOP");
				}
#endif

#ifdef FUNC_CARD_EN
				if((gSys.SystemMode == SYS_MODE_SD) && IsCardLink())
				{
					DispString("STOP");
				}
#endif
			}
		}
		DispIcon(ICON_DP_FM_MHZ, LIGHT_OFF);
	}
	else //if((gDisplayMode == DISP_DEV_LED8888) || (gDisplayMode == DISP_DEV_LED1888))
	{
		if(gPlayCtrl.State == PLAY_STATE_PAUSE) 
		{
			DispString(" PAU");
		}
		else if(gPlayCtrl.State == PLAY_STATE_STOP)//else if(gPlayCtrl.State == PLAY_STATE_STOP)
		{
			//DBG(("LED STOP\n"));
#ifdef FUNC_USB_EN
			if((gSys.SystemMode == SYS_MODE_USB) && IsUSBLink())
			{
				DispString(" USB");
			}
#endif
		
#ifdef FUNC_CARD_EN
			if((gSys.SystemMode == SYS_MODE_SD) && IsCardLink())
			{
				DispString(" SD ");
			}
#endif	
		}
		DispIcon(ICON_COLON, LIGHT_OFF);
		DispIcon(ICON_DP_FM_MHZ, LIGHT_OFF);
	}
#endif
}


#endif
