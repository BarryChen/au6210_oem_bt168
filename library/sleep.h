#ifndef __SLEEP_H__
#define __SLEEP_H__

// 'WIS'��WAKEUP IN SLEEP����д
// Wake up mode.
#define		WIS_NONE				0x0000

#define		WIS_GPB6_LOW			0x0001
#define		WIS_GPB6_HIGH			0x0101

#define		WIS_GPD7_LOW			0x0002
#define		WIS_GPD7_HIGH			0x0202

#define		WIS_GPE0_LOW			0x0004
#define		WIS_GPE0_HIGH			0x0404

#define		WIS_GPE1_LOW			0x0008
#define		WIS_GPE1_HIGH			0x0808

#define		WIS_PWR_KEY				0x1010
#define		WIS_RTC_ALARM			0x0020


// оƬ��������ʱ��2����־λ
#define		SYS_ON_FLG_POR						(1 << 0)	//power on reset��оƬ����
#define		SYS_ON_FLG_WDG						(1 << 1)	//watchdog reset��оƬ��������
// flags of all wake up sources (from sleep), its are used for
// functions: GetWakeupInSleepTrigFlg()
// 'WIS'��WAKEUP IN SLEEP����д
#define		WIS_FLG_RTC         	  			(1 << 2)
#define		WIS_FLG_GPB6						(1 << 3)
#define		WIS_FLG_GPD7            			(1 << 4)
#define		WIS_FLG_GPE0          				(1 << 5)
#define		WIS_FLG_GPE1          				(1 << 6)
#define		WIS_FLG_PWR_KEY						(1 << 7)


// Sleep mode
#define 	SLEEP_MOD_NORMAL					0
#define		SLEEP_MOD_OTP_PD					1

	
// Set chip go to sleep mode.
VOID GoToSleepMode(BYTE SleepMode);

// Config the wakeup source and active voltage level of GPIO.
// 1). GPIO����Դ B6,D7,EO,E0�����������øߵ͵�ƽ����
// 2). RTC ����ʱ����Ҫʹ��RTC_WAKEUP��RTC32K�ж�      
// 3). ��ȷ����WakeUpԴ�󣬲�ִ��GoToSleepMode���������ⲿ����WakeUp������оƬ�ᱻ��λ��
VOID SetWakeupSrcInSleep(WORD WakeUpMode);

// clear the wakeup source 
// �����SLEEP״̬�µĻ���Դ��Ҫ���� 
VOID ClrWakeupSrcInSleep(WORD WakeUpMode); 

// Get WakeUp Source flag.
// ϵͳ������ʱ�������øú���
// ִ�б�������WakeUp���ܱ���ֹ�����֮ǰ�ѵ���SetWakeUpMode��������WakeUp������
// ��ֱ�ӵ���GoToSleepMode������WakeUp���������оƬ��������WakeUp������
BYTE GetWakeupFlgFromSleep(VOID);


#endif
