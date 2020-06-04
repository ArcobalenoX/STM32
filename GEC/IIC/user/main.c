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
		printf("recv:  %s",recv_buf);
		
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
		
		if(strncmp((char*)recv_buf,"pwset:",6)==0)
		{
			
			printf("set password\r\n");
			for (i=0;i<4;i++)
			{
			
				ic_write_byte(i,recv_buf[6+i]);	
				pw[i]=recv_buf[6+i];
			}
			ic_write_byte(4,'Y');	
			
			printf("pwset over\r\n");
		}
		
		if(strncmp((char*)recv_buf,"pw?",3)==0)
		{
			
			u8 data;
			printf("pw is ");
			for (i=0;i<4;i++)
			{
				data=ic_read_byte(i);
				printf("%c",data);			
			}
			printf("\r\n");					
		}
		if(strncmp((char*)recv_buf,"time?",5)==0)
		{
			RTC_TimeTypeDef	RTC_TimeStruct;
			
			RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct);
			printf("%d h %d m %d s \r\n",	RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes,RTC_TimeStruct.RTC_Seconds);
		}
				if(strncmp((char*)recv_buf,"date?",5)==0)
		{
			RTC_DateTypeDef	RTC_DateStruct;
			
			RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
			printf("20%d Y %d M %d D \r\n",	RTC_DateStruct.RTC_Year,RTC_DateStruct.RTC_Month,RTC_DateStruct.RTC_Date);
		

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
	
	//串口1初始化
	usart1_init();
	//IIC--24C02初始化
	IICIO_init();

	RTC_init();
	Alarm_init();
/*
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
*/

	printf("Start\r\n");
	while( 1 )
	{
		usart_deal();
			
	}
	
}


