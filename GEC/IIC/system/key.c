#include "key.h"

u8 pw[ 4 ] = { S1_PRESS, S2_PRESS, S4_PRESS, S3_PRESS };


// 按键的初始化
void key_init( void )
{
	
	//1. 配置启动 IO口时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE, ENABLE);
	
	//2. 配置IO口的功能
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin 	= GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode 	= GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	GPIO_Init( GPIOA, &GPIO_InitStruct );
	
	GPIO_InitStruct.GPIO_Pin 	= GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_Init( GPIOE, &GPIO_InitStruct );

}


// 按键的检测，并返回按键的 枚举值
u8 key_det( void )
{

	u8 type = NONE_PRESS;
	
	if( !S1 || !S2 || !S3 || !S4  ){
		
		delay_ms( 40 ); //按下消抖
		
		if( !S1 )type = S1_PRESS;
		else if( !S2 )type = S2_PRESS;
		else if( !S3 )type = S3_PRESS;
		else if( !S4 )type = S4_PRESS;
		
		//等待按键送开
		while( !S1 || !S2 || !S3 || !S4 ){}	
		delay_ms( 40 );	//送开消抖	

	}

	
	return type;
}




//密码检测
void PassWord( void )
{
	int i;
	
	//声明一个数组保存 按键值
	u8 key_val[ 4 ];
	bool rslt;
	
PW:
	rslt = true;
	for( i = 0; i<4; i++ ){
		
		// 等待 直到有 按键按下 则返回
		while( key_val[i] = key_det(), key_val[i] ==  NONE_PRESS );

		if( key_val[i] != pw[i] )rslt = false;
	}
	
	if( rslt == true ){
		led_blink( 3, 100 );
		goto end;
	}else{
		beep_ctrl( 3, 100 );
		goto PW;
	} 
	
end:	
	return ;
}


// PA0-S1   PE2-S2   PE3-S3  PE4-S4
void key_exti_init( void ){
	
	//0. 打开 syscfg 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	//1. 对应引脚配置为输入模式
	//key_init();
 	
	//2. 为中断线 选择 一个 对应的 IO 口
	SYSCFG_EXTILineConfig( EXTI_PortSourceGPIOA,  EXTI_PinSource0);
	SYSCFG_EXTILineConfig( EXTI_PortSourceGPIOE,  EXTI_PinSource2);
	SYSCFG_EXTILineConfig( EXTI_PortSourceGPIOE,  EXTI_PinSource3);
	SYSCFG_EXTILineConfig( EXTI_PortSourceGPIOE,  EXTI_PinSource4);
	
	//3. 配置中断的 方式
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line 	= EXTI_Line0 | EXTI_Line2 | EXTI_Line3 | EXTI_Line4;
	EXTI_InitStruct.EXTI_LineCmd 	= ENABLE;
	EXTI_InitStruct.EXTI_Mode 	= EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger 	= EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStruct);
	
	//4. 配置中断优先级
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel 			= EXTI0_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd 			= ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority 	= 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority 		= 1;
	NVIC_Init(&NVIC_InitStruct);

	NVIC_InitStruct.NVIC_IRQChannel 			= EXTI2_IRQn;
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel 			= EXTI3_IRQn;
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel 			= EXTI4_IRQn;
	NVIC_Init(&NVIC_InitStruct);
}




