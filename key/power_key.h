#ifndef __POWER_KEY_H__
#define __POWER_KEY_H__

#include "message.h"


// Initialize POWER_KEY scan operation.
VOID PowerKeyScanInit(VOID);

// POWER_KEY����ͨ�İ�����ͬ�����Ӱ�ť���أ����أ�ʱ����Ҫ���û���ϵͳ���ػ�����Ȼ��Ҳ�����ö̰����ܡ�
// �̰�����ʱ�����Ͷ̰���Ϣ�������̰����䣬����ϵͳ���ػ���Ϣ��
MESSAGE PowerKeyEventGet(VOID);


#endif
