#include "fsmc.h"
/*************************************************
Function: FSMC_GPIO_Conf
Description: 配置FSMC用到的GPIO
Input: 无
Output:无
Return: 无
Others: 无
*************************************************/
void FSMC_GPIO_Conf(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	/*开启GPIO时钟*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE|
							RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOG,ENABLE);
	/*配置FSMC_D0*/
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = FSMC_D0_Pin;
	GPIO_Init(FSMC_D0_Port, &GPIO_InitStruct);
	GPIO_PinAFConfig(FSMC_D0_Port,FSMC_D0_PinSource, FSMC_AF);
	
	/*配置FSMC_D1*/
	GPIO_InitStruct.GPIO_Pin = FSMC_D1_Pin;
	GPIO_Init(FSMC_D1_Port, &GPIO_InitStruct);
	GPIO_PinAFConfig(FSMC_D1_Port,FSMC_D1_PinSource, FSMC_AF);
	
	
	/*配置FSMC_D2*/
	GPIO_InitStruct.GPIO_Pin = FSMC_D2_Pin;
	GPIO_Init(FSMC_D2_Port, &GPIO_InitStruct);
	GPIO_PinAFConfig(FSMC_D2_Port,FSMC_D2_PinSource, FSMC_AF);
	
	
	/*配置FSMC_D3*/
	GPIO_InitStruct.GPIO_Pin = FSMC_D3_Pin;
	GPIO_Init(FSMC_D3_Port, &GPIO_InitStruct);
	GPIO_PinAFConfig(FSMC_D3_Port,FSMC_D3_PinSource, FSMC_AF);
	
	
	/*配置FSMC_D4*/
	GPIO_InitStruct.GPIO_Pin = FSMC_D4_Pin;
	GPIO_Init(FSMC_D4_Port, &GPIO_InitStruct);
	GPIO_PinAFConfig(FSMC_D4_Port,FSMC_D4_PinSource, FSMC_AF);
	
	
	/*配置FSMC_D5*/
	GPIO_InitStruct.GPIO_Pin = FSMC_D5_Pin;
	GPIO_Init(FSMC_D5_Port, &GPIO_InitStruct);
	GPIO_PinAFConfig(FSMC_D5_Port,FSMC_D5_PinSource, FSMC_AF);

	/*配置FSMC_D6*/
	GPIO_InitStruct.GPIO_Pin = FSMC_D6_Pin;
	GPIO_Init(FSMC_D6_Port, &GPIO_InitStruct);
	GPIO_PinAFConfig(FSMC_D6_Port,FSMC_D6_PinSource, FSMC_AF);

	/*配置FSMC_D7*/
	GPIO_InitStruct.GPIO_Pin = FSMC_D7_Pin;
	GPIO_Init(FSMC_D7_Port, &GPIO_InitStruct);
	GPIO_PinAFConfig(FSMC_D7_Port,FSMC_D7_PinSource, FSMC_AF);
	
	/*配置FSMC_D8*/
	GPIO_InitStruct.GPIO_Pin = FSMC_D8_Pin;
	GPIO_Init(FSMC_D8_Port, &GPIO_InitStruct);
	GPIO_PinAFConfig(FSMC_D8_Port,FSMC_D8_PinSource, FSMC_AF);
	
	
	/*配置FSMC_D9*/
	GPIO_InitStruct.GPIO_Pin = FSMC_D9_Pin;
	GPIO_Init(FSMC_D9_Port, &GPIO_InitStruct);
	GPIO_PinAFConfig(FSMC_D9_Port,FSMC_D9_PinSource, FSMC_AF);
	
	
	/*配置FSMC_D10*/
	GPIO_InitStruct.GPIO_Pin = FSMC_D10_Pin;
	GPIO_Init(FSMC_D10_Port, &GPIO_InitStruct);
	GPIO_PinAFConfig(FSMC_D10_Port,FSMC_D10_PinSource, FSMC_AF);
	
	
	/*配置FSMC_D11*/
	GPIO_InitStruct.GPIO_Pin = FSMC_D11_Pin;
	GPIO_Init(FSMC_D11_Port, &GPIO_InitStruct);
	GPIO_PinAFConfig(FSMC_D11_Port,FSMC_D11_PinSource, FSMC_AF);
	
	
	/*配置FSMC_D12*/
	GPIO_InitStruct.GPIO_Pin = FSMC_D12_Pin;
	GPIO_Init(FSMC_D12_Port, &GPIO_InitStruct);
	GPIO_PinAFConfig(FSMC_D12_Port,FSMC_D12_PinSource, FSMC_AF);
	
	
	/*配置FSMC_D13*/
	GPIO_InitStruct.GPIO_Pin = FSMC_D13_Pin;
	GPIO_Init(FSMC_D13_Port, &GPIO_InitStruct);
	GPIO_PinAFConfig(FSMC_D13_Port,FSMC_D13_PinSource, FSMC_AF);
	
	
	/*配置FSMC_D14*/
	GPIO_InitStruct.GPIO_Pin = FSMC_D14_Pin;
	GPIO_Init(FSMC_D14_Port, &GPIO_InitStruct);
	GPIO_PinAFConfig(FSMC_D14_Port,FSMC_D14_PinSource, FSMC_AF);
	
	
	/*配置FSMC_D15*/
	GPIO_InitStruct.GPIO_Pin = FSMC_D15_Pin;
	GPIO_Init(FSMC_D15_Port, &GPIO_InitStruct);
	GPIO_PinAFConfig(FSMC_D15_Port,FSMC_D15_PinSource, FSMC_AF);
	
	/*配置FSMC_A6*/
	GPIO_InitStruct.GPIO_Pin = FSMC_A6_Pin;
	GPIO_Init(FSMC_A6_Port, &GPIO_InitStruct);
	GPIO_PinAFConfig(FSMC_A6_Port,FSMC_A6_PinSource, FSMC_AF);
	
	/*配置FSMC_NWE*/
	GPIO_InitStruct.GPIO_Pin = FSMC_NWE_Pin;
	GPIO_Init(FSMC_NWE_Port, &GPIO_InitStruct);
	GPIO_PinAFConfig(FSMC_NWE_Port,FSMC_NWE_PinSource, FSMC_AF);
	
	/*配置FSMC_NOE*/
	GPIO_InitStruct.GPIO_Pin = FSMC_NOE_Pin;
	GPIO_Init(FSMC_NOE_Port, &GPIO_InitStruct);
	GPIO_PinAFConfig(FSMC_NOE_Port,FSMC_NOE_PinSource, FSMC_AF);
	
	/*配置FSMC_NE4*/
	GPIO_InitStruct.GPIO_Pin = FSMC_NE4_Pin;
	GPIO_Init(FSMC_NE4_Port, &GPIO_InitStruct);
	GPIO_PinAFConfig(FSMC_NE4_Port,FSMC_NE4_Pinsource, FSMC_AF);
	
}

/*************************************************
Function: FSMC_Conf
Description: 配置驱动液晶显示器的FSMC
Input: 无
Output:无
Return: 无
Others: 无
*************************************************/
void FSMC_Conf(void)
{
	/*此处参照FSMC控制NOR FLASH的时序对LCD显示器进行操作*/
	FSMC_NORSRAMTimingInitTypeDef FSMC_TimingInitStruct;
	FSMC_NORSRAMInitTypeDef FSMC_InitStruct;
	
	/*配置引脚*/
	FSMC_GPIO_Conf();
	
	/*开启FSMC时钟*/
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);
	
	/*配置Timeing结构体成员值*/
	
	/*使用模式B控制LCD*/
	FSMC_TimingInitStruct.FSMC_AccessMode = FSMC_AccessMode_B;
	
	/*地址建立时间，适应LCD显示器的值（5个HCLK时钟周期）*/
	FSMC_TimingInitStruct.FSMC_AddressSetupTime = 0x04;
	/*数据建立时间，适应LCD显示器的值（5个HCLK时钟周期）*/
	FSMC_TimingInitStruct.FSMC_DataSetupTime = 0x04;
	
	
	/*以下异步模式中未用到*/
	FSMC_TimingInitStruct.FSMC_AddressHoldTime = 0x00;
	FSMC_TimingInitStruct.FSMC_BusTurnAroundDuration = 0x00;
	FSMC_TimingInitStruct.FSMC_CLKDivision = 0x00;
	FSMC_TimingInitStruct.FSMC_DataLatency = 0x00;
	
	/*结构体写入完成，开始配置初始化结构体的值*/
	FSMC_InitStruct.FSMC_Bank = FSMC_Bank1_NORSRAMx;
	FSMC_InitStruct.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_InitStruct.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	FSMC_InitStruct.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
	FSMC_InitStruct.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
	FSMC_InitStruct.FSMC_MemoryType = FSMC_MemoryType_NOR;
	FSMC_InitStruct.FSMC_ReadWriteTimingStruct = &FSMC_TimingInitStruct;
	FSMC_InitStruct.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_InitStruct.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_InitStruct.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_InitStruct.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_InitStruct.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_InitStruct.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	FSMC_InitStruct.FSMC_WriteTimingStruct = &FSMC_TimingInitStruct;
	FSMC_NORSRAMInit(&FSMC_InitStruct);
	
	/*使能FSMC*/
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAMx,ENABLE);
	
}




