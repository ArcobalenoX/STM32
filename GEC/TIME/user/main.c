/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define	   _REG_LED

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/



/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	
	//配置优先级分组
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_2 );
	
	//led 初始化
	led_beep_init();

	//key 初始化
	key_init();
	
	//检测密码
	//PassWord();
	
	//中断初始化
	key_exti_init();
	
	
	TIM2_Init(2000-1,8400-1);	 //	(84M/arr/psc)5HZ--200MS
	TIM5_Init(10000-1,8400-1);   //	(84M/arr/psc)=1HZ--1S
	TIM10_Init(50000-1,16800-1); // (168M/arr/psc) = 0.2HZ--5S
	TIM14_Init(100,8400-1);
	TIM_SetCompare1(TIM14,50);
	static int i=0;

	while( 1 )
	{
		//PFOut(9) ^= 1; delay_ms(500);
		for(i=0;i<100;i+=10)
		{
			TIM_SetCompare1(TIM14,i);
			delay_ms(50);
		}
		for(i=100;i>0;i-=10)
		{
			TIM_SetCompare1(TIM14,i);
			delay_ms(50);
		}			
			
	}
	
}


