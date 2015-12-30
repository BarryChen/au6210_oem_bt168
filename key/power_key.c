#include <reg51.h>	

#include "type.h"	
#include "syscfg.h"
#include "utility.h"
#include "debug.h"
#include "timer.h"
#include "sysctrl.h"
#include "key.h"
#include "power_key.h"
#include "bt.h"


#if	(PWR_KEY_MODE == PWR_KEY_PUSH_BUTTON)

	#warning	"MV: PUSH BUTTON POWER KEY ENABLE!"

extern BOOL	gIsPwrkeyPadOn;				//POWER_KEY���ŵ�״̬�ж����ڶ�ʱ�ж���ִ�еģ�ͨ����ȫ�ֱ����뱾�ļ������ش������̽���
										//ϵͳ����ʱPOWER_KEYĬ���ǽ�ͨ״̬
#define 	POWER_KEY_CP_TIME			100			//Ҫ���ᴥ���ػ�ʱ��CPʱ����Ҫ��
#define 	POWER_KEY_CPH_TIME			100


typedef enum _POWER_KEY_STATE
{
	POWER_KEY_STATE_IDLE,
	POWER_KEY_STATE_PRESS_DOWN,
	POWER_KEY_STATE_CP

} POWER_KEY_STATE;


TIMER			PowerKeyWaitTimer;
POWER_KEY_STATE	PowerKeyState;
BOOL			IsPwrkeyInSysOn = TRUE;		//��ʾ��ǰ�Ƿ���ϵͳ�����׶ε�POWER_KEYʶ��

#ifdef AU6210K_XLX_ALD800
static CODE BYTE PowerKeyEvent[3] = {MSG_MODE_SW, MSG_SYS_ON, MSG_SYS_OFF};	
#else
static CODE BYTE PowerKeyEvent[3] = {MSG_NONE, MSG_SYS_ON, MSG_SYS_OFF};	
#endif



// Initialize POWER_KEY scan operation.
VOID PowerKeyScanInit(VOID)
{
	DBG(("PowerKeyScanInit*******\n"));
	PowerKeyState = POWER_KEY_STATE_IDLE;
	IsPwrkeyInSysOn = TRUE;					//������������ϵͳ�������˺�����ϵͳ�ر�
}

extern BOOL BtPowerIsOn;
//
// POWER_KEY����ͨ�İ�����ͬ�����Ӱ�ť���أ����أ�ʱ����Ҫ���û���ϵͳ���ػ�����Ȼ��Ҳ�����ö̰����ܡ�
// �̰�����ʱ�����Ͷ̰���Ϣ�������̰����䣬����ϵͳ���ػ���Ϣ��
MESSAGE PowerKeyEventGet(VOID)							
{
	//DBG(("PowerKeyEventGet*******\n"));
	//DBG(("PowerKeyState:%bx\n", PowerKeyState));
	//DBG(("gIsPwrkeyPadOn:%bx\n", gIsPwrkeyPadOn));
	switch(PowerKeyState)
	{
		case POWER_KEY_STATE_IDLE:
			if(gIsPwrkeyPadOn == FALSE)
			{
				IsPwrkeyInSysOn = FALSE;
				if(BtPowerIsOn)
				{
					baGPIOCtrl[GPIO_D_OUT] &= ~0x20; //D5
				}
				return MSG_NONE;
			}
			DBG(("GOTO POWER PRESS DOWN!\n"));

#if defined(TIME_POWER_ON_HOLD)
			if(IsPwrkeyInSysOn == TRUE)
			{
				TimeOutSet(&PowerKeyWaitTimer, TIME_POWER_ON_HOLD);	
			}
			else
#endif
			{
				TimeOutSet(&PowerKeyWaitTimer, TIME_POWER_OFF_HOLD);
			}

			if(BtPowerIsOn)
			{
				if(GET_BT_CALL_STATUS() || (gSys.SystemMode == SYS_MODE_BLUETOOTH))
					baGPIOCtrl[GPIO_D_OUT] |= 0x20; //D5
			}

			PowerKeyState = POWER_KEY_STATE_PRESS_DOWN;
			break;

		case POWER_KEY_STATE_PRESS_DOWN:
			if(gIsPwrkeyPadOn == FALSE)
			{
				IsPwrkeyInSysOn = FALSE;
				PowerKeyState = POWER_KEY_STATE_IDLE;
				if(BtPowerIsOn)
				{
					baGPIOCtrl[GPIO_D_OUT] &= ~0x20; //D5
				}
				
				return PowerKeyEvent[0];			//return key sp value
			}
			else if(IsTimeOut(&PowerKeyWaitTimer))
			{
				TimeOutSet(&PowerKeyWaitTimer, POWER_KEY_CPH_TIME);
				PowerKeyState = POWER_KEY_STATE_CP;
				if(IsPwrkeyInSysOn == TRUE)			//ϵͳ����ʱ������MSG_SYS_ON��Ϣ������֪ͨPOWER_KEY��������ʱ���ѵ�
				{
					DBG(("PowerKeyEvent[1]:%bx\n", PowerKeyEvent[1]));
					return PowerKeyEvent[1];
				}
				else								//ϵͳ����֮��POWER_KEY�ĳ���ֻ����MSG_SYS_OFF��Ϣ�����ڹر�ϵͳ
				{
					DBG(("PowerKeyEvent[2]:%bx\n", PowerKeyEvent[2]));
					return PowerKeyEvent[2];
				}
			}
			break;
			
		case POWER_KEY_STATE_CP:
			if(gIsPwrkeyPadOn == FALSE)
			{
				IsPwrkeyInSysOn = FALSE;			//ϵͳ������POWER_KEY���������ñ�Ϊ�ػ�
				PowerKeyState = POWER_KEY_STATE_IDLE;
			}
			else if(IsTimeOut(&PowerKeyWaitTimer))
			{
				TimeOutSet(&PowerKeyWaitTimer, POWER_KEY_CPH_TIME);
				if(IsPwrkeyInSysOn == FALSE)		//ϵͳ������POWER_KEY�ĳ���ֻ����MSG_SYS_OFF��Ϣ�����ڹر�ϵͳ
				{
					DBG(("PowerKeyEvent[2]:%bx\n", PowerKeyEvent[2]));
					return PowerKeyEvent[2];
				}
			}
			break;	
					
		default:
			PowerKeyState = POWER_KEY_STATE_IDLE;
			break;
	}
	return MSG_NONE;
}

#endif
