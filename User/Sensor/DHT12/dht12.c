#include "dht12.h"
#include "iic.h"

/*************************************************
Function: DHT12_Read_
Description: 读取指定寄存器里的内容
Input: datasto为用于存储数据的数组指针，addr为寄存器地址，num 为读取8位数据的个数
Output: 无
Return: 无
Others: 无 
*************************************************/
void DHT12_Read_(uint8_t *datasto)
{
	uint8_t i;
	Start_Signal();
	/*发送设备写地址*/
	I2C_Write_Byte(DHT12_DEV_WRITE_ADDR);
	while(!Wait_Ask());
	
	/*写入寄存器地址，从0x00开始读取*/
	I2C_Write_Byte(0x00);
	while(!Wait_Ask());
	
	/*重新发送起始信号*/
	Start_Signal();
	
	/*发送设备读地址*/
	I2C_Write_Byte(DHT12_DEV_READ_ADDR);
	while(!Wait_Ask());
	
	/*连续读取5个数据*/
	for(i = 0;i<5;i++)
	{
		datasto[i] = I2C_Read_Byte();
		if(i == 4)
		{
			NAsk_Signal();
		}
		else
		{
			Ask_Signal();
		}
		
	}
	/*终止信号*/
	Stop_Signal();
	
}

/*************************************************
Function: DHT12_Deal_Tem_Hum
Description: 处理数据转换为温湿度
Input: datasto为用于存储数据的数组指针,其中第一位为湿度信息，其次为温度信息
	   datasrc为数据源指针，即所读取寄存器的5个数据
Output: 无
Return: 返回值为零则说明传回数据有误，数据校验正确返回1
Others: 无 
*************************************************/
uint8_t DHT12_Deal_Tem_Hum(float* datasto,uint8_t *datasrc)
{
	uint8_t state = 0;
	uint8_t check = 0,i;
	float Tem_Int = 0,Tem_Dec = 0,Hum_Int = 0,Hum_Dec = 0;
	/*统一转换为浮点数*/
	Hum_Int = (float)datasrc[0];
	Hum_Dec = (float)datasrc[1];
	/*此处最高位表示温度正负*/
	Tem_Int = (float)(datasrc[2]&0x7F);
	Tem_Dec = (float)datasrc[3];
	
	/*计算校验和*/
	for(i = 0;i<4;i++)
	{
		check += datasrc[i];
	}
	
	/*判断校验和是否正确*/
	if(check == datasrc[4])
	{
		/*校验和正确返回1*/
		state = 1;
		
		/*将湿度小数位与整数位结合，并写到datasto[0]中*/
		/*小数位只有一位，故将其除10可得真实小数值*/
		datasto[0] = Hum_Int + (Hum_Dec/10.0f);
		
		/*将温度小数位与整数位结合，并写到datasto[1]中*/
        
		/*温度需判断正负*/
		if((datasrc[2]&0x80)==0x80)/*最高位为1，表示温度为负值*/
		{
			
			datasto[1] = (0-Tem_Int-(Tem_Dec/10.0f));
		}
		else
		{
			datasto[1] = Tem_Int + (Tem_Dec/10.0f);
		}
		
	}
	
	return state;
}

