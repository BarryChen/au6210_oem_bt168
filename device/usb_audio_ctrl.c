#include "syscfg.h"
#include "sysctrl.h"
#include "audio_stream.h"
#include "device_audio.h"
#include "device_hcd.h"
//#include "display.h"
#include "breakpoint.h"
//#include "24cxx.h"
#include "user_interface.h"
#include "debug.h"
#include "sys_on_off.h"


#if (defined(FUNC_AUDIO_EN) || defined(FUNC_READER_EN) || defined(FUNC_AUDIO_READER_EN))

//����������ģʽ��ʹ�������VID��PID
WORD AudioVID = 0x0000;
WORD AudioPID = 0x0106;

//һ��ͨģʽ��ʹ�������VID��PID
WORD AudioReaderVID = 0x0000;
WORD AudioReaderPID = 0x0108;

//����������ģʽ��һ��ͨģʽ������ʹ�������STRING
BYTE* AudioString_Manu = "MVSILICON.INC.";			//max length: 32bytes
BYTE* AudioString_Product = "MV USB AUDIO";			//max length: 32bytes
BYTE* AudioString_SerialNumber = "2011090112345";	//max length: 32bytes



//��USB����ʱ���������ڷ����ǣ�
//1. ����DAC���������Ϊ��������ֵgVolArr[gSys.Volume]��PC����������˺�Ľ����
//	 ���Ա���ͨ����������gSys.Volume���Ըı�������PC�˵�������Ҳ���Ըı�������
//2. ����PC�˵��ڡ�������������ʱ��PC�ᷢ����֪ͨUSB��������������Ϊָ��ֵ��
//	 ����������������ֵ������AudioLeftVolume��AudioRightVolume�����У�
//	 Ȼ�����DeviceAudioSetVolume()����������������á�
//3. ��������������ֵ�ֱ���ڣ�PC�˵�balance����ʵ�ʾ��Ƕ�USB���������������ֱ����ò�ͬ����ֵ��
//4. ��USB���������ӵ�PCʱ��USB�����ὫAudioLeftVolume��AudioRightVolume��ֵ���ظ�PC����Ϊ��ʼĬ��������


//����������������PC�˵�����ֵ
WORD AudioLeftVolume = AUDIO_MAX_VOLUME / 10;		//PC��Ĭ������Ϊ���������10%
WORD AudioRightVolume = AUDIO_MAX_VOLUME / 10;	//PC��Ĭ������Ϊ���������10%



//���漸��������AudioStreamProcess()�����ص�

//����������PC�˵��ڡ�������---����������������---ƽ�⡱ʱ����õ����������
VOID DeviceAudioSetVolume(VOID)
{
	extern WORD	CODE gVolArr[];
	WORD LeftVolume;
	WORD RightVolume;
	
	LeftVolume = ((DWORD)AudioLeftVolume) * gVolArr[gSys.Volume] / AUDIO_MAX_VOLUME;
	RightVolume = ((DWORD)AudioRightVolume) * gVolArr[gSys.Volume] / AUDIO_MAX_VOLUME;
	InDacSetVolume(LeftVolume, RightVolume);
//	DBG(("AudioLeftVolume:%d, AudioRightVolume:%d, gVolArr[%bd]:%d, LeftVolume:%d, RightVolume:%d\n", AudioLeftVolume, AudioRightVolume, gSys.Volume, gVolArr[gSys.Volume], LeftVolume, RightVolume));
#ifdef FUNC_BREAK_POINT_EN
	BP_SaveInfo(&gBreakPointInfo.PowerMemory.Volume, sizeof(gBreakPointInfo.PowerMemory.Volume));
#endif
}


//����ʹ�ܣ�PC�˵��ڡ�������---ȫ��������ʱ����õ����������
VOID DeviceAudioMuteEn(VOID)
{
	InDacSetVolume(0, 0);
}


//����ȡ����PC�˵��ڡ�������---ȫ��������ʱ����õ����������
VOID DeviceAudioMuteDis(VOID)
{
	DeviceAudioSetVolume();
	UnMute();
	gSys.MuteFg = FALSE;
}


//�豸���ú�
VOID DeviceAudioConfigOK(VOID)
{
	AudioStreamInit();
	SetChipWorkOnAudioMode();
	SetIntEnBit(MASK_INT_OTG_MC);	
}


//initial usb audio device. 
BOOL DeviceAudioCtrlInit(VOID)
{		
	DeviceAudioInit();
	DeviceAudioSetVolume();
	UnMute();
	gSys.MuteFg = FALSE;
	gAudioState = AUDIO_INIT;
#ifdef FUNC_BREAK_POINT_EN
	BP_SaveInfo(&gBreakPointInfo.PowerMemory.SystemMode, sizeof(gBreakPointInfo.PowerMemory.SystemMode));
#endif
	return TRUE;

#ifdef FUNC_USB_VOLUP_EN

	USB_VOLUP_Off();//usb״̬�¸�����
			
#endif		
}


//end usb audio device.
VOID DeviceAudioCtrlEnd(VOID)
{	
	UsbDeviceDisConnect();
	ClrIntEnBit(MASK_INT_OTG_MC);
}


//usb audio device state control.
VOID DeviceAudioStateCtrl(VOID)
{	
	BYTE Event = MessageGet(MSG_FIFO_KEY);

	//DBG(("Event;%bx\n",Event));
	if(gAudioState != AUDIO_INIT)
	{// Audio��ʼ���ɹ�������Ӧ��������.
		if(Event == MSG_VOL_ADD)
		{		
			VolumeAdjust(UP);
		}
		else if(Event == MSG_VOL_SUB)
		{		
			VolumeAdjust(DOWN);
		}
		else if((Event == MSG_PLAY_PAUSE) || (Event == MSG_PLAY_1))
		{
			DBG(("MSG_PAUSE\n"));
			DeviceAudioSendPcCmd(PC_CMD_PLAY_PAUSE);
		}
		else if(Event == MSG_STOP)
		{
			DBG(("MSG_STOP\n"));
			DeviceAudioSendPcCmd(PC_CMD_STOP);
		}
		else if(Event == MSG_NEXT)
		{
			DBG(("MSG_NEXT\n"));
			DeviceAudioSendPcCmd(PC_CMD_NEXT);
		}
		else if(Event == MSG_PRE)
		{
			DBG(("MSG_PRE\n"));
			DeviceAudioSendPcCmd(PC_CMD_PRE);
		}
		else if(Event == MSG_MUTE)
		{		
			MuteStatusChange();
			DBG(("MSG_MUTE,gSys.MuteFg:%bd\n", gSys.MuteFg));
			DeviceAudioSendPcCmd(PC_CMD_MUTE);
		}
#ifdef FUN_SYSTEM_POWEROFF_WAIT_TIME
		SystemOffTimeMsgPro(Event);
#endif
	}
	
	//USB���������
	DeviceAudioRequestProcess();

	//USB ISO���ݰ�����
	AudioStreamProcess();
}


#endif
