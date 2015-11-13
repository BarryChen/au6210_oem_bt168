#ifndef __DEVICE_AUDIO_H__
#define	__DEVICE_AUDIO_H__


//pc volume: 0 --- 999
#define AUDIO_MAX_VOLUME	999
#define	USB_DAC_FREQ		44100//48000


//pc command
#define PC_CMD_NEXT			0x04
#define PC_CMD_PRE			0x08
#define PC_CMD_MUTE			0x10
#define PC_CMD_PLAY_PAUSE	0x40
#define PC_CMD_STOP			0x80

#define MASK_AUDIO_PLAY		0x01
typedef enum _AUDIO_STATE
{
	AUDIO_INIT = 0,
	AUDIO_IDLE = 1,
	AUDIO_PLAY = 2

} AUDIO_STATE;

extern AUDIO_STATE gAudioState;


// usb audio device initial
VOID DeviceAudioInit(VOID);

// usb audio device process
VOID DeviceAudioRequestProcess(VOID);

// send command to pc
BOOL DeviceAudioSendPcCmd(BYTE PcCmd);

//����USB�豸����ģʽ
#define USB_DEVICE_AUDIO			1	//PCʶ��Ϊ����������
#define USB_DEVICE_READER			2	//PCʶ��Ϊ�����Ķ�����
#define USB_DEVICE_AUDIO_READER		3	//PCͬʱʶ��������Ͷ�����
VOID DeviceAudioSetMode(BYTE DeviceMode);

#endif
