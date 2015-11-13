#include "type.h"
#include "syscfg.h"
#include "utility.h"
#include "dac.h"
#include "sysctrl.h"
#include "debug.h"
#include "ir_key.h"
#include "timer.h"


#ifdef FUNC_TIMER1_REFRESH_LED_EN
extern BYTE gClkDivRate;
#endif


// System clock divider setting.
VOID SysClkDivSet(BYTE ClkDivRate)
{
#if 1
//	DBG(("CLK divid enable!\n"));

//	ModuleClkDis(GCLK_USB_MASK | GCLK_CARD_MASK | GCLK_DECD_MASK | GCLK_DAC_MASK);

//	//���ѡ�����DPLL����ϵͳʱ������12MHZ�������ٷ�Ƶ
//	ClockSwitchToDPLL();
//	SetClkDivRate(ClkDivRate);
//	TimerCountSet(ClkDivRate);
	
	//ѡ���Ƶ��ϵͳʱ����������DPLL����RC����
	//���ѡ�����RC��������ϵͳʱ������3MHZ�Ļ������ٷ�Ƶ  
	//ClkDivRate������4��8��12��16��4�������� 
	//�����Ϊ4������������4ȡ������
	SetClkDivRate(ClkDivRate / 4);
	ClockSwitchToRc();
	TimerCountSet((ClkDivRate / 4) * 4);

#ifdef FUNC_TIMER1_REFRESH_LED_EN
	gClkDivRate = ClkDivRate * 4;
#endif
#endif
	gSys.IsClkDiv = TRUE;
}


// System clock divider clear.
VOID SysClkDivClr(VOID)
{
	ClockSwitchToDPLL();	//ϵͳʱ���л���DPLL
	SetClkDivRate(1);
	TimerCountSet(1);
//	ClkDivSwitchDis();
//	WaitMs(2);
//	ModuleClkEn(GCLK_USB_MASK | GCLK_CARD_MASK | GCLK_DECD_MASK | GCLK_DAC_MASK);

#ifdef FUNC_TIMER1_REFRESH_LED_EN
	gClkDivRate = 1;
#endif

	gSys.IsClkDiv = FALSE;

#ifdef FUNC_WATCHDOG_EN
	FeedWatchDog();
#endif
}


