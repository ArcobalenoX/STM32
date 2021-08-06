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
#include "fun.h"

u8 PWM1=150; //水平
u8 PWM2=150; //垂直
volatile u8 matkey;

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
    delay_init(168);    //初始化延时函数
    uart_init(115200);	//初始化串口波特率为115200
    EXTIX_Init();		//PA0 PE2 PE3 PE4按键外部中断初始化
    LCD_Init();			//LCD初始化
    TIM3_PWM_Init(2000-1,840-1);	//PB5 50Hz 20ms
    TIM4_PWM_Init(2000-1,840-1);	//PB7 50Hz 20ms
    RELAY_Init();				//继电器开关
    matrixkey_init();			//矩阵键盘
    TIM5_Int(5000-1,840-1);    //矩阵键盘定时器扫描50ms
    OPENMV_Init();
    TIM_SetCompare2(TIM3,PWM1);
    TIM_SetCompare2(TIM4,PWM2);

    while(1)
    {

        POINT_COLOR=RED;
        LCD_ShowString(150,100,200,16,16,"MAIN");
        LCD_ShowString(150,120,200,16,16,"Mode Select");
        LCD_ShowString(150,140,200,16,16,"1.1 S1 Fire");
        LCD_ShowString(150,160,200,16,16,"1.2 S2 Distance");
        LCD_ShowString(150,180,200,16,16,"1.3 S3 Distance Angle");
        LCD_ShowString(150,200,200,16,16,"2.1 S5 Auto Fire1");
        LCD_ShowString(150,220,200,16,16,"2.2 S6 Auto Fire2");

        LCD_ShowString(150,240,200,16,16,"S10 sevor");

        if(matkey==S1)  //1.1直接发射
        {
            LCD_Clear(WHITE);
            POINT_COLOR=RED;
            LCD_ShowString(200,100,200,16,16,"Fire");
            LCD_ShowString(200,120,200,16,16,"S12 Shot");
            LCD_ShowString(200,140,200,16,16,"S16 Quit ");
            while(1)
            {
                if(matkey==S16)
                    break;
                if(matkey==S12)
                {
                    LCD_ShowString(200,200,200,16,16,"Shot ing");
                    RELAY_ON();
                    delay_ms(1000);
                    delay_ms(1000);
                    RELAY_OFF();
                }
            }
            LCD_Clear(WHITE);
        }

        if(matkey==S2)  //1.2输入距离
        {

            u8 a[3]= {0,0,0};
            u16 dis;
            u8 flag=0;
            u8 t=255;
            u8 fire=0;

            LCD_Clear(WHITE);
            POINT_COLOR=RED;
            LCD_ShowString(150,100,200,16,16," Input Distance");

            LCD_ShowString(100,140,300,16,16,"S1 --1   S2 --2   S3 --3  S4 --Clear  ");
            LCD_ShowString(100,160,300,16,16,"S5 --4   S6 --5   S7 --6  S8 --Affirm ");
            LCD_ShowString(100,180,300,16,16,"S9 --7   S10--8   S11--9  S12--Shot   ");
            LCD_ShowString(100,200,300,16,16,"S13--0   S14--input dis   S16--Quit   ");

            LCD_ShowString(100,220,200,16,16,"PWM1：");
            LCD_ShowString(100,240,200,16,16,"PWM2：");


            while(1)
            {
                LCD_ShowNum(150,220,PWM1,3,16);
                TIM_SetCompare2(TIM3,PWM1);
                LCD_ShowNum(150,240,PWM2,3,16);
                TIM_SetCompare2(TIM4,PWM2);

                if(matkey==S16)
                {
                    break;
                }
                if(matkey==S4)
                {
                    LCD_ShowString(200,260,200,16,16,"clear    ");
                    LCD_Fill(100,300,400,400,WHITE);
                    dis=0;
                    flag=0;
                }

                if(matkey==S8)
                {
                    LCD_ShowString(200,260,200,16,16,"affirm    ");
                    dis=a[0]*100+a[1]*10+a[2];
                    LCD_ShowNum(300,300,dis,5,16);
                    fire=1;
                }

                if(matkey==S12&&fire==1)
                {
                    LCD_ShowString(200,260,200,16,16,"Fire       ");
                    aim(dis,35);
                    LCD_ShowNum(150,220,PWM1,3,16);
                    LCD_ShowNum(150,240,PWM2,3,16);
                    fire=0;
                    delay_ms(1000);
                    RELAY_ON();
                    delay_ms(1000);
                    delay_ms(1000);
                    delay_ms(1000);
                    RELAY_OFF();
                }

                if(matkey==S14)
                {
                    LCD_Fill(100,260,400,400,WHITE);
                    LCD_ShowString(200,300,200,16,16,"input distance");
                    while(flag!=3)
                    {
                        t=switch_key(matkey);
                        if(0<=t&&t<=9&&flag==0)
                        {
                            a[flag]=t;
                            LCD_ShowNum(100+flag*20,300,a[flag],2,16);
                            while(1)
                            {
                                if(matkey==S8)
                                {
                                    flag=1;
                                    break;
                                }
                                if(matkey==S4)
                                {
                                    LCD_Fill(100+flag*20,300,100+flag*20+20,350,WHITE);
                                    break;
                                }
                            }
                        }
                        else if(0<=t&&t<=9&&flag==1)
                        {
                            a[flag]=t;
                            LCD_ShowNum(100+flag*20,300,a[flag],2,16);
                            while(1)
                            {
                                if(matkey==S8)
                                {
                                    flag=2;
                                    break;
                                }
                                if(matkey==S4)
                                {
                                    LCD_Fill(100+flag*20,300,100+flag*20+20,350,WHITE);
                                    break;
                                }
                            }
                        }
                        else if(0<=t&&t<=9&&flag==2)
                        {
                            a[flag]=t;
                            LCD_ShowNum(100+flag*20,300,a[flag],2,16);
                            while(1)
                            {
                                if(matkey==S8)
                                {
                                    flag=3;
                                    break;
                                }
                                if(matkey==S4)
                                {
                                    LCD_Fill(100+flag*20,300,100+flag*20+20,350,WHITE);
                                    break;
                                }
                            }
                        }
                    }
                }

            }
            LCD_Clear(WHITE);
        }

        if(matkey==S3)  //1.3输入距离 角度
        {


            u8 a[3]= {0,0,0};
            u8 b[3]= {0,0,0};
            u16 dis=0;
            u16 ang=0;
            u8 flag=0;
            u8 flag2=0;
            u8 t=255;
            u8 fire=0;

            LCD_Clear(WHITE);
            POINT_COLOR=RED;
            LCD_ShowString(120,100,200,16,16," Input Distance And Angle");
            LCD_ShowString(100,140,300,16,16,"S1 --1   S2 --2   S3 --3  S4 --Clear");
            LCD_ShowString(100,160,300,16,16,"S5 --4   S6 --5   S7 --6  S8 --Affirm");
            LCD_ShowString(100,180,300,16,16,"S9 --7   S10--8   S11--9  S12--Shot");
            LCD_ShowString(100,200,300,16,16,"S13--0   S14--D   S15--A  S16--Quit ");

            LCD_ShowString(100,220,200,16,16,"PWM1：");
            LCD_ShowString(100,240,200,16,16,"PWM2：");


            while(1)
            {
                LCD_ShowNum(150,220,PWM1,3,16);
                TIM_SetCompare2(TIM3,PWM1);
                LCD_ShowNum(150,240,PWM2,3,16);
                TIM_SetCompare2(TIM4,PWM2);

                if(matkey==S16)
                {

                    break;
                }
                if(matkey==S4)
                {
                    LCD_ShowString(200,260,200,16,16,"clear        ");
                    LCD_Fill(100,300,400,350,WHITE);
                    dis=0;
                    ang=0;
                    flag=0;
                    flag2=0;
                    fire=0;
                }


                if(matkey==S8 && flag==3 && flag2==3)
                {
                    LCD_ShowString(200,260,200,16,16,"affirm       ");
                    LCD_Fill(100,320,400,400,WHITE);
                    fire=1;
                    flag=0;
                    flag2=0;
                }

                if(matkey==S12&&fire==1)
                {
                    LCD_ShowString(200,260,200,16,16,"Fire         ");

                    aim(dis,(ang)/10);
                    fire=0;
                    delay_ms(1000);
                    RELAY_ON();
                    fire=0;
                }
                if(matkey==S14)//输入距离
                {
                    flag=0;
                    LCD_Fill(100,260,400,400,WHITE);
                    LCD_ShowString(200,300,200,16,16,"input distance");
                    while(flag!=3)
                    {
                        t=switch_key(matkey);
                        if(0<=t&&t<=9&&flag==0)//输入百位
                        {
                            a[flag]=t;
                            LCD_ShowNum(100+flag*20,300,a[flag],2,16);
                            while(1)
                            {
                                if(matkey==S8)//确认
                                {
                                    flag=1;
                                    break;
                                }
                                if(matkey==S4)//撤销
                                {
                                    LCD_Fill(100+flag*20,300,100+flag*20+20,350,WHITE);
                                    break;
                                }
                            }
                        }
                        else if(0<=t&&t<=9&&flag==1)//输入十位
                        {
                            a[flag]=t;
                            LCD_ShowNum(100+flag*20,300,a[flag],2,16);
                            while(1)
                            {
                                if(matkey==S8)
                                {
                                    flag=2;
                                    break;
                                }
                                if(matkey==S4)
                                {
                                    LCD_Fill(100+flag*20,300,100+flag*20+20,350,WHITE);
                                    break;
                                }
                            }
                        }
                        else if(0<=t&&t<=9&&flag==2)//输入个位
                        {
                            a[flag]=t;
                            LCD_ShowNum(100+flag*20,300,a[flag],2,16);
                            while(1)
                            {
                                if(matkey==S8)
                                {
                                    flag=3;
                                    break;
                                }
                                if(matkey==S4)
                                {
                                    LCD_Fill(100+flag*20,300,100+flag*20+20,350,WHITE);
                                    break;
                                }
                            }
                        }
                    }
                    dis=a[0]*100+a[1]*10+a[2];
                    LCD_ShowNum(300,300,dis,5,16);
                }

                if(matkey==S15)//输入角度
                {
                    LCD_ShowString(200,320,200,16,16,"input angle");
                    flag2=0;
                    while(flag2!=3)
                    {
                        t=switch_key(matkey);
                        if(0<=t&&t<=9&&flag2==0)//输入百位
                        {
                            b[flag2]=t;
                            LCD_ShowNum(100+flag2*20,320,b[flag2],2,16);
                            while(1)
                            {
                                if(matkey==S8)//确认输入
                                {
                                    flag2=1;
                                    break;
                                }
                                if(matkey==S4)
                                {
                                    LCD_Fill(100+flag2*20,320,100+flag2*20+20,350,WHITE);
                                    break;
                                }
                            }
                        }
                        else if(0<=t&&t<=9&&flag2==1)//输入十位
                        {
                            b[flag2]=t;
                            LCD_ShowNum(100+flag2*20,320,b[flag2],2,16);
                            while(1)
                            {
                                if(matkey==S8)
                                {
                                    flag2=2;
                                    break;
                                }
                                if(matkey==S4)
                                {
                                    LCD_Fill(100+flag2*20,320,100+flag2*20+20,350,WHITE);
                                    break;
                                }
                            }
                        }
                        else if(0<=t&&t<=9&&flag2==2)//输入个位
                        {
                            b[flag2]=t;
                            LCD_ShowNum(100+flag2*20,320,b[flag2],2,16);
                            while(1)
                            {
                                if(matkey==S8)
                                {
                                    flag2=3;
                                    break;
                                }
                                if(matkey==S4)
                                {
                                    LCD_Fill(100+flag2*20,320,100+flag2*20+20,350,WHITE);
                                    break;
                                }
                            }
                        }
                    }
                    ang=b[0]*100+b[1]*10+b[2];
                    LCD_ShowNum(300,320,ang,5,16);
                }
            }
            LCD_Clear(WHITE);
        }


        if(matkey==S5)  //自动射击1
        {
           
            LCD_Clear(WHITE);
            POINT_COLOR=RED;
            LCD_ShowString(150,100,200,16,16,"AUTO SHOT 1 ");
            LCD_ShowString(150,160,200,16,16,"S16 Quit" );
            while(1)
            {
                if(matkey==S16)
                {
                    break;
                }
                PAout(11)=0;
            }
            LCD_Clear(WHITE);
        }
		
        if(matkey==S6)  //自动射击2
        {
            
            LCD_Clear(WHITE);
            POINT_COLOR=RED;
            LCD_ShowString(150,100,200,16,16,"AUTO SHOT 2 ");
            LCD_ShowString(150,160,200,16,16,"S16 Quit" );
            while(1)
            {
                if(matkey==S16)
                {
                    break;
                }
                PAout(11)=1;             
            }
            LCD_Clear(WHITE);
        }

        if(matkey==S10)  //舵机测试
        {
            LCD_Clear(WHITE);
            POINT_COLOR=RED;
            LCD_ShowString(100,100,200,16,16,"SET SERVO PWM");
            LCD_ShowString(200,120,200,16,16,"S1 ON  ");
            LCD_ShowString(200,140,200,16,16,"S2 OFF ");
            LCD_ShowString(200,160,200,16,16,"S16 Quit");
            LCD_ShowString(100,200,200,16,16,"PWM1：");
            LCD_ShowString(100,220,200,16,16,"PWM2：");
            while(1)
            {
                if(matkey==S16)
                {
                    break;
                }
                if(matkey==S1)
                {
                    LCD_ShowString(200,180,200,16,16,"RELAY ON  ");
                    RELAY_ON();
                }
                if(matkey==S2)
                {
                    LCD_ShowString(200,180,200,16,16,"RELAY OFF ");
                    RELAY_OFF();
                }
                LCD_ShowNum(150,200,PWM1,3,16);
                TIM_SetCompare2(TIM3,PWM1);
                LCD_ShowNum(150,220,PWM2,3,16);
                TIM_SetCompare2(TIM4,PWM2);
            }
            LCD_Clear(WHITE);
        }
    }
	

}

