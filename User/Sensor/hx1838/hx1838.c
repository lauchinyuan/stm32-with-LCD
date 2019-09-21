#include "hx1838.h"

extern uint32_t IC_Remote_Read;
extern uint8_t IC_Stase;
extern uint8_t IC_Remote_state;

/*************************************************
Function: HX1838_REC_Data
Description: 分析接受到的数据并返回实际数据值
Input: 无
Output: 无
Return: 实际8位有效数据值
Others: 无 
*************************************************/
uint8_t HX1838_REC_Data(void)
{
    uint8_t redata = 0;
    uint8_t data1 = 0,data2 = 0;
    if(IC_Stase==1)/*捕获完成*/
    {
        /*获取地址码*/
        data1 = (IC_Remote_Read&0xFF000000) >> 24;
        
        /*获取地址码反码*/
        data2 = (IC_Remote_Read&0x00FF0000) >> 16;
        
        if(data1==(uint8_t)~data2)/*地址码校验*/
        {
            /*获取数据码*/
            data1 = (IC_Remote_Read&0x0000FF00) >> 8;
        
            /*获取数据码反码*/
            data2 = (IC_Remote_Read&0x000000FF);
            
            if(data1==(uint8_t)~data2)/*地址码校验，检验反码与源码是否相等*/
            {
                redata = data1;
            }
        }
    }
    IC_Remote_Read = 0;/*将捕获的原始数据值重置为0*/
    IC_Stase = 0;/*标记，标识捕获未完成，以进入下次捕获*/
    IC_Remote_state = 0;/*作用同上*/
    return redata;
}


/*************************************************
Function: HX1838_Deal_Data
Description: 处理按键键值并返回相关数据
Input: 无
Output: 无
Return: 1-6对应1-6按键，返回7代表<向左切，返回8代表>向右切换
Others: 无 
*************************************************/
uint8_t HX1838_Deal_Data(uint8_t data)
{
    uint8_t redata = 0;/*返回的键值*/
    if(data==0xa2)
    {
        redata = 1;/*按下按键1*/
    }
    else if(data==0xc5||data==0x62)
    {
        redata = 2;
    }
    else if(data==0xe2)
    {
        redata = 3;
    }
    else if(data==0x22||data==0x45)
    {
        redata = 4;
    }
    else if(data==0x05||data==0x02)
    {
        redata = 5;
    }
    else if(data==0xc2)
    {
        redata = 6;
    }
    else if(data==0x10||data==0x21)
    {
        redata = 7;
    }
    else if(data==0xb5||data==0x5a)
    {
        redata = 8;
    }
    return redata;
    
}

