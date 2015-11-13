#ifndef __LCD_SEG_H__
#define __LCD_SEG_H__


//�ú궨�岻Ҫ�޸ģ��ײ����øò���
#define 	LCD_BIAS_2		0
#define 	LCD_BIAS_3		1	
#define 	LCD_BIAS_4		2


// LCDģ���ʼ��(����LCDʹ�õ�IO������BIAS)  
// LCD : XXXX XXXX | XXXX XXX16	|	15	14	13	12	11	10	9	8	|	7	6	5	4	3	2	1	0
// GPIO: XXXX XXXX | XXXX XXXA7	|	A6	D7	D6	D5	D4	D3	D2	D1	|	D0	B7	B6	B4  B3	B2	B1	B0
// MaskSeg/MaskCom��bitӳ���ϵΪBit[0:16]һһ��ӦLcd Channel[0:16](Bit0->Channel0 etc..)
// MaskSeg: ָ��SEG�˿�ʹ������ЩLcd Channel,�����Чλ��Ӧ�Ĺܽ�ΪSEG[0],�����Чλ��Ӧ�Ĺܽ�ΪSEG[M-1]
// MaskCom: ָ��COM�˿�ʹ������ЩLcd Channel,�����Чλ��Ӧ�Ĺܽ�ΪCOM[0],�����Чλ��Ӧ�Ĺܽ�ΪCOM[N-1]
// Bias: ѡ��ƫ�õ�ѹ����
VOID LcdInit(DWORD MaskSeg, DWORD MaskCom, BYTE Bias);

//�˳�LCDģʽ
VOID LcdDeinit(VOID);

//������ʾ���ݼ���ˢ������
//Ҫ��ʾ�������ѷ���Buf[]��,ĳ������Ϊ1ʱ,��Ӧ�ıʻ�����;
// ���SEG����������8,Buf[]���ȵ���COM����,Buf[i]��ӦCOM[i]����ʾ����;
// ���SEG��������8,Buf[]���ȵ���2��COM����,��Buf[2*i + 0]��ӦCOM[i]����ʾ���ݵĸ��ֽ�,Buf[2*i + 1]��ӦCOM[i]����ʾ���ݵĵ��ֽ�;
VOID LcdSetData(BYTE* Buf);

// ����LCD��ʾˢ��Mem����
VOID LcdSetRefMem(VOID);

#endif
