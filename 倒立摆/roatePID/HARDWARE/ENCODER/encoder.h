#ifndef __ENCODER_H
#define __ENCODER_H
#include <sys.h>
#include "delay.h"
/**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
#define ENCODER_TIM_PERIOD (u16)(20000)   //���ɴ���65535 ��Ϊ��ʱ����16λ�ġ�
void Encoder_Init_TIM4(void);
#endif
