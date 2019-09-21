#include "max6675.h"

/****
供能描述：读取MAX6675返回的16位原始数据
返回16位数据
****/
uint16_t MAX6675_ReadData(void)
{
	uint16_t temp = 0;
	
	/*拉低片选*/
	MAX6675_CSS_Low;
	
	/*此处实际上只有在读取数据，故发送假动作DUMMY*/
	temp = MAX6675_SPI_Read_Write(DUMMY);
	
	MAX6675_CSS_Hight;
	
	return temp;
}

/*************************************************
Function: MAX6675_AVE
Description: 读取多个16位原始数据，并通过中值滤波算法过滤跳动数据
Input: 无
Output:16位数据结果
Return: 无
Others: 无
*************************************************/
uint16_t MAX6675_AVE(void)
{
    uint16_t data[21] = {0};
    uint8_t i = 0,j = 0;
    uint8_t min = 0;
    uint16_t temp = 0;
    for(i = 0;i<21;i++)
    {
        data[i] = MAX6675_ReadData();
        General_TIM_Delay_ms(2);
    }
    
    /*选择排序法*/
    for (i = 0 ; i < 20 ; i++) 
    {
        min = i;
        for (j = i + 1; j < 21; j++)
        {
             if (data[j] < data[min])
             {
                 min = j;    
                 
             }
             temp = data[min];
             data[min] = data[i];
             data[i] = temp;
        }
                
    }
    
    return data[10];/*返回中间值*/
  
}
/*************************************************
Function: MAX6675_DealData
Description: 处理MAX6675所读取的16位数据，将其转换为温度值
Input: 输入16位原始数据
Output:无
Return: 浮点数类型的温度值
Others: 无
*************************************************/
float MAX6675_DealData(uint16_t data)
{
	float temp;
	/*获取Bit3至Bit14共12位有效值*/
	uint16_t vaild = ((data&0x7FF8)>>3);
	
	/*计算温度值，此处由MAX6675内部ADC获得12位精确度数值，测量范围为0-1023.75℃*/
	temp = (vaild*0.25)-2;
	
	return temp;
}

/*************************************************
Function: MAX6675_SPI_Conf
Description: 配置MAX6675所用到的SPI2
Input: 无
Output:无
Return: 无
Others: 无
*************************************************/
void MAX6675_SPI_Conf(void)
{
	SPI_InitTypeDef SPI_Init_Struct;
	
	
	MAX6675_SPI_GPIO_Conf();/*配置GPIO*/
	
	MAX6675_SPI_RCC_Cmd(MAX6675_RCC_SPI,ENABLE);/*开启SPI时钟*/
	
	/*配置SPI参数*/
	SPI_Init_Struct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;/*所用MAX6675外设最高时钟主频为4.3MHZ*/
	
	SPI_Init_Struct.SPI_CPHA = SPI_CPHA_1Edge;/*结合数据手册配置*/
	SPI_Init_Struct.SPI_CPOL = SPI_CPOL_Low;
	SPI_Init_Struct.SPI_DataSize = SPI_DataSize_16b;
	SPI_Init_Struct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_Init_Struct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_Init_Struct.SPI_Mode = SPI_Mode_Master;
	
	SPI_Init_Struct.SPI_NSS = SPI_NSS_Soft;/*选择软件片选*/
	
	SPI_Init_Struct.SPI_CRCPolynomial = 7;/*未使能CRC校验时，此处配置无效*/
	SPI_Init(MAX6675_SPI_Used,&SPI_Init_Struct);/*未使能CRC校验时，此处配置无效*/
	
	SPI_Cmd(MAX6675_SPI_Used,ENABLE);/*使能SPI*/
	
}

/*************************************************
Function: MAX6675_SPI_GPIO_Conf
Description: 配置SPI2相关引脚
Input: 无
Output:无
Return: 无
Others: 无
*************************************************/
void MAX6675_SPI_GPIO_Conf(void)
{
	GPIO_InitTypeDef GPIOINIT_Struct;
	
	/*开启GPIO时钟*/
	RCC_AHB1PeriphClockCmd(MAX6675_RCC_GPIO,ENABLE);
	
	/*MISO引脚配置*/
	GPIOINIT_Struct.GPIO_Mode = GPIO_Mode_AF;
	GPIOINIT_Struct.GPIO_OType = GPIO_OType_PP;
	GPIOINIT_Struct.GPIO_Pin = MAX6675_SPI_MISO_Pin;
	GPIOINIT_Struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIOINIT_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MAX6675_SPI_MISO_Port,&GPIOINIT_Struct);
	GPIO_PinAFConfig(MAX6675_SPI_MISO_Port, MAX6675_SPI_MISO_PinSource, MAX6675_SPI_AF);

	/*SCK引脚配置*/
	GPIOINIT_Struct.GPIO_Mode = GPIO_Mode_AF;
	GPIOINIT_Struct.GPIO_OType = GPIO_OType_PP;
	GPIOINIT_Struct.GPIO_Pin = MAX6675_SPI_SCK_Pin;
	GPIOINIT_Struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIOINIT_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MAX6675_SPI_SCK_Port,&GPIOINIT_Struct);	
	GPIO_PinAFConfig(MAX6675_SPI_SCK_Port, MAX6675_SPI_SCK_PinSource, MAX6675_SPI_AF);


	/*软件CS引脚配置*/
	GPIOINIT_Struct.GPIO_Mode = GPIO_Mode_OUT;
	GPIOINIT_Struct.GPIO_OType = GPIO_OType_PP;
	GPIOINIT_Struct.GPIO_Pin = MAX6675_SPI_SOFT_CS_Pin;
	GPIOINIT_Struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIOINIT_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MAX6675_SPI_SOFT_CS_Port,&GPIOINIT_Struct);
	
	/*片选默认高电平*/
	MAX6675_CSS_Hight;
}

/*************************************************
Function: MAX6675_SPI_Read_Write
Description: SPI读写函数
Input: Data为写入的16位数据
Output:无
Return: 读取的16位数据
Others: 无
*************************************************/
uint16_t MAX6675_SPI_Read_Write(uint16_t Data)
{
	uint16_t d;
	
	while(SPI_I2S_GetFlagStatus(MAX6675_SPI_Used, SPI_I2S_FLAG_TXE)==RESET);
	SPI_I2S_SendData(MAX6675_SPI_Used,Data);
	
	/*检验数据是否发送完成*/
	while(SPI_I2S_GetFlagStatus(MAX6675_SPI_Used, SPI_I2S_FLAG_RXNE)==RESET);
	
	/*数据发送完成时也接收到数据*/
	d = (uint16_t)SPI_I2S_ReceiveData(MAX6675_SPI_Used);

	return d;
}



