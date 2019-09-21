#ifndef _W25Q16_H_
#define _W25Q16_H_
#include "stm32f4xx.h"

/*引脚相关定义*/
#define W25Q16_CS_GPIOPort          GPIOB
#define W25Q16_CS_GPIOPin           GPIO_Pin_14
#define W25Q16_GPIO_RCC             (RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC)

#define W25Q16_SPI_AF				GPIO_AF_SPI1

#define W25Q16_SPI_SCK_Pin 		    GPIO_Pin_3
#define W25Q16_SPI_SCK_Port 		GPIOB
#define W25Q16_SPI_SCK_PinSource	GPIO_PinSource3

#define W25Q16_SPI_MISO_Pin 		GPIO_Pin_4
#define W25Q16_SPI_MISO_Port 		GPIOB
#define W25Q16_SPI_MISO_PinSource	GPIO_PinSource4

#define W25Q16_SPI_MOSI_Pin 		GPIO_Pin_5
#define W25Q16_SPI_MOSI_Port 		GPIOB
#define W25Q16_SPI_MOSI_PinSource	GPIO_PinSource5


#define W25Q16_SPI_Used 			SPI1
#define W25Q16_SPI_RCC_Cmd			RCC_APB2PeriphClockCmd
#define W25Q16_RCC_SPI 			    RCC_APB2Periph_SPI1







/*用户自定义函数*/
static void W25Q16_GPIO_Conf(void);
void W25Q16_SPI_Conf(void);
static uint16_t W25Q16_Read_Write(uint16_t Data);
static void W25Q16_Write_Enable(void);
static void W25Q16_Wait(void);
void W25Q16_Read_Byte(uint32_t addr,uint8_t *datasto,uint8_t num);
void W25Q16_Sector_Erase(uint32_t addr);
void W25Q16_Write_Byte(uint32_t addr,uint8_t *datasend,uint8_t num);

/*片选引脚操作*/
#define W25Q16_CS_Hight			     GPIO_SetBits(W25Q16_CS_GPIOPort,W25Q16_CS_GPIOPin)
#define W25Q16_CS_Low				 GPIO_ResetBits(W25Q16_CS_GPIOPort,W25Q16_CS_GPIOPin)

#endif

