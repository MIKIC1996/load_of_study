#include "bsp_exti.h"

#include "bsp_key.h"


static void NVIC_config(){
	///NVIC的优先级模式设置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	///设置EXTI,使能+设置优先级
	NVIC_InitTypeDef init;
	init.NVIC_IRQChannelPreemptionPriority = 1;
	init.NVIC_IRQChannelSubPriority = 1;
	init.NVIC_IRQChannelCmd = ENABLE;
	
	init.NVIC_IRQChannel =  KEY1_EXTI_IRQ; //EXTI0_IRQn
	NVIC_Init(&init);
	init.NVIC_IRQChannel = KEY2_EXTI_IRQ;
	NVIC_Init(&init);
}




void key_exti_config(){
	///配置nvic
	NVIC_config();
	
	//开启外设时钟 GPIO + AFIO
	RCC_APB2PeriphClockCmd(KEY1_EXTI_CLK,ENABLE);
	RCC_APB2PeriphClockCmd(KEY2_EXTI_CLK,ENABLE);
	
	//GPIO设置
	GPIO_InitTypeDef ginit;
	ginit.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	ginit.GPIO_Pin = KEY1_GPIO_PIN;
	GPIO_Init(KEY1_GPIO_PORT,&ginit);
	ginit.GPIO_Pin = KEY2_GPIO_PIN;
	GPIO_Init(KEY2_GPIO_PORT,&ginit);
	
	//AFIO 设置 
	GPIO_EXTILineConfig(KEY1_EXTI_PORT,KEY1_EXTI_PIN); //启用GPIO pin作为EXTI
	GPIO_EXTILineConfig(KEY2_EXTI_PORT,KEY2_EXTI_PIN); 
	
	EXTI_InitTypeDef einit;
	einit.EXTI_Mode = EXTI_Mode_Interrupt; //中断模式
	einit.EXTI_LineCmd = ENABLE;
	
	einit.EXTI_Line = KEY1_EXTI_LINE;
	einit.EXTI_Trigger = EXTI_Trigger_Rising; //key1上升沿中断
	EXTI_Init(&einit);
	
	einit.EXTI_Line = KEY2_EXTI_LINE;
	einit.EXTI_Trigger = EXTI_Trigger_Falling; //key2下降沿中断
	EXTI_Init(&einit);
	

	

}








