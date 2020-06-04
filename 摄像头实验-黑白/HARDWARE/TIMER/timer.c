#include "timer.h"
#include "led.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/4
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 


extern u8 X_MAX,Y_MAX;    //С���������Ϣ
extern u8 X_MAX_LSAT, X_MIN_LSAT, Y_MAX_LSAT, Y_MIN_LSAT;   //��һ��С������λ����Ϣ
extern u8 X,Y;      //С���������Ϣ
extern u8 X_MIN,Y_MIN;
extern int PWM_X,PWM_Y;          //pid������ʼ��
extern float Err_X,Err_Y;
extern float Err_X_LAST,Err_Y_LAST;
extern float Aim_X,Aim_Y;
extern float Kp,Ki,Kd;


//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///ʹ��TIM3ʱ��
	
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_Period=arr;   //�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}


void TIM3_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
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

        TIM_SetCompare1(TIM14,PWM_X);        //�޸ıȽ�ֵ���޸�ռ�ձ�
        TIM_SetCompare1(TIM11,PWM_Y);        //�޸ıȽ�ֵ���޸�ռ�ձ�
		
		printf("X:%d Y:%d\r\n",X,Y);
    }
    TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
}


