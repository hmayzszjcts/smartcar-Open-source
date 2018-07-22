#include "include.h"
uint8 flag_100ms,flag_record;  //控制主循环的执行
void main (void)
{
  DisableInterrupts;  //关闭中断 
  All_Init();   
  EnableInterrupts; //打开中断
 
  while(1)
  {
    if(SendPara)
    {
      Send_Para();
      SendPara=0;
    }
    if(!SaveData&&Uart_Send) //保存数据时不能发串口
    {
      if(SendCCD)
      {
       Send_CCD();
       SendCCD=0;
      }
       Send_Variable();     
    } 
    if(SendSD)
    {
      SendSD=0;
      Send_SD();
    } 
    if(flag_100ms)        //只有停止的时候才执行
    {
       Read_Switch();
       if(OLED_Refresh)
       { 
        Check_BottonPress();        //防止误触发
        if(Stop==true||SaveData==false)  OLED_Draw_UI();
       }
       else Check_StartPress();     //关闭液晶显示，检查启动按键是否按下
       flag_100ms=0;
    }
    if(flag_record&&SaveData&&SD_Operation)   //如果SD允许操作并且打开了SD卡记录功能，那么
   {
      if(Stop!=true) //如果小车正在运行，则记录
     {
      if (Starting) Block_Index=1;
      else Record();                     //存储数据  
     }
     else if(CarStopedJustNow) //小车刚停止
    {
        CarStopedJustNow=false; 
        Write_Information();
    }
      flag_record=false;
   }
  } 
}
void pit0_isr(void)
{
  static uint8  part=0; 
  static uint8  cnt100ms;
  part++;
  cnt100ms++;
  SpeedCount++;    
  //循环计数控制
  
   switch(part) 
 {
    case 1: 
           //直立控制
            if(CCD_Exposure_Time==1)   CCD_Exposure();  //曝光        
            Get_Attitude();       
            Angle_Calculate();
            Angle_Control();
            //速度控制  
            Get_Speed();   //5ms采集一次速度
            SpeedCount++;  
            if(SpeedCount>= Speed_Filter_Times) 
           {                 
               Speed_Control();
               SpeedCount=0;
           } 
           Speed_Control_Output(); 
            break; 
    case 2: //CCD采集 
        
            CCD_Capture(); //采集CCD                              
            break;
    case 3: 
             //方向控制
            if(CCD_Exposure_Time==4)    CCD_Exposure(); 
            CCD_Normalization();    
            break;  
    case 4: 
            if(CCD_Exposure_Time==3)   CCD_Exposure(); 
            RoadType_Distinguish();  
           if((RunTime<0.1)&&(Stop==0))  //防止起步的时候乱飘
          {
            Rightlastfind=0;
            Leftlastfind=0;
            Middle_Err=0;
          }
            Direction_Control(); 
            DirectionCount=0; 
            flag_record=1;                        //开记录标志位
            SendCCD=1; 
             break;
    case 5:
             part=0;  
             if(CCD_Exposure_Time==2)   CCD_Exposure(); 
             break;
    default:
            break;   
 }
 
  DirectionCount++;
  Direction_Control_Output();
  Moto_Out();          //每毫秒都输出控制
  
 ///////////////////////////////////////////////LED状态指示 
  if(cnt100ms==100)
 {
    if(Stop)
    {
        
          LED2_TURN;
          LED1_OFF;
          LED0_OFF;
    }
    else
    {
     if(Starting)
     {
     LED0_TURN;
     LED1_OFF;
     LED2_OFF;
     }
     else
     {
     
     LED0_OFF;
     LED1_TURN;
     LED2_OFF;
     }
    }
    cnt100ms=0;
    flag_100ms=1;
 }
 ////////////////////////////////////////////启动
 if(Starting) 
 {  
      Start_Cnt--;
      if(Start_Cnt==0)
      {
       Starting=false;
      }
   
  } 
 /////////////////////////////////////////////统计时间及速度
 if(Stop==false&&Starting==false)
 {
  RunTime=RunTime+0.001;
  AverageSpeed=Distance/RunTime;//求得运行时间
 }
}