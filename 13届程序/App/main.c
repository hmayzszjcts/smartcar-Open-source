/*******************************************************************************/
//
//                      oo——NXP2018_PRO——oo
//
//                          PART1:初始化区段
/*******************************************************************************/
/************************包含的头文件****************************/
#include "common.h"
#include "include.h"
#include "OLED.h"
#include "SEEKFREE_18TFT.h"
/***********************参数定义&设置****************************/
//---------------------------------------------------------------
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//---------------------------------------------------------------
/***********************系统运行参数*****************************/
uint8 code_Time_flag=0;                 //程序运行时间
/**************************舵机*********************************/
uint8  KP_A=6,KP_B=27,KD=150;            //MAIN舵机PID
uint32 DJ_midpoint=7330;                //舵机中值
uint32 DJ_PWM;                          //输出PWM
/************************速度控制*******************************/
uint8  speed_need=20;                   //目标速度
uint8  speed_need_Boost=30;             //目标高速
uint8  speed_need_normal=30;            //目标速度
uint8  speed_need_L=30;                 //目标弯道
uint8  speed_SW_flag=1;                 //速度选择标志
/*************************电机控制******************************/
float Speed_P=4,Speed_I=0.15,Speed_D=1; //MAIN电机PID
uint8 Block_motor_time_flag=0;          //堵转计时标志
uint8 Block_motor_duty_flag=0;          //堵转事件标志
uint8 Block_motor_delay_flag=0;         //堵转弛懈标志
/**************************编码器********************************/
float feed_fix=10.6;           //编码器修正系数
uint32 Feed_flag=0;             //编码器采集计数
uint32 Feed_speed=0;            //编码器采集速度
/***********************摄像头有关参数***************************/
/*调控参量*/
uint8 img_y_max=50;             //扫描纵坐标最近值
uint8 img_y_min=10;             //扫描纵坐标最远值
uint8 img_y_control=30;         //扫描纵坐标控制值
/*传递参量*/
uint8 imgbuff[CAMERA_SIZE];     //定义存储接收图像的数组
uint8 img[CAMERA_W*CAMERA_H];   //摄像头解压数组
uint8 img_x=40;                 //扫描横坐标
uint8 img_y=30;                 //扫描纵坐标
uint8 start_point=40;           //扫描起始点
uint8 mid_point[60];            //提取的中线  
uint8 mid_point_His[10];        //历史的中线 
uint8 left_point[60];           //左边界
int right_point[60];          //右边界
uint8 init_len[60];             //初始化赛道宽度
uint8 point_len[60];            //实时赛道宽度
uint8 street_len=0;             //直道长度
uint8 len_His[10];              //直道长度历史数组
/*圆环补线*/
float L_Cur_K=0;                //左圆环补线斜率
float R_Cur_K=0;                //由圆环补线斜率
/******************识别与判断标志***************************/
/*丢线标志*/
uint8 all_lost=0;               //全丢标志
uint8 lost_left[60];            //左丢线标志
uint8 lost_right[60];           //右丢线标志
/*起停*/
uint8 KEY_start_flag=0;         //一键启动标志
uint16 KEY_start_time_flag=0;   //启动时间标志
/*十字*/
uint8 lost_flag=0;              //全丢线初始识别标志
uint8 lost_delay_flag=0;        //十字弛懈标志
uint16 lost_car_time_flag=0;    //十字计时变量
uint8 lost_duty_flag=0;         //全丢线准确识别标志(十字)
/*圆环预判断*/
uint8 cur_L_ready_flag=0;         //左圆环预判断初始识别标志
uint8 cur_L_ready_delay_flag=0;   //左圆环预判断弛懈标志
uint16 cur_L_ready_time_flag=0;   //左圆环预判断计时变量
uint8 cur_L_ready_rest_flag=0;    //左圆环预判断复位变量
uint8 cur_R_ready_flag=0;         //右圆环预判断初始识别标志
uint8 cur_R_ready_delay_flag=0;   //右圆环预判断弛懈标志
uint16 cur_R_ready_time_flag=0;   //右圆环预判断计时变量
uint8 cur_R_ready_rest_flag=0;    //右圆环预判断复位变量
/*圆环准确识别*/
uint8 cur_L_real_flag=0;          //左圆环准确判断识别标志
uint8 cur_L_real_delay_flag=0;    //左圆环准确弛懈识别标志
uint8 cur_L_real_rest_flag=0;     //左圆环准确复位识别标志
uint16 cur_L_real_time_flag=0;    //左圆环准确弛懈识别标志
uint16 cur_L_real_time_flag1=0;   //左圆环准确弛懈识别标志1

uint8 cur_R_real_flag=0;          //右圆环准确判断识别标志
uint8 cur_R_real_delay_flag=0;    //右圆环准确弛懈识别标志
uint8 cur_R_real_rest_flag=0;     //右圆环准确复位识别标志
uint16 cur_R_real_time_flag=0;    //右圆环准确弛懈识别标志
uint16 cur_R_real_time_flag1=0;   //右圆环准确弛懈识别标志1
/***************************END**********************************/
//---------------------------------------------------------------
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//---------------------------------------------------------------
/*************************函数声明*******************************/
/*初始化部分*/
void IO_init(void);                     //车辆端口初始化
/*调试界面部分*/
void speed_SW(void);                    //速度档位选择
void Board_led_duty(void);              //板载LED闪烁函数
void BEEP_duty(void);                   //蜂鸣器控制函数        
uint8 Button_Duty(uint8 Button_val);     //按键设置函数
void GUI_Duty(void);                    //调试界面函数
/*图像处理部分*/
void img_ext(uint8 *dst,uint8 *src,uint32 srclen);//数组解压函数
void len_init(void);                    //赛道宽度初始化
void street_duty(void);                 //赛道直道长度测量
uint8 PointL_DUTY(uint8 L_Start,uint8 L_y);//边线扫描函数
uint8 PointR_DUTY(uint8 R_Start,uint8 R_y);//边线扫描函数
void mid_Point_DUTY(uint8 Mid_len,uint8 Mid_y);//补线算法
void IMG_DUTY(void);                    //主图像处理函数
/*事件管理部分*/
void EVENT_Duty(void);                  //事件管理函数
/*车辆控制部分*/
void DJ_PID(unsigned char midpoint_in);//舵机PD
void FEED_COUNT(void);                  //编码器计数结算
void speed_PID(unsigned char speed_in);//转速PID控制函数
void CAR_Drive_duty(void);              //执行机构综合控制函数
/*中断函数部分*/
void PORTC_IRQHandler();                //PORTC中断服务函数
void DMA0_IRQHandler();                 //DMA中断服务函数
void PIT0_IRQHandler();                 //PIT0中断服务函数
void PORTD_IRQHandler();                //PORTD中断服务函数
/**********************车辆端口初始化****************************/
void IO_init(void)
{
  /*****蜂鸣器输出端口******/
  gpio_init (PTE28 ,GPO,0);
  /******板载LED端口*******/
  gpio_init (PTA17,GPO,1);
  gpio_init (PTC0, GPO,1);
  gpio_init (PTD15,GPO,1);
  gpio_init (PTE26,GPO,1);
  /*****播码开关接口*******/
  gpio_init (PTE4, GPI,0);
  gpio_init (PTE6, GPI,0);
  gpio_init (PTE8, GPI,0);
  gpio_init (PTE10,GPI,0);
  /*******五轴按键接口*****/
  gpio_init (PTE7,GPI,0);  //左     
  gpio_init (PTE5,GPI,0);  //下     
  gpio_init (PTE12,GPI,0); //右
  gpio_init (PTE11,GPI,0); //中
  gpio_init (PTE9,GPI,0);  //上
  /*******编码器接口*******/
  gpio_init (PTD0,GPI,0);
  gpio_init (PTD1,GPI,0);
  port_init (PTD0,ALT1|IRQ_EITHER|PULLUP );
  /******电机PWM接口*******/
  ftm_pwm_init(FTM0, FTM_CH0,1000,0); 
  ftm_pwm_init(FTM0, FTM_CH1,1000,0); 
  /******舵机PWM接口*******/
  ftm_pwm_init(FTM2, FTM_CH0,50,7200);
}
/*******************************************************************************/
//                      上一部分：初始化区段
//
//                      下一部分：调试界面部分
/*******************************************************************************/
/********************速度档位选择************************/
void speed_SW(void)
{
  if(speed_SW_flag==1)           //速度档位1      
  {
    speed_need_Boost=30;         //目标高速 30
    speed_need_normal=25;        //目标速度 25
    speed_need_L=26;             //目标弯道
    KP_A=10,KP_B=27,KD=150;  //6
  }
  else if(speed_SW_flag==2)     //速度档位2      
  {
    speed_need_Boost=32;         //目标高速
    speed_need_normal=26;        //目标速度
    speed_need_L=27;             //目标弯道
    KP_A=20,KP_B=27,KD=180;
  }
  else if(speed_SW_flag==3)     //速度档位3      
  {
    speed_need_Boost=33;         //目标高速
    speed_need_normal=27;        //目标速度
    speed_need_L=28;             //目标弯道
    KP_A=20,KP_B=27,KD=180;
  }
  else if(speed_SW_flag==4)     //速度档位4      
  {
    speed_need_Boost=34;         //目标高速
    speed_need_normal=27;        //目标速度
    speed_need_L=28;             //目标弯道
    KP_A=25,KP_B=27,KD=180;
  }
  else;
}
/********************板载LED闪烁函数************************/
void Board_led_duty(void)
{
  gpio_turn (PTD15);
  gpio_turn (PTE26);
  gpio_turn (PTA17);
  gpio_turn (PTC0);
}
/********************蜂鸣器控制函数************************/
void BEEP_duty(void)
{
  //if(!gpio_get(PTE11)||lost_flag)     //工作条件
  if(!gpio_get(PTE11)||cur_L_real_delay_flag||cur_R_real_delay_flag)    
    gpio_set (PTE28,0);        
  else gpio_set (PTE28,1);
}
/********************按键设置函数****************************/
uint8 Button_Duty(uint8 Button_val)     //按键计数函数
{
  if(!gpio_get(PTE9))                   //按键加                   
  {
    DELAY_MS(10);
    if(!gpio_get(PTE9))                 //二次判断
    {
      Button_val++;
      if(Button_val>250)                //限幅
        Button_val=250;
      while(!gpio_get(PTE9));
    }
  }
  if(!gpio_get(PTE5))                   //按键减
  {
    DELAY_MS(10);
    if(!gpio_get(PTE5))                 //二次判断
    {
      Button_val--;
      if(Button_val<2)                  //限幅
        Button_val=1;
      while(!gpio_get(PTE5));
    }
  }
   return Button_val;
}
/*******************调试界面函数**************************/
void GUI_Duty(void)   
{
  /*********系统设置***********/
  unsigned char static GUI_flag=0;     //调试界面变量
  unsigned char static display_y=25;   //调试行数变量
  if(!gpio_get(PTE12))                  //改变界面
  {
    DELAY_MS(10);
    if(!gpio_get(PTE12))                //二次确认
    {
      LCD_Fill(0x00);                   //清屏
      GUI_flag++;
      if(GUI_flag>2)                    //设置界面页数
        GUI_flag=0;
      while(!gpio_get(PTE12));
    }
  }
  /*********板载LED指示********/
  Board_led_duty();
  /*********蜂鸣器控制********/
  BEEP_duty();
  /*********调试界面#0***********/
  if(GUI_flag==0)               
  {
    LED_PrintImage(img,60,80);                  //OLED显示图像
    display_y=Button_Duty(display_y);           //按键设置行数
    Display_uint8(display_y,85,0);              //OLED显示行数
    Display_uint8(mid_point[display_y],85,2);   //OLED显示中线
    Display_uint8(street_len,85,4);             //赛道直道长度测量
    //Display_uint8(code_Time_flag,85,6);       //显示程序运行时间
    Display_uint8(point_len[display_y],85,6);   //OLED显示赛道宽度
  }
  /*********调试界面#1***********/
  else if(GUI_flag==1)
  {
    LCD_P8x16Str(1,0,"DJ_PWM");                 //OLED显示舵机PWM
    Display(DJ_PWM,1,2);
    LCD_P8x16Str(1,4,"Speed_SET");              //OLED显示设定速度
    /***速度档位选择***/
    Display(speed_SW_flag,1,6);                 //显示速度选择标志
    speed_SW_flag=Button_Duty(speed_SW_flag);   //速度档位选择
    if(speed_SW_flag>4)speed_SW_flag=1;
    speed_SW();                                 //速度档位选择函数
  }
  else if(GUI_flag==2)
  {
    LCD_P8x16Str(10,2,"START ENGINE!");         //一键启动
    if(!gpio_get(PTE11)) 
    {KEY_start_flag=1;Block_motor_delay_flag=0;}//启动标志值置1+电机堵转保护复位
    else;
    if(Block_motor_duty_flag)
      LCD_P8x16Str(10,5,"Block_motor!");       //电机堵转
    else if(!KEY_start_flag)
      LCD_P8x16Str(10,5,"    Loop    ");       //停车
    else if(KEY_start_flag && KEY_start_time_flag<100)
      LCD_P8x16Str(10,5,"    READY   ");       //准备
    //else if(KEY_start_time_flag>100 && !stop_motors_flag)
    else if(KEY_start_time_flag>100)
      LCD_P8x16Str(10,5,"    Voom!    ");      //启动！
    else
      LCD_P8x16Str(10,5,"    STOP    ");       //停车
  }
  else;
  /************END**************/ 
}
/*******************************************************************************/
//                      上一部分：调试界面部分
//
//                      下一部分：图像处理部分
/*******************************************************************************/
/********************摄像头数组解压*************************/
void img_ext(uint8 *dst, uint8 *src, uint32 srclen)
{
    uint8 colour[2] = {255, 0}; //0 和 1 分别对应的颜色
    //注：山外的摄像头 0 表示 白色，1表示 黑色
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
/*********************赛道宽度初始化************************/
void len_init(void)                   //赛道宽度初始化
{
  for(img_y=img_y_min;img_y<=img_y_max;img_y++)
  {
    init_len[img_y]=img_y+30;
  }
}
/********************赛道直道长度测量**********************/
void street_duty(void)                //赛道直道长度测量
{
  /*中线直道长度测量*/
  for(img_y=55;img_y>=1;img_y--)
  {
    if(img[40+img_y*80]==0)
      break;
    else;
  }
  street_len=59-img_y;
  /*右圆环切入点测量*/
  for(img_y=55;img_y>=1;img_y--)
  {
    if(img[right_point[img_y_max-3]-3+img_y*80]==0)
      break;
    else;
  }
  R_Cur_K=10*(right_point[img_y_max-3]-35)/(59-img_y);
  /*左圆环切入点测量*/
  for(img_y=55;img_y>=1;img_y--)
  {
    if(img[left_point[img_y_max-3]+3+img_y*80]==0)
      break;
    else;
  }
  L_Cur_K=10*(50-left_point[img_y_max-3])/(59-img_y);
}
/*******************边界扫描函数***********************/
//输入1：扫描的起始点；输入二：扫描的Y
//输出 ：扫描的边界值
uint8 PointL_DUTY(uint8 L_Start,uint8 L_y)
{
  unsigned char static L_point;
  for(img_x=L_Start;img_x>1;img_x--) //左边界提取
  {
    if(img[img_x+L_y*80]==0)
    {
      L_point=img_x;
      break;
    }
    else L_point=1;
  }
  return L_point;
}
uint8 PointR_DUTY(uint8 R_Start,uint8 R_y)
{
  unsigned char static R_point;
  for(img_x=R_Start;img_x<79;img_x++)//右边界提取
  {
    if(img[img_x+R_y*80]==0)
    {
      R_point=img_x;
      break;
    }
    else R_point=79;
  }
  return R_point;
}
/********************边线处理函数*************************/
void mid_Point_DUTY(uint8 Mid_len,uint8 Mid_y)          //补线算法
{  
  /*********圆环边线处理***********/
  /*入环*/
  if(cur_R_real_delay_flag||cur_L_real_delay_flag)
  {
    if(cur_R_real_delay_flag&&cur_R_real_time_flag<20)
    {
      left_point[Mid_y]=(uint8)(R_Cur_K*(59-Mid_y)/10);
      if(left_point[Mid_y]+Mid_len/2<79)
        mid_point[Mid_y]=left_point[Mid_y]+Mid_len/2;
      else mid_point[Mid_y]=79;
    }
    else if(cur_L_real_delay_flag&&cur_L_real_time_flag<20)
    {
      right_point[Mid_y]=(uint8)(80-L_Cur_K*(59-Mid_y)/10);
      if(right_point[Mid_y]-Mid_len/2>1)
        mid_point[Mid_y]=right_point[Mid_y]-Mid_len/2;
      else mid_point[Mid_y]=1;
    }
     /*出环*/
    else if(cur_R_real_rest_flag&&cur_R_real_delay_flag)
    {
      if(mid_point_His[9]<79)
        mid_point[Mid_y]=mid_point_His[9];
      else mid_point[Mid_y]=mid_point_His[9];
    }
    else if(cur_L_real_rest_flag&&cur_L_real_delay_flag)
    {
      if(mid_point_His[9]>21)
        mid_point[Mid_y]=mid_point_His[9];
      else mid_point[Mid_y]=mid_point_His[9];
    }
    /*环中*/
    else
    {
      if(left_point[Mid_y]<5 && !lost_flag)           //左边丢线补线
      {
        if(right_point[Mid_y]>30)
          mid_point[Mid_y]=right_point[Mid_y]-Mid_len/2;
        else mid_point[Mid_y]=1;
      }
      else if(right_point[Mid_y]>75 && !lost_flag)   //右边丢线补线
      {
        if(left_point[Mid_y]<50)
          mid_point[Mid_y]=left_point[Mid_y]+Mid_len/2;
        else mid_point[Mid_y]=79;
      }
      else mid_point[Mid_y]=(left_point[Mid_y]+right_point[Mid_y])/2; //中线计算
    }
  }
 
  /*********十字边线处理***********/
  else if(lost_duty_flag&&!(cur_R_real_delay_flag||cur_L_real_delay_flag))
  {
       mid_point[Mid_y]=mid_point_His[6];
  }
  /*********正常边线处理***********/
  else
  {
    if(left_point[Mid_y]<5 && !lost_flag)           //左边丢线补线
    {
      if(right_point[Mid_y]>30)
        mid_point[Mid_y]=right_point[Mid_y]-Mid_len/2;
      else mid_point[Mid_y]=1;
    }
    else if(right_point[Mid_y]>75 && !lost_flag)   //右边丢线补线
    {
      if(left_point[Mid_y]<50)
        mid_point[Mid_y]=left_point[Mid_y]+Mid_len/2;
      else mid_point[Mid_y]=79;
    }
    else mid_point[Mid_y]=(left_point[Mid_y]+right_point[Mid_y])/2; //中线计算
  }
}
/*******************主图像处理函数***********************/
void IMG_DUTY(void)                                     //主图像处理函数
{
  street_duty();                                         //赛道直道长度测量
  /*****MAIN边线提取+识别程序******/
  lost_flag=0;                                           //丢线变量清零
  all_lost=0;                                            //全丢标志清零
  start_point=(left_point[img_y_max]+right_point[img_y_max])/2;//扫描初值设定
  for(img_y=img_y_max;img_y>=img_y_min;img_y--)          //15-40行提取
  {
    /*********边界提取***********/
    if(all_lost==0)
    {
      left_point[img_y]=PointL_DUTY(start_point,img_y);  //左边界提取
      right_point[img_y]=PointR_DUTY(start_point,img_y); //右边界提取
    }
    else if(all_lost==1)
    {
      left_point[img_y]=1;
      right_point[img_y]=1;
    }
    else if(all_lost==2)
    {
      left_point[img_y]=79;
      right_point[img_y]=79;
    }else;
    /*********变量计算***********/
    start_point=(left_point[img_y]+right_point[img_y])/2;//扫描初值设定
    /*初值限幅8.17*/
    if(start_point>70){start_point=79;all_lost=2;}
    else if(start_point<10){start_point=1;all_lost=1;}
    else;
    /************END*************/
    point_len[img_y]=right_point[img_y]-left_point[img_y];//计算赛道宽度
    if(left_point[img_y]<5)lost_left[img_y]=1;           //左丢线判断
    else lost_left[img_y]=0;
    if(right_point[img_y]>75)lost_right[img_y]=1;        //右丢线判断
    else lost_right[img_y]=0;
  }
  /**********圆环预判断************/
  if(lost_right[23]&&lost_right[25]&&!lost_left[23]&&!lost_left[25]&&street_len>47&&point_len[25]>55&&point_len[25]<75)
    cur_R_ready_flag=1;//右圆环预识别
  else cur_R_ready_flag=0;
  if(lost_left[23]&&lost_left[25]&&!lost_right[23]&&!lost_right[25]&&street_len>47&&point_len[25]>55&&point_len[25]<75)
    cur_L_ready_flag=1;//左圆环预识别
  else cur_L_ready_flag=0;
  /**********丢线判断************/
  if(right_point[25]>75&&left_point[25]<5&&right_point[30]>75&&left_point[30]<5)//十字识别
    {
      lost_flag=1;                                       //全部丢线
    }else;
  /********事件管理处理**********/
  EVENT_Duty();
  /*********20-40行补线处理********/
  for(img_y=img_y_max;img_y>=img_y_min;img_y--)          //15-40行处理
  {
    mid_Point_DUTY(init_len[img_y],img_y);               //中线计算
    img[mid_point[img_y]+img_y*80]=0;                    //打印中线点
    img[left_point[img_y]+img_y*80]=0;                   //打印左边线
    img[right_point[img_y]+img_y*80]=0;                  //打印右边线
  }
  /**********中线历史数组************/
  mid_point_His[9]=mid_point_His[8];
  mid_point_His[8]=mid_point_His[7];
  mid_point_His[7]=mid_point_His[6];
  mid_point_His[6]=mid_point_His[5];
  mid_point_His[5]=mid_point_His[4];
  mid_point_His[4]=mid_point_His[3];
  mid_point_His[3]=mid_point_His[2];
  mid_point_His[2]=mid_point_His[1];
  mid_point_His[1]=mid_point_His[0];
  mid_point_His[0]=mid_point[img_y_control];
  /**********直道长度历史数组************/
  len_His[9]=len_His[8];
  len_His[8]=len_His[7];
  len_His[7]=len_His[6];
  len_His[6]=len_His[5];
  len_His[5]=len_His[4];
  len_His[4]=len_His[3];
  len_His[3]=len_His[2];
  len_His[2]=len_His[1];
  len_His[1]=len_His[0];
  len_His[0]=street_len;
}
/*******************************************************************************/
//                      上一部分：图像处理部分
//
//                      下一部分：事件管理部分
/*******************************************************************************/
void EVENT_Duty(void)
{
  /************任务时钟*****************/
  lost_car_time_flag++;
  cur_R_ready_time_flag++;
  cur_R_real_time_flag++;
  cur_L_ready_time_flag++;
  cur_L_real_time_flag++;
  /************启动计时*****************/
  if(KEY_start_flag)KEY_start_time_flag++;
  else;
  if(KEY_start_time_flag>5000) KEY_start_time_flag=5000;
  else;
  /*************十字部分****************/
  if(lost_flag|| lost_delay_flag)//十字识别
  {
    lost_delay_flag=1;
    lost_duty_flag=1;
    if((lost_flag==0&&lost_car_time_flag>15)||lost_car_time_flag>30)
    {
      lost_delay_flag=0;
    }else;
  }
  else
  {
    lost_car_time_flag=0;
    lost_duty_flag=0;
  }
  /*************圆环预识别****************/
  /*右预识别*/
  if(cur_R_ready_flag||cur_R_ready_delay_flag)
  { cur_R_ready_delay_flag=1;
    if((!lost_right[23]&&!lost_right[25])&&!lost_left[23]&&!lost_left[25]&&street_len>50&&cur_R_ready_time_flag>3)//丢线为1
    {cur_R_ready_rest_flag=1;}else;
    if(cur_R_ready_time_flag>150||lost_left[23]||lost_left[25])
    {cur_R_ready_delay_flag=0;cur_R_ready_rest_flag=0;}
    else if(cur_R_ready_rest_flag&&lost_right[23]&&lost_right[25]&&!lost_left[23]&&!lost_left[25]&&street_len>50)
    { cur_R_ready_delay_flag=0;cur_R_ready_rest_flag=0;cur_R_real_flag=1;}else;
  }
  else
  {cur_R_ready_delay_flag=0;
    cur_R_ready_time_flag=0;
    cur_R_ready_rest_flag=0;
  }
  /*左预识别*/
  if(cur_L_ready_flag||cur_L_ready_delay_flag)
  { cur_L_ready_delay_flag=1;
    if((!lost_left[23]&&!lost_left[25])&&!lost_right[23]&&!lost_right[25]&&street_len>50&&cur_L_ready_time_flag>3)
    {cur_L_ready_rest_flag=1;}else;
    if(cur_L_ready_time_flag>50||lost_right[23]||lost_right[25])
    {cur_L_ready_delay_flag=0 ;cur_R_ready_rest_flag=0;}
    else if(cur_L_ready_rest_flag&&lost_left[23]&&lost_left[25]&&!lost_right[23]&&!lost_right[25]&&street_len>50)
    { cur_L_ready_delay_flag=0;cur_L_ready_rest_flag=0;cur_L_real_flag=1;}else;
  }
  else
  { cur_L_ready_delay_flag=0;
    cur_L_ready_time_flag=0;
    cur_L_ready_rest_flag=0;
  }
  /*************圆环准确识别****************/
  /*右准确识别*/
  if(cur_R_real_flag||cur_R_real_delay_flag)
  { cur_R_real_flag=0;
    cur_R_real_delay_flag=1;
    if(cur_R_real_time_flag>50&&(lost_left[23]&&lost_left[25]))
      cur_R_real_rest_flag=1;else;
    if(cur_R_real_rest_flag)cur_R_real_time_flag1++;else;
    if(cur_R_real_time_flag1>100)cur_R_real_time_flag=100;else;
    if(cur_R_real_time_flag1>20)//&&!lost_left[23]&&!lost_left[25]&&!lost_right[23]&&!lost_right[25])
    {cur_R_real_delay_flag=0;}else;
  }
  else
  {cur_R_real_delay_flag=0;
    cur_R_real_time_flag=0;
    cur_R_real_time_flag1=0;
    cur_R_real_rest_flag=0;
  }
  /*左准确识别*/
  if(cur_L_real_flag||cur_L_real_delay_flag)
  { cur_L_real_flag=0;
    cur_L_real_delay_flag=1;
    if(cur_L_real_time_flag>50&&(lost_right[23]&&lost_right[25]))
      cur_L_real_rest_flag=1;else;
    if(cur_L_real_rest_flag)cur_L_real_time_flag1++;else;
    if(cur_L_real_time_flag1>100)cur_L_real_time_flag1=100;
    if(cur_L_real_time_flag1>20)//&&!lost_left[23]&&!lost_left[25]&&!lost_right[23]&&!lost_right[25])
    {cur_L_real_delay_flag=0;}else;
  }
  else
  {cur_L_real_delay_flag=0;
    cur_L_real_time_flag=0;
    cur_L_real_time_flag1=0;
    cur_L_real_rest_flag=0;
  }
}
/*******************************************************************************/
//                      上一部分：事件管理部分
//
//                      下一部分：车辆控制部分
/*******************************************************************************/
/***********************DJ_PID**************************/
void DJ_PID(unsigned char midpoint_in)  //舵机PD
{
  signed int static   midpoint_error;   //中线误差
  signed int static  lastpoint_error;   //上次误差
  if(lost_duty_flag&&!(cur_R_real_delay_flag||cur_L_real_delay_flag))
    midpoint_error=(signed int)((midpoint_in-40)*0.6); //计算误差十字
  else
  midpoint_error=midpoint_in-40;         //计算误差
  DJ_PWM=DJ_midpoint+(KP_A*midpoint_error*midpoint_error/1000+KP_B)*midpoint_error+KD*(midpoint_error-lastpoint_error);
  lastpoint_error=midpoint_error;        //记录上次误差
  if(DJ_PWM<DJ_midpoint-1300)            //输出限幅
    DJ_PWM=DJ_midpoint-1300;
  else if(DJ_PWM>DJ_midpoint+1300)
    DJ_PWM=DJ_midpoint+1300;
  else;
  ftm_pwm_duty(FTM2,FTM_CH0,DJ_PWM);     //输出PWM
}
/*******************编码器计数结算************************/
void FEED_COUNT(void)
{
  Feed_speed=(uint32)(Feed_flag/feed_fix);
  Feed_flag=0;
}
/*******************转速PID控制函数*************************/
void speed_PID(unsigned char speed_in)
{
  signed int static speed_out;         //输出实际速度
  signed int static PWM_CH5;           //输出PWM_CH5
  signed int static PWM_CH2;           //输出PWM_CH2
  signed int static speed_error;       //速度误差       
  signed int static last_error_1;      //上次偏差
  signed int static last_error_2;      //上上次偏差
  speed_error=speed_in-Feed_speed;      //计算转速误差
  speed_out +=Speed_P*(speed_error-last_error_1)+Speed_I*speed_error+   \
              Speed_D*(speed_error-2*last_error_1+last_error_2);
  last_error_2=last_error_1;            //记录上上次误差
  last_error_1=speed_error;             //记录上次误差
  /*******输出限幅 危险，误动！*********/
  if(speed_out>230)speed_out=230;
  else if(speed_out<-230)speed_out=-230;
  else;
  /*******堵转事件&发车启动控制********/
  if(Feed_speed<15 && (speed_out>30||speed_out<-30))
    Block_motor_time_flag++;            //堵转计时
  else Block_motor_time_flag=0;        
  if(Block_motor_time_flag>20||Block_motor_delay_flag)
  {                                     //堵转时间阈值：250ms
    Block_motor_time_flag=0;            //堵转计时复位
    Block_motor_delay_flag=1;           //堵转自锁置位
    Block_motor_duty_flag=1;            //堵转事件置位
    speed_out=0;                        //堵转输出保护
  }
  else
  {
    Block_motor_delay_flag=0;           //堵转自锁复位
    Block_motor_duty_flag=0;            //堵转事件复位
  }
  /**********双极性PWM计算*************/
  PWM_CH5=(signed int)(125-(speed_out/2));
  PWM_CH2=(signed int)(125+(speed_out/2));
  /*******PWM限幅 危险，误动！*********/
  if(PWM_CH5>240)PWM_CH5=240;
  else if(PWM_CH5<10)PWM_CH5=10;
  else;
  if(PWM_CH2>240)PWM_CH2=240;
  else if(PWM_CH2<10)PWM_CH2=10;
  else;
  /*************PWM输出****************/
  ftm_pwm_duty(FTM0,FTM_CH0,PWM_CH5);
  ftm_pwm_duty(FTM0,FTM_CH1,PWM_CH2);
}
/*****************执行机构综合控制函数*********************/
void CAR_Drive_duty(void)           //执行机构综合控制函数
{
  /************舵机控制**************/
  if(gpio_get(PTE4))
   DJ_PID(40);                      //舵机打中值
  else DJ_PID(mid_point[img_y_control]);//舵机控制
  /************速度控制**************/
  if(street_len>50)
  speed_need=speed_need_Boost;  
  else
  { 
    if(street_len>40&&street_len<50)
    speed_need=speed_need_L;            
    else
    speed_need=speed_need_normal;
  }
  /************电机控制**************/
  if(KEY_start_time_flag<100)//停车
  {
    ftm_pwm_duty(FTM0,FTM_CH0,130);
    ftm_pwm_duty(FTM0,FTM_CH1,125);
  }
  else                      //PID控制    
  {
    speed_PID(speed_need);                            
  }
}
/*******************************************************************************/
//                      上一部分：车辆控制部分
//
//                      下一部分：主函数部分
/*******************************************************************************/
void  main(void)
{
  /**************初始化区段***************/
  SCB->CPACR|=((3UL<<10*2)|(3UL<<11*2));//开启硬件浮点
  IO_init();                            //I/O初始化
  LCD_Init();                           //OLED初始化
  len_init();                           //赛道宽度初始化
  camera_init(imgbuff);                 //摄像头初始化
  pit_init_ms(PIT0,10);                 //pit0定时中断(用于编码器采集)
  NVIC_SetPriorityGrouping(4);          //设置优先级分组,3bit
  NVIC_SetPriority(PORTC_IRQn,0);       //配置优先级PORTA
  NVIC_SetPriority(PORTD_IRQn,1);       //配置优先级PORTD
  NVIC_SetPriority(DMA0_IRQn,2);        //配置优先级DMA0
  NVIC_SetPriority(PIT0_IRQn,3);        //配置优先级PIT0
  set_vector_handler(PORTD_VECTORn ,PORTD_IRQHandler);//设置PORTD的中断复位函数
  set_vector_handler(PORTC_VECTORn ,PORTC_IRQHandler);//设置PORTA的中断服务函数
  set_vector_handler(DMA0_VECTORn  ,DMA0_IRQHandler); //设置 DMA0的中断服务函数
  set_vector_handler(PIT0_VECTORn  ,PIT0_IRQHandler); //设置 PIT0 的中断服务函数
  enable_irq(PORTD_IRQn);               //使能PORTD中断
  enable_irq(PIT0_IRQn);                //使能PIT0中断
  left_point[img_y_max]=0;
  right_point[img_y_max]=80;
  /**************主循环区段***************/
  while(1)
  {
    //pit_time_start(PIT1);                     //取消屏蔽将测量程序总时间
    camera_get_img();                           //摄像头获取图像
    //pit_time_start(PIT1);                     //取消屏蔽将测量处理运算时间
    img_extract(img, imgbuff, CAMERA_SIZE);     //数据解压
    IMG_DUTY();                                 //主图像处理函数
    CAR_Drive_duty();                           //执行机构综合控制
    GUI_Duty();                                 //调试界面函数
    //code_Time_flag =pit_time_get_us(PIT1)/1000;//取消屏蔽将测量时间
  }
}
/*******************************************************************************/
//                     上一部分：主函数部分
//
//                     下一部分：中断函数部分
/*******************************************************************************/
/*****************摄像头场/行中断**********************/
void PORTC_IRQHandler()
{
    uint8  n;                    //引脚号
    uint32 flag;
    flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;           //清中断标志位
    n = 6;                     //场中断
    if(flag & (1 << n))         //PTA29触发中断
    {camera_vsync();}
}
/********************DMA数据传输中断*********************/
void DMA0_IRQHandler()
{
    camera_dma();
}
/*******************PIT0编码器采样中断*******************/
void PIT0_IRQHandler()
{
    PIT_Flag_Clear(PIT0);
    FEED_COUNT();
}
/*****************PORTD编码器计数中断********************/
void PORTD_IRQHandler(void)
{
    uint8  n = 0;              //引脚号
    n = 0;
    if(PORTD_ISFR & (1 << n))  //PTD0 触发中断
    {
        PORTD_ISFR  = (1 << n);//写1清中断标志位
        Feed_flag++;           //编码器采集计数...
    }
}