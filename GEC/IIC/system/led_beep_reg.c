#include "led_beep_reg.h"

#define  _USE_LIBDEF

void led_init_reg( void )
{
	
#ifdef	_USE_MYDEF
	// F组时钟 打开
	RCC_AHB1EN |= 1<<5;
	
	//PF9 --> 输出模式
	GPIOF_MODER |= 1<<18;
	GPIOF_MODER &= ~(1<<19);
	
	//PF9 --> 输出 开漏
	GPIOF_OTYPER |= 1<<9;

	//PF9 --> 无上下拉
	GPIOF_PUPDR &= ~(3<<18);
	
	//PF9 --> 25M
	GPIOF_OSPEEDER |= 1<<18;
	GPIOF_OSPEEDER &= ~(1<<19);
	
#elif defined(_USE_LIBDEF)
	
	RCC->AHB1ENR |= 1<<5;
	GPIOF->MODER |= 1<<18;
	GPIOF->MODER &= ~(1<<19);
	GPIOF->OTYPER |= 1<<9;
	GPIOF->PUPDR &= ~(3<<18);
	
	GPIOF->OSPEEDR |= 1<<18;
	GPIOF->OSPEEDR &= ~(1<<19);
	
#endif
}
