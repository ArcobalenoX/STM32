/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

uint8_t recv_buf[256]={0};//串口接收的缓冲区
uint8_t cnt=0;//缓冲区的下标
uint8_t usart1_recv_flag =0;//判断是否接收完成,未完成==0,完成==1

void usart_deal(void)
{

	//当判断到串口接收字符串以后,进入处理程序
	if(usart1_recv_flag==1 && recv_buf[0]!=0)
	{
		printf("recv:%s",recv_buf);
		
		//清空recv_buf
		memset((void*)recv_buf,0,sizeof recv_buf);			
		//将缓冲区下标的数字置0
		cnt=0;
		//处理完毕后,将接收完成的标志位置0
		usart1_recv_flag=0;
	}
		
}



//32位随机数
void rng_init(void)
{
	RCC->AHB2ENR |= RCC_AHB2Periph_RNG;
	RNG->CR |= RNG_CR_RNGEN;
}

uint32_t getrng(void)
{
	while((RNG->SR & RNG_FLAG_DRDY) == RESET);
	return RNG->DR;
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	//延时 初始化
	delay_init();
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
	
	rng_init();
	iwdg_init();
	
	usart1_init();
	F7_adc_init();	
	A5_adc_init();	
	
	/*//PWM
	TIM2_Init(2000-1,8400-1);	 //	(84M/arr/psc)5HZ--200MS
	TIM5_Init(10000-1,8400-1);   //	(84M/arr/psc)=1HZ--1S
	TIM10_Init(50000-1,16800-1); // (168M/arr/psc) = 0.2HZ--5S
	TIM14_Init(100,8400-1);
	TIM_SetCompare1(TIM14,50);
	*/

	int val;
	printf("Start\r\n");
	while( 1 )
	{
		usart_deal();
		
		//光敏电阻
		val=F7ADC();
		printf("F7:%d    VOL: %f %%\r\n",val,100-val/24.0);
		
		//变阻
		val=A5ADC();
		printf("A5:%d    VOL: %f V\r\n",val,val*3.3/4096);
	
		//随机数
		printf("rng:%u\r\n",getrng()%100);
		
		IWDG_ReloadCounter();
		delay_ms(2000);
			
	}
	
}


