#ifndef __FOLDER_H__
#define __FOLDER_H__


// Folder info structure define.
typedef struct _FOLDER
{	
	WORD			FolderNum;							//�ļ��к�
	WORD			ValidFolderNum;						//��Ч�ļ��к�

   	DWORD			ParentFoldSect;						//Folder��Ŀ¼������
	DWORD			DirSecNum;							//FolderĿ¼��洢λ��������
	BYTE			DirOffset;							//FolderĿ¼���������е�ƫ����

	DWORD			EndDirSecNum;						//�ļ��������һ���ļ�Ŀ¼������������
	BYTE			EndDirOffset;						//�ļ��������һ���ļ�Ŀ¼�����������е�ƫ����

	DWORD			StartSecNum;					   	//folder�洢λ��������
	BYTE			ShortName[9];						//folder short name

	WORD			StartFileNum;						//for calculate absolute file number in whole partition.

	WORD			IncFileCnt;							//include file sum of the folder, not recursion
	WORD			IncFolderCnt;						

	WORD			RecFileCnt;							//recursion file sum of the current folder
	WORD			RecFolderCnt;
		
} FOLDER;


// ���ļ�����Ŵ��ļ���
// �򿪳ɹ�������TRUE��Ŀ���ļ�����Ϣ��䵽Folder��ָ�Ľṹ�塣
// ��ʧ�ܣ�����FALSE��
// ParentFolder == NULL: �������豸�ϵĵ�FolderNum���ļ��С�
// ParentFolder != NULL: ��ParentFolder�ļ����еĵ�FolderNum���ļ��С�
BOOL FolderOpenByNum(FOLDER* Folder, FOLDER* ParentFolder, WORD FolderNum);

// ���ļ�����Ч��ţ��˳����ļ��к����ţ����ļ��С�
// ��������������FolderOpenByNum()��
BOOL FolderOpenByValidNum(FOLDER* Folder, FOLDER* ParentFolder, WORD ValidFolderNum);

// ���ļ������ƴ��ļ��С�
// �򿪳ɹ�������TRUE��Ŀ���ļ�����Ϣ��䵽Folder��ָ�Ľṹ�塣
// ��ʧ�ܣ�����FALSE��
// ParentFolder == NULL: �򿪸�Ŀ¼�е�FolderName�ļ��С�
// ParentFolder != NULL: ��ParentFolder�ļ����е�FolderName�ļ��С�
// FolderName[]���Ȳ�����8�ֽڣ���Ŀ¼����Ϊ"\\"
BOOL FolderOpenByName(FOLDER* Folder, FOLDER* ParentFolder, BYTE* FolderName);

// ��ȡָ���ļ��еĳ��ļ�����
// ���ļ����Ϊ66���ֽڣ�����LongFileName[]��������Ҫ��66���ֽڣ����򽫻ᵼ�²���Ԥ�ϵĴ���
// ��ȡ�����ļ���������TRUE��
// �޳��ļ���������FALSE��
BOOL FolderGetLongName(FOLDER* Folder, BYTE* LongFileName);	//LongFileName[]: 66 Bytes


#endif
