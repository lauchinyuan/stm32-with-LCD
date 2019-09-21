#ifndef _TCS_3472_H_
#define _TCS_3472_H_
#include "stm32f4xx.h"
#include "w25q16.h"
#include "tim.h"
#include "led.h"

/*设备地址定义*/
#define TCS3472_DEVICE_ID 					0x52

#define TCS3472_DEVICE_READ_ID 				((0x52)|0x01)
#define TCS3472_DEVICE_WRITE_ID 			((0x52)|0x00)

/*外部Flash地址定义，以下空间用来存储RGB白平衡转换因子*/
#define TCS3472_R_Factor                    0x1B0000
#define TCS3472_G_Factor                    0x1B0004
#define TCS3472_B_Factor                    0x1B0008


/*寄存器地址定义*/
#define TCS3472_ID_REGISTER					0x12
#define TCS3472_ENABLE_REGISTER 			0x00
#define TCS3472_RGBC_TIMING_REGISTER		0x01
#define TCS3472_WTIME_REGISTER				0x03
#define TCS3472_CONTROL_REGISTER			0x0F
#define TCS3472_STATUS_REGISTER				0x13

#define TCS3472_CDATAL_REGISTER				0x14
#define TCS3472_CDATAH_REGISTER				0x15

#define TCS3472_RDATAL_REGISTER				0x16
#define TCS3472_RDATAH_REGISTER				0x17

#define TCS3472_GDATAL_REGISTER				0x18
#define TCS3472_GDATAH_REGISTER				0x19

#define TCS3472_BDATAL_REGISTER				0x1A
#define TCS3472_BDATAH_REGISTER				0x1B

#define TCS3472_ID_REGISTER                 0x12


/*操作指令定义*/
#define TCS3472_COMMAND_Bit			0x80
#define TCS3472_ENABLE_PON			0x01
#define TCS3472_ENABLE_AEN			0x02
#define TCS3472_ENABLE_WEN			0x80

/*参数定义*/
#define WTIME_Used					0xFF    /*等待模式单次等待时间为0.029s*/
#define ATIME_Used					0xFF    /*数据转换用时2.4ms,最大计数值为1024（增益为60X时）*/
#define AGAIN_Used                  0xFF    /*增益为60X*/


/*自定义函数*/
void TCS3472_Write_(uint8_t *data,uint8_t num,uint8_t addr);
void TCS3472_Read_(uint8_t *datasto,uint8_t num,uint8_t addr);
void TCS3472_Read_RGBData(uint16_t* temp);
void TCS3472_Set_WTIME(uint8_t wtime);
void TCS3472_Set_ATIME(uint8_t atime);
void TCS3472_Set_AGAIN(uint8_t again);
uint8_t TCS3472_Get_Conv_Status(void);
void TCS3472_Init(void);
void TCS3472_Enable(void);
void TCS3472_White_Balance(void);
uint32_t TCS3472_Read_Deal(void);

#endif 

