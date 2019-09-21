#include "stm32f4xx.h"
#include "main.h"

/*条件编译相关*/
#define MPU9250_CALIB  1    /*置1时对角速度计数据进行校准*/
#define TCS3472_CALIB  0    /*置1时对颜色传感器进行白平衡校准*/
#define TSC2046_CALIB  0    /*置1时使用触摸校准画面，对触摸面板进行校准*/

int main(void)
{
    
    /*系统初始化*/    
    SYSTEM_Init();
    
    #if MPU9250_CALIB /*MPU9250加速度计及陀螺仪校准*/    
    MPU9250_Offset();  
    #endif  /*MPU9250_CALIB*/
    
    #if TCS3472_CALIB /*TCS3472颜色传感器白平衡校准*/    
    TCS3472_White_Balance();
    #endif  /*TCS3472_CALIB*/
    
    #if TSC2046_CALIB /*触摸面板校正，校准时会在液晶屏显示校准点，依次点击校准点即可*/    
    TSC2046_Calibration();    
    #endif  /*TSC2046_CALIB*/
        
     while(1)
    {
    
/*******************MPU9250工作，不需判断延时***********************/        
        
            MPU9250_Get_Angle(&MPUANG);/*将角度值赋值到结构体*/
            ang = (int16_t)MPUANG.Angle_Y;/*从结构体得到角度值*/
            HOST_Send_float(MPUANG.Angle_Y,0xF3);/*将数据发送到匿名上位机*/
            
            
            if(Print_Model==4)/*在倾角显示界面（Print_Model=4）要将倾角状态通过屏幕显示，
                                其中（112，80）为画线原点，Line_Length为线长度*/
            {
               x2 = 112 + Line_Length * cos(ang*RAD_to_DEG);/*计算画线终点的X,Y坐标*/
               y2 = 80 + Line_Length * sin(ang*RAD_to_DEG);
                
               ILI9341_Drew_Rec(112,80-Line_Length,112+Line_Length,80+Line_Length,BLACK);/*清空上一画面*/
                
               ILI9341_Drew_Line(112,80,x2,y2,RED);/*画线*/
               sprintf(str,"倾角为%.3lf°\n倾斜状态如下",MPUANG.Angle_Y);/*转换为字符串*/
               ILI9341_Line_Print(3,str);/*将字符串输出至屏幕*/
               
            }
            else if(Print_Model==6)/*在全局模式（Print_Model=6）也要显示，其中（128，50）为画线原点*/
            {
               x2 = 128 + Line_Length * cos(ang*RAD_to_DEG);/*计算画线终点的X,Y坐标*/
               y2 = 50 +  Line_Length * sin(ang*RAD_to_DEG);
                
               ILI9341_Drew_Rec(128,50-Line_Length,128+Line_Length,50+Line_Length,BLACK);/*清空上一画面*/
               ILI9341_Drew_Line(128,50,x2,y2,RED);
                
               sprintf(str,"倾角为%.3lf°\n",MPUANG.Angle_Y);
               ILI9341_Line_Print(3,str);   
            }
            
            /*此处设定显示倾角界面（Print_Model=4）及全局模式（Print_Model=6）时无法通过倾角改变屏幕界面*/
             else if(MPUANG.Angle_Y>30)/*当传感器倾角大于30°时，准备进行界面切换*/
            {
                
                ILI9341_Print_Str(112,144,"即将向右切换");
                General_TIM_Delay_ms(500);/*延时等待判断结果*/
                MPU9250_Get_Angle(&MPUANG);/*再判断一次角度，是否真的需要切换*/
                if(MPUANG.Angle_Y>30)/*需要向右切换*/
                {
                    ILI9341_Next_Win();
                }
                else
                {
                    ILI9341_Drew_Rec(112,0,128,240,ILI9431_Back_Color);/*不需切换，清除提示语句*/
                }
                General_TIM_Delay_ms(5);
            }
            
            else if(MPUANG.Angle_Y<-30)/*当传感器倾角大于30°时，准备进行界面切换*/
            {
                ILI9341_Print_Str(112,0,"即将向左切换");
                General_TIM_Delay_ms(500);
                MPU9250_Get_Angle(&MPUANG);
                if(MPUANG.Angle_Y<-30)/*需要向左切换*/
                {
                    ILI9341_Last_Win();
                }
                else
                {
                    ILI9341_Drew_Rec(112,0,128,240,ILI9431_Back_Color);/*不需切换，清除提示语句*/
                }
                
                General_TIM_Delay_ms(5);
            }
             
        
/***********************第一个判断，判断是否有遥控信号被采集*********************/       
    if(IC_Stase==1)
    {
        remote_rec = HX1838_Deal_Data(HX1838_REC_Data());/*获取按键键值*/
            switch(remote_rec)/*判断按键键值并做相关处理（改变界面状态Print_Model）*/
            {
            case 1:/*按下1*/
                Print_State = 0;/*使屏幕可以打印提示信息*/
                ILI9431_Drew_All_Bak();/*清空屏幕*/
                Print_Model = 0;/*改变Print_Model值*/
            break;
            
            case 2:/*按下2*/
                Print_State = 0;
                ILI9431_Drew_All_Bak();
                Print_Model = 1;
            break;
            
            case 3:/*按下3*/
                Print_State = 0;
                ILI9431_Drew_All_Bak();
                Print_Model = 2;
            break;
            
            case 4:/*按下4*/
                Print_State = 0;
                ILI9431_Drew_All_Bak();
                Print_Model = 3;
            break;
            
            case 5:/*按下5*/
                Print_State = 0;
                ILI9431_Drew_All_Bak();
                Print_Model = 4;
            break;
            
            case 6:/*按下6*/
                Print_State = 0;
                ILI9431_Drew_All_Bak();
                Print_Model = 5;
            break;
            
            case 7:/*按下向左<键*/
                ILI9341_Last_Win();
            break;
            
            case 8:/*按下向右>键*/
                ILI9341_Next_Win();
            break;
            
            default:
            break;
                
            }

    }    
  

       
/***************第二个判断，判断是否需要打印画面静态文字提示信息********************/
       if(Print_State == 0)
        {
            Print_State = 1;/*改变标志，防止屏幕不断打印占用系统资源*/ 
            
            ILI9341_Drew_Rec(250,0,300,64,RED);/*左右切换按键红色区域*/
            ILI9341_Drew_Rec(250,176,300,240,RED);
            
             switch(Print_Model)/*判断要显示哪个画面*/
             {
                 case 0:/*主画面0，显示DHT12温度信息*/
                     ILI9341_Print_Str(200,28,"触摸两侧按钮可切换窗口");
                     ILI9341_Print_Str(230,22,"当前显示DHT12温度采集结果");
                     ILI9341_Print_Str(301,0,"全局模式");
                     ILI9341_Print_Str(301,176,"触点信息");   
                 break;
                 
                 case 1:/*主画面1，显示触点信息*/
                     ILI9341_Print_Str(230,36,"当前显示触点坐标信息");
                     ILI9341_Print_Str(180,60,"按下后开始显示");
                     ILI9341_Print_Str(301,0,"DHT温度");
                     ILI9341_Print_Str(301,146,"MAX6675温度");
                 break;
                 
                 case 2:/*主画面2，显示RGB颜色信息*/
                     ILI9341_Print_Str(230,6,"当前显示热电偶温度传感器结果");
                     ILI9341_Print_Str(301,0,"触点信息");
                     ILI9341_Print_Str(301,176,"RGB颜色");
                 break;
                 
                 case 3:/*主画面3，显示颜色传感器结果*/
                     ILI9341_Print_Str(230,16,"当前显示RGB颜色传感器结果");
                     ILI9341_Print_Str(301,0,"触点信息");
                     ILI9341_Print_Str(301,176,"倾角信息");
                 break;
                 
                 case 4:/*主画面4，显示倾斜状态*/
                     ILI9341_Print_Str(180,28,"此窗口无法使用倾角切换");
                     ILI9341_Print_Str(230,26,"当前显示加速度计倾角信息");
                     ILI9341_Print_Str(301,0,"RGB颜色");
                     ILI9341_Print_Str(301,176,"时间调整");
                 break;
                 
                 case 5:/*主画面5，时间调整画面*/
                     ILI9341_Print_Str(301,0,"倾角信息");
                     ILI9341_Print_Str(301,176,"全局模式");
                        switch(Clock_Print_Model)/*判断时间调整画面模式，并作出相关反应*/
                        {
                            case 0:/*主画面6*/
                                /*显示与时间调整有关的提示信息*/
                            ILI9341_Print_Str(201,0,"时钟调整");
                            ILI9341_Print_Str(201,176,"日期调整");              
                            ILI9341_Drew_Rec(160,0,200,64,0x0A10);
                            ILI9341_Drew_Rec(160,176,200,240,0x0A10);                
                            break;
                            
                            case 1:/*主画面6中的子画面1*/
                                /*调整时钟时的画面*/
                            ILI9341_Print_Str(241,102,"确认");
                            ILI9341_Drew_Rec(200,90,240,150,0x0F30);/*0x0F30为一颜色值（RGB565）*/
                            RCT_Tem_Time.tem_hour = RTC_TimeStructure.RTC_Hours;
                            RCT_Tem_Time.tem_min = RTC_TimeStructure.RTC_Minutes;
                            sprintf(str,"小时\n上调\n\n%02d\n\n小时\n下调\n",RCT_Tem_Time.tem_hour);
                            ILI9341_Print_Str(64,0,str);
                            sprintf(str,"分钟\n上调\n\n%02d\n\n分钟\n下调\n",RCT_Tem_Time.tem_min);
                            ILI9341_Print_Str(64,192,str);                
                            break;
                            
                            case 2:/*主画面6中的子画面2*/
                                /*调整日期时的画面*/
                            ILI9341_Print_Str(241,102,"确认");
                            ILI9341_Drew_Rec(200,90,240,150,0x0F30);
                            RCT_Tem_Time.tem_year = RTC_DateStructure.RTC_Year+2000;
                            RCT_Tem_Time.tem_month = RTC_DateStructure.RTC_Month;
                            RCT_Tem_Time.tem_date = RTC_DateStructure.RTC_Date;
                            RCT_Tem_Time.tem_week = RTC_DateStructure.RTC_WeekDay;
                            
                            sprintf(str,"年份\n上调\n\n%04ld\n\n年份\n下调\n",RCT_Tem_Time.tem_year);
                            ILI9341_Print_Str(64,0,str);
                            sprintf(str,"月份\n上调\n\n%02d\n\n月份\n下调\n",RCT_Tem_Time.tem_month);
                            ILI9341_Print_Str(64,64,str);
                            sprintf(str,"日期\n上调\n\n%02d\n\n日期\n下调\n",RCT_Tem_Time.tem_date);
                            ILI9341_Print_Str(64,128,str);
                            sprintf(str,"星期\n上调\n\n%02d\n\n星期\n下调\n",RCT_Tem_Time.tem_week);
                            ILI9341_Print_Str(64,192,str);  
                            ILI9341_Line_Print(3,"注意平闰年及大小月");
                            break;
                            
                            }
                
                    break; 
                 
                 case 6:/*主画面6，全局模式*/
                     ILI9341_Print_Str(180,28,"此窗口无法使用倾角切换");
                     ILI9341_Print_Str(230,36,"当前显示全部采集信息");
                     ILI9341_Print_Str(301,0,"时间调整");
                     ILI9341_Print_Str(301,176,"DHT温度");
                 break;
                 
                 default:
                     break;
             }

        }  

        
/**************第三个判断，判断DHT12是否可以采集温度数据，若可采集，则IIC总线给DHT12使用*********************/         
        if((SenState&0x02)==0x02)
        {
            /*满足时间间隔，则采集数据*/
            DHT12_Read_(Dhtdata);
            if(DHT12_Deal_Tem_Hum(Dhtout,Dhtdata))/*数据校准正确*/
            {
                HOST_Send_float(Dhtout[1],0xF1);
                HOST_Send_float(Dhtout[0],0xF2);
                if(Print_Model==0)/*在第一个窗口单独打印到屏幕上*/
                {
                    sprintf(dhtsoult,"温度：%.2f℃\n湿度：%.2f％\n",Dhtout[1],Dhtout[0]);
                    ILI9341_Line_Print(3,dhtsoult);
                }
                else if(Print_Model==6)/*全局模式中也要显示*/
                {
                    sprintf(dhtsoult,"温度：%.2f℃\n湿度：%.2f％\n",Dhtout[1],Dhtout[0]);
                    ILI9341_Line_Print(6,dhtsoult);
                }
            }
     
            /*标志位回置*/
            SenState &= ~(0x02);
        }        

        
/**********第四个判断，判断屏幕指定区域是否被按下，若是以则改变相关状态值，使屏幕可以改变打印画面************/       
         if(TCS_2046_PEN_Low)/*屏幕按下时，触摸中断引脚变为低电平*/
         {
             
             TSC2046_Calculate_XY(touch_posit);/*计算物理坐标*/
             
              if(Print_Model==1)/*在主界面1(Print_Model==1)中要将触点信息显示在屏幕上*/
             {
                 sprintf(str,"触点坐标为（%03hu，%03hu）",touch_posit[0],touch_posit[1]);
                 ILI9341_Print_Str(32,0,str);
             } 
             else if(Print_Model==6)/*全局模式(Print_Model==6)中也要显示*/
              {
                 sprintf(str,"触点坐标为（%03hu，%03hu）",touch_posit[0],touch_posit[1]);
                 ILI9341_Line_Print(8,str);
              }
             point_state = TSC2046_Applic(touch_posit);/*获取TSC2046_Applic返回值，此值代表相关触点区域*/            
             switch(point_state)/*依据point_state的不同值进行不同操作*/
             {
                 case 1:/*切换到上一窗口*/
                     ILI9341_Last_Win();
                     break;
                 
                 
                 case 2:/*切换到下一窗口*/
                     ILI9341_Next_Win();
                     break;
                 
                 
                 case 3:
                     /*进入时间调整子画面（Clock_Print_Model=1）*/
                  Clock_Print_Model = 1;
                  Print_State = 0;
                  ILI9431_Drew_All_Bak();                 
                     break;
                 
                 
                 case 4:
                     /*进入日期调整子画面（Clock_Print_Model=2）*/
                  Clock_Print_Model = 2;
                  Print_State = 0;
                  ILI9431_Drew_All_Bak();                 
                     break;
                 
                 
                 case 5:/*按下确认键*/
                    Print_State = 0;
                    if(Clock_Print_Model==1)/*时钟调整过后按确认键，表示要重新设置时间*/
                    {
                        /*设置新时钟*/                
                      RTC_Set_Time(RTC_DateStructure.RTC_Year-48,
                        RTC_DateStructure.RTC_Month,
                        RTC_DateStructure.RTC_Date,
                        RTC_DateStructure.RTC_WeekDay,
                        RCT_Tem_Time.tem_hour,RCT_Tem_Time.tem_min,00,RTC_H12_PM);
                      
                    } 
                    
                    else if(Clock_Print_Model==2)/*日期调整过后按确认键，表示要重新设置时间*/
                    {
                        /*设置新日期*/               
                      RTC_Set_Time(RCT_Tem_Time.tem_year,RCT_Tem_Time.tem_month,RCT_Tem_Time.tem_date,RCT_Tem_Time.tem_week,
                        RTC_TimeStructure.RTC_Hours,
                        RTC_TimeStructure.RTC_Minutes,
                        RTC_TimeStructure.RTC_Seconds,RTC_H12_PM);
                      
                    }
                    
                      Clock_Print_Model = 0;/*回到时间调整主界面（Clock_Print_Model=0）*/
                      ILI9431_Drew_All_Bak();                                  
                     break;
                 
                 
                 case 6:/*小时值上调*/                     
                 if(RCT_Tem_Time.tem_hour<23)/*hour最大为23*/
                 {
                     RCT_Tem_Time.tem_hour++;
                     General_TIM_Delay_ms(50);/*防止过快切换*/
                 }
                 else/*超出重置*/
                 {
                     RCT_Tem_Time.tem_hour = 0;
                 }
                 sprintf(str,"%02d",RCT_Tem_Time.tem_hour);/*将调整过程中的数字转换为字符串*/
                 ILI9341_Print_Str(112,0,str);/*显示调整画面中的数值改变*/                 
                     break;
                 
                 
                 case 7:/*小时值下调*/                     
                 if(RCT_Tem_Time.tem_hour>0)/*hour最小为0*/
                 {
                     RCT_Tem_Time.tem_hour--;
                     General_TIM_Delay_ms(50);/*防止过快切换*/
                 }
                 else/*超出重置*/
                 {
                     RCT_Tem_Time.tem_hour = 23;
                 }
                 sprintf(str,"%02d",RCT_Tem_Time.tem_hour);/*将调整过程中的数字转换为字符串*/
                 ILI9341_Print_Str(112,0,str);
                     break;
                 
                 
                 case 8:
                     /*分钟值上调*/
                 if(RCT_Tem_Time.tem_min<59)/*min最大为59*/
                 {
                     RCT_Tem_Time.tem_min++;
                     General_TIM_Delay_ms(50);/*防止过快切换*/
                 }
                 else/*超出重置*/
                 {
                     RCT_Tem_Time.tem_min = 0;
                 }
                 sprintf(str,"%02d",RCT_Tem_Time.tem_min);/*将调整过程中的数字转换为字符串*/
                 ILI9341_Print_Str(112,192,str);                 
                     break;
                 
                 
                 case 9:
                     /*分钟值下调*/
                 if(RCT_Tem_Time.tem_min>0)/*min最为小0*/
                 {
                     RCT_Tem_Time.tem_min--;
                     General_TIM_Delay_ms(50);/*防止过快切换*/
                 }
                 else/*超出重置*/
                 {
                     RCT_Tem_Time.tem_min = 59;
                 }
                 sprintf(str,"%02d",RCT_Tem_Time.tem_min);/*将调整过程中的数字转换为字符串*/
                 ILI9341_Print_Str(112,192,str);
                     break;
                 
                 
                 case 10:
                     /*年份向上调整*/
                 RCT_Tem_Time.tem_year++;
                 General_TIM_Delay_ms(50);/*防止过快切换*/
                 sprintf(str,"%04ld",RCT_Tem_Time.tem_year);/*将调整过程中的数字转换为字符串*/
                 ILI9341_Print_Str(112,0,str);
                     break;
                 
                 
                 case 11:
                     /*年份向下调整*/
                 RCT_Tem_Time.tem_year--;
                 General_TIM_Delay_ms(50);/*防止过快切换*/
                 sprintf(str,"%04ld",RCT_Tem_Time.tem_year);/*将调整过程中的数字转换为字符串*/
                 ILI9341_Print_Str(112,0,str);
                     break;
                 
                 
                 case 12:
                     /*月份向上调整*/
                 if(RCT_Tem_Time.tem_month<12)/*月份最大值为12*/
                 {
                     RCT_Tem_Time.tem_month++;
                 }   
                 else
                 {
                     RCT_Tem_Time.tem_month = 1;
                 }
                 General_TIM_Delay_ms(50);/*防止过快切换*/
                 sprintf(str,"%02d",RCT_Tem_Time.tem_month);
                 ILI9341_Print_Str(112,64,str);
                     break;
                 
                 
                 case 13:
                     /*月份向下调整*/
                 if(RCT_Tem_Time.tem_month>0)/*月份最大值为12*/
                 {
                     RCT_Tem_Time.tem_month--;
                 }   
                 else
                 {
                     RCT_Tem_Time.tem_month = 12;
                 }
                 General_TIM_Delay_ms(50);/*防止过快切换*/
                 sprintf(str,"%02d",RCT_Tem_Time.tem_month);
                 ILI9341_Print_Str(112,64,str);
                     break;
                 
                 
                 case 14:
                     /*日期向上调整*/
                 if(RCT_Tem_Time.tem_date<31)/*月份最大值为31*/
                 {
                     RCT_Tem_Time.tem_date++;
                 }   
                 else
                 {
                     RCT_Tem_Time.tem_date = 1;
                 }
                 General_TIM_Delay_ms(50);/*防止过快切换*/
                 sprintf(str,"%02d",RCT_Tem_Time.tem_date);
                 ILI9341_Print_Str(112,128,str);
                     break;
                 
                 
                 case 15:
                      /*日期向下调整*/
                 if(RCT_Tem_Time.tem_date>1)
                 {
                     RCT_Tem_Time.tem_date--;
                 }   
                 else
                 {
                     RCT_Tem_Time.tem_date = 31;
                 }
                 General_TIM_Delay_ms(50);/*防止过快切换*/
                 sprintf(str,"%02d",RCT_Tem_Time.tem_date);
                 ILI9341_Print_Str(112,128,str);
                     break;
                 
                 
                 case 16:
                     /*星期向上调整*/
                 if(RCT_Tem_Time.tem_week<7)/*星期最大值为7*/
                 {
                     RCT_Tem_Time.tem_week++;
                 }   
                 else
                 {
                     RCT_Tem_Time.tem_week = 1;
                 }
                 General_TIM_Delay_ms(50);/*防止过快切换*/
                 sprintf(str,"%02d",RCT_Tem_Time.tem_week);
                 ILI9341_Print_Str(112,192,str);
                     break;
                 
                 case 17:
                     /*星期向下调整*/
                 if(RCT_Tem_Time.tem_week>1)/*星期最小值为1*/
                 {
                     RCT_Tem_Time.tem_week--;
                 }   
                 else
                 {
                     RCT_Tem_Time.tem_week = 7;
                 }
                 General_TIM_Delay_ms(50);/*防止过快切换*/
                 sprintf(str,"%02d",RCT_Tem_Time.tem_week);
                 ILI9341_Print_Str(112,192,str);
                     break;
                 
                 default:
                     break;
                 
             }
         }



/**********第五个判断，判断TCS3472传感器延时条件（约200ms）是否满足，且不与DHT12抢占IIC资源***********/       
      if(((SenState&0x01)==0x01))
      {
            SenState &= ~(0x01);/*清除标志位*/ 
            RGB888color = TCS3472_Read_Deal();
            HOST_Send_32bit(RGB888color,0xF5);
            if(Print_Model==3)
            {
                 sprintf(str,"RGB888值为%#x\n颜色如下",RGB888color);
                 ILI9341_Line_Print(3,str);
                 RGB565color = RGB888ToRGB565(RGB888color);
                 ILI9341_Drew_Rec(64,0,128,64,RGB565color);
            }
            if(Print_Model==6)
            {
                 sprintf(str,"RGB888值为%#x\n",RGB888color);
                 ILI9341_Line_Print(4,str);
                 RGB565color = RGB888ToRGB565(RGB888color);
                 ILI9341_Drew_Rec(32,176,96,240,RGB565color);
            }
          
      }	
      
      
/****************第六个判断，判断是否要采集MAX6675数据*********************/
      if((SenState&0x04)==0x04)
      {
          SenState &= ~(0x04);/*清除标志位*/
          
          maxsoult = MAX6675_DealData(MAX6675_AVE());
          HOST_Send_float(maxsoult,0xF4);/*发送结果到匿名上位机*/
          sprintf(str,"感测温度为%.2f℃\n",maxsoult);
          if(Print_Model==2)
          {
              
              ILI9341_Line_Print(3,str);
          }
          if(Print_Model==6)
          {
              
              ILI9341_Line_Print(5,str);   
          }
          
          
      }
 

/*获取实时时间*/
    RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
    RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
/******************第七个判断，判断时间值是否有更新*********************/
        if (Rtctmp != RTC_TimeStructure.RTC_Seconds)/*秒值有更新*/
        {
            
            sprintf(str,"时钟 : %0.2d:%0.2d:%0.2d\n日期 : Y:20%0.2d-M:%0.2d-D:%0.2d-W:%0.2d\n",
                    RTC_TimeStructure.RTC_Hours,
                    RTC_TimeStructure.RTC_Minutes,
                    RTC_TimeStructure.RTC_Seconds,
                    RTC_DateStructure.RTC_Year,
                    RTC_DateStructure.RTC_Month,
                    RTC_DateStructure.RTC_Date,
                    RTC_DateStructure.RTC_WeekDay);
            ILI9341_Print_Str(0,0,str);/*打印时间*/
            
            Rtctmp = RTC_TimeStructure.RTC_Seconds;/*更新临时变量*/
        }


  }
 

}

/*************************************************
Function: SYSTEM_Init
Description: 系统配置初始化
Input: 无
Output:无
Return: 无
Others: 无
*************************************************/
void SYSTEM_Init(void)
{
    IC_TIM_Init();/*定时器初始化*/
    General_TIM_Init();
    
    LED_GPIO_Conf();/*LED用于简单调试*/
	USART_User_Init();

    W25Q16_SPI_Conf();/*外部Flash硬件SPI初始化*/
    
    ILI9341_Init();/*液晶屏初始化*/

    TSC2046_SoftSPI_GPIO_Conf();/*TSC2046触摸面板模拟SPI初始化*/
    
    IIC_CONF();/*使用模拟IIC*/
    
    MPU9250_Init();/*MPU9250初始化*/
    TCS3472_Init();/*颜色传感器初始化*/
    MAX6675_SPI_Conf();/*热电偶温度传感器初始化*/
    RTC_UserInit();/*实时时钟初始化*/
     
}
