#ifndef __24C02_H_
#define __24C02_H_

#include "IIC.h"



u8 ic_read_byte(u8 addr);
void ic_write_byte(u8 addr,u8 data);



#endif

