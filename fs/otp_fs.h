#ifndef __OTP_FS_H__
#define __OTP_FS_H__

#include <reg51.h>
#include "type.h"

#define  OTP_MAX_SONG_NUM     1

typedef struct _OTP_FILE_ENTRY
{
	BYTE CODE *StartDataPtr; //MP3����ָ��
	WORD DataLen;		     //MP3���ݳ���

} OTP_FILE_ENTRY;

typedef  struct _OTP_FILE_HANDLE
{
	BYTE CurrFileNum;
	BYTE CODE *StartDataPtr; //MP3��ʼ����ָ��
	WORD DataLen;		     //MP3���ݳ���
	BYTE CODE *CurrDataPtr;	 //MP3��ǰ����ָ��
	WORD RemainBytes;		 //��ʣ��û�ж�ȡ���ֽ���

} OTP_FILE_HANDLE;



BOOL OtpFileOpen(WORD FileNum);
WORD OtpFileRead(WORD Addr, WORD Len);
BOOL IsEndOfFile(VOID);

#endif


