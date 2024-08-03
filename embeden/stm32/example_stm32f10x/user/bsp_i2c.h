#ifndef __BSP_I2C_H
#define __BSP_I2C_H

#include "stm32f10x.h"

//IIC
#define SDY_I2C		I2C1
#define SDY_I2C_CLK		RCC_APB1Periph_I2C1
#define SDY_I2C_CLK_CMD		RCC_APB1PeriphClockCmd

#define SDY_I2C_SPEED		400000 //频率设为400kb/s
#define SDY_I2C_SELF_ADDR_7  0x0A //设置stm32的I2c地址，和其他不一样即可


///EEPROM
/* 
 * AT24C02 2kb = 2048bit = 2048/8 B = 256 B 
 * 32 页，每页有8个字节，总共 256个字节
 *
 * Device Address
 * 1 0 1 0 A2 A1 A0 R/W
 * 1 0 1 0 0  0  0  0 = 0XA0
 * 1 0 1 0 0  0  0  1 = 0XA1 
 */
#define SDY_I2C_EE_PAGE_SIZE 8 //EEPROM 每页有几个字节
#define SDY_I2C_EE_ADDR_BLOCK0	0xA0 


///GPIO
#define SDY_I2C_GPIO_CLK			RCC_APB2Periph_GPIOB
#define SDY_I2C_GPIO_CLK_CMD		RCC_APB2PeriphClockCmd
#define SDY_I2C_SCL_PORT			GPIOB
#define SDY_I2C_SCL_PIN				GPIO_Pin_6
#define SDY_I2C_SDA_PORT			GPIOB
#define SDY_I2C_SDA_PIN				GPIO_Pin_7



///调试输出
#define SDY_I2C_INFO(fmt,arg...)	printf("<EE_INFO> "fmt"\n",##arg);
#define SDY_I2C_ERROR(fmt,arg...)	 printf("<EE_ERR> "fmt"\n",##arg);
#define SDY_I2C_DEBUG(fmt,arg...)		printf("<EE_DEBUG> "fmt"\n",##arg);


/*等待超时时间*/
#define SDY_I2C_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SDY_I2C_LONG_TIMEOUT         ((uint32_t)(10 * SDY_I2C_FLAG_TIMEOUT))





void i2c_config();

uint8_t sdy_i2c_ee_writeByte(uint8_t data, uint8_t memAddr);

void sdy_i2c_wait_ee_standby();

uint8_t sdy_i2c_ee_writeBytes(uint8_t* pBuffer,uint8_t WriteAddr, uint16_t NumByteToWrite);

uint8_t sdy_i2c_ee_writePage(const u8* data, u16 size ,u8 memAddr);

uint8_t sdy_i2c_ee_writeBuffer(const u8* data, u16 size ,u8 memAddr);

uint8_t sdy_i2c_ee_readBuffer(u8* dst,u8 memAddr,u16 readSize);


#endif