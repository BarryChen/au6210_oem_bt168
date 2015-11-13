#ifndef __FILE_H__
#define __FILE_H__


#include "folder.h"


//define file seek position.
#define	SEEK_FILE_SET				0					//seek file from the first sector of file
#define	SEEK_FILE_CUR				1					//seek file from the current sector of file
#define	SEEK_FILE_END				2					//seek file from the last sector of file 

#define	MAX_FILE_LONGNAME_SIZE		64
#define	MAX_LONGNAME_BUFFSIZE		(MAX_FILE_LONGNAME_SIZE + 2)

// File system control error type.
typedef enum _FS_ERROR
{
	FS_ERROR_NONE = 0,
	FS_ERROR_PARTITION_LOAD,
	FS_ERROR_FILE_LIST_CREATE,
	FS_ERROR_NO_FILE,
	FS_ERROR_FILE_CMPT,
	FS_ERROR_FILEOPEN_FAILED,
	FS_ERROR_FOLDOPEN_FAILED,
	FS_ERROR_ANALYSIS_FAILED,
	FS_ERROR_READ_FAILED,
	FS_ERROR_INVALID_ADDRESS,
	FS_ERROR_SUM

} FS_ERROR;

typedef enum _FILE_TYPE
{
	FILE_TYPE_UNKNOWN = 0,
	FILE_TYPE_MP3,

} FILE_TYPE;


//�ļ��������ṹ
typedef struct _FILE
{
	WORD	FileNumInDisk;			//�������豸�ϵ��ļ���
	WORD	FileNumInFolder;		//�ڵ�ǰ�ļ����е��ļ���
	WORD	FolderNum;				//�����ļ��еı��
//	WORD	ValidFolderNum;			//�����ļ��е���Ч�ļ��б��

	DWORD	ParentFoldSect;			//�ļ���Ŀ¼������
	DWORD	DirSecNum;				//�ļ�Ŀ¼��洢λ��������
	BYTE	DirOffset;				//�ļ�Ŀ¼�������������е�ƫ����

	BYTE	ShortName[11];			//���ļ���

	BYTE	FileType;				//�ļ�������

//	DWORD	StartClusNum;			//�ļ���һ���غ�

	DWORD	StartSec;				//�ļ���һ��������
	DWORD	SecNum;					//�ļ�ռ�õ���������
	DWORD	OpSec;					//��ǰҪ��д��������
	DWORD	OpSecCnt;				//��ǰλ�����ļ���ƫ��������
	DWORD	Size;					//�ļ������ֽ���

} FILE;


extern FS_ERROR gFsError;

// �ж��ļ������ͣ����ļ�ϵͳ�ײ����
BYTE FileGetType(VOID);

// ���ļ����ݵ�������
// Len���ȱ���Ϊ512�ֽ�������
WORD FileRead(FILE* File, WORD Buffer, WORD Len);

// �ж��ļ��Ƿ����
BOOL FileEOF(FILE* File);

// ���ص��ļ���ͷ
VOID FileRewind(FILE* File);

// �ļ�ָ�붨λ
BOOL FileSeek(FILE* File, SDWORD Offset, BYTE Base);

// ���ļ���Ŵ��ļ�
// �򿪳ɹ�������TRUE��Ŀ���ļ���Ϣ��䵽File��ָ�Ľṹ�塣
// ��ʧ�ܣ�����FALSE��
// Folder == NULL: �������豸�ϵĵ�FileNum���ļ���
// Folder != NULL: ��Folder�ļ����еĵ�FileNum���ļ��С�
BOOL FileOpenByNum(FILE* File, FOLDER* Folder, WORD FileNum);

// ���ļ����ƴ�ָ���ļ����е��ļ���
// �򿪳ɹ�������TRUE��Ŀ���ļ���Ϣ��䵽File��ָ�Ľṹ�塣
// ��ʧ�ܣ�����FALSE��
// Folderָ�벻��Ϊ��ָ�롣
BOOL FileOpenByName(FILE* File, FOLDER* Folder, BYTE* FileName);

// ��ȡָ���ļ��ĳ��ļ�����
// ���ļ����Ϊ66���ֽڣ�����LongFileName[]��������Ҫ��66���ֽڣ����򽫻ᵼ�²���Ԥ�ϵĴ���
// ��ȡ�����ļ���������TRUE��
// �޳��ļ���������FALSE��
BOOL FileGetLongName(FILE* File, BYTE* LongFileName);	//LongFileName[]: 66 Bytes


#endif
