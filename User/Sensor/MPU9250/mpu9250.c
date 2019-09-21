#include "mpu9250.h"
#include "led.h"
#include "math.h"
#include "inoutput.h"

/*************************************************
Function: MPU9250_Init
Description: MPU9250初始化
Input: 无
Output:无
Return: 无
Others: 无
*************************************************/
void MPU9250_Init(void)
{
    MPU9250_Write_Byte(MPU9250_PWR_MNG_REG,0x00);/*唤醒MPU9250*/
    MPU9250_Write_Byte(MPU9250_SMPLRT_DIV_REG,0x07); /*设置采样速率分频，此处为8分频*/
    MPU9250_Write_Byte(MPU9250_ACC_CONF2_REG,0x06); /*设置滤波器*/
    MPU9250_Write_Byte(MPU9250_CONF_REG,0x06);
    MPU9250_Write_Byte(MPU9250_ACC_CONF1_REG,0x00);/*配置加速度计量程为+-2g*/
}

/*************************************************
Function: MPU9250_Write_Byte
Description: 从寄存器起始地址开始写入num个字节数据
Input: Addr为指定地址，data为多字节数据首地址，num为数据字节数
Output:无
Return: 无
Others: 无
*************************************************/
void MPU9250_Write(uint8_t Addr,uint8_t* data,uint8_t num)
{
    uint8_t i;
	Start_Signal();
	I2C_Write_Byte(MPU9250_WRITE_ADDR);/*发送设备写指令*/
    
	while(!Wait_Ask());/*等待响应信号*/
    LED2_ON();
	I2C_Write_Byte(Addr);/*写入寄存器寻址指令*/
	while(!Wait_Ask());
	for(i = 0;i < num;i++)
	{
		I2C_Write_Byte(data[i]);
		while(!Wait_Ask());
	}
	Stop_Signal();
}


/*************************************************
Function: MPU9250_Write_Byte
Description: 从寄存器起始地址开始写入单个字节数据
Input: Addr为指定地址，data为单字节数据
Output:无
Return: 无
Others: 无
*************************************************/
void MPU9250_Write_Byte(uint8_t Addr,uint8_t data)
{
    
	MPU9250_Write(Addr,&data,1);
    
}

/*************************************************
Function: MPU9250_Read
Description: 从寄存器起始地址开始连续读取num个字节数据
Input: Addr为寄存器起始地址，num为要读取的字节数
Output:读取结果存储在datasto数组中
Return: 无
Others: 无
*************************************************/
void MPU9250_Read(uint8_t Addr,uint8_t *datasto,uint8_t num )
{
    uint8_t i;
	Start_Signal();
    
	I2C_Write_Byte(MPU9250_WRITE_ADDR);/*发送设备写指令*/   
	while(!Wait_Ask());
    
	I2C_Write_Byte(Addr);/*写入寄存器寻址指令*/
	while(!Wait_Ask());
    
	Start_Signal();/*第二次起始信号*/	
	
	I2C_Write_Byte(MPU9250_READ_ADDR);/*发送设备读指令*/
	while(!Wait_Ask());
	
	/*接收数据*/
	for(i=0;i<num;i++)
	{
		datasto[i] = I2C_Read_Byte();
		if(i==num - 1)/*最后一位发送非应答信号*/
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
Function: MPU9250_Cal_Accel
Description: 读取并处理加速度计数据（将8位原始数据转换为16位数据）
Input: 无
Output:无
Return: 返回16位加速度计数据
Others: 无
*************************************************/
int16_t MPU9250_Cal_Accel(void)
{
    int16_t dataout = 0;
    
    uint8_t data[2] = {0};
    
    /*分别读取高低8位数据*/
    MPU9250_Read(MPU9250_ACCEL_YOUT_H,data,2);
    
    /*拼接成16位数据*/
    dataout = (((uint16_t)(data[0]<<8))|data[1]);
    return dataout;
}

/*************************************************
Function: MPU9250_Offset
Description: 对加速度计输出的值进行误差补偿，并将补偿值存储于外部Flash中
Input: 无
Output:无
Return: 无
Others: 调用此函数时应确保传感器平放，且保持静止
*************************************************/
void MPU9250_Offset(void)
{
    int16_t Accel_Y_Offset = -30000;/*用于校准的值，此值从-30000开始遍历，直到满足(MPUdata+Accel_Y_Offset)=0*/    
    int16_t MPUdata = 0;/*MPU9250原始16位数据*/
    MPUdata = MPU9250_Cal_Accel();/*获取原始值*/
    while((MPUdata+Accel_Y_Offset)!=0)/*校正加速度计Y轴*/
    {
        Accel_Y_Offset++;/*不满足条件则自加*/
        if(Accel_Y_Offset>30000)/*超出校准范围*/
        {
            printf("加速度校正失败\n");
            break;
        }
    }
    
    /*将校正因子存储在外部Flash中，以便调用*/
    W25Q16_Sector_Erase(MPU9250_ACC_YOFFSET_ARRD);/*擦除扇区*/
    
    /*所得每个值占2个字节空间*/
    W25Q16_Write_Byte(MPU9250_ACC_YOFFSET_ARRD,(uint8_t*)&Accel_Y_Offset,2); 
    
}


/*************************************************
Function: MPU9250_Get_Angle
Description: 简单计算重力加速度夹角(角度)
Input: 无
Output: data为存储角度变量的结构体首地址
Return: 无
Others: 无 
*************************************************/
void MPU9250_Get_Angle(MPU9250_Angle *data)
{  
    int16_t MPUdata = 0;
    int16_t MPUAccl_Y_Offset = 0;/*校正值*/

    W25Q16_Read_Byte(MPU9250_ACC_YOFFSET_ARRD,(uint8_t*)&MPUAccl_Y_Offset,2);/*读取校正值*/

    MPUdata = MPU9250_filter();/*通过滤波获取16位数据*/
    
    /* 结合三角函数利用加速度计算x, y, z 轴倾角，得到弧度值*/
    data->Angle_Y = asin((MPUdata + MPUAccl_Y_Offset) / 16384.0);
    
    data->Angle_Y  *=  DEG_to_RAD;/* 弧度值转换为角度值 */
} 


/*************************************************
Function: MPU9250_filter
Description: 对原始数据进行滑动滤波处理
Input: 无
Output: 无
Return: 滤波处理后的数据（当前滤波窗口平均值）
Others: 此函数前N个输出值不准确 
*************************************************/
#define N 9/*宏定义*/
int16_t value_buf[N]; 
extern uint8_t q = 0;
int16_t MPU9250_filter(void)
{
     uint8_t count = 0;
     int32_t sum=0;  
     int16_t temp=0;
     value_buf[q++] = MPU9250_Cal_Accel();/*逐个赋值*/
    if (q == N) /*超出范围，从数组中第一个数据重新赋值*/
    {
         q = 0;
    }
    for (count=0;count<N;count++)
    {
        sum += value_buf[count];/*求当前滤波窗口总和*/
    }
    temp=(int16_t)(sum/N);/*计算当前窗口平均值*/
    return temp;/*返回当前窗口平均值*/
}
#undef N/*取消宏定义*/
