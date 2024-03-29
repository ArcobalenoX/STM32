#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "adc.h"
#include "key.h"
#include "exti.h"
#include "pwm.h"
#include "encoder.h"

#define ZHONGZHI 3110  //原版3085

int Encoder,Position_Zero=10000;
float Angle_Balance;
float Balance_KP = 80, Balance_KD = 400,Balance_KI=0,Position_KP=25,Position_KD=600;
int Balance_Pwm,Position_Pwm;
int Moto;

int Balance(float Angle);
int Position(int Encoder);
void Set_Pwm(void);

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
    delay_init(168);    //初始化延时函数
    uart_init(115200);	//初始化串口波特率为115200
    EXTIX_Init();		//PA0 PE2 PE3 PE4按键外部中断初始化
    LED_Init();					//PF9 PF10初始化LED
    LCD_Init();					//LCD初始化
    Adc_Init(); 				//PA5 adc初始化
    motor_init();				//PA9 PA10 电机初始化
    TIM14_PWM_Init(8400-1,0);	//PA7 输出PWM频率为 84M/8400=10KHz.
    Encoder_Init_TIM4();		//PB6 PB7 编码器

    /*LCD显示*/
    POINT_COLOR=RED;
    LCD_ShowString(30,50,200,16,16,"Explorer STM32F4");
    LCD_ShowString(30,70,200,16,16,"PWM ADC ENCODER TEST");
    LCD_ShowString(30,90,200,16,16,"WK_UP:+  KEY1:-");
    LCD_ShowString(30,110,200,16,16,"KEYL:+  KEYR:-");

    POINT_COLOR=BLUE;//设置字体为蓝色
    LCD_ShowString(30,180,200,16,16,"ADC_NUM:");
    LCD_ShowString(30,200,200,16,16,"ENCODER:");

    LCD_ShowString(30,230,200,16,16,"zkb/8400  :");
    LCD_ShowString(30,250,200,16,16,"Balance_KP:");
    LCD_ShowString(30,270,200,16,16,"Balance_KD:");

    while(1)
    {
        Encoder=TIM_GetCounter(TIM4);//得到编码器的值
        Angle_Balance=Get_Adc_Average(ADC_Channel_5,10); //得到ADC转换值

        if(ZHONGZHI-300<Angle_Balance && Angle_Balance<ZHONGZHI+300)
        {
            Balance_Pwm =Balance(Angle_Balance);
//      Position_Pwm = Position(Encoder);
//      Moto = Balance_Pwm - Position_Pwm;

            Moto = Balance_Pwm;

            Set_Pwm();
            //	TIM_SetCompare1(TIM14,8000);
        }
        else
        {
            direction(2);
            TIM_SetCompare1(TIM14,0);

        }
        LCD_ShowNum(120,180,Angle_Balance,5,16);
        //printf("Angle_Balance %f\n\n",Angle_Balance);
        LCD_ShowNum(120,200,Encoder,5,16);
        //printf("Encoder %d\n\n",Encoder);
        LCD_ShowNum(120,230,Moto,5,16);
        //printf("Moto %d\n\n",Moto);
        LCD_ShowNum(120,250,Balance_KP,3,16);
        LCD_ShowNum(120,270,Balance_KD,3,16);

        //  delay_ms(10);
    }


}

/**************************************************************************
函数功能：倾角PD控制
入口参数：角度
返回  值：倾角控制PWM
**************************************************************************/
int Balance(float Angle)
{
    float Bias;                       //倾角偏差
    static float Last_Bias,D_Bias,I_Bias;    //PID相关变量
    int balance;                      //PWM返回值
    Bias=Angle-ZHONGZHI;              //求出平衡的角度中值 和机械相关
    D_Bias=Bias-Last_Bias;            //求出偏差的微分 进行微分控制
	I_Bias+=Bias;
	
    if(Bias<0)
    {
        balance=-300-Balance_KP*Bias-Balance_KD*D_Bias;   //===计算倾角控制的电机PWM  PD控制
    }
    else
    {
        balance=300-Balance_KP*Bias-Balance_KD*D_Bias;   //===计算倾角控制的电机PWM  PD控制
    }
    Last_Bias=Bias;                   //保持上一次的偏差
    return balance;
}


/**************************************************************************
函数功能：位置PD控制
入口参数：编码器
返回  值：位置控制PWM
**************************************************************************/
int Position(int Encoder)
{
    static float Position_PWM,Last_Position,Position_Bias,Position_Differential;
    static float Position_Least;
    Position_Least =Encoder-Position_Zero;             //===
    Position_Bias *=0.8;
    Position_Bias += Position_Least*0.2;	             //===一阶低通滤波器
    Position_Differential=Position_Bias-Last_Position;
    Last_Position=Position_Bias;
    Position_PWM=Position_Bias*Position_KP+Position_Differential*Position_KD; //===速度控制
    return Position_PWM;
}


/**************************************************************************
函数功能：赋值给PWM寄存器
**************************************************************************/
void Set_Pwm()
{

    int Amplitude=8300;    //===PWM满幅是8400 限制在8000
    if(Moto<-Amplitude) Moto=-Amplitude;
    if(Moto>Amplitude)  Moto=Amplitude;

    if(Moto<0)
    {
        direction(0);
        Moto=-Moto;
    }
    else
    {
        direction(1);
    }
    TIM_SetCompare1(TIM14,Moto);
}



