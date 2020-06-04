#include "iwdg.h"

//独立看门狗
void iwdg_init(void)
{
	//启动寄存器访问权限
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	//设置预分频
	IWDG_SetPrescaler(IWDG_Prescaler_32);
	//设置重装载值
	IWDG_SetReload(0x0FFF);
	//启动看门狗
	IWDG_Enable();
}


//IWDG_ReloadCounter();


