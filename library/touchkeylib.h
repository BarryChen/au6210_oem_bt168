#ifndef __TOUCHEKYLIB_H__
#define __TOUCHEKYLIB_H__

#include "type.h"


//����TouchKeyʱ���ų�ʼ����Ӧ����
//����FreOutPutPinѡ��Ƶ���������
#define	TK_FREQU_OUTPUTPIN_A6	0

//����TouchKeyPinѡ��Ƶ���������
#define	TOUCHKEYPIN_MASK		0XE3//������bit0~bit7���ò���bit��1Ϊ���ø�pinΪTouchKeyΪ0������Ϊtouchkey pin

//touchykey��Ӧ��ͨ��
#define	TK_EO_CH0		0
#define	TK_E1_CH1		1
#define	TK_E2_CH2		2
#define	TK_E3_CH3		3
#define	TK_A0_CH4		4
#define	TK_A7_CH5		5
#define	TK_B0_CH6		6
#define	TK_B1_CH7		7


//���ܣ�TouchKeyPin IO��ʼ��
//����1��FreOutPutPinѡ��Ƶ���������
//����2��TouchKeyPin ��Ӧ��touchkey������maskλ��E0~E3 A0 A7 B0 B1��Ӧ�ò�����bit0~bit7���ò���bit��1Ϊ���ø�pinΪTouchKeyΪ0������Ϊtouchkey pin����ò�������Ϊ0xff���ʾ����8��TouchKeyͨ��
VOID TouchKeyPortInit(BYTE FreOutPutPin, BYTE TouchKeyPin);

//�뿪TouchKeyʱ���Ż�ԭΪĬ��״̬
VOID TouchKeyPortDeInit();

//����ĳ��ͨ����ʼ�ɼ�
//��8·��E0~E3 A0 A7 B0 B1��Ӧ�ò���Ϊ��TK_EO_CH0��TK_E1_CH1��TK_E2_CH2��TK_E3_CH3��TK_A0_CH4��TK_A7_CH5��TK_B0_CH6��TK_B7_CH7
VOID TouchKeyStartConvert(BYTE Ch);

//�жϵ�ǰ������ͨ���ɼ��Ƿ����
//����ֵFALSE:ת����δ��ɣ���Ҫ�����ȴ�
//����ֵTRUE:ת����ɣ����Զ�ȡDout
BOOL TouchKeyIsConvertOK(VOID);

//��ȡ��ǰͨ���Ĳɼ�ֵ����ͨ��ת����ɺ��ȡ��ͨ����Dout���
//����ֵΪ��ͨ��Dout���
WORD TouchKeyGetDout(VOID);


#endif