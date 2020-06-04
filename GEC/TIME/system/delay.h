#ifndef  __DELAY_H
#define  __DELAY_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void TimingDelay_Decrement(void);


void delay_init( void );
void Delay(__IO uint32_t nTime);
void delay_ms( __IO uint32_t nms );



#endif

