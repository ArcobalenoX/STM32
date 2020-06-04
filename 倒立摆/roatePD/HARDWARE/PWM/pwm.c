#include "pwm.h"
#include "led.h"
#include "usart.h"

//TIM9 PWM���ֳ�ʼ��
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM14_PWM_Init(u32 arr,u32 psc)
{
    //�˲������ֶ��޸�IO������

    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);  	//TIM14ʱ��ʹ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//ʹ��PORTFʱ��

    GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM14); //GPIOF9����Ϊ��ʱ��14

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;           //GPIOF9
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
    GPIO_Init(GPIOA,&GPIO_InitStructure);              //��ʼ��PF9

    TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
    TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;

    TIM_TimeBaseInit(TIM14,&TIM_TimeBaseStructure);//��ʼ����ʱ��14

    //��ʼ��TIM14 Channel1 PWMģʽ
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
    TIM_OC1Init(TIM14, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 4OC1

    TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);  //ʹ��TIM14��CCR1�ϵ�Ԥװ�ؼĴ���

    TIM_ARRPreloadConfig(TIM14,ENABLE);//ARPEʹ��

    TIM_Cmd(TIM14, ENABLE);  //ʹ��TIM14


}

void motor_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12; //298���������Ӧ����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA
}

void direction(char dir)
{
	if(dir==0)
	{
		PAout(11)=1;
		PAout(12)=0;
	}
	else if(dir==1)
	{
		PAout(11)=0;
		PAout(12)=1;
	}
	else if(dir==2)
	{
		PAout(11)=0;
		PAout(12)=0;
	}
	
	
	
}

