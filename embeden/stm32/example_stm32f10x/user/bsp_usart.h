#ifndef __BSP_USART_H
#define __BSP_USART_H

#include "stm32f10x.h"
#include <stdio.h>

/** 
  * ���ں궨�壬��ͬ�Ĵ��ڹ��ص����ߺ�IO��һ������ֲʱ��Ҫ�޸��⼸����
	* 1-�޸�����ʱ�ӵĺ꣬uart1���ص�apb2���ߣ�����uart���ص�apb1����
	* 2-�޸�GPIO�ĺ�
  */
#define SDY_USART								USART1
#define SDY_USART_CLK						RCC_APB2Periph_USART1
#define SDY_USART_APB_X_CLKCMD	RCC_APB2PeriphClockCmd
#define SDY_USART_BAUDRATE			115200

#define SDY_USART_GPIO_CLK						RCC_APB2Periph_GPIOA
#define SDY_USART_GPIO_APB_X_CLKCMD     RCC_APB2PeriphClockCmd

///USART ���õ�GPIO
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