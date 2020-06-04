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
float Balance_KP = 25, Balance_KD = 240,Balance_KI=0,Position_KP=0,Position_KD=0;
int Balance_Pwm,Position_Pwm;
int Moto;
u8 Position_Target;


int Balance(float Angle);
int Position(int Encoder);
void Set_Pwm(void);


int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
    delay_init(168);   			//初始化延时函数
    uart_init(115200);			//初始化串口波特率为115200
    EXTIX_Init();				//PA0 PE2 PE3 PE4按键外部中断初始化
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


    LCD_ShowString(30,250,200,16,16,"Balance_KP:");
    LCD_ShowString(30,270,200,16,16,"Balance_KD:");
    LCD_ShowString(30,290,200,16,16,"Balance_KI:");
    LCD_ShowString(30,310,300,16,16,"Position_KP:");
    LCD_ShowString(30,330,300,16,16,"Position_KD:");
    LCD_ShowString(30,350,300,16,16,"Position_KI:");
    LCD_ShowString(30,370,300,16,16,"Balance_Pwm:");
    LCD_ShowString(30,390,300,16,16,"Position_Pwm:");
    LCD_ShowString(30,410,200,16,16,"10kHz zkb/8400:");
	
	
    while(1)
    {
        Encoder=TIM_GetCounter(TIM4);//得到编码器的值
        Angle_Balance=Get_Adc_Average(ADC_Channel_5,10); //得到ADC转换值
	
        Balance_Pwm = Balance(Angle_Balance);
        if(++Position_Target > 5)	Position_Pwm = Position(Encoder),Position_Target = 0;
        Position_Pwm = Position(Encoder);
		
        if(ZHONGZHI-300<Angle_Balance && Angle_Balance<ZHONGZHI+300)
        {

            Moto = Balance_Pwm - Position_Pwm;
			//Moto = - Position_Pwm;	
			//Moto = Balance_Pwm;
			
            Set_Pwm();

        }
        else
        {
            direction(2);
            
        }

        LCD_ShowNum(150,180,Angle_Balance,5,16);   
        LCD_ShowNum(150,200,Encoder,5,16);     
       
        LCD_ShowNum(150,250,Balance_KP,3,16);
        LCD_ShowNum(150,270,Balance_KD,3,16);
        LCD_ShowNum(150,290,Balance_KI,3,16);
        LCD_ShowNum(150,310,Position_KP,3,16);
        LCD_ShowNum(150,330,Position_KD,3,16);
//      LCD_ShowNum(150,350,Position_KI,3,16);
		if(Balance_Pwm<0)
			LCD_ShowNum(150,370,-Balance_Pwm,10,16);
		else 
			LCD_ShowNum(150,370,Balance_Pwm,10,16);
		
		if(Position_Pwm<0)
			LCD_ShowNum(150,390,-Position_Pwm,10,16);
		else
			LCD_ShowNum(150,390,Position_Pwm,10,16);
		
		LCD_ShowNum(150,410,Moto,10,16);
  
    }
}

/**************************************************************************
函数功能：倾角PID控制
入口参数：角度
返回  值：倾角控制PWM
**************************************************************************/
int Balance(float Angle)
{
    float Bias;                       //倾角偏差
    static float Last_Bias,D_Bias,I_Bias;
    float I_sum;    //PID相关变量
    int balance;                      //PWM返回值
    Bias=Angle-ZHONGZHI;              //求出平衡的角度中值 和机械相关
    D_Bias=Bias-Last_Bias;            //求出偏差的微分 进行微分控制
    I_Bias+=Bias;				   	  //偏差积分

    if(Bias<0)
    {
        balance=-300-Balance_KP*Bias-Balance_KD*D_Bias-0.05*Balance_KI*I_Bias;   //===计算倾角控制的电机PWM  PD控制
    }
    else
    {
        balance=300-Balance_KP*Bias-Balance_KD*D_Bias-0.05*Balance_KI*I_Bias;   //===计算倾角控制的电机PWM  PD控制
    }
	
   // balance=-Balance_KP*Bias-Balance_KD*D_Bias-0.05*Balance_KI*I_Bias;

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
    Position_Least =(Encoder-Position_Zero);             //===最近位置差值
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

    int Amplitude=8100;    //===PWM满幅是8400 限制在8100
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



