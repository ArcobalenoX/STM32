#ifndef __ENCODER_H
#define __ENCODER_H
#include <sys.h>
#include "delay.h"
/**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
#define ENCODER_TIM_PERIOD (u16)(65535)   //不可大于65535 因为F103的定时器是16位的。
extern  int Encoder_Left;
extern int circle_count;

void Encoder_Init_TIM4(void);
int Read_Encoder(u8 TIMX);
void TIM3_Int_Init(u16 arr,u16 psc);


#endif
