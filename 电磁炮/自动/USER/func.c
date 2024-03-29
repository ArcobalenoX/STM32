#include "fun.h"
#include "matrixkey.h"
#include "pwm.h"
extern u8 PWM1,PWM2;
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

u8 dis_to_pwm[101]= {	165,165,166,166,166,167,167,167,167,167,
                        167,168,168,168,168,168,168,169,169,169,
                        169,169,170,170,170,170,170,171,171,171,
                        171,171,172,172,172,172,173,173,173,173,
                        174,174,174,174,175,175,175,175,176,176,
                        176,177,177,177,177,178,178,178,179,179,
                        179,180,180,180,180,181,181,181,182,182,
                        182,183,183,183,184,184,184,184,185,185,
                        185,186,186,186,186,187,187,187,188,188,
                        188,188,188,189,189,189,189,189,190,190,190
                    };


u8 ang_to_pwm[61]= {	120,121,122,123,124,125,126,127,128,
                        129,130,131,132,133,134,135,136,137,
                        138,139,140,141,142,143,144,145,146,147,
                        148,150,152,153,154,155,156,157,158,159,160,
                        161,162,163,164,165,166,166,167,168,169,
                        170,171,172,173,174,175,176,177,178,179,180

                   };

void aim(int distance,int angle)
{
    if(150<distance && distance<350)
        PWM2=dis_to_pwm[distance-200];
    if(0<=angle && angle<=60)
        PWM1=ang_to_pwm[angle];
    if(100<PWM1&&PWM1<200)
    {
        TIM_SetCompare2(TIM3,PWM1);
    }
    if(150<PWM2&&PWM2<200)
    {
        TIM_SetCompare2(TIM4,PWM2);
    }

}
