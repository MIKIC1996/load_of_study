#include "bsp_i2c.h"
#include <stdio.h>


								


///地址
uint16_t eeprom_addr = SDY_I2C_EE_ADDR_BLOCK0;

///GPIO
static void sdy_i2c_gpio_config(){
	
	///使能gpio时钟
	SDY_I2C_GPIO_CLK_CMD(SDY_I2C_GPIO_CLK,ENABLE);
	///使能I2C时钟，必须在这里提前使能
	SDY_I2C_CLK_CMD(SDY_I2C_CLK,ENABLE);
	///gpio config
	GPIO_InitTypeDef init;
	///SCL
	init.GPIO_Mode = GPIO_Mode_AF_OD; //使用复用开漏复用
	init.GPIO_Speed = GPIO_Speed_50MHz;
	init.GPIO_Pin = SDY_I2C_SCL_PIN;
	GPIO_Init(SDY_I2C_SCL_PORT,&init);	
	
	///SDA
	init.GPIO_Pin = SDY_I2C_SDA_PIN;
	GPIO_Init(SDY_I2C_SDA_PORT,&init);	
	
	
}


//I2C
void i2c_config(){
	
	///GPIO配置
	sdy_i2c_gpio_config();
	
	I2C_InitTypeDef init;
	///模式
	init.I2C_Mode = I2C_Mode_I2C; 
	///时钟占空比
	init.I2C_DutyCycle = I2C_DutyCycle_2;
	///i2c 寻址模式，改为7字节地址
	init.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	///I2C 自身地址
	init.I2C_OwnAddress1 = SDY_I2C_SELF_ADDR_7;
	///启动ACK
	init.I2C_Ack = I2C_Ack_Enable;
	///通讯速率
	init.I2C_ClockSpeed = SDY_I2C_SPEED;
	
	///初始化
	I2C_Init(SDY_I2C,&init);
	///使能I2C
	I2C_Cmd(SDY_I2C,ENABLE);
	
	/*
	I2C_InitTypeDef  I2C_InitStructure; 
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	
  I2C_InitStructure.I2C_OwnAddress1 =I2Cx_OWN_ADDRESS7; 
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;
	 

  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	

  I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;
  

  I2C_Init(EEPROM_I2Cx, &I2C_InitStructure);
  
  I2C_Cmd(EEPROM_I2Cx, ENABLE);   
	*/
}



///写入一个字节
///@param data :写入数据
///@param addr :要写入内部存储区地址
///@return bool 是否成功
uint8_t sdy_i2c_ee_writeByte(uint8_t data, uint8_t memAddr){
	///发送开始信号
	I2C_GenerateSTART(SDY_I2C,ENABLE);
	///重置超时时间
	uint32_t timeout = SDY_I2C_LONG_TIMEOUT;
	///检查相关事件 EV5,并且清除他
	while(!I2C_CheckEvent(SDY_I2C,I2C_EVENT_MASTER_MODE_SELECT)){
		if(timeout-- == 0){
			SDY_I2C_ERROR("time out of ev5");
			return 0;
		}
	}
	
	///重置超时时间
	timeout = SDY_I2C_LONG_TIMEOUT;
	///发送EEPROM地址,写地址
	I2C_Send7bitAddress(SDY_I2C,
											eeprom_addr,
											I2C_Direction_Transmitter);
	///等待事件 EV6
	while(!I2C_CheckEvent(SDY_I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){
		if(timeout-- == 0){
			SDY_I2C_ERROR("time out of ev6");
			return 0;
		}
	}
	
	///重置超时时间
	timeout = SDY_I2C_LONG_TIMEOUT;
	///发送内部存储地址,EEPROM自己决定的，以数据形式发送
	I2C_SendData(SDY_I2C,memAddr);
	///等待事件EV8
	while(!I2C_CheckEvent(SDY_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
		if(timeout-- == 0){
			SDY_I2C_ERROR("time out of ev8_2 when send mem addr");
			return 0;
		}
	}
	
	///重置超时时间
	timeout = SDY_I2C_FLAG_TIMEOUT;
	///发送数据
	I2C_SendData(SDY_I2C,data);
	///等待事件EV8
	while(!I2C_CheckEvent(SDY_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
		if(timeout-- == 0){
			SDY_I2C_ERROR("time out of ev8_2 when send data");
			return 0;
		}
	}
	
	///产生停止信号
	I2C_GenerateSTOP(SDY_I2C,ENABLE);
	return 1;
}




///等待EEPROM 进入就绪状态
void sdy_i2c_wait_ee_standby(){
	vu16 tmp = 0;
	//发送一个信号等待回应，有回应就是就绪了
	do{
		I2C_GenerateSTART(SDY_I2C,ENABLE);
		tmp = I2C_ReadRegister(SDY_I2C,I2C_Register_SR1);
		I2C_Send7bitAddress(SDY_I2C,eeprom_addr,I2C_Direction_Transmitter);
	}while( !(I2C_ReadRegister(SDY_I2C,I2C_Register_SR1) & 0x0002) );
	
	//清除AF
	I2C_ClearFlag(SDY_I2C,I2C_FLAG_AF);
	//发送stop
	I2C_GenerateSTOP(SDY_I2C,ENABLE);
	
}


///写入多个数据
uint8_t sdy_i2c_ee_writeBytes(uint8_t* pBuffer,uint8_t WriteAddr,
                        uint16_t NumByteToWrite)
{
		
    uint16_t i;
    uint8_t res;
/*每写一个字节调用一次I2C_EE_ByteWrite函数*/
    for (i=0; i<NumByteToWrite; i++)
    {
        /*等待EEPROM准备完毕*/
        sdy_i2c_wait_ee_standby();
        /*按字节写入数据*/
        res = sdy_i2c_ee_writeByte(*(pBuffer++),WriteAddr++);
    }
    return res;
}



///写入一页的数据
uint8_t sdy_i2c_ee_writePage(const u8* data, u16 size ,u8 memAddr){
	assert_param(size <= 8);
	uint32_t timeout = SDY_I2C_LONG_TIMEOUT * 10;
	///若I2C忙碌等待
	while(I2C_GetFlagStatus(SDY_I2C, I2C_FLAG_BUSY))   
  {
    if((timeout--) == 0){
			SDY_I2C_ERROR("I2C is busy");
			return 0;
		}
  } 
	///发送开始信号
	timeout = SDY_I2C_LONG_TIMEOUT;
	I2C_GenerateSTART(SDY_I2C,ENABLE);
	
	///检查相关事件 EV5,并且清除他
	while(!I2C_CheckEvent(SDY_I2C,I2C_EVENT_MASTER_MODE_SELECT)){
		if(timeout-- == 0){
			SDY_I2C_ERROR("time out of ev5");
			return 0;
		}
	}
	
	///重置超时时间
	timeout = SDY_I2C_LONG_TIMEOUT;
	///发送EEPROM地址,写地址
	I2C_Send7bitAddress(SDY_I2C,
											eeprom_addr,
											I2C_Direction_Transmitter);
	///等待事件 EV6
	while(!I2C_CheckEvent(SDY_I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){
		if(timeout-- == 0){
			SDY_I2C_ERROR("time out of ev6");
			return 0;
		}
	}
	
	///重置超时时间
	timeout = SDY_I2C_LONG_TIMEOUT;
	///发送内部存储地址,EEPROM自己决定的，以数据形式发送
	I2C_SendData(SDY_I2C,memAddr);
	///等待事件EV8
	while(!I2C_CheckEvent(SDY_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
		if(timeout-- == 0){
			SDY_I2C_ERROR("time out of ev8_2 when send mem addr");
			return 0;
		}
	}
	
	
	while(size--){
		///重置超时时间
		timeout = SDY_I2C_FLAG_TIMEOUT;
		///发送数据
		I2C_SendData(SDY_I2C,*data);
		///指针++
		data++;
		///等待事件EV8
		while(!I2C_CheckEvent(SDY_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
			if(timeout-- == 0){
				SDY_I2C_ERROR("time out of ev8_2 when send data");
				return 0;
			}
		}
	}
	
	///产生停止信号
	I2C_GenerateSTOP(SDY_I2C,ENABLE);
	return 1;
}


///优化过的 对齐写入函数
uint8_t sdy_i2c_ee_writeBuffer(const u8* data, u16 size ,u8 memAddr){
	
	///计算在单页中的地址
	u16 pageAddr = memAddr % SDY_I2C_EE_PAGE_SIZE;
	///补满第一页，需要的个数
	u16 firstPageFillCount = SDY_I2C_EE_PAGE_SIZE - pageAddr;

	while(size > 0){
		///获取本次实际要写的数目
		u16 curWriteSize = 0; 
		if(firstPageFillCount > 0){ //补满第一页
			curWriteSize = firstPageFillCount;
			firstPageFillCount = 0; //第一次写入的置零
		}else if(size >= 8){ //中间页
			curWriteSize = 8;
		}else{ //最后一页
			curWriteSize = size;
		}
		sdy_i2c_ee_writePage(data,curWriteSize,memAddr);
		sdy_i2c_wait_ee_standby(); //等待写入成功
		data += curWriteSize; //数据源指针更新
		memAddr += curWriteSize; //EEPROM 存储地址更新
		size -= curWriteSize; //总数减去当前写入的数目
	}
	return 1;
	
}




uint8_t sdy_i2c_ee_readBuffer(u8* dst,u8 memAddr,u16 readSize){
	
	uint32_t timeOut = SDY_I2C_LONG_TIMEOUT;
	while(I2C_GetFlagStatus(SDY_I2C, I2C_FLAG_BUSY)){
		if(timeOut-- == 0){
			SDY_I2C_DEBUG("time out because busy");
			return 0;
		}
	}
	
	///先写入要读取的地址
	///发送开始信号
	I2C_GenerateSTART(SDY_I2C,ENABLE);
	timeOut = SDY_I2C_LONG_TIMEOUT;
	///检测EV5
	while(!I2C_CheckEvent(SDY_I2C,I2C_EVENT_MASTER_MODE_SELECT)){
		if(timeOut-- == 0){
			SDY_I2C_DEBUG("time out checking ev5");
			return 0;
		}
	}
	
	///发送地址
	I2C_Send7bitAddress(SDY_I2C,eeprom_addr,I2C_Direction_Transmitter);
	timeOut = SDY_I2C_LONG_TIMEOUT;
	///检测EV6
	while(!I2C_CheckEvent(SDY_I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){
		if(timeOut-- == 0){
			SDY_I2C_DEBUG("time out checking ev6");
			return 0;
		}
	}
	
	//通过重新使能I2C,硬件清除EV6 ???
	I2C_Cmd(SDY_I2C,ENABLE);
	
	
	///发送目标地址
	I2C_SendData(SDY_I2C,memAddr);
	timeOut = SDY_I2C_LONG_TIMEOUT;
	///检测EV8并清除
	while(!I2C_CheckEvent(SDY_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
		if(timeOut-- == 0){
			SDY_I2C_DEBUG("time out checking ev8");
			return 0;
		}
	}
	
	///重新开始读的过程
	///发送开始信号
	I2C_GenerateSTART(SDY_I2C,ENABLE);
	timeOut = SDY_I2C_LONG_TIMEOUT;
	///检测EV5
	while(!I2C_CheckEvent(SDY_I2C,I2C_EVENT_MASTER_MODE_SELECT)){
		if(timeOut-- == 0){
			SDY_I2C_DEBUG("time out checking ev5");
			return 0;
		}
	}
	
	///发送地址
	I2C_Send7bitAddress(SDY_I2C,eeprom_addr,I2C_Direction_Receiver);
	timeOut = SDY_I2C_LONG_TIMEOUT;
	///检测EV6
	while(!I2C_CheckEvent(SDY_I2C,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)){
		if(timeOut-- == 0){
			SDY_I2C_DEBUG("time out checking ev6");
			return 0;
		}
	}
	
	///进入读入数据阶段
	while(readSize){
		///读最后一个字节之前可以提前设置好结束状态
		if(readSize == 1){
			I2C_AcknowledgeConfig(SDY_I2C,DISABLE);//下次要发送NACK
			I2C_GenerateSTOP(SDY_I2C,ENABLE); //当前传输完成后产生STOP
		}
		
		timeOut = SDY_I2C_LONG_TIMEOUT;
		///等待数据就绪
		while(I2C_CheckEvent(SDY_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED)==0)  
		{
			if((timeOut--) == 0){ 
				SDY_I2C_DEBUG("time out of read");
				return 0;
			}
		} 
		*dst = I2C_ReceiveData(SDY_I2C);
		dst++;
		readSize--;
	}
	
	///重新使能ACK
  I2C_AcknowledgeConfig(SDY_I2C, ENABLE);
  
  return 1;
	
}











