#include "encoder.h"
#include "stm32f4xx.h"
/**************************************************************************
函数功能：把TIM4初始化为编码器接口模式
入口参数：无
返回  值：无
**************************************************************************/
void Encoder_Init_TIM4(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//使能定时器2的时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能PA端口时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	//端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 			//浮空输入
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;    
    GPIO_InitStructure.GPIO_Speed =GPIO_Speed_100MHz;
	
    GPIO_Init(GPIOB, &GPIO_InitStructure);					//根据设定参数初始化GPIOA

  
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4);//PB0引脚复用
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_TIM4);//PB1引脚服用
	
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // 预分频器
    TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD; //设定计数器自动重装值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//选择时钟分频：不分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM向上计数
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3

    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter =6;
    TIM_ICInit(TIM4, &TIM_ICInitStructure);

    TIM_ClearFlag(TIM4, TIM_FLAG_Update);//清除TIM的更新标志位
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    //Reset counter
    TIM_SetCounter(TIM4,0);
    TIM_Cmd(TIM4, ENABLE);
}
/**************************************************************************
函数功能：TIM4中断服务函数
入口参数：无
返回  值：无
**************************************************************************/
void TIM4_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET)
    {
        if((TIM4->CR1>>4 & 0x01)==0) //DIR==0
            circle_count++;
        else if((TIM4->CR1>>4 & 0x01)==1)//DIR==1
            circle_count--;
    }
    TIM_ClearITPendingBit(TIM4,TIM_IT_Update);

    TIM_ClearFlag(TIM4, TIM_FLAG_Update); 
	TIM4->SR&=~(1<<0);//清除中断标志位
}

/**************************************************************************
函数功能：单位时间读取编码器计数
入口参数：定时器
返回  值：速度值
**************************************************************************/
int Read_Encoder(u8 TIMX)
{
    int Encoder_TIM;
    switch(TIMX)
    {
    case 2:
        Encoder_TIM= (short)TIM2 -> CNT;
        TIM2 -> CNT=0;
        break;
    case 3:
        Encoder_TIM= (short)TIM3 -> CNT;
        TIM3 -> CNT=0;
        break;
    case 4:
        Encoder_TIM= (short)TIM4 -> CNT;
        TIM4 -> CNT=0;
        break;
    default:
        Encoder_TIM=0;
    }
    return Encoder_TIM;
}

void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

    TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
    TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE );

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

    TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设

}

void TIM3_IRQHandler(void)   //TIM3中断
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源
     //   Encoder_Left=(short)TIM4->CNT;
     //   TIM4->CNT=0;
    }
}

