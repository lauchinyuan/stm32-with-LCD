#ifndef  _TIM_H_
#define  _TIM_H_
#include "stm32f4xx.h"

/*此文件中通用定时器13用于延时函数，
通用定时器9用于遥控脉冲信号的输入捕获*/


/*通用定时器相关定义*/
#define General_TIM_Used            TIM13
#define General_TIM_IRQ_Channel     TIM8_UP_TIM13_IRQn

#define General_TIM_Clock_Cmd       RCC_APB1PeriphClockCmd
#define General_TIM_RCC             RCC_APB1Periph_TIM13

/*输入捕获定时器相关定义*/
#define IC_TIM_Used            TIM9
#define IC_TIM_IRQ_Channel     TIM1_BRK_TIM9_IRQn

#define IC_TIM_Clock_Cmd       RCC_APB2PeriphClockCmd
#define IC_TIM_RCC             RCC_APB2Periph_TIM9

#define IC_TIM_GPIOPort        GPIOE
#define IC_TIM_GPIOPin         GPIO_Pin_5
#define IC_TIM_GPIO_RCC        RCC_AHB1Periph_GPIOE
#define IC_TIM_GPIO_Pinsource  GPIO_PinSource5
#define IC_TIM_GPIO_AF         GPIO_AF_TIM9
#define IC_TIM_Channel         TIM_Channel_1
#define IC_TIM_ITS             TIM_IT_CC1

/*参数宏定义*/

/*设置计数频率为10000HZ*/

/*由于工程未更改 system_stm32f4xx.c中PLL_M中的值，系统时钟并非168MHZ，故此处值不为16800-1*/
#define General_TIM_Prescaler_Used  (2800-1)
#define IC_TIM_Prescaler_Used       (700-1)


/*一般定时实现毫秒级定时，让定时器每一毫秒产生一次更新中断*/
#define General_TIM_Period_Used     (10-1)

/*捕获定时器溢出时间为10ms*/
#define IC_TIM_Period_Used          (1000-1)

/*用户自定义函数*/
void General_TIM_Conf(void);
void General_TIM_NVIC_Conf(void);
void General_TIM_Init(void);
void General_TIM_Delay_ms(uint16_t n);
void IC_TIM_GPIO_Conf(void);
void IC_TIM_NVIC_Conf(void);
void IC_TIM_Conf(void);
void IC_TIM_Init(void);

#endif
