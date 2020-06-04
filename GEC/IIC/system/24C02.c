#include "24C02.h"


//往器件写入一个字节
u8 ic_read_byte(u8 addr)
{
	u8 tmp;
	start();
	send_byte(0xa0+0);
	wait_ack();
	send_byte(addr);
	wait_ack();
	start();
	send_byte(0xa0+1);
	wait_ack();
	tmp = read_byte(0);
	stop();
	return tmp;
}

//从器件读出一个字节
void ic_write_byte(u8 addr,u8 data)
{
	start();
	send_byte(0xa0+0);
	wait_ack();
	send_byte(addr);
	wait_ack();
	send_byte(data);
	wait_ack();
	stop();
	delay_ms(10); //易错点，不加延时写入不了
}
