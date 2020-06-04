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
	
//�ض���fputc���� 
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

	//����GPIOʱ�Ӻʹ���ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	//PA9��PA10����Ϊ����ģʽ����ʼ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//������IO�˿��봮�ڹ���
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

	//���ڽṹ�帳ֵ����ʼ��
	USART_InitStructure.USART_BaudRate = bout;//���ñ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//��������λ8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//����ֹͣλ1λ
	USART_InitStructure.USART_Parity = USART_Parity_No;//û����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//�ر�Ӳ������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//ʹ��RX��TXģ��
	USART_Init(USART1,&USART_InitStructure);

	//�жϽṹ�帳ֵ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//�򿪴���1
	USART_Cmd(USART1, ENABLE);
	
	//ʹ�ܴ���1���Ͷ��ж�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}


void adc_in_init()
{
	ADC_InitTypeDef       ADC_InitStructure;//ADCһ��ṹ��
	ADC_CommonInitTypeDef ADC_CommonInitStructure;//ADCӲ�����Ƴ�ʼ���ṹ��
	GPIO_InitTypeDef      GPIO_InitStructure;//GPIO�ṹ��

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//��GPIOA���ʱ�� 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);//����ADC1��ʱ��

	//GPIO�ṹ���ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//3������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ��ģʽ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//û��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//ADCӲ�����Ƴ�ʼ���ṹ���ʼ��----���������е�ADC��ͨ��ģʽ
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;//ʱ�ӷָ�
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;//ȡ��DMAʹ��
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//ÿ5�����ڲ���һ��
	ADC_CommonInit(&ADC_CommonInitStructure);

	//��ʼ��ADC1ģ��
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//����ADC����Ϊ12λ---���ȡֵ4095
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//ȡ��DMAʹ��
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//��ģת����һֱ����
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//�ر�Ӳ������
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;//��ֹ�������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//��ֵ�Ҷ���
	ADC_InitStructure.ADC_NbrOfConversion = 1;//ÿ��ת��ֻת��1����ֵ
	ADC_Init(ADC1, &ADC_InitStructure);

	//ѡ��ADC1ģ���ͨ��5
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_3Cycles);

	//ʹ��ADC1ģ��
	ADC_Cmd(ADC1, ENABLE);
}

void dac_out_init()
{
	DAC_InitTypeDef  DAC_InitStructure;//����DAC�ṹ��
	GPIO_InitTypeDef      GPIO_InitStructure;//GPIO�ṹ��

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//����GPIOA��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);//����dacʱ��

	//GPIO�ṹ���ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//4������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ��ģʽ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//û��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//DAC�ṹ���ʼ��
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;//����Ҫ��ʱ��������ģ���ѹ
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;//�����岨�ε�����
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;//ʹ��DAC���ģ��
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);//��ʼ��DACͨ��1

	//����DAC1ת��
	DAC_Cmd(DAC_Channel_1, ENABLE);
}


int main()
{
	uint32_t dac;
	uint32_t adc;
	uint32_t dac_v;
	uint32_t adc_v;
	//�ж����ȼ�����
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	//���ڳ�ʼ��
	USART_Config(115200);
	
	//adc��ʼ��
	adc_in_init();
	
	//dac��ʼ��
	dac_out_init();
	
	RCC_AHB1ENR |= 0x01<<5;
    GPIOF_MODER |= 0x01<<18;
    GPIOF_OTYPER |= 0x01 << 9;
    GPIOF_OSPEEDER |= 0x11<<18;
    GPIOF_OPUPDR |= 0x01 << 18;
	
	while(1)
	{
		//��ȡDAC����ĵ�ѹֵ	�ڶ������������ֵֻ�ܵ���VREF(3300)
		DAC_SetChannel1Data(DAC_Align_12b_R, 124);

		dac_v = 124*3300/4095;
		
		//��ȡDACģ�����ĵ�ѹֵ
		dac = DAC_GetDataOutputValue(DAC_Channel_1);
		printf("dac_v = %d\r\n",dac_v);
		printf("dac=%d\r\n",dac);
		
		//��ʼ����ADC1��ģ��ת��
		ADC_SoftwareStartConv(ADC1);

		//�ж�ADCģ���Ƿ�ת�����
		while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==RESET);
		
		//��ȡADCת�������ֵ
		adc = ADC_GetConversionValue(ADC1);
		
		printf("adc=%d\r\n",adc);
	
		//��ȡ��ѹֵ
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
	//�жϴ����Ƿ���ķ������ж�
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		d = USART_ReceiveData(USART1);
	 
	  USART_SendData(USART1,d);
		 
		//����жϱ�־λ
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)!=RESET);
	}
}





