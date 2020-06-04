#include "stm32f4xx.h"
#include"stdio.h"
static GPIO_InitTypeDef 	GPIO_InitStruct;
static EXTI_InitTypeDef   EXTI_InitStructure;
static NVIC_InitTypeDef   NVIC_InitStructure;
static USART_InitTypeDef USART_InitStructure;

int m = 0;
int n = 0;

static int key1_flag=0;
static int key2_flag=0;
static int key3_flag=0;
static int key4_flag=0;


#define led0(n) n?GPIO_ResetBits(GPIOF,GPIO_Pin_9):GPIO_SetBits(GPIOF,GPIO_Pin_9)
#define led1(n) n?GPIO_ResetBits(GPIOF,GPIO_Pin_10):GPIO_SetBits(GPIOF,GPIO_Pin_10)
#define led2(n) n?GPIO_ResetBits(GPIOE,GPIO_Pin_13):GPIO_SetBits(GPIOE,GPIO_Pin_13)
#define led3(n) n?GPIO_ResetBits(GPIOE,GPIO_Pin_14):GPIO_SetBits(GPIOE,GPIO_Pin_14)
#define beep(n) n?GPIO_SetBits(GPIOF,GPIO_Pin_8):GPIO_ResetBits(GPIOF,GPIO_Pin_8)


void delay_us(int n)
{
	SysTick->CTRL = 0; 	
	SysTick->LOAD = n*168 - 1; 
	SysTick->VAL = 0; 									
	SysTick->CTRL = 5; 								
	while ((SysTick->CTRL & 0x00010000)==0);					
	SysTick->CTRL = 0; 								
}

void delay_ms(u32 n)
{

	while(n--)
		delay_us(1000);
}
int fputc(int ch,FILE *f)
{
	USART_SendData(USART1,ch);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);	
	return ch;
}



//超声波
void sr04_init(void)
{


	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;			
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;			
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;			
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;		
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;		
	GPIO_Init(GPIOB, &GPIO_InitStruct);	
	
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;				
	GPIO_Init(GPIOE, &GPIO_InitStruct);	

	GPIO_ResetBits(GPIOB, GPIO_Pin_6);
}
uint32_t sr04_get_distance(void)
{
	uint32_t t=0;
	
	GPIO_SetBits(GPIOB, GPIO_Pin_6);
	delay_us(20);
	GPIO_ResetBits(GPIOB, GPIO_Pin_6);

	while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6)==0);
	
	while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6))
	{
	
		delay_us(8);
		
		t++;
	}
	t = t/2;

	return t*3;
	
	
}
void sr04(void)
{
	uint32_t distance = sr04_get_distance();		
	
	if(distance>=20 && distance<4000)
			{	
				printf("distance=%dmm\r\n",distance);				
				if(distance>=20 && distance<50)
				{
					beep(1);delay_ms(100);beep(0);		delay_ms(100);												
				}
				else if(distance>=50 && distance<500)
				{ 
					beep(1);delay_ms(300);	beep(0);			delay_ms(300);	
				}	
				else if(distance>=500 && distance<1000)
				{
					beep(1);delay_ms(500);	beep(0);			delay_ms(500);
				}	
				else if(distance>=1000)
				{ 
					beep(1);delay_ms(1000);	beep(0);			delay_ms(1000);
				}
			}

			
			distance = 0;

		}


//温湿度
void dht11_output_mode(void)
{
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;				
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;		
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;			
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;		
	GPIO_InitStruct.GPIO_PuPd =  GPIO_PuPd_UP;			
	GPIO_Init(GPIOG, &GPIO_InitStruct);	
}
void dht11_input_mode(void)
{
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;				
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;		
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;		
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;		
	GPIO_Init(GPIOG, &GPIO_InitStruct);
}

uint32_t dht11_start(void)
{
	uint32_t t=0;

	dht11_output_mode();

	GPIO_ResetBits(GPIOG, GPIO_Pin_9);	
	delay_ms(20);

	GPIO_SetBits(GPIOG, GPIO_Pin_9);
	delay_us(30);	

	dht11_input_mode();	

	t=0;
	while(t<1000)
	{
		if(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9)==0)
			break;
		t++;
		delay_us(1);	
	}
	

	if(t>=1000)
		return 1;

	while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9)==0);

	while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9));	

	return 0;
}
uint8_t dht11_read_byte(void)
{
	uint8_t d=0;
	uint32_t i=0;
	for(i=0; i<8; i++)
	{
	
		while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9)==0);	
	
		delay_us(40);	
		
		if(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9))
		{
			d|=1<<(7-i);
			while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9));
		}
	}

	return d;
}
uint32_t dht11_read_data(uint8_t *pbuf)
{	
	uint32_t i=0;
	uint8_t check_sum=0;

	
	while(dht11_start()==1);

	
	for(i=0; i<5; i++)
	{
		pbuf[i]=dht11_read_byte();

	}

	delay_us(100);
	

	check_sum=pbuf[0]+pbuf[1]+pbuf[2]+pbuf[3];
	
	
	if(check_sum!=pbuf[4])
		return 1;
	
	return 0;

}



void dht11(void){
	uint8_t dht11_data[5]={0};
	if(dht11_read_data(dht11_data)==0)
	{
		if(dht11_data[2]>0&&dht11_data[2]<50)
		{
		printf("temp=%d.%d\r\n",dht11_data[2],dht11_data[3]);							
		}
	}
		
	if(dht11_data[0]>20&&dht11_data[0]<90)
	{
		printf("humi=%d.%d\r\n",dht11_data[0],dht11_data[1]);
		if(dht11_data[0]<60)
		{
			led0(1);
		}
		else if(dht11_data[0]>60)
		{
			led0(1);
			led1(1);						
			if(dht11_data[0]>70)
			{								
			led0(1);
			led1(1);
			led2(1);								
			}
			if(dht11_data[0]>80)
			{							
			led0(1);
			led1(1);
			led2(1);
			led3(1);
			delay_ms(500);
			led0(0);
			led1(0);
			led2(0);
			led3(0);
			}							
		}			
	}
				
			
}
//配置LED灯连接的引脚参数PE13,PE14,PF9,PF10
void led_init(void)
{	
	//led1,led2
	GPIO_InitStruct.GPIO_Mode =  GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz ;	
	GPIO_Init(GPIOF, &GPIO_InitStruct);
	//led3,led4
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	//初始化完后让灯熄灭
	led0(0);
	led1(0);
	led2(0);
	led3(0);

}
//配置按键引脚,PA0,PE2,PE3,PE4,
void key_init(void)
{
	//key1
	GPIO_InitStruct.GPIO_Mode =  GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	//key2,3,4
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	GPIO_Init(GPIOE, &GPIO_InitStruct);
}
//beep---PF8--PP,OUT,NOPULL
void beep_init(void)
{
	GPIO_InitStruct.GPIO_Mode =  GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz ;	
	GPIO_Init(GPIOF, &GPIO_InitStruct);
	//beep not working
	beep(0);
	
}

void usart_init(int baud){
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); 						
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); 	
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; 	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOA,&GPIO_InitStruct); 		
	
	USART_InitStructure.USART_BaudRate = baud;	
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	
	USART_InitStructure.USART_Parity = USART_Parity_No;	
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure); 	
	USART_Cmd(USART1, ENABLE);  

	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;		
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
	NVIC_Init(&NVIC_InitStructure);		

	USART_Cmd(USART1, ENABLE);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
}




void usart1_send_bytes(uint8_t *pbuf,uint32_t len)
{
	while(len--)
	{
		USART_SendData(USART1,*pbuf++);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	}
}


void usart1_send_str(char *pbuf)
{
	while(pbuf && *pbuf)
	{
		USART_SendData(USART1,*pbuf++);
		
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	}
}

void USART1_IRQHandler(void)
{	

	if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET)
	{
		
		int d=USART_ReceiveData(USART1);
		
		
		
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		
		
	}
}





//配置4个按键的中断
void key_exti(void)
{
	
	//将4个按键连接的引脚连接到对应的外部中断控制线
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);
	//配置参数
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;					
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; 		
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;					
	EXTI_Init(&EXTI_InitStructure);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line2|EXTI_Line3|EXTI_Line4;
	EXTI_Init(&EXTI_InitStructure);	
	//配置4个按键的中断优先级
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;			
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//响应优先级
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;				
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;				
	NVIC_Init(&NVIC_InitStructure);

}
//外部事件0，2，3，4中断服务函数，这里是对应4个按键的中断
void EXTI0_IRQHandler(void)
{
	static int i=0;
	  if(EXTI_GetITStatus(EXTI_Line0) == SET)
	  {
			if(i==0)
			{
					i=1;
					key1_flag=1;
			}
		else if(i==1)
			{
					i=0;
					key1_flag=0;
			}
			
				
			EXTI_ClearITPendingBit(EXTI_Line0);
	  }
}
void EXTI2_IRQHandler(void)
{
	static int i=0;
	  if(EXTI_GetITStatus(EXTI_Line2) == SET)
	  {
			if(i==0)
			{
					i=1;
					key2_flag=1;
			}
		else if(i==1)
			{
					i=0;
					key2_flag	=0;
			}
			
				
			EXTI_ClearITPendingBit(EXTI_Line2);
	  }
}
void EXTI3_IRQHandler(void)
{
	static int i=0;
	  if(EXTI_GetITStatus(EXTI_Line3) == SET)
	  {
			if(i==0)
			{
					i=1;
					key3_flag=1;
			}
		else if(i==1)
			{
					i=0;
					key3_flag	=0;
			}
				
			EXTI_ClearITPendingBit(EXTI_Line3);
	  }
}
void EXTI4_IRQHandler(void)
{		
		static int i=0;
		if(EXTI_GetITStatus(EXTI_Line4) == SET)
	  {		
			if(i==0)
			{
					i=1;
					key4_flag=1;
			}
		else if(i==1)
			{
					i=0;
					key4_flag	=0;
			}
					
	 }
	   		
		EXTI_ClearITPendingBit(EXTI_Line4);
}

	


int main(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF ,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);	
	//启动系统配置控制器（即打开中断）
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	//使能串口1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);	
	//设置优先级分组为2组（只能设置一次）
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//各个硬件初始化
	led_init();
	key_init();
	beep_init();
	key_exti();
	usart_init(115200);
	sr04_init();
	usart1_send_str("arcobaleno\r\n");
while(1)
{
	if(key4_flag==1)		
	{
		if(key3_flag==1)		
		{	
			if(key2_flag==1)
			{
			sr04();
			}
			else if(key1_flag==1)
			{
			dht11();
			}
			else
			{
					beep(0);led0(0);led1(0);led2(0);led3(0);	
			}		
		}
		
	}
	else
	{
		beep(0);led0(0);led1(0);led2(0);led3(0);	
	}
	
	
}



}



