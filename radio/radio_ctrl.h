#ifndef __RADIO_CTRL_H__
#define __RADIO_CTRL_H__
#include "syscfg.h"


// Radio Freqency setting
#ifdef AU6210K_BOOMBOX_DEMO
#define MAX_RADIO_STATION_NUM		40	//ǰ20����̨Ƶ�ʣ���20�����Ӧ��Ԥ���
#else
#define MAX_RADIO_STATION_NUM		50
#endif

// Radio Tune Space
#define TUNE_SPACE
#define RADIO_TUNE_SPACE_STEP			

//seek timer value
#ifdef FUNC_RADIO_AUTOSEEK_EN
#define RadioSeekTimeValue		100
#else
#define RadioSeekTimeValue		10	//50
#endif

//�ѵ�ĳ��Ƶ�����Ƿ���Ҫͣ��ָ��ʱ��ͬʱ��ʾ̨��
#define RADIO_SEEK_WAIT_EN
#ifdef RADIO_SEEK_WAIT_EN 
#if defined(FUNC_SINGLE_LED_EN)
#ifdef FUNC_EXMUTE_EN
#define RadioPreVimeTimerValue		2000	//�й���Muteʱ��ʱ�䲻��̫��
#else
#define RadioPreVimeTimerValue		1000 
#endif
#else
#define RadioPreVimeTimerValue		1000 
#endif
#endif


typedef enum _RADIO_NAME
{
	RADIO_NONE		= 0,	
	RADIO_RDA5807	= 1,	
	RADIO_SI4702	= 2,	
	RADIO_CS2010	= 3,	
	RADIO_CL6017X	= 4,	
	RADIO_KT0830E	= 5,
	RADIO_AR1000F	= 6,	
	RADIO_BK1080	= 7,
	RADIO_QN8035	= 8,
	RADIO_MVFM		= 9
	
} RADIO_NAME;


typedef enum _RADIO_SEEK_RESULT
{
	RADIO_SEEK_NONE = 0,
	RADIO_SEEK_FALSE,
	RADIO_SEEK_STC,
	RADIO_SEEK_VALIDSTATION,
	RADIO_SEEK_BANDLIMIT

} RADIO_SEEK_RESULT;


typedef enum _RADIO_SEEK_STATE
{
	RADIO_SEEK_START		= 0,
	RADIO_SEEK_SETTHRESHOLD = 1,
	RADIO_SEEK_SETFREQ		= 2,
	RADIO_SEEK_READRSSI		= 3,
	RADIO_SEEK_END

} RADIO_SEEK_STATE;


typedef enum _SEEK_MODE
{
	SEEK_NONE				= 0,
	SEEK_AUTOSCAN			= 1,
	SEEK_SCANUP				= 2,
	SEEK_SCANDOWN			= 3	
		
} SEEK_MODE;


typedef enum _RADIO_STATE
{
	RADIO_INIT = 0,
	RADIO_IDLE,
	RADIO_SEEK,
	RADIO_PROG,
	RADIO_READ
		
} RADIO_STATE;

//BandXtal:
//bit7 crystal or exclk 
//bit6~bit4 reserved for exclk select
//bit3~bit2 band select,For example:875~1080 and others,default :875~1080 other value may change some codes
//bit1~bit0 step select,For example:25K,50k,100k,200k,default 100k other value may change some codes
typedef struct _RADIO_DATA
{
	BYTE	BandXtalSel;		//�������ñ�������       	
	WORD	CurrFreq; 			//��ǰƵ��
	BYTE    CurrStation;		//��ǰ̨��// Start from 1,0��ʾû��̨
	BYTE    Area1StationSum;	//760~874�����Ƶ����
	BYTE    StationSum;			//�ܵ���Ч̨��Ŀ(760~874 + 875~1080)// Start from 1,0��ʾû��̨
	BYTE	Stations[MAX_RADIO_STATION_NUM];	

} RADIO_DATA;


//#define EEPROM_RADIO_DATA_BASEADDR	0x60
//#define NVM_RADIO_DATA_BASEADDR		0x10	// only for test!!!
//#define RADIO_DATA_BASEADDR			EEPROM_RADIO_DATA_BASEADDR

//��̨�����е������յ�Ƶ��
#define RADIO_LOWER_BOUND		875
#define RADIO_UPPER_BOUND		1080

//������̨����ѡ��
#define RADIO_STEP_100KHZ		10
#define RADIO_STEP_50KHZ		5
#define RADIO_STEP_200KHZ		20
#define RADIO_STEPSIZE			RADIO_STEP_100KHZ

//�Զ���̨����ѡ��
#define RADIO_SEEK_STEP_50KHZ	0
#define RADIO_SEEK_STEP_100KHZ	1
#define RADIO_SEEK_STEP_200KHZ	2
#define RADIO_SEEK_STEP			RADIO_SEEK_STEP_100KHZ


//�洢���ⲿ�洢���е�Ƶ����Ҫ���ϴ˻�����������ȷ��Ƶ��
//����1�����(��1�ο���:RADIO_FREQ_BASE_760)~��2�ο���(RADIO_FREQ_BASE_875)
//����2����2�ο���(RADIO_FREQ_BASE_875)     ~�յ�
//����1����2��С��256����760~1080�������ȡ760��875
#define RADIO_FREQ_BASE_760		760	//874-760
#define RADIO_FREQ_BASE_875		875	//1080-875


// RADIO control structure.
typedef struct _RADIO_CTRL
{
	RADIO_STATE	State;
	SEEK_MODE	SeekMode;
	TIMER		SeekPreviewTimer;	//seek time and preview time
#ifdef FUNC_SLAVE_UART_EN
       WORD RadioLowerBound;
       WORD RadioUpperBound;
	WORD RadioFreqBase;
	BYTE   RadioSeekStep;
	BYTE   RadioSeekResult;
#endif

} RADIO_CTRL;


#define RadioBlinkTime		300
#define RadioProgramWaitTime	5000


#define RadioDisCurFreNum		0	//��ʾ��ǰƵ��
#define RadioDisCurChAutoscan	1	//��ʾ��ǰ̨�ţ��Զ���̨������
#define RadioDisCurChChange		2	//��ʾ��ǰ̨�ţ��ֶ��л�Ƶ��
#define RadioDisCurFreBlink		3	//��˸
#define RadioDisCurProgramNum	4	//��ʾ��ǰƵ�ʴ洢̨��
#define RadioDisCurPresetCh		5	//��ʾ��ǰԤ��̨


//��������
extern RADIO_CTRL	gRadioCtrl;
extern BYTE RadioDisFlag;

#ifndef FUNC_BREAK_POINT_EN
extern RADIO_DATA  gRadioData2Store;
#define gRadioData gRadioData2Store
#else
#define gRadioData gBreakPointInfo.PowerMemory.RadioData2Store
#endif

extern RADIO_NAME Radio_Name;	//����FM���ͺ�

#ifdef AU6210K_BOOMBOX_DEMO
extern BOOL gRadioMonoEnableFlag;	
#endif


//��������
//#define __RADIOCTRL_DEBUG__
#ifdef   __RADIOCTRL_DEBUG__
#define RadioShowPowerOn				0
#define RadioShowCancelSearch			1
#define RadioShowSearchEndOverflow		2
#define RadioShowSearchEndBufferEnough	3
#define RadioShowSearchUpDown			4
#define RadioShowSaveCurFreToCurChannle	5
#define RadioShowSaveFre				6
#define RadioShowCurStationChange		7
#define RadioShowFreChange				8
VOID RadioShowDebug(BYTE index);
#else
#define RadioShowDebug(x)    //debug����
#endif

VOID RadioCtrlInit(VOID);//����FMǰ��ʼ������
VOID RadioCtrlDeinit(VOID);//�뿪FMǰ��ʼ������
VOID RadioEnterInit(VOID);
VOID CancelSearch(VOID);
VOID RadioArrayInsert(VOID);
VOID RadioEventProcess(VOID);//������Ӧ

#ifdef FUNC_DISP_EN	
VOID RadioDisBlink(VOID);
#endif

VOID RadioStationSearch(VOID);
VOID RadioStateCtrl(VOID);//������Ӧ,״̬����

#endif
