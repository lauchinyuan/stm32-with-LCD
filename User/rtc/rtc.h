#ifndef _RTC_H_
#define _RTC_H_
#include "stm32f4xx.h"

/*结构体定义*/
typedef struct RCT_Time
{
    uint16_t tem_year;/*临时时间值，用于更改时间*/
    uint8_t tem_month;
    uint8_t tem_date;
    uint8_t tem_hour;
    uint8_t tem_min;
    uint8_t tem_week;
} RCT_Temp;

/*相关宏定义*/
#define RTC_CLOCK_SOURCE_LSI     /*使用LSE时钟*/
#define RTC_ASYHCHPREDIV       0X7F/*异步分频因子*/
#define RTC_SYHCHPREDIV        0XFF/*同步分频因子*/
#define RTC_Format_BINorBCD    RTC_Format_BIN/*时间格式*/
#define RTC_BKP_DRX            RTC_BKP_DR0/*使用DR0作为备份寄存器*/


/*用户自定义函数*/
void RTC_UserInit(void);
void RTC_NVIC_Conf(void);
void RTC_Conf(void);
void RTC_Set_Time(uint16_t year,uint8_t month,uint8_t date,uint8_t weekday,uint8_t hour,uint8_t min,uint8_t sec,uint8_t AM_OR_PM);
void RTC_LCD_Show_Time(void);
#endif

