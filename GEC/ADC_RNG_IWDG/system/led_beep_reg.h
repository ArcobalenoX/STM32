#ifndef  __LED_BEEP_REG_H
#define  __LED_BEEP_REG_H

#include "stm32f4xx.h"
#include "delay.h"


#define		RCC_AHB1EN	(*(volatile unsigned *)(0x40023800 + 0x30))

#define 	GPIOF_MODER	(*(volatile unsigned *)(0x40021400 + 0x00))
#define 	GPIOF_OTYPER	(*(volatile unsigned *)(0x40021400 + 0x04))
#define 	GPIOF_OSPEEDER	(*(volatile unsigned *)(0x40021400 + 0x08))
#define 	GPIOF_PUPDR	(*(volatile unsigned *)(0x40021400 + 0x0C))
#define 	GPIOF_ODR	(*(volatile unsigned *)(0x40021400 + 0x14))
	
#define 	GPIOF_BSSRH	(*(volatile unsigned short *)(0x40021400 + 0x1A))
#define 	GPIOF_BSSRL	(*(volatile unsigned short *)(0x40021400 + 0x18))
	
//#define		led1_reg( n )	n?( GPIOF_BSSRH = 1<<9 ) : ( GPIOF_BSSRL |= 1<<9 )
#define		led1_reg( n )	n?( GPIOF->BSRRH = 1<<9 ) : ( GPIOF->BSRRL |= 1<<9 )

void led_init_reg( void );

#endif

