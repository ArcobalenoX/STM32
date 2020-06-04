#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"
#include "stdio.h"
 
#define RCC_AHB1ENR *(volatile unsigned int *)(0x40023800 + 0x30)
#define GPIOF_MODER *(volatile unsigned int *)(0x40021400 + 0x00)
#define GPIOF_OTYPER *(volatile unsigned int *)(0x40021400 + 0x04)
#define GPIOF_OSPEEDER *(volatile unsigned int *)(0x40021400 + 0x08)
#define GPIOF_OPUPDR *(volatile unsigned int *)(0x40021400 + 0x0C)
#define GPIOF_ODR *(volatile unsigned int *)(0x40021400 + 0x14)
	
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	USART_SendData(USART1,ch);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);  
	
	return ch;
}   


void USART_Config(int bout)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	//开启GPIO时钟和串口时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	//PA9和PA10设置为复用模式并初始化
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//将复用IO端口与串口关联
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

	//串口结构体赋值并初始化
	USART_InitStructure.USART_BaudRate = bout;//设置比特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//设置数据位8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//设置停止位1位
	USART_InitStructure.USART_Parity = USART_Parity_No;//没有奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//关闭硬件流控
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//使能RX和TX模块
	USART_Init(USART1,&USART_InitStructure);

	//中断结构体赋值并初始化
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//打开串口1
	USART_Cmd(USART1, ENABLE);
	
	//使能串口1发送端中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}


void adc_in_init()
{
	ADC_InitTypeDef       ADC_InitStructure;//ADC一般结构体
	ADC_CommonInitTypeDef ADC_CommonInitStructure;//ADC硬件控制初始化结构体
	GPIO_InitTypeDef      GPIO_InitStructure;//GPIO结构体

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//打开GPIOA组的时钟 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);//开启ADC1的时钟

	//GPIO结构体初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//3号引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟模式
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//没有上下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//ADC硬件控制初始化结构体初始化----设置了所有的ADC的通道模式
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;//时钟分割
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;//取消DMA使能
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//每5个周期采样一次
	ADC_CommonInit(&ADC_CommonInitStructure);

	//初始化ADC1模块
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//设置ADC精度为12位---最大取值4095
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//取消DMA使能
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//数模转换器一直工作
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//关闭硬件流控
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;//禁止触发检测
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//数值右对齐
	ADC_InitStructure.ADC_NbrOfConversion = 1;//每次转换只转换1个数值
	ADC_Init(ADC1, &ADC_InitStructure);

	//选择ADC1模块的通道5
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_3Cycles);

	//使能ADC1模块
	ADC_Cmd(ADC1, ENABLE);
}

void dac_out_init()
{
	DAC_InitTypeDef  DAC_InitStructure;//声明DAC结构体
	GPIO_InitTypeDef      GPIO_InitStructure;//GPIO结构体

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//开启GPIOA组时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);//开启dac时钟

	//GPIO结构体初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//4号引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟模式
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//没有上下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//DAC结构体初始化
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;//不需要定时器来产生模拟电压
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;//不定义波形的种类
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;//使能DAC输出模块
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);//初始化DAC通道1

	//开启DAC1转换
	DAC_Cmd(DAC_Channel_1, ENABLE);
}


int main()
{
	uint32_t dac;
	uint32_t adc;
	uint32_t dac_v;
	uint32_t adc_v;
	//中断优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	//串口初始化
	USART_Config(115200);
	
	//adc初始化
	adc_in_init();
	
	//dac初始化
	dac_out_init();
	
	RCC_AHB1ENR |= 0x01<<5;
    GPIOF_MODER |= 0x01<<18;
    GPIOF_OTYPER |= 0x01 << 9;
    GPIOF_OSPEEDER |= 0x11<<18;
    GPIOF_OPUPDR |= 0x01 << 18;
	
	while(1)
	{
		//获取DAC输出的电压值	第二个参数的最大值只能等于VREF(3300)
		DAC_SetChannel1Data(DAC_Align_12b_R, 124);

		dac_v = 124*3300/4095;
		
		//获取DAC模拟量的电压值
		dac = DAC_GetDataOutputValue(DAC_Channel_1);
		printf("dac_v = %d\r\n",dac_v);
		printf("dac=%d\r\n",dac);
		
		//开始进行ADC1的模数转换
		ADC_SoftwareStartConv(ADC1);

		//判断ADC模块是否转换完成
		while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==RESET);
		
		//获取ADC转换后的数值
		adc = ADC_GetConversionValue(ADC1);
		
		printf("adc=%d\r\n",adc);
	
		//获取电压值
		adc_v = 3300*adc/4096 ;
		printf("adc_v=%dmv\r\n",adc_v);		
		
		if(adc_v>=1000)
		{ GPIOF_ODR &= ~(0x01 << 9);
				delay_ms(1000);
			GPIOF_ODR |= 0x01 << 9;
			delay_ms(1000);
		}
		else
		{
			GPIO_SetBits(GPIOF,GPIO_Pin_9);
			delay_ms(100);
			GPIO_ResetBits(GPIOF,GPIO_Pin_9);
			delay_ms(100);
		}

		
		delay_ms(500);
		delay_ms(500);

	}

}



void USART1_IRQHandler(void)
{
	char d;
	//判断串口是否真的发生了中断
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		d = USART_ReceiveData(USART1);
	 
	  USART_SendData(USART1,d);
		 
		//清除中断标志位
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)!=RESET);
	}
}





