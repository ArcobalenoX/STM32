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


u8 PWM1=150; //水平
u8 PWM2=150; //垂直
volatile u8 matkey;
u16 dis_pwm2_arr[101][2];
u16 ang_pwm1_arr[61][2];

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
    TIM5_Int(5000-1,840-1);    //矩阵键盘定时器扫描50ms
    OPENMV_Init();

    while(1)
    {

        POINT_COLOR=RED;
        LCD_ShowString(150,100,200,16,16,"MAIN");
        LCD_ShowString(150,120,200,16,16,"Mode Select");
        LCD_ShowString(150,140,200,16,16,"S1 1.1Fire");
        LCD_ShowString(150,160,200,16,16,"S2 1.2Distance");
        LCD_ShowString(150,180,200,16,16,"S3 1.3Distance Angle");
        LCD_ShowString(150,200,200,16,16,"S5 2.1Auto Fire1");
        LCD_ShowString(150,220,200,16,16,"S6 2.2Auto Fire2");

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
                    if(dis>200)
                        aim(dis,0);
                    fire=0;
                    delay_ms(1000);
                    RELAY_ON();
                }

                if(matkey==S14)
                {
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
                    fire=0;
                }

                if(matkey==S8)
                {
                    LCD_ShowString(200,260,200,16,16,"affirm       ");
                    dis=a[0]*100+a[1]*10+a[2];
                    LCD_ShowNum(300,300,dis,5,16);
                    ang=b[0]*100+b[1]*10+b[2];
                    LCD_ShowNum(300,320,ang,5,16);
                    fire=1;
                }

                if(matkey==S12&&fire==1)
                {
                    LCD_ShowString(200,260,200,16,16,"Fire         ");
					aim(dis,ang/10);
					delay_ms(1000);
					RELAY_ON();
                    fire=0;
                }

                if(matkey==S14)//输入距离
                {
                    flag=0;
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

                if(matkey==S15)//输入角度
                {
                    LCD_ShowString(200,320,200,16,16,"input angle");
                    flag=0;
                    while(flag!=3)
                    {
                        t=switch_key(matkey);
                        if(0<=t&&t<=9&&flag==0)
                        {
                            b[flag]=t;
                            LCD_ShowNum(100+flag*20,320,b[flag],2,16);
                            while(1)
                            {
                                if(matkey==S8)
                                {
                                    flag=1;
                                    break;
                                }
                                if(matkey==S4)
                                {
                                    LCD_Fill(100+flag*20,320,100+flag*20+20,350,WHITE);
                                    break;
                                }
                            }
                        }
                        else if(0<=t&&t<=9&&flag==1)
                        {
                            b[flag]=t;
                            LCD_ShowNum(100+flag*20,320,b[flag],2,16);
                            while(1)
                            {
                                if(matkey==S8)
                                {
                                    flag=2;
                                    break;
                                }
                                if(matkey==S4)
                                {
                                    LCD_Fill(100+flag*20,320,100+flag*20+20,350,WHITE);
                                    break;
                                }
                            }
                        }
                        else if(0<=t&&t<=9&&flag==2)
                        {
                            b[flag]=t;
                            LCD_ShowNum(100+flag*20,320,b[flag],2,16);
                            while(1)
                            {
                                if(matkey==S8)
                                {
                                    flag=3;
                                    break;
                                }
                                if(matkey==S4)
                                {
                                    LCD_Fill(100+flag*20,320,100+flag*20+20,350,WHITE);
                                    break;
                                }
                            }
                        }
                    }
                }

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
		if(matkey==S5)  //自动射击测试
        {
			
            int i,dir=1;
            LCD_Clear(WHITE);
            POINT_COLOR=RED;
            LCD_ShowString(100,100,200,16,16,"openmv test ");

            LCD_ShowString(200,160,200,16,16,"S16 Quit");
            LCD_ShowString(100,200,200,16,16,"PWM1：");
            LCD_ShowString(100,220,200,16,16,"PWM2：");
            while(1)
            {
                PWM1+=dir;
				delay_ms(20);
                TIM_SetCompare2(TIM3,PWM1);
                if(PWM1<110||PWM1>190)
                {
                    dir*=-1;
                }
                if(matkey==S16)
                {
                    break;
                }
                if(PAin(11)==0&&PAin(12)==0)
                {
                    LCD_ShowString(200,180,200,16,16,"center ON  ");
                    dir=0;
					//aim(
                    RELAY_ON();
                }
				
                /*            if(PAin(11)==1&&PAin(12)==0)
                            {
                                LCD_ShowString(200,180,200,16,16,"left  ");
                               // if(100<PWM1&&PWM1<150)
                               // {
                                    PWM1+=1;
                                    TIM_SetCompare2(TIM3,PWM1);

                               // }
                                delay_ms(100);
                            }

                            if(PAin(11)==0&&PAin(12)==1)
                            {
                                LCD_ShowString(200,180,200,16,16,"right  ");
                               // if(150<PWM1&&PWM1<250)
                               // {
                                    PWM1-=1;
                                    TIM_SetCompare2(TIM3,PWM1);
                               // }
                                delay_ms(100);
                            }
                */
                LCD_ShowNum(150,200,PWM1,3,16);
                TIM_SetCompare2(TIM3,PWM1);
                LCD_ShowNum(150,220,PWM2,3,16);
                TIM_SetCompare2(TIM4,PWM2);

            }
            LCD_Clear(WHITE);
        }
		
		
        if(matkey==S11)  //openmv测试
        {
            int i,dir=1;
            LCD_Clear(WHITE);
            POINT_COLOR=RED;
            LCD_ShowString(100,100,200,16,16,"openmv test ");

            LCD_ShowString(200,160,200,16,16,"S16 Quit");
            LCD_ShowString(100,200,200,16,16,"PWM1：");
            LCD_ShowString(100,220,200,16,16,"PWM2：");
            while(1)
            {
                PWM1+=dir;
                //delay_ms(20);
                TIM_SetCompare2(TIM3,PWM1);
                if(PWM1<110||PWM1>190)
                {
                    dir*=-1;

                }
                if(matkey==S16)
                {
                    break;
                }
                if(PAin(11)==0&&PAin(12)==0)
                {
                    LCD_ShowString(200,180,200,16,16,"center ON  ");
                    break;

                    RELAY_ON();
                }
                /*            if(PAin(11)==1&&PAin(12)==0)
                            {
                                LCD_ShowString(200,180,200,16,16,"left  ");
                               // if(100<PWM1&&PWM1<150)
                               // {
                                    PWM1+=1;
                                    TIM_SetCompare2(TIM3,PWM1);

                               // }
                                delay_ms(100);
                            }

                            if(PAin(11)==0&&PAin(12)==1)
                            {
                                LCD_ShowString(200,180,200,16,16,"right  ");
                               // if(150<PWM1&&PWM1<250)
                               // {
                                    PWM1-=1;
                                    TIM_SetCompare2(TIM3,PWM1);
                               // }
                                delay_ms(100);
                            }
                */
                LCD_ShowNum(150,200,PWM1,3,16);
                TIM_SetCompare2(TIM3,PWM1);
                LCD_ShowNum(150,220,PWM2,3,16);
                TIM_SetCompare2(TIM4,PWM2);

            }
            LCD_Clear(WHITE);
        }


    }

}



void aim(int distance,int angle)
{

    PWM2=0.5*(distance-200)+150;
	
    PWM1=115+angle;
    delay_ms(1000);
	
    if(100<PWM1&&PWM1<200)
    {
        TIM_SetCompare2(TIM3,PWM1);
    }
    if(150<PWM2&&PWM2<200)
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

