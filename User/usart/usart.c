#include "usart.h"

/*************************************************
Function: USART_GPIO_Conf
Description: 配置USART相关GPIO引脚
Input: 无
Output:无
Return: 无
Others: 无
*************************************************/
void USART_GPIO_Conf(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/*开启GPIO时钟*/
	RCC_AHB1PeriphClockCmd((USART_RX_GPIO_RCC|USART_TX_GPIO_RCC), ENABLE);
	
	/*配置TX*/
	/*启用复用模式*/
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = USART_TX_GPIOPin;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART_TX_GPIOPort,&GPIO_InitStruct);
	
	/*配置RX*/
	/*启用复用模式*/
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = USART_RX_GPIOPin;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART_RX_GPIOPort,&GPIO_InitStruct);
	
	/*选定GPIO复用功能*/
	GPIO_PinAFConfig(USART_RX_GPIOPort,USART_RX_PinSource,USART_AF_FUNCTION);
	GPIO_PinAFConfig(USART_TX_GPIOPort,USART_TX_PinSource,USART_AF_FUNCTION);
	
}

/*************************************************
Function: USART_Conf
Description: 配置USART相关参数
Input: 无
Output:无
Return: 无
Others: 无
*************************************************/
void USART_Conf(void)
{
	USART_InitTypeDef USART_InitStruct;
	
	/*开启USART时钟*/
	USART_RCC_Cmd_FUNCTION(USART_RCC_FUNCTION_PARA,ENABLE);
	
	/*配置相关参数*/
	/*
    向W25Q16写入字库文件时速度太快导致字模数据丢失，
    此处应降低波特率为19200*/
	USART_InitStruct.USART_BaudRate =115200;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = (USART_Mode_Rx|USART_Mode_Tx);
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART_Used,&USART_InitStruct);
	
}


/*************************************************
Function: USART_User_Init
Description: Usart初始化
Input: 无
Output:无
Return: 无
Others: 无
*************************************************/
void USART_User_Init(void)
{
	/*配置中断优先级，不用时注释掉即可*/
//	USART_NVIC_Conf();
	
	USART_GPIO_Conf();
	USART_Conf();
	
	/*中断请求，不用时注释掉即可*/
//	USART_ITConfig(USART_Used,USART_IT_RXNE, ENABLE);
	
	/*使能USART*/
	USART_Cmd(USART_Used,ENABLE);
	
	/*手动清除TC标志位*/
	USART_ClearFlag(USART_Used, USART_FLAG_TC);
}


/*************************************************
Function: USART_Send_Byte
Description: 通过串口发送一个八位数据
Input: data为数据
Output: 无
Return: 无
Others: 无 
*************************************************/
void USART_Send_Byte(uint8_t data)
{
	USART_SendData(USART_Used,data);
	
	/*等待发送完成*/
	while(USART_GetFlagStatus(USART_Used, USART_FLAG_TXE)==0);
}

/*************************************************
Function: USART_Send_NByte
Description: 通过串口发送多个八位数据
Input: data为数据，Num为数据个数
Output: 无
Return: 无
Others: 无 
*************************************************/
void USART_Send_NByte(uint8_t *data,uint8_t Num)
{
    uint8_t i = 0;
    for(i = 0;i < Num;i++)
    {
        USART_Send_Byte(data[i]);
    }
}

/*************************************************
Function: USART_Receive_Byte
Description: 通过串口接受一个八位数据
Input:无
Output: 接收到的数据
Return: 无
Others: 无 
*************************************************/
uint8_t USART_Receive_Byte(void)
{
	uint8_t temp;
	while(USART_GetFlagStatus(USART_Used, USART_FLAG_RXNE)==0);
	temp = (uint8_t)(USART_ReceiveData(USART_Used)&0xFF);
	return temp;
}


/*************************************************
Function: USART_NVIC_Conf
Description: 配置USART产生的中断优先级
Input: 无
Output:无
Return: 无
Others: 无
*************************************************/
void USART_NVIC_Conf(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStruct.NVIC_IRQChannel = USART_IRQ_Channel;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
}
