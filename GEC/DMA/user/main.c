/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

int i=0;
int src[10]={0};
int dst[10]={0};
char srcstr[100]="USART_DMAReq_Tx: USART DMA transmit request \r\n";

uint8_t recv_buf[256]={0};//串口接收的缓冲区
uint8_t cnt=0;//缓冲区的下标
uint8_t usart1_recv_flag =0;//判断是否接收完成,未完成==0,完成==1

void usart_deal(void)
{

	//当判断到串口接收字符串以后,进入处理程序
	if(usart1_recv_flag==1 && recv_buf[0]!=0)
	{
		printf("recv:%s",recv_buf);
		
		if(strncmp((char*)recv_buf,"dma",3)==0)
		{
			DMA2S0_cmd();
			printf("dma it\r\n");
			for(i=0;i<10;i++)
			{
			printf("%d\r\n",dst[i]);
			delay_ms(100);
			}
		}

		
		if(strncmp((char*)recv_buf,"tomem",5)==0)
		{
			DMA2_USART_RX();
			delay_ms(100);
			printf("%s\r\n",srcstr);
		}
		
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
	

	usart1_init();




	for(i=0;i<10;i++)
	{
		src[i]=i+1;;	
	}

	
	dma_mem2mem_init((u32)src,(u32)dst);	
	printf("Start\r\n");
	
	for(i=0;i<10;i++)
	{
		printf("%d\r\n",dst[i]);
		delay_ms(100);
	}
	
	
	dma_mem2usart_init((u32)srcstr,(u32)&(USART1->DR));
	printf("memory DMA to usart\r\n");	
	delay_ms(100);
	DMA2_USART_TX();
	
	
	dma_usart2mem_init((u32)&(USART1->DR),(u32)srcstr);
	printf("usart DMA to memory\r\n");	

	while( 1 )
	{
		usart_deal();
		dht11();
		delay_ms(1000);
	}
	
}


