#ifndef __BT_CTRL_H__
#define __BT_CTRL_H__


#ifdef FUNC_BLUETOOTH_CSR_EN
//蓝牙GPIO口控制

#define BT_POWER_ON_INIT()/*D0*/	do{ \	
	SetGpioRegBit(GPIO_B_PU, (1 << 7));\
	SetGpioRegBit(GPIO_B_PD, (1 << 7));	\
	SetGpioRegBit(GPIO_B_OE, (1 << 7));\
	ClrGpioRegBit(GPIO_B_IE, (1 << 7));\
	ClrGpioRegBit(GPIO_B_OUT, (1 << 7));\
	}while(0)


#define		BT_POWER_L()	SetGpioRegBit(GPIO_B_OE,	1<<7),\	
	                       	ClrGpioRegBit(GPIO_B_OUT, 	1<<7);

#define 	BT_POWER_H()  	SetGpioRegBit(GPIO_B_OE, 	1<<7),\	
	                        SetGpioRegBit(GPIO_B_OUT, 	1<<7);

//////////////////////////////////////////////////////////////
//灯设置状态 从MCU输出的口
//////////////////////////////////////////////////////////////
#define		LED_BLUE_H()    SetGpioRegBit(GPIO_A_OUT, 	1<<0);
#define		LED_BLUE_L()    ClrGpioRegBit(GPIO_A_OUT, 	1<<0);    

#define		LED_RED_H()     SetGpioRegBit(GPIO_B_OUT, 	1<<5);
#define		LED_RED_L()     ClrGpioRegBit(GPIO_B_OUT, 	1<<5);

#endif


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
//灯 输入在这里设置
//////////////////////////////////////////////////////////////////////
#define BTLED_PORT //need config
#define MCUIO_PORT  //need config

#ifdef BTLED_PORT
#define BTIO_PORT_BLUE_LED_INIT()/*D0*/	do{ \	
	ClrGpioRegBit(GPIO_D_PU, (1 << 0));\
	ClrGpioRegBit(GPIO_D_PD, (1 << 0));	\
	ClrGpioRegBit(GPIO_D_OE, (1 << 0));\
	SetGpioRegBit(GPIO_D_IE, (1 << 0));\
	}while(0)
#define GET_BLUE_LED_STATUS()/*D0*/	 \	
	GetGpioReg(GPIO_D_IN) & (1 << 0)



#define BTIO_PORT_RED_LED_INIT()/*D1*/	do{ \	
	ClrGpioRegBit(GPIO_D_PU, (1 << 1));\
	ClrGpioRegBit(GPIO_D_PD, (1 << 1));	\
	ClrGpioRegBit(GPIO_D_OE, (1 << 1));\
	SetGpioRegBit(GPIO_D_IE, (1 << 1));\
	}while(0)
#define GET_RED_LED_STATUS()/*D1*/	\	
	GetGpioReg(GPIO_D_IN) & (1 << 1)

#endif




#ifdef MCUIO_PORT
#define MCUIO_PORT_red_LED_INIT()/*D1*/	do{ \	
	SetGpioRegBit(GPIO_B_PU, (1 << 5));\
	ClrGpioRegBit(GPIO_B_PD, (1 << 5));	\
	ClrGpioRegBit(GPIO_B_OE, (1 << 5));\
	SetGpioRegBit(GPIO_B_OUT, (1 << 5));\
	ClrGpioRegBit(GPIO_B_IE, (1 << 5));\
	}while(0)
#define SET_MCU_red_LED_PORT()/*D1*/	do{ \	
	SetGpioRegBit(GPIO_B_OUT, (1 << 5)); \
	}while(0)

#define CLR_MCU_red_LED_PORT()/*D1*/	do{ \	
	ClrGpioRegBit(GPIO_B_OUT, (1 << 5)); \
	}while(0)
	

#define MCUIO_PORT_blue_LED_INIT()/*D1*/	do{ \	
	SetGpioRegBit(GPIO_A_PU, (1 << 0));\
	ClrGpioRegBit(GPIO_A_PD, (1 << 0));	\
	ClrGpioRegBit(GPIO_A_OE, (1 << 0));\
	SetGpioRegBit(GPIO_A_OUT, (1 << 0));\
	ClrGpioRegBit(GPIO_A_IE, (1 << 0));\
	}while(0)
#define SET_MCU_blue_LED_PORT()/*D1*/	do{ \	
	SetGpioRegBit(GPIO_A_OUT, (1 << 0));\
	}while(0)
#define CLR_MCU_blue_LED_PORT()/*D1*/	do{ \	
	ClrGpioRegBit(GPIO_A_OUT, (1 << 0));\
	}while(0)


#endif


#define btIO_Red_Led_is_High()  \	
	GET_RED_LED_STATUS()
	

#define btIO_Blue_Led_is_High()  \	
	GET_BLUE_LED_STATUS()


#define setMCU_Red__Led_Port()  do{ \	
	SET_MCU_red_LED_PORT(); \
	}while(0)	

#define clrMCU_Red__Led_Port()  do{ \	
	CLR_MCU_red_LED_PORT(); \
	}while(0)

#define setMCU_Blue__Led_Port()  do{ \	
	SET_MCU_blue_LED_PORT(); \
	}while(0)	

#define clrMCU_Blue__Led_Port()  do{ \	
	CLR_MCU_blue_LED_PORT(); \
	}while(0)





//////////////////////////////////////////////////////////////////////
//mute设置
//////////////////////////////////////////////////////////////////////

#if 0//def AU6210K_LK_SJ_CSRBT

#define BTIO_PORT_MUTE_INIT()/*B6*/	do{ \	
	SetGpioRegBit(GPIO_A_PU, (1 << 7));\
	ClrGpioRegBit(GPIO_A_PD, (1 << 7));	\
	ClrGpioRegBit(GPIO_A_OE, (1 << 7));\
	SetGpioRegBit(GPIO_A_IE, (1 << 7));\
	}while(0)
#define GET_BT_MUTE_STATUS()/*B6*/	\	
	GetGpioReg(GPIO_A_IN) & (1 << 7)
	

#elif defined(AU6210K_ZB_BT007_CSR) 

#define BTIO_PORT_MUTE_INIT()/*a7*/	do{ \	
	SetGpioRegBit(GPIO_B_IE, (1 << 6));\
	ClrGpioRegBit(GPIO_B_OE, (1 << 6));\
	SetGpioRegBit(GPIO_B_PU, (1 << 6));\
	SetGpioRegBit(GPIO_B_PD, (1 << 6));	\
	}while(0)
#define GET_BT_MUTE_STATUS()/*a7*/	\	
	GetGpioReg(GPIO_B_IN) & (1 << 6)


#define btIO_Mute_Need_mute() \
	GET_BT_MUTE_STATUS()




#ifdef CSR_CALL_CHECK_PORT
#define BTIO_PORT_CALL_INIT()/*a7*/	do{ \	
	SetGpioRegBit(GPIO_A_IE, (1 << 7));\
	ClrGpioRegBit(GPIO_A_OE, (1 << 7));\
	SetGpioRegBit(GPIO_A_PU, (1 << 7));\
	SetGpioRegBit(GPIO_A_PD, (1 << 7));	\
	}while(0)
#define GET_BT_CALL_STATUS()/*a7*/	\	
	GetGpioReg(GPIO_A_IN) & (1 << 7)

#endif

	




		
#else //纳瑞csr+6210k+dsp/pt2313 D8案子是用B5 ,而B6做电压检测了
#if 0
#define BTIO_PORT_MUTE_INIT()/*B5*/	do{ \	
	ClrGpioRegBit(GPIO_B_PU, (1 << 5));\
	ClrGpioRegBit(GPIO_B_PD, (1 << 5));	\
	ClrGpioRegBit(GPIO_B_OE, (1 << 5));\
	SetGpioRegBit(GPIO_B_IE, (1 << 5));\
	}while(0)
#define GET_BT_MUTE_STATUS()/*B5*/	\	
	GetGpioReg(GPIO_B_IN) & (1 << 5)
#endif
#endif
	
VOID LineInCtrlInit(VOID);

VOID LineinIODeinit(VOID);

// line-in mode state control.
VOID LineInStateCtrl(VOID);
typedef enum _BLUETOOTH_STATUS {
	BT_IDLE = 0,
	BT_CONNECTED,
	BT_INCOMING_CALL,
	BT_MAKE_CALL,
	BT_IN_THE_CALL,
}BLUETOOTH_STATUS;
extern BLUETOOTH_STATUS gBlutoothStatus;
VOID BluetoothCtrlInit();
VOID BluetothCtrlEnd();
// bluetooth mode state control.
VOID BluetoothStateCtrl();
VOID SetBluetoothVolume(BYTE Vol);
BOOL BTisMute();
#endif
