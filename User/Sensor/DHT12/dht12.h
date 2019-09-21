#ifndef _DHT_12_H_
#define _DHT_12_H_
#include "stm32f4xx.h"

/*设备地址定义*/
#define DHT12_DEV_ADDR			0xB8
#define DHT12_DEV_READ_ADDR		0xB9
#define DHT12_DEV_WRITE_ADDR	0xB8

/*BYTE ADDR定义*/
#define DHT12_HUM_INT   		0x00    /*湿度整数位*/
#define DHT12_HUM_DEC			0x01	/*湿度小数位*/
#define DHT12_TEM_INT			0x02	/*温度整数位*/
#define DHT12_TEM_DEC			0x03    /*温度小数位*/
#define DHT12_CHECK_SUM			0x04	/*校验和*/
/*自定义函数*/
void DHT12_Read_(uint8_t *datasto);
uint8_t DHT12_Deal_Tem_Hum(float* datasto,uint8_t *datasrc);

#endif    /*_DHT_12_H_*/
