#ifndef _LED_H_
#define _LED_H_
#include "stm32f4xx.h"

/*GPIO端口及引脚定义*/
#define LED1_GPIO_Port 			GPIOF
#define LED1_GPIO_Pin			GPIO_Pin_9

#define LED2_GPIO_Port 			GPIOF
#define LED2_GPIO_Pin			GPIO_Pin_10

/*函数参数定义*/
#define LED1_RCC_Periph 		RCC_AHB1Periph_GPIOF
#define LED2_RCC_Periph 		RCC_AHB1Periph_GPIOF



/*用户自定义函数*/
void LED_GPIO_Conf(void);
void LED_OFF(void);
void LED2_ON(void);
void LED1_ON(void);
void LED_ON_OFF(void);





#endif
