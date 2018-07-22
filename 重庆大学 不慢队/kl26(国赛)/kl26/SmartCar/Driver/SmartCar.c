#include "include.h"
uint8 flag_100ms,flag_record;  //控制主循环的执行

void PIT_IRQHandler(void);

void main (void)
{
  DisableInterrupts;  //关闭中断 
  All_Init();   
  set_vector_handler(PIT_VECTORn ,PIT_IRQHandler);
  set_vector_handler(UART0_VECTORn ,UART0_RX_IRQHandler); 
  enable_irq (PIT_IRQn);    
  NVIC_SetPriority(UART0_IRQn,1);
  NVIC_SetPriority(PIT_IRQn,2);
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
    if(SendSD&&Stop)
    {
      SendSD=0;
      Send_SD();
    } 

    if(flag_100ms)        
    {
       Read_Switch();
       if(OLED_Refresh)
       { 
        Check_BottonPress();  
       if(Stop==true||SaveData==false)  OLED_Draw_UI();
       }
       else Check_StartPress();    
       flag_100ms=0;
    }        
    if(flag_record&&SaveData&&SD_OK) 
   {
      if(Stop!=true)             //如果小车正在运行，则记录
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
void PIT_IRQHandler(void)
{
  if(PIT_TFLG(PIT0) == 1 )  
  {
      static uint8  part=0; 
      static uint8  cnt100ms;
      //灯塔相关
      static uint8 light_tower_cnt;
      static uint8 light_tower_delay;
      static int pulse_cnt;
      static uint8 light_tower_on=0;
      
      part++;
      cnt100ms++;
      SpeedCount++;    
      //循环计数控制
       switch(part) 
     {
        case 1: 
               //直立控制
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
               CCD_Normalization();
               read_buff[0]= Get_Y_Gyro();
                break;  
        case 4: 
                RoadType_Distinguish();  
                               
               if((RunTime<0.1)&&(Stop==0))  //防止起步的时候乱飘
              {
                Rightlastfind=0;
                Leftlastfind=0;
                Middle_Err=0;
              }
                Direction_Control(); 
                DirectionCount=0;     
                flag_record=1;                    //开记录标志位
                SendCCD=1; 
                break;
        case 5:
                part=0; 
                read_buff[1]= Get_Y_Gyro();
                
                break;
        default:
                break;   
     }
      DirectionCount++;
      Direction_Control_Output();
      Moto_Out();          
    ///////////////////////////////////////////////////灯塔测试

    if(Stop)   
    {
        light_tower_delay++;
         if(light_tower_on&&light_tower_delay==100) 
         {
            light_tower_delay=0;
             pulse_cnt=lptmr_pulse_get();
             if(pulse_cnt<10)
             {
               if(Light_Tower_Index!=0) Light_Tower_OFF;
               light_tower_on=0;
             }
             lptmr_pulse_clean();
         }
          if(light_tower_on==0&&light_tower_delay==10)  //关断灯塔10ms后再打开
          {
            light_tower_delay=0;
            Light_Tower_ON;
            light_tower_on=1;
            lptmr_pulse_clean();
          }
     }  
     ///////////////////////////////////////////////LED状态指示 
      if(cnt100ms>=100)
     {
        if(Stop)
        {
          LED_BLUE_TURN;
          LED_RED_OFF;
          LED_GREEN_OFF;
        }
        else
        {
         if(Starting)
         {
          LED_GREEN_TURN;
          LED_RED_OFF;
          LED_BLUE_OFF;
         }
         else
         {
          LED_RED_TURN;
          LED_GREEN_OFF;
          LED_BLUE_OFF;
         }
        }
        cnt100ms=0;
        flag_100ms=1;
     }
     ////////////////////////////////////////////启动
     if(Starting) 
     {  
        if(Light_Tower_Index==0)        //不起用灯塔检测，倒计时启动
        {
          Start_Cnt--;
          if(Start_Cnt==0)
          {
           Starting=false;
          }
        }
        else                                      //灯塔起跑
        {
          light_tower_delay++;
          if(light_tower_detected==0)
         {
            if(light_tower_delay==200)                   
            { 
              pulse_cnt=lptmr_pulse_get();    
              if(pulse_cnt>150) 
              {
               light_tower_detected=1;
               light_tower_delay=0;
               Light_Tower_ON;
              }
              else   Light_Tower_OFF;
              lptmr_pulse_clean();
            }
            if(light_tower_delay>=210)
            {
              light_tower_delay=0;
              Light_Tower_ON;
            }
          }
         else
         {
            if(light_tower_delay==10)                   //起跑时距离灯塔很近所以接收到的是连续波，一旦断掉就认为可以起跑了
            { 
              pulse_cnt=lptmr_pulse_get();   
              if(pulse_cnt<5) 
              {
                Starting=false; 
                light_tower_cnt=0;
                light_tower_on=0;
              }
              light_tower_delay=0;
              lptmr_pulse_clean();
            }
          }
        }
      }
     ///////////////////////////////////////////////////停车
      if(((RunTime>28&&Light_Tower_Index==1)||(Light_Tower_Index==3))&&Stop==0&&Starting==0)  //灯塔停车
     { 
       light_tower_delay++;
        if(light_tower_on&&light_tower_delay==50) //50ms 扫描
        {
          light_tower_delay=0;
          pulse_cnt=lptmr_pulse_get();
          if(pulse_cnt>30)            //说明遇到强信号了
          {
            light_tower_cnt++;
          }
          else
          {
            if(light_tower_cnt>=2)
            {
                ForceStop=true;//防止干扰的产生
            }
            light_tower_cnt=0;
          }
          Light_Tower_OFF;
          light_tower_on=0;
          lptmr_pulse_clean();
        }
        if(light_tower_on==0&&light_tower_delay==10)  //关断灯塔10ms后再打开
        {
          light_tower_delay=0;
          Light_Tower_ON;
          light_tower_on=1;
          lptmr_pulse_clean();
        }
      }  
     /////////////////////////////////////////////统计时间及速度
     if(Stop==false&&Starting==false)
     {
      RunTime=RunTime+0.001;
      AverageSpeed=Distance/RunTime;//求得运行时间
     }
  } 
   PIT_Flag_Clear(PIT0);       //清中断标志位
}