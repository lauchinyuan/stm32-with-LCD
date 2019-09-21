#include "tsc2046.h"
extern uint8_t Clock_Print_Model;/*用于时间调整时切换画面*/
extern uint8_t Print_Model;
/*************************************************
Function: TSC2046_SoftSPI_GPIO_Conf
Description: 软件模拟SPI所用引脚配置
Input: 无
Output:无
Return: 无
Others: 无
*************************************************/
void TSC2046_SoftSPI_GPIO_Conf(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    /*开启GPIO时钟*/
    RCC_AHB1PeriphClockCmd(TSC2046_RCC_CLOCK,ENABLE);
    
     /*配置模拟MISO线*/
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Pin = TSC2046_MISO_Pin;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(TSC2046_MISO_Port, &GPIO_InitStruct);
    
     /*配置Pen*/
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Pin = TSC2046_PEN_Pin;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(TSC2046_PEN_Port, &GPIO_InitStruct);
    
    /*配置模拟SCK线*/
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin = TSC2046_SCK_Pin;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(TSC2046_SCK_Port, &GPIO_InitStruct);
    
    /*配置模拟MOSI线*/
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin = TSC2046_MOSI_Pin;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(TSC2046_MOSI_Port, &GPIO_InitStruct);
    
    /*配置模拟CS线*/
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin = TSC2046_CS_Pin;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(TSC2046_CS_Port, &GPIO_InitStruct);
    
}


/*************************************************
Function: TSC2046_Ave_Value
Description: 通过模拟SPI时序向TSC2046发送一个字节数据命令,并读取12位ADC转换结果值
Input: data为所发送的数据
Output:无
Return: 12位ADC转换结果值
Others: 无
*************************************************/
uint16_t TSC2046_Write_Read(uint8_t data)
{
    uint8_t i = 0;
    uint16_t Read_Data = 0;
    
     /*默认电平*/
    TCS_2046_SCK_Low;
    TSC2046_DelayUS (5);
    
    /*拉低片选引脚*/
    TCS_2046_CS_Low;
    TSC2046_DelayUS (5);
    
    /*逐位发送命令*/
    for(i = 0;i<8;i++)
    {
        /*若最高位为1*/
        if((data&0x80)==0x80)
        {
           TCS_2046_MOSI_Hight;
        }
        else
        {
            TCS_2046_MOSI_Low;
            
        }
        TSC2046_DelayUS (5);
        
        /*拉高SCK电平（上升沿），此时从机获得一位数据*/
        TCS_2046_SCK_Hight;
        TSC2046_DelayUS (5);
        
        TCS_2046_SCK_Low;
        TSC2046_DelayUS (5);
        
        data<<=1;
    }
    
    /*根据时序图，要给BUSY信号留一个时钟周期后，从机才会发送数据*/
     TCS_2046_SCK_Hight;
     TSC2046_DelayUS (5);
        
     TCS_2046_SCK_Low;
     TSC2046_DelayUS (5);
    
    /*逐位接收数据，中间片选持续拉低*/
    
    for(i = 0;i<12;i++)
    {
        Read_Data<<=1;
        
        /*上升沿，使从机先发送数据*/
        TCS_2046_SCK_Hight;
        TSC2046_DelayUS (5);
        
        /*读取数据位*/
        if(TCS_2046_MISO_Hight)
        {
            Read_Data|=0x01;
        }
        
        else
        {
             Read_Data|=0x00;
        }
        TSC2046_DelayUS (5);
        
        TCS_2046_SCK_Low;
        TSC2046_DelayUS (5);
        
        
    }
    
    /*补全剩余时钟信号*/
    for(i = 0;i<4;i++)
    {
        TCS_2046_SCK_Hight;
        TSC2046_DelayUS (5);
        
        TCS_2046_SCK_Low;
        TSC2046_DelayUS (5);
    }
    
    
   return Read_Data; 
}
/*************************************************
Function: TSC2046_DelayUS
Description: 用于TSC2046的简单微妙级延时函数
Input: ulCount为延时毫秒数
Output:无
Return: 无
Others: 无
*************************************************/
static void TSC2046_DelayUS (uint32_t ulCount )
{
	uint32_t i;
	for ( i = 0; i < ulCount; i ++ )
	{  
		uint8_t uc = 12;/*设置值为12，大约延1微秒*/   
		while ( uc -- );    
	}
	
}

/*************************************************
Function: TSC2046_Ave_Value
Description: 连续采集多次ADC所得数据，去掉最大最小值，求平均值
Input: Cmd仅配合SPi通信协议，无实际作用
Output:无
Return: 平均结果
Others: 无
*************************************************/
uint16_t TSC2046_Ave_Value(uint8_t Cmd)
{
    uint8_t i;
    uint16_t soult = 0;
    uint32_t sum  = 0;
    uint16_t max = 0,min = 0;
    uint16_t data[15] = {0};
 
    /*记录并处理数据*/
    for(i = 0;i<15;i++)
    {
        /*采集12次X数据*/
        data[i]=TSC2046_Write_Read(Cmd);
    }
    if(i==15)
    {
        /*若有12次采集*/
        /*判断最大最小值并剔除*/
        for(i = 0;i<15;i++)
        {
            if(max<data[i])
            {
                max = data[i];
            }
            if(min>data[i])
            {
                min = data[i];
            }
            
            sum+=data[i];
        
        }
        
        /*计算平均值*/
        soult = ((sum-max-min)/13);  
       
       
    }
    
    else
        {
            /*若未满足采样次数，则提示采样失败*/
             ILI9341_Print_Str(200,0,"触摸时间不足，重新触摸");
            
        }
    
    
     return soult;
}


/*************************************************
Function: TSC2046_Calibration
Description: 触摸面板配合液晶显示屏进行校准,此处使用5点校准法,此方法有一定误差
Input: 无
Output:无
Return: 无
Others: 参考文章中的计算方法 https://blog.csdn.net/yangseng1987/article/details/20780379
*************************************************/
void TSC2046_Calibration(void)
{
    /*交叉点物理坐标*/
    uint16_t Drew_X[5] = {90,270,270,90,180};
    uint16_t Drew_Y[5] = {160,160,80,80,120};
    
    uint16_t Logic_X[5] = {0},Logic_Y[5] = {0};/*定义用于存储逻辑值的数组*/
     
    uint8_t i = 0;/*用i进行触摸次数统计*/
    
    uint8_t Drew_flag = 0;/*画点标志，标志为0时画交叉点*/
    
    float kx = 0,ky = 0;/*最终所得校正系数值*/
    
    /*在While循环中采集触点数据*/
    while(i<5)
    {
        /*交叉点显示，为防止重复调用画交叉点函数占用资源，此处用一标志Dreew_flag*/
        if(Drew_flag == 0)
        {
            Drew_flag = 1;
            ILI9431_Drew_Cross(Drew_X[i],Drew_Y[i],BLUE); 
        }
        
        if(TCS_2046_PEN_Low)/*检测到触点*/
        {
            /*检测到触摸则记录ADC转化所得的逻辑值*/
            Logic_Y[i] = TSC2046_Ave_Value(TCS_2046_Cmd_Read_Y_Pos);
            Logic_X[i] = TSC2046_Ave_Value(TCS_2046_Cmd_Read_X_Pos);
            
            /*延时，防止i过快计数*/
            General_TIM_Delay_ms(500);
            
            /*数据采集完，清除上一坐标点*/
            ILI9431_Clean_Cross(Drew_X[i],Drew_Y[i]);
            
            /*进入下一画面*/
            /*Dreew_flag重新置0，使屏幕可以画点*/
            Drew_flag = 0;
             
            i++;
        }
    }
    
    /*跳出While循环表示逻辑数据记录完毕*/
    
    /*计算系数*/
    kx = (float)(((float)(Logic_X[1] - Logic_X[0])/(float)(Drew_X[1] - Drew_X[0]))+((float)(Logic_X[2] - Logic_X[3])/(float)(Drew_X[2] - Drew_X[3])))/2;
    ky = (float)(((float)(Logic_Y[2] - Logic_Y[1])/(float)(Drew_Y[2] - Drew_Y[1]))+((float)(Logic_Y[2] - Logic_Y[1])/(float)(Drew_Y[2] - Drew_Y[1])))/2;
    
    /*将所得系数及中心校准点数据存储到外部Flash中，此处定义存储顺序为kx->ky->中心点逻辑X值->中心点逻辑Y值*/
    
    /*擦除扇区*/
    W25Q16_Sector_Erase(TCS_2046_Sec_Addr);
    
    /*float型变量占据4个字节空间,ADC转换所得值占2个字节空间*/
    
    /*逐个写入数据值*/
    W25Q16_Write_Byte(TCS_2046_kx_Addr,(uint8_t*)&kx,4);
    W25Q16_Write_Byte(TCS_2046_ky_Addr,(uint8_t*)&ky,4);
    W25Q16_Write_Byte(TCS_2046_Cen_Logic_X_Addr,(uint8_t*)&Logic_X[4],2);
    W25Q16_Write_Byte(TCS_2046_Cen_Logic_Y_Addr,(uint8_t*)&Logic_Y[4],2);
    
}

/*************************************************
Function: TSC2046_Calculate_XY
Description: 计算物理触摸点并存储在指定数组中
Input: 无
Output:datasto即为最终结果所在数组
Return: 无
Others: 无
*************************************************/
void TSC2046_Calculate_XY(uint16_t *datasto)
{
    float kx = 0,ky = 0;
    uint16_t logic_X = 0,logic_Y = 0;
    uint16_t Current_Logicx = 0,Current_Logicy = 0;
    
    /*首先读取存储在外部Flash中的校准数据*/
    W25Q16_Read_Byte(TCS_2046_kx_Addr,(uint8_t*)&kx,4);
    W25Q16_Read_Byte(TCS_2046_ky_Addr,(uint8_t*)&ky,4);
    W25Q16_Read_Byte(TCS_2046_Cen_Logic_X_Addr,(uint8_t*)&logic_X,2);
    W25Q16_Read_Byte(TCS_2046_Cen_Logic_Y_Addr,(uint8_t*)&logic_Y,2);
    
    /*读取实时触点ADC转化值*/
    Current_Logicx = TSC2046_Ave_Value(TCS_2046_Cmd_Read_X_Pos);
    
    
    Current_Logicy = TSC2046_Ave_Value(TCS_2046_Cmd_Read_Y_Pos);
     
    /*利用校准原理的公式计算物理坐标*/
    
    /* X = ( XL - XLC ) / KX + XC 
       Y = ( YL - YLC ) / KY + YC*/
    
    datasto[0] = ((uint16_t)(((Current_Logicx - logic_X)/kx)+180));
    
    
    datasto[1] = (uint16_t)(((Current_Logicy - logic_Y)/ky)+120);
    
    /*计算结束*/
    
}
/*************************************************
Function: TSC2046_Applic
Description: 结合触摸屏编写的应用程序，此处用于判断触点位置，并返回相应值
Input: 触点位置，其中postion[1]为物理坐标X轴值，postion[2]为物理坐标Y轴值
Output:无
Return: 返回1代表触点在向左切换区块
        返回2代表触点在屏幕向右切换区块
        返回3表示调整时钟
        返回4表示调整日期
        返回5代表调整完毕（新时间设置完毕）
        返回6代表小时上调
        返回7代表小时下调
        返回8代表分钟上调
        返回9代表分钟下调
        返回10代表年份上调
        返回11代表年份下调
        返回12代表月份上调
        返回13代表月份下调
        返回14代表日期上调
        返回15代表日期下调
Others: 无
*************************************************/
uint8_t TSC2046_Applic(uint16_t *postion)
{  
    uint8_t state = 0; 
    
/************在任何Print_Modle下，只要不是正在调整时间，都要判断是否左右切换画面****************/
    
        if((postion[0]>250-TCS_2046_Calibration_Error)&&(postion[0]<300+TCS_2046_Calibration_Error))
        {
            /*屏幕上两个区域X轴范围相同，均为250-300，此处加上误差允许值*/
            
            /*判断触点Y坐标是否也落在指定区域*/
            if(postion[1]<(64+TCS_2046_Calibration_Error))
            {
                /*表示触点落在左边区块*/
                
                state = 1;
            }
            
             if(postion[1]>(176-TCS_2046_Calibration_Error))
            {
                /*表示触点落在右边区块*/
                
                state = 2;
            }
        }
        
/**********************仅在时间调整界面（Print_Model==5）判断以下触点**************************/
        if(Print_Model==5)
        {
            if(Clock_Print_Model==0)/*在时间调整的初始界面，判断是否按下调整区域*/
            {
                /*在初始界面中点击了调整区域*/
                 if((postion[0]>160-TCS_2046_Calibration_Error)&&(postion[0]<200+TCS_2046_Calibration_Error))
                {
                
                
                    /*判断触点Y坐标是否也落在指定区域*/
                    if(postion[1]<(64+TCS_2046_Calibration_Error))
                    {
                        /*表示触点落在左边区块*/   
                        state = 3; /*表示要调整时间*/
                    }
                
                     else if(postion[1]>(176-TCS_2046_Calibration_Error))
                    {
                        /*表示触点落在右边区块*/
                        state = 4;/*表示要调整日期*/
                        
                    }
               }
            }
            
            
            
            
            
/*在时钟调整界面中，判断小时与分钟的向上向下调整区域是否触及*/
            else if(Clock_Print_Model==1)
            {
                
                if((postion[0]>64-TCS_2046_Calibration_Error)&&(postion[0]<96+TCS_2046_Calibration_Error))
                {
                
                
                    /*判断触点Y坐标是否也落在指定区域*/
                    if(postion[1]<(32+TCS_2046_Calibration_Error))
                    {
                        /*表示触点落在左边区块*/   
                        state = 6; /*表示小时向上调整*/
                    }
                
                     else if(postion[1]>(192-TCS_2046_Calibration_Error))
                    {
                        /*表示触点落在右边区块*/
                        state = 8;/*表示分钟值向上调整*/
                        
                    }
                    
                    
               }
                
               else if((postion[0]>144-TCS_2046_Calibration_Error)&&(postion[0]<176+TCS_2046_Calibration_Error))
                {
                
                
                    /*判断触点Y坐标是否也落在指定区域*/
                    if(postion[1]<(32+TCS_2046_Calibration_Error))
                    {
                        /*表示触点落在左边区块*/   
                        state = 7; /*表示小时向下调整*/
                    }
                
                     else if(postion[1]>(192-TCS_2046_Calibration_Error))
                    {
                        /*表示触点落在右边区块*/
                        state = 9;/*表示分钟值向下调整*/
                        
                    }
                    
                    
               }
                
               /*判断是否在时钟界面按下确认键*/
               if((postion[0]>200-TCS_2046_Calibration_Error)&&(postion[0]<240+TCS_2046_Calibration_Error))/*X范围一致*/
               {
                   if((postion[1]>90-TCS_2046_Calibration_Error)&&(postion[1]<150+TCS_2046_Calibration_Error))/*Y范围一致，确认按下确认键*/
                   {
                       state = 5;
                   }
               }
                
                
            }
            
/*在日期调整界面中，判断年月日上下调整区域是否触及*/            
            else if(Clock_Print_Model==2)
            {
                
                /*X坐标落在向上调整相关位置*/
                if((postion[0]>64-TCS_2046_Calibration_Error)&&(postion[0]<96+TCS_2046_Calibration_Error))
                {
                
                
                    /*判断触点Y坐标是否也落在指定区域*/
                    if(postion[1]<(25+TCS_2046_Calibration_Error))
                    {
                        /*表示触点落在最左边区块，表示年份向上调整*/   
                        state = 10; /*表示年份向上调整*/
                    }
                    
                    /*落在从左往右第二区块*/
                    else if(postion[1]>(64+TCS_2046_Calibration_Error)&&postion[1]<(64+32+TCS_2046_Calibration_Error))
                    {
                        state = 12;/*表示月份向上调整*/
                    }
                    /*落在从左往右第三区块*/
                    else if(postion[1]>(128+TCS_2046_Calibration_Error)&&postion[1]<(128+32+TCS_2046_Calibration_Error))
                    {
                        state = 14;/*表示日期向上调整*/
                    }
                    /*表示触点落在右边区块*/
                     else if(postion[1]>(192-TCS_2046_Calibration_Error))
                    {
                       
                        state = 16;/*表示星期向上调整*/
                        
                    }
                    
                    
               }
                
               
               
               
               /*X坐标落在向下调整相关位置*/
                if((postion[0]>64+5*16-TCS_2046_Calibration_Error)&&(postion[0]<96+5*16+TCS_2046_Calibration_Error))
                {
                
                
                    /*判断触点Y坐标是否也落在指定区域*/
                    if(postion[1]<(25+TCS_2046_Calibration_Error))
                    {
                        /*表示触点落在最左边区块，表示年份向下调整*/   
                        state = 11; /*表示年份向上调整*/
                    }
                    
                    /*落在从左往右第二区块*/
                    else if(postion[1]>(64+TCS_2046_Calibration_Error)&&postion[1]<(64+32+TCS_2046_Calibration_Error))
                    {
                        state = 13;/*表示月份向下调整*/
                    }
                    /*落在从左往右第三区块*/
                    else if(postion[1]>(128+TCS_2046_Calibration_Error)&&postion[1]<(128+32+TCS_2046_Calibration_Error))
                    {
                        state = 15;/*表示日期向下调整*/
                    }
                    /*表示触点落在右边区块*/
                     else if(postion[1]>(192-TCS_2046_Calibration_Error))
                    {
                       
                        state = 17;/*表示星期向下调整*/
                        
                    }
                    
                    
               }
                                             
                
                /*判断是否在日期界面按下确认键*/
               if((postion[0]>200-TCS_2046_Calibration_Error)&&(postion[0]<240+TCS_2046_Calibration_Error))/*X范围一致*/
               {
                   if((postion[1]>90-TCS_2046_Calibration_Error)&&(postion[1]<150+TCS_2046_Calibration_Error))/*Y范围一致，确认按下确认键*/
                   {
                       state = 5;
                   }
               }
            }

        }
    return state;
    
}



