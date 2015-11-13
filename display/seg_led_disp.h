#ifndef __SEG_LED_DISP__
#define __SEG_LED_DISP__

#ifdef FUNC_SEG_LED_EN

//7��6��LED������������ɨ���㷨���£�
 
//7��LED����ϰ���Ͻ�7���ܽŵ���ų�Ϊ1��--7��
//Ϊ�˷���д���룬��ģ���н�7������Ÿ�Ϊ0-6
//COM�׶ηֱ�ΪCOM0--COM6
//SEG����ֱ�ΪSEG0--SEG6
 
//ɨ���㷨
 
//COM��ѡ��: ����ߵ�ƽ
//COM��ѡ��: ����̬
 
//SEG��: ʹ����������Դ
//SEG��: ����̬
 
//COM0: 1����COM����ߵ�ƽ, ����6������SEG, ���ıʻ�SEGʹ����������Դ����ıʻ�SEG����Ϊ����̬ 
//COM1: 2����COM����ߵ�ƽ, ����6������SEG, ���ıʻ�SEGʹ����������Դ����ıʻ�SEG����Ϊ����̬
//COM2: 3����COM����ߵ�ƽ, ����6������SEG, ���ıʻ�SEGʹ����������Դ����ıʻ�SEG����Ϊ����̬
//COM3: 4����COM����ߵ�ƽ, ����6������SEG, ���ıʻ�SEGʹ����������Դ����ıʻ�SEG����Ϊ����̬
//COM4: 5����COM����ߵ�ƽ, ����6������SEG, ���ıʻ�SEGʹ����������Դ����ıʻ�SEG����Ϊ����̬
//COM5: 6����COM����ߵ�ƽ, ����6������SEG, ���ıʻ�SEGʹ����������Դ����ıʻ�SEG����Ϊ����̬
//COM6: 7����COM����ߵ�ƽ, ����6������SEG, ���ıʻ�SEGʹ����������Դ����ıʻ�SEG����Ϊ����̬
 

#if (defined(FUNC_7PIN_SEG_LED_EN) || defined(FUNC_6PIN_SEG_LED_EN))

#ifdef FUNC_6PIN_SEG_LED_EN
#define MAX_LED_PIN_NUM		6	//LED ������
#else
#define MAX_LED_PIN_NUM		7	//LED ������
#endif


//LED ��������GPIO ���Ӷ���
#if 0//def AU6210K_6PIN_LED_DEMO
//6ֻ�Ŷ���LED��ʾ��
	#define LED_PIN1_PORT_PU 		GPIO_D_PU
	#define LED_PIN1_PORT_PD 		GPIO_D_PD
	#define LED_PIN1_PORT_OE 		GPIO_D_OE
	#define LED_PIN1_PORT_IE 		GPIO_D_IE
	#define LED_PIN1_PORT_ICS_1MA7 	GPIO_D_ICS_1MA7
	#define LED_PIN1_PORT_ICS_2MA4 	GPIO_D_ICS_2MA4
	#define LED_PIN1_PORT_OUT 		GPIO_D_OUT
	#define LED_PIN1_BIT			(1 << 0)

	#define LED_PIN2_PORT_PU 		GPIO_D_PU
	#define LED_PIN2_PORT_PD 		GPIO_D_PD
	#define LED_PIN2_PORT_OE 		GPIO_D_OE
	#define LED_PIN2_PORT_IE 		GPIO_D_IE
	#define LED_PIN2_PORT_ICS_1MA7 	GPIO_D_ICS_1MA7
	#define LED_PIN2_PORT_ICS_2MA4 	GPIO_D_ICS_2MA4
	#define LED_PIN2_PORT_OUT 		GPIO_D_OUT
	#define LED_PIN2_BIT			(1 << 1)

	#define LED_PIN3_PORT_PU 		GPIO_D_PU
	#define LED_PIN3_PORT_PD 		GPIO_D_PD
	#define LED_PIN3_PORT_OE 		GPIO_D_OE
	#define LED_PIN3_PORT_IE 		GPIO_D_IE
	#define LED_PIN3_PORT_ICS_1MA7 	GPIO_D_ICS_1MA7
	#define LED_PIN3_PORT_ICS_2MA4 	GPIO_D_ICS_2MA4
	#define LED_PIN3_PORT_OUT 		GPIO_D_OUT
	#define LED_PIN3_BIT			(1 << 2)

	#define LED_PIN4_PORT_PU 		GPIO_D_PU
	#define LED_PIN4_PORT_PD 		GPIO_D_PD
	#define LED_PIN4_PORT_OE 		GPIO_D_OE
	#define LED_PIN4_PORT_IE 		GPIO_D_IE
	#define LED_PIN4_PORT_ICS_1MA7 	GPIO_D_ICS_1MA7
	#define LED_PIN4_PORT_ICS_2MA4 	GPIO_D_ICS_2MA4
	#define LED_PIN4_PORT_OUT 		GPIO_D_OUT
	#define LED_PIN4_BIT			(1 << 3)

	#define LED_PIN5_PORT_PU 		GPIO_D_PU
	#define LED_PIN5_PORT_PD 		GPIO_D_PD
	#define LED_PIN5_PORT_OE 		GPIO_D_OE
	#define LED_PIN5_PORT_IE 		GPIO_D_IE
	#define LED_PIN5_PORT_ICS_1MA7 	GPIO_D_ICS_1MA7
	#define LED_PIN5_PORT_ICS_2MA4 	GPIO_D_ICS_2MA4
	#define LED_PIN5_PORT_OUT 		GPIO_D_OUT
	#define LED_PIN5_BIT			(1 << 4)

	#define LED_PIN6_PORT_PU 		GPIO_D_PU
	#define LED_PIN6_PORT_PD 		GPIO_D_PD
	#define LED_PIN6_PORT_OE 		GPIO_D_OE
	#define LED_PIN6_PORT_IE 		GPIO_D_IE
	#define LED_PIN6_PORT_ICS_1MA7 	GPIO_D_ICS_1MA7
	#define LED_PIN6_PORT_ICS_2MA4 	GPIO_D_ICS_2MA4
	#define LED_PIN6_PORT_OUT 		GPIO_D_OUT
	#define LED_PIN6_BIT			(1 << 5)

	#define LED_PIN7_PORT_PU 		GPIO_D_PU
	#define LED_PIN7_PORT_PD 		GPIO_D_PD
	#define LED_PIN7_PORT_OE 		GPIO_D_OE
	#define LED_PIN7_PORT_IE 		GPIO_D_IE
	#define LED_PIN7_PORT_ICS_1MA7 	GPIO_D_ICS_1MA7
	#define LED_PIN7_PORT_ICS_2MA4 	GPIO_D_ICS_2MA4
	#define LED_PIN7_PORT_OUT 		GPIO_D_OUT
	#define LED_PIN7_BIT			0
#elif 1
	#define LED_PIN1_PORT_PU 		GPIO_D_PU
	#define LED_PIN1_PORT_PD 		GPIO_D_PD
	#define LED_PIN1_PORT_OE 		GPIO_D_OE
	#define LED_PIN1_PORT_OE 		GPIO_D_OE
	#define LED_PIN1_PORT_ICS_1MA7 	GPIO_D_ICS_1MA7
	#define LED_PIN1_PORT_ICS_2MA4 	GPIO_D_ICS_2MA4
	#define LED_PIN1_PORT_OUT 		GPIO_D_OUT
	#define LED_PIN1_BIT			(1 << 6)

	#define LED_PIN2_PORT_PU 		GPIO_D_PU
	#define LED_PIN2_PORT_PD 		GPIO_D_PD
	#define LED_PIN2_PORT_OE 		GPIO_D_OE
	#define LED_PIN2_PORT_OE 		GPIO_D_OE
	#define LED_PIN2_PORT_ICS_1MA7 	GPIO_D_ICS_1MA7
	#define LED_PIN2_PORT_ICS_2MA4 	GPIO_D_ICS_2MA4
	#define LED_PIN2_PORT_OUT 		GPIO_D_OUT
	#define LED_PIN2_BIT			(1 << 5)

	#define LED_PIN3_PORT_PU 		GPIO_D_PU
	#define LED_PIN3_PORT_PD 		GPIO_D_PD
	#define LED_PIN3_PORT_OE 		GPIO_D_OE
	#define LED_PIN3_PORT_OE 		GPIO_D_OE
	#define LED_PIN3_PORT_ICS_1MA7 	GPIO_D_ICS_1MA7
	#define LED_PIN3_PORT_ICS_2MA4 	GPIO_D_ICS_2MA4
	#define LED_PIN3_PORT_OUT 		GPIO_D_OUT
	#define LED_PIN3_BIT			(1 << 4)

	#define LED_PIN4_PORT_PU 		GPIO_D_PU
	#define LED_PIN4_PORT_PD 		GPIO_D_PD
	#define LED_PIN4_PORT_OE 		GPIO_D_OE
	#define LED_PIN4_PORT_OE 		GPIO_D_OE
	#define LED_PIN4_PORT_ICS_1MA7 	GPIO_D_ICS_1MA7
	#define LED_PIN4_PORT_ICS_2MA4 	GPIO_D_ICS_2MA4
	#define LED_PIN4_PORT_OUT 		GPIO_D_OUT
	#define LED_PIN4_BIT			(1 << 3)

	#define LED_PIN5_PORT_PU 		GPIO_A_PU
	#define LED_PIN5_PORT_PD 		GPIO_A_PD
	#define LED_PIN5_PORT_OE 		GPIO_A_OE
	#define LED_PIN5_PORT_OE 		GPIO_A_OE
	#define LED_PIN5_PORT_ICS_1MA7 	GPIO_A_ICS_1MA7
	#define LED_PIN5_PORT_ICS_2MA4 	GPIO_A_ICS_2MA4
	#define LED_PIN5_PORT_OUT 		GPIO_A_OUT
	#define LED_PIN5_BIT			(1 << 2)

	#define LED_PIN6_PORT_PU 		GPIO_A_PU
	#define LED_PIN6_PORT_PD 		GPIO_A_PD
	#define LED_PIN6_PORT_OE 		GPIO_A_OE
	#define LED_PIN6_PORT_OE 		GPIO_A_OE
	#define LED_PIN6_PORT_ICS_1MA7 	GPIO_A_ICS_1MA7
	#define LED_PIN6_PORT_ICS_2MA4 	GPIO_A_ICS_2MA4
	#define LED_PIN6_PORT_OUT 		GPIO_A_OUT
	#define LED_PIN6_BIT			(1 << 1)

	#define LED_PIN7_PORT_PU 		GPIO_A_PU
	#define LED_PIN7_PORT_PD 		GPIO_A_PD
	#define LED_PIN7_PORT_OE 		GPIO_A_OE
	#define LED_PIN7_PORT_OE 		GPIO_A_OE
	#define LED_PIN7_PORT_ICS_1MA7 	GPIO_A_ICS_1MA7
	#define LED_PIN7_PORT_ICS_2MA4 	GPIO_A_ICS_2MA4
	#define LED_PIN7_PORT_OUT 		GPIO_A_OUT
	#define LED_PIN7_BIT			(1 << 0)
#else
//7ֻ�Ŷ���LED��ʾ��
#if (defined(FUNC_LED_ADC1_KEY_MULTIPLE_EN) || defined(FUNC_LED_ADC2_KEY_MULTIPLE_EN))
	#define LED_PIN1_PORT_PU 		GPIO_A_PU
	#define LED_PIN1_PORT_PD 		GPIO_A_PD
	#define LED_PIN1_PORT_OE 		GPIO_A_OE
	#define LED_PIN1_PORT_IE 		GPIO_A_IE
	#define LED_PIN1_PORT_ICS_1MA7 	GPIO_A_ICS_1MA7
	#define LED_PIN1_PORT_ICS_2MA4 	GPIO_A_ICS_2MA4
	#define LED_PIN1_PORT_OUT 		GPIO_A_OUT
	#define LED_PIN1_BIT			(1 << 0)
#else	
	#define LED_PIN1_PORT_PU 		GPIO_D_PU
	#define LED_PIN1_PORT_PD 		GPIO_D_PD
	#define LED_PIN1_PORT_OE 		GPIO_D_OE
	#define LED_PIN1_PORT_IE 		GPIO_D_IE
	#define LED_PIN1_PORT_ICS_1MA7 	GPIO_D_ICS_1MA7
	#define LED_PIN1_PORT_ICS_2MA4 	GPIO_D_ICS_2MA4
	#define LED_PIN1_PORT_OUT 		GPIO_D_OUT
	#define LED_PIN1_BIT			(1 << 7)
#endif
	
	#define LED_PIN2_PORT_PU 		GPIO_D_PU
	#define LED_PIN2_PORT_PD 		GPIO_D_PD
	#define LED_PIN2_PORT_OE 		GPIO_D_OE
	#define LED_PIN2_PORT_IE 		GPIO_D_IE
	#define LED_PIN2_PORT_ICS_1MA7 	GPIO_D_ICS_1MA7
	#define LED_PIN2_PORT_ICS_2MA4 	GPIO_D_ICS_2MA4
	#define LED_PIN2_PORT_OUT 		GPIO_D_OUT
	#define LED_PIN2_BIT			(1 << 6)

	#define LED_PIN3_PORT_PU 		GPIO_D_PU
	#define LED_PIN3_PORT_PD 		GPIO_D_PD
	#define LED_PIN3_PORT_OE 		GPIO_D_OE
	#define LED_PIN3_PORT_IE 		GPIO_D_IE
	#define LED_PIN3_PORT_ICS_1MA7 	GPIO_D_ICS_1MA7
	#define LED_PIN3_PORT_ICS_2MA4 	GPIO_D_ICS_2MA4
	#define LED_PIN3_PORT_OUT 		GPIO_D_OUT
	#define LED_PIN3_BIT			(1 << 5)

	#define LED_PIN4_PORT_PU 		GPIO_D_PU
	#define LED_PIN4_PORT_PD 		GPIO_D_PD
	#define LED_PIN4_PORT_OE 		GPIO_D_OE
	#define LED_PIN4_PORT_IE 		GPIO_D_IE
	#define LED_PIN4_PORT_ICS_1MA7 	GPIO_D_ICS_1MA7
	#define LED_PIN4_PORT_ICS_2MA4 	GPIO_D_ICS_2MA4
	#define LED_PIN4_PORT_OUT 		GPIO_D_OUT
	#define LED_PIN4_BIT			(1 << 4)

	#define LED_PIN5_PORT_PU 		GPIO_D_PU
	#define LED_PIN5_PORT_PD 		GPIO_D_PD
	#define LED_PIN5_PORT_OE 		GPIO_D_OE
	#define LED_PIN5_PORT_IE 		GPIO_D_IE
	#define LED_PIN5_PORT_ICS_1MA7 	GPIO_D_ICS_1MA7
	#define LED_PIN5_PORT_ICS_2MA4 	GPIO_D_ICS_2MA4
	#define LED_PIN5_PORT_OUT 		GPIO_D_OUT
	#define LED_PIN5_BIT			(1 << 3)

#if defined(FUNC_LED_ADC1_KEY_MULTIPLE_EN)
	#define LED_PIN6_PORT_PU		GPIO_D_PU
	#define LED_PIN6_PORT_PD		GPIO_D_PD
	#define LED_PIN6_PORT_OE		GPIO_D_OE
	#define LED_PIN6_PORT_IE		GPIO_D_IE
	#define LED_PIN6_PORT_ICS_1MA7	GPIO_D_ICS_1MA7
	#define LED_PIN6_PORT_ICS_2MA4	GPIO_D_ICS_2MA4
	#define LED_PIN6_PORT_OUT		GPIO_D_OUT
	#define LED_PIN6_BIT			(1 << 7)
#else
	#define LED_PIN6_PORT_PU 		GPIO_A_PU
	#define LED_PIN6_PORT_PD 		GPIO_A_PD
	#define LED_PIN6_PORT_OE 		GPIO_A_OE
	#define LED_PIN6_PORT_IE 		GPIO_A_IE
	#define LED_PIN6_PORT_ICS_1MA7 	GPIO_A_ICS_1MA7
	#define LED_PIN6_PORT_ICS_2MA4 	GPIO_A_ICS_2MA4
	#define LED_PIN6_PORT_OUT 		GPIO_A_OUT
	#define LED_PIN6_BIT			(1 << 1)
#endif

#if defined(FUNC_LED_ADC2_KEY_MULTIPLE_EN)
	#define LED_PIN7_PORT_PU		GPIO_B_PU
	#define LED_PIN7_PORT_PD		GPIO_B_PD
	#define LED_PIN7_PORT_OE		GPIO_B_OE
	#define LED_PIN7_PORT_IE		GPIO_B_IE
	#define LED_PIN7_PORT_ICS_1MA7	GPIO_B_ICS_1MA7
	#define LED_PIN7_PORT_ICS_2MA4	GPIO_B_ICS_2MA4
	#define LED_PIN7_PORT_OUT		GPIO_B_OUT
	#define LED_PIN7_BIT			(1 << 6)
#else
	#define LED_PIN7_PORT_PU 		GPIO_A_PU
	#define LED_PIN7_PORT_PD 		GPIO_A_PD
	#define LED_PIN7_PORT_OE 		GPIO_A_OE
	#define LED_PIN7_PORT_IE 		GPIO_A_IE
	#define LED_PIN7_PORT_ICS_1MA7 	GPIO_A_ICS_1MA7
	#define LED_PIN7_PORT_ICS_2MA4 	GPIO_A_ICS_2MA4
	#define LED_PIN7_PORT_OUT 		GPIO_A_OUT
	#define LED_PIN7_BIT			(1 << 0)
#endif
#endif


//LED ���Ŷ�Ӧ GPIO ��ʼ��
#define LedPinGpioInit()	baGPIOCtrl[LED_PIN1_PORT_PU] |= LED_PIN1_BIT, \
						baGPIOCtrl[LED_PIN2_PORT_PU] |= LED_PIN2_BIT, \
						baGPIOCtrl[LED_PIN3_PORT_PU] |= LED_PIN3_BIT, \
						baGPIOCtrl[LED_PIN4_PORT_PU] |= LED_PIN4_BIT, \
						baGPIOCtrl[LED_PIN5_PORT_PU] |= LED_PIN5_BIT, \
						baGPIOCtrl[LED_PIN6_PORT_PU] |= LED_PIN6_BIT, \
						baGPIOCtrl[LED_PIN7_PORT_PU] |= LED_PIN7_BIT, \
						baGPIOCtrl[LED_PIN1_PORT_PD] &= (~LED_PIN1_BIT), \  
						baGPIOCtrl[LED_PIN2_PORT_PD] &= (~LED_PIN2_BIT), \  
						baGPIOCtrl[LED_PIN3_PORT_PD] &= (~LED_PIN3_BIT), \  
						baGPIOCtrl[LED_PIN4_PORT_PD] &= (~LED_PIN4_BIT), \  
						baGPIOCtrl[LED_PIN5_PORT_PD] &= (~LED_PIN5_BIT), \  
						baGPIOCtrl[LED_PIN6_PORT_PD] &= (~LED_PIN6_BIT), \  
						baGPIOCtrl[LED_PIN7_PORT_PD] &= (~LED_PIN7_BIT), \  
						baGPIOCtrl[LED_PIN1_PORT_OE] &= (~LED_PIN1_BIT), \
						baGPIOCtrl[LED_PIN2_PORT_OE] &= (~LED_PIN2_BIT), \   
						baGPIOCtrl[LED_PIN3_PORT_OE] &= (~LED_PIN3_BIT), \       
						baGPIOCtrl[LED_PIN4_PORT_OE] &= (~LED_PIN4_BIT), \       
						baGPIOCtrl[LED_PIN5_PORT_OE] &= (~LED_PIN5_BIT), \       
						baGPIOCtrl[LED_PIN6_PORT_OE] &= (~LED_PIN6_BIT), \       
						baGPIOCtrl[LED_PIN7_PORT_OE] &= (~LED_PIN7_BIT), \       
						baGPIOCtrl[LED_PIN1_PORT_ICS_1MA7] &= (~LED_PIN1_BIT), \
						baGPIOCtrl[LED_PIN2_PORT_ICS_1MA7] &= (~LED_PIN2_BIT), \   
						baGPIOCtrl[LED_PIN3_PORT_ICS_1MA7] &= (~LED_PIN3_BIT), \       
						baGPIOCtrl[LED_PIN4_PORT_ICS_1MA7] &= (~LED_PIN4_BIT), \       
						baGPIOCtrl[LED_PIN5_PORT_ICS_1MA7] &= (~LED_PIN5_BIT), \       
						baGPIOCtrl[LED_PIN6_PORT_ICS_1MA7] &= (~LED_PIN6_BIT), \       
						baGPIOCtrl[LED_PIN7_PORT_ICS_1MA7] &= (~LED_PIN7_BIT), \       
						baGPIOCtrl[LED_PIN1_PORT_ICS_2MA4] &= (~LED_PIN1_BIT), \
						baGPIOCtrl[LED_PIN2_PORT_ICS_2MA4] &= (~LED_PIN2_BIT), \   
						baGPIOCtrl[LED_PIN3_PORT_ICS_2MA4] &= (~LED_PIN3_BIT), \       
						baGPIOCtrl[LED_PIN4_PORT_ICS_2MA4] &= (~LED_PIN4_BIT), \       
						baGPIOCtrl[LED_PIN5_PORT_ICS_2MA4] &= (~LED_PIN5_BIT), \       
						baGPIOCtrl[LED_PIN6_PORT_ICS_2MA4] &= (~LED_PIN6_BIT), \       
						baGPIOCtrl[LED_PIN7_PORT_ICS_2MA4] &= (~LED_PIN7_BIT)      


//LED ��������ʹ��ǰҪ�Ƚ���
#define LedAllPinGpioInput()	baGPIOCtrl[LED_PIN1_PORT_OE] &= (~LED_PIN1_BIT), \
							baGPIOCtrl[LED_PIN2_PORT_OE] &= (~LED_PIN2_BIT), \   
							baGPIOCtrl[LED_PIN3_PORT_OE] &= (~LED_PIN3_BIT), \       
							baGPIOCtrl[LED_PIN4_PORT_OE] &= (~LED_PIN4_BIT), \       
							baGPIOCtrl[LED_PIN5_PORT_OE] &= (~LED_PIN5_BIT), \       
							baGPIOCtrl[LED_PIN6_PORT_OE] &= (~LED_PIN6_BIT), \       
							baGPIOCtrl[LED_PIN7_PORT_OE] &= (~LED_PIN7_BIT), \       
							baGPIOCtrl[LED_PIN1_PORT_ICS_1MA7] &= (~LED_PIN1_BIT), \
							baGPIOCtrl[LED_PIN2_PORT_ICS_1MA7] &= (~LED_PIN2_BIT), \   
							baGPIOCtrl[LED_PIN3_PORT_ICS_1MA7] &= (~LED_PIN3_BIT), \       
							baGPIOCtrl[LED_PIN4_PORT_ICS_1MA7] &= (~LED_PIN4_BIT), \       
							baGPIOCtrl[LED_PIN5_PORT_ICS_1MA7] &= (~LED_PIN5_BIT), \       
							baGPIOCtrl[LED_PIN6_PORT_ICS_1MA7] &= (~LED_PIN6_BIT), \       
							baGPIOCtrl[LED_PIN7_PORT_ICS_1MA7] &= (~LED_PIN7_BIT), \       
							baGPIOCtrl[LED_PIN1_PORT_ICS_2MA4] &= (~LED_PIN1_BIT), \
							baGPIOCtrl[LED_PIN2_PORT_ICS_2MA4] &= (~LED_PIN2_BIT), \   
							baGPIOCtrl[LED_PIN3_PORT_ICS_2MA4] &= (~LED_PIN3_BIT), \       
							baGPIOCtrl[LED_PIN4_PORT_ICS_2MA4] &= (~LED_PIN4_BIT), \       
							baGPIOCtrl[LED_PIN5_PORT_ICS_2MA4] &= (~LED_PIN5_BIT), \       
							baGPIOCtrl[LED_PIN6_PORT_ICS_2MA4] &= (~LED_PIN6_BIT), \       
							baGPIOCtrl[LED_PIN7_PORT_ICS_2MA4] &= (~LED_PIN7_BIT)      



//����LED ������������ߵ�ƽ
#define LED_PIN1_OUT_HIGH	baGPIOCtrl[LED_PIN1_PORT_OE] |= LED_PIN1_BIT, \
							baGPIOCtrl[LED_PIN1_PORT_OUT] |= LED_PIN1_BIT;
                      
#define LED_PIN2_OUT_HIGH	baGPIOCtrl[LED_PIN2_PORT_OE] |= LED_PIN2_BIT, \
							baGPIOCtrl[LED_PIN2_PORT_OUT] |= LED_PIN2_BIT;

#define LED_PIN3_OUT_HIGH	baGPIOCtrl[LED_PIN3_PORT_OE] |= LED_PIN3_BIT, \
							baGPIOCtrl[LED_PIN3_PORT_OUT] |= LED_PIN3_BIT;

#define LED_PIN4_OUT_HIGH	baGPIOCtrl[LED_PIN4_PORT_OE] |= LED_PIN4_BIT, \
							baGPIOCtrl[LED_PIN4_PORT_OUT] |= LED_PIN4_BIT;

#define LED_PIN5_OUT_HIGH	baGPIOCtrl[LED_PIN5_PORT_OE] |= LED_PIN5_BIT, \
							baGPIOCtrl[LED_PIN5_PORT_OUT] |= LED_PIN5_BIT;

#define LED_PIN6_OUT_HIGH	baGPIOCtrl[LED_PIN6_PORT_OE] |= LED_PIN6_BIT, \
							baGPIOCtrl[LED_PIN6_PORT_OUT] |= LED_PIN6_BIT;

#define LED_PIN7_OUT_HIGH	baGPIOCtrl[LED_PIN7_PORT_OE] |= LED_PIN7_BIT, \
							baGPIOCtrl[LED_PIN7_PORT_OUT] |= LED_PIN7_BIT;                      


//����LED �������ź���Դ����
#if 0
#define LED_PIN1_IN_ON	baGPIOCtrl[LED_PIN1_PORT_ICS_1MA7] |= LED_PIN1_BIT;

#define LED_PIN2_IN_ON	baGPIOCtrl[LED_PIN2_PORT_ICS_1MA7] |= LED_PIN2_BIT;


#define LED_PIN3_IN_ON	baGPIOCtrl[LED_PIN3_PORT_ICS_1MA7] |= LED_PIN3_BIT;

#define LED_PIN4_IN_ON	baGPIOCtrl[LED_PIN4_PORT_ICS_1MA7] |= LED_PIN4_BIT;

#define LED_PIN5_IN_ON	SetA2IcsReg(A2_ICS_FOR_GPIO, A2_ICS_1MA7);//baGPIOCtrl[LED_PIN5_PORT_ICS_1MA7] |= LED_PIN5_BIT;						

#define LED_PIN6_IN_ON	baGPIOCtrl[LED_PIN6_PORT_ICS_1MA7] |= LED_PIN6_BIT;

#define LED_PIN7_IN_ON	baGPIOCtrl[LED_PIN7_PORT_ICS_1MA7] |= LED_PIN7_BIT; 
#else
#define LED_PIN1_IN_ON	baGPIOCtrl[LED_PIN1_PORT_ICS_2MA4] |= LED_PIN1_BIT \                      
						//baGPIOCtrl[LED_PIN1_PORT_ICS_1MA7] |= LED_PIN1_BIT;

#define LED_PIN2_IN_ON	baGPIOCtrl[LED_PIN2_PORT_ICS_2MA4] |= LED_PIN2_BIT \                      
						//baGPIOCtrl[LED_PIN2_PORT_ICS_1MA7] |= LED_PIN2_BIT;


#define LED_PIN3_IN_ON	baGPIOCtrl[LED_PIN3_PORT_ICS_2MA4] |= LED_PIN3_BIT \                      
						//baGPIOCtrl[LED_PIN3_PORT_ICS_1MA7] |= LED_PIN3_BIT;

#define LED_PIN4_IN_ON	baGPIOCtrl[LED_PIN4_PORT_ICS_2MA4] |= LED_PIN4_BIT \                      
						//baGPIOCtrl[LED_PIN4_PORT_ICS_1MA7] |= LED_PIN4_BIT;

#define LED_PIN5_IN_ON	baGPIOCtrl[LED_PIN5_PORT_ICS_2MA4] |= LED_PIN5_BIT \                      
						//baGPIOCtrl[LED_PIN5_PORT_ICS_1MA7] |= LED_PIN5_BIT;

#define LED_PIN6_IN_ON	baGPIOCtrl[LED_PIN6_PORT_ICS_2MA4] |= LED_PIN6_BIT \                      
						//baGPIOCtrl[LED_PIN6_PORT_ICS_1MA7] |= LED_PIN6_BIT;

#define LED_PIN7_IN_ON	baGPIOCtrl[LED_PIN7_PORT_ICS_2MA4] |= LED_PIN7_BIT \                      
						//baGPIOCtrl[LED_PIN7_PORT_ICS_1MA7] |= LED_PIN7_BIT;    
#endif

//��ֹLED �������ŵ�������
#define LED_PIN1_IN_OFF	baGPIOCtrl[LED_PIN1_PORT_ICS_1MA7] &= (~LED_PIN1_BIT), \                      
						baGPIOCtrl[LED_PIN1_PORT_ICS_2MA4] &= (~LED_PIN1_BIT);

#define LED_PIN2_IN_OFF	baGPIOCtrl[LED_PIN2_PORT_ICS_1MA7] &= (~LED_PIN2_BIT), \                      
						baGPIOCtrl[LED_PIN2_PORT_ICS_2MA4] &= (~LED_PIN2_BIT);

#define LED_PIN3_IN_OFF	baGPIOCtrl[LED_PIN3_PORT_ICS_1MA7] &= (~LED_PIN3_BIT), \                      
						baGPIOCtrl[LED_PIN3_PORT_ICS_2MA4] &= (~LED_PIN3_BIT);

#define LED_PIN4_IN_OFF	baGPIOCtrl[LED_PIN4_PORT_ICS_1MA7] &= (~LED_PIN4_BIT), \                      
						baGPIOCtrl[LED_PIN4_PORT_ICS_2MA4] &= (~LED_PIN4_BIT);

#define LED_PIN5_IN_OFF	baGPIOCtrl[LED_PIN5_PORT_ICS_1MA7] &= (~LED_PIN5_BIT), \                      
						baGPIOCtrl[LED_PIN5_PORT_ICS_2MA4] &= (~LED_PIN5_BIT);

#define LED_PIN6_IN_OFF	baGPIOCtrl[LED_PIN6_PORT_ICS_1MA7] &= (~LED_PIN6_BIT), \                      
						baGPIOCtrl[LED_PIN6_PORT_ICS_2MA4] &= (~LED_PIN6_BIT);

#define LED_PIN7_IN_OFF	baGPIOCtrl[LED_PIN7_PORT_ICS_1MA7] &= (~LED_PIN7_BIT), \                      
						baGPIOCtrl[LED_PIN7_PORT_ICS_2MA4] &= (~LED_PIN7_BIT);     

#endif

VOID DispLedIcon(ICON_IDX Icon, ICON_STATU Light);
VOID LedDispDevSymbol(VOID);
VOID LedDispRepeat(VOID);
VOID LedDispPlayState(VOID);

#endif

#endif

