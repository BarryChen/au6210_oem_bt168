#ifndef __DIR_H__
#define __DIR_H__


//file attributes define
#define	ATTR_READ_ONLY			0x01
#define	ATTR_HIDDEN				0x02
#define	ATTR_SYSTEM 			0x04
#define	ATTR_VOLUME_ID			0x08
#define	ATTR_DIRECTORY			0x10
#define	ATTR_ARCHIVE			0x20
#define	ATTR_LONG_NAME			0x0f	//ATTR_READ_ONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_VOLUME_ID
#define	ATTR_LONG_NAME_MASK		0x3f	//ATTR_READ_ONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_VOLUME_ID | ATTR_DIRECTORY | ATTR_ARCHIVE
#define ATTR_SYSHIDD_FOLD_MASK	0x16	//ATTR_DIRECTORY | ATTR_HIDDEN | ATTR_SYSTEM
#define ATTR_HIDDEN_FOLD_MASK	0x12	//ATTR_DIRECTORY | ATTR_HIDDEN								



typedef enum _ENTRY_TYPE
{
	ENTRY_FILE,
	ENTRY_FOLDER,		//�ļ���Ŀ¼��������Ͻ�������ļ���
	ENTRY_HIDDEN_FOLD,
	ENTRY_SYSHIDD_FOLD,
	ENTRY_RECYCLED,
	ENTRY_END, 			//��ǰĿ¼�������
	ENTRY_DOT,
	ENTRY_LONG_NAME,
	ENTRY_DELETED_FILE,
	ENTRY_UNKNOWN

} ENTRY_TYPE;


#define	MAX_STACK_DEPTH 	8


typedef struct _FDI_STATCK
{			
	DWORD		SecNum[MAX_STACK_DEPTH];			//���ڵ�����������������
	BYTE		Offset[MAX_STACK_DEPTH];			//���ڵ������������������ڲ���ƫ����	
	WORD		FolderNum[MAX_STACK_DEPTH];			//���ڵ�ͬһ�ļ����У��ڱ��ڵ�֮ǰ���ļ�����

	BYTE		Depth;								//����·���ϵĽڵ����

} FDI_STATCK; 


//define File Description Block
typedef struct _FDB
{
	BYTE	FileName[8];
	BYTE	ExtName[3];
	BYTE	Attribute;
	BYTE 	NTRes;
	BYTE	CrtTimeTenth;
	WORD	CreateTime;
	WORD	CreateDate;
	WORD	AccessDate;
	WORD	StartHigh;			//this entry's first cluster number. High
	WORD	ModifyTime;
	WORD	ModifyDate;
	WORD	StartLow;			//this entry's first cluster number. Low
	DWORD	Size;				//32-bit DWORD holding this file��s size in bytes

} FDB;


// Define long name dirctory entry sturct.
typedef struct _LONG_DIR_ENTRY
{
	BYTE		Ord;
	BYTE		Name1[10];
	BYTE		Attribute;
	BYTE		Type;
	BYTE		CheckSum;
	BYTE		Name2[12];
	BYTE		Resv[2];
	BYTE		Name3[4];

} LONG_DIR_ENTRY;


extern FDB*			gCurrentEntry;	 
extern DWORD		gDirSecNum;
extern BYTE 		gEntryOffset;
extern DWORD 		gFolderDirStart;


extern BOOL HiddenFolderEnable;		//�Ƿ񲥷������ļ����еĸ�����Ĭ�ϲ���
extern BOOL RecycledFolderEnable;	//�Ƿ񲥷Ż���վ�ļ����еĸ�����Ĭ�ϲ���


VOID DirSetStartEntry(DWORD StartSecNum, BYTE Offset, BOOL InitFlag);

DWORD DirGetSonClusNum(VOID);

DWORD DirGetSonSecNum(VOID);

VOID DirSetSonClusNum(DWORD ClusNum);

// �ڵ�ǰĿ¼������һ���ļ�Ŀ¼��
BYTE DirGetNextEntry(VOID);

BOOL DirFindNextFolder(VOID);

// ������Ŀ¼
BOOL DirEnterSubFolder(VOID);

// ���ظ�Ŀ¼
BOOL DirEnterParentFolder(VOID);

VOID RewindFolderStart(VOID);

BOOL DirNameCheck(BYTE* DstFileName, BYTE* SrcFileName);

BOOL FindNextFolder(VOID);


#endif
