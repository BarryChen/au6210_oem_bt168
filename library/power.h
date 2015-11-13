#ifndef __POWER_H__
#define __POWER_H__


// LDOIN����ѹ��Χ
#define 	LDOIN_MIN_VOL		3000	    //��ͼ���ѹ3.00V
#define 	LDOIN_MAX_VOL		4600	    //��߼���ѹ4.60V


// �ܽ�ϵͳ��Powerdown״̬���ѵĻ���Դ��־
// 'WIP'��WAKEUP IN POWERDOWN����д
#define 	WIP_SRC_LP_RTC			0x01				
#define		WIP_SRC_PWR_KEY 		0x02


//����оƬ8s resetģʽ�����ػ���Ӳ����
// 'CRM' CHIP RESET MODE ����д
//�������ص�lib�У����������Ż�  
#define CRM_IN_PUSH_BUTTON		0x01//��ť����(����)
#define CRM_IN_SLIDE_SWITCH		0x00//��������(Ӳ����)


// return votage at LDOIN pin, unit is mV.
// 3000mV--4600mV.
WORD GetLdoinVoltage(VOID);

// ���ϵͳ���������LP_RTCģ��������ӷ�ʽ����Ƭ������Դ����ϵͳ���У�
// ���ô˺���������LP_RTC��Ϊϵͳ��powerdown״̬�»���ϵͳ�Ļ���Դ��
// ���⣬Ҫʵ��������ܣ�LP_RTC��Ҫ�����������������������ӹ��ܵȡ�
VOID SetWakeupLpRtcInPD(VOID);

//�����POWERDOWN״̬�µ͹���RTC����оƬ����
//���øú�����Ҫ����
VOID ClrWakeupLpRtcInPD(VOID);

//��PowerKey���µ�s_onoff״̬�仯����Ӧʱ��
//Ĭ��ֵΪ0x63-->0.8s����С8ms�����Լ2s����λ8ms
VOID SetChipResponseTime(BYTE Cnt);

//1 indicates PAD_ONKEY = 1 lasts for 8s can reset system, 
//0 indicates PAD_ONKEY = 0 && mcu_off_seq = 0 lasts for 8s can reset system.
//byPassģʽ����Ӳ���أ���Ҫ����Ϊ0
//����ģʽʱ����Ҫ����Ϊ1
VOID SetChip8SRstMode(BYTE Mode);

// Set chip go to lowest power consumption (power down) mode.
// ִ�иú�����оƬ������ԴLDO33O��COREVDD��Դ���رգ�оƬ����POWER DOWN״̬
VOID GoToPDMode(VOID);

// ��powerdown״̬����ϵͳ�󣬴˺����������һ������Դ�����˸���Ϊ
BYTE GetWakeupFlgFromPD(VOID);


#endif
