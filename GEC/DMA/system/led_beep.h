#ifndef  __LED_BEEP_H
#define  __LED_BEEP_H

#include "stm32f4xx.h"
#include "delay.h"

/* Exported types ------------------------------------------------------------*/
/* Exported defines ------------------------------------------------------------*/
#define		led1( n )		n?(GPIO_ResetBits( GPIOF, GPIO_Pin_9 )):(GPIO_SetBits( GPIOF, GPIO_Pin_9 ))
#define		led2( n )		n?(GPIO_ResetBits( GPIOF, GPIO_Pin_10 )):(GPIO_SetBits( GPIOF, GPIO_Pin_10 ))
#define		led3( n )		n?(GPIO_ResetBits( GPIOE, GPIO_Pin_13 )):(GPIO_SetBits( GPIOE, GPIO_Pin_13 ))
#define		led4( n )		n?(GPIO_ResetBits( GPIOE, GPIO_Pin_14 )):(GPIO_SetBits( GPIOE, GPIO_Pin_14 ))

#define		all_ctrl( n )		(led1( n ), led2( n ), led3( n ), led4( n ))

#define		beep( n )		n?(GPIO_SetBits( GPIOF, GPIO_Pin_8 )):(GPIO_ResetBits( GPIOF, GPIO_Pin_8 ))


/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void led_beep_init( void );
void led_flow( uint16_t n , uint16_t time );
void one_run( uint16_t n , uint16_t time );
void beep_ctrl( uint16_t n , uint16_t time );
void led_blink( uint16_t n , uint16_t time );

#endif


