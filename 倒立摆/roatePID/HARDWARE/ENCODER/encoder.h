#ifndef __ENCODER_H
#define __ENCODER_H
#include <sys.h>
#include "delay.h"
/**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
#define ENCODER_TIM_PERIOD (u16)(20000)   //不可大于65535 因为定时器是16位的。
void Encoder_Init_TIM4(void);
#endif
