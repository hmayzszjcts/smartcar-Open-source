#include "usart_file.h"
#include "control.h"
#include "img_array.h"
#include "path.h"
#include "usart_file.h"
#include  "info_deal_save.h"
#include "my_UI.h"
#include "direction.h"
#include "speed_new.h"

uint8 imgbuff[CAMERA_SIZE];                             //定义存储接收图像的数组
uint32 total_time=0;
//uint8 img[CAMERA_H][CAMERA_W];

//函数声明 z120 
void sendimg(void *imgaddr, uint32 imgsize);
void PORTA_IRQHandler();
void DMA0_IRQHandler();
uint8 my_debug_flag=0;
void my_delay(uint32_t z)
{
    while(z--);
}
void draw_mark_line()
{
    uint16 x=0,y=0;
    Site_t site;
    for(y=0;y<IMG_H;y+=10)
    {
        for(x=0;x<IMG_W;x++)
        {
            site.x=x;
            site.y=y;
            LCD_point(site,RED);
        }
    }
    for(y=0;y<IMG_H;y++)
    {
        for(x=0;x<IMG_W;x+=10)
        {
            site.x=x;
            site.y=y;
            LCD_point(site,RED);
        }
    }
}

//void motor_dir_init()
//{/************************************************************************
//  配置电机方向
//  */////////////////////////////////////////
//  gpio_init(PTD6,GPI,0);//初始低电平
//  port_init_NoALT(PTD6,PULLUP);
//  
////  gpio_set (PTD6, 1);//设置引脚高电平
//
//}
void init_fun()
{
    key_init(KEY_MAX);//按键初始化  
    duoji_init();//舵机pwm初始化
    LCD_init();  //彩屏初始化    
    //init_Par_from_FLASH();//内部flash初始化，用于存放蓝牙串口调参的参数
    usart_init();//串口初始化
    camera_init(imgbuff);//相机初始化
   //配置中断服务函数
    set_vector_handler(PORTA_VECTORn , PORTA_IRQHandler);   //设置 PORTA 的中断服务函数为 PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);     //设置 DMA0 的中断服务函数为 PORTA_IRQHandler
}

void check_start_key()
{
    if(key_get(KEY_U)==0||key_get(KEY_R)==0||key_get(KEY_START)==0||key_get(KEY_A)==0)
    {
        DELAY_MS(1000);
        run_start_flag=~run_start_flag;
        total_time=0;
    }
    else if(key_get(KEY_D)==0||key_get(KEY_L)==0)
    {
        DELAY_MS(500);
        if(key_get(KEY_D)==0||key_get(KEY_L)==0)
        {
            if(my_debug_flag) 
              my_debug_flag=0;
            else my_debug_flag=1;
        }
    }
}
/*!
 *  @brief      main函数
 *  @since      v5.0
 *  @note       山外摄像头 LCD ---------------------测试实验
 */

/*
第十三届  光电摄像头
*/
    



void  main(void) 
{

 //！！！！！****一定要先读my_cfg.h中的注释**·***！！！！！！
    run_start_flag=0;
    init_fun();//各种初始化  (按键 舵机 彩屏 flash 摄像头 中断配置) 
    init_ctrl();//初始化速度控制（中断 电机）
    renew_UI();//界面初始化，并通过按键调参 ，直到调完才跳出   
    MOTOR_init();//初始化电机
    PID_init(); 
    //DELAY_MS(2000);//延时两秒发车
    run_start_flag=1;//开始跑的标志位
    total_time=40;//记录运行次数，起跑线检测时用到，用于跑一段时间在检测起跑线 
    
    // change_angle(6600);//舵机中立位置（用于测试）
    while(1)
    {      
        //cmd_deal();        //用于处理串口发来的数据 ，打开蓝牙调参         
      if(img_deal_flag)
      {
          img_deal_flag=0;        
          camera_get_img();  //摄像头获取图像 
          ctrl_main() ;       //计算图像，及方向控制的入口函数   
          show_run_info();
          total_time++;      //记录控制的次数
      }

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

    while(!PORTA_ISFR);
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

/*!
 *  @brief      DMA0中断服务函数
 *  @since      v5.0
 */
void DMA0_IRQHandler()
{
    camera_dma();
}


//发送图像到上位机显示
//不同的上位机，不同的命令，这里使用 yy_摄像头串口调试 软件
//如果使用其他上位机，则需要修改代码
void sendimg(void *imgaddr, uint32 imgsize)
{
    uint8 cmd[4] = {0, 255, 1, 0 };    //yy_摄像头串口调试 使用的命令

    uart_putbuff(VCAN_PORT, (uint8_t *)cmd, sizeof(cmd));    //先发送命令

    uart_putbuff(VCAN_PORT, imgaddr, imgsize); //再发送图像
}
