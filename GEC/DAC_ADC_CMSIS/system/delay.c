#include "stm32f4xx.h"
#include "delay.h"

//��ʱ����
void delay_ms(int nms)
{
	int m,n;
	
	//����Ҫ��ʱ���¼������������,,һ��500ms�ı���,һ���ǵ���500ms�Ĳ���
	m = nms/500;
	n = nms%500;
	
	while(m--)
	{
		//�رյδ�ʱ��
		SysTick->CTRL = 0;
		//����value������Ϊ0
		
		SysTick->VAL = 0;

		//����relad�Ĵ���,Ҳ������Ҫ���ݼ�����ֵ
		/*168000000/8--->���δ�ʱ���������Ƶ����21Mhz
		��/1000---->��ÿ���������ֵ���
		��Ϊ��ʱʱ����nms������,����*nms*/
		//LOAD�Ĵ��������һ������ʱ798ms
		SysTick->LOAD = (168000000/8/1000) * 500;

		//���õδ�ʱ��,ʹ���ں˵�ʱ��Ƶ��
		SysTick->CTRL = 1;

		//�ȴ�config��ʱ����16λ�Ƿ���1
		while ((SysTick->CTRL & 0x00010000)==0);

		//�رյδ�ʱ��
		SysTick->CTRL=0;
	}
	
	if(n)
	{
		//�رյδ�ʱ��
		SysTick->CTRL = 0;
		//����value������Ϊ0
		SysTick->VAL = 0;

		//����relad�Ĵ���,Ҳ������Ҫ���ݼ�����ֵ
		/*168000000/8--->���δ�ʱ���������Ƶ����21Mhz
		��/1000---->��ÿ���������ֵ���
		��Ϊ��ʱʱ����nms������,����*nms*/
		//LOAD�Ĵ��������һ������ʱ798ms
		SysTick->LOAD = (168000000/8/1000) * n;

		//���õδ�ʱ��,ʹ���ں˵�ʱ��Ƶ��
		SysTick->CTRL = 1;

		//�ȴ�config��ʱ����16λ�Ƿ���1
		while ((SysTick->CTRL & 0x00010000)==0);

		//�رյδ�ʱ��
		SysTick->CTRL=0;
	}
}

//��ʱ΢��
void delay_us(int nus)
{
	int m,n;
	
	//����Ҫ��ʱ���¼������������,,һ��500ms�ı���,һ���ǵ���500ms�Ĳ���
	m = nus/500;
	n = nus%500;
	
	while(m--)
	{
		//�رյδ�ʱ��
		SysTick->CTRL = 0;
		//����value������Ϊ0
		
		SysTick->VAL = 0;

		SysTick->LOAD = (168000000/8/1000000) * 500;

		//���õδ�ʱ��,ʹ���ں˵�ʱ��Ƶ��
		SysTick->CTRL = 1;

		//�ȴ�config��ʱ����16λ�Ƿ���1
		while ((SysTick->CTRL & 0x00010000)==0);

		//�رյδ�ʱ��
		SysTick->CTRL=0;
	}
	
	if(n)
	{
		//�رյδ�ʱ��
		SysTick->CTRL = 0;
		//����value������Ϊ0
		SysTick->VAL = 0;

		SysTick->LOAD = (168000000/8/1000000) * n;

		//���õδ�ʱ��,ʹ���ں˵�ʱ��Ƶ��
		SysTick->CTRL = 1;

		//�ȴ�config��ʱ����16λ�Ƿ���1
		while ((SysTick->CTRL & 0x00010000)==0);

		//�رյδ�ʱ��
		SysTick->CTRL=0;
	}
}


