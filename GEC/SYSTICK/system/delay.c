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

void delay_nms(int nms)
{
	for(int i=0;i<nms/700;i++)
	{
		SysTick->CTRL = 0; // Disable SysTick
		SysTick->LOAD = 700*21000-1; // Count from 255 to 0 (256 cycles)
		SysTick->VAL = 0; // Clear current value as well as count flag
		SysTick->CTRL = 1; // Enable SysTick timer with processor clock
		while ((SysTick->CTRL & 0x00010000)==0);// Wait until count flag is set
	}
	if(nms%700)
	{
		SysTick->CTRL = 0; // Disable SysTick
		SysTick->LOAD = nms%700*21000-1; // Count from 255 to 0 (256 cycles)
		SysTick->VAL = 0; // Clear current value as well as count flag
		SysTick->CTRL = 1; // Enable SysTick timer with processor clock
		while ((SysTick->CTRL & 0x00010000)==0);// Wait until count flag is set
	}
}


void delay_nus(int nus)
{
	for(int i=0;i<nus/50;i++)
	{
		SysTick->CTRL = 0; // Disable SysTick
		SysTick->LOAD = 50*168-1; // Count from 255 to 0 (256 cycles)
		SysTick->VAL = 0; // Clear current value as well as count flag
		SysTick->CTRL = 1; // Enable SysTick timer with processor clock
		while ((SysTick->CTRL & 0x00010000)==0);// Wait until count flag is set
	}
	
	if(nus%50)
	{
		SysTick->CTRL = 0; // Disable SysTick
		SysTick->LOAD = nus%50*168-1; // Count from 255 to 0 (256 cycles)
		SysTick->VAL = 0; // Clear current value as well as count flag
		SysTick->CTRL = 5; // Enable SysTick timer with processor clock
		while ((SysTick->CTRL & 0x00010000)==0);// Wait until count flag is set
	}
}

