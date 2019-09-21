#include  "tim.h"

/*在中断服务函数中更新的变量，用于计次*/
extern uint16_t times;

/*************************************************
Function: General_TIM_Conf
Description: 配置通用定时器中断优先级
Input:无
Output:无
Return: 无
Others: 此通用定时器用于延时及相关时间
*************************************************/
void General_TIM_NVIC_Conf(void)
{
    NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStruct.NVIC_IRQChannel = General_TIM_IRQ_Channel;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
}

/*************************************************
Function: General_TIM_Conf
Description: 配置通用定时器
Input:无
Output:无
Return: 无
Others: 无
*************************************************/
void General_TIM_Conf(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
    
    /*开启TIM时钟*/
    General_TIM_Clock_Cmd(General_TIM_RCC,ENABLE);
    
    /*参数配置*/
    TIM_TimeBaseInitStruct.TIM_Prescaler = General_TIM_Prescaler_Used;/*预分频因子*/
    TIM_TimeBaseInitStruct.TIM_Period = General_TIM_Period_Used;/*计数周期*/
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
    TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
     
    TIM_TimeBaseInit(General_TIM_Used, &TIM_TimeBaseInitStruct);
    
}



/*************************************************
Function: General_TIM_Init
Description: 通用定时器初始化
Input: 无
Output:无
Return: 无
Others: 无
*************************************************/
void General_TIM_Init(void)
{
    General_TIM_NVIC_Conf();
    
    /*定时器参数配置*/
    General_TIM_Conf();
    
    TIM_ITConfig(General_TIM_Used, TIM_IT_Update, ENABLE);
    
    /*使能定时器*/
    TIM_Cmd(General_TIM_Used, ENABLE);
    
    /*初始时清除中断标志*/
    TIM_ClearITPendingBit(General_TIM_Used, TIM_IT_Update);
    
}

/*************************************************
Function: General_TIM_Delay_ms
Description: 使用通用定时器进行毫秒级延时
Input: n为延时毫秒数
Output:无
Return: 无
Others: 无
*************************************************/
void General_TIM_Delay_ms(uint16_t n)
{
    times = 0;/*先将times置零*/
    while(times<n)
    {
        /*time每1ms自加一次，当计数次数小于n时，等待*/
    }   
}


/*************************************************
Function: IC_TIM_GPIO_Conf
Description: 输入捕获定时器GPIO初始化
Input: 无
Output:无
Return: 无
Others: 无
*************************************************/
void IC_TIM_GPIO_Conf(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
	/*开启GPIO时钟*/ 
	RCC_AHB1PeriphClockCmd(IC_TIM_GPIO_RCC,ENABLE); 
	
	/*配置GPIO相关参数*/
	GPIO_InitStructure.GPIO_Pin = IC_TIM_GPIOPin;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(IC_TIM_GPIOPort,&GPIO_InitStructure);
    
    /*复用功能配置*/
    GPIO_PinAFConfig(IC_TIM_GPIOPort,IC_TIM_GPIO_Pinsource,IC_TIM_GPIO_AF); 
    
}


/*************************************************
Function: IC_TIM_NVIC_Conf
Description: 配置输入捕获定时器中断优先级
Input:  无
Output: 无
Return: 无
Others: 无
*************************************************/
void IC_TIM_NVIC_Conf(void)
{
    NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStruct.NVIC_IRQChannel = IC_TIM_IRQ_Channel;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;/*输入捕获中断优先级设为最高*/
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
}


/*************************************************
Function: IC_TIM_Conf
Description: 配置输入捕获定时器
Input:无
Output:无
Return: 无
Others: 无
*************************************************/
void IC_TIM_Conf(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
    TIM_ICInitTypeDef  TIM_ICInitStructure;
    
    /*开启TIM时钟*/
    IC_TIM_Clock_Cmd(IC_TIM_RCC,ENABLE);
    
    /*基本时间参数配置*/
    TIM_TimeBaseInitStruct.TIM_Prescaler = IC_TIM_Prescaler_Used;
    TIM_TimeBaseInitStruct.TIM_Period = IC_TIM_Period_Used;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;/*向上计数*/
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;/*不使用重复计数*/
    TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseInit(IC_TIM_Used, &TIM_TimeBaseInitStruct);
    
    /*输入捕获配置*/
    TIM_ICInitStructure.TIM_Channel = IC_TIM_Channel;
    TIM_ICInitStructure.TIM_ICFilter = 0x03;      /*8个定时器时钟周期滤波，连续采集8次都是高电平才有效*/
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;/*初次捕获上升沿*/
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;/*没一个边沿都要检测*/
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;/*直连模式*/
    TIM_ICInit(IC_TIM_Used, &TIM_ICInitStructure);
    
    
}

/*************************************************
Function: IC_TIM_Init
Description: 输入捕获定时器初始化
Input: 无
Output:无
Return: 无
Others: 无
*************************************************/
void IC_TIM_Init(void)
{
   
    IC_TIM_NVIC_Conf();/*配置中断优先级*/
    
    IC_TIM_GPIO_Conf();/*端口配置*/
        
    IC_TIM_Conf();/*定时器参数配置*/
    
    TIM_ITConfig(IC_TIM_Used, IC_TIM_ITS, ENABLE);/*中断请求*/

    TIM_Cmd(IC_TIM_Used,ENABLE);/*使能定时器*/
      
    TIM_ClearITPendingBit(IC_TIM_Used, TIM_IT_Update);/*初始时清除中断标志*/
    
}


