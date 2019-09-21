#include "iic.h"
#include "tim.h"

/*************************************************
Function: IIC_CONF
Description: 配置IIC
Input: 无
Output:无
Return: 无
Others: 无
*************************************************/
void IIC_CONF(void)
{
	GPIO_InitTypeDef GPIOInit_Struct;
	RCC_AHB1PeriphClockCmd(GPIO_CLOCK,ENABLE);
	
	/*配置GPIO模拟SDA线*/
	GPIOInit_Struct.GPIO_Mode = GPIO_Mode_OUT;
	GPIOInit_Struct.GPIO_OType = GPIO_OType_PP;
	GPIOInit_Struct.GPIO_Pin = I2C_SDA_GPIO_Pin;
	GPIOInit_Struct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIOInit_Struct.GPIO_Speed = GPIO_Fast_Speed;
	GPIO_Init(I2C_SDA_GPIOPort,&GPIOInit_Struct);
	/*配置GPIO模拟SCL线*/
	GPIOInit_Struct.GPIO_Mode = GPIO_Mode_OUT;
	GPIOInit_Struct.GPIO_OType = GPIO_OType_PP;
	GPIOInit_Struct.GPIO_Pin = I2C_SCL_GPIO_Pin;
	GPIOInit_Struct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIOInit_Struct.GPIO_Speed = GPIO_Fast_Speed;
	GPIO_Init(I2C_SCL_GPIOPort,&GPIOInit_Struct);
	
    General_TIM_Delay_ms(10);
    
	/*初始状态*/
    I2C_SDA_1();
	I2C_SCL_1();
}

/*************************************************
Function: Wait_Ask
Description: 等待从机应答信号
Input: 无
Output:无
Return: 返回值为0时，未收到应答信号，返回值为1时，收到应答信号
Others: 无
*************************************************/
uint8_t Wait_Ask(void)
{
    uint8_t state = 0;
    
    I2C_SDA_IN();

    /*确保SDA是在SCL低电平产生变化*/
    I2C_SCL_0();
	i2c_Delay();
    
	/*将SDA置为高电平,CPU释放SDA总线*/
	I2C_SDA_1();
	i2c_Delay();
	/*产生时钟*/
	I2C_SCL_1();
	i2c_Delay();
	if(SDA_Vaule==0)
	{
		state = 1;
	}
	I2C_SCL_0();
	i2c_Long_Delay();
	return state;
}

/*************************************************
Function: Start_Signal
Description: 模拟IIC协议的起始信号
Input: 无
Output:无
Return: 无
Others: 无
*************************************************/
void Start_Signal(void)
{
    
    I2C_SDA_OUT();

	/*将SDA、SCL置为高电平*/
	I2C_SDA_1();
	i2c_Delay();
	I2C_SCL_1();
	i2c_Delay();
	/* SDA出现下跳沿表示I2C总线启动信号 */
	I2C_SDA_0();
	i2c_Delay();
	
	I2C_SCL_0();
	i2c_Delay();
	
}

/*************************************************
Function: Stop_Signal
Description: 模拟IIC协议的终止信号
Input: 无
Output:无
Return: 无
Others: 无
*************************************************/
void Stop_Signal(void)
{
    I2C_SDA_OUT();
    
    /*确保SDA是在SCL低电平产生变化*/
    I2C_SCL_0();
	i2c_Delay();
    
	/*初始条件*/
	I2C_SDA_0();
	i2c_Delay();
	I2C_SCL_1();
	i2c_Delay();
	/* SDA出现上升沿表示I2C总线停止信号 */
	I2C_SDA_1();
	i2c_Delay();
	
	
}

/*************************************************
Function: Ask_Signal
Description: 主机模拟IIC协议应答
Input: 无
Output:无
Return: 无
Others: 无
*************************************************/
void Ask_Signal(void)
{
    I2C_SDA_OUT();
    
    /*确保SDA是在SCL低电平产生变化*/
    I2C_SCL_0();
	i2c_Delay();
	
	I2C_SDA_0();
	i2c_Delay();
	I2C_SCL_1();/* CPU产生1个时钟周期 */
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
    
	I2C_SDA_1();/* CPU释放SDA总线 */
	i2c_Delay();
}


/*************************************************
Function: NAsk_Signal
Description: 主机模拟IIC协议非应答
Input: 无
Output:无
Return: 无
Others: 无
*************************************************/
void NAsk_Signal(void)
{
    I2C_SDA_OUT();
    
    /*确保SDA是在SCL低电平产生变化*/
    I2C_SCL_0();
	i2c_Delay();
	
    
	I2C_SDA_1();
	i2c_Delay();
	I2C_SCL_1();/* CPU产生1个时钟周期 */
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
                    
                   
}


/*************************************************
Function: I2C_Read_Byte
Description: 模拟IIC协议读取一个字节数据
Input: 无
Output:无
Return: 数据结果
Others: 无
*************************************************/
uint8_t I2C_Read_Byte(void)
{
	uint8_t i;
	uint8_t temp = 0;
    
    I2C_SDA_IN();
    
    /*确保SDA是在SCL低电平产生变化*/
    I2C_SCL_0();
	i2c_Long_Delay();
	
	
	for(i = 0;i<8;i++)
	{
		temp<<=1;
		I2C_SCL_1();/* CPU产生1个时钟周期 */
		i2c_Long_Delay();
		
		if(SDA_Vaule!=0)
		{
			temp|=0x01;
		}
		I2C_SCL_0();
		i2c_Long_Delay();
	}
	
	
	return temp;

}


/*************************************************
Function: I2C_Write_Byte
Description: 模拟IIC协议发送一个字节数据
Input: 需发送的数据
Output:无
Return: 无
Others: 无
*************************************************/
void I2C_Write_Byte(uint8_t data)
{    
	uint8_t i;
    
    I2C_SDA_OUT();
    
	I2C_SCL_0();
	i2c_Delay();
	 
	for(i = 0;i<8;i++)
	{
		if((data&0x80)!=0)
		{
			I2C_SDA_1();
		}
		else
		{
			I2C_SDA_0();
		}
		i2c_Delay();
		
		I2C_SCL_1();	/* CPU产生1个时钟 */
		i2c_Delay();
		I2C_SCL_0();
		i2c_Delay();	
				
		data<<= 1;
		if(i==7)
		{
			I2C_SDA_1();/*释放总线*/
			i2c_Delay();
		}
	}
	
}



/*************************************************
Function: i2c_Delay
Description: 简单的IIc延时函数
Input: 无
Output:无
Return: 无
Others: 无
*************************************************/
void i2c_Delay(void)
{
    /*180为DHT12温度传感器经多次实验所得得较为可靠的值，
    此值偏低时数据采集校验和在传感器工作时间较长时有误，偏高时影响采样速率*/
	uint16_t i;
	for (i = 0; i < 10; i++);
}


/*************************************************
Function: i2c_Long_Delay
Description: 简单的IIc长时间延时函数
Input: 无
Output:无
Return: 无
Others: 此处主要是为了保持DHT12能正常发出相应
*************************************************/
void i2c_Long_Delay(void)
{
    uint8_t i;
    for(i = 0;i<18;i++)
    {
        i2c_Delay();
    }
}



/*************************************************
Function: I2C_SDA_IN
Description: 配置SDA为输入模式，用于转换SDA模式
Input: 无
Output:无
Return: 无
Others: 无
*************************************************/
void I2C_SDA_IN(void)
{
   GPIO_InitTypeDef  GPIO_InitStructure;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
   GPIO_InitStructure.GPIO_Pin = I2C_SDA_GPIO_Pin;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed; 
   GPIO_Init(I2C_SDA_GPIOPort, &GPIO_InitStructure);	
}
 


/*************************************************
Function: I2C_SDA_OUT
Description: 配置SDA为输出模式，用于转换SDA模式
Input: 无
Output:无
Return: 无
Others: 无
*************************************************/
void I2C_SDA_OUT(void)
{
    GPIO_InitTypeDef GPIOInit_Struct;
    GPIOInit_Struct.GPIO_Mode = GPIO_Mode_OUT;
	GPIOInit_Struct.GPIO_OType = GPIO_OType_PP;
	GPIOInit_Struct.GPIO_Pin = I2C_SDA_GPIO_Pin;
	GPIOInit_Struct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIOInit_Struct.GPIO_Speed = GPIO_Fast_Speed;
	GPIO_Init(I2C_SDA_GPIOPort,&GPIOInit_Struct);	
}

