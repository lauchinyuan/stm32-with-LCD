#include "host.h"

/*************************************************
Function: HOST_Send_float
Description: 发送单个浮点型数据到匿名上位机
Input: data为浮点型数据，fun为指定功能字，用于区别不同数据帧
Output: 无
Return: 无
Others: 无 
*************************************************/
void HOST_Send_float(float data,uint8_t fun)
{
    uint8_t sum = 0;/*校验和*/
	uint8_t i = 0;
	uint8_t cnt = 0;
    unsigned char *p;
    uint8_t data_to_send[23] = {0};/*此处必要时可增大空间*/
	
	data_to_send[cnt++]=0xAA;/*帧头为0xAAAA*/
	data_to_send[cnt++]=0xAA;
    
	data_to_send[cnt++]=fun;/*功能字，区别接收的数据类型*/
    
	data_to_send[cnt++]=0;/*需要发送数据的字节数，暂时给0，后面计算完数据数量再赋值*/
	
    p=(unsigned char *)&data;/*flot数据占据4个字节*/
    data_to_send[cnt++]=(unsigned char)(*(p+3));
    data_to_send[cnt++]=(unsigned char)(*(p+2));
    data_to_send[cnt++]=(unsigned char)(*(p+1));
    data_to_send[cnt++]=(unsigned char)(*(p+0));
	
	data_to_send[3] = cnt-4;/*将数据字节个数数赋值给数据帧第四位*/
	
	for(i=0;i<cnt;i++)/*计算校验和*/
    {
        sum += data_to_send[i];
    }
		
	data_to_send[cnt++] = sum;/*最后一位是校验位*/
	
    USART_Send_NByte(data_to_send, cnt);/*使用串口发送数据*/

}


/*************************************************
Function: HOST_Send_32bit
Description: 发送单个32位数据到匿名上位机
Input:  data为32位数据,fun为指定功能字，用于区别不同数据帧
Output: 无
Return: RGB888数据
Others: 无 
*************************************************/
void HOST_Send_32bit(uint32_t data,uint8_t fun)
{
    uint8_t sum = 0;/*校验和*/
	uint8_t i = 0;
	uint8_t cnt = 0;
    unsigned char *p;
    uint8_t data_to_send[23] = {0};/*此处必要时可增大空间*/
	
	data_to_send[cnt++]=0xAA;/*帧头为0xAAAA*/
	data_to_send[cnt++]=0xAA;
    
	data_to_send[cnt++]=fun;/*功能字，区别接收的数据类型*/
    
	data_to_send[cnt++]=0;/*需要发送数据的字节数，暂时给0，后面计算完数据数量再赋值*/
	
    p=(unsigned char *)&data;/*32位数据占据4个字节*/
    data_to_send[cnt++]=(unsigned char)(*(p+3));
    data_to_send[cnt++]=(unsigned char)(*(p+2));
    data_to_send[cnt++]=(unsigned char)(*(p+1));
    data_to_send[cnt++]=(unsigned char)(*(p+0));
	
	data_to_send[3] = cnt-4;/*将数据字节个数数赋值给数据帧第四位*/
	
	for(i=0;i<cnt;i++)/*计算校验和*/
    {
        sum += data_to_send[i];
    }
		
	data_to_send[cnt++] = sum;/*最后一位是校验位*/
	
    USART_Send_NByte(data_to_send, cnt);/*使用串口发送数据*/
}



