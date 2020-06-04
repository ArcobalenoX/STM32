#include "usart.h"
#include "stdio.h"

#if 1
#pragma import(__use_no_semihosting) 
struct __FILE 
{ 
	int handle; 
}; 
FILE __stdout; 
void _sys_exit(int x) 
{ 
	x = x; 
} 
int fputc(int ch, FILE *f)
{ 
	while((USART1->SR&0X40)==0);
	USART1->DR = (u8) ch; 
	return ch;
}
#endif

//USART1对应的IO引脚 PA9 PA10
void usart1_init(void)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	//打开GPIOA组的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//打开串口1的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	//端口复用
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);

	//GPIO为复用模式
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉电阻
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//Tx
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//推挽
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//Rx
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//配置串口
	USART_InitStructure.USART_BaudRate = 115200;//波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//数据位8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//停止位1位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件流控
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//打开接收和发送
	USART_Init(USART1, &USART_InitStructure);
	
	//中断配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//打开串口1的接收标志位
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	//使能串口1
	USART_Cmd(USART1, ENABLE);
}

uint8_t d;

void USART1_IRQHandler(void)
{	
	
	//判断串口1是否接收到数据
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		//判断缓冲区是否已被处理
		if(usart1_recv_flag==0)
		{
			//读取从串口1读到的字节
			d = USART_ReceiveData(USART1);
		
			//将接收到的数据依次存放到缓冲区中
			recv_buf[cnt]=d;
			cnt++;
			//判断是否接收到\r\n或者超过了缓冲区大小
			if((cnt>=2 && recv_buf[cnt-2] == '\r' && recv_buf[cnt-1] == '\n' )|| cnt == 255)
				usart1_recv_flag=1;			
		}
		/*
		//将读到的字节通过串口1发送出去
		USART_SendData(USART1,d);
		//等待串口1发送数据发送结束
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
		
		//USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		USART1->SR = (uint16_t)0x5f;
		*/
		
	}

}



