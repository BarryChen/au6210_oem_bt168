#ifndef __DECD_H__
#define __DECD_H__



#define FAST_FORWARD		0
#define	FAST_BACKWARD		1


// EQ
#define 	EQ_BANDS_NUM 			10
// �����ܵ�EQ������Ŀ
// �û��ɸ�����Ҫ������ʵ��ʹ�õ�EQ����
#define 	DECD_EQ_SUM				11
	
// ����ÿ��EQ������
#define		DECD_EQ_NORMAL			0
#define		DECD_EQ_ROCK			1
#define		DECD_EQ_POP				2
#define		DECD_EQ_CLASSIC			3
#define		DECD_EQ_JASS			4		
#define		DECD_EQ_BLUE			5
#define		DECD_EQ_HALL			6
#define		DECD_EQ_BASS			7
#define		DECD_EQ_SOFT			8
#define		DECD_EQ_COUNTRY			9
#define		DECD_EQ_OPERA			10


typedef enum _SONG_TYPE
{
	SONG_TYPE_UNKOWN,
	SONG_TYPE_MP3,
	SONG_TYPE_PCM_1CH,
	SONG_TYPE_PCM_2CH,
	//SONG_TYPE_PCM_AUDIO

} SONG_TYPE;


typedef struct _SONG_INFO
{
	BYTE	SongType;			//��Ŀ����
	BYTE	ChannelNum;			//��������
	WORD	SamplesPerSecond;	//������
	DWORD	BytesPerSecond;		//����
	DWORD	TotalPlayTime;		//�ܲ���ʱ��	
	DWORD	CurPlayTime;		//��ǰ����ʱ��	
	DWORD	FrameLen;			//MP3֡���ȡ�WMA�����ȡ�ADPCM�鳤��	
	DWORD	FrameNum;			//֡����
	DWORD	FileHeaderLen;		//�ļ��У���Ч����ǰ���ļ�ͷ�ֽ���

	WORD	PlayBuffer;			//��ǰ�������ݻ������׵�ַ
	WORD	DataOffset;			//
	
	BOOL	IsVBR;				//VBR flag
	BYTE	Mp3MpegVersion;		//mpeg version value
	BYTE	Mp3Layer;			//layer value

} SONG_INFO;


extern SONG_INFO gSongInfo;


// Set decoder EQ value.
VOID DecoderSetEQ(BYTE* EQ_Table);

// Pause decoding
// Description: Send pause signal to decoder
VOID DecoderPause(VOID);

// Resume decoder
// Description: Send resume signal to decoder
VOID DecoderStartPlay(VOID);	

// ��������ǰ�ķ���
BOOL DecoderAnalysis(FILE* File);				
				
// Judge fifo state
// If Fifo empty return TRUE,otherwise return FALSE.
BOOL	IsFifoEmpty(VOID);					

// Set decoder mode
BOOL SetDecoderMode(BYTE DecoderMode);

// Set decoder mode
// Calling this will notify the decoder that data is prepared,Decoder will start the DMA transfer.
BOOL SetDecoderData(WORD Addr, WORD Len);

BOOL IsDecoderDone(VOID);

// ��λ��ָ��ʱ��
BOOL DecoderSeekToTime(DWORD PlayTime);

// ���ÿ�����������ͺͱ���
BOOL DecoderFastPlaySet(BYTE FastPlayType, BYTE FastPlayRate);

// fast playing process
BOOL DecoderFastPlayProc(VOID);


#endif
