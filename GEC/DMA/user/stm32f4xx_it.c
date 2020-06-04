/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    04-August-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "delay.h"
#include "BitBandOP.h"
#include "key.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  TimingDelay_Decrement();
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @brief  exti0 线中断--》 S1 PA0 按键1
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler( void  )
{
	key_det(); //消除按下抖动
	
	if( EXTI_GetITStatus( EXTI_Line0 ) == SET )
	{
		PFOut(8) ^= 1;			
		EXTI_ClearITPendingBit( EXTI_Line0 );
	}

}





/**
  * @brief  exti2 线中断--》 S2 PE2 按键2
  * @param  None
  * @retval None
  */
void EXTI2_IRQHandler( void  )
{
	key_det(); //消除按下抖动
	
	if( EXTI_GetITStatus( EXTI_Line2 ) == SET )
	{
		PFOut(10) ^= 1;			
		EXTI_ClearITPendingBit( EXTI_Line2 );
	}

}



/**
  * @brief  exti3 线中断--》 S3 PE3 按键3
  * @param  None
  * @retval None
  */
void EXTI3_IRQHandler( void  )
{
	key_det(); //消除按下抖动
	
/*
	if( EXTI_GetITStatus( EXTI_Line3 ) == SET )
	{
		PEOut(13) ^= 1;			
		EXTI_ClearITPendingBit( EXTI_Line3 );
	}
*/
	/*寄存器方式*/
	 if ((EXTI->PR & EXTI_Line3) != RESET)
	 {
		PEOut(13) ^= 1;			
		EXTI->PR = EXTI_Line3;
	 }
	
	
}

/**
  * @brief  exti4 线中断--》 S4 PE4 按键4
  * @param  None
  * @retval None
  */
void EXTI4_IRQHandler( void  )
{
	//key_det(); //消除按下抖动
	delay_ms( 20 );
	while( !S4 ){}
	delay_ms( 20 );
	
	if( EXTI_GetITStatus( EXTI_Line4 ) == SET )
	{
		PEOut(14) ^= 1;			
		EXTI_ClearITPendingBit( EXTI_Line4 );
	}

}

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
