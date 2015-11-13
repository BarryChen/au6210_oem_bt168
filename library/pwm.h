#ifndef __PWM_H__
#define	__PWM_H__

typedef enum _PWM_OUTPUT_SEL			  
{
	PWM3_OUTPUT_GPIOD7,
	PWM3_OUTPUT_GPIOA2,
	PWM2_OUTPUT_GPIOD6,
	PWM2_OUTPUT_GPIOA1,
	PWM1_OUTPUT_GPIOD5,
	PWM1_OUTPUT_GPIOA0,

} PWM_OUTPUT_SEL;

#define PWM_CLK_12MHZ		0
#define PWM_CLK_46875HZ		1

#define PWM_DRIVE_MODE_OUTPUT		0		//PWM�źŴ�GPIOֱ�����
#define PWM_DRIVE_MODE_ICS_1MA7		1		//PWM�źſ���1.7mA��������Դ
#define PWM_DRIVE_MODE_ICS_2MA4		2		//PWM�źſ���2.4mA��������Դ
#define PWM_DRIVE_MODE_ICS_4MA1		3		//PWM�źſ���4.1mA��������Դ


VOID PwmEnableChannel(PWM_OUTPUT_SEL PwmOutSel);
VOID PwmDisableChannel(PWM_OUTPUT_SEL PwmOutSel);

//ClkSel: ʱ��Դѡ��PWM_CLK_12MHZ ���� PWM_CLK_46875HZ
// 0: 12MHZ,    183.1HZ --- 6MHZ 
// 1: 46875HZ, 	0.715HZ  --- 46875HZ    
//Div: ��Ƶ�ȣ�1---65535
//Duty: ռ�ձȣ�0---65535, ����Duty <= Div, ռ�ձ� = Duty/Div
//ע�⣺1)�����øú����󣬱��������ͬͨ����PwmEnable��������PWM���
//	2)�����ִ������ͬͨ����PwmDisablel���� �󣬱�������ִ����1)����
VOID PwmConfigChannel(PWM_OUTPUT_SEL PwmOutSel, BYTE ClkSel, WORD Div, WORD Duty);

//PwmOutSel: ������������Ե�PWMͨ��
//DriveMode: 0---PWM��GPIOֱ�����
//			 1---PWM�źſ���1.7mA��������Դ��PWM�źŸߵ�ƽ�ڼ�1.7mA��������Դ��ʹ�ܣ��͵�ƽ�ڼ�GPIOΪ����̬�����ں�����Ӧ��
//           2---PWM�źſ���2.4mA��������Դ��PWM�źŸߵ�ƽ�ڼ�2.4mA��������Դ��ʹ�ܣ��͵�ƽ�ڼ�GPIOΪ����̬�����ں�����Ӧ��
//           3---PWM�źſ���4.1mA��������Դ��PWM�źŸߵ�ƽ�ڼ�4.1mA��������Դ��ʹ�ܣ��͵�ƽ�ڼ�GPIOΪ����̬�����ں�����Ӧ��
//��������ñ�������Ĭ��PWM��GPIOֱ�����
VOID PwmSetDriveMode(PWM_OUTPUT_SEL PwmOutSel, BYTE PwmDriveMode);

#endif
