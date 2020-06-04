#include "led_beep.h"


void led_beep_init( void )
{
	//1. 配置启动 IO口时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOE, ENABLE);
	
	//2. 配置IO口的功能
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin 	= GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode 	= GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_OD;
	GPIO_InitStruct.GPIO_PuPd 	= GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed 	= GPIO_Speed_25MHz;
	GPIO_Init( GPIOF, &GPIO_InitStruct );
	
	GPIO_InitStruct.GPIO_Pin 	= GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_Init( GPIOE, &GPIO_InitStruct );
	
	GPIO_InitStruct.GPIO_Pin 	= GPIO_Pin_8;
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP;	
	GPIO_Init( GPIOF, &GPIO_InitStruct );
	
	// 3. 熄灭4个led灯
	all_ctrl( 0 );
}

void led_blink( uint16_t n , uint16_t time ){

	while( n-- ){
		all_ctrl( 1 ); Delay( time );
		all_ctrl( 0 ); Delay( time );
	}
	
	return;
}




void led_flow( uint16_t n , uint16_t time )
{
	while( n-- ){
		all_ctrl( 0 );
		
		led1( 1 );
		Delay( time );

		led2( 1 );
		Delay( time );		

		led3( 1 );
		Delay( time );

		led4( 1 );
		Delay( time );		
	}
}



void one_run( uint16_t n , uint16_t time )
{
	all_ctrl( 0 );
	
	while( n-- ){
		
		led4( 0 ), led1( 1 );
		Delay( time );
		
		led1( 0 ), led2( 1 );
		Delay( time );

		led2( 0 ), led3( 1 );
		Delay( time );

		led3( 0 ), led4( 1 );
		Delay( time );		
	}
	
	led4( 0 );

}



void beep_ctrl( uint16_t n , uint16_t time )
{
	while( n-- )
	{
		beep( 1 );
		Delay( time );
		
		beep( 0 );
		Delay( time );
	}
}


