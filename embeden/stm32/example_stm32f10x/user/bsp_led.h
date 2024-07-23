#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "stm32f10x.h"




#define LED1_GPIO_PORT	    GPIOB
#define LED1_GPIO_CLK		RCC_APB2Periph_GPIOB
#define LED1_GPIO_PIN		GPIO_Pin_5


#define LED2_GPIO_PORT	    GPIOB
#define LED2_GPIO_CLK		RCC_APB2Periph_GPIOB
#define LED2_GPIO_PIN		GPIO_Pin_0


#define LED3_GPIO_PORT	    GPIOB
#define LED3_GPIO_CLK		RCC_APB2Periph_GPIOB
#define LED3_GPIO_PIN		GPIO_Pin_1


#define LED_ON 0 //低电平开灯
#define LED_OFF 1


#define LED1_TOGGLE		 toggle_led(LED1_GPIO_PORT,LED1_GPIO_PIN);
#define LED1_OFF		   close_led(LED1_GPIO_PORT,LED1_GPIO_PIN);
#define LED1_ON			   open_led(LED1_GPIO_PORT,LED1_GPIO_PIN);

#define LED2_TOGGLE		 toggle_led(LED2_GPIO_PORT,LED2_GPIO_PIN);
#define LED2_OFF		   close_led(LED2_GPIO_PORT,LED2_GPIO_PIN);
#define LED2_ON			   open_led(LED2_GPIO_PORT,LED2_GPIO_PIN);

#define LED3_TOGGLE		 toggle_led(LED3_GPIO_PORT,LED3_GPIO_PIN);
#define LED3_OFF		   close_led(LED3_GPIO_PORT,LED3_GPIO_PIN);
#define LED3_ON			   open_led(LED3_GPIO_PORT,LED3_GPIO_PIN);

/* 基本混色，后面高级用法使用PWM可混出全彩颜色,且效果更好 */

//红
#define LED_RED  \
					LED1_ON;\
					LED2_OFF\
					LED3_OFF

//绿
#define LED_GREEN		\
					LED1_OFF;\
					LED2_ON\
					LED3_OFF

//蓝
#define LED_BLUE	\
					LED1_OFF;\
					LED2_OFF\
					LED3_ON

					
//黄(红+绿)					
#define LED_YELLOW	\
					LED1_ON;\
					LED2_ON\
					LED3_OFF
//紫(红+蓝)
#define LED_PURPLE	\
					LED1_ON;\
					LED2_OFF\
					LED3_ON

//青(绿+蓝)
#define LED_CYAN \
					LED1_OFF;\
					LED2_ON\
					LED3_ON
					
//白(红+绿+蓝)
#define LED_WHITE	\
					LED1_ON;\
					LED2_ON\
					LED3_ON
					
//黑(全部关闭)
#define LED_RGBOFF	\
					LED1_OFF;\
					LED2_OFF\
					LED3_OFF



/// @LED gpio初始化
void led_gpio_config();



void open_led(GPIO_TypeDef* port,uint16_t pin);

void close_led(GPIO_TypeDef* port,uint16_t pin);

uint8_t is_led_open(GPIO_TypeDef* port,uint16_t pin);

void toggle_led(GPIO_TypeDef* port,uint16_t pin);


#endif