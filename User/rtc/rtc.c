#include "rtc.h"
/*************************************************
Function: RTC_UserInit
Description: 实时时钟初始化
Input: 无
Output:无
Return: 无
Others: 无
*************************************************/
void RTC_UserInit(void)
{
    if (RTC_ReadBackupRegister(RTC_BKP_DRX) != 0x2019)/*第一次配置，此处0x2019为一标志*/
    {
        /*配置RTC时钟及分频*/
        RTC_Conf();
        
        /*向后备寄存器写入标志，代表已经设置过时间，再次上电时不用配置*/
        RTC_WriteBackupRegister(RTC_BKP_DRX, 0x2019);
        
        /*设置初始时间*/
        RTC_Set_Time(2019,7,31,6,20,37,00,RTC_H12_PM);
    }
    else/*并非第一次配置*/
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);/*使能PWR时钟*/
        PWR_BackupAccessCmd(ENABLE);/*后备寄存器使能*/
        RTC_WaitForSynchro();/*等待同步*/        
    }
}


/*************************************************
Function: RTC_Conf
Description: 选择RTC时钟源，设置RTC_CLK的分频系数
Input: 无
Output:无
Return: 无
Others: 设置异步分频因子为（127）
                同步分频因子为（255）
                则产生的最终驱动日历的时钟 = 32.768/(127+1)*(255+1)=1HZ
                则每秒更新一次
*************************************************/
void RTC_Conf(void)
{
    RTC_InitTypeDef RTC_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);/*使能 PWR 时钟*/
    PWR_BackupAccessCmd(ENABLE);/*后备寄存器使能*/
    RCC_LSEConfig(RCC_LSE_ON);/* 使能LSE */
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);/* 等待LSE稳定 */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);/*使用32.768khz晶振作为晶振源*/
    RCC_RTCCLKCmd(ENABLE);	/*使能RTC时钟*/
    RTC_WaitForSynchro();   /*等待同步*/
    
    /*开始向RTC_InitStructure结构体赋值*/
    RTC_InitStructure.RTC_AsynchPrediv = RTC_ASYHCHPREDIV;/*异步分频因子*/
    RTC_InitStructure.RTC_SynchPrediv =  RTC_SYHCHPREDIV;/*同步分频因子*/
    RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;/*24时制*/
    RTC_Init(&RTC_InitStructure);
}

/*************************************************
Function: RTC_Set_Time
Description: 时间和日期设置函数
Input: 时间值,其中weekday为周数，可以是1至7，AM_OR_PM表示午前午后，可为RTC_H12_AM或RTC_H12_PM
Output:无
Return: 无
Others: AM_OR_PM表示午前午后，可为RTC_H12_AM或RTC_H12_PM
*************************************************/
void RTC_Set_Time(uint16_t year,uint8_t month,uint8_t date,uint8_t weekday,uint8_t hour,uint8_t min,uint8_t sec,uint8_t AM_OR_PM)
{
    RTC_TimeTypeDef RTC_TimeStructure;
    RTC_DateTypeDef RTC_DateStructure;
    /*设定时钟信息*/
    RTC_TimeStructure.RTC_H12 = AM_OR_PM;/*午前、午后设定*/
    RTC_TimeStructure.RTC_Hours = hour;
    RTC_TimeStructure.RTC_Minutes = min;
    RTC_TimeStructure.RTC_Seconds = sec;
    RTC_SetTime(RTC_Format_BINorBCD, &RTC_TimeStructure);
    
    /*设定年月日信息*/
    RTC_DateStructure.RTC_WeekDay = weekday;
    RTC_DateStructure.RTC_Date = date;
    RTC_DateStructure.RTC_Month = month;
    RTC_DateStructure.RTC_Year = year-48;/*以2018年为基准*/
    RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure);
      
}

