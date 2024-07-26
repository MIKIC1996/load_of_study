#include "bsp_dma.h"




void sdy_dma_m_m_config(const uint32_t* src, uint32_t* dst, uint32_t size ){
	DMA_InitTypeDef init;
	///开启DMA时钟
	RCC_AHBPeriphClockCmd(SDY_DMA_M_M_CLOCK,ENABLE);
	
	
	/// 数据源,虽然是m_m，依然把地址写在外设地址
	init.DMA_PeripheralBaseAddr = (uint32_t)src;
	/// 目标地址
	init.DMA_MemoryBaseAddr = (uint32_t)dst;
	/// 方向,外设为源地址
	init.DMA_DIR = DMA_DIR_PeripheralSRC;
	/// 传输大小
	init.DMA_BufferSize = size;
	/// 外设和存储 的地址都递增
	init.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	init.DMA_MemoryInc = DMA_MemoryInc_Enable;
	/// 数据单位，全部采用4字节，如果src 单位 长于 dst长度，则数据过去会高位丢失
	/// 如果 src 单位 短于 dst长度，src数据占据 dst的低位，高位保持为0，不会数据丢失
	init.DMA_PeripheralDataSize  = DMA_PeripheralDataSize_Word;
	init.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	/// 模式，非循环
	init.DMA_Mode = DMA_Mode_Normal;
	/// 优先级 高
	init.DMA_Priority = DMA_Priority_High;
	/// 使能 m->m 存储到存储
	init.DMA_M2M = DMA_M2M_Enable;
	
	///初始化通道
	DMA_Init(SDY_DMA_M_M_CHANAL,&init);
	///清除DMA 数据流传输完成标志位
	DMA_ClearFlag(SDY_DMA_M_M_FLAG_TC);
	///最后使能DMA
	DMA_Cmd(SDY_DMA_M_M_CHANAL,ENABLE);
	
}



void sdy_dma_m_p_usart_config(const uint32_t src,uint32_t size){

	
	
}



uint8_t  sdy_buffer_cmp(const uint32_t* b1, uint32_t* b2,uint16_t size){

	while(size--){
		if(*b1 != *b2)
			return 0;
		b1++;
		b2++;
	}
	return 1;
}

