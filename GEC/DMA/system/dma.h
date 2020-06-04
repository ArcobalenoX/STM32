#ifndef __DMA_H_
#define __DMA_H_

#include "stm32f4xx.h"
#include "usart.h"
	
void dma_mem2mem_init(u32 SrcBaseAddr,u32 DstBaseAddr);

void DMA2S0_cmd(void);


void dma_usart2mem_init(u32 SrcBaseAddr,u32 DstBaseAddr);
void DMA2_USART_TX(void);
void dma_mem2usart_init(u32 SrcBaseAddr,u32 DstBaseAddr);
void DMA2_USART_RX(void);

#endif


