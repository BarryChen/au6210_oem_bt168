#ifndef __USER_INTERFACE_H__
#define __USER_INTERFACE_H__

#define UP		1
#define DOWN	0

extern BOOL IsDacMute;
#ifdef FUNC_SAVE_PLAYTIME_EN
extern BOOL gSavePlayTimeEnFlag;
#endif

// ����û���Ҫʵ�ְ���������Ӧ���ܣ�Ҫ��Ӧ�ô�������дQuickResponse()����
BOOL QuickResponse(VOID);

// ����û���Ҫ��1msϵͳʱ����������Ҫ��Ӧ�ô�������дTimerTick1ms()����
VOID TimerTick1ms(VOID);

// ѡ��DAC ���ⲿ����Mute״̬
VOID MuteOn(BOOL DacMuteEn, BOOL ExMuteEn);

// ���Mute״̬
VOID UnMute(VOID);

// �ı�Mute״̬
VOID MuteStatusChange(VOID);

// ����ϵͳ����
VOID SetVolume(VOID);

// ����ϵͳ������ͬʱ���Mute״̬
VOID SetVolumeWithMute(VOID);

// ����ϵͳ������ͬʱ���Mute״̬����ʾ����
VOID SetVolumeWithDisplay(VOID);

// ��������������
VOID PushKeyBeep(BYTE ContinueNum);

//����ϵͳ����
VOID VolumeAdjust(BYTE Direction);


#endif
