#ifndef __GPIO_KEY_H__
#define __GPIO_KEY_H__

// 3��GPIO��ʵ��5������ɨ���㷨����ԭ��:
//1)������GPIO�̽���һ��һ��ʹ���ڲ���������һ��ʹ���ڲ�ǿ������
//		��ô������GPIO�϶��ǵ͵�ƽ��0.1V���ң��� 
//2)������GPIO�̽���һ��һ��ʹ���ڲ�ǿ������һ��ʹ���ڲ���������
//		��ô������GPIO�϶��Ǹߵ�ƽ��3.0V���ң�

//
// Initialize gpio key scan (GPIO) operation.
//
VOID GpioKeyScanInit(VOID);

//
// Key process, image key value to key event.
//
MESSAGE GpioKeyEventGet(VOID);
#endif 

