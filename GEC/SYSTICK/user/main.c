/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

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
	
	delay_init();// delay  初始化
	led_beep_init();
	key_init();
	
	while(1)
	{
		PFout(9)=1;	
		delay_nms(1000);
		PFout(9)=0;
		delay_nms(1000);	
		if(PEin(2)==0)
		{
			PFout(10)=0;
		}
		else
		{
			PFout(10)=1;
		}
		if(PEin(3)==1)
		{
			PEout(13)=0;
		}
		else
		{
			PEout(13)=1;
		
		}
			
		/*
		led2(0);	
		delay_nus(2000);
		led2(1);
		delay_nus(1000);
		*/	
	}
	

}


