#ifndef __DISPLAY_H__
#define __DISPLAY_H__


typedef enum _DISP_DEV
{
	DISP_DEV_NONE = 0,
	DISP_DEV_SLED,
	DISP_DEV_LED1888,
	DISP_DEV_LED8888,
	DISP_DEV_LED57,
	DISP_DEV_LCD58
}DISP_DEV;


#define AMPLITUDE_INTERVAL		100
#define FFFB_BLINK_INTERVAL		300
#define PLAY_BLINK_INTERVAL		500
#define MUTE_BLINK_INTERVAL		1000
#define SCAN_BLINK_INTERVAL		300
#define DEV_SYMBOL_INTERVAL		1000
#define NORMAL_INTERVAL			1000
#define INTERIM_INTERVAL		2000	// EQ��Repeat��Volume��ʾ����״̬
#define SET_INTERVL				5000	// RTC���õ���ʾ״̬����ʱ��

typedef struct _ST_BLINK
{
	BYTE MuteBlink		:1;	//������SLED MP3 PAUSE/STOP/MUTE��˸��־(������˸)
	BYTE HourSet		:1;	//������SLED MP3�������/Radio��̨��˸��־(������˸)
	BYTE MinSet			:1;	//������SLED MP3������˸��־(������˸)
	BYTE RepeatBlink	:1;
	BYTE MuteBlankScreen:1;
	BYTE HourBlankScreen:1;
	BYTE MinBlankScreen	:1;
	BYTE RepeatOff		:1;

} ST_BLINK;

typedef union _UN_BLINK
{
	ST_BLINK 	Blink;
	BYTE		BlinkFlag;

} UN_BLINK;
	
extern UN_BLINK gBlink;
extern TIMER	DispTmr;
extern TIMER            LedFlashTmr;
extern BOOL             LedFlashOn ;
extern BYTE gDispPrevRep;
extern BYTE gDispPrevEQ;
extern BYTE gDispPrevVol;
extern BYTE gDispPrevPlayState;
extern DISP_DEV	gDisplayMode;


// Clear Screen
VOID ClearScreen(VOID);

// Display initilization
// Indicate whether the system in "STAND-BY" state or not.
VOID DispInit(BOOL IsStandBy);

// Display de-initialization.
// Indicate whether the system in "STAND-BY" state or not.
VOID DispDeInit(VOID);

// ��ʾˢ��.
// Call by display refresh ISR.
VOID LedFlushDisp(VOID);

// ��ʾˢ��.
// Lcd display refresh ISR.
VOID LcdFlushDisp(VOID);

// ����Repeatģʽʱ����.
VOID DispRepeat(BOOL IsForced);

// ����EQʱ����.
VOID DispEQ(BOOL IsForced);

// ����״̬�仯ʱ����(Play/Pause/Stop/FF/FB).
VOID DispPlayState(VOID);

// �ļ��й��ܴ�/�ر�ʱ����.
VOID DispFoldState(VOID);

// ��������ʱ����.
VOID DispVolume(BOOL IsForced);
#ifdef FUNC_PT231X_EN
// ��������ʱ����.
VOID DispTreb(BOOL IsForced);

// ��������ʱ����.
VOID DispBass(BOOL IsForced);
#endif
// Mute״̬�仯ʱ����.
VOID DispMute(VOID);

//" LOD"
VOID DispLoad(VOID);

// �ļ��л�ʱ����.
VOID DispDev(VOID);

#ifdef FUNC_RTC_EN
VOID DispRtc(VOID);
#endif

VOID DispNum(WORD Num);

// �豸�л�ʱ����.
VOID DispFileNum(VOID);

#define DIS_PLAYTIME_PALY    0
#define DIS_PLAYTIME_PAUSE   1
VOID DispPlayTime(BYTE Type);
VOID DispResume(VOID);

// ����������.
VOID Display(VOID);

//��ʾPower on��Ϣ
VOID DispPowerOn(VOID);

// ����Audio��ʾʱ����
VOID DispAudio(VOID);

#endif
