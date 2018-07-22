#include "common.h"
#include "include.h"
//全局变量定义
uint8 flag_100ms,SaveData=0;
uint16 var;
void PIT_IRQHandler(void);
void main()
{
  DisableInterrupts;  //关闭中断 
  All_Init();
  set_vector_handler(PIT0_VECTORn ,PIT_IRQHandler);
  //set_vector_handler(UART0_VECTORn ,UART0_RX_IRQHandler); 
  enable_irq (PIT0_IRQn);    
 // NVIC_SetPriority(UART0_IRQn,1);
  NVIC_SetPriority(PIT0_IRQn,0);

  
  
  EnableInterrupts; //打开中断
  while(1)
  {
    
      uart_putchar(UART4,0x55);
  uart_putchar(UART4,0xaa);
  uart_putchar(UART4,0xae);
      Send_Variable();
      Send_Inductor_Adc();
    if(flag_100ms)        
    {
       Read_Switch();
     
       if(OLED_Refresh)
       { 
        Check_BottonPress();  
        if(Stop==true||SaveData==false)  
        OLED_Draw_UI();
       }
       else Check_StartPress();    
       flag_100ms=0;
    }
  }
}

void PIT_IRQHandler(void)
{


  if(PIT_TFLG(PIT0) == 1)
  {
     static uint8  part=0;
     static uint8  cnt100ms;
     
     part++;
     cnt100ms++;
     SpeedCount++;   
     DirectionCount++;
     switch(part) 
     {
        case 1://直立控制
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
        case 2:
               EM_ADC();
               if((RunTime<0.1)&&(Stop==0))  //防止起步的时候乱飘
              {
                Middle_Err=0;
              }
               RoadType_Distinguish();
               break;
               
        case 3:
               read_buff[0]= Get_Y_Gyro();
               break; 
        case 4:
                Direction_Control(); 
                DirectionCount=0;
                break; 
        case 5:
                part=0;
                read_buff[1]= Get_Y_Gyro();
                break; 
     default:
                break;
     }
     Direction_Control_Output();
     Moto_Out();

     

  
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

     /////////////////////////////////////////////统计时间及速度
     if(Stop==false&&Starting==false)
     {
      RunTime=RunTime+0.001;
      AverageSpeed=Distance/RunTime;//求得运行时间
     }
  }
     PIT_Flag_Clear(PIT0);       //清中断标志位
}