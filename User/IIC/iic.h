

#ifndef _IIC_H_
#define _IIC_H_

#include "stm32f4xx.h"


#define I2C_SDA_GPIO_Pin 	GPIO_Pin_7
#define I2C_SCL_GPIO_Pin 	GPIO_Pin_6
#define I2C_SCL_GPIOPort 	GPIOB
#define I2C_SDA_GPIOPort 	GPIOB


#define GPIO_CLOCK 			RCC_AHB1Periph_GPIOB

#define I2C_SCL_1()  I2C_SCL_GPIOPort->BSRRL =I2C_SCL_GPIO_Pin/* SCL = 1 */
#define I2C_SCL_0()  I2C_SCL_GPIOPort->BSRRH = I2C_SCL_GPIO_Pin/* SCL = 0 */
#define I2C_SDA_1()  I2C_SDA_GPIOPort->BSRRL =I2C_SDA_GPIO_Pin/* SDA = 1 */
#define I2C_SDA_0()	 I2C_SDA_GPIOPort->BSRRH = I2C_SDA_GPIO_Pin/* SDA = 0 */
#define SDA_Vaule 	((I2C_SDA_GPIOPort->IDR & I2C_SDA_GPIO_Pin) != 0)



/*用户自定义函数*/
void IIC_CONF(void);
void Start_Signal(void);
void Stop_Signal(void);
void i2c_Delay(void);
void i2c_Long_Delay(void);
void NAsk_Signal(void);
void Ask_Signal(void);
void I2C_Write_Byte(uint8_t data);
uint8_t I2C_Read_Byte(void);
uint8_t Wait_Ask(void);
void I2C_SDA_OUT(void);
void I2C_SDA_IN(void);


#endif

