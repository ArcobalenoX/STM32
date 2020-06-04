#include "24C02.h"


//������д��һ���ֽ�
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

//����������һ���ֽ�
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
	delay_ms(10); //�״�㣬������ʱд�벻��
}
