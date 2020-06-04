#include "time.h"


//定时器初始化
void TIM5_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  ///使能TIM5时钟

    TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
    TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);//初始化TIM5

    TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE); //允许定时器5更新中断
    TIM_Cmd(TIM5,ENABLE); //使能定时器5

    NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn; //定时器5中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}
//定时器5中断服务函数
void TIM5_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM5,TIM_IT_Update)==SET) //溢出中断
    {
		 PEOut(14) =  ~PEOut(14); 
       
    }
    TIM_ClearITPendingBit(TIM5,TIM_IT_Update);  //清除中断标志位
}


//定时器初始化
void TIM2_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  ///使能TIM时钟

    TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
    TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//初始化TIM

    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //允许定时器更新中断
    TIM_Cmd(TIM2,ENABLE); //使能定时器

    NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; //定时器中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}
//定时器中断服务函数
void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //溢出中断
    {
        PEOut(13) =  ~PEOut(13); 
    }
    TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //清除中断标志位
}

//定时器初始化
void TIM10_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10,ENABLE);  ///使能TIM时钟

    TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
    TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM10,&TIM_TimeBaseInitStructure);//初始化TIM

    TIM_ITConfig(TIM10,TIM_IT_Update,ENABLE); //允许定时器更新中断
    TIM_Cmd(TIM10,ENABLE); //使能定时器

    NVIC_InitStructure.NVIC_IRQChannel=TIM1_UP_TIM10_IRQn; //定时器中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}
//定时器中断服务函数
void TIM1_UP_TIM10_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM10,TIM_IT_Update)==SET) //溢出中断
    {
        PFOut(10) ^=  1; 
    }
    TIM_ClearITPendingBit(TIM10,TIM_IT_Update);  //清除中断标志位
}



//定时器初始化
void TIM14_Init(u16 arr,u16 psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);  	//TIM4时钟使能
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); 	//使能PORTF时钟

    GPIO_PinAFConfig(GPIOF,GPIO_PinSource9,GPIO_AF_TIM14); //GPIOF9复用为定时器14

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;           //GPIOF9
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
    GPIO_Init(GPIOF,&GPIO_InitStructure);              //初始化PF9

    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式	
    TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
    TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM14,&TIM_TimeBaseStructure);//初始化定时器14

    //初始化TIM4 Channel1 PWM模式
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性低
	TIM_OCInitStructure.TIM_Pulse=5000;
    TIM_OC1Init(TIM14, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1
    TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);  //使能TIM4在CCR1上的预装载寄存器

    TIM_ARRPreloadConfig(TIM14,ENABLE);//ARPE使能

    TIM_Cmd(TIM14, ENABLE);  //使能TIM4

}




