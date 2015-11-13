#ifndef __DAC_H__
#define __DAC_H__


// Mixer Channel Select
#define DAC_CH_NONE		0x00
#define DAC_CH_DAC			0x09	//DECODER
#define DAC_CH_LINEIN		0x24	//LINE_IN_L, LINE_IN_R
#define DAC_CH_ONCHIP_FM_L	0x02	//ONCHIP_FM_L
#define DAC_CH_ONCHIP_FM_R	0x10	//ONCHIP_FM_R
#define DAC_CH_ONCHIP_FM	0x12	//ONCHIP_FM_L, ONCHIP_FM_R
#define DAC_CH_E0_L			0x40	//GPE0
#define DAC_CH_E1_R			0x80	//GPE1
#define DAC_CH_E0E1			0xC0	//GPE0, GPE1



#define MUTE_LOUT			0x04
#define MUTE_ROUT			0x40
#define MUTE_LR_BOTH	(MUTE_LOUT|MUTE_ROUT)


//�����AU6210XX-E�ͺţ���Ҫʹ������ĺ꣬DAC������������
//#define InDacSetSampleRate  		InDacSetSampleRate_2
//#define InDacSetVolume  			InDacSetVolume_2
//#define InDacInit					InDacInit_2
//#define InDacPowerDown  			InDacPowerDown_2
//#define InDacMuteEn 				InDacMuteEn_2
//#define InDacMuteDis				InDacMuteDis_2
//#define InDacChannelSel 			InDacChannelSel_2
//#define InDacLineInGain 			InDacLineInGain_2
//#define InDacAdjustSrAsPerPcmFifo	InDacAdjustSrAsPerPcmFifo_2
//#define InDacVCOMSet				InDacVCOMSet_2
//#define InDacExFmConnect			InDacExFmConnect_2
//#define InDacExFmDisConnect 		InDacExFmDisConnect_2
//#define UseAsSlaveDecd  			UseAsSlaveDecd_2


// Set Sample Rate
VOID InDacSetSampleRate(WORD SampleRate);
	
// Set DAC L/R Channel digital volume(0~4095)
VOID InDacSetVolume(WORD LVol, WORD RVol);

// DAC��ʼ��
// ���������������ò����ʣ�����ͻ����Ÿ��������������ò����ʣ����ܲ���״̬��PAUSE����������
VOID InDacInit(BOOL IsFastCharge);

// DAC PowerDown
// ����Sleep״̬ǰ������ñ�����������WakeUp����֮�����POPO����
VOID InDacPowerDown(BOOL IsFastCharge);

// DAC Mute Enable.
// ��������Mixer Out�źŲ�������LineIn/FMIn/DACIN����Ч��
VOID InDacMuteEn(VOID);

// DAC Mute Disable.
// ��������Mixer Out�źŲ�������LineIn/FMIn/DACIN����Ч��
VOID InDacMuteDis(VOID);

// DAC(Mixer)ͨ��ѡ��(DAC/LineIn/FMIn)
// 1).����FMͨ����Ӧ�ô���������ʵ����Ҫ����SET_A6A7_FMIN��SET_C0C1_FMIN��
//    ����ȷ������ӦIO��״̬.
// 2).�ܹ���DAC_CH_DAC/DAC_CH_LINEIN/DAC_CH_ONCHIP_FM/DAC_CH_E0E1�ĸ�ͨ���ɹ�ѡ��,
//    ����DAC_CH_LINEIN��DAC_CH_ONCHIP_FM��DAC_CH_E0E1���������ʹ����ͬ�ļĴ�����ֻ
//    Ҫ�޸�����һ·����������·����Ӱ�졣
// 3).ͬʱ���ͨ�������һ���ʱ��ʵ�ʵ�����������ֲ��нϴ�ƫ�����ʵ����Ϊ׼��
VOID InDacChannelSel(BYTE Channel);

// Set LineIn/FMIn Volume(L��R).
VOID InDacLineInGain(BYTE LGain, BYTE RGain);

// DAC������΢��
VOID InDacAdjustSrAsPerPcmFifo(WORD TotalPcmBytes);

// DAC ������������.
// ����isVCOMEnΪTRUE����DAC�������ݹ��ܡ�
VOID InDacVCOMSet(BOOL IsVCOMEn);

// mixer connect to External FM.
// Ϊ��֧��bibi��Ӧ�ý�EX FM L/R��һ·����·�ź�ֱ�Ӳ���Mixer�ϡ�
// ��EX FM L/R��������ʱ����Ҫ����ӦIO��OEλ��Ϊ1(��������IE��PU��PD��Ӧλ)��
VOID InDacExFmConnect(BYTE FmCh);

// mixer connect to External FM.
// Ϊ��֧��bibi��Ӧ�ý�EX FMֱ�Ӵ�Mixer�Ͽ���
VOID InDacExFmDisConnect(VOID);

// Use as Slave Decoder(Ĭ�����ò�����44.1K)
// GPD[0]->BCLK,  INPUT
// GPD[1]->LRCLK, INPUT 
// GPD[2]->DATA,  OUTPUT
VOID UseAsSlaveDecd(VOID);

//����Dac Vmid�����ѹ�����ڽ�����ֿͻ�FMģʽ��ˢ����������
//��������ΪTRUE������Ϊ1.5V�����ˢ���������⣬Dac�������˥��0.93
//��������ΪFLASE������Ϊ1.6V��Dac�������������ģʽ����Ϊ1.6V
VOID InDacSetVmid(BOOL VmdSclFlag);

#endif
