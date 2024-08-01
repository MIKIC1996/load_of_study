#ifndef __BSP_SPI_H
#define __BSP_SPI_H

#include "stm32f10x.h"


#define SDY_SPI                 SPI1
#define SDY_SPI_APBX_CLK_CMD    RCC_APB2PeriphClockCmd
#define SDY_SPI_CLK             RCC_APB2Periph_SPI1


#define SDY_SPI_FLASH_GPIO_PORT     GPIOA
#define SDY_SPI_FLASH_GPIO_CLK_CMD  RCC_APB2PeriphClockCmd
#define SDY_SPI_FLASH_GPIO_CLK      RCC_APB2Periph_GPIOA

///NSS引脚
#define SDY_SPI_FLASH_NSS_GPIO_PIN  GPIO_Pin_4

///SCK引脚
#define SDY_SPI_FLASH_SCK_GPIO_PIN  GPIO_Pin_5

///MISO引脚
#define SDY_SPI_FLASH_MISO_GPIO_PIN GPIO_Pin_6

///MOSI引脚
#define SDY_SPI_FLASH_MOSI_GPIO_PIN GPIO_Pin_7


///控制NSS引脚高低的宏函数
#define SDY_SPI_NSS_LOW()   GPIO_ResetBits(SDY_SPI_FLASH_GPIO_PORT,SDY_SPI_FLASH_NSS_GPIO_PIN);
#define SDY_SPI_NSS_HIGH()  GPIO_SetBits(SDY_SPI_FLASH_GPIO_PORT,SDY_SPI_FLASH_NSS_GPIO_PIN);


#define SDY_SPI_ERROR(fmt,arg...)	 printf("<FLASH_ERR> "fmt"\n",##arg);

#endif

