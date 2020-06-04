#ifndef __IIC_H_
#define __IIC_H_

#include "stm32f4xx.h"
#include "delay.h"
#include "BitBandOP.h"


//SCL--PB8 
//SDA--PB9

void IICIO_init(void);
void start(void);
void stop(void);
char wait_ack(void);
void send_ack(void);
void send_no_ack(void);

void send_byte(u8 txd);
u8 read_byte(u8 ack);






#endif


