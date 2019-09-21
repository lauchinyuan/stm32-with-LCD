#ifndef _USART_H_
#define _USART_H_
#include "stm32f4xx.h"

/*引脚定义*/
#define USART_RX_GPIOPort		GPIOA
#define USART_RX_GPIOPin		GPIO_Pin_10

#define USART_TX_GPIOPort		GPIOA
#define USART_TX_GPIOPin		GPIO_Pin_9

#define USART_RX_GPIO_RCC		RCC_AHB1Periph_GPIOA
#define USART_TX_GPIO_RCC		RCC_AHB1Periph_GPIOA

#define USART_AF_FUNCTION		GPIO_AF_USART1

#define USART_TX_PinSource		GPIO_PinSource9
#define USART_RX_PinSource		GPIO_PinSource10

#define USART_Used				USART1
#define USART_IRQ_Channel		USART1_IRQn

/*库函数及参数定义*/
#define USART_RCC_Cmd_FUNCTION	RCC_APB2PeriphClockCmd
#define USART_RCC_FUNCTION_PARA	RCC_APB2Periph_USART1


/*用户自定义函数*/
void USART_GPIO_Conf(void);
void USART_Conf(void);
void USART_User_Init(void);
void USART_Send_Byte(uint8_t data);
void USART_Send_Char(uint8_t* ch);
uint8_t USART_Receive_Byte(void);
void USART_NVIC_Conf(void);
void USART_Send_NByte(uint8_t *data,uint8_t Num);


#endif /*_USART_H_*/
