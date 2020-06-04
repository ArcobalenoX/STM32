#ifndef  __USART_H
#define  __USART_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

void usart1_init(void);

extern uint8_t d;


extern  uint8_t recv_buf[256];//串口接收的缓冲区
extern  uint8_t cnt;//缓冲区的下标
extern  uint8_t usart1_recv_flag ;//判断是否接收完成,未完成==0,完成==1



#endif
