#include <string.h>
#include <reg51.h>										//include public header files

#include "type.h"										//include project header files
#include "syscfg.h"
#include "utility.h"
#include "debug.h"
#include "timer.h"
#include "sysctrl.h"
#include "key.h"
#include "touch_key.h"
#include "gpio.h"
#include "touchkey_ctrl.h"
#include "message.h"
#include "touchkeylib.h"
#include "touch_key.h"
#include "touchkey_ctrl.h"


#ifdef FUNC_TOUCH_KEY_EN


TOUCH_KEY TK;
BYTE gTouchKeyVol;


static VOID DoutSmooth(WORD TKDout)
{
	BYTE i;
	BYTE MaxCount = 0;
	BYTE MinCount = 0; 
	BYTE count = 0;
	DWORD sum = 0;

//�˲�����...	
	if(TK.ChData[TK.Index].SmoothCount < DOUT_SMOOTH_LEN)//û����6��  �ͱ���
	{
		TK.ChData[TK.Index].SmoothBuffer[TK.ChData[TK.Index].SmoothCount] = TKDout;
		TK.ChData[TK.Index].SmoothCount++; 
	}
	else 
	{	
		for(i = 0; i < DOUT_SMOOTH_LEN; i++)
		{
			if(TK.ChData[TK.Index].SmoothBuffer[MaxCount] < TK.ChData[TK.Index].SmoothBuffer[i])
			{
				MaxCount = i;
			}
			else if(TK.ChData[TK.Index].SmoothBuffer[MinCount]  > TK.ChData[TK.Index].SmoothBuffer[i])
			{
				MinCount = i;
			}
			sum += TK.ChData[TK.Index].SmoothBuffer[i];
		}
		sum -= TK.ChData[TK.Index].SmoothBuffer[MinCount];
		sum -= TK.ChData[TK.Index].SmoothBuffer[MaxCount];		
		TK.ChData[TK.Index].SmoothOutput = sum >> 2;

//Base�״θ�ֵ
		if(TK.BaseFirFlag & (1 << TK.Index))
		{
			TK.ChData[TK.Index].Base  = TK.ChData[TK.Index].SmoothOutput; 
			TK.BaseFirFlag &= ~ (1 << TK.Index); 
		}
		
		{
			TK.ChData[TK.Index].Delta = TK.ChData[TK.Index].SmoothOutput - TK.ChData[TK.Index].Base;//ƽ��ֵ - ��ֵ = ƫ��ֵ
			//DBG(("%02bx  %d\n",TK.Index, TK.ChData[TK.Index].Delta));	
			if(TK.ChData[TK.Index].Delta < 0)//ƽ��ֵ < ��ֵ   �ж��ͷ� 
			{	
				//���ҽ���û�а���ʱ��baseУ׼��ֹ����Ӱ��base 	
				if(TK.BaseCheckState == 0)
				{
				    count =  TK.ChData[TK.Index].BaseCount;		
					TK.ChData[TK.Index].BaseCount++;
		
					if(TK.ChData[TK.Index].BaseCount >= 30)//��ֹ���µĸ�����ŵ�
					{
						TK.ChData[TK.Index].Base = (TK.ChData[TK.Index].Base + TK.ChData[TK.Index].SmoothOutput) >> 1;
						TK.ChData[TK.Index].Delta = 0;
						TK.ChData[TK.Index].BaseCount = 0;	
					}
				}

				//�����ͷ�
				if(TK.ChData[TK.Index].KeyState == 1)
				{
					TK.ChData[TK.Index].KeyState = 0;
					gTouchKeyVol = 0;	
					//DBG(("\n%2BX RL1 %u\n",TK.Index,TK.ChData[TK.Index].SmoothOutput));
				}	
				 
				//��Ч��Ч�����ͷ�
				TK.BaseCheckState &= ~(1 << TK.Index);
				//DBG(("1\n"));
			}
			else if(TK.ChData[TK.Index].Delta < TK.ChData[TK.Index].Threshold)//ƫ�� С��  �̶�ƫ�� �����������ͷ�
			{
				TK.ChData[TK.Index].BaseCount = 0;
		
				//���ҽ���û�а���ʱ��baseУ׼��ֹ����Ӱ��base 	
				if(!TK.BaseCheckState)
				{
					TK.ChData[TK.Index].Base += 1;
				}
				
				//�����ͷ�
				if(TK.ChData[TK.Index].KeyState == 1)
				{
					TK.ChData[TK.Index].KeyState = 0;
					gTouchKeyVol = 0;	
					//DBG(("\n%2BX RL2 %u\n",TK.Index,TK.ChData[TK.Index].SmoothOutput));
				}
				//gTouchKeyVol = 0;
				//��Ч��Ч�����ͷ�	
				TK.BaseCheckState &= ~(1 << TK.Index);
				//DBG(("2\n"));
			}
			else
			{
				TK.ChData[TK.Index].BaseCount = 0;		
				if(TK.ChData[TK.Index].Delta < (TK.ChData[TK.Index].Threshold << 1))//��ֹ���ϵĸ�����ŵ�	
				{
					if(TK.ChData[TK.Index].KeyState == 0)
					{			
						//�仯����Ϊ��������������Ϊ�Ա߰��������Ľϴ����		
						for(i = 0; i < 8; i++)
						{
							if(TK.ChData[i].Delta > TK.ChData[TK.Index].Delta)
							{
								break;
							}
						}			
						if(i > 7)
						{
							TK.ChData[TK.Index].KeyState = 1;//׼ȷ����
							gTouchKeyVol = (1 << TK.Index);
							//DBG(("gTouchKeyVol = %02BX\n", gTouchKeyVol));
						}			
					}
					//���԰���
					TK.BaseCheckState |= 1 << TK.Index;
				}
//				else
//				{
//					gTouchKeyVol = 0;
//				}
				//DBG(("3\n"));	
			}
		}
		//DBG(("XX %02BX\n",TK.ChData[TK.Index].KeyState));
		if(TK.ChData[TK.Index].RepCount >= DOUT_SMOOTH_LEN)
		{
			TK.ChData[TK.Index].RepCount = 0;
		}
	    TK.ChData[TK.Index].SmoothBuffer[TK.ChData[TK.Index].RepCount] = TKDout;
		TK.ChData[TK.Index].RepCount++;
	}
}


VOID TouchKeyDoutPro(VOID)
{
	WORD  Dout;
	
	if(TouchKeyIsConvertOK())//ת�����
	{		
		Dout =  TouchKeyGetDout();//��ȡDout
		DoutSmooth(Dout);	
		do
		{
			if(TK.Index < 7) 
			{
				TK.Index++;
			}
			else 
			{
				TK.Index = 0;
			}
		}
		while((TK.ChannelEn & (1 << TK.Index)) == 0);//channel switch
		
		TouchKeyStartConvert(TK.Index);
	}	
}


#endif