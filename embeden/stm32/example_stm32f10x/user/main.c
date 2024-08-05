#include "stm32f10x.h"

#include "ff.h"

#include "bsp_led.h"
#include "bsp_exti.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
#include "bsp_dma.h"
#include "bsp_i2c.h"



#define SOFT_DELAY Delay(0x0FFFFF);

void Delay(__IO u32 nCount); 



void led_exti_test(){
	led_gpio_config();
	key_exti_config();

  while(1);
}


///隔一定时间亮灯
void systick_test(){
	led_gpio_config();
	systick_init();

	while(1){
		delay_s(3);
			LED1_TOGGLE;
		delay_s(3);
			LED2_TOGGLE;
		delay_s(3);
			LED3_TOGGLE;
	}
	
}


void usart_test_send_str(){
	sdy_usart_config();
	
	sdy_usart_send_string(SDY_USART,"串口测试\n");
	printf("我是stm32f103\n");
	
	while(1);
}



///DMA 存储-》存储测试
static const uint32_t dma_m_m_src[32]= {
                                    0x01020304,0x05060708,0x090A0B0C,0x0D0E0F10,
                                    0x11121314,0x15161718,0x191A1B1C,0x1D1E1F20,
                                    0x21222324,0x25262728,0x292A2B2C,0x2D2E2F30,
                                    0x31323334,0x35363738,0x393A3B3C,0x3D3E3F40,
                                    0x41424344,0x45464748,0x494A4B4C,0x4D4E4F50,
                                    0x51525354,0x55565758,0x595A5B5C,0x5D5E5F60,
                                    0x61626364,0x65666768,0x696A6B6C,0x6D6E6F70,
                                    0x71727374,0x75767778,0x797A7B7C,0x7D7E7F80};

static uint32_t dma_m_m_dst[32];																		
																	
																		
																		
void dma_test_m_m(){
	
  systick_init();
	led_gpio_config();
	
	LED_PURPLE //亮紫色灯
	delay_s(3) ;//延时3s
	///DMA传输
	sdy_dma_m_m_config(dma_m_m_src,dma_m_m_dst,32);
	///等待
	while(DMA_GetFlagStatus(SDY_DMA_M_M_FLAG_TC) == RESET);

	///比较传输的结果
  if(sdy_buffer_cmp(dma_m_m_src,dma_m_m_dst,32)){
		LED_WHITE;
	}else{
		LED_RED;
	}

	while(1);
}



uint8_t sstr[101];

void dma_test_m_p(){
	
	for(uint32_t i = 0; i< 100 ;++i){			
		sstr[i] = 'P';
	}
	sstr[100] = 0x00;
	
	systick_init(); //开启时钟
	led_gpio_config(); //开启LED
  
	sdy_usart_config(); //串口配置
	sdy_dma_m_p_usart_config((const uint32_t*)sstr,100);
	
	///串口请求DMA发送数据
	USART_DMACmd(SDY_USART,USART_DMAReq_Tx,ENABLE);
	
	
	///CPU闪灯
	while(1){
		LED1_TOGGLE
		delay_s(3);
	}
}

void dma_test_p_m(){
	
	for(uint32_t i = 0; i< 101 ;++i){			
		sstr[i] = '\0';
	}
	
	systick_init(); //开启时钟
	led_gpio_config(); //开启LED
  
	sdy_usart_config(); //串口配置
	sdy_dma_p_m_usart_config((uint32_t*)sstr,100);
	
	///串口请求DMA发送数据
	USART_DMACmd(SDY_USART,USART_DMAReq_Rx,ENABLE);
	
	
	///CPU闪灯
	while(1){
		LED1_TOGGLE
		delay_s(10);
		sdy_usart_send_string(SDY_USART,(char*)sstr);
	}
}


uint8_t I2c_Buf_Write[256];
uint8_t I2c_Buf_Read[256];


uint8_t I2C_Test(void)
{
	//启用串口调试信息输出
	sdy_usart_config();
	
	///配置I2C
	i2c_config();
	
	sdy_i2c_ee_writePage(I2c_Buf_Write,8,0x00);
	
	uint16_t i;
	printf("data writen into eeprom\n\r");
	for ( i=0; i<=255; i++ ) //填充缓冲
  {   
    I2c_Buf_Write[i] = i;

    printf("0x%02X ", I2c_Buf_Write[i]);
    if(i%16 == 15)    
        printf("\n\r");    
   }

  //将I2c_Buf_Write中顺序递增的数据写入EERPOM中 
	sdy_i2c_ee_writeBuffer( I2c_Buf_Write, 256 , 0x00);
  
  SDY_I2C_INFO("write success\n\r");
   
  SDY_I2C_INFO("read data\n\r");
	 
  //将EEPROM读出数据顺序保持到I2c_Buf_Read中
	sdy_i2c_ee_readBuffer(I2c_Buf_Read, 0x00, 256); 
   
  //将I2c_Buf_Read中的数据通过串口打印
	for (i=0; i<256; i++)
	{	
		if(I2c_Buf_Read[i] != I2c_Buf_Write[i])
		{
			SDY_I2C_ERROR("0x%02X \n\r", I2c_Buf_Read[i]);
			SDY_I2C_ERROR("error:I2C EEPROM data is not same\n\r");
			return 0;
		}
    printf("0x%02X ", I2c_Buf_Read[i]);
    if(i%16 == 15)    
        printf("\n\r");
    
	}
  SDY_I2C_INFO("I2C(AT24C02)test success\n\r");
  
  while(1);
}


/// @brief flash 测试
extern sdy_spi_flash_test(void);




///fatfs 测试
FATFS fs;													/* FatFs文件系统对象 */
FIL fnew;													/* 文件对象 */
FRESULT res_flash;                /* 文件操作结果 */
UINT fnum;            					  /* 文件成功读写数量 */
BYTE ReadBuffer[1024]={0};        /* 读缓冲区 */
BYTE WriteBuffer[] =              /* 写缓冲区*/
"pan jia yi tuo ku zi\r\n"; 


void sdy_fatfs_file_test(){

	/*----------------------- 文件系统测试：写测试 -------------------*/
	/* 打开文件，每次都以新建的形式打开，属性为可写 */
	printf("\r\n****** file write test ******\r\n");	
	res_flash = f_open(&fnew, "1:FatFsTest.txt",FA_CREATE_ALWAYS | FA_WRITE );
	if ( res_flash == FR_OK )
	{
		printf(">open/create FatFsTest.txt success,writing data into it now.\r\n");
    /* 将指定存储区内容写入到文件内 */
		res_flash=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
    if(res_flash==FR_OK)
    {
      printf(">write success,write data:%d\n",fnum);
      printf(">data:\r\n%s\r\n",WriteBuffer);
    }
    else
    {
      printf("！！write failed:(%d)\n",res_flash);
    }    
		/* 不再读写，关闭文件 */
    f_close(&fnew);
	}
	else
	{	
		LED_RED;
		printf(">open/create file failed \r\n");
	}
	
/*------------------- 文件系统测试：读测试 --------------------------*/
	printf("****** file read test... ******\r\n");
	res_flash = f_open(&fnew, "1:FatFsTest.txt",FA_OPEN_EXISTING | FA_READ); 	 
	if(res_flash == FR_OK)
	{
		LED_GREEN;
		printf(">open file success.\r\n");
		res_flash = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum); 
    if(res_flash==FR_OK)
    {
      printf(">file read success,data: %d\r\n",fnum);
      printf(">read data:\r\n%s \r\n", ReadBuffer);	
    }
    else
    {
      printf("read file failed:(%d)\n",res_flash);
    }		
	}
	else
	{
		LED_RED;
		printf("open file failed.\r\n");
	}
	/* 不再读写，关闭文件 */
	f_close(&fnew);	

}


extern void sdy_spi_falsh_config(void);
extern void sdy_spi_flash_erase_chip(void);

int sdy_fatfs_test(void)
{
	/* 初始化LED */
	led_gpio_config();	
	LED_BLUE;
	
	/* 初始化调试串口，一般为串口1 */
	sdy_usart_config();	
  
	///先把flash全片擦除
	printf("now erase chip\r\n");
	sdy_spi_falsh_config();
	sdy_spi_flash_erase_chip();
	printf("erase chip success\r\n");

	
	//在外部SPI Flash挂载文件系统，文件系统挂载时会对SPI设备初始化
	//初始化函数调用流程如下
	//f_mount()->find_volume()->disk_initialize->SPI_FLASH_Init()
	res_flash = f_mount(&fs,"1:",1);
	
  //----------------------- 格式化测试 ------ 
	// 如果没有文件系统就格式化创建创建文件系统 
	if(res_flash == FR_NO_FILESYSTEM)
	{
		printf("> FLASH no file system , will create one...\r\n");
    // 格式化 
		res_flash=f_mkfs("1:",0,0);							
		
		if(res_flash == FR_OK)
		{
			printf("> FLASH create file system success. \r\n");
      // 格式化后，先取消挂载 
			res_flash = f_mount(NULL,"1:",1);			
      // 重新挂载			
			res_flash = f_mount(&fs,"1:",1);
		}
		else
		{
			LED_RED;
			printf("> create system failed \r\n");
			while(1);
		}
	}
  else if(res_flash!=FR_OK)
  {
    printf("> External Flash mounted file system failed. (%d)\r\n",res_flash);
    printf("> Possible reason: SPI Flash initialization failed. \r\n");
		while(1);
  }
  else
  {
    printf("> The file system has been successfully mounted and can be tested for read and write operations\r\n");
  }
  
	
	
	///文件测试
	sdy_fatfs_file_test();



  
	/* 不再使用文件系统，取消挂载文件系统 */
	f_mount(NULL,"1:",1);
  
  /* 操作完成，停机 */
	while(1)
	{
	}
}





int main(){
	
	sdy_fatfs_test();
	
	return 0;
}


void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}