#ifndef __BSP_SYSTICK_H
#define __BSP_SYSTICK_H


#include "stm32f10x.h"


#define SDY_SYSTICK_MS 1000



void systick_init();

void enableSystick();

void disableSystick();

void countByInq();

void delay_s(uint32_t s);

void delay_ms(uint32_t ms);



void async_count_ms(uint32_t ms);

void async_count_s(uint32_t sec);

uint8_t check_time_out();


#endif