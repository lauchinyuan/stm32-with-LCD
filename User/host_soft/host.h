#ifndef _HOST_H_
#define _HOST_H_
#include "stm32f4xx.h"
#include "usart.h"

/*上位机协议相关定义*/
#define BYTE0(Temp)       (*(int8_t *)(&Temp))       /*取出变量的低字节*/
#define BYTE1(Temp)       (*((int8_t *)(&Temp) + 1)) /*取存储在此变量下一内存字节的内容，即高字节，以下同理*/
#define BYTE2(Temp)       (*((int8_t *)(&Temp) + 2))
#define BYTE3(Temp)       (*((int8_t *)(&Temp) + 3))


/*用户自定义函数*/
//void HOST_Send_MPU_Data(int16_t* MPUdata);
void HOST_Send_float(float data,uint8_t fun);
void HOST_Send_32bit(uint32_t data,uint8_t fun);

#endif
