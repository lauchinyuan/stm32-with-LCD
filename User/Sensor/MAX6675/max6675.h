#ifndef  _MAX_6675_H_
#define  _MAX_6675_H_
#include "stm32f4xx.h"
#include "tim.h"


/*宏定义*/
#define DUMMY 		0x0000


/*引脚相关*/
#define MAX6675_RCC_GPIO 			(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC)
#define MAX6675_SPI_AF				GPIO_AF_SPI2

#define MAX6675_SPI_SCK_Pin 		GPIO_Pin_10
#define MAX6675_SPI_SCK_Port 		GPIOB
#define MAX6675_SPI_SCK_PinSource	GPIO_PinSource10

#define MAX6675_SPI_MISO_Pin 		GPIO_Pin_2
#define MAX6675_SPI_MISO_Port 		GPIOC
#define MAX6675_SPI_MISO_PinSource	GPIO_PinSource2

#define MAX6675_SPI_SOFT_CS_Pin 	GPIO_Pin_4
#define MAX6675_SPI_SOFT_CS_Port 	GPIOC

#define MAX6675_SPI_Used 			SPI2
#define MAX6675_SPI_RCC_Cmd			RCC_APB1PeriphClockCmd
#define MAX6675_RCC_SPI 			RCC_APB1Periph_SPI2

#define MAX6675_CSS_Hight			GPIO_SetBits(MAX6675_SPI_SOFT_CS_Port,MAX6675_SPI_SOFT_CS_Pin)
#define MAX6675_CSS_Low				GPIO_ResetBits(MAX6675_SPI_SOFT_CS_Port,MAX6675_SPI_SOFT_CS_Pin)


/*用户自定义函数*/
uint16_t MAX6675_ReadData(void);
float MAX6675_DealData(uint16_t data);
void MAX6675_SPI_Conf(void);
void MAX6675_SPI_GPIO_Conf(void);
uint16_t MAX6675_SPI_Read_Write(uint16_t Data);
uint16_t MAX6675_AVE(void);


#endif /*_MAX_6675_H_*/ 

