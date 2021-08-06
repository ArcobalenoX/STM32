#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"

extern volatile u8 matkey;

void TIM5_Int(u16 arr,u16 psc);


#endif
