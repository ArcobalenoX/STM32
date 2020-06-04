#include "timer.h"
#include "led.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//定时器 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/4
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 


extern u8 X_MAX,Y_MAX;    //小球的坐标信息
extern u8 X_MAX_LSAT, X_MIN_LSAT, Y_MAX_LSAT, Y_MIN_LSAT;   //上一次小球坐标位置信息
extern u8 X,Y;      //小球的质心信息
extern u8 X_MIN,Y_MIN;
extern int PWM_X,PWM_Y;          //pid参数初始化
extern float Err_X,Err_Y;
extern float Err_X_LAST,Err_Y_LAST;
extern float Aim_X,Aim_Y;
extern float Kp,Ki,Kd;


//通用定时器3中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟
	
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM3,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}


void TIM3_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
    {

        Kp=3.4;
        Kd=80.9;

        Aim_X=72;
        Aim_Y=88;

        Err_X=X-Aim_X;
        Err_Y=Y-Aim_Y;


        PWM_X=9340+(Err_X*Kp+(Err_X-Err_X_LAST)*Kd);
        PWM_Y=9300+(Err_Y*Kp+(Err_Y-Err_Y_LAST)*Kd);

        if(PWM_Y>9370)PWM_Y=9370;
        if(PWM_Y<9230)PWM_Y=9230;

        if(PWM_X>9410)PWM_X=9410;
        if(PWM_X<9270)PWM_X=9270;

        Err_X_LAST=Err_X;
        Err_Y_LAST=Err_Y;

        TIM_SetCompare1(TIM14,PWM_X);        //修改比较值，修改占空比
        TIM_SetCompare1(TIM11,PWM_Y);        //修改比较值，修改占空比
		
		printf("X:%d Y:%d\r\n",X,Y);
    }
    TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
}


