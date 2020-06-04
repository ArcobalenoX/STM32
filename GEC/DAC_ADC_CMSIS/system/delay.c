#include "stm32f4xx.h"
#include "delay.h"

//延时毫秒
void delay_ms(int nms)
{
	int m,n;
	
	//把需要延时的事件拆成两个部分,,一个500ms的倍数,一个是低于500ms的部分
	m = nms/500;
	n = nms%500;
	
	while(m--)
	{
		//关闭滴答定时器
		SysTick->CTRL = 0;
		//设置value计数器为0
		
		SysTick->VAL = 0;

		//设置relad寄存器,也就是需要被递减的数值
		/*168000000/8--->将滴答定时器的输入的频率是21Mhz
		再/1000---->将每毫秒计数的值求出
		因为延时时间是nms个毫秒,所以*nms*/
		//LOAD寄存器最大能一次性延时798ms
		SysTick->LOAD = (168000000/8/1000) * 500;

		//启用滴答定时器,使用内核的时钟频率
		SysTick->CTRL = 1;

		//等待config定时器的16位是否被置1
		while ((SysTick->CTRL & 0x00010000)==0);

		//关闭滴答定时器
		SysTick->CTRL=0;
	}
	
	if(n)
	{
		//关闭滴答定时器
		SysTick->CTRL = 0;
		//设置value计数器为0
		SysTick->VAL = 0;

		//设置relad寄存器,也就是需要被递减的数值
		/*168000000/8--->将滴答定时器的输入的频率是21Mhz
		再/1000---->将每毫秒计数的值求出
		因为延时时间是nms个毫秒,所以*nms*/
		//LOAD寄存器最大能一次性延时798ms
		SysTick->LOAD = (168000000/8/1000) * n;

		//启用滴答定时器,使用内核的时钟频率
		SysTick->CTRL = 1;

		//等待config定时器的16位是否被置1
		while ((SysTick->CTRL & 0x00010000)==0);

		//关闭滴答定时器
		SysTick->CTRL=0;
	}
}

//延时微秒
void delay_us(int nus)
{
	int m,n;
	
	//把需要延时的事件拆成两个部分,,一个500ms的倍数,一个是低于500ms的部分
	m = nus/500;
	n = nus%500;
	
	while(m--)
	{
		//关闭滴答定时器
		SysTick->CTRL = 0;
		//设置value计数器为0
		
		SysTick->VAL = 0;

		SysTick->LOAD = (168000000/8/1000000) * 500;

		//启用滴答定时器,使用内核的时钟频率
		SysTick->CTRL = 1;

		//等待config定时器的16位是否被置1
		while ((SysTick->CTRL & 0x00010000)==0);

		//关闭滴答定时器
		SysTick->CTRL=0;
	}
	
	if(n)
	{
		//关闭滴答定时器
		SysTick->CTRL = 0;
		//设置value计数器为0
		SysTick->VAL = 0;

		SysTick->LOAD = (168000000/8/1000000) * n;

		//启用滴答定时器,使用内核的时钟频率
		SysTick->CTRL = 1;

		//等待config定时器的16位是否被置1
		while ((SysTick->CTRL & 0x00010000)==0);

		//关闭滴答定时器
		SysTick->CTRL=0;
	}
}


