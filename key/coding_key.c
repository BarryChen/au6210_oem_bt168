/*
*****************************************************************************
*					Mountain View Silicon Tech. Inc.
*	Copyright 2010, Mountain View Silicon Tech. Inc., ShangHai, China
*					All rights reserved.
*
* Filename:			coding_key.c
* Description:		coding_key scan and process c file
*
* Change History:
*			Orson	- 6/xx/2010 - V0.1
*					- created
******************************************************************************
*/
#include <reg51.h>										//include public header files

#include "type.h"										//include project header files
#include "syscfg.h"
#include "utility.h"
#include "debug.h"
#include "timer.h"
#include "sysctrl.h"
#include "key.h"
#include "coding_key.h"
#include "message.h"


#ifdef FUNC_CODING_KEY_EN
/*
**********************************************************
*					LOCAL MACRO
**********************************************************
*/


//clockwise rotation
//    ----|		   |--------|		 |-------------------
//A       |        |        |        |
//        |--------|        |--------|

//     -------|        |--------|		 |---------------
//B           |        |        |		 |
//            |--------|        |--------|


//counterclockwise rotation
//    --------|		   |--------|		 |---------------
//A           |		   |        |		 |
//            |--------|        |--------|

//     ---|        |--------|		 |-------------------
//B       |        |        |		 |
//        |--------|        |--------|



/*
**********************************************************
*					LOCAL VARIABLE
**********************************************************
*/
static BYTE		ClockWiseCnt;
static BYTE		CounterClockWiseCnt;

/*
**********************************************************
*					LOCAL TABLE
**********************************************************
*/



/*
**********************************************************
*					LOCAL FUNCTIONS
**********************************************************
*/



/*
**********************************************************
*					GLOBAL FUNCTION
**********************************************************
*/
// Initialize coding key scan (GPIO) operation.
// Config interrupt at negative edge of signal-A 
VOID CodingKeyScanInit()
{
	//enable pull up resister.
	SetGpioRegBit(CODING_KEY_A_PORT_IE, CODING_KEY_A_BIT);
	ClrGpioRegBit(CODING_KEY_A_PORT_OE, CODING_KEY_A_BIT);					
	ClrGpioRegBit(CODING_KEY_A_PORT_PU, CODING_KEY_A_BIT);					
	ClrGpioRegBit(CODING_KEY_A_PORT_PD, CODING_KEY_A_BIT);					
	SetGpioRegBit(CODING_KEY_A_PORT_DS, CODING_KEY_A_BIT);	

	SetGpioRegBit(CODING_KEY_B_PORT_IE, CODING_KEY_B_BIT);
	ClrGpioRegBit(CODING_KEY_B_PORT_OE, CODING_KEY_B_BIT);					
	ClrGpioRegBit(CODING_KEY_B_PORT_PU, CODING_KEY_B_BIT);					
	ClrGpioRegBit(CODING_KEY_B_PORT_PD, CODING_KEY_B_BIT);					
	SetGpioRegBit(CODING_KEY_B_PORT_DS, CODING_KEY_B_BIT);					

	//interrupt at negative edge of signal-A	
	SetGpioRegBit(GPIO_GLB_INTE, 0x01);	//ʹ��GPIOģ����ж��ܿ���λ
	SetGpioRegBit(CODING_KEY_A_PORT_PTRIG, CODING_KEY_A_BIT);	//����Ϊ�����س����жϷ�ʽ
	SetGpioRegBit(CODING_KEY_A_PORT_INTC, CODING_KEY_A_BIT);	//�����ӦGPIO���жϱ�־
	SetGpioRegBit(CODING_KEY_A_PORT_INTE, CODING_KEY_A_BIT);	//ʹ�ܶ�ӦGPIO���жϿ���λ
	SetIntEnBit(MASK_INT_GPIO);	//ʹ��GPIOģ����ж��ܿ���λ

	ClockWiseCnt = 0;
	CounterClockWiseCnt = 0;
}


// Key process, image key value to key event.
MESSAGE CodingKeyEventGet() 
{
 	MESSAGE event = MSG_NONE;

	if(ClockWiseCnt)
	{
		event = MSG_VOL_ADD;
	}
	else if(CounterClockWiseCnt)
	{
		event = MSG_VOL_SUB;
	}
	ClockWiseCnt = 0;
	CounterClockWiseCnt = 0;
	return event; 
}


// Coding key signal-A interrupt.
VOID CodingKeyInterrupt()					
{	
	if((baGPIOCtrl[CODING_KEY_A_PORT_IN] & CODING_KEY_A_BIT)
		||(ClockWiseCnt != 0)
		||(CounterClockWiseCnt != 0))
	{
		return;
	}

	if(baGPIOCtrl[CODING_KEY_B_PORT_IN] & CODING_KEY_B_BIT)	
	{ 
		//clockwise rotation
		ClockWiseCnt++;
	}
	else	
	{
		//counterclockwise rotation
		CounterClockWiseCnt++;
	}
}


#endif

