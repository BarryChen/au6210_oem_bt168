#ifndef __FAT_H__
#define __FAT_H__


// Define EOF flag for FAT16 and FAT32
#define EOF_FAT32				0x0FFFFFF8
#define	EOF_FAT16				0xFFF8


// Get the sector number
DWORD GetSecNum(DWORD clusNum);

// Get the cluster number
DWORD GetClusNum(DWORD secNum);

// Load one directory sector
BOOL ReadDirSec(DWORD secNum);
	
// Load one FAT sector
BOOL ReadFATSec(DWORD secNum);

// Get next cluster number
DWORD GetNextClusNum(DWORD ClusNum);

// Get next sector number
DWORD GetNextSecNum(DWORD SecNum);

// ����FAT����
//BOOL ModifyFatItem(DWORD ClusNum, DWORD WriteData);

// �жϴ��Ƿ����
//BOOL IsFreeClus(DWORD ClusNum);

// ���㵱ǰ����ʣ��Ŀ�����������
//BYTE FreeSectorOfClus(DWORD CurSectorNum);

// ����һ����
//DWORD FreeClusAlloc(DWORD StartClusNum);

// ɾ��һ������
//VOID DeleteClusterChain(DWORD StartClusNum);

// ��һ�������ĵ�ǰλ������������ɸ���
//BOOL SeekClusterChain(DWORD StartClusNum, DWORD ClusCnt);

#endif
