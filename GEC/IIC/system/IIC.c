#include "IIC.h"

#define SDA_OUT  {GPIOB->MODER &=~(3<<18);GPIOB->MODER |=(1<<18);}
#define SDA_IN      (GPIOB->MODER &=~(3<<18));

#define SCL(x) x?(PBOut(8)=1):(PBOut(8)=0)
#define SDA(x) x?(PBOut(9)=1):(PBOut(9)=0)
#define SDA_VAL PBIn(9)

void IICIO_init(void)
{
	//1. 配置启动 IO口时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin 	= GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode 	= GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_OD;
	GPIO_InitStruct.GPIO_PuPd 	= GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed 	= GPIO_Speed_100MHz;
	GPIO_Init( GPIOB, &GPIO_InitStruct );
}



void start(void)
{
		
	SDA_OUT;
	SCL(0);delay_us(4);
	SDA(1);delay_us(4);
	SCL(1);delay_us(4);
	SDA(0);delay_us(4);
	//释放总线
	SCL(0);delay_us(4);
	SDA(1);delay_us(4);
}


void stop(void)
{
	SDA_OUT;
	SCL(0);delay_us(4);
	SDA(0);delay_us(4);
	SCL(1);delay_us(4);
	SDA(1);delay_us(4);
	SCL(0);delay_us(4);
}

char wait_ack(void)
{
	int rtim=0;
	SCL(0);delay_us(4);
	SDA(1);delay_us(4);
	SDA_IN;//易错点，不要放在下一步后，要在之前释放 SDA
	SCL(1);delay_us(4);
	while(SDA_VAL){
	if(++rtim>250){
	stop(); 
	return 1;
	} }
	SCL(0);delay_us(4);
	return 0;
}

void send_ack(void)
{
	SDA_OUT;
	SCL(0);delay_us(4);
	SDA(0);delay_us(4);
	SCL(1);delay_us(4);
}
void send_no_ack(void)
{
	SDA_OUT;
	SCL(0);delay_us(4);
	SDA(1);delay_us(4);
	SCL(1);delay_us(4);
}

void send_byte(u8 txd)
{
	char i=8;
	SDA_OUT;
	while(i--)
	{
		SCL(0);delay_us(4);
		if((txd&0x80)>>7 == 1)SDA(1);
		else SDA(0);
		delay_us(4);
		SCL(1);delay_us(4);
		txd = txd<<1;
	} 
}

u8 read_byte(u8 ack)
{
	unsigned char rx=0;
	char i=8;
	SDA_IN;
	while(i--){
	SCL(0);delay_us(4);
	SCL(1);delay_us(2);
	rx = rx<<1; //易错点，最后一次 rx++后不需移位
	if(SDA_VAL==1)rx++;
	delay_us(2);
	}
	if(ack==1)send_ack();
	else send_no_ack();
	return rx;
}
