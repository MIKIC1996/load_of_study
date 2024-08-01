#include "bsp_spi.h"

#include <stdio.h>
#include "bsp_systick.h"




/// @brief 初始化函数
void sdy_spi_falsh_config(){

    ///spi时钟使能
    SDY_SPI_APBX_CLK_CMD(SDY_SPI_CLK,ENABLE);
    ///falsh相关GPIO使能
    SDY_SPI_FLASH_GPIO_CLK_CMD(SDY_SPI_FLASH_GPIO_CLK,ENABLE);

    ///GPIO配置
    GPIO_InitTypeDef ginit;
    ///配置NSS,作为普通引脚使用即可，使用推挽输出
    ginit.GPIO_Mode = GPIO_Mode_Out_PP; 
    ginit.GPIO_Speed = GPIO_Speed_50MHz;
    ginit.GPIO_Pin = SDY_SPI_FLASH_NSS_GPIO_PIN;
    GPIO_Init(SDY_SPI_FLASH_GPIO_PORT,&ginit);

    ///配置SCK,模式为复用推挽
    ginit.GPIO_Mode = GPIO_Mode_AF_PP;
    ginit.GPIO_Pin = SDY_SPI_FLASH_SCK_GPIO_PIN;
    GPIO_Init(SDY_SPI_FLASH_GPIO_PORT,&ginit);

    ///配置MISO
    ginit.GPIO_Pin = SDY_SPI_FLASH_MISO_GPIO_PIN;
    GPIO_Init(SDY_SPI_FLASH_GPIO_PORT,&ginit);

    ///配置MOSI
    ginit.GPIO_Pin = SDY_SPI_FLASH_MOSI_GPIO_PIN;
    GPIO_Init(SDY_SPI_FLASH_GPIO_PORT,&ginit);

    ///spi 配置
    SPI_InitTypeDef sinit;
    sinit.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //双线全双工
    sinit.SPI_Mode = SPI_Mode_Master; //单片机做主机
    sinit.SPI_DataSize = SPI_DataSize_8b; //数据帧长度为8字节
    ///spi模式3  sck空闲是高电平 + 偶数边沿采样 = 上升沿采样
    sinit.SPI_CPOL = SPI_CPOL_High;
    sinit.SPI_CPHA = SPI_CPHA_2Edge; 
    ///NSS用软件控制
    sinit.SPI_NSS = SPI_NSS_Soft;
    ///时钟频率采用四分频, APB2频率一般是72m ,所以这里频率 = 72/4 = 18m
    sinit.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    ///高位先行
    sinit.SPI_FirstBit = SPI_FirstBit_MSB;
    ///CRC校验值随便设，目前还不知道为什么
    sinit.SPI_CRCPolynomial = 7;

    SPI_Init(SDY_SPI,&sinit);
}



/// @brief 发送一个数据
/// @param data 
/// @return bool 
u8 sdy_spi_flash_send_byte(u8 data){
    const u32 maxWaitMsec = 100;
    ///开启异步计时
    async_count_ms(maxWaitMsec);
    ///等待
    while( SPI_I2S_GetFlagStatus(SDY_SPI,SPI_I2S_FLAG_TXE) == RESET){
        if(check_time_out()){
            SDY_SPI_ERROR("time out because not txe")
            return 0; 
        }   
    }

    return 1;

}













