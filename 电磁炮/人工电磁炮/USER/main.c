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
#include "matrixkey.h"
#include "relay.h"
#include "timer.h"

u8 PWM1=150;
u8 PWM2=150;
volatile u8 matkey;

void aim(int distance,int angle);
u8 switch_key(u8 key);

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
    delay_init(168);    //初始化延时函数
    uart_init(115200);	//初始化串口波特率为115200
    EXTIX_Init();		//PA0 PE2 PE3 PE4按键外部中断初始化
    LCD_Init();			//LCD初始化
    TIM3_PWM_Init(2000-1,840-1);	//PB5 50Hz 20ms
    TIM4_PWM_Init(2000-1,840-1);	//PB7 50Hz 20ms
    matrixkey_init();			//矩阵键盘
    RELAY_Init();				//继电器两路开关
    TIM5_Int(1000-1,840-1);    //矩阵键盘定时器扫描10ms

    while(1)
    {
        LCD_ShowString(200,100,200,16,16,"MAIN");
        LCD_ShowString(200,120,200,16,16,"Mode Select");
        LCD_ShowString(200,140,200,16,16,"S1 Fire");
        LCD_ShowString(200,160,200,16,16,"S2 Distance");
        LCD_ShowString(200,180,200,16,16,"S3 Distance Angle");
        LCD_ShowString(200,200,200,16,16,"S4 Auto");
        LCD_ShowString(200,220,200,16,16,"S5 Recip Auto");

        LCD_ShowNum(200,500,matkey,3,16);

        if(matkey==S1)  //1.1直接发射
        {
            LCD_Clear(WHITE);
            POINT_COLOR=RED;
            LCD_ShowString(200,100,200,16,16,"Fire");
            LCD_ShowString(200,120,200,16,16,"S12 Short");
            LCD_ShowString(200,140,200,16,16,"S16 Quit ");
            while(1)
            {
                if(matkey==S12)
                {
                    LCD_ShowString(200,200,200,16,16,"Short ing");
                }
                if(matkey==S16)
                {
                    LCD_ShowString(200,200,200,16,16,"quit");
                    break;
                }
            }

            LCD_Clear(WHITE);
        }

        if(matkey==S2)  //1.2输入距离
        {

            u8 a[3]= {0,0,0};
            u8 flag=0;
            u8 t;
            u16 num;
            u8 fire=0;

            LCD_Clear(WHITE);
            POINT_COLOR=RED;
            LCD_ShowString(150,100,200,16,16," Input Distance");

            LCD_ShowString(100,140,300,16,16,"S1 --1   S2 --2   S3 --3  S4--cancel");
            LCD_ShowString(100,160,300,16,16,"S5 --4   S6 --5   S7 --6  S8--affirm");
            LCD_ShowString(100,180,300,16,16,"S9 --7   S10--8   S11--9  S12--fire");
            LCD_ShowString(100,200,300,16,16,"S13--0                    S16--quit ");


            // LCD_ShowString(200,200,200,16,16," ");
            // LCD_ShowString(200,220,200,16,16," ");

            while(1)
            {

                if(matkey==S16)
                {
                    break;
                }
                while(1)
                {

                    t=switch_key(matkey);
                    if(0<=t&&t<=9&&flag==0)
                    {
                        a[flag]=t;
                        LCD_ShowNum(100+flag*20,300,a[flag],2,16);
                        flag=1;
                    }
                    else if(0<=t&&t<=9&&flag==1)
                    {
                        a[flag]=t;
                        LCD_ShowNum(100+flag*20,300,a[flag],2,16);
                        flag=2;
                    }
                    else if(0<=t&&t<=9&&flag==2)
                    {
                        a[flag]=t;
                        LCD_ShowNum(100+flag*20,300,a[flag],2,16);
                        flag=3;
                    }


                    if(matkey==S4)
                    {
                        LCD_ShowString(200,400,200,16,16,"cancel");
                        num=0;
                        flag=0;
                    }

                    if(matkey==S8&&flag==3)
                    {
                        LCD_ShowString(200,500,200,16,16,"affirm distance ");
                        num=a[0]*100+a[1]*10+a[2];
                        LCD_ShowNum(200,400,num,5,16);
                        num=0;
                        flag=0;
                        break;

                    }
                }

            }

            if(matkey==S12&&fire==1)
            {
                LCD_ShowString(200,120,200,16,16,"Fire");
                aim(num,0);
            }


            LCD_Clear(WHITE);
        }


        if(matkey==S3)  //1.3输入距离和角度
        {

            u8 a[3]= {0,0,0};
            u8 t;
            u8 flag=0;
            u16 num;
            LCD_Clear(WHITE);
            LCD_ShowString(30,40,200,16,16," input distance");

            while(1)
            {
                while(matkey!=S12)
                {
                    if(matkey==S2&&flag==0)
                    {

                        a[0]=switch_key (matkey);
                        flag=1;
                        LCD_ShowNum(100,80,a[0],2,16);						
                    }
                    if(matkey==S2&&flag==1)
                    {
                        a[1]=2;
                        flag=2;
                        LCD_ShowNum(120,80,a[1],2,16);
                    }
                    if(matkey==S3&&flag==2)
                    {
                        a[2]=3;
                        flag=3;
                        LCD_ShowNum(140,80,a[2],2,16);
                    }

                }
            }
            if(flag==3)
                num=a[0]*100+a[1]*10+a[2];

            LCD_ShowNum(200,80,num,5,16);

            LCD_ShowString(30,60,200,16,16,"S2 input angle");
            LCD_Clear(WHITE);
        }


        if(matkey==S5)//继电器测试
        {
            LCD_Clear(WHITE);
            LCD_ShowString(200,40,200,16,16, "relay  test");
            LCD_ShowString(200,80,200,16,16, "S1 ON  ");
            LCD_ShowString(200,100,200,16,16,"S2 OFF ");
            LCD_ShowString(200,120,200,16,16,"S16 Quit");
            while(1)
            {

                if(matkey==S16)
                {
                    LCD_Clear(WHITE);
                    break;
                }
                if(matkey==S1)
                {
                    LCD_ShowString(200,150,200,16,16,"RELAY ON  ");
                    RELAY_ON();
                }
                if(matkey==S2)
                {
                    LCD_ShowString(200,150,200,16,16,"RELAY OFF ");
                    RELAY_OFF();
                }

            }
        }


        if(matkey==S6)  //舵机测试
        {
            LCD_Clear(WHITE);
            POINT_COLOR=RED;
            LCD_ShowString(100,100,200,16,16,"SET SERVO PWM");
            LCD_ShowString(100,120,200,16,16,"PWM1：");
            LCD_ShowString(100,140,200,16,16,"PWM2：");
            while(1)
            {

                if(matkey==S16)
                {
                    LCD_Clear(WHITE);
                    break;
                }
                LCD_ShowNum(150,120,PWM1,3,16);
                TIM_SetCompare2(TIM3,PWM1);
                LCD_ShowNum(150,140,PWM2,3,16);
                TIM_SetCompare2(TIM4,PWM2);

            }
        }

    }
}

void aim(int distance,int angle)
{


    if(distance==250)
    {

        TIM_SetCompare2(TIM3,PWM1);
    }

    if(angle==0)
    {

        TIM_SetCompare2(TIM4,PWM2);
    }

}

u8 switch_key(u8 key)
{
    u8 ret;
    switch(key)
    {
    case S1:
        ret=1;
        break;
    case S2:
        ret=2;
        break;
    case S3:
        ret=3;
        break;
    case S5:
        ret=4;
        break;
    case S6:
        ret=5;
        break;
    case S7:
        ret=6;
        break;
    case S9:
        ret=7;
        break;
    case S10:
        ret=8;
        break;
    case S11:
        ret=9;
        break;
    case S13:
        ret=0;
        break;
    default:
        ret=255;
    }
    return ret;
}

