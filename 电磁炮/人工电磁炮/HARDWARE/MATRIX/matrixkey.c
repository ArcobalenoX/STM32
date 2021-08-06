#include "matrixkey.h"
void matrixkey_init()
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOA B C E ʱ��

    //�����
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_8|GPIO_Pin_11;//
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //���
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//
    GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��

    //������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //���
    GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
}

unsigned char matrixkey() //����ɨ��
{   
	unsigned char key_value_temp; //��ʱ����ֵ��Ĭ��1111 1111�������ƣ�
    unsigned char key_value=0xff;//��ֵ
    key_value_temp=0xff;//��ֵ��ʱ����
	
	//������̳�������//�������͵�1234�У�������������
    Pin_r_1=0;
    Pin_r_2=1;
    Pin_r_3=1;
    Pin_r_4=1; //���͵�1�С�//Pin_r_1��1�����ߣ�Pin_r_2��2�����ߣ���������
    key_value_temp=read_column();//��������
    if (key_value_temp != 0xff)//��ʾ��һ���а�������
    {
        key_value=key_value_temp & 0x1f;//��ȡ�����ţ�����0001 1101��ʾ��1�е�2�еİ�������
    }
	
    Pin_r_1=1;
    Pin_r_2=0;
    Pin_r_3=1;
    Pin_r_4=1; //���͵�2��
    key_value_temp=read_column();//��������
    if (key_value_temp != 0xff)//��ʾ��2���а�������
    {
        key_value=key_value_temp &0x2f;
    }
	
    Pin_r_1=1;
    Pin_r_2=1;
    Pin_r_3=0;
    Pin_r_4=1; //���͵�3��
    key_value_temp=read_column();//��������
    if (key_value_temp != 0xff)//��ʾ��3���а�������
    {
        key_value=key_value_temp &0x3f;
    }
	
    Pin_r_1=1;
    Pin_r_2=1;
    Pin_r_3=1;
    Pin_r_4=0; //���͵�4��
    key_value_temp=read_column();//��������
    if (key_value_temp != 0xff)//��ʾ��4���а�������
    {
        key_value=key_value_temp &0x4f;   //��2����ȡ��������
    }
    return key_value;
}

unsigned char read_column() //��������̵���
{
    unsigned char key_column;
    key_column=0xff;
    if((Pin_c_1==0||Pin_c_2==0||Pin_c_3==0||Pin_c_4==0))
    {
        delay_ms(10);//ȥ����
        if(Pin_c_1==0 ) key_column=key_column & 0xfe;//1110��ʾ��1�а������¡� key_column�ĵ�4λ��ʾ�����ţ���4λ��Ϊ1111
        if(Pin_c_2==0 ) key_column=key_column & 0xfd;//1101��ʾ��2�а������¡�
        if(Pin_c_3==0 ) key_column=key_column & 0xfb;//1011��ʾ��3�а������¡�
        if(Pin_c_4==0 ) key_column=key_column & 0xf7;//0111��ʾ��4�а������¡�
    }
    return key_column;
}