/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/* 这个是一个案例文件													   */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */

///SPI驱动函数文件
#include "bsp_spi.h"

///屏蔽这些案例，改成自己的
/* Definitions of physical drive number for each drive */
//#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
//#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
//#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */


///移植修改,设备编号
#define SDY_FATFS_DEV_ATA			0 //预留SD卡
#define SDY_FATFS_DEV_SPI_FLASH 	1 //spi flash



/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = STA_NOINIT;///未初始化
	switch (pdrv) {
	case SDY_FATFS_DEV_ATA :
		break;

	case SDY_FATFS_DEV_SPI_FLASH :
		if(sdy_spi_flash_read_id() == 0xEF4017){ //如果读到的flash ID正确
			stat &= ~STA_NOINIT; 
		}
		break;

	}
	return stat;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = STA_NOINIT;
	switch (pdrv) {
	case SDY_FATFS_DEV_ATA :
		break;

	case SDY_FATFS_DEV_SPI_FLASH :
		sdy_spi_falsh_config();
		///唤醒
		sdy_spi_flash_wakeUp();
		//为什么要延时一段时间
		u32 i = 500;
		while(i--);
		///获取状态
		stat = disk_status(pdrv);
		break;
	}
	return stat;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read 读取的扇区数量 */
)
{
	DRESULT res = RES_PARERR; //初始化 无效参数
	switch (pdrv) {
	case SDY_FATFS_DEV_ATA :
		break;

	case SDY_FATFS_DEV_SPI_FLASH :
		///空出前2M的空间
		sector += 512;  //每个扇区4kb, 512 * 4kb = 2M
		sdy_spi_flash_read_buffer(buff,count<<12 /* count * 4kb  */,sector << 12  );
		res = RES_OK; //成功
		break;
	}

	return res;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res = RES_PARERR;

	switch (pdrv) {
	case SDY_FATFS_DEV_ATA :
		break;

	case SDY_FATFS_DEV_SPI_FLASH :
		sector += 512;
		sdy_spi_flash_erase_sector(sector << 12); //这里只清除了一个扇区？
		sdy_spi_flash_write_buffer(buff,count << 12, sector << 12);
		res = RES_OK;
		break;
	}

	return res;
}



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res = RES_PARERR;
	switch (pdrv) {
	case SDY_FATFS_DEV_ATA:
		break;
	case SDY_FATFS_DEV_SPI_FLASH:

		///根据指令
		switch(cmd){
		/* 扇区数量：1536*4096/1024/1024=6(MB) */
		case GET_SECTOR_COUNT:
			*(DWORD*)buff = 1546;
			res = RES_OK;
			break;
		///扇区大小KB
		case GET_SECTOR_SIZE: 
			*(WORD*)buff = 4096; 
			res = RES_OK;
			break; 
		///同时 擦除扇区的个数,只有一个
		case GET_BLOCK_SIZE:
			*(DWORD*)buff = 1;
			res = RES_OK;
			break;
		}
		
	}

	return res;
}


///时间戳获取
__weak DWORD get_fattime(void)
{
    /* 返回当前时间戳 */
    return    ((DWORD)(2015 - 1980) << 25)  /* Year 2015 */
            | ((DWORD)1 << 21)        /* Month 1 */
            | ((DWORD)1 << 16)        /* Mday 1 */
            | ((DWORD)0 << 11)        /* Hour 0 */
            | ((DWORD)0 << 5)         /* Min 0 */
            | ((DWORD)0 >> 1);        /* Sec 0 */
}

