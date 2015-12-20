#ifndef __DEVICE_H__
#define __DEVICE_H__

#include "syscfg.h"

BOOL InitDev(VOID);						

BOOL IsUSBLink(VOID); 		//���USB����״̬
BOOL IsCardLink(VOID); 		//���SD������״̬
BOOL IsLineInLink(VOID);	//���line-in����״̬
BOOL IsRadioLink(VOID);		//���RADIO����״̬
BOOL IsCardInsert(VOID);  	//����Ƿ����²����SD��
BOOL IsUsbInsert(VOID);		//����Ƿ����²���U��
BOOL IsLineInInsert(VOID);	//����Ƿ����²���line-in
BOOL IsIpodLink(VOID);
BOOL IsPcLink(VOID); 		//���PC����״̬
BOOL IsPcInsert(VOID); 		//����Ƿ����²���PC

BOOL IsBtCallComing(VOID);
BOOL IsBtCallEnd(VOID);

#ifdef OPTION_CHARGER_DETECT
extern BOOL IsChargerLinkFlag;
BOOL IsInCharge(VOID);		//Ӳ�����PC����������״̬
BOOL IsChargerInsert(VOID);	//Ӳ������Ƿ����²���PC������
#endif

#ifdef	FUNC_HEADPHONE_DETECT_EN
extern BOOL IsHeadphoneLinkFlag;
BOOL IsHeadPhoneLink(VOID);		//Ӳ������������״̬
BOOL IsHeadPhonInsert(VOID);		 //Ӳ������Ƿ����²������
#endif

extern BOOL IsCardLinkFlag;
extern BOOL IsUsbLinkFlag;
extern BOOL IsLineInLinkFlag;
extern BOOL IsPcLinkFlag;

#ifdef FUNC_LED_LINEIN_MULTIPLE_EN
extern TIMER gLineInCheckWaitTimer;
#endif

#endif
