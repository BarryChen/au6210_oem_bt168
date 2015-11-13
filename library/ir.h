#ifndef __IR_H__
#define	__IR_H__

typedef enum			  
{
	IR_INPUT_GPIOB6,
	IR_INPUT_GPIOE0,

}  IRGPIO_INPUT_SEL;

VOID IrInit(VOID);

VOID IRDisable(VOID);

//ѡ��IR�Ķ˿�GPIOB6��GPIOE0
//IR��Ӳ����֧�ָ�ѹ���룬IR����ͷ�������Ϊ3.3v 
VOID IRChannelSel(IRGPIO_INPUT_SEL IRChannel);

BOOL IrIsKeyCome(VOID);

DWORD IrGetKeyCode(VOID);

BOOL IrIsContinuePrs(VOID);

VOID IrIgnoreLeadHeader(BOOL IsIgnore);

//sleep ���IR״̬����ʱ��ȡ��IR��ֵ
DWORD IrKeyDataVal(VOID);

#endif
