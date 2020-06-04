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


typedef enum{

	NONE_PRESS = 0,
	S1_PRESS = 1,
	S2_PRESS,
	S3_PRESS,
	S4_PRESS,
	
}key_Press_Type;


void key_init( void );
key_Press_Type key_det( void );
void PassWord( void );
void key_exti_init( void );

#endif



