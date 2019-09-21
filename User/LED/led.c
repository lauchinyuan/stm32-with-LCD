#include "led.h"
#include "tim.h"

/*************************************************
Function: LED_GPIO_Conf
Description: 配置LED相关GPIO
Input: 无
Output:无
Return: 无
Others: 无
*************************************************/
void LED_GPIO_Conf(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_AHB1PeriphClockCmd(LED1_RCC_Periph,ENABLE);/*开启GPIO时钟*/
	RCC_AHB1PeriphClockCmd(LED2_RCC_Periph,ENABLE);
	
	/*配置GPIO相关参数，先配置LED1*/
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = LED1_GPIO_Pin;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Low_Speed;

	GPIO_Init(LED1_GPIO_Port,&GPIO_InitStruct);
	
	/*配置LED2*/
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = LED2_GPIO_Pin;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Low_Speed;
	
	GPIO_Init(LED2_GPIO_Port,&GPIO_InitStruct);

	LED_OFF();/*默认关灯*/
}


/*************************************************
Function: LED_OFF
Description: 熄灭所有LED
Input: 无
Output:无
Return: 无
Others: 无
*************************************************/
void LED_OFF(void)
{
	GPIO_SetBits(LED2_GPIO_Port,LED2_GPIO_Pin);
	GPIO_SetBits(LED1_GPIO_Port,LED1_GPIO_Pin);
}


/*************************************************
Function: LED2_ON
Description: 点亮LED2
Input: 无
Output:无
Return: 无
Others: 无
*************************************************/
void LED2_ON(void)
{
	GPIO_ResetBits(LED2_GPIO_Port, LED2_GPIO_Pin);
}
	
/*************************************************
Function: LED1_ON
Description: 点亮LED1
Input: 无
Output:无
Return: 无
Others: 无
*************************************************/
void LED1_ON(void)
{
	GPIO_ResetBits(LED1_GPIO_Port, LED1_GPIO_Pin);
}


/*************************************************
Function: LED_ON_OFF
Description: LED闪烁
Input: 无
Output:无
Return: 无
Others: 无
*************************************************/
void LED_ON_OFF(void)
{
      while (1)
  {
      LED_OFF();
      General_TIM_Delay_ms(200);
      LED1_ON();
      LED2_ON();
      General_TIM_Delay_ms(200);
      
  }
    
}

