#ifndef __FSINFO_H__
#define __FSINFO_H__

#include <string.h>
#include "type.h"
#include "syscfg.h"


#define		SECTOR_SIZE			512		//bytes per sector


// Define device ID in system.
typedef	enum _DEV_ID
{//���ڵײ����������ã�������ʽ������ö�ٱ���ֵ���ܱ��޸�.
	DEV_ID_NONE	= 0,
	DEV_ID_USB	= 1,
	DEV_ID_SD	= 2,
	DEV_ID_OTP	= 3,		
	DEV_ID_SPI	= 4,		
   	DEV_SUM

} DEV_ID;


// File system structure
typedef struct _FS_INFO
{//���ڸýṹ������ڵײ�������ʹ�ã��ýṹ�岻�ܱ��޸�.
	DEV_ID 	DevID;				//device id of this drive
	BOOL 	FAT32;				//is FAT32 file system
	BOOL	IsCpatFS;			//is compatalbe file system
	
	BOOL	IsFATChange;
	BOOL	IsDIRChange;
	
	DWORD	CurrFATSecNum;		//current FAT sector number
	DWORD	CurrDirSecNum;		//current directory sector number

	BYTE  	ClusterSize;		//The number of bytes of Cluster.    
	DWORD   FatStart;			//the sector number of FAT start
	DWORD  	RootStart;			//This is set to the cluster number of the first cluster of the root director.,
	DWORD  	DataStart;			//the first sector of data
	DWORD  	MaxCluster;			//max cluster number in this drive
	DWORD  	MaxSector;			//max sector number in this drive

	DWORD  	FreeClusNum;			

	WORD	FileSum;			//�����豸�ϵĸ����ļ�����
	WORD	FileSumInFolder;	//�ļ�ϵͳ�ڲ�ʹ��		
	WORD	FolderSum;			//�����豸�ϵ��ļ�������
	WORD	FolderNumInFolder;	//�ļ�ϵͳ�ڲ�ʹ��

	WORD	ValidFolderSum;		//�����豸�ϵķǿ��ļ�������

	WORD	RootFileSum;		//���ļ����µ��ļ�����

} FS_INFO;


extern FS_INFO	gFsInfo;


// ����һ�麯��Ϊ�ļ�ϵͳ�ڲ�ʹ�á�
VOID FSINFO_Init(VOID);
VOID FSINFO_SaveFdi(VOID);
VOID FSINFO_ChangeFolderNum(VOID);
VOID FSINFO_ChangeFileNum(VOID);
VOID FSINFO_InitOpenFolder(WORD StartFolderNum, WORD FolderNum);
BOOL FSINFO_FindLastNode(DWORD SecNum, BYTE Offset, BYTE Depth, BOOL RecFlag, BOOL RootFlag);
VOID FSINFO_InitOpenFile(WORD FileNum);


// ��ĳ���ļ��б�ʶ����Ϊ�ǿա�
VOID ClrFolderEmptyFlag(WORD FolderNum);

// �ж�ĳ���ļ����Ƿ�Ϊ�ա�
BOOL IsFolderEmpty(WORD FolderNum);

// ����ԭʼ�ļ��кż�����Ч�ļ��кţ��˳����ļ��к����ţ���
WORD GetValidFolderNum(WORD OriginalFolderNum);

// ������Ч�ļ��кţ��˳����ļ��к����ţ�����ԭʼ�ļ��кš�
WORD GetOriginalFolderNum(WORD ValidFolderNum);

// ����ǿ��ļ�����Ŀ
//WORD GetValidFolderSum(VOID);


#endif
