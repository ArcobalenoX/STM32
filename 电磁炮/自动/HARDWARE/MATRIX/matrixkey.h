#ifndef _MATRIXKEY_H
#define _MATRIXKEY_H
#include "sys.h"
#include "delay.h"
#define Pin_r_1 PCout(6) //D0
#define Pin_r_2 PCout(8) //D2
#define Pin_r_3 PCout(11) //D4
#define Pin_r_4 PEout(5) //D6
#define Pin_c_1 PCin(7)//D1
#define Pin_c_2 PCin(9) //D3
#define Pin_c_3 PBin(6) //D5 
#define Pin_c_4 PEin(6) //D7

#define S1 78
#define S2 77
#define S3 75
#define S4 71
#define S5 62
#define S6 61
#define S7 59
#define S8 55
#define S9 46
#define S10 45
#define S11 43
#define S12 39
#define S13 30
#define S14 29
#define S15 27
#define S16 23

void matrixkey_init();
unsigned char matrixkey();
unsigned char read_column();

#endif
