#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_exti.h"
#include "bsp_systick.h"
#include "bsp_usart.h"

#define SOFT_DELAY Delay(0x0FFFFF);

void Delay(__IO u32 nCount); 



void led_exti_test(){
	led_gpio_config();
	key_exti_config();

		while (1)
	{

	}
	
}


///隔一定时间亮灯
void systick_test(){
	led_gpio_config();
	systick_init();

	while(1){
		delay_s(3);
			LED1_TOGGLE;
		delay_s(3);
			LED2_TOGGLE;
		delay_s(3);
			LED3_TOGGLE;
	}
	
}


void usart_test_send_str(){
	sdy_usart_config();
	
	sdy_usart_send_string(SDY_USART,"串口测试\n");
	printf("我是stm32f103\n");
	
	while(1);
}


int main(){
	
	usart_test_send_str();
	
	return 0;
}


void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}