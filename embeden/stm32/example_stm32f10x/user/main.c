#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_exti.h"
#include "bsp_systick.h"


#define SOFT_DELAY Delay(0x0FFFFF);

void Delay(__IO u32 nCount); 



void led_exti_test(){
	led_gpio_config();
	key_exti_config();

		while (1)
	{

	}
	
}


///��һ��ʱ������
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



int main(){
	
	systick_test();
	
	return 0;
}


void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}