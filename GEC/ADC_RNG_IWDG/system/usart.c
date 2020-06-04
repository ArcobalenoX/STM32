#include "usart.h"
#include "stdio.h"

#if 1
#pragma import(__use_no_semihosting) 
struct __FILE 
{ 
	int handle; 
}; 
FILE __stdout; 
void _sys_exit(int x) 
{ 
	x = x; 
} 
int fputc(int ch, FILE *f)
{ 
	while((USART1->SR&0X40)==0);
	USART1->DR = (u8) ch; 
	return ch;
}
#endif

//USART1��Ӧ��IO���� PA9 PA10
void usart1_init(void)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	//��GPIOA���ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//�򿪴���1��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	//�˿ڸ���
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);

	//GPIOΪ����ģʽ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//Tx
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//Rx
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//���ô���
	USART_InitStructure.USART_BaudRate = 115200;//������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//����λ8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//ֹͣλ1λ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�򿪽��պͷ���
	USART_Init(USART1, &USART_InitStructure);
	
	//�ж�����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//�򿪴���1�Ľ��ձ�־λ
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	//ʹ�ܴ���1
	USART_Cmd(USART1, ENABLE);
}

uint8_t d;

void USART1_IRQHandler(void)
{	
	
	//�жϴ���1�Ƿ���յ�����
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		//�жϻ������Ƿ��ѱ�����
		if(usart1_recv_flag==0)
		{
			//��ȡ�Ӵ���1�������ֽ�
			d = USART_ReceiveData(USART1);
		
			//�����յ����������δ�ŵ���������
			recv_buf[cnt]=d;
			cnt++;
			//�ж��Ƿ���յ�\r\n���߳����˻�������С
			if((cnt>=2 && recv_buf[cnt-2] == '\r' && recv_buf[cnt-1] == '\n' )|| cnt == 255)
				usart1_recv_flag=1;			
		}
		/*
		//���������ֽ�ͨ������1���ͳ�ȥ
		USART_SendData(USART1,d);
		//�ȴ�����1�������ݷ��ͽ���
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
		
		//USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		USART1->SR = (uint16_t)0x5f;
		*/
		
	}

}



