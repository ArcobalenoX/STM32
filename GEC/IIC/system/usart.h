#ifndef  __USART_H
#define  __USART_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stdio.h"

void usart1_init(void);

extern uint8_t d;


extern  uint8_t recv_buf[256];//���ڽ��յĻ�����
extern  uint8_t cnt;//���������±�
extern  uint8_t usart1_recv_flag ;//�ж��Ƿ�������,δ���==0,���==1


void _sys_exit(int x) ;
int fputc(int ch, FILE *f);

#endif
