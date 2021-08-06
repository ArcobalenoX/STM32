#include "relay.h"

void RELAY_Init(void)
{

    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输入模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOE2,3,4

}

void RELAY_ON(void)
{
	PAout(6)=1;
	PAout(7)=0;
}


void RELAY_OFF(void)
{
	PAout(6)=0;
	PAout(7)=0;
}