#include "adc.h"

//ADC ���� GPIOF7
void F7_adc_init(void)
{
	ADC_InitTypeDef       ADC_InitStructure;//ADCһ��ṹ��
	ADC_CommonInitTypeDef ADC_CommonInitStructure;//ADCӲ�����Ƴ�ʼ���ṹ��
	GPIO_InitTypeDef      GPIO_InitStructure;//GPIO�ṹ��

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//��GPIOF���ʱ�� 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);//����ADC3��ʱ��

	//GPIO�ṹ���ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//7������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ��ģʽ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//û��������
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	//ADCӲ�����Ƴ�ʼ���ṹ���ʼ��----���������е�ADC��ͨ��ģʽ
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;//ʱ�ӷָ�
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;//ȡ��DMAʹ��
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//ÿ5�����ڲ���һ��
	ADC_CommonInit(&ADC_CommonInitStructure);

	//��ʼ��ADC3ģ��
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//����ADC����Ϊ12λ---���ȡֵ4095
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//��ֹɨ��
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//��ģת����һֱ����
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//�ر�Ӳ������
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;//��ֹ�������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//��ֵ�Ҷ���
	ADC_InitStructure.ADC_NbrOfConversion = 1;//ÿ��ת��ֻת��1����ֵ
	ADC_Init(ADC3, &ADC_InitStructure);

	//ѡ��ADC3ģ���ͨ��5
	ADC_RegularChannelConfig(ADC3, ADC_Channel_5, 1, ADC_SampleTime_3Cycles);

	//ʹ��ADC3ģ��
	ADC_Cmd(ADC3, ENABLE);
}

uint16_t F7ADC(void)
{
	uint16_t val;
	//������ת��
	ADC_SoftwareStartConv(ADC3);
	//�ȴ�ת������
	//while(ADC_GetSoftwareStartConvStatus(ADC3)==SET);
	while(ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC ) != SET);
	//��ȡת�����
	val = ADC_GetConversionValue(ADC3);
	return val;
}

//ADC ���� GPIOA5
void A5_adc_init(void)
{
	ADC_InitTypeDef       ADC_InitStructure;//ADCһ��ṹ��
	ADC_CommonInitTypeDef ADC_CommonInitStructure;//ADCӲ�����Ƴ�ʼ���ṹ��
	GPIO_InitTypeDef      GPIO_InitStructure;//GPIO�ṹ��

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//��GPIOA���ʱ�� 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);//����ADC1��ʱ��

	//GPIO�ṹ���ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//5������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ��ģʽ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//û��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//ADCӲ�����Ƴ�ʼ���ṹ���ʼ��----���������е�ADC��ͨ��ģʽ
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;//ʱ�ӷָ�
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;//��ֹDMA
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//ÿ5�����ڲ���һ��
	ADC_CommonInit(&ADC_CommonInitStructure);

	//��ʼ��ADC1ģ��
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//����ADC����Ϊ12λ---���ȡֵ4095
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//��ֹɨ��
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//��ģת����һֱ����
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//�ر�Ӳ������
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;//��ֹ�������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//��ֵ�Ҷ���
	ADC_InitStructure.ADC_NbrOfConversion = 1;//ÿ��ת��ֻת��1����ֵ
	ADC_Init(ADC2, &ADC_InitStructure);

	//ѡ��ADC1ģ���ͨ��5
	ADC_RegularChannelConfig(ADC2, ADC_Channel_5, 1, ADC_SampleTime_3Cycles);

	//ʹ��ADC1ģ��
	ADC_Cmd(ADC2, ENABLE);
}

uint16_t A5ADC(void)
{
	uint16_t val;
	//������ת��
	ADC_SoftwareStartConv(ADC2);
	//�ȴ�ת������
	while(ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC ) != SET);
	//��ȡת�����
	val = ADC_GetConversionValue(ADC2);
	return val;
}


