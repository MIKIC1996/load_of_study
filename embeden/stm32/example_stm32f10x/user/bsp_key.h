#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#include "stm32f10x.h"


#define KEY1_GPIO_PORT	GPIOA
#define KEY1_GPIO_CLK		RCC_APB2Periph_GPIOA
#define	KEY1_GPIO_PIN		GPIO_Pin_0


#define KEY2_GPIO_PORT	GPIOC
#define KEY2_GPIO_CLK		RCC_APB2Periph_GPIOC
#define	KEY2_GPIO_PIN		GPIO_Pin_13


#define KEY_ON 		1
#define KEY_OFF 	0

///初始化操作
void key_gpio_config();

///检测按键
uint8_t key_scan(GPIO_TypeDef* port,uint16_t pin);


#endif