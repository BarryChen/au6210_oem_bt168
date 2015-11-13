#ifndef __GPIO_H__
#define	__GPIO_H__


extern volatile BYTE XDATA	baGPIOCtrl[];


//////////////////////////////////////////////////////////
// GPIO control register index.
// Bank A
#define		GPIO_A_IN						0x00
#define		GPIO_A_OUT						0x01
#define		GPIO_A_IE						0x02
#define		GPIO_A_OE						0x03
#define		GPIO_A_DS						0x04
#define		GPIO_A_PU						0x05
#define		GPIO_A_PD						0x06

//ע��GPIO_A2��ICS������Ҫͨ��SetA2IcsReg�������У�����λ����ͨGPIO�Ĵ������÷���һ��
#define		GPIO_A_ICS_1MA7					0x07	//Input Current Sink 1.7mA control register
#define		GPIO_A_ICS_2MA4					0x08	//Input Current Sink 2.4mA control register
                                			
// Bank B                       			
#define		GPIO_B_IN						0x09
#define		GPIO_B_OUT						0x0A
#define		GPIO_B_IE	 					0x0B
#define		GPIO_B_OE						0x0C
#define		GPIO_B_DS						0x0D
#define		GPIO_B_PU						0x0E
#define		GPIO_B_PD						0x0F
#define		GPIO_B_ICS_1MA7					0x10	//Input Current Sink 1.7mA control register
#define		GPIO_B_ICS_2MA4					0x11	//Input Current Sink 2.4mA control register
                                			                                			
// Bank C                       			
#define		GPIO_C_IN						0x12
#define		GPIO_C_OUT						0x13
#define		GPIO_C_IE						0x14
#define		GPIO_C_OE						0x15
#define		GPIO_C_DS						0x16
#define		GPIO_C_PU						0x17
#define		GPIO_C_PD						0x18
#define		GPIO_C_ICS_1MA7					0x19	//Input Current Sink 1.7mA control register
#define		GPIO_C_ICS_2MA4					0x1A	//Input Current Sink 2.4mA control register

// Bank D
#define		GPIO_D_IN						0x1B
#define		GPIO_D_OUT						0x1C
#define		GPIO_D_IE						0x1D
#define		GPIO_D_OE						0x1E
#define		GPIO_D_DS						0x1F
#define		GPIO_D_PU						0x20       
#define		GPIO_D_PD						0x21  
#define		GPIO_D_ICS_1MA7					0x22	//Input Current Sink 1.7mA control register
#define		GPIO_D_ICS_2MA4					0x23	//Input Current Sink 2.4mA control register

// Bank E
#define		GPIO_E_IN						0x24
#define		GPIO_E_OUT						0x25
#define		GPIO_E_IE						0x26
#define		GPIO_E_OE						0x27
#define		GPIO_E_DS						0x28
#define		GPIO_E_PU						0x29         
#define		GPIO_E_PD						0x2A   
#define		GPIO_E_ICS_1MA7					0x2B	//Input Current Sink 1.7mA control register
#define		GPIO_E_ICS_2MA4					0x2C	//Input Current Sink 2.4mA control register

#define		GPIO_FMD_IN						0x2D
#define     GPIO_FMD_OUT	            	0x2E
#define     GPIO_FMD_IE		            	0x2F
#define     GPIO_FMD_OE     		       	0x30
#define     GPIO_FMD_PU            			0x31
#define     GPIO_FMD_PD    		        	0x32
#define     GPIO_FMA_EN         		 	0x33

#define     GPIO_GLB_INTE					0x34
#define     GPIO_D_SEP_INTE					0x35
#define     GPIO_E_SEP_INTE					0x36
#define     GPIO_D_SEP_PTRIG				0x37
#define     GPIO_E_SEP_PTRIG				0x38
#define     GPIO_D_SEP_INTC					0x39
#define     GPIO_E_SEP_INTC					0x3A
#define     GPIO_D_SEP_INTS					0x3B
#define     GPIO_E_SEP_INTS					0x3C
//////////////////////////////////////////////////////////
// Multiplex functions' IO index.
#define     GPIO_CFG1_SEL					0x3D
	#define		MASK_SD_TO_A3A4A5				(0 << 0)
	#define		MASK_SD_TO_A0E2E3				(1 << 0)
	#define		MASK_SD_TO_D5D6D7				(1 << 1)
	#define		MASK_SD_NOT_TO_GPIO				((1 << 0) | (1 << 1))
	#define     MASK_51_UART_RXD_TO_A1			(1 << 2)
	#define     MASK_51_UART_TXD_TO_A2			(1 << 3)
	#define		MASK_I51_UART_TO_A1A2		   	(3 << 2)
	#define     MASK_I51_UART_TXD_DUG			(1 << 4)
	#define     MASK_IR_TO_B6					(1 << 5)
	#define     MASK_IR_TO_E0					(1 << 6)



///////////////////////////////////////////////////////////////////////////
//��E0����Ϊģ������, ����ADC����, ������Ƶ����
#define SET_E0_ANALOG_IN()		ClrGpioRegBit(GPIO_E_IE, (1 << 0)),\
								ClrGpioRegBit(GPIO_E_OE, (1 << 0)),\
								ClrGpioRegBit(GPIO_E_PU, (1 << 0)),\
								SetGpioRegBit(GPIO_E_PD, (1 << 0))

//��E1����Ϊģ������, ����ADC����, ������Ƶ����
#define SET_E1_ANALOG_IN()		ClrGpioRegBit(GPIO_E_IE, (1 << 1)),\
								ClrGpioRegBit(GPIO_E_OE, (1 << 1)),\
								ClrGpioRegBit(GPIO_E_PU, (1 << 1)),\
								SetGpioRegBit(GPIO_E_PD, (1 << 1))

//��B6����Ϊģ������, ����ADC����
#define SET_B6_ANALOG_IN()		ClrGpioRegBit(GPIO_B_IE, (1 << 6)),\
								ClrGpioRegBit(GPIO_B_OE, (1 << 6)),\
								ClrGpioRegBit(GPIO_B_PU, (1 << 6)),\
								SetGpioRegBit(GPIO_B_PD, (1 << 6))

//��D3����Ϊģ������, ����ONCHIP_FM_L����������
#define SET_D3_ANALOG_IN()		ClrGpioRegBit(GPIO_D_IE, (1 << 3)),\
								ClrGpioRegBit(GPIO_D_OE, (1 << 3)),\
								ClrGpioRegBit(GPIO_D_PU, (1 << 3)),\
								SetGpioRegBit(GPIO_D_PD, (1 << 3))
							
//��D4����Ϊģ������, ����ONCHIP_FM_R����������
#define SET_D4_ANALOG_IN()		ClrGpioRegBit(GPIO_D_IE, (1 << 4)),\
								ClrGpioRegBit(GPIO_D_OE, (1 << 4)),\
								ClrGpioRegBit(GPIO_D_PU, (1 << 4)),\
								SetGpioRegBit(GPIO_D_PD, (1 << 4))

//��B6����Ϊģ������, ����ADC����
#define SET_D7_ANALOG_IN()		ClrGpioRegBit(GPIO_D_IE, (1 << 7)),\
								ClrGpioRegBit(GPIO_D_OE, (1 << 7)),\
								ClrGpioRegBit(GPIO_D_PU, (1 << 7)),\
								SetGpioRegBit(GPIO_D_PD, (1 << 7))
								
///////////////////////////////////////////////////////////////////////////
//��E0����ΪGPIO����
#define SET_E0_GPIO_IN()		SetGpioRegBit(GPIO_E_IE, (1 << 0)),\
								ClrGpioRegBit(GPIO_E_OE, (1 << 0)),\
								ClrGpioRegBit(GPIO_E_PU, (1 << 0)),\
								ClrGpioRegBit(GPIO_E_PD, (1 << 0))

//��E1����ΪGPIO����
#define SET_E1_GPIO_IN()		SetGpioRegBit(GPIO_E_IE, (1 << 1)),\
								ClrGpioRegBit(GPIO_E_OE, (1 << 1)),\
								ClrGpioRegBit(GPIO_E_PU, (1 << 1)),\
								ClrGpioRegBit(GPIO_E_PD, (1 << 1))

//��B6����ΪGPIO����
#define SET_B6_GPIO_IN()		SetGpioRegBit(GPIO_B_IE, (1 << 6)),\
								ClrGpioRegBit(GPIO_B_OE, (1 << 6)),\
								ClrGpioRegBit(GPIO_B_PU, (1 << 6)),\
								ClrGpioRegBit(GPIO_B_PD, (1 << 6))

//��D3����ΪGPIO����
#define SET_D3_GPIO_IN()		SetGpioRegBit(GPIO_D_IE, (1 << 3)),\
								ClrGpioRegBit(GPIO_D_OE, (1 << 3)),\
								ClrGpioRegBit(GPIO_D_PU, (1 << 3)),\
								ClrGpioRegBit(GPIO_D_PD, (1 << 3))
							
//��D4����ΪGPIO����
#define SET_D4_GPIO_IN()		SetGpioRegBit(GPIO_D_IE, (1 << 4)),\
								ClrGpioRegBit(GPIO_D_OE, (1 << 4)),\
								ClrGpioRegBit(GPIO_D_PU, (1 << 4)),\
								ClrGpioRegBit(GPIO_D_PD, (1 << 4))

//��B6����ΪGPIO����
#define SET_D7_GPIO_IN()		SetGpioRegBit(GPIO_D_IE, (1 << 7)),\
								ClrGpioRegBit(GPIO_D_OE, (1 << 7)),\
								ClrGpioRegBit(GPIO_D_PU, (1 << 7)),\
								ClrGpioRegBit(GPIO_D_PD, (1 << 7))

//ѡ��A3A4A5��ΪSD������, A3---DAT, A4---CLK, A5---CMD
#define SET_CARD_TO_A3A4A5()	baGPIOCtrl[GPIO_CFG1_SEL] = (baGPIOCtrl[GPIO_CFG1_SEL] & (~0x03)) | MASK_SD_TO_A3A4A5
	
//ѡ��A0E2E3��ΪSD������, A0---DAT, E3---CLK, E2---CMD
#define SET_CARD_TO_A0E2E3()	baGPIOCtrl[GPIO_CFG1_SEL] = (baGPIOCtrl[GPIO_CFG1_SEL] & (~0x03)) | MASK_SD_TO_A0E2E3

//ѡ��D5D6D7��ΪSD������, D5---DAT, D6---CLK, D7---CMD
#define SET_CARD_TO_D5D6D7()	baGPIOCtrl[GPIO_CFG1_SEL] = (baGPIOCtrl[GPIO_CFG1_SEL] & (~0x03)) | MASK_SD_TO_D5D6D7

//ȡ��ѡ��GPIO��ΪSD������
#define SET_CARD_NOT_TO_GPIO()	baGPIOCtrl[GPIO_CFG1_SEL] = (baGPIOCtrl[GPIO_CFG1_SEL] & (~0x03)) | MASK_SD_NOT_TO_GPIO


// All the function as follow operation register directly, so the input parameter "RegIndex" must correct,
// otherwise, the result is unexpectable.
// Set GPIO register value
VOID SetGpioReg(BYTE RegIndex, BYTE Val);

// Get GPIO register value
BYTE GetGpioReg(BYTE RegIndex);

// Set GPIO register bit
VOID SetGpioRegBit(BYTE RegIndex, BYTE Mask);

// Clear gpio register bit
VOID ClrGpioRegBit(BYTE RegIndex, BYTE Mask);



//GPIO A2��������ԴICS�ĵ���ֵ
typedef enum _A2_ICS_VAL
{
	A2_ICS_CLOSE	= 0,			//�ر�A2 ICS
	A2_ICS_1MA7		= 1,			//ICS = 1.7mA
	A2_ICS_2MA4		= 2,			//ICS = 2.4mA
	A2_ICS_4MA1		= 3				//ICS = 4.1mA

} A2_ICS_VAL;

// GPIO A2��������ԴICS�Ĺ��ܽ�ɫ
typedef enum _A2_ICS_AF
{
	A2_ICS_FOR_GPIO			= 0,	//Ϊ��ͨGPIO��������ICS�Ĵ���
	A2_ICS_FOR_CHARGE_LED	= 1,	//Ϊ���ָʾ��������ICS�Ĵ���
	A2_ICS_FOR_PWM			= 2		//ΪPWM��������ICS�Ĵ���

} A2_ICS_AF;

// ����A2����������ԴICS����һ�ֹ��ܽ�ɫ���Լ�����ֵ�Ĵ�С
// ����A2_ICS_AF��A2_ICS_VAL����;��ö������˵��
VOID SetA2IcsReg(A2_ICS_AF A2IcsAf, A2_ICS_VAL A2IcsVal);

#endif
