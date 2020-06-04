#ifndef  __KEY_H
#define  __KEY_H

#include "stm32f4xx.h"
#include "delay.h"
#include "stdbool.h"
#include "led_beep.h"

#define		S1	( GPIO_ReadInputDataBit( GPIOA, GPIO_Pin_0 ) )
#define		S2	( GPIO_ReadInputDataBit( GPIOE, GPIO_Pin_2 ) )
#define		S3	( GPIO_ReadInputDataBit( GPIOE, GPIO_Pin_3 ) )
#define		S4	( GPIO_ReadInputDataBit( GPIOE, GPIO_Pin_4 ) )


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

#endif



