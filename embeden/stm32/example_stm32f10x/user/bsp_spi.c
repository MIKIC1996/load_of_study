#include "bsp_spi.h"

#include <stdio.h>
#include "bsp_systick.h"



/// 外部函数声明
extern void sdy_usart_config(void);


/// @brief 初始化函数
void sdy_spi_falsh_config(void){
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

    ///先输出高信号（NSS），表示不选中任何设备
    SDY_SPI_NSS_HIGH()

    
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

    ///初始化
    SPI_Init(SDY_SPI,&sinit);
    
    ///使能SPI
    SPI_Cmd(SDY_SPI,ENABLE);

}



/// @brief 发送一个数据 ， 然后接收一个数据， SPI的发送 与接收是同步进行的
/// @param data 
/// @return bool 
u8 sdy_spi_flash_send_and_recv_byte(u8 data){
    const u32 maxWaitMsec = 1000;
    ///开启异步计时
    async_count_ms(maxWaitMsec);
    ///等待 TXE 置 1，说明数据已经全部发送，可以发送下一个字节数据了（并不代表flash芯片的忙碌状态）
    while( SPI_I2S_GetFlagStatus(SDY_SPI,SPI_I2S_FLAG_TXE) == RESET){
        if(check_time_out()){ //超时
            SDY_SPI_ERROR("time out because not txe")
            return 0; 
        }   
    }

    ///写入要发送的数据
    SPI_I2S_SendData(SDY_SPI,data);

    ///等待接收寄存器非空，表示有新数据了
    ///先开启异步计时
    async_count_ms(maxWaitMsec);
    while(SPI_I2S_GetFlagStatus(SDY_SPI,SPI_I2S_FLAG_RXNE) == RESET){
        if(check_time_out()){ //超时
            SDY_SPI_ERROR("time out because not rxne")
            return 0; 
        }   
    }

    ///返回收到的数据
    return SPI_I2S_ReceiveData(SDY_SPI);
}



///写使能（）
void sdy_spi_flash_write_enable(void){
    SDY_SPI_NSS_LOW()

    sdy_spi_flash_send_and_recv_byte(SDY_FLASH_WRITE_ENABLE);

    SDY_SPI_NSS_HIGH()
}


///读取芯片的ID
u32 sdy_spi_flash_read_id(void){
    
    ///片选NSS低电平，表示开始通讯
    SDY_SPI_NSS_LOW()
    ///发送指令
    sdy_spi_flash_send_and_recv_byte(SDY_FLASH_JEDEC_ID);

    /*读取*/
    u32 byteHigh = sdy_spi_flash_send_and_recv_byte(SDY_SPI_FLASH_DUMMY_BYTE);
    u32 byteMid = sdy_spi_flash_send_and_recv_byte(SDY_SPI_FLASH_DUMMY_BYTE);
    u32 byteLow = sdy_spi_flash_send_and_recv_byte(SDY_SPI_FLASH_DUMMY_BYTE);

    ///停止通讯
    SDY_SPI_NSS_HIGH()

    ///从高位到低位组合 返回
    return (byteHigh<<16) | (byteMid << 8) | (byteLow);

}




/// 读取flash的状态寄存器，可以用来确认其是否忙碌
u8 sdy_spi_flash_read_sr(void){
    SDY_SPI_NSS_LOW()
    ///写入指令
    sdy_spi_flash_send_and_recv_byte(SDY_FLASH_READ_STATUS_REGISTER);
    ///获得状态
    u8 ret = sdy_spi_flash_send_and_recv_byte(SDY_SPI_FLASH_DUMMY_BYTE);
    SDY_SPI_NSS_HIGH();
    return ret;
}



/// @brief 如果是进行flash存储矩阵的写入操作（不是单纯像上面一样发指令），当写入以后，需要检查flash的状态寄存器，检查是否忙碌状态
void sdy_spi_flash_wait_for_not_busy(void){

    SDY_SPI_NSS_LOW()
    ///写入指令
    u8 ret = 0;
    sdy_spi_flash_send_and_recv_byte(SDY_FLASH_READ_STATUS_REGISTER);
    
    do{
        ///获得状态
        ret = sdy_spi_flash_send_and_recv_byte(SDY_SPI_FLASH_DUMMY_BYTE);
    }while(ret & SDY_FLASH_BUSY_FLAG); //不断读取检查 busy状态
    
    SDY_SPI_NSS_HIGH();
}



///擦除扇区
void sdy_spi_flash_erase_sector(u32 sectorAddr){
    ///因为要操作 flash存储矩阵，所以要打开写使能
    sdy_spi_flash_write_enable();
    ///以防万一，先检查 flash存储矩阵的忙碌状态
    sdy_spi_flash_wait_for_not_busy(); 

    SDY_SPI_NSS_LOW()
    ///发送扇区擦除指令
    sdy_spi_flash_send_and_recv_byte(SDY_FLASH_SECTOR_ERASE);
    ///发送地址，3个字节,高位先发
    sdy_spi_flash_send_and_recv_byte( (sectorAddr & 0xff0000) >> 16);
    sdy_spi_flash_send_and_recv_byte( (sectorAddr & 0xff00) >> 8);
    sdy_spi_flash_send_and_recv_byte( sectorAddr & 0xff);
    SDY_SPI_NSS_HIGH();

    ///等待存储矩阵 操作完成
    sdy_spi_flash_wait_for_not_busy();
}



///页写入函数,以256字节为1页进行写入
///flash的单次写入 最大为256字节，超过的部分会覆盖之前写的
void sdy_spi_flash_write_page(u8 const* ptr, u32 size, u32 addr){
    assert_param(size <= 256);
    
    sdy_spi_flash_write_enable();
    sdy_spi_flash_wait_for_not_busy();

    SDY_SPI_NSS_LOW()
    sdy_spi_flash_send_and_recv_byte(SDY_FLASH_PAGE_PROGRAM);

    sdy_spi_flash_send_and_recv_byte( (addr & 0xff0000) >> 16);
    sdy_spi_flash_send_and_recv_byte( (addr & 0xff00) >> 8);
    sdy_spi_flash_send_and_recv_byte( addr & 0xff);

    while(size--){
        sdy_spi_flash_send_and_recv_byte(*ptr++);
        ///单次256字节内不需要等待 flash解除忙碌状态
    }
    SDY_SPI_NSS_HIGH()

    sdy_spi_flash_wait_for_not_busy(); //最后解除忙碌状态
}


/// @brief 不定量写入
/// @param ptr 
/// @param size 
/// @param addr 
void sdy_spi_flash_write_buffer(u8 const* ptr , u32 size, u32 addr){
    const u32 pageSize = 256;
    u32 pageAddr = addr % pageSize;
	  u32 firstPageWriteSize = (pageSize - pageAddr) < size ? (pageSize - pageAddr) : size;
    while(size > 0){
        u32 curWriteSize = 0;
        if(firstPageWriteSize > 0){
            curWriteSize = firstPageWriteSize;
            firstPageWriteSize = 0;
        }else if(size >= pageSize){
            curWriteSize = pageSize;
        }else{
            curWriteSize = size;
        }
        sdy_spi_flash_write_page(ptr,curWriteSize,addr);
        ptr += curWriteSize;
        addr += curWriteSize;
        size -= curWriteSize;
    }
}


///从flash中读取
void sdy_spi_flash_read_buffer(u8* dst ,u32 size , u32 addr){
    SDY_SPI_NSS_LOW()

    sdy_spi_flash_send_and_recv_byte(SDY_FLASH_READ_DATA);
    sdy_spi_flash_send_and_recv_byte( (addr & 0xff0000) >> 16);
    sdy_spi_flash_send_and_recv_byte( (addr & 0xff00) >> 8);
    sdy_spi_flash_send_and_recv_byte( addr & 0xff);

    while(size--){
        *dst = sdy_spi_flash_send_and_recv_byte(SDY_SPI_FLASH_DUMMY_BYTE);
        ++dst;
    }
    
    SDY_SPI_NSS_HIGH()

}


//进入掉电模式
void sdy_spi_flash_powerDown(void)   
{ 
  /* 通讯开始：CS低 */
  SDY_SPI_NSS_LOW();

  /* 发送 掉电 命令 */
  sdy_spi_flash_send_and_recv_byte(SDY_FLASH_POWER_DOWN);

  /*通讯结束：CS高 */
  SDY_SPI_NSS_HIGH();
}   

//唤醒
void sdy_spi_flash_wakeUp(void)   
{
  /*选择 FLASH: CS 低 */
  SDY_SPI_NSS_LOW();

  /* 发送 上电 命令 */
  sdy_spi_flash_send_and_recv_byte(SDY_FLASH_RELEASE_POWER_DOWM);

   /* 停止信号 FLASH: CS 高 */
  SDY_SPI_NSS_HIGH();
}  




/* 获取缓冲区的长度 */
#define  countof(a)      (sizeof(a) / sizeof(*(a)))
#define  BufferSize (countof(Tx_Buffer)-1)


uint8_t Tx_Buffer[] = "pan jia yi tuo ku zi\r\n";
uint8_t Rx_Buffer[BufferSize];

void sdy_spi_flash_test(void){
    sdy_spi_falsh_config();
    sdy_usart_config(); //启用串口

    ///写入使能
    sdy_spi_flash_write_enable();

    ///读取芯片ID
    u32 flashId = sdy_spi_flash_read_id();

    u8* ptr = (u8*)(&flashId);
    ptr +=2;

    printf("manufactor: 0x%02X \n", *ptr);
    --ptr;
    printf("device: 0x%02X \n",*ptr);
    --ptr;
    printf("capacitys: 0x%02X \n",*ptr);

    /* 擦除将要写入的 SPI FLASH 扇区，FLASH写入前要先擦除 */
    // 这里擦除4K，即一个扇区，擦除的最小单位是扇区
    sdy_spi_flash_erase_sector(0x00);	 	 
    

    /* 将发送缓冲区的数据写到flash中 */
    sdy_spi_flash_write_buffer(Tx_Buffer, BufferSize, 0x00);		
    printf("\r\n writeData: %s \r\t", Tx_Buffer);
    

    /* 将刚刚写入的数据读出来放到接收缓冲区中 */
    sdy_spi_flash_read_buffer(Rx_Buffer, BufferSize,0x00);
    printf("\r\n readData: %s \r\n", Rx_Buffer);
    
	while(1);
}







