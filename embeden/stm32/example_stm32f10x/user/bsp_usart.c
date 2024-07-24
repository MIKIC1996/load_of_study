#include "bsp_usart.h"


///����NVIC�� USART ���ȼ�
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
	
	///NVIC���ȼ�����
	nvic_usart_config();
	
	//�򿪴��ں� ����GPIO��ʱ��
	SDY_USART_APB_X_CLKCMD(SDY_USART_CLK,ENABLE);
	SDY_USART_GPIO_APB_X_CLKCMD(SDY_USART_GPIO_CLK,ENABLE);
	
	///GPIO ��ʼ��
	GPIO_InitTypeDef ginit;
	ginit.GPIO_Mode = GPIO_Mode_AF_PP; //TXʹ�����츴��ģʽ
	ginit.GPIO_Speed = GPIO_Speed_50MHz;
	ginit.GPIO_Pin = SDY_USART_TX_GPIO_PIN;
	GPIO_Init(SDY_USART_TX_GPIO_PORT ,&ginit);
	
	ginit.GPIO_Mode = GPIO_Mode_IN_FLOATING; //RXʹ�ø�������
	ginit.GPIO_Pin = SDY_USART_RX_GPIO_PIN;
	GPIO_Init(SDY_USART_RX_GPIO_PORT, &ginit);
	
	///��������
	USART_InitTypeDef uinit;
	///������
	uinit.USART_BaudRate = SDY_USART_BAUDRATE;
	///֡�����ݳ�
	uinit.USART_WordLength = USART_WordLength_8b;
	///ֹͣλ����
	uinit.USART_StopBits = USART_StopBits_1;
	///У��λ
	uinit.USART_Parity = USART_Parity_No;
	///Ӳ��������
	uinit.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	///���ù���ģʽ
	uinit.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_Init(SDY_USART,&uinit);
	
	///���ô����ж�,׼���ö���ʱ������ж�
	USART_ITConfig(SDY_USART,USART_IT_RXNE,ENABLE);
	
	///����ʹ��
	USART_Cmd(SDY_USART,ENABLE);
	
}


void sdy_usart_send_byte(USART_TypeDef* usart, uint8_t ch){
	USART_SendData(SDY_USART,ch);
	/// �ȴ��������ݼĴ���Ϊ�� ��TXE �� 1
	while(USART_GetFlagStatus(SDY_USART,USART_FLAG_TXE) == RESET);
}

void sdy_usart_send_string(USART_TypeDef* usart,char* str){
	
	uint32_t k = 0;
	do{
		sdy_usart_send_byte(usart, *(str + k));
		++k;
	}while(*(str + k) != '\0');
	
	///�ȴ��������
	while(USART_GetFlagStatus(usart,USART_FLAG_TC) == RESET);
}



void sdy_usart_send_half_word(USART_TypeDef* usart,uint16_t ch){
	uint8_t temp_h, temp_l;
	
	/* ȡ���߰�λ */
	temp_h = (ch&0XFF00)>>8;
	/* ȡ���Ͱ�λ */
	temp_l = ch&0XFF;
	
	/* ���͸߰�λ */
	USART_SendData(usart,temp_h);	
	while (USART_GetFlagStatus(usart, USART_FLAG_TXE) == RESET);
	
	/* ���͵Ͱ�λ */
	USART_SendData(usart,temp_l);	
	while (USART_GetFlagStatus(usart, USART_FLAG_TXE) == RESET);	
}



///�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ����� */
		USART_SendData(SDY_USART, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(SDY_USART, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		/* �ȴ������������� */
		while (USART_GetFlagStatus(SDY_USART, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(SDY_USART);
}









