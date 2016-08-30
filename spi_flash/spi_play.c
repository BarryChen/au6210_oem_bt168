#include "syscfg.h"
#include "sysctrl.h"
#include "playctrl.h"
#include "spi_play.h"
#include "spi_fs.h"
#include "devctrl.h"
#include "debug.h"
#include "spi_flash.h"
#include "message.h"
#include "key.h"
#include "timer.h"
#include "spi_flash.h"
#include "spi_fs.h"
#include "user_interface.h"
#include "pcm_amplitude.h"
#include "device.h"
#include "bt.h"
#include "Adc_key.h"
#include "Bluetooth_cmd.h"
#include "bt.h"
#include "npca110x.h"

#ifdef FUNC_SPI_KEY_SOUND_EN

typedef struct _ST_SPIFILEINFO
{
	DWORD dwStartAddr;
	DWORD dwFileSize;
	DWORD dwOffset;
	BYTE  ucFileType;
	BYTE  ucFileNum;
	SONG_INFO gSongInfo;
	BYTE  Volume;

} ST_SPIFILEINFO;

ST_SPIFILEINFO gstSpiFileInfo;
static BOOL isSpiPlay = FALSE;

extern BOOL		s_BreakPointFlag;

extern VOID PlayVol2Decd(VOID);
extern VOID SongPlayTimePause(VOID);
extern VOID BluetoothRefreshStatus();
BOOL GetSpiPlay()
{
	return isSpiPlay ;
}
BOOL SPIFileEOF()
{
	DBG(("gSpiFsCtrl.CurrOpLen:		%lu\n", (DWORD)gSpiFsCtrl.CurrOpLen));
	DBG(("gSpiFsCtrl.CurrOpFile.FileSize:	  %lu\n", (DWORD)gSpiFsCtrl.CurrOpFile.FileSize));
	if(gSpiFsCtrl.CurrOpLen >= gSpiFsCtrl.CurrOpFile.FileSize)
	{
		return TRUE;
	}

	return FALSE;
}


//fet file info
BOOL SPIFileGetInfo()
{
	//gstSpiFileInfo.dwStartAddr = 48;
	//gstSpiFileInfo.dwFileSize = 153435;//1114697;

	gstSpiFileInfo.dwOffset = 0;
	gstSpiFileInfo.ucFileType = SONG_TYPE_MP3;

	// Setting song information	
	gstSpiFileInfo.gSongInfo.ChannelNum = 1;
	gstSpiFileInfo.gSongInfo.IsVBR = FALSE;
	gstSpiFileInfo.gSongInfo.Mp3MpegVersion = 3;//MPEG_1;
	gstSpiFileInfo.gSongInfo.Mp3Layer = 1;//LAYER_3;
	gstSpiFileInfo.gSongInfo.BytesPerSecond = 192000;
	gstSpiFileInfo.gSongInfo.SamplesPerSecond = 44100;
	gstSpiFileInfo.gSongInfo.SongType = SONG_TYPE_MP3;

	return TRUE;
}

BOOL PlayPhoneNumBreakoff = FALSE;  		//���ŵ绰����ʱ�Ƿ�����жϱ�־
extern BLUETOOTH_STATUS gBlutoothStatus;


//ָ������ʱ�䣬��ʼ��decoder����decoder���ݣ�����decoder����
//��������˻ָ�����
//���벥��״̬

BOOL SPI_SongPlayStart(BYTE SpiFileNum)
{	
	isSpiPlay = TRUE;

	if(!SPIFileGetInfo())
	{
		//DBG(("SPIFileGetInfo Error!\n"));
		return FALSE;
	}

	RstDecd();
 	SetDecoderMode(gstSpiFileInfo.gSongInfo.SongType);	       
	if(!SpiFlashFileOpen(SpiFileNum, 0))
	{
		DBG(("SpiFlashFileOpen() Failed!\n"));
		return FALSE;
	}
	
	if(!SpiFlashFileRead(gSongInfo.PlayBuffer, 512))
	{
		DBG(("SpiFlashFileRead() Failed!\n"));
		return FALSE;
	}
	SetDecoderData(gSongInfo.PlayBuffer, 512);	
#ifdef AU6210K_NR_D_8_CSRBT
	gSys.Volume = VOLUME_MAX - 6;	
#else

#ifdef AU6210K_ZB_BT007_CSR
	if(SPIPLAY_SONG_IR_KEY == SpiFileNum || SPIPLAY_SONG_IR_ON_OFF == SpiFileNum || SPIPLAY_SONG_POWER_ON_SOUND1 == SpiFileNum)
		gSys.Volume = VOLUME_MAX/2;	
	else
#endif		
		gSys.Volume = VOLUME_MAX-8;	
#endif
	InDacChannelSel(DAC_CH_DAC);       
#ifdef AU6210K_NR_D_8_CSRBT
	NPCA110X_ADC_Input_CH_Select(INPUT_CH2);
	NPCA110X_SetOutMute(FALSE);
#endif

	InDacSetSampleRate(gstSpiFileInfo.gSongInfo.SamplesPerSecond);
	InDacMuteDis();	
#ifdef FUNC_EXMUTE_EN
#ifdef	FUNC_HEADPHONE_DETECT_EN
	if(IsHeadphoneLinkFlag == FALSE)
#endif
	{
		ExMuteOff();
		DBG1(("r90rr\n"));
	}	
#endif
	WaitMs(50);	

	PlayVol2Decd();
	DecoderStartPlay();
	DBG(("<-SPI_SongPlayStart()\n"));
	return TRUE;
}


BOOL SPI_SongPlayDo(VOID)					
{	
	if(SPIFileEOF())
	{
		DBG(("SPIFileEOF!\n"));
		return FALSE;
	}
	
	if(!IsDecoderDone())
	{
		return TRUE;
	}
	
	if(!SpiFlashFileRead(gSongInfo.PlayBuffer, 512))
	{
		DBG(("SpiFlashFileRead() Failed,1!\n"));
		return FALSE;
	}

	SetDecoderData(gSongInfo.PlayBuffer, 512);	
	//DBG(("<-SPI_SongPlayDo()\n"));
	
	return TRUE;
}


//ֹͣ���룬�ر�DAC
VOID SPISongPlayStop(VOID)						
{
	isSpiPlay = FALSE;
	MuteOn(TRUE, TRUE);
}


static VOID NormalSongPlayResume(VOID)
{
	s_BreakPointFlag = TRUE;
	if (!SongPlayInit())
	{
		DBG(("Song Init Error!\n"));
		return;
	}		
}
#ifdef FUNC_MIN_MAX_VOLUME_LED_WITH_TONE
BOOL type_flag = FALSE;
#endif

BOOL SPI_PlaySelectNum(BYTE SpiPlayNum, BOOL PlayResumeFlag)
{	
	BYTE Volume;
	
#ifdef FUNC_MIN_MAX_VOLUME_LED_WITH_TONE
	BYTE times=0;
	BOOL vol_disp_flag = TRUE;
	TIMER		Disp_vol;

	TimeOutSet(&Disp_vol, 200);
	ClrGpioRegBit(GPIO_E_OUT, (1 << 2));
	ClrGpioRegBit(GPIO_A_OUT, (1 << 0));
#endif

	if (SpiPlayNum > gSpiFsCtrl.FsHead.Mp3FileSum)
	{
		return FALSE;
	}

	if(SpiPlayNum == SPIPLAY_SONG_IR_ON_OFF || 
		SpiPlayNum == SPIPLAY_SONG_IR_KEY )
		return FALSE;
#ifdef FUNC_MIN_MAX_VOLUME_LED_WITH_TONE
	if(SpiPlayNum == SPIPLAY_SONG_MAX_VOLUME ||
		SpiPlayNum == SPIPLAY_SONG_MIN_VOLUME)
		type_flag = TRUE;
	else
		type_flag = FALSE;
#endif
		
	if(SpiFlashModel == SpiFlash_None)
	{
		DBG(("unsupport spi dev!\n"));
		return FALSE;
	}

	Volume = gSys.Volume;	
	DecoderPause();
	SongPlayTimePause();
	
	if(gSys.SystemMode == SYS_MODE_RADIO)
	{
		MuteOn(TRUE, TRUE);		
	}
	
	SysClkDivClr();	
 	SPI_SongPlayStart(SpiPlayNum);

	while(SPI_SongPlayDo())
	{
#ifdef FUNC_WATCHDOG_EN
		FeedWatchDog();
#endif
	
#ifdef FUNC_MIN_MAX_VOLUME_LED_WITH_TONE
		if(IsTimeOut(&Disp_vol) && (times < 4) && (type_flag))
		{
			times++;
			TimeOutSet(&Disp_vol, 200);
			if(vol_disp_flag)
			{
				vol_disp_flag = FALSE;
				SetGpioRegBit(GPIO_E_OUT, (1 << 2));
				SetGpioRegBit(GPIO_A_OUT, (1 << 0));
			}
			else
			{
				vol_disp_flag = TRUE;
				ClrGpioRegBit(GPIO_E_OUT, (1 << 2));
				ClrGpioRegBit(GPIO_A_OUT, (1 << 0));
			}
		}
#endif

	}
	type_flag = FALSE;

	SPISongPlayStop();	

	gSys.Volume = Volume;		
	
	if(PlayResumeFlag)
	{
		NormalSongPlayResume();	//�ָ�MP3��������
		UnMute();
	}

	DBG(("SpiFlashPlayEnd \n"));
#if 0//def AU6210K_ZB_BT007_IR_IC_IS_334M_CSR
	if(gSys.SystemMode == SYS_MODE_BLUETOOTH)	
	{
		if (BluetoothMuteOnOff())
		{
			ExMuteOn();
		}
		else if (gSys.Volume > VOLUME_MIN)
		{
			NormalSongPlayResume();	//�ָ�MP3��������
			ExMuteOff();
		}
	}
#endif	

	return TRUE;
}

#endif	

