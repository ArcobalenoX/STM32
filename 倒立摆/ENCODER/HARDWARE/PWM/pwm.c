#include "pwm.h"
#include "led.h"
#include "usart.h"

//TIM4 PWM部分初始化
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM4_PWM_Init(u32 arr,u32 psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	//TIM2时钟使能
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//使能PORTA时钟

    GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4); //GPIOB6复用为定时器4

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;           //GPIOB6
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
    GPIO_Init(GPIOB,&GPIO_InitStructure);              //初始化PA2

    TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
    TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;//不时钟分割
    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);//初始化定时器2

    //初始化TIM4 Channel1 PWM模式
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
    TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据TIM_OCx指定的参数初始化外设TIM1通道1

    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM2在CCR3上的预装载寄存器

    TIM_ARRPreloadConfig(TIM4,ENABLE);//ARPE使能

    TIM_Cmd(TIM4, ENABLE);  //使能TIM4
}

void motor_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA,GPIOE时钟

	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10; //298电机正负对应引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA
}

void direction(char dir)
{
	if(dir==0)
	{
		PAout(9)=1;
		PAout(10)=0;
	}
	else if(dir==1)
	{
		PAout(9)=0;
		PAout(10)=1;
	}
	
	
}

