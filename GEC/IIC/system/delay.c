#include "delay.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t uwTimingDelay;
RCC_ClocksTypeDef RCC_Clocks;

/* Private function prototypes -----------------------------------------------*/



void delay_init( void )
{

	/* SysTick end of count event each 100us */
	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 10000 );
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{ 
  uwTimingDelay = nTime;

  while(uwTimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (uwTimingDelay != 0x00)
  { 
    uwTimingDelay--;
  }
}



void delay_ms( __IO uint32_t nms ){
	
	__IO uint32_t state;
	
	for( int i = 0; i < nms/700 ; i++ ){
	
		SysTick->CTRL = 0; // 关闭定时器
		SysTick->LOAD = 700 * 21000 - 1; // 50ms
		SysTick->VAL = 0; // 清除当前的值 && SysTick->CTRL位16清0
		SysTick->CTRL = 1; // 启动了定时器 && 21MHz
		
		do{
			state = SysTick->CTRL;
		}while ( (state&0x01) && !(state & 0x00010000) );// 等待计时结束

		SysTick->CTRL = 0; // 关闭定时器		
	}
	
	if( nms % 700 ){
	
		SysTick->CTRL = 0; // 关闭定时器
		SysTick->LOAD = nms%700 * 21000 - 1; // 
		SysTick->VAL = 0; // 清除当前的值 && SysTick->CTRL位16清0
		SysTick->CTRL = 1; // 启动了定时器 && 21MHz
		do{
			state = SysTick->CTRL;
		}while ( (state&0x01) && !(state & 0x00010000) );// 等待计时结束

		
		SysTick->CTRL = 0; // 关闭定时器	
	}
	
}


void delay_us( __IO uint32_t nus ){
	
	__IO uint32_t state;
	

	SysTick->CTRL = 0; // 关闭定时器
	SysTick->LOAD = nus * 21 - 1; // 
	SysTick->VAL = 0; // 清除当前的值 && SysTick->CTRL位16清0
	SysTick->CTRL = 1; // 启动了定时器 && 21MHz
	do{
		state = SysTick->CTRL;
	}while ( (state&0x01) && !(state & 0x00010000) );// 等待计时结束

	
	SysTick->CTRL = 0; // 关闭定时器	
	
	
}

