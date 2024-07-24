#include "bsp_systick.h"

#include "core_cm3.h"
#include "bsp_led.h"


static uint32_t counter = 0;



void systick_init(){
	//配置中断频率为 1ms
	if(0 !=  SysTick_Config(SystemCoreClock/SDY_SYSTICK_MS)){
		while(1);
	}
	disableSystick(); //先关闭计时
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



// couter 减1的时间 等于 1/systick_clk
// 当counter 从 reload 的值减小到0的时候，为一个循环，如果开启了中断则执行中断服务程序，
// 同时 CTRL 的 countflag 位会置1
// 这一个循环的时间为 reload * (1/systick_clk)

void SysTick_Delay_Us( __IO uint32_t us)
{
	uint32_t i;
	SysTick_Config(SystemCoreClock/1000000);
	
	for(i=0;i<us;i++)
	{
		// 当计数器的值减小到0的时候，CRTL寄存器的位16会置1	
		while( !((SysTick->CTRL)&(1<<16)) );
	}
	// 关闭SysTick定时器
	SysTick->CTRL &=~SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Delay_Ms( __IO uint32_t ms)
{
	uint32_t i;	
	SysTick_Config(SystemCoreClock/1000);
	
	for(i=0;i<ms;i++)
	{
		// 当计数器的值减小到0的时候，CRTL寄存器的位16会置1
		// 当置1时，读取该位会清0
		while( !((SysTick->CTRL)&(1<<16)) );
	}
	// 关闭SysTick定时器
	SysTick->CTRL &=~ SysTick_CTRL_ENABLE_Msk;
}


