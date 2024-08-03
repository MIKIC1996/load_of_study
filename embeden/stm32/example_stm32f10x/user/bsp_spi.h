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


///flash状态寄存器 busy位
#define SDY_FLASH_BUSY_FLAG 0x01



///控制NSS引脚高低的宏函数
#define SDY_SPI_NSS_LOW()   GPIO_ResetBits(SDY_SPI_FLASH_GPIO_PORT,SDY_SPI_FLASH_NSS_GPIO_PIN);
#define SDY_SPI_NSS_HIGH()  GPIO_SetBits(SDY_SPI_FLASH_GPIO_PORT,SDY_SPI_FLASH_NSS_GPIO_PIN);


#define SDY_SPI_ERROR(fmt,arg...)	 printf("<FLASH_ERR> "fmt"\n",##arg);



///无效任意值
#define SDY_SPI_FLASH_DUMMY_BYTE 0XFF

/// flash芯片定义的置零编码
///写使能
#define SDY_FLASH_WRITE_ENABLE              0x06
///写失能
#define SDY_FLASH_WRITE_DISABLE             0x04
///读状态寄存器
#define SDY_FLASH_READ_STATUS_REGISTER      0x05
///写状态寄存器
#define SDY_FLASH_WRITE_STATUS_REGISTER     0x01
///读取数据
#define SDY_FLASH_READ_DATA                 0x03

#define SDY_FLASH_FAST_READ                 0x0b

#define SDY_FLASH_FAST_READ_DUAL_OUTPUT     0x3b
///按页写入数据？
#define SDY_FLASH_PAGE_PROGRAM              0x02
///块擦除
#define SDY_FLASH_BLOCK_ERASE               0xD8
///扇区擦除
#define SDY_FLASH_SECTOR_ERASE              0x20
///整片擦除
#define SDY_FLASH_CHIP_ERASE                0xc7

#define SDY_FLASH_POWER_DOWN                0xb9

#define SDY_FLASH_RELEASE_POWER_DOWM        0xab
///芯片识别号
#define SDY_FLASH_DEVICE_ID                 0xab
///厂商识别号
#define SDY_FLASH_MANUFACT_DEVICE_ID        0x90
///获取厂商 与 芯片识别号
#define SDY_FLASH_JEDEC_ID                  0x9f





#endif

