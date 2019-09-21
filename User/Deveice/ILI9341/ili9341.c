#include "ili9341.h"
#include "w25q16.h"
#include "fsmc.h"
extern  uint16_t CurrentX;
extern  uint16_t CurrentY;
extern uint8_t Print_Model;
extern uint8_t Print_State;
extern uint8_t Clock_Print_Model;

/*************************************************
Function: ILI9341_GPIO_Conf
Description: 配置驱动液晶显示器相关引脚
Input: 无
Output:无
Return: 无
Others: 无
*************************************************/
void ILI9341_GPIO_Conf(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/*开启GPIO时钟*/
	RCC_AHB1PeriphClockCmd(ILI9431_BK_CLK, ENABLE);
	
	/*配置背光引脚*/
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = ILI9431_BK_Pin;
	GPIO_Init(ILI9431_BK_PORT, &GPIO_InitStruct);
}

/*************************************************
Function: ILI9341_Init
Description: 初始化ILI9341
Input: 无
Output:无
Return: 无
Others: 无
*************************************************/
void ILI9341_Init(void)
{
	
	FSMC_Conf();/*利用FSMC模拟时序*/
	
	ILI9341_GPIO_Conf();/*配置相关引脚*/
      
    ILI9341_Default();/*配置屏幕参数*/
    
    ILI9431_Drew_All_Bak();/*将屏幕清空*/
    
    ILI9341_BackLight(1);/*开启屏幕背光源*/
}

/*************************************************
Function: ILI9341_Print_GBChar
Description: 从指定的坐标点开始打印一个汉字
Input: x为子字符串起始点的X坐标值，y为字符串起始点的Y坐标值，ch为汉字字符
Output:无
Return: 无
Others: 字库资源从w25q16中调取GB2312字库
*************************************************/
void ILI9341_Print_GBChar(uint16_t x,uint16_t y,uint16_t ch)
{
    uint8_t i,j,k;/*用于循环*/
    uint8_t chdata1[ILI9431_Data_Size]  ={0};/*单个字符字模数据*/
    uint32_t pos = 0;/*（字符）与（字库首字符）的偏移值*/
    uint32_t devi = 0;/*（字符字模数据）与（字库第一个字节数据）之间相差的字节数*/
    uint8_t t = 0;
    unsigned char High8bit,Low8bit;/*输入字库数据的高低8位*/
    
   
    High8bit= (ch&0xFF00)>> 8;      /* 取高8位数据 */
    Low8bit= ch & 0x00FF;           /* 取低8位数据*/   

    
      /*计算字符偏移值*/    
      if(High8bit < 0xAA)
      {
            /* GB2312 A1A0~A9FF: 1~9区, 标点符号及日文片、平假名等特殊字符 */
            pos = ((High8bit-0xA1)*94 + Low8bit-0xA1);
      }
      else if((High8bit > 0xAF) && (High8bit < 0xF8))
      {
            /* GB2312 B0A0~F7FF: 16~55区: 一级汉字, 56~87区: 二级汉字 */
            /*此处1410为所用字库偏移值*/
            pos = (1410 + (High8bit-0xB0)*94 + Low8bit-0xA1);
      }
      else
      {
            pos = 0;/* GB2312 其他区位未编码(保留) */
      }
      
      devi = pos*ILI9431_Data_Size;
      
   
    /*从w25q16中获取字模数据,即连续读取ILI9431_Data_Size个字节*/
    W25Q16_Read_Byte((W25Q16_GB2312_Start_Addr+(uint32_t)devi),chdata1,ILI9431_Data_Size);
    
    /*开启打印窗口*/
    ILI9341_Open_Windows(x,y,x+B-1,y+A-1);
    
    /*开始填充*/
    ILI9341_Write_Command(0x2C);
    
    /*第i+1行*/
	for(i = 0;i<B;i++)
	{
        
        /*第i+1行的第j+1个字节*/
		for(j = 0;j<(A/8);j++)
		{
            
            /*每个字节逐位打印*/
			for(k = 0;k<8;k++)
            {
                /*计算此时读取的字节所在位置*/
                t = i*(A/8)+j;
                
                if((chdata1[t]&0x80)==0x80)/*需要填充字体颜色到像素点*/
                {
                    ILI9341_Write_Data(ILI9431_Text_Color);
                }
				
                else/*填充背景颜色到像素点*/
                {
                    ILI9341_Write_Data(ILI9431_Back_Color);
                }
                chdata1[t]<<=1;
            }
		}
        
		
	}
     
}


/*************************************************
Function: ILI9341_Print_ASChar
Description: 从指定的坐标点开始打印一个英文字符
Input: x为子字符串起始点的X坐标值，y为字符串起始点的Y坐标值，ch为字符
Output:无
Return: 无
Others: 字库资源从w25q16中调取ASCII字库
*************************************************/
void ILI9341_Print_ASChar(uint16_t x,uint16_t y,uint8_t ch)
{
    uint8_t i,j,k;/*用于循环*/
    uint8_t chdata1[ILI9431_Data_Size]  ={0};/*单个字符的字膜数据*/
    uint32_t pos = 0;/*（字符）相对于（字库首字符）的偏移量*/
    uint32_t devi = 0;/*字符字模数据相对于第一个字节数据的偏移量*/
    uint8_t t = 0;
    
   /*计算字符偏移值*/  
      pos = ch - ' ';
      
      devi = pos*ILI9431_Data_Size/2;/*每个ASCII字符字模数据占用ILI9431_Data_Size/2个字节空间*/
 
    /*通过SPI协议从w25q16中获取字模数据,即连续读取ILI9431_Data_Size个字节，*/
    W25Q16_Read_Byte((W25Q16_ASCII_Start_Addr+(uint32_t)devi),chdata1,ILI9431_Data_Size/2);
    
    /*开启打印窗口*/
    ILI9341_Open_Windows(x,y,x+B-1,y+(A/2)-1);
    
    ILI9341_Write_Command(0x2C);/*发送填充命令*/
    
    /*第i+1行*/
	for(i = 0;i<B;i++)
	{ 
        /*第i+1行的第j+1个字节*/
		for(j = 0;j<(A/16);j++)
		{
            
            /*每个字节逐位打印*/
			for(k = 0;k<8;k++)
            {
                /*计算此时读取的字节所在位置*/
                t = i*(A/16)+j;
                
                if((chdata1[t]&0x80)==0x80)
                {
                    ILI9341_Write_Data(ILI9431_Text_Color);
                }
				
                else
                {
                    ILI9341_Write_Data(ILI9431_Back_Color);
                }
                chdata1[t]<<=1;
            }
		}		
	}
    
}

/*************************************************
Function: ILI9341_Print_Str
Description: 从指定的坐标点开始打印中英文字符串
Input: x为子字符串起始点的X坐标值，y为字符串起始点的Y坐标值，str为字符串
Output:无
Return: 无
Others: 无
*************************************************/
void ILI9341_Print_Str(uint16_t x,uint16_t y,char* str)
{
    uint8_t i = 0;
    uint16_t ch = 0;
    /*LCD屏打印时，先往Y轴增加，再往Y轴增加*/
    CurrentX = x;       /*当前坐标X轴分量*/
    CurrentY = y;       /*当前坐标Y轴分量*/

    while(str[i]!='\0')
    {
        /*先判断是否需要换行*/
        if((CurrentY+A)>ILI9431_Y_Lenth||str[i]=='\n')
        {
            /*Y轴回到设定的起点*/
            CurrentY = y;
            
            CurrentX += B;
            if(str[i]=='\n')
            {
                /*若是应转义字符而换行则忽略该字符(不打印)*/
                i++;
                continue;
            }
        }
        
        /*STM32使用小端模式，此处进行高低八位数据交换*/
        ch = * ( uint16_t * ) &str[i];
        
        ch = ( ch << 8 ) + ( ch >> 8 );
        
        /*判断所用字库*/
        if(((ch&0xFF00)>>8)>=0xA1)
        {
            /*使用GB2312字库*/
             ILI9341_Print_GBChar(CurrentX,CurrentY,ch);

             CurrentY += A; /*坐标点更新*/

             i+=2;/*一个GB2312字符占两个字节*/
            
        }
        else
        {
            /*使用ASCII字库*/
            ILI9341_Print_ASChar(CurrentX,CurrentY,(uint8_t)(str[i]));
            
            CurrentY += (A/2);/*字母只占一半宽度*/
            i+=1;
            
        }  
        
    }
}

/*************************************************
Function: ILI9341_BackLight
Description: 控制ILI9341背光灯
Input: state为1时开启背光，为时关闭背光
Output:无
Return: 无
Others: 无
*************************************************/
void ILI9341_BackLight(uint8_t state)
{
	if(state == 1)
	{
		
		GPIO_SetBits(ILI9431_BK_PORT,ILI9431_BK_Pin);/*设置背光引脚高电平开启背光*/
		
	}
	else if(state == 0)
	{
		
		GPIO_ResetBits(ILI9431_BK_PORT,ILI9431_BK_Pin);/*设置背光引脚低电平关闭背光*/
	}
	
}



/*************************************************
Function: ILI9341_Write_Command
Description: 利用FSMC模拟时序向屏幕发送命令
Input: cmd为命令
Output:无
Return: 无
Others: 无
*************************************************/
void ILI9341_Write_Command(uint16_t cmd)
{
	uint16_t *p = (uint16_t*)(ILI9431_Cmd_ADDR);
	*p = cmd;
}



/*************************************************
Function: ILI9341_Write_Data
Description: 利用FSMC模拟时序向屏幕发送数据
Input: data为数据
Output:无
Return: 无
Others: 无
*************************************************/
void ILI9341_Write_Data(uint16_t data)
{
	uint16_t *p = (uint16_t*)(ILI9431_Data_ADDR);
	*p = data;
}



/*************************************************
Function: ILI9341_GPIO_Conf
Description: 写入液晶厂商数据手册提供的配置数据，初始化显示以实现后续操作
Input: 无
Output:无
Return: 无
Others: 该函数具体操作不懂
*************************************************/
void ILI9341_Default(void)
{
	/*以下配置数据由厂商提供*/
	/*  Power control B (CFh)  */
	DEBUG_DELAY  ();
	ILI9341_Write_Command ( 0xCF  );
	ILI9341_Write_Data ( 0x00  );
	ILI9341_Write_Data ( 0x81  );
	ILI9341_Write_Data ( 0x30  );
	
	/*  Power on sequence control (EDh) */
	DEBUG_DELAY ();
	ILI9341_Write_Command ( 0xED );
	ILI9341_Write_Data ( 0x64 );
	ILI9341_Write_Data ( 0x03 );
	ILI9341_Write_Data ( 0x12 );
	ILI9341_Write_Data ( 0x81 );
	
	/*  Driver timing control A (E8h) */
	DEBUG_DELAY ();
	ILI9341_Write_Command ( 0xE8 );
	ILI9341_Write_Data ( 0x85 );
	ILI9341_Write_Data ( 0x10 );
	ILI9341_Write_Data ( 0x78 );
	
	/*  Power control A (CBh) */
	DEBUG_DELAY ();
	ILI9341_Write_Command ( 0xCB );
	ILI9341_Write_Data ( 0x39 );
	ILI9341_Write_Data ( 0x2C );
	ILI9341_Write_Data ( 0x00 );
	ILI9341_Write_Data ( 0x34 );
	ILI9341_Write_Data ( 0x02 );
	
	/* Pump ratio control (F7h) */
	DEBUG_DELAY ();
	ILI9341_Write_Command ( 0xF7 );
	ILI9341_Write_Data ( 0x20 );
	
	/* Driver timing control B */
	DEBUG_DELAY ();
	ILI9341_Write_Command ( 0xEA );
	ILI9341_Write_Data ( 0x00 );
	ILI9341_Write_Data ( 0x00 );
	
	/* Frame Rate Control (In Normal Mode/Full Colors) (B1h) */
	DEBUG_DELAY ();
	ILI9341_Write_Command ( 0xB1 );
	ILI9341_Write_Data ( 0x00 );
	ILI9341_Write_Data ( 0x1B );
	
	/*  Display Function Control (B6h) */
	DEBUG_DELAY ();
	ILI9341_Write_Command ( 0xB6 );
	ILI9341_Write_Data ( 0x0A );
	ILI9341_Write_Data ( 0xA2 );
	
	/* Power Control 1 (C0h) */
	DEBUG_DELAY ();
	ILI9341_Write_Command ( 0xC0 );
	ILI9341_Write_Data ( 0x35 );
	
	/* Power Control 2 (C1h) */
	DEBUG_DELAY ();
	ILI9341_Write_Command ( 0xC1 );
	ILI9341_Write_Data ( 0x11 );
	
	/* VCOM Control 1 (C5h) */
	ILI9341_Write_Command ( 0xC5 );
	ILI9341_Write_Data ( 0x45 );
	ILI9341_Write_Data ( 0x45 );
	
	/*  VCOM Control 2 (C7h)  */
	ILI9341_Write_Command ( 0xC7 );
	ILI9341_Write_Data ( 0xA2 );
	
	/* Enable 3G (F2h) */
	ILI9341_Write_Command ( 0xF2 );
	ILI9341_Write_Data ( 0x00 );
	
	/* Gamma Set (26h) */
	ILI9341_Write_Command ( 0x26 );
	ILI9341_Write_Data ( 0x01 );
	DEBUG_DELAY ();
	
	/* Positive Gamma Correction */
	ILI9341_Write_Command ( 0xE0 ); //Set Gamma
	ILI9341_Write_Data ( 0x0F );
	ILI9341_Write_Data ( 0x26 );
	ILI9341_Write_Data ( 0x24 );
	ILI9341_Write_Data ( 0x0B );
	ILI9341_Write_Data ( 0x0E );
	ILI9341_Write_Data ( 0x09 );
	ILI9341_Write_Data ( 0x54 );
	ILI9341_Write_Data ( 0xA8 );
	ILI9341_Write_Data ( 0x46 );
	ILI9341_Write_Data ( 0x0C );
	ILI9341_Write_Data ( 0x17 );
	ILI9341_Write_Data ( 0x09 );
	ILI9341_Write_Data ( 0x0F );
	ILI9341_Write_Data ( 0x07 );
	ILI9341_Write_Data ( 0x00 );
	
	/* Negative Gamma Correction (E1h) */
	ILI9341_Write_Command ( 0XE1 ); //Set Gamma
	ILI9341_Write_Data ( 0x00 );
	ILI9341_Write_Data ( 0x19 );
	ILI9341_Write_Data ( 0x1B );
	ILI9341_Write_Data ( 0x04 );
	ILI9341_Write_Data ( 0x10 );
	ILI9341_Write_Data ( 0x07 );
	ILI9341_Write_Data ( 0x2A );
	ILI9341_Write_Data ( 0x47 );
	ILI9341_Write_Data ( 0x39 );
	ILI9341_Write_Data ( 0x03 );
	ILI9341_Write_Data ( 0x06 );
	ILI9341_Write_Data ( 0x06 );
	ILI9341_Write_Data ( 0x30 );
	ILI9341_Write_Data ( 0x38 );
	ILI9341_Write_Data ( 0x0F );
	
	/* memory access control set */
	DEBUG_DELAY ();
	ILI9341_Write_Command ( 0x36 ); 	
	ILI9341_Write_Data ( 0xC8 );    /*竖屏  左上角到 (起点)到右下角 (终点)扫描方式*/
	DEBUG_DELAY ();
	
	/* column address control set */
	ILI9341_Write_Command (0x2A ); 
	ILI9341_Write_Data ( 0x00 );
	ILI9341_Write_Data ( 0x00 );
	ILI9341_Write_Data ( 0x00 );
	ILI9341_Write_Data ( 0xEF );
	
	/* page address control set */
	DEBUG_DELAY ();
	ILI9341_Write_Command ( 0x2B ); 
	ILI9341_Write_Data ( 0x00 );
	ILI9341_Write_Data ( 0x00 );
	ILI9341_Write_Data ( 0x01 );
	ILI9341_Write_Data ( 0x3F );
	
	/*  Pixel Format Set (3Ah)  */
	DEBUG_DELAY ();
	ILI9341_Write_Command ( 0x3a ); 
	ILI9341_Write_Data ( 0x55 );
	
	/* Sleep Out (11h)  */
	ILI9341_Write_Command ( 0x11 );	
	ILI9341_Delay ( 0xAFFf<<2 );
	DEBUG_DELAY ();
	
	/* Display ON (29h) */
	ILI9341_Write_Command ( 0x29 ); 
}

/*************************************************
Function: ILI9341_Delay
Description: 普通软件延时函数
Input: 无
Output:无
Return: 无
Others: 无
*************************************************/
void ILI9341_Delay(uint32_t time)
{
	while(time--);
}

/*************************************************
Function: ILI9341_Drew_Rec
Description: 在液晶屏上画出一单色矩形
Input: 矩形区域以(x1,y1),(x2,y2)两点为对角顶点,color为填充的颜色(RGB565)
Output:无
Return: 无
Others: 无
*************************************************/
void ILI9341_Drew_Rec(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color)
{
    
	uint32_t i = 0;

    ILI9341_Open_Windows(x1,y1,x2,y2);/*开启填充窗口*/
	
	ILI9341_Write_Command(0x2C);/*填充颜色*/
	
	for(i = 0;i<(x2-x1+1)*(y2-y1+1);i++)/*向每个像素点都填充颜色*/
	{
		ILI9341_Write_Data(color);
        
	}

}


/*************************************************
Function: ILI9341_Open_Windows
Description: 在液晶屏上开一个矩形区域，准备填充
Input: 矩形区域以(x1,y1),(x2,y2)两点为对角顶点
Output:无
Return: 无
Others: 此处要求x1<x2,y1<y2
*************************************************/
void ILI9341_Open_Windows(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
	
	ILI9341_Write_Command(0x2A);/*设定行界限*/
		
	ILI9341_Write_Data((y1&0xFF00)>>8);/*写入起始行高八位*/
	
	ILI9341_Write_Data(y1&0x00FF);/*写入起始行低八位，以下同理*/
	
	ILI9341_Write_Data((y2&0xFF00)>>8);
	ILI9341_Write_Data(y2&0x00FF);
	
	
	ILI9341_Write_Command(0x2B);/*设定列界限*/
	
	
	ILI9341_Write_Data((x1&0xFF00)>>8);/*写入起始列高八位*/
	
	ILI9341_Write_Data(x1&0x00FF);/*写入起始列低八位，以下同理*/
	
	ILI9341_Write_Data((x2&0xFF00)>>8);
	ILI9341_Write_Data(x2&0x00FF);
}


/*************************************************
Function: RGB888ToRGB565
Description: 对TCS34725采集到的RGB888数据装换为液晶屏使用的RGB565数据
Input: n888Color为RGB888颜色值
Output:无
Return: 转化后的RGB565值
Others: 无
*************************************************/
uint16_t RGB888ToRGB565(uint32_t n888Color)
{
	uint16_t n565Color = 0;
 
	/* 获取RGB单色，并截取高位*/
	uint8_t cRed   = (n888Color & RGB888_RED)   >> 19;
	uint8_t cGreen = (n888Color & RGB888_GREEN) >> 10;
	uint8_t cBlue  = (n888Color & RGB888_BLUE)  >> 3;
 
	/*数据拼接*/
	n565Color = (cRed << 11) + (cGreen << 5) + (cBlue << 0);
	return n565Color;
}

/*************************************************
Function: ILI9431_Drew_All_Bak
Description: 开启显示屏全部显示区域，并将其设为底色
Input: 无
Output:无
Return: 无
Others: 无
*************************************************/
void ILI9431_Drew_All_Bak(void)
{
    ILI9341_Drew_Rec(0,0,ILI9431_X_Lenth,ILI9431_Y_Lenth,ILI9431_Back_Color);
    
}

/*************************************************
Function: TSC2046_Calibration
Description: 以设定点为中心画一十字架
Input: x,y分别为坐标点x,y轴值，color为填充颜色
Output:无
Return: 无
Others: 无
*************************************************/
void ILI9431_Drew_Cross(uint16_t x,uint16_t y,uint16_t color)
{
    uint8_t i,j;
    /*开窗，准备填充*/
    ILI9341_Open_Windows(x-8,y-8,x+7,y+7);
    
    /*发送填充命令，开始填充*/
    ILI9341_Write_Command(0x2C);
    
    /*共有16行*/
    for(i=0;i<16;i++)
    {
        /*每行16个像素点*/
        for(j=0;j<16;j++)
        {
            /*判断填充条件*/
            if(i==7||i==8||j==7||j==8)/*填充中间两行及中间两列*/
            { 
                ILI9341_Write_Data(color);           
            }
            else/*否则填充底色*/
            {
                
                ILI9341_Write_Data(ILI9431_Back_Color);
            }
        }      
    }
}


/*************************************************
Function: ILI9431_Clean_Cross
Description: 清除以坐标点为中心的交叉点
Input: x,y分别为坐标点x,y轴值
Output:无
Return: 无
Others: 无
*************************************************/
void ILI9431_Clean_Cross(uint16_t x,uint16_t y)
{
    ILI9341_Drew_Rec(x - 8,y -8,x + 7,y + 7,ILI9431_Back_Color);
}


/*************************************************
Function: ILI9341_Draw_Point
Description: 在屏幕上画出（x1,y1）点
Input: x,y分别为坐标点x,y轴值,color为点颜色
Output:无
Return: 无
Others: 无
*************************************************/
void ILI9341_Draw_Point(uint16_t x,uint16_t y,uint16_t color)
{
    ILI9341_Open_Windows(x,y,x+1,y+1);/*开窗*/
     
    ILI9341_Write_Command(0x2C);/*发送填充命令*/
    ILI9341_Write_Data(color); /*填充颜色*/
}

/*************************************************
Function: ILI9341_Drew_Line
Description: 在屏幕上画出以（x1,y1）,(x2,y2)两点为端点的直线
Input: （x1,y1）,(x2,y2)两点为直线端点坐标,color为点颜色
Output:无
Return: 无
Others: 无
*************************************************/
void ILI9341_Drew_Line(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color)
{
    float k = 0;/*类似直线方程斜率，用于确定上下两行需画点的位置左右相差几个像素点*/
    uint16_t x,y,min,max;
     k = (float)(y2-y1)/(float)(x2-x1);
    
    if(x1==x2)/*两点X坐标相同则与斜率无关*/
    {
        min = y1<y2?y1:y2;/*判断y1,y2中的大小两值*/
        max = y1<y2?y2:y1;
        for(y=min; y<max; y++)
        {
            ILI9341_Draw_Point(x1+1,y,color);/*带加粗效果*/
            ILI9341_Draw_Point(x1-1,y,color);
            ILI9341_Draw_Point(x1,y,color);
        }
    }
    else
    {
        min = x1<x2?x1:x2;/*判断x1,x2中的大小两值*/
        max = x1<x2?x2:x1;
        for(x=min; x<max; x++)
        {
            y = (k * (x - x1)) + y1;/*计算y值所在点*/
            ILI9341_Draw_Point(x+1,y,color);/*画三个点，加粗效果*/
            ILI9341_Draw_Point(x-1,y,color);
            ILI9341_Draw_Point(x,y,color);
        }
    }
    
}

/*************************************************
Function: ILI9341_Next_Win
Description: 屏幕进入下一窗口
Input: 无
Output:无
Return: 无
Others: 无
*************************************************/
void ILI9341_Next_Win(void)
{
    Clock_Print_Model = 0;/*换界面时Clock_Print_Model重置为0*/
    Print_State = 0;/*改变打印状态，允许屏幕打印提示信息*/
    ILI9431_Drew_All_Bak();/*清空当前画面*/
    if(Print_Model<6)
     {
         Print_Model += 1;/*进入下一画面模式*/
     }
    else/*画面已满，从零开始*/
    {
         Print_Model = 0;
    }
}

/*************************************************
Function: ILI9341_Last_Win
Description: 屏幕进入上一窗口
Input: 无
Output:无
Return: 无
Others: 无
*************************************************/
void ILI9341_Last_Win(void)
{
    Clock_Print_Model = 0;/*换界面时Clock_Print_Model重置为0*/
    Print_State = 0;/*改变打印状态，允许屏幕打印提示信息*/
    ILI9431_Drew_All_Bak();/*清空当前画面*/
    if(Print_Model>0)
     {
         Print_Model -= 1;/*进入上一画面模式*/
     }
    else/*画面已满，从画面6开始*/
    {
         Print_Model = 6;
    }
}

/*************************************************
Function: ILI9341_Line_Print
Description: 在指定的行数打印字符串
Input: line为指定行数，str为字符串
Output:无
Return: 无
Others: 无
*************************************************/
void ILI9341_Line_Print(uint8_t line,char* str)
{
    ILI9341_Print_Str(B*(line-1),0,str);/*调用字符串输出函数*/
}


