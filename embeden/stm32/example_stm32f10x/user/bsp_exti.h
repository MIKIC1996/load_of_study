#ifndef __BSP_EXTI_H
#define __BSP_EXTI_H


#include "stm32f10x.h"





///NVIC marco
#define KEY1_EXTI_IRQ		EXTI0_IRQn
#define KEY2_EXTI_IRQ		EXTI15_10_IRQn



///EXTI marco
#define KEY1_EXTI_PORT		GPIO_PortSourceGPIOA
#define KEY1_EXTI_PIN			GPIO_PinSource0
#define KEY1_EXTI_LINE		EXTI_Line0 //用0号线
#define KEY1_EXTI_CLK			(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO)

#define KEY2_EXTI_PORT		GPIO_PortSourceGPIOC
#define KEY2_EXTI_PIN			GPIO_PinSource13
#define KEY2_EXTI_LINE		EXTI_Line13 //用13号线
#define KEY2_EXTI_CLK			(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO)

///handle marco
#define KEY1_IRQHandler            EXTI0_IRQHandler
#define KEY2_IRQHandler            EXTI15_10_IRQHandler


void key_exti_config();







#endif