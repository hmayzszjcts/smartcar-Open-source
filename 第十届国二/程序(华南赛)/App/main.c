/**************************************************
 * @file       main.c
 * @brief      山外K60 平台主程序
 * @author     山外科技
 * @version    v5.2
 * @date       2015-05-28
 * author      
 * 备注        FX硬件浮点已开
               超频210M  帧率112  较稳定（√） BUS 105M   阈值 0X45
               标频150M  帧率75   较稳定
//舵机中值：    845
//左打死  :     953
//右打死：      736
 **************************************************/
#include "common.h"
#include "include.h" 
#include "init.h"
#include "tx_process.h"
 
extern uint16  val_z, val_y; 
uint8 _1MS_flag=0,heixian_flag_0=1;   //1MS时间标志
int8   tingche_flag=0;
/**************************************************/
void PORTA_IRQHandler();
void DMA0_IRQHandler();
void sendimg(uint8 *imgaddr, uint32 imgsize);
void img_extract(uint8 *dst, uint8 *src, uint32 srclen);
void PIT0_IRQHandler();
void PIT1_IRQHandler();
/************************************************************
 *    @brief      main函数
      v5.2
 *    @note      
***********************************************************/
uint8 imgbuff[CAMERA_SIZE];    //定义存储接收图像的数组
uint8 img[CAMERA_H][CAMERA_W];
extern int32 angle_pwm;
void  main(void)
{         
    //设置优先级分组,4bit 抢占优先级,没有亚优先级 
    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(PORTA_IRQn,0);         //配置优先级
   // NVIC_SetPriority(UART3_RX_TX_IRQn,0); //亚优先级  在线调参数和配合Freecale上位机
    NVIC_SetPriority(PIT0_IRQn,1);          //配置优先级
    NVIC_SetPriority(DMA0_IRQn,2);          //配置优先级
    //NVIC_SetPriority(PIT1_IRQn,3);          //配置优先级

    FTM_init();
    DangWei_();
    //液晶初始化
    Site_t site     = {0, 0}  ;                           //显示图像左上角位置
    Size_t imgsize  = {CAMERA_W, CAMERA_H} ;             //图像大小
    Size_t size;  
    //显示区域图像大小         
     LCD_init();   
     size.H = CAMERA_H;
     size.W = CAMERA_W;
    //液晶初始化结束
    camera_init(imgbuff);                                   //配置中断复位函数
    
    //set_vector_handler(UART3_RX_TX_VECTORn , uart3_test_handler);
    //uart_rx_irq_en(UART3); 
    
    set_vector_handler(PORTA_VECTORn , PORTA_IRQHandler);   //设置LPTMR的中断复位函数为 PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn  , DMA0_IRQHandler);     //设置LPTMR的中断复位函数为 PORTA_IRQHandler
    pit_init_ms(PIT0,4);                                               //pit 定时中断(用于按键定时扫描)
    set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);                  //设置 PIT0 的中断服务函数为 
    enable_irq(PIT0_IRQn);
    
    /*********************** 按键消息 初始化  ***********************/
   //  key_event_init();                                                   //按键消息初始化
   //  pit_init_ms(PIT1,10);                                               //pit 定时中断(用于按键定时扫描)
   //  set_vector_handler(PIT1_VECTORn ,PIT1_IRQHandler);                  //设置 PIT0 的中断复位函数为 PIT0_IRQHandler
   //  enable_irq(PIT1_IRQn);
    /*********************** 按键消息 初始化  ***********************/
    while(1)
    {
            
         // PTD15_OUT=0;                         	//示波器测周期用
            camera_get_img();                                       //9MS  摄像头获取图像
          
            if(PTA12_IN==1&&PTA13_IN==1&&PTA14_IN==1&&PTA15_IN==1&&PTA16_IN==1&&PTA17_IN==1)
            {
              if( CAMERA_COLOR == 0)                                
              LCD_Img_Binary_Z(site, size, imgbuff, imgsize);  
            }                                                        //显示图像
            img_extract((uint8 *)img,(uint8 *)imgbuff, CAMERA_SIZE);  //寻两边黑线，再中线获取       
            PROCESS();
         // send_to_scope();      //车在运行时不要用蓝牙接收数据
         // deal_key_event();     //山外小液晶调参数最后不用，用拨码开关稳定。
         // PTD15_OUT=1;                                //测周期用
    }
}    
//舵机中值：    845
//左打死  :     953
//右打死：      736

void PIT0_IRQHandler()   //4MS进入一次中断    20MS控制舵机打角一次   获取一次速度   控制电机一次
{   
            JianChe();              
            if(PTD0_IN&&PTD1_IN)
            {
                heixian_flag_0=0; 
            }
            else
                heixian_flag_0=1;
            if(PTD6_IN)
            {
                tingche_flag=1;
            }
            
            _1MS_flag=_1MS_flag+1;
            if(_1MS_flag==5)   
            {
               if( angle_pwm>DJ_zuo )  angle_pwm=DJ_zuo ;      //扩大范围0—330
               if( angle_pwm<DJ_you )  angle_pwm=DJ_you ;   
               ftm_pwm_duty(S3010_FTM,S3010_CH,angle_pwm);      // 舵机打角
         
               get_speed();
               Speed_Contorl();
               _1MS_flag=0;              
             }
             PIT_Flag_Clear(PIT0);
}


void PIT1_IRQHandler()  //10MS进入一次中断    捕获按键状态  触发按键中断
{
    key_IRQHandler();

    PIT_Flag_Clear(PIT1);
}


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
/*
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
void sendimg(uint8 *imgaddr, uint32 imgsize)
{
    uint8 cmd[4] = {0, 255, 1, 0 };    //yy_摄像头串口调试 使用的命令

    uart_putbuff(VCAN_PORT, cmd, sizeof(cmd));    //先发送命令

    uart_putbuff(VCAN_PORT, imgaddr, imgsize); //再发送图像
}

//压缩二值化图像解压（空间 换 时间 解压）
//srclen 是二值化图像的占用空间大小
void img_extract(uint8 *dst, uint8 *src, uint32 srclen)      //解压为二维数组
{
    uint8 colour[2] = {1,0}; //0 和 1 分别对应的颜色
    //注：山外的摄像头 0 表示 白色，1 表示黑色
    uint8 tmpsrc;
    while(srclen --)
    {
        tmpsrc = *src++;
        *dst++ = colour[ (tmpsrc >> 7 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 6 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 5 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 4 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 3 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 2 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 1 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 0 ) & 0x01 ];
    }
}