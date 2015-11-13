#ifndef __ADC_H__
#define	__ADC_H__


// ADC referenct voltage select
//���º�����ѡ��ο���ѹ
#define		ADC_VREF_BG24			0  	   //ʹ��BandGap 2.4V��Ϊ�ο���ѹ
#define		ADC_VREF_AVDD33			1  	   //ʹ��VDDA 3.3V��Ϊ�ο���ѹ


// ADC Channel Select
//���º�����ѡ�����ͨ��
#define  ADC_CHANNEL_AUDIO_L		1	   //DAC����������
#define  ADC_CHANNEL_AUDIO_R		2	   //DAC����������
#define  ADC_CHANNEL_B6				4	   //GPIO B6ͨ������
#define  ADC_CHANNEL_D7				5      //GPIO D7ͨ������
#define  ADC_CHANNEL_E0				6	   //GPIO E0ͨ������
#define  ADC_CHANNEL_E1				7      //GPIO E1ͨ������



// ADC value generate function, read value from one channel.
BYTE AdcReadOneChannel(BYTE	Channel, BYTE Vref);


#endif

