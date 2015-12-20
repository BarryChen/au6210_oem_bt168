#include "syscfg.h"
#include "device.h"
#include "device_hcd.h"
#include "radio_app_interface.h"
#include "radio_ctrl.h"
#include "debug.h"
#include "seg_panel.h"
#include "seg_led_disp.h"
#include "bt.h"

BOOL gBtCallIsHigh_Flag = FALSE;
BOOL IsBtCallComingFlag = FALSE;
BOOL IsBtCallEndFlag = FALSE;


BOOL IsCardLinkFlag = FALSE;
BOOL IsUsbLinkFlag = FALSE;
BOOL IsLineInLinkFlag = FALSE;
BOOL IsPcLinkFlag = FALSE;
#ifdef OPTION_CHARGER_DETECT
BOOL IsChargerLinkFlag = FALSE;
#endif

#ifdef	FUNC_HEADPHONE_DETECT_EN
BOOL IsHeadphoneLinkFlag = TRUE;
#endif

#ifdef FUNC_LED_LINEIN_MULTIPLE_EN
TIMER gLineInCheckWaitTimer;
#endif

#ifdef FUNC_SEG_LED_EN
extern BOOL gLedDispRefreshFlag;
#endif


/////////////////////////////////////////////////////////////////////////////////////
#ifdef FUNC_USB_EN
//���U������״̬
BOOL IsUSBLink(VOID)
{
	return HostIsLink();
}


//����Ƿ����²���U��
BOOL IsUsbInsert(VOID)
{	
	BOOL Temp = IsUsbLinkFlag;
	IsUsbLinkFlag = IsUSBLink();
	return (!Temp && IsUsbLinkFlag);
}
#endif


#if (defined(FUNC_CARD_EN) || defined(FUNC_AUDIO_EN) || defined(FUNC_READER_EN) || defined(FUNC_AUDIO_READER_EN))

#define 	CARD_DETECT_TIME	300

extern VOID CardSdClkStop(VOID);
extern BOOL gCardPrevLinkState;

TIMER	CardDctTimer;


//���SD������״̬
BOOL IsCardLink(VOID)
{
#define CARD_JETTER_TIME 10//����ʱ��

	static BOOL CardLinkFlag = FALSE;

#ifdef	SD_DETECT_HW_SW
	static BOOL IsWaitCardPwrup = TRUE;

	CardSdClkStop();	//ֹͣSD��CLK����������ٶ�ϵͳ�ĸ���	

#if (defined(SD_DETECT_PIN_USE_A4) || defined(SD_DETECT_PIN_USE_D6) || defined(SD_DETECT_PIN_USE_E3))
	SET_CARD_NOT_TO_GPIO();		//��SD_CLK�����ͨGPIO
#endif

	ClrGpioRegBit(CARD_DETECT_PORT_DS, CARD_DETECT_BIT);		//ʹ���������������SD��������
	ClrGpioRegBit(CARD_DETECT_PORT_PU, CARD_DETECT_BIT);
	ClrGpioRegBit(CARD_DETECT_PORT_PD, CARD_DETECT_BIT);	
	ClrGpioRegBit(CARD_DETECT_PORT_OE, CARD_DETECT_BIT);
	SetGpioRegBit(CARD_DETECT_PORT_IE, CARD_DETECT_BIT);
	//GPIO����Ӳ���ʱ��Ϊ�źŽ����ȴ�һ��ʱ��
	//����ʹ��2.2Kʱ,�ȴ�10us��ʹ��10Kʱ, ��Ҫ��20us�����ȶ����
	WaitUs(20);

	if(GetGpioReg(CARD_DETECT_PORT_IN) & CARD_DETECT_BIT)
	{
		//DBG(("NO CARD HW IN\n"));
		IsWaitCardPwrup = TRUE;
		CardLinkFlag = FALSE;
		return FALSE;
	}
	else
	{
		if(IsWaitCardPwrup == TRUE)
		{
			IsWaitCardPwrup = FALSE;
			gCardPrevLinkState = TRUE;
			CardOpen();
			TimeOutSet(&CardDctTimer, CARD_DETECT_TIME);	//���ڿ��Ĳ��������������ֿ����룬��ִ��������ʱCARD_DETECT_TIMEʱ��
		}
	}

#ifdef  SD_DETECT_PIN_USE_A4
	SET_CARD_TO_A3A4A5();	//��A4�ָ�����ΪSD����
#endif
	
#ifdef	SD_DETECT_PIN_USE_D6
	SET_CARD_TO_D5D6D7();	//��D6�ָ�����ΪSD����
#endif
	
#ifdef	SD_DETECT_PIN_USE_E3
	SET_CARD_TO_A0E2E3();	//��E3�ָ�����ΪSD����
#endif
	
	//����Ӳ����⵽����������ǰһ�ο�������״̬ΪFALSEʱ���ᰴtimeoutʱ��ȥִ��һ�������⡣
	//�����ڿ���ͨ�Ź����г��ִ���Ҳ��ִ��һ�������⡣

	if(((CardLinkFlag == FALSE) && IsTimeOut(&CardDctTimer)) || (gCardPrevLinkState == FALSE))
#else //!SD_DETECT_HW_SW
	if(IsTimeOut(&CardDctTimer) || (gCardPrevLinkState == FALSE))  
#endif
	{
	 	//DBG(("CARD SW\n"));	 	
		TimeOutSet(&CardDctTimer, CARD_DETECT_TIME);
		gCardPrevLinkState = TRUE;
		//ִ�б�������
		CardLinkFlag = CardDetect();
		//ֹͣSD��CLK�����,���ٶ�ϵͳ����
		CardSdClkStop();

		WaitMs(20);
		//DBG1(("CardLinkFlag: %d\n", (WORD)CardLinkFlag));
	}

	return CardLinkFlag;
}


//����Ƿ����²����SD��
BOOL IsCardInsert(VOID)
{
	BOOL Temp = IsCardLinkFlag;

	IsCardLinkFlag = IsCardLink();
//	DBG(("CardIn:%d, %d\n", (WORD)Temp, (WORD)IsCardLinkFlag));
	return (!Temp && IsCardLinkFlag);
}
#endif


#ifdef FUNC_LINEIN_EN
//���line-in����״̬
//ʹ���ڲ���������line-inʱ������Ϊ�ߵ�ƽ����line-inʱ������Ϊ�͵�ƽ
BOOL IsLineInLink(VOID)
{
#define LINEIN_JETTER_TIME 20//����ʱ��
	static BYTE LowLevelTimes = 0;
#ifdef LINEIN_DETECT_BIT
#ifdef FUNC_LED_LINEIN_MULTIPLE_EN
	if(!IsTimeOut(&gLineInCheckWaitTimer))	
	{
		return IsLineInLinkFlag;
	}
	TimeOutSet(&gLineInCheckWaitTimer, 150);
		
#if (defined(FUNC_7PIN_SEG_LED_EN) || defined(FUNC_6PIN_SEG_LED_EN))
	gLedDispRefreshFlag = FALSE;   
	LedPinGpioInit();
	baGPIOCtrl[LINEIN_DETECT_PORT_IE] |= LINEIN_DETECT_BIT;
	baGPIOCtrl[LINEIN_DETECT_PORT_PU] &= ~LINEIN_DETECT_BIT;
#endif
#else
	baGPIOCtrl[LINEIN_DETECT_PORT_IE] |= LINEIN_DETECT_BIT;
	baGPIOCtrl[LINEIN_DETECT_PORT_PU] &= ~LINEIN_DETECT_BIT;
	baGPIOCtrl[LINEIN_DETECT_PORT_PD] &= ~LINEIN_DETECT_BIT;	
	baGPIOCtrl[LINEIN_DETECT_PORT_OE] &= ~LINEIN_DETECT_BIT;
#endif
	WaitUs(10);

	if(baGPIOCtrl[LINEIN_DETECT_PORT_IN] & LINEIN_DETECT_BIT)
	{
		IsLineInLinkFlag = FALSE;
		LowLevelTimes = 0;
	}
	else
	{
		if(LowLevelTimes < LINEIN_JETTER_TIME)
		{
			LowLevelTimes++;
		}
		else
		{
			IsLineInLinkFlag = TRUE;
		}
	}
	
#ifdef FUNC_LED_LINEIN_MULTIPLE_EN
	gLedDispRefreshFlag = TRUE;
#endif

#else
	IsLineInLinkFlag = TRUE;
#endif
	return IsLineInLinkFlag;
}


//����Ƿ����²���line-in
BOOL IsLineInInsert(VOID)
{	
	BOOL Temp = IsLineInLinkFlag;

	IsLineInLinkFlag = IsLineInLink();
	return (!Temp && IsLineInLinkFlag);
}

#endif

#ifdef FUNC_BT_CHAN_AUTO
BOOL IsBtCallComing(VOID)
{	
	BOOL Temp = IsBtCallComingFlag;

	IsBtCallComingFlag = GET_BT_CALL_STATUS();
	return (!Temp && IsBtCallComingFlag);//������
}	

BOOL IsBtCallEnd(VOID)
{	
	BOOL Temp = IsBtCallEndFlag;

	IsBtCallEndFlag = GET_BT_CALL_STATUS();
	return (Temp && !IsBtCallEndFlag);//�½���
}	

#endif



#ifdef FUNC_RADIO_EN
//����Ƿ��в���Radio ģ��
BOOL IsRadioLink(VOID)			
{
	if(Radio_Name == RADIO_NONE)
	{
		RadioInit();
	}
	return (Radio_Name != RADIO_NONE);
}
#endif


#if (defined(FUNC_AUDIO_EN) || defined(FUNC_READER_EN) || defined(FUNC_AUDIO_READER_EN))
//���PC����״̬
BOOL IsPcLink(VOID)			
{
	return UsbDeviceIsLink();
}


//����Ƿ����²���PC
BOOL IsPcInsert(VOID)
{	
	BOOL Temp = IsPcLinkFlag;

	IsPcLinkFlag = IsPcLink();
	return (!Temp && IsPcLinkFlag);
}
#endif


#ifdef OPTION_CHARGER_DETECT
//Ӳ�����PC ����������״̬
//ʹ���ڲ���������PC����������ʱ������Ϊ�͵�ƽ����ʱ����Ϊ�ߵ�ƽ
BOOL IsInCharge(VOID)
{
	baGPIOCtrl[CHARGE_DETECT_PORT_PU] |= CHARGE_DETECT_BIT;
	baGPIOCtrl[CHARGE_DETECT_PORT_PD] |= CHARGE_DETECT_BIT;
	baGPIOCtrl[CHARGE_DETECT_PORT_OE] &= ~CHARGE_DETECT_BIT;
	baGPIOCtrl[CHARGE_DETECT_PORT_IE] |= CHARGE_DETECT_BIT;
	WaitUs(2);
	
	if(baGPIOCtrl[CHARGE_DETECT_PORT_IN] & CHARGE_DETECT_BIT)
	{
		return TRUE;
	}   	
#if 0//def AU6210K_NR_D_8_CSRBT
	baGPIOCtrl[GPIO_E_PU] |= (1 << 1);
	baGPIOCtrl[GPIO_E_PD] |= (1 << 1);
	baGPIOCtrl[GPIO_E_OE] &= ~(1 << 1);
	baGPIOCtrl[GPIO_E_IE] |= (1 << 1);
	WaitUs(2);
#endif
	return FALSE;
}


//����Ƿ����²���PC������
BOOL IsChargerInsert(VOID)
{	
	BOOL Temp = IsChargerLinkFlag;

	IsChargerLinkFlag = IsInCharge();
	return ((!Temp) && IsChargerLinkFlag);
}
#endif


#ifdef	FUNC_HEADPHONE_DETECT_EN
//���HeadPhone����״̬
//ʹ���ڲ���������HeadPhoneʱ������Ϊ�ߵ�ƽ����HeadPhoneʱ������Ϊ�͵�ƽ
BOOL IsHeadPhoneLink(VOID)
{	
#ifdef HEADPHONE_DETECT_BIT
	baGPIOCtrl[HEADPHONE_DETECT_PORT_PU] &= ~HEADPHONE_DETECT_BIT;
	baGPIOCtrl[HEADPHONE_DETECT_PORT_PD] &= ~HEADPHONE_DETECT_BIT;	
	baGPIOCtrl[HEADPHONE_DETECT_PORT_OE] &= ~HEADPHONE_DETECT_BIT;
	baGPIOCtrl[HEADPHONE_DETECT_PORT_IE] |= HEADPHONE_DETECT_BIT;
	WaitUs(2);
	if(baGPIOCtrl[HEADPHONE_DETECT_PORT_IN] & HEADPHONE_DETECT_BIT)
	{
		IsHeadphoneLinkFlag = FALSE;
	}
	else
	{
		IsHeadphoneLinkFlag = TRUE;
	}	
	
#ifdef FUNC_HEADPHONE_SINGLE_LED_EN	//�������˿��뵥��LED�˿ڹ���
	baGPIOCtrl[HEADPHONE_DETECT_PORT_IE] &= ~HEADPHONE_DETECT_BIT;
	baGPIOCtrl[HEADPHONE_DETECT_PORT_OE] |= HEADPHONE_DETECT_BIT;
#endif
#endif

	return IsHeadphoneLinkFlag;
}


//����Ƿ����²���HeadPhone
BOOL IsHeadPhonInsert(VOID)
{	
	BOOL Temp = IsHeadphoneLinkFlag;
	
	IsHeadphoneLinkFlag = IsHeadPhoneLink();
	return ((!Temp) && IsHeadphoneLinkFlag);
}

#endif

