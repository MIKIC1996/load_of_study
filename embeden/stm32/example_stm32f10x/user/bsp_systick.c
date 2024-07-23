#include "bsp_systick.h"

#include "core_cm3.h"
#include "bsp_led.h"


static uint32_t counter = 0;



void systick_init(){
	//配置中断频率为 1ms
	if(0 !=  SysTick_Config(SystemCoreClock/SDY_SYSTICK_MS)){
		while(1);
	}
}


//使能定时器
void enableSystick(){
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;	
}

//关闭定时器
void disableSystick(){
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}


///每次中断调用这个函数
void countByInq(){
	if(counter != 0x00)
		--counter;
}


///利用定时器和中断写延时函数
///延时多少秒
void delay_s(uint32_t s){
	counter = s * 1000;
	enableSystick();
	while(counter!=0);
	disableSystick();
}


///延时多少秒
void delay_ms(uint32_t ms){
	counter = ms;
	enableSystick();
	counter = ms;
	disableSystick();
}



