#include "dma.h"

void dma_mem2mem_init(u32 SrcBaseAddr,u32 DstBaseAddr)
{
	DMA_InitTypeDef DMA_InitStruct;
	//使能时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	//默认值
	DMA_StructInit(&DMA_InitStruct);
	while (DMA_GetCmdStatus(DMA2_Stream0) != DISABLE) {} //等待DMA1_Stream0可配置

	//设定值
	/* Initialize the DMA_Channel member */
	DMA_InitStruct.DMA_Channel = DMA_Channel_0;

	/* Initialize the DMA_PeripheralBaseAddr member */
	DMA_InitStruct.DMA_PeripheralBaseAddr = SrcBaseAddr;

	/* Initialize the DMA_Memory0BaseAddr member */
	DMA_InitStruct.DMA_Memory0BaseAddr = DstBaseAddr;

	/* Initialize the DMA_DIR member */
	DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToMemory;

	/* Initialize the DMA_BufferSize member */
	DMA_InitStruct.DMA_BufferSize = 100;

	/* Initialize the DMA_PeripheralInc member */
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Enable;

	/* Initialize the DMA_MemoryInc member */
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;

	/* Initialize the DMA_PeripheralDataSize member */
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;

	/* Initialize the DMA_MemoryDataSize member */
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;

	/* Initialize the DMA_Mode member */
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;

	/* Initialize the DMA_Priority member */
	DMA_InitStruct.DMA_Priority = DMA_Priority_Low;

	/* Initialize the DMA_FIFOMode member */
	DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;

	/* Initialize the DMA_FIFOThreshold member */
	DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;

	/* Initialize the DMA_MemoryBurst member */
	DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;

	/* Initialize the DMA_PeripheralBurst member */
	DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

	DMA_Init(DMA2_Stream0,&DMA_InitStruct);
	
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel 			= DMA2_Stream0_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd 			= ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority 	= 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority 		= 1;
	NVIC_Init(&NVIC_InitStruct);
	
	DMA_ITConfig(DMA2_Stream0,DMA_IT_TC,ENABLE);
	

}

void DMA2S0_cmd(void)
{
	int cnt=10000;
	DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
	DMA_Cmd(DMA2_Stream0, ENABLE);	
	while(DMA_GetCmdStatus(DMA2_Stream0)!=ENABLE && cnt >0){cnt--;}
	if(cnt==0)
	{
			printf("DMA DISABLE\r\n");
	}
	else
	{
			printf("DMA ENABLE\r\n");
	}

}


void DMA2_Stream0_IRQHandler()
{
	if( DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0)==SET)
	{
		printf("DMA_IT_TCIF0 \r\n");
		
		DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
	}

}
//串口TX--DMA2S7通道4
void dma_mem2usart_init(u32 SrcBaseAddr,u32 DstBaseAddr)
{
	DMA_InitTypeDef DMA_InitStruct;
	//使能时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	//默认值
	DMA_StructInit(&DMA_InitStruct);
	while (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE) {} //等待DMA1_Stream0可配置

	//设定值
	/* Initialize the DMA_Channel member */
	DMA_InitStruct.DMA_Channel = DMA_Channel_4;

	/* Initialize the DMA_PeripheralBaseAddr member */
	DMA_InitStruct.DMA_PeripheralBaseAddr =DstBaseAddr;

	/* Initialize the DMA_Memory0BaseAddr member */
	DMA_InitStruct.DMA_Memory0BaseAddr = SrcBaseAddr ;

	/* Initialize the DMA_DIR member */
	DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;

	/* Initialize the DMA_BufferSize member */
	DMA_InitStruct.DMA_BufferSize = 100;

	/* Initialize the DMA_PeripheralInc member */
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

	/* Initialize the DMA_MemoryInc member */
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;

	/* Initialize the DMA_PeripheralDataSize member */
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;

	/* Initialize the DMA_MemoryDataSize member */
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;

	/* Initialize the DMA_Mode member */
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;

	/* Initialize the DMA_Priority member */
	DMA_InitStruct.DMA_Priority = DMA_Priority_Low;

	/* Initialize the DMA_FIFOMode member */
	DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;

	/* Initialize the DMA_FIFOThreshold member */
	DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;

	/* Initialize the DMA_MemoryBurst member */
	DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;

	/* Initialize the DMA_PeripheralBurst member */
	DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

	DMA_Init(DMA2_Stream7,&DMA_InitStruct);
	

	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel 			= DMA2_Stream7_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd 			= ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority 	= 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority 		= 1;
	NVIC_Init(&NVIC_InitStruct);
	
	DMA_ITConfig(DMA2_Stream7,DMA_IT_TC,ENABLE);
	
}

void DMA2_USART_TX(void)
{
	int cnt=10000;
	DMA_ClearITPendingBit(DMA2_Stream7, DMA_IT_TCIF7);
	DMA_Cmd(DMA2_Stream7, ENABLE);	
	while(DMA_GetCmdStatus(DMA2_Stream7)!=ENABLE && cnt >0){cnt--;}
	if(cnt==0)
	{
			printf("DMA DISABLE\r\n");
	}
	else
	{
			printf("DMA ENABLE\r\n");
	}

}

void DMA2_Stream7_IRQHandler()
{
	if( DMA_GetITStatus(DMA2_Stream7, DMA_IT_TCIF7)==SET)
	{
		printf("DMA_IT_TCIF7 \r\n");
		
		DMA_ClearITPendingBit(DMA2_Stream7, DMA_IT_TCIF7);
	}

}


//串口RX--DMA2S5通道4
void dma_usart2mem_init(u32 SrcBaseAddr,u32 DstBaseAddr)
{
	DMA_InitTypeDef DMA_InitStruct;
	//使能时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	//默认值
	DMA_StructInit(&DMA_InitStruct);
	while (DMA_GetCmdStatus(DMA2_Stream5) != DISABLE) {} //等待DMA1_Stream0可配置

	//设定值
	/* Initialize the DMA_Channel member */
	DMA_InitStruct.DMA_Channel = DMA_Channel_4;

	/* Initialize the DMA_PeripheralBaseAddr member */
	DMA_InitStruct.DMA_PeripheralBaseAddr =SrcBaseAddr;

	/* Initialize the DMA_Memory0BaseAddr member */
	DMA_InitStruct.DMA_Memory0BaseAddr = DstBaseAddr ;

	/* Initialize the DMA_DIR member */
	DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;

	/* Initialize the DMA_BufferSize member */
	DMA_InitStruct.DMA_BufferSize = 100;

	/* Initialize the DMA_PeripheralInc member */
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

	/* Initialize the DMA_MemoryInc member */
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;

	/* Initialize the DMA_PeripheralDataSize member */
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;

	/* Initialize the DMA_MemoryDataSize member */
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;

	/* Initialize the DMA_Mode member */
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;

	/* Initialize the DMA_Priority member */
	DMA_InitStruct.DMA_Priority = DMA_Priority_Low;

	/* Initialize the DMA_FIFOMode member */
	DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;

	/* Initialize the DMA_FIFOThreshold member */
	DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;

	/* Initialize the DMA_MemoryBurst member */
	DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;

	/* Initialize the DMA_PeripheralBurst member */
	DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

	DMA_Init(DMA2_Stream5,&DMA_InitStruct);
	

	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel 			= DMA2_Stream5_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd 			= ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority 	= 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority 		= 1;
	NVIC_Init(&NVIC_InitStruct);
	
	DMA_ITConfig(DMA2_Stream5,DMA_IT_TC,ENABLE);
	
}

void DMA2_USART_RX(void)
{
	int cnt=10000;
	DMA_ClearITPendingBit(DMA2_Stream5, DMA_IT_TCIF5);
	DMA_Cmd(DMA2_Stream5, ENABLE);	
	while(DMA_GetCmdStatus(DMA2_Stream5)!=ENABLE && cnt >0){cnt--;}
	if(cnt==0)
	{
			printf("DMA DISABLE\r\n");
	}
	else
	{
			printf("DMA ENABLE\r\n");
	}

}

void DMA2_Stream5_IRQHandler()
{
	if( DMA_GetITStatus(DMA2_Stream5, DMA_IT_TCIF5)==SET)
	{
		printf("DMA_IT_TCIF5 \r\n");
		
		DMA_ClearITPendingBit(DMA2_Stream5, DMA_IT_TCIF5);
	}

}




