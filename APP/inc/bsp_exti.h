#ifndef __EXTI_H
#define	__EXTI_H

#include "stm32f4xx.h"

//Òý½Å¶¨Òå
/*******************************************************/
#define SR501_INT_GPIO_PORT                GPIOF
#define SR501_INT_GPIO_CLK                 RCC_AHB1Periph_GPIOF
#define SR501_INT_GPIO_PIN                 GPIO_Pin_7
#define SR501_INT_EXTI_PORTSOURCE          EXTI_PortSourceGPIOF
#define SR501_INT_EXTI_PINSOURCE           EXTI_PinSource7
#define SR501_INT_EXTI_LINE                EXTI_Line7
#define SR501_INT_EXTI_IRQ                 EXTI9_5_IRQn

#define SR501_IRQHandler                   EXTI9_5_IRQHandler


/*******************************************************/


void EXTI_SR501_Config(void);

#endif /* __EXTI_H */
