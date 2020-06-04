#ifndef  __KEY_H
#define  __KEY_H

#include "stm32f4xx.h"
#include "delay.h"
#include "stdbool.h"
#include "led_beep.h"
#include "BitBandOP.h"


#define		S1	PAIn(0)
#define		S2	PEIn(2)
#define		S3	PEIn(3)
#define		S4	PEIn(4)



#define	NONE_PRESS  0
#define	S1_PRESS    1
#define	S2_PRESS	2
#define	S3_PRESS	3
#define	S4_PRESS	4
	

extern u8 pw[ 4 ];

void key_init( void );
u8 key_det( void );
void PassWord( void );
void key_exti_init( void );

#endif



