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

#define ZHONGZHI 3110  //ԭ��3085

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
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
    delay_init(168);   			//��ʼ����ʱ����
    uart_init(115200);			//��ʼ�����ڲ�����Ϊ115200
    EXTIX_Init();				//PA0 PE2 PE3 PE4�����ⲿ�жϳ�ʼ��
    LED_Init();					//PF9 PF10��ʼ��LED
    LCD_Init();					//LCD��ʼ��
    Adc_Init(); 				//PA5 adc��ʼ��
    motor_init();				//PA9 PA10 �����ʼ��
    TIM14_PWM_Init(8400-1,0);	//PA7 ���PWMƵ��Ϊ 84M/8400=10KHz.
    Encoder_Init_TIM4();		//PB6 PB7 ������

    /*LCD��ʾ*/
    POINT_COLOR=RED;
    LCD_ShowString(30,50,200,16,16,"Explorer STM32F4");
    LCD_ShowString(30,70,200,16,16,"PWM ADC ENCODER TEST");
    LCD_ShowString(30,90,200,16,16,"WK_UP:+  KEY1:-");
    LCD_ShowString(30,110,200,16,16,"KEYL:+  KEYR:-");

    POINT_COLOR=BLUE;//��������Ϊ��ɫ
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
        Encoder=TIM_GetCounter(TIM4);//�õ���������ֵ
        Angle_Balance=Get_Adc_Average(ADC_Channel_5,10); //�õ�ADCת��ֵ
	
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
�������ܣ����PID����
��ڲ������Ƕ�
����  ֵ����ǿ���PWM
**************************************************************************/
int Balance(float Angle)
{
    float Bias;                       //���ƫ��
    static float Last_Bias,D_Bias,I_Bias;
    float I_sum;    //PID��ر���
    int balance;                      //PWM����ֵ
    Bias=Angle-ZHONGZHI;              //���ƽ��ĽǶ���ֵ �ͻ�е���
    D_Bias=Bias-Last_Bias;            //���ƫ���΢�� ����΢�ֿ���
    I_Bias+=Bias;				   	  //ƫ�����

    if(Bias<0)
    {
        balance=-300-Balance_KP*Bias-Balance_KD*D_Bias-0.05*Balance_KI*I_Bias;   //===������ǿ��Ƶĵ��PWM  PD����
    }
    else
    {
        balance=300-Balance_KP*Bias-Balance_KD*D_Bias-0.05*Balance_KI*I_Bias;   //===������ǿ��Ƶĵ��PWM  PD����
    }
	
   // balance=-Balance_KP*Bias-Balance_KD*D_Bias-0.05*Balance_KI*I_Bias;

    Last_Bias=Bias;                   //������һ�ε�ƫ��
    return balance;
}


/**************************************************************************
�������ܣ�λ��PD����
��ڲ�����������
����  ֵ��λ�ÿ���PWM
**************************************************************************/
int Position(int Encoder)
{
    static float Position_PWM,Last_Position,Position_Bias,Position_Differential;
    static float Position_Least;
    Position_Least =(Encoder-Position_Zero);             //===���λ�ò�ֵ
    Position_Bias *=0.8;
    Position_Bias += Position_Least*0.2;	             //===һ�׵�ͨ�˲���
    Position_Differential=Position_Bias-Last_Position;
    Last_Position=Position_Bias;
    Position_PWM=Position_Bias*Position_KP+Position_Differential*Position_KD; //===�ٶȿ���
    return Position_PWM;
}

/**************************************************************************
�������ܣ���ֵ��PWM�Ĵ���
**************************************************************************/
void Set_Pwm()
{

    int Amplitude=8100;    //===PWM������8400 ������8100
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



