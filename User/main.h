#ifndef _MAIN_H_
#define _MAIN_H_

#include "led.h"
#include "usart.h"
#include "inoutput.h"
#include "iic.h"
#include "tcs3472.h"
#include "dht12.h"
#include "max6675.h"
#include "ili9341.h"
#include "rtc.h"
#include "w25q16.h"
#include "tsc2046.h"
#include "tim.h"
#include "hx1838.h"
#include "mpu9250.h"
#include "host.h"
#include "math.h"

uint16_t CurrentX = 0;       /*屏幕坐标X分量*/
uint16_t CurrentY = 0;       /*屏幕坐标Y分量*/

/*此值用于判断DHT12两次转换之间是否有满足两秒间隔，
   此值会在定时器中断服务函数中置1，置1代表满足时间间隔*/
extern uint8_t SenState;

uint8_t Print_Model = 6;/*用于判断屏幕打印哪个窗口,复位情况下Print_Model = 6及全局信息显示*/
    
uint8_t Print_State = 0;/*屏幕提示打印状态，此值为0时，屏幕可打印提示信息*/

float Dhtout[2] = {0};/*DHT12温度变量*/
uint8_t Dhtdata[5] = {0};

/*液晶屏变量,液晶屏最多可容下675个字符，如有需要以下数组可增加元素个数*/
char str[20] = {0};
char dhtsoult[20] = {0};
int16_t ang = 0;/*画线弧度*/

/*实时时钟变量*/
uint8_t Clock_Print_Model = 0;/*用于时间调整时切换画面*/
uint8_t Rtctmp=0;             /*时间变量，记录当前RTC秒值*/
RTC_TimeTypeDef RTC_TimeStructure;          /*时钟结构体变量，记录时分秒*/
RTC_DateTypeDef RTC_DateStructure;          /*日期结构体变量，记录年月日*/
RCT_Temp RCT_Tem_Time;/*临时时间结构体变量，用于更改时间*/



uint8_t point_state;/*用于记录触摸应用函数 TSC2046_Applic（） 返回值*/
    
float maxsoult = 0;/*MAX6675变量*/
    
uint16_t RGB565color = 0;/*TCS34725颜色传感器变量*/
uint32_t RGB888color = 0;

uint16_t touch_posit[2] = {0};/*TSC2046触摸面板变量*/
  
uint8_t remote_rec = 0;/*遥控器变量，记录遥控器函数 HX1838_Deal_Data（） 返回值*/
extern uint8_t IC_Stase;

MPU9250_Angle MPUANG;/*MPU9250结构体变量，存储角度*/

uint16_t x2,y2; /*画线坐标*/ 


/*参数定义*/     
#define Line_Length   50/*画线长度*/

/*用户自定义函数*/
void SYSTEM_Init(void);

#endif

