#include "iwdg.h"

//�������Ź�
void iwdg_init(void)
{
	//�����Ĵ�������Ȩ��
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	//����Ԥ��Ƶ
	IWDG_SetPrescaler(IWDG_Prescaler_32);
	//������װ��ֵ
	IWDG_SetReload(0x0FFF);
	//�������Ź�
	IWDG_Enable();
}


//IWDG_ReloadCounter();


