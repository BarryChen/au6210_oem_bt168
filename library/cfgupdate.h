#ifndef __CFGUPDATE_H__
#define __CFGUPDATE_H__

typedef struct _CORRECT_INFO
{
	BYTE ValidFlg;      //������Ϣ��Ч��־Ӧ��Ϊ0xAA
	WORD StartAddr;		//2Byte, OTP�������ʼ��ַ
	BYTE Dat[5];		//��¼�Ļ��鴦����ȷ��Ϣ���ò��ֵĴ�С��Ҫ����OTP�л����ͳ����������޸�
} CORRECT_INFO;



BOOL NeedCfgUpdate(VOID);

BOOL CfgInfoUpdate(BYTE* PCfgBuf);

BOOL CfgFileValidCheck(BYTE* PCfgBuf);

VOID DispUpdateError(VOID);

#endif

