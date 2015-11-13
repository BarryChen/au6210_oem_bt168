#ifndef __SYS_ON_OFF_H__
#define __SYS_ON_OFF_H__


//ϵͳ����������ϵͳ��SLEEP״̬�»����������ߴ�POWER DOWN״̬����Ƭ������Դ�ϵ緽ʽ��������ش���
//һ������£�2��������ʽ���û���Ʒ�в���ͬʱ����
VOID SystemOn(VOID);

//ϵͳ�رգ�����ϵͳ��SLEEP��ʽ�رջ�����Ƭ������Դ�µ�POWER DOWN��ʽ�رյ���ش���
VOID SystemOff(VOID);

typedef enum _SLEEP_STATE
{
	SLEEP_00 = 0,	
	SLEEP_120,
	SLEEP_105,
	SLEEP_90,
	SLEEP_75,
	SLEEP_60,
	SLEEP_45,
	SLEEP_30,
	SLEEP_15,	
	SLEEP_MAX

} SLEEP_STATE;


#ifdef FUN_SYSTEM_POWEROFF_WAIT_TIME
typedef struct _SystemTimingOff
{
	SLEEP_STATE     SleepState;
	BYTE     ProState;
	BYTE     EnState;
	TIMER   AutoOutTimer;
	TIMER   DisTimer;
}SystemTimingOff;


//flagȡֵ
//0~120m,1~90m,2~60m,3~30m,4~15m 

//ProStateȡֵ
#define SYS_OFF_TIMEING_IDLE                      0//����״̬
#define SYS_OFF_TIMEING_READ_ONLY_IDLE            1//��״̬��ص�����״̬
#define SYS_OFF_TIMEING_READ_ONLY_TIMING          2//��״̬��ص���ʱ�ػ������ö�ʱ�ػ�
#define SYS_OFF_TIMEING_SET_DIS                   3//�رն�ʱ�ػ�
#define SYS_OFF_TIMEING_SET_MODE_VALID            4//��Ч�Ķ�ʱ�ػ�ʱ��
#define SYS_OFF_TIMEING_SET_MODE_INVALID          5//��Ч�Ķ�ʱ�ػ�ʱ��
#define SYS_OFF_TIMEING_WAIT_POWEROFF             6//�ȴ���ʱ�ػ�ʱ�䵽

//AutoOutTimerȡֵ
//120m,90m,60m,30m,15m

//DisTimerȡֵ
//3s

extern SystemTimingOff gSysTimeOffState;

VOID SystemOffTimeInit(VOID);

#define SYSOFF_TIME_READ_ONLY	0	
#define SYSOFF_TIME_SET_VALUE   1

VOID SystemOffTimeEnDis(BYTE Type);
VOID SystemOffTimeSet(VOID);
VOID SystemOffTimeMsgPro(BYTE Msg);
VOID SystemOffTimeWait(VOID);	
#endif

#endif
