#include "DHT11.h"



//ÎÂÊª¶È
void dht11_output_mode(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);	
	GPIO_InitTypeDef 	GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;				
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;		
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;			
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;		
	GPIO_InitStruct.GPIO_PuPd =  GPIO_PuPd_UP;			
	GPIO_Init(GPIOG, &GPIO_InitStruct);	
	
	//GPIOG->MODER &= ~(3<<18);
	//GPIOG->MODER |= (1<<18);	
}

void dht11_input_mode(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);	
	GPIO_InitTypeDef 	GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;				
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;		
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;		
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;		
	GPIO_Init(GPIOG, &GPIO_InitStruct);
	
	//GPIOG->MODER &= ~(3<<18);
	
}

uint32_t dht11_start(void)
{
	uint32_t t=0;

	dht11_output_mode();

	PGOut(9)=0;	
	delay_ms(20);
	PGOut(9)=1;	
	delay_us(30);	

	dht11_input_mode();	

	t=0;
	while(t<1000)
	{
		if(PGIn(9)==0)
			break;
		t++;
		delay_us(1);	
	}
	

	if(t>=1000)
		return 1;

	while(PGIn(9)==0);

	while(PGIn(9));	

	return 0;
}
uint8_t dht11_read_byte(void)
{
	uint8_t d=0;
	uint32_t i=0;
	for(i=0; i<8; i++)
	{
	
		while(PGIn(9)==0);	
	
		delay_us(40);	
		
		if(PGIn(9))
		{
			d|=1<<(7-i);
			while(PGIn(9));
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



void dht11(void)
{
	uint8_t dht11_data[5]={0};
	if(dht11_read_data(dht11_data)==0)
	{
		if(dht11_data[2]>0&&dht11_data[2]<50)
		{
			printf("temp=%d.%d ¡æ\r\n",dht11_data[2],dht11_data[3]);							
		}
		if(dht11_data[0]>0&&dht11_data[0]<100)
		{
			printf("humd=%d.%d %%\r\n",dht11_data[0],dht11_data[1]);							
		}
		
			
	}
		
}



