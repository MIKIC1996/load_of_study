#include "bsp_usart.h"


///配置NVIC的 USART 优先级
static void nvic_usart_config(){
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef init;
	init.NVIC_IRQChannel = SDY_USART_IRQ;
	init.NVIC_IRQChannelPreemptionPriority=1;
	init.NVIC_IRQChannelSubPriority = 1;
	init.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&init);
}


void sdy_usart_config(){
	
	///NVIC优先级配置
	nvic_usart_config();
	
	//打开串口和 复用GPIO的时钟
	SDY_USART_APB_X_CLKCMD(SDY_USART_CLK,ENABLE);
	SDY_USART_GPIO_APB_X_CLKCMD(SDY_USART_GPIO_CLK,ENABLE);
	
	///GPIO 初始化
	GPIO_InitTypeDef ginit;
	ginit.GPIO_Mode = GPIO_Mode_AF_PP; //TX使用推挽复用模式
	ginit.GPIO_Speed = GPIO_Speed_50MHz;
	ginit.GPIO_Pin = SDY_USART_TX_GPIO_PIN;
	GPIO_Init(SDY_USART_TX_GPIO_PORT ,&ginit);
	
	ginit.GPIO_Mode = GPIO_Mode_IN_FLOATING; //RX使用浮空输入
	ginit.GPIO_Pin = SDY_USART_RX_GPIO_PIN;
	GPIO_Init(SDY_USART_RX_GPIO_PORT, &ginit);
	
	///串口配置
	USART_InitTypeDef uinit;
	///波特率
	uinit.USART_BaudRate = SDY_USART_BAUDRATE;
	///帧的数据长
	uinit.USART_WordLength = USART_WordLength_8b;
	///停止位长度
	uinit.USART_StopBits = USART_StopBits_1;
	///校验位
	uinit.USART_Parity = USART_Parity_No;
	///硬件流控制
	uinit.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	///配置工作模式
	uinit.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_Init(SDY_USART,&uinit);
	
	///配置串口中断,准备好读的时候出发中断
	USART_ITConfig(SDY_USART,USART_IT_RXNE,ENABLE);
	
	///串口使能
	USART_Cmd(SDY_USART,ENABLE);
	
}


void sdy_usart_send_byte(USART_TypeDef* usart, uint8_t ch){
	USART_SendData(SDY_USART,ch);
	/// 等待发送数据寄存器为空 ，TXE 置 1
	while(USART_GetFlagStatus(SDY_USART,USART_FLAG_TXE) == RESET);
}

void sdy_usart_send_string(USART_TypeDef* usart,char* str){
	
	uint32_t k = 0;
	do{
		sdy_usart_send_byte(usart, *(str + k));
		++k;
	}while(*(str + k) != '\0');
	
	///等待发送完成
	while(USART_GetFlagStatus(usart,USART_FLAG_TC) == RESET);
}



void sdy_usart_send_half_word(USART_TypeDef* usart,uint16_t ch){
	uint8_t temp_h, temp_l;
	
	/* 取出高八位 */
	temp_h = (ch&0XFF00)>>8;
	/* 取出低八位 */
	temp_l = ch&0XFF;
	
	/* 发送高八位 */
	USART_SendData(usart,temp_h);	
	while (USART_GetFlagStatus(usart, USART_FLAG_TXE) == RESET);
	
	/* 发送低八位 */
	USART_SendData(usart,temp_l);	
	while (USART_GetFlagStatus(usart, USART_FLAG_TXE) == RESET);	
}



///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口 */
		USART_SendData(SDY_USART, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(SDY_USART, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		/* 等待串口输入数据 */
		while (USART_GetFlagStatus(SDY_USART, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(SDY_USART);
}









