#ifndef __CHARGE_LED_H__
#define	__CHARGE_LED_H__


#define	FREQ_0HZ5		15
#define	FREQ_1HZ		7
#define	FREQ_2HZ		3
#define	FREQ_4HZ		1
#define	FREQ_8HZ		0


#define	CHARGE_DONE_LIGHT	1
#define	CHARGE_DONE_DARK	0


// ʹ�ܳ����ʾ���ܣ�ռ�ձ�Ϊ50%
// 1.Freq��ʾ���ָʾ����˸Ƶ��
// 2.Polarity��ʾ�����ɣ�����ص�ѹ����4.15V���󣬵�����������
VOID ChargeLedOpen(BYTE Freq, BYTE Polarity);

//�ر�LED���ָʾ��ע���ʱGPIO_A2_ICS��������Դ�ᱻ�ر�
VOID ChargeLedClose(VOID);

#endif