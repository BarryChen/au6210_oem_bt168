#ifndef __CLK_H__
#define __CLK_H__

#include "syscfg.h"


// bit mask of sperate module, used by ModuleClkEn and ModuleClkDis functions.
#define 	MSK_DAC_CLOCK				(1 << 0)
#define 	MSK_DEC_CLOCK				(1 << 1)
#define 	MSK_USB_CLOCK				(1 << 2)
#define 	MSK_SD_CLOCK				(1 << 8)
#define 	MSK_ADC_CLOCK				(1 << 9)
#define 	MSK_PWM_CLOCK				(1 << 10)
#define 	MSK_I2C_CLOCK				(1 << 11)


//DAC master clk selection
#define     MSK_MCLK_SEL				(1 << 0)
#define     DAC_MCLK_SRC_OSC			0
#define     DAC_MCLK_SRC_PLL			1

#define		MASK_ALL_CLOCK				0xFFFF


// Module clock enable.
VOID ModuleClkEn(WORD ModuleMask);

VOID ModuleClkDis(WORD ModuleMask);

// ����ϵͳ��Ƶ��
// rate��ΧΪ1��128. Ϊ1ʱ,ȡ��ϵͳ��Ƶ����
// ����1ʱ,ʹ��ϵͳ��Ƶ, �����ö�Ӧ�ķ�Ƶ��
VOID SetClkDivRate(BYTE	Rate);

// Get system clock divide rate.
BYTE GetClkDivRate(VOID);	

//ѡ��DAC��master Clock��Դ
//0: ����osc,����ȷ,�������ܻ����������
//1: ����pll,���ܻ����������
VOID SetDacMclkSrc(BYTE DacMclkSrc);

VOID ClockSwitchToRc(VOID);

VOID ClockSwitchToDPLL(VOID);

#endif
