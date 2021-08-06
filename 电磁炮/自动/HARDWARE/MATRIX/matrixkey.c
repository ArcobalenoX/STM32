#include "matrixkey.h"
void matrixkey_init()
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOA B C E 时钟

    //行输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_8|GPIO_Pin_11;//
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //输出
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//
    GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化

    //列输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //输出
    GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
}

unsigned char matrixkey() //按键扫描
{   
	unsigned char key_value_temp; //临时按键值，默认1111 1111（二进制）
    unsigned char key_value=0xff;//键值
    key_value_temp=0xff;//键值临时变量
	
	//矩阵键盘程序流程//依次拉低第1234行，读入列线数据
    Pin_r_1=0;
    Pin_r_2=1;
    Pin_r_3=1;
    Pin_r_4=1; //拉低第1行。//Pin_r_1第1行行线，Pin_r_2第2行行线，其余类似
    key_value_temp=read_column();//读列数据
    if (key_value_temp != 0xff)//表示第一行有按键按下
    {
        key_value=key_value_temp & 0x1f;//获取按键号，例如0001 1101表示第1行第2列的按键按下
    }
	
    Pin_r_1=1;
    Pin_r_2=0;
    Pin_r_3=1;
    Pin_r_4=1; //拉低第2行
    key_value_temp=read_column();//读列数据
    if (key_value_temp != 0xff)//表示第2行有按键按下
    {
        key_value=key_value_temp &0x2f;
    }
	
    Pin_r_1=1;
    Pin_r_2=1;
    Pin_r_3=0;
    Pin_r_4=1; //拉低第3行
    key_value_temp=read_column();//读列数据
    if (key_value_temp != 0xff)//表示第3行有按键按下
    {
        key_value=key_value_temp &0x3f;
    }
	
    Pin_r_1=1;
    Pin_r_2=1;
    Pin_r_3=1;
    Pin_r_4=0; //拉低第4行
    key_value_temp=read_column();//读列数据
    if (key_value_temp != 0xff)//表示第4行有按键按下
    {
        key_value=key_value_temp &0x4f;   //（2）读取列线数据
    }
    return key_value;
}

unsigned char read_column() //读矩阵键盘的列
{
    unsigned char key_column;
    key_column=0xff;
    if((Pin_c_1==0||Pin_c_2==0||Pin_c_3==0||Pin_c_4==0))
    {
        delay_ms(10);//去抖动
        if(Pin_c_1==0 ) key_column=key_column & 0xfe;//1110表示第1列按键按下。 key_column的低4位表示按键号，高4位常为1111
        if(Pin_c_2==0 ) key_column=key_column & 0xfd;//1101表示第2列按键按下。
        if(Pin_c_3==0 ) key_column=key_column & 0xfb;//1011表示第3列按键按下。
        if(Pin_c_4==0 ) key_column=key_column & 0xf7;//0111表示第4列按键按下。
    }
    return key_column;
}