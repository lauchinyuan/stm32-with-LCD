#include "tcs3472.h"
#include "iic.h"
#include "stdio.h"
#include "tim.h"

/*************************************************
Function: TCS3472_Write_
Description: 往指定地址的寄存器里写内容
Input: addr为寄存器地址,num为写入8位数据的个数,data为数据首地址
Output:无
Return: 无
Others: 无
*************************************************/
void TCS3472_Write_(uint8_t *data,uint8_t num,uint8_t addr)
{
	uint8_t i;
	Start_Signal();
	I2C_Write_Byte(TCS3472_DEVICE_WRITE_ID);/*发送设备写指令*/
	while(!Wait_Ask());/*等待响应信号*/
	I2C_Write_Byte(addr|TCS3472_COMMAND_Bit);/*写入寄存器寻址指令*/
	while(!Wait_Ask());
	for(i = 0;i < num;i++)
	{
		I2C_Write_Byte(data[i]);
		while(!Wait_Ask());
	}
	Stop_Signal();
	
}

/*************************************************
Function: TCS3472_Read_
Description: 读取指定寄存器里的内容
Input: addr为寄存器地址,num为读取8位数据的个数
Output:datasto为数据结果首地址
Return: 无
Others: 无
*************************************************/
void TCS3472_Read_(uint8_t *datasto,uint8_t num,uint8_t addr)
{
	uint8_t i;
	Start_Signal();
	I2C_Write_Byte(TCS3472_DEVICE_WRITE_ID);/*发送设备写指令*/   
	while(!Wait_Ask());
	I2C_Write_Byte(addr|TCS3472_COMMAND_Bit);/*写入寄存器寻址指令*/
	while(!Wait_Ask());
	Start_Signal();/*第二次起始信号*/	
	
	I2C_Write_Byte(TCS3472_DEVICE_READ_ID);/*发送设备读指令*/
	while(!Wait_Ask());
	
	/*接收数据*/
	for(i=0;i<num;i++)
	{
		datasto[i] = I2C_Read_Byte();
		if(i==num - 1)
		{
			NAsk_Signal();
		}
		else
		{
			Ask_Signal();
		}
	}
	Stop_Signal();
}

/*************************************************
Function: TCS3472_Enable
Description: 使TCS34725传感器进入工作模式（AEN）
Input: 无
Output:无
Return: 无
Others: 无
*************************************************/
void TCS3472_Enable(void)
{
	/*先进入Idle模式*/
	uint8_t Command = TCS3472_ENABLE_PON;/*PON命令*/
     
	TCS3472_Write_(&Command,1,TCS3472_ENABLE_REGISTER);/*将命令写至寄存器*/
	
	/*进入工作模式，且开启等待功能*/
	Command = (TCS3472_ENABLE_PON|TCS3472_ENABLE_AEN|TCS3472_ENABLE_WEN);
    
	TCS3472_Write_(&Command,1,TCS3472_ENABLE_REGISTER);
    
	/*此处初始化至少要2.4毫秒的等待时间，故加上延时函数，延时3毫秒*/
    General_TIM_Delay_ms(3);
	
}

/*************************************************
Function: TCS3472_Set_WTIME
Description: 设定等待时间WTIME值
Input: wtime为向寄存器写入的值
Output:无
Return: 无
Others: 无
*************************************************/
void TCS3472_Set_WTIME(uint8_t wtime)
{
	TCS3472_Write_(&wtime,1,TCS3472_WTIME_REGISTER);
}


/*************************************************
Function: TCS3472_Get_Conv_Status
Description: 获取状态寄存器最低位的值以确认是否完成转换
Input: 无
Output:无
Return: 若转换完成返回1，否则返回0
Others: 无
*************************************************/
uint8_t TCS3472_Get_Conv_Status(void)
{
	uint8_t state = 0;/*转换状态*/	
	TCS3472_Read_(&state,1,TCS3472_STATUS_REGISTER);
	
	if((state&0x01) == 0x01)/*转换完成*/
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*************************************************
Function: TCS3472_Read_RGBData
Description: 获取RGB数据值，且读取顺序为C->R->G->B
Input: 无
Output:datasto为用于存储数据的数组指针
Return: 无
Others: 无
*************************************************/
void TCS3472_Read_RGBData(uint16_t* temp)
{
    uint8_t datasto[8] = {0};
    
	while(!TCS3472_Get_Conv_Status());/*等待转换完成*/

    TCS3472_Read_(datasto,8,TCS3472_CDATAL_REGISTER);/*一次读取八个字节*/
    
    /*数据处理，将原始数据逐个转换为RGB数据*/
    
    /*获取16位C,R,G,B值*/
    temp[0] = ((uint16_t)(datasto[1]<<8)|datasto[0]);       /*十六位C原始数据*/
    temp[1] = ((uint16_t)(datasto[3]<<8)|datasto[2]);       /*十六位R原始数据*/
    temp[2] = ((uint16_t)(datasto[5]<<8)|datasto[4]);       /*十六位G原始数据*/
    temp[3] = ((uint16_t)(datasto[7]<<8)|datasto[6]);       /*十六位B原始数据*/
    
}

/*************************************************
Function: TCS3472_White_Balance
Description: 对获取到的16位RGB数据进行白平衡处理，并将白平衡转化因子存储在W25Q16中
Input: 无
Output:无
Return: 无
Others: 调用此函数时颜色传感器应处于白色环境下
*************************************************/
void TCS3472_White_Balance(void)
{
    float RGB_Factor[3] = {0};/*RGB校准因子*/
    uint8_t i,j = 0;          
    uint16_t data[6][4] = {0};/*2维数组存储采集数据*/
    uint32_t sum[3] = {0};     /*分别为RGB三种颜色数据和*/

    LED1_ON();/*灯1亮表示开始转换*/
    
    for(i = 0;i<6;i++)/*连续采集多次数据*/
    {
        TCS3472_Read_RGBData(data[i]);
        General_TIM_Delay_ms(10);
    }
    /*分别计算所采集RGB值的平均值*/
    
    for(i = 0;i<3;i++)/*遍历RGB三色*/
    {
        for(j = 0;j<6;j++)/*遍历6组数据*/
        {
            sum[i]+=data[j][i+1];
        }
        sum[i] /= 6;/*求平均值*/
    }
    
    /*计算校准因子*/
    RGB_Factor[0] = 255.0/sum[0];
    RGB_Factor[1] = 255.0/sum[1];
    RGB_Factor[2] = 255.0/sum[2];
    
    /*将转化因子存储在外部Flash中，以便后续调用*/
    W25Q16_Sector_Erase(TCS3472_R_Factor);/*擦除扇区*/
    
    W25Q16_Write_Byte(TCS3472_R_Factor,(uint8_t*)RGB_Factor,12);/*每个float型变量占据4个字节空间*/
    
    LED_OFF();
    LED2_ON();/*灯2亮表示转换完成*/
    
}
/*************************************************
Function: TCS3472_Read_Deal
Description: 读取RGB16位原始数据并利用外部Flash中的校准因子处理数据
Input:  无
Output: 无
Return: RGB888数据
Others: 无 
*************************************************/
uint32_t TCS3472_Read_Deal(void)
{
    float kr = 0,kg = 0, kb = 0;/*RGB校准因子*/
    uint32_t sum[3] = {0};
    uint8_t i,j;
    uint16_t data[6][4] = {0};/*数据缓存数组*/
    uint32_t result = 0;/*输出结果*/
    W25Q16_Read_Byte(TCS3472_R_Factor,(uint8_t*)&kr,4);/*从外部Flash中读取数据*/
    W25Q16_Read_Byte(TCS3472_G_Factor,(uint8_t*)&kg,4);
    W25Q16_Read_Byte(TCS3472_B_Factor,(uint8_t*)&kb,4);
    
    for(i = 0;i<6;i++)/*连续采集多次数据,采集数据次数与白平衡校准时一致*/
    {
        TCS3472_Read_RGBData(data[i]);
        General_TIM_Delay_ms(10);
    }
    /*分别计算所采集RGB值的平均值*/
    for(i = 0;i<3;i++)
    {
        for(j = 0;j<6;j++)
        {
            sum[i]+=data[j][i+1];
        }
        sum[i] /= 6;
    }
    
    /*计算最终结果*/
    result |= (uint8_t)(sum[0]*kr);
    result<<=8;
    result |= (uint8_t)(sum[1]*kg);
    result<<=8;
    result |= (uint8_t)(sum[2]*kb);
   return  result;
}


/*************************************************
Function: TCS3472_Init
Description: TCS3472初始化
Input: 无
Output:无
Return: 无
Others: 无
*************************************************/
void TCS3472_Init(void)
{
	/*随后会开启等待模式(WEN)，故设置等待时间寄存器的值*/
	TCS3472_Set_WTIME(WTIME_Used);
    
	/*设置数据转换时间*/
	TCS3472_Set_ATIME(ATIME_Used);
    
    /*设置增益*/
    TCS3472_Set_AGAIN(AGAIN_Used);
    
	/*使能TCS3472，进入AEN*/
	TCS3472_Enable();
	
}


/*************************************************
Function: TCS3472_Set_ATIME
Description: 设置内部积分时间，此值决定所采集值的最大值
Input: atime为向寄存器写入的值
Output:无
Return: 无
Others: 无
*************************************************/
void TCS3472_Set_ATIME(uint8_t atime)
{
	TCS3472_Write_(&atime,1,TCS3472_RGBC_TIMING_REGISTER);
}


/*************************************************
Function: TCS3472_Set_AGAIN
Description: 设置增益，此值决定所采集值的最大值
Input: atime为向寄存器写入的值
Output:无
Return: 无
Others: 无
*************************************************/
void TCS3472_Set_AGAIN(uint8_t again)
{
	TCS3472_Write_(&again,1,TCS3472_CONTROL_REGISTER);
}


