/*
 * @file       main.c
 * @brief      主函数
 */

#include "common.h"
#include "include.h"
#include "function.h"

/*************
本程序将演示板名优科创摄像头平衡车的运用
引脚说明：

摄像头：
 SCCB_SCL        PTC17
 SCCB_SDA        PTC16
 PCLK            PTC12
 场中断          PTC13
 数据口          PTD0- PTD7

oled 引脚 在oled12864.h中定义：
#define RST    PTA16
#define DC     PTB2
#define D0     PTB9
#define D1     PTB3

串口：  在PORT_cfg.h  中定义
波特率 115200
#define UART0_RX    PTA15       
#define UART0_TX    PTA14  

6.1调试
**************/

/*!
 *  @brief      发送图像到上位机显示  
 */
void sendimg(uint8 *imgaddr, uint32 imgsize)
{
  my_putchar(0x55);
  my_putchar(0xaa);
  my_putchar(0xac);
  my_putchar(8);
  //一共800个数据位
  uart_putbuff(UART_PORT, imgaddr, imgsize); //发送图像
  uart_putbuff(UART_PORT, (uint8*)(&LMR[0][0]),     180); //发送边线及中线
  uart_putbuff(UART_PORT, (uint8*)(&LMR[0][0]),     20); //预留20个数据位
  my_putchar(0x0c);
}



void  main(void)
{ 
  init();
 // OLED_Draw_camera();
  Para_Init();
 
   while(1)
    {
//       if(beep)            //滴滴想      //13512254304  6289y3
//      {
//        BEEP_ON;
//        DELAY_MS(50);
//        BEEP_OFF;
//        beep=0;
//      }  
//       Read_Switch();
//
//      if(SendPara)   //改参数 串口..
//      {
//        SendPara=0;
//        Send_Parameter();
//      }
//      
//      if(SendSD)
//     {
//       Send_Begin();
//       Send_SD();
//       SendSD=false;
//      }
////      
//       if((SD_Save||SD_Write_info)&&SD_OK)
//      { 
//        Variable_update();
//        if(SD_Save) Record();
//        else
//        {
//          Write_Information();
//          SD_Write_info=0;
//        }
//        
//      }
      
      
      Check_BottonPress();//检测按键
      
//      if(Uart_Send) //拨码开关2往上拨 发送数据 1
//      {
//       Send_Begin();
//       Variable_update();
//       Send_Variable();
//       sendimg(imgbuff,CAMERA_SIZE);
//       //printf("\n我勒个擦");
//      }
      if(OLED_Refresh)
      {
        OLED_Draw_UI();
      }      
     } 
}
void PIT_IRQHandler()  //2ms一次中断
{
  static uint8 flag_100ms,cnt=0;
  PIT_Flag_Clear(PIT0);       //清中断标志位 
  flag_100ms++;
  if(flag_100ms>50)
  {
   flag_100ms=0;  
   Speed_Control();  //100ms进行一次速度控制
  
   LED_RED_TURN;
   SpeedCount=0;
  }
   if(imgprocessed==0)  //此段不超过0.5ms 200ms主频  6ms 或8ms 执行一次
   {
      img_extract(img,imgbuff,CAMERA_SIZE);
      Search();
      Direction_Control();
      imgprocessed=1;
   }
   cnt++;
  
   if(cnt==1)      //4ms运行一次
   {
     Get_Attitude(); 
     Angle_Calculate();
     Angle_Control(); 
     Get_Speed();
   }
   if(cnt>=2)
   {
     cnt=0;
   }
   SpeedCount++;
   Speed_Control_Output();
   Moto_Out();
}






//
//void  main(void)
//{ 
//// init();
//OLED_Init();
////   FTM_PWM_init(FTM0,FTM_CH0,200,10);   
////   FTM_PWM_init(FTM0,FTM_CH1,10*1000,0);  
////   FTM_PWM_init(FTM0,FTM_CH2,200,10);  
////  FTM_PWM_init(FTM0,FTM_CH3,10*1000,0); 
// //while(1){
//    OLED_Draw_Logo();
//    DELAY_MS(5000);
//    OLED_Draw_Logo1();
//      DELAY_MS(5000);
//// FTM_PWM_Duty(FTM0,FTM_CH2,100); //占空比精度为10000  20 31
// // FTM_PWM_Duty(FTM0,FTM_CH0,0);
//     
// // FTM_PWM_Duty(FTM0,FTM_CH3,5000); 
////   FTM_PWM_Duty(FTM0,FTM_CH1,0);
// //}
//}
