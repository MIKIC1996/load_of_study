#include "bsp_systick.h"

#include "core_cm3.h"
#include "bsp_led.h"


static uint32_t counter = 0;



void systick_init(){
	//�����ж�Ƶ��Ϊ 1ms
	if(0 !=  SysTick_Config(SystemCoreClock/SDY_SYSTICK_MS)){
		while(1);
	}
	disableSystick(); //�ȹرռ�ʱ
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



// couter ��1��ʱ�� ���� 1/systick_clk
// ��counter �� reload ��ֵ��С��0��ʱ��Ϊһ��ѭ��������������ж���ִ���жϷ������
// ͬʱ CTRL �� countflag λ����1
// ��һ��ѭ����ʱ��Ϊ reload * (1/systick_clk)

void SysTick_Delay_Us( __IO uint32_t us)
{
	uint32_t i;
	SysTick_Config(SystemCoreClock/1000000);
	
	for(i=0;i<us;i++)
	{
		// ����������ֵ��С��0��ʱ��CRTL�Ĵ�����λ16����1	
		while( !((SysTick->CTRL)&(1<<16)) );
	}
	// �ر�SysTick��ʱ��
	SysTick->CTRL &=~SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Delay_Ms( __IO uint32_t ms)
{
	uint32_t i;	
	SysTick_Config(SystemCoreClock/1000);
	
	for(i=0;i<ms;i++)
	{
		// ����������ֵ��С��0��ʱ��CRTL�Ĵ�����λ16����1
		// ����1ʱ����ȡ��λ����0
		while( !((SysTick->CTRL)&(1<<16)) );
	}
	// �ر�SysTick��ʱ��
	SysTick->CTRL &=~ SysTick_CTRL_ENABLE_Msk;
}


