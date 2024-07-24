#ifndef __BSP_USART_H
#define __BSP_USART_H

#include "stm32f10x.h"
#include <stdio.h>

/** 
  * 串口宏定义，不同的串口挂载的总线和IO不一样，移植时需要修改这几个宏
	* 1-修改总线时钟的宏，uart1挂载到apb2总线，其他uart挂载到apb1总线
	* 2-修改GPIO的宏
  */
#define SDY_USART								USART1
#define SDY_USART_CLK						RCC_APB2Periph_USART1
#define SDY_USART_APB_X_CLKCMD	RCC_APB2PeriphClockCmd
#define SDY_USART_BAUDRATE			115200

#define SDY_USART_GPIO_CLK						RCC_APB2Periph_GPIOA
#define SDY_USART_GPIO_APB_X_CLKCMD     RCC_APB2PeriphClockCmd

///USART 复用的GPIO
#define  SDY_USART_TX_GPIO_PORT       GPIOA   
#define  SDY_USART_TX_GPIO_PIN        GPIO_Pin_9
#define  SDY_USART_RX_GPIO_PORT       GPIOA
#define  SDY_USART_RX_GPIO_PIN        GPIO_Pin_10

#define  SDY_USART_IRQ                USART1_IRQn
#define  SDY_USART_IRQ_HANDLER        USART1_IRQHandler


void sdy_usart_config();
void sdy_usart_send_byte(USART_TypeDef* usart, uint8_t ch);
void sdy_usart_send_string(USART_TypeDef* usart,char* str);
void sdy_usart_send_half_word(USART_TypeDef* usart,uint16_t);



#endif