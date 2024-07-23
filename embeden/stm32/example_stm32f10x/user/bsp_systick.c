#include "bsp_systick.h"

#include "core_cm3.h"
#include "bsp_led.h"


static uint32_t counter = 0;



void systick_init(){
	//�����ж�Ƶ��Ϊ 1ms
	if(0 !=  SysTick_Config(SystemCoreClock/SDY_SYSTICK_MS)){
		while(1);
	}
}


//ʹ�ܶ�ʱ��
void enableSystick(){
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;	
}

//�رն�ʱ��
void disableSystick(){
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}


///ÿ���жϵ����������
void countByInq(){
	if(counter != 0x00)
		--counter;
}


///���ö�ʱ�����ж�д��ʱ����
///��ʱ������
void delay_s(uint32_t s){
	counter = s * 1000;
	enableSystick();
	while(counter!=0);
	disableSystick();
}


///��ʱ������
void delay_ms(uint32_t ms){
	counter = ms;
	enableSystick();
	counter = ms;
	disableSystick();
}



