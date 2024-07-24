#include "bsp_key.h"


void key_gpio_config(){
	RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK | KEY2_GPIO_CLK , ENABLE);
	
	GPIO_InitTypeDef init;
	init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	init.GPIO_Pin = KEY1_GPIO_PIN;
	GPIO_Init(KEY1_GPIO_PORT,&init);
	init.GPIO_Pin = KEY2_GPIO_PIN;
	GPIO_Init(KEY2_GPIO_PORT,&init);
}


uint8_t key_scan(GPIO_TypeDef* port,uint16_t pin){
	if(GPIO_ReadInputDataBit(port,pin) == KEY_ON){
		while(GPIO_ReadInputDataBit(port,pin) == KEY_ON); //等用户释放按键
		return KEY_ON;
	}else{
		return KEY_OFF;
	}

}