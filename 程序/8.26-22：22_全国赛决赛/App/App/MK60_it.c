/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_it.c
 * @brief      山外K60 平台中断服务函数
 * @author     山外科技
 * @version    v5.0
 * @date       2013-06-26
 */

#include    "MK60_it.h"
#include    "common.h"
#include    "include.h"

/*********************************中断服务函数********************************/

/*!
 *  @brief      UART4中断服务函数
 *  @since      v5.0
 */
void uart4_handler(void)
{
    char str[2];

    if(uart_querychar (UART4, str) != 0)   //接收数据寄存器满
    {
        //用户需要处理接收数据
        /*if (str[0] == 1)
		{
			Run_Flag = 1;
		}
		else if (str[0] == 2)
		{
			Run_Flag = 0;
		}*/
    }
}
/*!
 *  @brief      PORTA中断服务函数
 *  @since      v5.0
 */
void PORTA_IRQHandler()
{
    uint8  n;    //引脚号
    uint32 flag;

    //while(!PORTA_ISFR);
    flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                                   //清中断标志位

    n = 29;                                             //场中断
    if(flag & (1 << n))                                 //PTA29触发中断
    {
        camera_vsync();
    }
#if ( CAMERA_USE_HREF == 1 )                            //使用行中断
    n = 28;
    if(flag & (1 << n))                                 //PTA28触发中断
    {
        camera_href();
    }
#endif

}

void PORTE_IRQHandler()
{
    uint8  n;    //引脚号
    uint32 flag;

    flag = PORTE_ISFR;
    PORTE_ISFR  = ~0;                                   //清中断标志位

    n = 27;
    if(flag & (1 << n))                                 //PTE27触发中断
    {
        nrf_handler();
    }
}

/*!
 *  @brief      DMA0中断服务函数
 *  @since      v5.0
 */
void DMA0_IRQHandler()
{
//    led(LED_MAX,LED_ON);            //用于逻辑分析

    camera_dma();
    camera_get_img();	//等待图像传输完毕（完成标志变为开始标志）
    flag_imgbuff =! flag_imgbuff;
    ov7725_eagle_img_buff = (flag_imgbuff == 0?p_imgbuff1:p_imgbuff2);//交替初始化图像地址

    flag_imgbuff != 0?img_extract(img, p_imgbuff1,CAMERA_SIZE):img_extract(img, p_imgbuff2,CAMERA_SIZE); 
    image_processing(img);            //图像算法分析313u   
    Point = Dynamic_Point_Weight(Point_Num);		//正值 图像向下平移 提前打角	必须为偶数否则出错
    S3010_Control();                  //舵机控制
//    speed_measure();	              //测速
//    MOTOR_Control();                //电机控制
//    led(LED_MAX,LED_OFF);            //用于逻辑分析
}

/*!
*  @brief      PIT0中断服务函数
*  @since      v5.0
*/
extern uchar Annulus_Flag;
extern uint16 Annulus_DelayMs;
extern int16 Starting_Time;
extern int8 Starting2_Time;
extern uint8 Ramp_Flag;
extern uint16 Ramp_Time;
extern uint8 Hazard_Time;
extern uchar Left_Hazard_Flag;
extern uchar Right_Hazard_Flag;

void PIT0_IRQHandler()
{	
    static uchar Time_150ms = 0;
/******** 起跑延时 *********/      
    Time_150ms++;
    if (Starting_Time>0)
    {
          Starting_Time--;
    }
    if (Starting2_Time>0)
    {
          Starting2_Time--;
    }
/******** 坡道延时 *********/  
    if (Ramp_Flag || Ramp_Time)
    {
        Ramp_Time++;
    }
    
 /******** 障碍延时 *********/  
//     if (Left_Hazard_Flag || Right_Hazard_Flag)
//     {
//        Hazard_Time++;
//        if(Hazard_Time>200)
//        {
//            Left_Hazard_Flag=0;
//            Right_Hazard_Flag=0;
//        }
//     }   
/******** 图像延时 *********/ 
    if (Time_150ms == 15)
    {
      Time_150ms = 0;
      Send_OK = 1;	// 图像150ms发送一次
    }
/******** 环路延时 *********/ 
//    if (Annulus_Flag)
//    {
//       Annulus_DelayMs++;
//    }
    if (Annulus2_Delay)
   {
        Annulus2_Delay--;
    }
/******** 起跑检测 *********/ 
    Starting_Check(); 
/******** 赛道检测 *********/    
    Track_Line();
/******** 速度控制 *********/ 
    Adjust_Speed(); 
/******** 变速控制 *********/ 
    Control_Speed();
/******** 差速控制 *********/ 
    CS_Control();
/******** 速度检测 *********/ 
    speed_measure();	//测速及停车	测试期间暂时不用
/******** 电机控制 *********/ 
    MOTOR_Control();
/******** 波形显示 *********/ 
//    Scope_Display();
    PIT_Flag_Clear(PIT0);       //清中断标志位
}


/*!
*  @brief      PIT1中断服务函数
*  @since      v5.0
*/

//void PIT1_IRQHandler()
//{
//    PIT_Flag_Clear(PIT1);       //清中断标志位
//    if (Annulus_Flag==1)
//    {
//        led(LED_MAX,LED_ON);// 打开LED指示灯		全部初始化成功
////        Track_Line_Num=0;
//    }
//    else
//    {
//        led(LED_MAX,LED_OFF);// 打开LED指示灯		全部初始化成功
//    }
//}

/******************************中断服务函数结束***********8*******************/


