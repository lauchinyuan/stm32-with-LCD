#ifndef _ILI9341_H_
#define _ILI9341_H_
#include "stm32f4xx.h"

/*RGB565颜色定义*/
#define   ILI9431_Back_Color        BLACK        /*背景颜色*/
#define   ILI9431_Text_Color        DCYAN       /*字体颜色*/

#define   BLACK                     0x0000       /*黑色*/
#define   PURPLE                    0x780F       /*紫色*/
#define   WHITE                     0xFFFF       /*白色*/
#define   RED                       0xF800       /*红色*/
#define   BLUE                      0x001F       /*蓝色*/
#define   OLIVE                     0x7BE0       /*橄榄绿*/
#define   CYAN                      0x07FF       /*青色*/
#define   MAGENTA                   0xF81F       /*品红*/
#define   DCYAN                     0x03EF       /*深青色*/

/*RGB565与RGB888转换定义*/
#define RGB888_RED      0x00ff0000
#define RGB888_GREEN    0x0000ff00
#define RGB888_BLUE     0x000000ff
 
#define RGB565_RED      0xf800
#define RGB565_GREEN    0x07e0
#define RGB565_BLUE     0x001f

/*字库字模参数定义*/
/*假设所用字模为GB2312 矩阵大小为A*B的字模*/
#define   A                         16       /*字体横向长度*/
#define   B                         16       /*字体纵向长度*/

#define   ILI9431_Y_Lenth           240     /*屏幕横向长度*/
#define   ILI9431_X_Lenth           360     /*屏幕纵向长度*/

#define   ILI9431_Data_Size         ((A/8)*B)   /*每个字所用的字节大小*/


/*背光控制引脚*/
#define      ILI9431_BK_CLK                RCC_AHB1Periph_GPIOB   
#define      ILI9431_BK_PORT               GPIOB
#define      ILI9431_BK_Pin                GPIO_Pin_15

/*由于使用的存储区域为FSMC_Bank1_NORSRAM4，故其对因地址为0x6C000000-0x6FFFFFFF，
在使用屏幕时，利用FSMC_A0模拟DCX，当其为高电平时表示数据，低电平时表示命令，
由于 HADDR 为字节地址，而存储器按字寻址，所以根据存储器数据宽度不同，
实际向存储器发送的地址也将有所不同，此处地址为16位地址，依参考手册，写入的值会左移1位
为了使A0的值为用户控制，需先右移1位*/
#define 	 ILI9431_Data_ADDR		(uint32_t)(0x6E000000|(1<<7))	
#define 	 ILI9431_Cmd_ADDR		(uint32_t)(0x6E000000&~(1<<7))

#define      W25Q16_ASCII_Start_Addr    (uint32_t)0x1F0000
#define      W25Q16_GB2312_Start_Addr    (uint32_t)0x000000

/*操作命令定义*/
#define ILI9431_Cmd_Read_ID4		0xD3
#define ILI9431_Cmd_Reset			0x01

#define DEBUG_DELAY()               ILI9341_Delay(0x5000)

/*用户自定义函数*/
void ILI9341_GPIO_Conf(void);
void ILI9341_Init(void);
void ILI9341_BackLight(uint8_t state);
void ILI9341_Delay(uint32_t time);
void ILI9341_Default(void);
uint16_t RGB888ToRGB565(uint32_t n888Color);

/*基本读写函数*/
void ILI9341_Write_Command(uint16_t cmd);
void ILI9341_Write_Data(uint16_t data);

/*屏幕命令操作*/
void ILI9341_Drew_Rec(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color);
void ILI9341_Open_Windows(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
void ILI9341_Print_GBChar(uint16_t x,uint16_t y,uint16_t ch);
void ILI9341_Print_ASChar(uint16_t x,uint16_t y,uint8_t ch);
void ILI9341_Print_Str(uint16_t x,uint16_t y,char* str);
void ILI9341_Line_Print(uint8_t line,char* str);
void ILI9431_Drew_Cross(uint16_t x,uint16_t y,uint16_t color);
void ILI9431_Drew_All_Bak(void);
void ILI9431_Clean_Cross(uint16_t x,uint16_t y);
void ILI9341_Draw_Point(uint16_t x,uint16_t y,uint16_t color);
void ILI9341_Drew_Line(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color);
void ILI9341_Next_Win(void);
void ILI9341_Last_Win(void);
#endif
