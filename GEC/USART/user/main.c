/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include "stdio.h"
#include "string.h"

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
 
uint8_t d;
uint8_t recv_buf[256]={0};//串口接收的缓冲区
uint8_t cnt=0;//缓冲区的下标
uint8_t usart1_recv_flag =0;//判断是否接收完成,未完成==0,完成==1


int main(void)
{
	
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	led_beep_init();
	key_init();
	usart1_init();			
	printf("Start\r\n");
	
	while(1)
	{
	/*	switch(d)
		{
			case '1': PFout(9) ^=1;break;
			case '2': PFout(10)^=1;break;
			case '3': PEout(13)^=1;break;
			case '4': PEout(14)^=1;break;			
			case '5': PFout(9)  =1;break;
			case '6': PFout(10) =1;break;
			case '7': PEout(13) =1;break;
			case '8': PEout(14) =1;break;						
			default:break;
		}
	*/
		
		
		//当判断到串口接收字符串以后,进入处理程序
		if(usart1_recv_flag==1 && recv_buf[0]!=0)
		{
			printf("recv:%s",recv_buf);
			
			
			if(strstr((char * )recv_buf,"led0on")!= NULL)
				PFout(9)=0;
			else if(strstr((char * )recv_buf,"led0off")!= NULL)
				PFout(9)=1;
			else if(strstr((char * )recv_buf,"led0obv")!= NULL)
				PFout(9)^=1;
							
			//清空recv_buf
			memset((void*)recv_buf,0,sizeof recv_buf);			
			//将缓冲区下标的数字置0
			cnt=0;
			//处理完毕后,将接收完成的标志位置0
			usart1_recv_flag=0;
		}
		
	}
	
}


