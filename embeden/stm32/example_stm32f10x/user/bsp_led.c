#include "bsp_led.h"



void led_gpio_config(){
  GPIO_InitTypeDef init;
  
	RCC_APB2PeriphClockCmd(LED1_GPIO_CLK | LED2_GPIO_CLK \
													|LED3_GPIO_CLK , ENABLE);
	
	init.GPIO_Mode = GPIO_Mode_Out_PP;
	init.GPIO_Speed = GPIO_Speed_50MHz;
	
	init.GPIO_Pin = LED1_GPIO_PIN;
	GPIO_Init(LED1_GPIO_PORT,&init);
	init.GPIO_Pin = LED2_GPIO_PIN;
	GPIO_Init(LED2_GPIO_PORT,&init);
	init.GPIO_Pin = LED3_GPIO_PIN;
	GPIO_Init(LED3_GPIO_PORT,&init);
	
	//先关灯
	GPIO_SetBits(LED1_GPIO_PORT,LED1_GPIO_PIN);
	GPIO_SetBits(LED2_GPIO_PORT,LED2_GPIO_PIN);
	GPIO_SetBits(LED3_GPIO_PORT,LED3_GPIO_PIN);
	
	
}


void open_led(GPIO_TypeDef* port,uint16_t pin){
	GPIO_ResetBits(port,pin); //输出低电平
}

void close_led(GPIO_TypeDef* port,uint16_t pin){
	GPIO_SetBits(port,pin);
}

uint8_t is_led_open(GPIO_TypeDef* port,uint16_t pin){
	return !( GPIO_ReadOutputData(port) & pin);
}

void toggle_led(GPIO_TypeDef* port,uint16_t pin){
	is_led_open(port,pin) ? close_led(port,pin) : open_led(port,pin);
}


