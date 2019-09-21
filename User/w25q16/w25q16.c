#include "w25q16.h"


/*************************************************
Function: W25Q16_GPIO_Conf
Description: 引脚配置
Input: 无
Output:无
Return: 无
Others: 板子所用固定片选引脚为PB14
*************************************************/
static void W25Q16_GPIO_Conf(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitTypeDef GPIOINIT_Struct;
    /*开启GPIO时钟*/
    RCC_AHB1PeriphClockCmd(W25Q16_GPIO_RCC, ENABLE);
	
	/*MISO引脚配置*/
	GPIOINIT_Struct.GPIO_Mode = GPIO_Mode_AF;
	GPIOINIT_Struct.GPIO_OType = GPIO_OType_PP;
	GPIOINIT_Struct.GPIO_Pin = W25Q16_SPI_MISO_Pin;
	GPIOINIT_Struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIOINIT_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(W25Q16_SPI_MISO_Port,&GPIOINIT_Struct);
	GPIO_PinAFConfig(W25Q16_SPI_MISO_Port, W25Q16_SPI_MISO_PinSource, W25Q16_SPI_AF);

	/*SCK引脚配置*/
	GPIOINIT_Struct.GPIO_Mode = GPIO_Mode_AF;
	GPIOINIT_Struct.GPIO_OType = GPIO_OType_PP;
	GPIOINIT_Struct.GPIO_Pin = W25Q16_SPI_SCK_Pin;
	GPIOINIT_Struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIOINIT_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(W25Q16_SPI_SCK_Port,&GPIOINIT_Struct);	
	GPIO_PinAFConfig(W25Q16_SPI_SCK_Port, W25Q16_SPI_SCK_PinSource, W25Q16_SPI_AF);
	
	/*MOSI引脚配置*/
	GPIOINIT_Struct.GPIO_Mode = GPIO_Mode_AF;
	GPIOINIT_Struct.GPIO_OType = GPIO_OType_PP;
	GPIOINIT_Struct.GPIO_Pin = W25Q16_SPI_MOSI_Pin;
    GPIOINIT_Struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIOINIT_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(W25Q16_SPI_MOSI_Port,&GPIOINIT_Struct);
	GPIO_PinAFConfig(W25Q16_SPI_MOSI_Port, W25Q16_SPI_MOSI_PinSource, W25Q16_SPI_AF);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin = W25Q16_CS_GPIOPin;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(W25Q16_CS_GPIOPort, &GPIO_InitStruct);
}

/*************************************************
Function: W25Q16_Write_Enable
Description: W25Q16写使能
Input: 无
Output:无
Return: 无
Others: 无
*************************************************/
static void W25Q16_Write_Enable(void)
{
	W25Q16_CS_Low;
	W25Q16_Read_Write(0x06);
	W25Q16_CS_Hight;
    
    W25Q16_Wait();/*等待读写完成*/
}


/*************************************************
Function: W25Q16_Sector_Erase
Description: 擦除地址所在扇区
Input: addr为24位地址
Output:无
Return: 无
Others: 无
*************************************************/
void W25Q16_Sector_Erase(uint32_t addr)
{
    /*将地址拆分为三个字节数据*/
    uint8_t Addr[3] = {0};
    Addr[0] = (addr&0x00FF0000) >> 16;
    Addr[1] = (addr&0x0000FF00) >> 8;
    Addr[2] = (addr&0x000000FF) >> 0;
    
    W25Q16_CS_Low;/*拉低片选，开始传输*/
    W25Q16_Read_Write(0x20);
    W25Q16_Read_Write(Addr[0]);
    W25Q16_Read_Write(Addr[1]);
    W25Q16_Read_Write(Addr[2]);
    W25Q16_CS_Hight;/*传输停止*/
 
    W25Q16_Wait();/*等待读写完成*/
   
    W25Q16_Write_Enable();/*擦除后需重新开启写使能*/
}

/*************************************************
Function: W25Q16_Write_Byte
Description: 向指定起始地址之后写入若干个数据
Input: addr为指定地址
       datasend为需发送的数据
       num为数据大小
Output:无
Return: 无
Others: 此处Num最大值为255
*************************************************/
void W25Q16_Write_Byte(uint32_t addr,uint8_t *datasend,uint8_t num)
{
     uint8_t Addr[3] = {0};
     uint8_t i;
    W25Q16_Write_Enable();
    /*将地址拆分为三个数据*/
   
    Addr[0] = (addr&0x00FF0000) >> 16;
    Addr[1] = (addr&0x0000FF00) >> 8;
    Addr[2] = (addr&0x000000FF) >> 0;
    W25Q16_CS_Low;
    W25Q16_Read_Write(0x02);
    W25Q16_Read_Write(Addr[0]);
    W25Q16_Read_Write(Addr[1]);
    W25Q16_Read_Write(Addr[2]);
    if(num<256)/*w25q16在一次片选拉低过程中，最多传输256个数据*/
    {
        
        for(i = 0;i<num;i++)/*连续发送*/
        {
            W25Q16_Read_Write(datasend[i]);
        }
    }
    W25Q16_CS_Hight;
    
    /*等待读写完成*/
    W25Q16_Wait();
    
   
}

/*************************************************
Function: W25Q16_Read_Byte
Description: 读取指定地址之后若干个字节数据
Input: addr为指定地址，num为需读取的字节数
Output: datasto为存储数据结果的首地址
Return: 无
Others: 无 
*************************************************/
void W25Q16_Read_Byte(uint32_t addr,uint8_t *datasto,uint8_t num)
{
    uint8_t i;
    /*将地址拆分为三个数据*/
    uint8_t Addr[3] = {0};
    Addr[0] = (addr&0x00FF0000) >> 16;
    Addr[1] = (addr&0x0000FF00) >> 8;
    Addr[2] = (addr&0x000000FF) >> 0;
    W25Q16_CS_Low;
    W25Q16_Read_Write(0x03);
    W25Q16_Read_Write(Addr[0]);
    W25Q16_Read_Write(Addr[1]);
    W25Q16_Read_Write(Addr[2]);
    
    /*连续读取*/
    for(i = 0;i<num;i++)
    {
        datasto[i] = W25Q16_Read_Write(0x00);
    }
    W25Q16_CS_Hight;
    /*等待读写完成*/
    W25Q16_Wait();
    
}


/*************************************************
Function: W25Q16_Wait
Description: 判断是否正在读写,若正在读写则继续等待
Input: 无
Output:无
Return: 无
Others: 无
*************************************************/
static void W25Q16_Wait(void)
{
    uint8_t temp = 0;
    W25Q16_CS_Low;
    W25Q16_Read_Write(0x05);
    
    /*连续读取状态寄存器1的值*/
    do
    {
        temp = W25Q16_Read_Write(0x00);
    }
    while(temp&0x01==1); /*判断状态寄存器最低位，最低位为1表示忙碌*/
    
    W25Q16_CS_Hight;
    
}

/*************************************************
Function: W25Q16_Read_Write
Description: 使用SPI协议对w25q16进行读写操作，此函数同时实现读取与写入
Input: 需写入的数据
Output:无
Return: 读取结果
Others: 无
*************************************************/
static uint16_t W25Q16_Read_Write(uint16_t Data)
{
	uint16_t d;
	
	while(SPI_I2S_GetFlagStatus(W25Q16_SPI_Used, SPI_I2S_FLAG_TXE)==RESET);/*等待发送数据寄存器为空*/
	SPI_I2S_SendData(W25Q16_SPI_Used,Data);
	
	/*检验数据是否发送完成*/
	while(SPI_I2S_GetFlagStatus(W25Q16_SPI_Used, SPI_I2S_FLAG_RXNE)==RESET);
	
	/*数据发送完成时也接收到数据*/
	d = SPI_I2S_ReceiveData(W25Q16_SPI_Used);
	
	
	return d;
}


/*************************************************
Function: W25Q16_SPI_Conf
Description: 配置W25Q16用到的SPI通信模式
Input: 无
Output:无
Return: 无
Others: 无
*************************************************/
void W25Q16_SPI_Conf(void)
{
	SPI_InitTypeDef SPI_Init_Struct;
	
	/*配置GPIO*/
    W25Q16_GPIO_Conf();

	W25Q16_CS_Hight;/*片选引脚初始化为高电平*/
    
	/*开启SPI时钟*/
	W25Q16_SPI_RCC_Cmd(W25Q16_RCC_SPI,ENABLE);
	
	/*配置SPI参数*/
	SPI_Init_Struct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
	
	/*W25Q16支持模式0和模式3，此处配置为模式3*/
	SPI_Init_Struct.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_Init_Struct.SPI_CPOL = SPI_CPOL_High;
	
	SPI_Init_Struct.SPI_DataSize = SPI_DataSize_8b;
	SPI_Init_Struct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_Init_Struct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_Init_Struct.SPI_Mode = SPI_Mode_Master;
	
	/*选择软件片选*/
	SPI_Init_Struct.SPI_NSS = SPI_NSS_Soft;
	
	/*未使能CRC校验时，此处配置无效*/
	SPI_Init_Struct.SPI_CRCPolynomial = 7;
    
	SPI_Init(W25Q16_SPI_Used,&SPI_Init_Struct);
	/*使能SPI*/
	SPI_Cmd(W25Q16_SPI_Used,ENABLE);
    
    /*配置写使能*/
    W25Q16_Write_Enable();
	
}






