#ifndef __BSP_DMA_H
#define __BSP_DMA_H


#include "stm32f10x.h"



#define SDY_DMA_M_M_CHANAL 		DMA1_Channel6
#define SDY_DMA_M_M_CLOCK			RCC_AHBPeriph_DMA1
#define SDY_DMA_M_M_FLAG_TC		DMA1_FLAG_TC6

///m-p
#define SDY_DMA_M_P_CHANNAL		DMA1_Channel4
#define SDY_DMA_M_P_CLOCK			RCC_AHBPeriph_DMA1

///p-m
#define SDY_DMA_P_M_CHANNAL		DMA1_Channel5
#define SDY_DMA_P_M_CLOCK			RCC_AHBPeriph_DMA1


// 外设寄存器地址
#define  USART_DR_ADDRESS        (USART1_BASE+0x04)


///存储器- 存储器实验配置
void sdy_dma_m_m_config(const uint32_t* src, uint32_t* dst, uint32_t size);

///存储器- 串口实验配置
void sdy_dma_m_p_usart_config(const uint32_t* src,uint32_t size);

///串口 - 存储器 实验配置
void sdy_dma_p_m_usart_config(uint32_t* dst,uint32_t size);


uint8_t  sdy_buffer_cmp(const uint32_t* b1, uint32_t* b2,uint16_t size);






#endif