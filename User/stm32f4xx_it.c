/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
  
  

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "led.h"
#include "w25q16.h"
#include "tim.h"
#include "usart.h"
//#include "systick.h"
//uint32_t Start_Addr  =   0x1F0000;/*写入字库时用到的首地址*/

uint8_t SenState = 0;
/*  bit0为TCS3475时间状态，为1表示距离上次测量已满足所设定的时间间隔
    bit1为DHT12时间状态，此值为1时表示距离上次测量DHT12温度已过去超过2秒
    bit2为MAX6675时间状态*/

/*相关时间因子定义，用于定时*/
uint16_t times = 0;/*用于普通延时函数*/
uint16_t times1 = 0;/*用于传感器时间间隔设置*/

/*遥控器输入捕获完成标志，置0说明捕获未完成，置1表示捕获完成，此值在中断服务函数中改变*/
uint8_t IC_Stase = 0;

/*遥控器捕获高低电平标志，此值为1表示上次捕获到上升沿，为0表示未捕获或上次捕获到下降沿*/
uint8_t IC_LandH = 0;

/*遥控器捕获CNT计数*/
uint16_t IC_CNT = 0;

/*遥控状态,此值为标识接受到起始同步信号*/
uint8_t IC_Remote_state = 0;

/*遥控器解码后的数据*/
uint32_t IC_Remote_Read = 0;

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
    LED_ON_OFF();
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
    LED_ON_OFF();
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/*串口中断服务函数*/
void USART1_IRQHandler(void)
{
    
 
//    uint8_t temp = 0;
//    if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET)
//    {
//        /*判断传输是否有进行*/
//        LED_OFF();
//		LED1_ON();
//        
//        temp = USART_ReceiveData(USART_Used);
//        
//        /*写入数据*/
//        W25Q16_Write_Byte(Start_Addr++,&temp,1);
//      
//        /*清除中断标志位*/
//        USART_ClearITPendingBit(USART1,USART_IT_RXNE); 
//    }
	
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
    LED_ON_OFF();
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
    LED_ON_OFF();
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
    LED_ON_OFF();
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
    LED_ON_OFF();
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
    LED_ON_OFF();
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
    LED_ON_OFF();
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    LED_ON_OFF();
	
}



/*次中断服务函数用于一般的延时，每1ms产生一次中断*/
void TIM8_UP_TIM13_IRQHandler(void)
{
    
    /*判断中断是否产生*/
    if(TIM_GetITStatus(General_TIM_Used, TIM_IT_Update)==SET)
    {
        times++; 
        times1++;
        
        if(times1%2==0)
        {
            SenState |= 0x01;/*TCS34725支持较快工作*/
        }
        
        if(times1%2000==0)/*DHT12温度传感器时间间隔2s足够*/
        {  
            SenState |= 0x02;
        }
        
        if(times1%100==0)/*设定MAX6675时间间隔为500ms，此传感器读取太快时数据不准*/
        {
            SenState |= 0x04;
        }
        
        if(times1>60000)/*计数重置*/
        {
            times1 = 0; 
            
        }
        if(times>60000)/*计数重置*/
        {
            times = 0; 
            
        }
        /*清除中断标志*/
        TIM_ClearITPendingBit(General_TIM_Used, TIM_IT_Update);
    }
}



/*遥控器用到的中断服务函数*/
void TIM1_BRK_TIM9_IRQHandler(void)
{
   
    if(TIM_GetITStatus(IC_TIM_Used, IC_TIM_ITS) != RESET)/*判断是否有捕获中断*/
    {
        
        /*判断是上升沿中断还是下降沿中断*/
        /*捕获到上升沿*/
        if(IC_LandH == 0)
        {
            
            /*清零计数并改变相关标志*/       
            TIM_SetCounter(IC_TIM_Used, 0);
            IC_LandH = 1;      
            /*改为捕获下降沿*/
            TIM_OC1PolarityConfig(IC_TIM_Used, TIM_OCPolarity_Low);
            
        }
        /*捕获到下降沿*/
        else if(IC_LandH == 1)
        {
             /*计算脉宽*/
           IC_CNT = TIM_GetCapture1(IC_TIM_Used);
            //printf("%d\n",IC_CNT);
            IC_LandH = 0;
 
            /*改为捕获上升沿*/
            TIM_OC1PolarityConfig(IC_TIM_Used, TIM_OCPolarity_High);
            
                  /*判断遥控信号类型*/
                if(IC_Remote_state==1)/*表示有接收到同步信号*/
                {
                         if((IC_CNT > 25) && (IC_CNT < 55))       /*逻辑 0 是 560us 低+560us 高*/
                        {
                            
                            IC_Remote_Read <<= 1;
                            IC_Remote_Read |= 0;
                        }else if((IC_CNT > 100) && (IC_CNT < 140))/*逻辑 1 是 560us 低+1680us 高*/
                        {
                            
                            IC_Remote_Read <<= 1;
                            IC_Remote_Read |= 1;
                        }
                        else if((IC_CNT > 155) && (IC_CNT < 200))/*结束信号*/
                        {
                            IC_Stase = 1;/*一次信号传输完成*/
                            IC_Remote_state=0;
                        }
                    
                }
                
                       /*表示未收到遥控信号*/
               else if(((IC_CNT > 300) && (IC_CNT < 450)))
                {
                    
                        IC_Remote_state=1;/*表示接收到同步信号，首次发送的是9ms的低电平脉冲，其后是4.5ms的高电平*/
                }
          
        }
         
        TIM_ClearITPendingBit(IC_TIM_Used, IC_TIM_ITS);/*清除中断标志*/
        
   }
    

    
}
/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
