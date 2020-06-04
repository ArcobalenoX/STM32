#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "adc.h"
#include "key.h"
#include "timer.h"
#include "pwm.h"
#include "encoder.h"


int Encoder_Left;
int circle_count;

int main(void)
{

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
    delay_init(168);    //��ʼ����ʱ����
    uart_init(115200);	//��ʼ�����ڲ�����Ϊ115200

    LCD_Init();					//LCD��ʼ��


    /*��������ʼ��*/
    Encoder_Init_TIM4();
    TIM3_Int_Init(1000-1,8400-1);

    /*LCD��ʾ*/
    POINT_COLOR=RED;
    LCD_ShowString(30,50,200,16,16,"Explorer STM32F4");
    LCD_ShowString(30,70,200,16,16,"ENCODER TEST");

    POINT_COLOR=BLUE;//��������Ϊ��ɫ

    LCD_ShowString(30,200,200,16,16,"TIM4  :");
    LCD_ShowString(30,220,200,16,16,"TIM3  :");
    LCD_ShowString(30,240,200,16,16,"circle_count  :");
    LCD_ShowString(30,260,200,16,16,"encoder  :");
//	Encoder_Left=TIM_GetCounter(TIM2)+3600*circle_count;//��ǰ�Ƕ�
    while(1)
    {

        LCD_ShowxNum(100,200,TIM_GetCounter(TIM4),16,16,0);
        LCD_ShowxNum(100,220,TIM_GetCounter(TIM3),16,16,0);
        LCD_ShowxNum(100,240,circle_count,16,16,0);
        LCD_ShowxNum(100,260,Encoder_Left,16,16,0);
		delay_ms(10);

    }
}




