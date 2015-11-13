#include "syscfg.h"
#include "file.h"
#include "debug.h"
#include "spi_fs.h"
#include "playctrl.h"
#include "breakpoint.h"

FS_ERROR gFsError;
extern WORD gFileRdSize;
BYTE gFsInitOK = 0;

#ifdef	FUNC_SPI_UPDATE_EN
extern BYTE gIsMVFFileFind;
extern FILE gUpdataFile;
#endif

#ifdef FUNC_QUICK_RESPONSE_EN
extern BOOL IsSwitchMode;
extern BOOL QuickResponse(VOID);
extern VOID KeyEventGet(VOID);
#endif

#ifdef FUNC_ENCRYPT_DECODE_EN
BOOL gIsEncryptFile = FALSE;
#endif

#ifdef FUNC_ENCRYPT_DECODE_EN
BOOL IsEncryptSong(VOID)
{
    return 	gIsEncryptFile;
}
#else
BOOL IsEncryptSong(VOID)
{
    return FALSE;
}
#endif




//�ж��ļ������ͣ����ļ�ϵͳ�ײ����
BYTE FileGetType(VOID)
{
	BYTE ExtFileName[3];
	BOOL Ret;

	ExtFileName[0] = gCurrentEntry->FileName[8];
	ExtFileName[1] = gCurrentEntry->FileName[9];
	ExtFileName[2] = gCurrentEntry->FileName[10];

	if((ExtFileName[0] == 'M') && (ExtFileName[1] == 'P') && (ExtFileName[2] == '3'))
	{
		Ret = FILE_TYPE_MP3;
#ifdef FUNC_ENCRYPT_DECODE_EN
		gIsEncryptFile = FALSE;
#endif
		
	}
#ifdef FUNC_ENCRYPT_DECODE_EN
	else if(memcmp(ExtFileName, ENCRYPT_EXT, 3) == 0)
	{
	    Ret = FILE_TYPE_MP3;
		gIsEncryptFile = TRUE;//�����ļ� 
	}
#endif
	
	else
	{
		//DBG(("FILE_TYPE_UNKNOWN!!\n"));
		Ret = FILE_TYPE_UNKNOWN;
	}

#ifdef FUNC_BREAK_POINT_EN	
	//ƥ���¼���������Ϣ��Ѱ�Ҷϵ�������
	if((gFsInitOK == 0) && (Ret != FILE_TYPE_UNKNOWN))
	{
		FindLatestSong();	
	}
#endif

	return Ret;
}


//ͳ���ļ����ļ�������
BOOL PreSearch(VOID)
{
	ENTRY_TYPE EntryType;

	//DBG(("PreSearch start!\n"));
	
#ifdef FUNC_HIDDEN_FOLD_EN
	HiddenFolderEnable = TRUE;		//���������ļ����еĸ���	
#else
	HiddenFolderEnable = FALSE;		//���������ļ����еĸ���	 
#endif

#ifdef FUNC_RECYCLED_FOLD_EN
	RecycledFolderEnable = TRUE;	//���Ż���վ�ļ����еĸ���	
#else
	RecycledFolderEnable = FALSE;	//���Ի���վ�ļ����еĸ���
#endif

	DirSetStartEntry(gFsInfo.RootStart, 0, TRUE);
	FSINFO_Init();

	gFolderDirStart = gFsInfo.RootStart;

	while(1)
	{
		FeedWatchDog();
			
		EntryType = DirGetNextEntry();

		if(gFsInfo.FileSum > 65534)
		{
			break;	
		}
		if(gEntryOffset == 0)
		{	
			FSINFO_SaveFdi();			
#ifdef FUNC_QUICK_RESPONSE_EN
			//ǿ���˳�
			KeyEventGet();
			if(QuickResponse())
			{
				return FALSE;	
			}
#endif
		}

		switch(EntryType)
		{
			case ENTRY_FILE: 	//�Ǹ���
				if(FileGetType() != FILE_TYPE_UNKNOWN)
				{
					//DBG(("�ҵ�������%-.8s%-.3s\n", &gCurrentEntry->FileName[0], &gCurrentEntry->FileName[8])); 
					gFsInfo.FileSum++;
					gFsInfo.FileSumInFolder++;
					//DBG(("\n******gFsInfo.FileSum:%u, gFsInfo.FileSumInFolder:%u\n",gFsInfo.FileSum,gFsInfo.FileSumInFolder));
#ifdef FUNC_BREAK_POINT_EN
					if((gIsFindLastestSong == TRUE) && (gCurFileNum == 0))
					{				
						gCurFileNum = gFsInfo.FileSum; //��¼������
						gCurFileNumInFolder = gFsInfo.FileSumInFolder; //��¼������
						gCurFolderNum = gFsInfo.FolderSum; //��¼�ļ��к�
						//DBG(("\n******gPlayCtrl.FolderEnable:%bu, gCurFileNum:%u, gCurFileNumInFolde:%u, gCurFolderNum:%u\n",gPlayCtrl.FolderEnable, gCurFileNum, gCurFileNumInFolder, gCurFolderNum));
					}
#endif
				}

#ifdef FUNC_SPI_UPDATE_EN
				//����Ƿ�ΪSPI FLASH�����ļ���*.MVF
				//ֻ����Ŀ¼
				if((gFsInfo.FolderSum == 1) && (gIsMVFFileFind != 1))
				{
					if(memcmp(&gCurrentEntry->FileName[8], "MVF", 3) == 0)
					{
						extern BOOL FileGetInfo(FILE* File);

						gIsMVFFileFind = 1;
						FileGetInfo(&gUpdataFile);
					}	
				}	
#endif
				break;

			case ENTRY_FOLDER:	//���ļ��У��´�ѭ�������������ļ�����
				gFsInfo.FolderNumInFolder++;
				//DBG(("ENTRY_FOLDER: %d\n", gFsInfo.FolderNumInFolder));
				break;
			
			case ENTRY_END:		//������һ���ļ��У�����֮ǰ��¼�Ĳ��ֽڵ�
				FSINFO_ChangeFileNum();

				//��¼���ļ����е��ļ�����
				if(gFsInfo.FolderSum == 1)
				{
				 	gFsInfo.RootFileSum = gFsInfo.FileSum;
				}

				//�ص���ǰ�ļ��еĿ�ͷλ��
				//DBG(("��ʼ������һ���ļ��е�λ�ã�\n"));
				RewindFolderStart(); 	
				
				//�ӵ�ǰλ�ã��������ļ�ϵͳ��Ѱ����һ���ļ���
				if(!FindNextFolder())	
				{	
					//DBG(("PreSearch end! %d ms \n", (WORD)PastTimeGet(&Timer))); 
					DBG(("PreSearch end!\n"));
					return TRUE;
				}
				else	//������һ���ļ��гɹ�
				{
					//DBG(("�ҵ���һ���ļ��У�����������ļ��У�%-.8s\n", &gCurrentEntry->FileName[0]));
					gFsInfo.FolderSum++;
					gFsInfo.FolderNumInFolder = 0;
					gFsInfo.FileSumInFolder = 0;
					if(!DirEnterSubFolder())
					{
						DBG(("�������ļ���ʧ�ܣ�\n"));
						gFsInfo.FolderSum--;
					}					
					//DBG(("�������ļ��гɹ���\n"));
				}
				break;

			default:
				break;
		}	
	}

	return TRUE;
}


//�ļ�ϵͳ��ʼ��������������Ԥ����ͳ���ļ�ϵͳ�й���Ϣ.
BOOL FSInit(BYTE DeviceID)
{						
	memset(&gFsInfo, 0, sizeof(FS_INFO));
	gFsInfo.DevID = DeviceID;
	gFsInfo.CurrFATSecNum = -1;
	gFsInfo.CurrDirSecNum = -1;
	gFsError = FS_ERROR_NONE;
	gFsInitOK = 0;
	gFileRdSize = 512;

	DBG(("gFileRdSize:0x%x\n", gFileRdSize));
	DBG((">>FsInit(%bd)\n", DeviceID));

#ifdef FUNC_OTP_PLAY_EN
	if(gFsInfo.DevID == DEV_ID_OTP)
	{
		extern BOOL OtpFsInit(VOID);
		DBG(("OtpFsInit()\n"));
		return OtpFsInit();
	}
#endif

#ifdef FUNC_SPI_PLAY_EN
	if(gFsInfo.DevID == DEV_ID_SPI)
	{
		extern BOOL SpiFlashFsInit(VOID);
		DBG(("SpiFlashFsInit()\n"));
		return SpiFlashFsInit();
	}
#endif

	if(!PartitionLoad())
	{
		DBG(("PartitionLoad() error!\n"));
		gFsError = FS_ERROR_PARTITION_LOAD;
		gFsInitOK = 2;	//?????????????????????????????
		return FALSE;
	}	
	DBG(("PartitionLoad() OK!\n"));

#ifdef FUNC_BREAK_POINT_EN
	//�ȶ�gIsFindLastestSong����һ��
	gIsFindLastestSong = FALSE;
#endif	

	if(!PreSearch())
	{
		DBG(("PreSearch() error!\n"));
		gFsError = FS_ERROR_FILE_LIST_CREATE;
		gFsInitOK = 3;	//?????????????????????????????
		return FALSE;
	}
	DBG(("PreSearch() OK\n"));

	if(gFsInfo.FileSum == 0)
	{
		gFsError = FS_ERROR_NO_FILE;
	}

	DBG(("Song sum in disk: %U\n", gFsInfo.FileSum));
	DBG(("Folder sum in disk: %U\n", gFsInfo.FolderSum));
	DBG(("Valid folder sum in disk: %U\n", gFsInfo.ValidFolderSum));
	DBG(("***********************************************\n"));
	DBG(("\n"));
	gFsInitOK = 1;
	
#ifdef FUNC_SPI_UPDATE_AUTO_EN
	if(gIsMVFFileFind)
	{
		DownloadMVFToFsh();
	}
#endif

	return TRUE;
}
				
