#include "RTC.h"

void RTC_init(void)
{
	//��Դʹ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	PWR_BackupAccessCmd(ENABLE);
	
	if(RTC_ReadBackupRegister(RTC_BKP_DR0)==22)
		return;
	
	//�ȴ������ȶ�
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);//ȷ�Ͽ���
	
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	RCC_RTCCLKCmd(ENABLE);
	
	//Ӳ����ʼ��
	RTC_InitTypeDef RTC_InitStruct;
	RTC_InitStruct.RTC_HourFormat=RTC_HourFormat_24;//24Сʱ��
	RTC_InitStruct.RTC_AsynchPrediv=128-1;//�첽��Ƶ
	RTC_InitStruct.RTC_SynchPrediv=256-1;//ͬ����Ƶ
	RTC_Init(&RTC_InitStruct);
	
	//����ʱ��
	RTC_TimeTypeDef RTC_TimeStruct;
	//RTC_TimeStruct.RTC_H12=RTC_H12_AM;	//12Сʱ��ʹ��
	RTC_TimeStruct.RTC_Hours=17;
	RTC_TimeStruct.RTC_Minutes=00;
	RTC_TimeStruct.RTC_Seconds=00;
	RTC_SetTime(RTC_Format_BIN, &RTC_TimeStruct);
	
	//��������
	RTC_DateTypeDef RTC_DateStruct;
	RTC_DateStruct.RTC_WeekDay = RTC_Weekday_Sunday;
	RTC_DateStruct.RTC_Date = 24;
	RTC_DateStruct.RTC_Month = RTC_Month_November;
	RTC_DateStruct.RTC_Year = 19;					//��2000�꿪ʼ
	RTC_SetDate(RTC_Format_BIN, &RTC_DateStruct);

	RTC_WriteBackupRegister(RTC_BKP_DR0,22);
}

void Alarm_init(void)
{
	RTC_AlarmCmd(RTC_Alarm_A,DISABLE);

	RTC_AlarmTypeDef RTC_AlarmStruct;
	RTC_AlarmStruct.RTC_AlarmTime.RTC_H12 = RTC_H12_AM;
	RTC_AlarmStruct.RTC_AlarmTime.RTC_Hours = 18;
	RTC_AlarmStruct.RTC_AlarmTime.RTC_Minutes = 02;
	RTC_AlarmStruct.RTC_AlarmTime.RTC_Seconds = 0;
	RTC_AlarmStruct.RTC_AlarmDateWeekDay=24;
	RTC_AlarmStruct.RTC_AlarmDateWeekDaySel=RTC_AlarmDateWeekDaySel_Date;
	//RTC_AlarmStruct.RTC_AlarmMask	
	RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStruct);
	
	RTC_AlarmCmd(RTC_Alarm_A,ENABLE);
	RTC_ITConfig(RTC_IT_ALRA,ENABLE);
	
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line 		= EXTI_Line17;
	EXTI_InitStruct.EXTI_LineCmd	= ENABLE;
	EXTI_InitStruct.EXTI_Mode 		= EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger 	= EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStruct);
	
	//4. �����ж����ȼ�
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel 			= RTC_Alarm_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd 			= ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority 	= 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority 		= 1;
	NVIC_Init(&NVIC_InitStruct);
}


void RTC_Alarm_IRQHandler()
{
	printf("RTC IT\r\n");
	RTC_ClearITPendingBit(RTC_IT_ALRA);
	
	if( EXTI_GetITStatus( EXTI_Line17 ) == SET )
	{
					
		EXTI_ClearITPendingBit( EXTI_Line17 );
	}
}


