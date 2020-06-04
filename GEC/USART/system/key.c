#include "key.h"

static const key_Press_Type pw[ 4 ] = { S1_PRESS, S2_PRESS, S4_PRESS, S3_PRESS };


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
key_Press_Type key_det( void )
{

	key_Press_Type type = NONE_PRESS;
	
	if( !S1 || !S2 || !S3 || !S4  ){
		
		Delay( 200 ); //按下消抖
		
		if( !S1 )type = S1_PRESS;
		else if( !S2 )type = S2_PRESS;
		else if( !S3 )type = S3_PRESS;
		else if( !S4 )type = S4_PRESS;
		
		//等待按键送开
		while( !S1 || !S2 || !S3 || !S4 ){}	
		Delay( 200 );	//送开消抖	

	}

	
	return type;
}




//密码检测
void PassWord( void )
{
	int i;
	
	//声明一个数组保存 按键值
	key_Press_Type key_val[ 4 ];
	bool rslt;
	
PW:
	rslt = true;
	for( i = 0; i<4; i++ ){
		
		// 等待 直到有 按键按下 则返回
		while( key_val[i] = key_det(), key_val[i] ==  NONE_PRESS );

		if( key_val[i] != pw[i] )rslt = false;
	}
	
	if( rslt == true ){
		led_blink( 3, 1000 );
		goto end;
	}else{
		beep_ctrl( 3, 1000 );
		goto PW;
	} 
	
end:	
	return ;
}



