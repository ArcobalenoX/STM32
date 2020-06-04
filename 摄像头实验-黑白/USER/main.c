#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "usmart.h"
#include "usart2.h"
#include "timer.h"
#include "ov2640.h"
#include "dcmi.h"
#include "string.h"
#include "exti.h"
//#include "pwm.h"
int graybase=0;
u16 rgb_buf[200][200];
u16 gray;
u16 hang=0;

u8 X_MAX=0,Y_MAX=0;    //小球的坐标信息
u8 X_MIN=190,Y_MIN=190;
u8 X_MAX_LSAT=0, X_MIN_LSAT=0, Y_MAX_LSAT=0, Y_MIN_LSAT=0;   //上一次小球坐标位置信息
u8 X=0,Y=0;      //小球的质心信息



//pid参数初始化
int PWM_X,PWM_Y=0;
float Err_X,Err_Y=0;
float Err_X_LAST,Err_Y_LAST=0;
float Aim_X,Aim_Y=0;
float Kp,Ki,Kd=0;

int main(void)
{
    u16 i,j;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
    delay_init(168);  //初始化延时函数
    uart_init(115200);                //初始化串口波特率为115200
    usart2_init(42,115200);           //初始化串口2波特率为115200
    LED_Init();                       //初始化LED
    LCD_Init();                       //LCD初始化
    KEY_Init();                       //按键初始化
    EXTIX_Init();
    TIM3_Int_Init(200-1,8400-1);//50hz计数,20ms秒钟中断一次
    //TIM14_PWM_Init(10000-1,168-1);        //重装载值10000，所以PWM频率为 150hz.      //舵机预留
    //TIM11_PWM_Init(10000-1,336-1);        //重装载值10000，所以PWM频率为 150hz.

    usmart_dev.init(84);                //初始化USMART
    POINT_COLOR=BLUE;//设置字体为红色
    LCD_ShowString(30,50,200,16,16,"Explorer STM32F4");
    LCD_ShowString(30,70,200,16,16,"OV2640 TEST");
    while(OV2640_Init())//初始化OV2640
    {
        LCD_ShowString(30,90,240,16,16,"OV2640 ERR");
        delay_ms(200);
        LCD_Fill(30,90,239,170,WHITE);
        delay_ms(200);
    }
    LCD_ShowString(30,90,200,16,16,"OV2640 OK");
    LCD_ShowString(30,110,200,16,16,"graybase:");
    LCD_ShowString(30,130,200,16,16,"X POSI:");
    LCD_ShowString(30,150,200,16,16,"Y POSI:");


    OV2640_RGB565_Mode();        //RGB565模式
    //OV2640_ImageSize_Set(1600,1200);
    OV2640_ImageWin_Set(600,400,400,400);
    OV2640_OutSize_Set(200,200);

    My_DCMI_Init();                        //DCMI配置
    DCMI_DMA_Init((u32)rgb_buf,sizeof(rgb_buf)/4,DMA_MemoryDataSize_HalfWord,DMA_MemoryInc_Enable);//DCMI DMA配置
    DCMI_Start();                 //启动传输

    while(1)
    {
        LCD_ShowNum(100,110,graybase,3,16);
        hang=0;
        LCD_SetCursor(140,300);
        LCD_WriteRAM_Prepare();                //开始写入GRAM
        for(i=0; i<200; i++)
        {
            for(j=0; j<200; j++)
            {
                if(j==199)
                {
                    hang++;
                    LCD_SetCursor(140,i+301);
                    LCD_WriteRAM_Prepare();                //开始写入GRAM
                }

                gray=((rgb_buf[i][j]>>11)*19595+((rgb_buf[i][j]>>5)&0x3f)*38469 +(rgb_buf[i][j]&0x1f)*7472)>>16;
                //LCD->LCD_RAM=rgb_buf[i][j];

                if(gray>=graybase)
                {
                    LCD->LCD_RAM=WHITE;


                }
                else
                {
                    LCD->LCD_RAM=BLACK;
                    if(i>10&&i<190&&j>10&&j<190)
                    {
                        if(i>X_MAX) X_MAX=i;
                        if(i<X_MIN) X_MIN=i;

                        if(j>Y_MAX) Y_MAX=j;
                        if(j<Y_MIN) Y_MIN=j;
                    }


                }

            }
        }

        X_MAX_LSAT = X_MAX;    //更新pid的real坐标信息 清除掉本次坐标用于再次遍历最大值 最小值
        X_MIN_LSAT = X_MIN;
        Y_MAX_LSAT = Y_MAX;
        Y_MIN_LSAT = Y_MIN;

        X_MAX=0;
        X_MIN=190;
        Y_MAX=0;
        Y_MIN=190;

        X=(X_MAX_LSAT+X_MIN_LSAT)/2;
        Y=(Y_MAX_LSAT+Y_MIN_LSAT)/2;
        LCD_ShowNum(100,130,X,3,16);
        LCD_ShowNum(100,150,Y,3,16);

        // TIM_SetCompare1(TIM14,9340);        //修改比较值，修改占空比
        // TIM_SetCompare1(TIM11,9300);        //修改比较值，修改占空比
    }
}


