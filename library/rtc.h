#ifndef __RTC_H__
#define __RTC_H__


// this calendar supports from 1980.01.01 to 2099.12.31 year
#define		RTC_START_YEAR		1980	
#define		RTC_END_YEAR		2099

//���Ӻ�Ϊ 1 ~ MAX_ALARM_NUM
#define		MAX_ALARM_NUM		2

#define   ALARM_MODE_ONCE_ONLY   0x01
#define   ALARM_MODE_PER_DAY     0x02
#define   ALARM_MODE_PER_WEEK    0x03
#define   ALARM_MODE_INVALID     0xFF//��Чmode


#define ALARM_STATUS_OPEN        0x01
#define ALARM_STATUS_CLOSE       0x02


// solar calendar and time structure.
typedef struct _RTC_DATE_TIME
{
	WORD	Year;
	BYTE	Mon;
	BYTE	Date;
	BYTE	WDay;	//week value: 0--6, 0 is sunday
	BYTE	Hour;
	BYTE	Min;
	BYTE	Sec;
				
} RTC_DATE_TIME;


typedef struct _ALARM_INFO
{
    BYTE AlarmMode;
    DWORD AlarmTimeAsSeconds;
	BYTE AlarmStatus;	//1:open, 2:close

} AlARM_INFO;


// lunar calendar structure.
typedef struct _RTC_LUNAR_DATE
{
	WORD Year;
	BYTE Month;
	BYTE Date;
	BYTE MonthDays;		//30 or 29
	BOOL IsLeapMonth;

} RTC_LUNAR_DATE;


// ��ϵͳ�ϵ��Ҫ�ȵ��ôκ���
VOID InitRtc(AlARM_INFO* pAlarmList, BYTE AlarmNum);

// ��ѯ�����жϱ�־
// return 0 if no alarm arrived
// return alarm number if alarm arrived
BYTE CheckRtcAlarmFlag(VOID);

// ��ĳ������
BOOL OpenAlarm(BYTE AlarmNum);

// �ر�ĳ������
BOOL CloseAlarm(BYTE AlarmNum);

// ��ȡ����״̬
BYTE GetAlarmStatus(BYTE AlarmNum);


// ���õ�ǰʱ�� 
VOID SetRTCCurrTime(RTC_DATE_TIME* CurrTime);

// ��ȡ��ǰʱ��
VOID GetRTCCurrTime(RTC_DATE_TIME* CurrTime);

// Get Current Alarm Setting
// input:  BYTE AlarmIndex
// ����ֵΪAlarmMode
BYTE GetAlarmTime(BYTE AlarmIndex, RTC_DATE_TIME* timePtr);

// Do alarm setting
BOOL SetAlarmTime(BYTE AlarmNum, BYTE AlarmMode, RTC_DATE_TIME* AlarmData);

// ��ȡ����ĳ��ĳ�µ�����
BYTE GetMonthDays(WORD Year, BYTE Mon);

//////////////////////////��ũ����صĺ���///////////////////////////////////////////////////////////////////////////////////////
// ��������תũ������
VOID SolarToLunar(RTC_DATE_TIME* DateTime, RTC_LUNAR_DATE* LunarDate);

// ��ȡũ����ݵ����
// ����0-9, �ֱ��Ӧ���: {"��", "��", "��", "��", "��", "��", "��", "��", "��", "��"}
BYTE GetHeavenlyStem(WORD Year);

// ��ȡũ����ݵĵ�֧
// ����0-11, �ֱ��Ӧ��֧: {"��", "��", "��", "î", "��", "��", "��", "δ", "��", "��", "��", "��"}
//          �ֱ��Ӧ��Ф: {"��", "ţ", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��"}
BYTE GetEarthlyBranch(WORD Year);

BOOL HalfSecStatus(VOID);


#endif
