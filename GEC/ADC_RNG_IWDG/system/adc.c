#include "adc.h"

//ADC 光敏 GPIOF7
void F7_adc_init(void)
{
	ADC_InitTypeDef       ADC_InitStructure;//ADC一般结构体
	ADC_CommonInitTypeDef ADC_CommonInitStructure;//ADC硬件控制初始化结构体
	GPIO_InitTypeDef      GPIO_InitStructure;//GPIO结构体

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//打开GPIOF组的时钟 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);//开启ADC3的时钟

	//GPIO结构体初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//7号引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟模式
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//没有上下拉
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	//ADC硬件控制初始化结构体初始化----设置了所有的ADC的通道模式
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;//时钟分割
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;//取消DMA使能
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//每5个周期采样一次
	ADC_CommonInit(&ADC_CommonInitStructure);

	//初始化ADC3模块
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//设置ADC精度为12位---最大取值4095
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//禁止扫描
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//数模转换器一直工作
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//关闭硬件流控
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;//禁止触发检测
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//数值右对齐
	ADC_InitStructure.ADC_NbrOfConversion = 1;//每次转换只转换1个数值
	ADC_Init(ADC3, &ADC_InitStructure);

	//选择ADC3模块的通道5
	ADC_RegularChannelConfig(ADC3, ADC_Channel_5, 1, ADC_SampleTime_3Cycles);

	//使能ADC3模块
	ADC_Cmd(ADC3, ENABLE);
}

uint16_t F7ADC(void)
{
	uint16_t val;
	//软启动转换
	ADC_SoftwareStartConv(ADC3);
	//等待转换结束
	//while(ADC_GetSoftwareStartConvStatus(ADC3)==SET);
	while(ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC ) != SET);
	//读取转换结果
	val = ADC_GetConversionValue(ADC3);
	return val;
}

//ADC 变阻 GPIOA5
void A5_adc_init(void)
{
	ADC_InitTypeDef       ADC_InitStructure;//ADC一般结构体
	ADC_CommonInitTypeDef ADC_CommonInitStructure;//ADC硬件控制初始化结构体
	GPIO_InitTypeDef      GPIO_InitStructure;//GPIO结构体

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//打开GPIOA组的时钟 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);//开启ADC1的时钟

	//GPIO结构体初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//5号引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟模式
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//没有上下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//ADC硬件控制初始化结构体初始化----设置了所有的ADC的通道模式
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;//时钟分割
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;//禁止DMA
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//每5个周期采样一次
	ADC_CommonInit(&ADC_CommonInitStructure);

	//初始化ADC1模块
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//设置ADC精度为12位---最大取值4095
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//禁止扫描
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//数模转换器一直工作
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//关闭硬件流控
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;//禁止触发检测
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//数值右对齐
	ADC_InitStructure.ADC_NbrOfConversion = 1;//每次转换只转换1个数值
	ADC_Init(ADC2, &ADC_InitStructure);

	//选择ADC1模块的通道5
	ADC_RegularChannelConfig(ADC2, ADC_Channel_5, 1, ADC_SampleTime_3Cycles);

	//使能ADC1模块
	ADC_Cmd(ADC2, ENABLE);
}

uint16_t A5ADC(void)
{
	uint16_t val;
	//软启动转换
	ADC_SoftwareStartConv(ADC2);
	//等待转换结束
	while(ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC ) != SET);
	//读取转换结果
	val = ADC_GetConversionValue(ADC2);
	return val;
}


