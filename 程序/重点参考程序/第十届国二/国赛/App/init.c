/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外初学论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       init.c
 * @brief      山外K60 平台主程序
 * @author     山外科技
 * @version    v5.0
 * @date       
 * author      
 */
#include "common.h"
#include "include.h"
#include  "init.h"
#include  "PID.h"

#define   angle_max        45

float  KKi;
float  KKp;
float  KKd;

float   P_1; 
float   P_2; 
float   P_3; 
float   D_1;
float   D_2; 
float   D_3;

float  S_1;
float  S_2;
float  S_3;

float   DP_1 ;       // 6         大弯道
float   DD_1 ;       // 4  
float   DS_1 ;       //1.2
float   DP_2 ;       //7        小弯道
float   DD_2 ;       // 5
float   DS_2 ;       //1.2
float   DP_3 ;       //2       直道
float   DD_3 ;       //1
float   DS_3 ;       // 0.5
     
float    CS_P_1,CS_P_2;
float speed_error[45]={   0,   1.0139,  1.0279, 1.0419, 1.0559, 1.0700, 1.0840,    //7
                      1.0981, 1.1124, 1.1266, 1.1410, 1.1554, 1.1700, 1.1846,     //7
                      1.1994, 1.2143, 1.2293, 1.2445, 1.2599, 1.2754, 1.2911,     //7
                      1.3070, 1.3232, 1.3395, 1.3561, 1.3730, 1.4076, 1.4253,     //7
                      1.4434, 1.4618, 1.4806, 1.4998, 1.5195, 1.5396, 1.5601,     //7
                      1.5812, 1.6028, 1.6249, 1.6477, 1.6712, 1.6953, 1.7203,     //7
                      1.7460, 1.7724, 1.800};   //3

int32  val_z=0, val_y=0;  
extern uint8 wandao_flag,inwandao_flag,s_zhidao_flag,Mode2_flag;
extern int8 tingche_flag;
void FTM_init()
{  
    //光电管
    gpio_init(PTD0,GPI,0);
    port_init_NoALT(PTD0,PULLUP);
    gpio_init(PTD1,GPI,0);
    port_init_NoALT(PTD1,PULLUP);
    //光电管
    
    //一键停车
    gpio_init(PTD6,GPI,0);
    port_init_NoALT(PTD6,PULLDOWN);
    //一键停车
    
    
    //红外管
    gpio_init(PTD2,GPI,0);
    port_init_NoALT(PTD2,PULLDOWN);
    gpio_init(PTD3,GPI,0);
    port_init_NoALT(PTD3,PULLDOWN);
    //红外管
    
   
    //拨码开关   从下往上
    gpio_init(PTA17,GPI,0);
    port_init_NoALT(PTA17,PULLUP);
    
    gpio_init(PTA16,GPI,0);
    port_init_NoALT(PTA16,PULLUP);
    
    gpio_init(PTA15,GPI,0);
    port_init_NoALT(PTA15,PULLUP);
    
    gpio_init(PTA14,GPI,0);
    port_init_NoALT(PTA14,PULLUP);
    
    gpio_init(PTA13,GPI,0);
    port_init_NoALT(PTA13,PULLUP);
    
    gpio_init(PTA12,GPI,0);
    port_init_NoALT(PTA12,PULLUP);
    //拨码开关   从下往上

    
    //测周期
    gpio_init(PTD15,GPO,0);
    //测周期
   
    adc_init(Gyro1);         //角加速度  Angular1
    adc_init (ZOUT); 

    ftm_quad_init(FTM1);
    ftm_quad_init(FTM2);
    ftm_pwm_init(S3010_FTM, S3010_CH,S3010_HZ, DJ_cen);     //初始化 舵机 PWM        
    ftm_pwm_init(MOTOR_FTM, MOTOR1_PWM,MOTOR_HZ,0);         //初始化 电机 PWM
    ftm_pwm_init(MOTOR_FTM, MOTOR2_PWM,MOTOR_HZ,0);         //初始化 电机 PWM
    ftm_pwm_init(MOTOR_FTM, MOTOR3_PWM,MOTOR_HZ,0);         //初始化 电机 PWM
    ftm_pwm_init(MOTOR_FTM, MOTOR4_PWM,MOTOR_HZ,0);         //初始化 电机 PWM  
  
}
  
void  DangWei_()
{
  if(PTA17_IN==0&&PTA16_IN==1&&PTA15_IN==1&&PTA14_IN==1&&PTA13_IN==1)  //高速档①
  {
      P_1=3.1;   //4   3.2
      P_2=2.2;   //2
      P_3=3.6;   //4
      D_1=15;  //15  
      D_2=20;    //20
      D_3=25;    //20
      S_1=0.5;    //急弯   
      S_2=0.5;
      S_3=0.5;
      
      DP_1=4.5   ;       // 6       大弯道
      DD_1=2.5   ;       // 4  
      DS_1=1.2 ;       //1.2
        
      DP_2=4.5   ;       //7        小弯道
      DD_2=2.5   ;       // 5
      DS_2=1.2 ;       //1.2
        
      DP_3=2   ;       //2        直道
      DD_3=1   ;       //1
      DS_3=0.5 ;       // 0.5
      
      KKi=7;          //速度
      KKp=6;          //速度
      KKd=3.5;        //速度
      
      
      CS_P_1=3.8;      //差速比 
      CS_P_2=3.8;      //差速比 
  } 
  if(PTA16_IN==0&&PTA17_IN==1&&PTA15_IN==1&&PTA14_IN==1&&PTA13_IN==1)  //中①速档
  {
      P_1=3.1;   //4
      P_2=2.2;   //2
      P_3=3.5;   //4
      D_1=15;    
      D_2=15;    
      D_3=20;
      S_1=0.5;    //急弯   
      S_2=0.5;
      S_3=0.5;
      
      DP_1=4.2   ;       // 6       大弯道
      DD_1=2.5   ;       // 4  
      DS_1=1.2 ;       //1.2
        
      DP_2=4.2  ;       //7        小弯道
      DD_2=2.5  ;       // 5
      DS_2=1.2 ;       //1.2
        
      DP_3=2.5   ;       //2        直道
      DD_3=1.5   ;       //1
      DS_3=0.5 ;       // 0.5
      
      KKi=7;          //速度
      KKp=6;          //速度
      KKd=3.5;        //速度
      
      CS_P_1=5;      //差速比 
      CS_P_2=5;      //差速比 
  }
  if(PTA15_IN==0&&PTA17_IN==1&&PTA16_IN==1&&PTA14_IN==1&&PTA13_IN==1)   //中②速档
  {
      P_1=3.5;   //4
      P_2=2.2;   //2
      P_3=3.5;   //4
      D_1=15;    
      D_2=15;  //  
      D_3=20;
      S_1=0.5;    //急弯   
      S_2=0.5;
      S_3=0.5;
      
      DP_1=4   ;       // 6       大弯道
      DD_1=2   ;       // 4  
      DS_1=1.2 ;       //1.2
        
      DP_2=4   ;       //7        小弯道
      DD_2=2   ;       // 5
      DS_2=1.2 ;       //1.2
        
      DP_3=2   ;       //2        直道
      DD_3=1   ;       //1
      DS_3=0.5 ;       // 0.5
      
      KKi=7;          //速度
      KKp=6;          //速度
      KKd=3.5;        //速度
      
      CS_P_1=5;      //差速比 
      CS_P_2=5;      //差速比 
  }
  if(PTA14_IN==0&&PTA17_IN==1&&PTA16_IN==1&&PTA15_IN==1&&PTA13_IN==1)   //低速档
  {
      P_1=3;   //3  6月30号参值
      P_2=2;   //1  6月30号参值
      P_3=3;   //4  6月30号参值
      D_1=6;    
      D_2=1;    
      D_3=15;
      S_1=0.5;    //急弯   
      S_2=0.5;
      S_3=0.5;
      
      DP_1=5   ;       // 6       大弯道
      DD_1=2   ;       // 4  
      DS_1=1.2 ;      //1.2
        
      DP_2=6   ;       //7        小弯道
      DD_2=2   ;       // 5
      DS_2=1.2 ;       //1.2
        
      DP_3=2   ;       //2        直道
      DD_3=1   ;      //1
      DS_3=0.5 ;      // 0.5
      
      KKi=7;          //速度
      KKp=6;          //速度
      KKd=3.5;        //速度
      
      CS_P_1=5;      //差速比 
      CS_P_2=5;      //差速比 
  }
  if(PTA13_IN==0&&PTA17_IN==1&&PTA16_IN==1&&PTA15_IN==1&&PTA14_IN==1)  //高速档②
  {
      P_1=3.5;   //4   3.2
      P_2=2.3;   //2
      P_3=3.0;   //4
      D_1=15;  //15  
      D_2=25;    //20
      D_3=20;    //20
      S_1=0.5;    //急弯   
      S_2=0.5;
      S_3=0.5;
      
      DP_1=4.5   ;       // 6       大弯道
      DD_1=2.5   ;       // 4  
      DS_1=1.2 ;       //1.2
        
      DP_2=4.5   ;       //7        小弯道
      DD_2=2.5   ;       // 5
      DS_2=1.2 ;       //1.2
        
      DP_3=2   ;       //2        直道
      DD_3=1   ;       //1
      DS_3=0.5 ;       // 0.5
      
      KKi=7;          //速度
      KKp=6;          //速度
      KKd=3.5;        //速度
      
      
      CS_P_1=5.3;      //差速比 
      CS_P_2=5.3;      //差速比 
  } 
  if(PTA17_IN==0&&PTA16_IN==0&&PTA15_IN==1&&PTA14_IN==1&&PTA13_IN==1)   //原高速档②
  {
      P_1=3.1;   //4   3.2
      P_2=2.2;   //2
      P_3=3.3;   //4
      D_1=15;    //15  
      D_2=20;    //20
      D_3=20;    //20
      S_1=0.5;    //急弯   
      S_2=0.5;
      S_3=0.5;
      
      DP_1=4.5   ;       // 6       大弯道
      DD_1=2.5   ;       // 4  
      DS_1=1.2 ;        //1.2
        
      DP_2=4.5   ;       //7        小弯道
      DD_2=2.5   ;       // 5
      DS_2=1.2 ;       //1.2
        
      DP_3=2   ;       //2        直道
      DD_3=1   ;       //1
      DS_3=0.5 ;       // 0.5
      
      KKi=7;          //速度
      KKp=6;          //速度
      KKd=3.5;        //速度
      
      
      CS_P_1=3.8;      //差速比 
      CS_P_2=3.8;      //差速比 
  }
}
/************************************************************
 *  @brief        测速函数
     v5.0
 *   @note      
 ***********************************************************/
int8 num_00,num_11,flag_00;
extern int8  ZJ_Process_flag;
void get_speed()   //
{
    val_z = ftm_quad_get(FTM1);          //获取FTM 正交解码 的脉冲数(负数表示反方向)
    val_y = ftm_quad_get(FTM2);          //获取FTM 正交解码 的脉冲数(负数表示反方向)
    ftm_quad_clean(FTM1); 
    ftm_quad_clean(FTM2);  
    if(val_z>=0)
    {
       val_z=val_z;
    }
    else
    {
        val_z=-val_z;
    }
    if(val_y>=0)
    {
        val_y=val_y;
    }
    else
    {
        val_y=-val_y;
    }
/*高①*/if(PTA17_IN==0&&PTA16_IN==1&&PTA15_IN==1&&PTA14_IN==1&&PTA13_IN==1)  //高速档①
    {
              if(ZJ_Process_flag&&flag_00==0)
              {
                  if((val_z+val_y)/2>330)  //高于速度2.5M 160
                  {
                       flag_00=1;
                       num_00= 42 ;           //右36
                       num_11= 42 ;           //左40
                  }
                  else if((val_z+val_y)/2>=250)  //高于速度2M 160
                  {
                       flag_00=1;
                       num_00= 38 ;           //右38
                       num_11= 38 ;           //左40
                  }
                  else if((val_z+val_y)/2>=200)  //高于速度2M 160
                  {
                       flag_00=1;
                       num_00= 36 ;           //右38
                       num_11= 36 ;           //左40
                  }
                  else if((val_z+val_y)/2>=150)  //高于小于1.5M 160
                  {
                       flag_00=1;
                       num_00= 30 ;         //右34
                       num_11= 30 ;         //左35
                  }
                  else if((val_z+val_y)/2>=0)  //速度小于1.5M  160
                  {
                       flag_00=1;
                       num_00= 26 ;         //右30
                       num_11= 27 ;         //左31
                  }
                  else
                  {
                    
                  }
              }
      }
/*中①*/if(PTA16_IN==0&&PTA17_IN==1&&PTA15_IN==1&&PTA14_IN==1&&PTA13_IN==1)  //中①速档
      {
              if(ZJ_Process_flag&&flag_00==0)
              {
                  if((val_z+val_y)/2>330)  //高于速度2.5M 160
                  {
                       flag_00=1;
                       num_00= 39 ;           //右36
                       num_11= 39 ;           //左40
                  }
                  else if((val_z+val_y)/2>=250)  //高于速度2M 160
                  {
                       flag_00=1;
                       num_00= 35 ;           //右38
                       num_11= 35 ;           //左40
                  }
                  else if((val_z+val_y)/2>=200)  //高于速度2M 160
                  {
                       flag_00=1;
                       num_00= 33 ;           //右38
                       num_11= 33 ;           //左40
                  }
                  else if((val_z+val_y)/2>=150)  //高于小于1.5M 160
                  {
                       flag_00=1;
                       num_00= 30 ;         //右34
                       num_11= 30 ;         //左35
                  }
                  else if((val_z+val_y)/2>=0)  //速度小于1.5M  160
                  {
                       flag_00=1;
                       num_00= 26 ;         //右30
                       num_11= 26 ;         //左31
                  }
                  else
                  {
                    
                  }
              }
      }
/*中②*/if(PTA15_IN==0&&PTA17_IN==1&&PTA16_IN==1&&PTA14_IN==1&&PTA13_IN==1)   //中②速档
      {
              if(ZJ_Process_flag&&flag_00==0)
              {
                  if((val_z+val_y)/2>310)  //高于速度2.5M 160
                  {
                       flag_00=1;
                       num_00= 40 ;           //右36
                       num_11= 40 ;           //左40
                  }
                  else if((val_z+val_y)/2>=250)  //高于速度2M 160
                  {
                       flag_00=1;
                       num_00= 35 ;           //右38
                       num_11= 35 ;           //左40
                  }
                  else if((val_z+val_y)/2>=200)  //高于速度2M 160
                  {
                       flag_00=1;
                       num_00= 33 ;           //右38
                       num_11= 33 ;           //左40
                  }
                  else if((val_z+val_y)/2>=150)  //高于小于1.5M 160
                  {
                       flag_00=1;
                       num_00= 30 ;         //右34
                       num_11= 30 ;         //左35
                  }
                  else if((val_z+val_y)/2>=0)  //速度小于1.5M  160
                  {
                       flag_00=1;
                       num_00= 26 ;         //右30
                       num_11= 27 ;         //左31
                  }
                  else
                  {
                    
                  }
              }
      }
/*低*/if(PTA14_IN==0&&PTA17_IN==1&&PTA16_IN==1&&PTA15_IN==1&&PTA13_IN==1)   //低速档
      {
              if(ZJ_Process_flag&&flag_00==0)
              {
                  if((val_z+val_y)/2>330)  //高于速度2.5M 160
                  {
                       flag_00=1;
                       num_00= 39 ;           //右36
                       num_11= 39 ;           //左40
                  }
                  else if((val_z+val_y)/2>=200)  //高于速度2M 160
                  {
                       flag_00=1;
                       num_00= 36 ;           //右38
                       num_11= 36 ;           //左40
                  }
                  else if((val_z+val_y)/2>=150)  //高于小于1.5M 160
                  {
                       flag_00=1;
                       num_00= 30 ;         //右34
                       num_11= 30 ;         //左35
                  }
                  else if((val_z+val_y)/2>=0)  //速度小于1.5M  160
                  {
                       flag_00=1;
                       num_00= 26 ;         //右30
                       num_11= 27 ;         //左31
                  }
                  else
                  {
                    
                  }
              }
     }
/*高②*/if(PTA13_IN==0&&PTA17_IN==1&&PTA16_IN==1&&PTA15_IN==1&&PTA14_IN==1)  //高速档②
      {
              if(ZJ_Process_flag&&flag_00==0)
              {
                  if((val_z+val_y)/2>330)  //高于速度2.5M 160
                  {
                       flag_00=1;
                       num_00= 40 ;           //右36
                       num_11= 40 ;           //左40
                  }
                  else if((val_z+val_y)/2>=250)  //高于速度2M 160
                  {
                       flag_00=1;
                       num_00= 38 ;           //右38
                       num_11= 38 ;           //左40
                  }
                  else if((val_z+val_y)/2>=200)  //高于速度2M 160
                  {
                       flag_00=1;
                       num_00= 36 ;           //右38
                       num_11= 36 ;           //左40
                  }
                  else if((val_z+val_y)/2>=150)  //高于小于1.5M 160
                  {
                       flag_00=1;
                       num_00= 30 ;         //右34
                       num_11= 30 ;         //左35
                  }
                  else if((val_z+val_y)/2>=0)  //速度小于1.5M  160
                  {
                       flag_00=1;
                       num_00= 26 ;         //右30
                       num_11= 27 ;         //左31
                  }
                  else
                  {
                    
                  }
              }
      }
/*高③*/if(PTA17_IN==0&&PTA16_IN==0&&PTA15_IN==1&&PTA14_IN==1&&PTA13_IN==1)
      {
              if(ZJ_Process_flag&&flag_00==0)
              {
                  if((val_z+val_y)/2>330)  //高于速度2.5M 160
                  {
                       flag_00=1;
                       num_00= 41 ;           //右36
                       num_11= 41 ;           //左40
                  }
                  else if((val_z+val_y)/2>=250)  //高于速度2M 160
                  {
                       flag_00=1;
                       num_00= 38 ;           //右38
                       num_11= 38 ;           //左40
                  }
                  else if((val_z+val_y)/2>=200)  //高于速度2M 160
                  {
                       flag_00=1;
                       num_00= 36 ;           //右38
                       num_11= 36 ;           //左40
                  }
                  else if((val_z+val_y)/2>=150)  //高于小于1.5M 160
                  {
                       flag_00=1;
                       num_00= 30 ;         //右34
                       num_11= 30 ;         //左35
                  }
                  else if((val_z+val_y)/2>=0)  //速度小于1.5M  160
                  {
                       flag_00=1;
                       num_00= 26 ;         //右30
                       num_11= 27 ;         //左31
                  }
                  else
                  {
                    
                  }
              }
      }
      if(val_y==0&&val_z==0)
      {
        ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);
        ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
        ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
        ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
      }
}
//舵机中值：   1320       
//左打死  :    1530      210
//右打死：     1110     -210
int32   error_0;
uint32  PWM_OUT_1, PWM_OUT_2;
int32   angle,     Speed_Set;    //左正右负  
int16    a;
extern   int8 shizhi_flag;
void CS_HS()
{
  if(angle<=10&&angle>=-10)   //直道
  {
      PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
      PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y);  
  }
  else if(angle>10)   //zuo 弯
  {
     a=(int16)(angle_max*(angle)*1.0/(DJ_zuo-DJ_cen))-1 ;  //angle_max   45
     if(a>45)  a=0;
     if(a<0)   a=0;
     error_0=(int32)(speed_error[a]*CS_P_1*10);
     if(Speed_Set-error_0/2<0)  Speed_Set=error_0/2;
     PWM_OUT_1=(uint32)speed_pid_1(Speed_Set-error_0/2,val_z);  
     PWM_OUT_2=(uint32)speed_pid_2(Speed_Set+(int32)(error_0*1.5),val_y);  
  }
  else if(angle<-10)   //you 弯
  {
     a=(int16)(angle_max*(-angle)*1.0/(DJ_cen-DJ_you))-1 ;
     if(a>45)  a=0;
     if(a<0)   a=0;
     error_0=(uint32)(speed_error[a]*CS_P_2*10);
     if(Speed_Set-error_0/2<0)  Speed_Set=error_0/2;
     PWM_OUT_1=(uint32)speed_pid_1(Speed_Set+(int32)(error_0*1.5),val_z);  
     PWM_OUT_2=(uint32)speed_pid_2(Speed_Set-error_0/2,val_y);  
  }
}

//8月
void CS_HS_1()
{
  if(angle<=25&&angle>=-25)   //直道
  {
      PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
      PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y);  
  }
  else if(angle>25)   //zuo 弯
  {
     a=(int16)(angle_max*(angle)*1.0/(DJ_zuo-DJ_cen))-1 ;  //angle_max   45
     if(a>45)  a=0;
     if(a<0)   a=0;
     error_0=(int32)(speed_error[a]*CS_P_1*10);
     if(Speed_Set-error_0/2<100)  Speed_Set=error_0/2;
     PWM_OUT_1=(uint32)speed_pid_1(Speed_Set-error_0/2,val_z);  
     PWM_OUT_2=(uint32)speed_pid_2(Speed_Set+(int32)(error_0*1.5),val_y);  
  }
  else if(angle<-25)   //you 弯
  {
     a=(int16)(angle_max*(-angle)*1.0/(DJ_cen-DJ_you))-1 ;
     if(a>45)  a=0;
     if(a<0)   a=0;
     error_0=(uint32)(speed_error[a]*CS_P_2*10);
     if(Speed_Set-error_0/2<0)  Speed_Set=error_0/2;
     PWM_OUT_1=(uint32)speed_pid_1(Speed_Set+(int32)(error_0*1.5),val_z);  
     PWM_OUT_2=(uint32)speed_pid_2(Speed_Set-error_0/2,val_y);  
  }
}

////////////////////////////////////////////////////
int8 Cont_1=0,Trg_1=0,ReadData_1=0,count_flag_1=0,qipao_flag_0=1;

void  JianChe()    //5MS检测一次，有两次检车到灯塔信号，即输出高电平。起跑在一米区内
{
  
  if(PTD2_IN&&PTD3_IN)
  {
      qipao_flag_0=0;    //检测到起跑信号
  }
  else
  {
      qipao_flag_0=1;
  }
                                           //没有    第一次检测   第二次检测    第三次检测     没有   第二次没有
   ReadData_1=0x01^qipao_flag_0;           // 0       1            1             1              0      0
   Trg_1=ReadData_1&(ReadData_1^Cont_1);   // 0       1            0             0              0      0 
   Cont_1=ReadData_1;                      // 0       1            1             1              0      0
   if(Trg_1==1)
   {
        count_flag_1= count_flag_1+1;  
   }
}

////////////////////////////////////////////////////


extern int8    y_flag,  z_flag;
extern int32   angle_pwm;  
extern int8    zj_flag,zj_flag_1,bridge_flag;
extern float  error_4;
int8  FLAG_Count=0, FLAG_Count_1=0, count_000=0,qipao_kaishi=0;

int16  num_flag;
void Speed_Contorl()
{
     qipao_kaishi=0;
     angle=angle_pwm-DJ_cen;
     if(tingche_flag==1)    
     {
            FLAG_Count=1;
            PWM_OUT_2=0;
            PWM_OUT_1=0;
            ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);
            ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
            ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
            ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);  
     }
     else   
     {
/*高①*/if(PTA17_IN==0&&PTA16_IN==1&&PTA15_IN==1&&PTA14_IN==1&&PTA13_IN==1)  //高速档①
       {
                  // Site_t site = {0,110};   //x = 10 ,y = 20
                 //  LCD_str(site,"GS", BLUE,RED);
                   if(!shizhi_flag)
                   {
                     if(! ZJ_Process_flag&&!(count_flag_1==2&&(PTD2_IN==1||PTD3_IN==1)))
                     {
                       if(val_z<200&&val_y<200&&(-10<angle&&angle<10))               //起跑加速
                       {
                           Speed_Set=500;  
                       }
                       else if(val_z<280&&val_y<280&&(-50<angle&&angle<50))               //起跑加速
                       {
                           Speed_Set=400;  
                       }
                       else if((val_z<270||val_y<270)&&(angle>50||angle<-50))      //弯道
                       {
                           Speed_Set=350;//250; 
                       }

                       else if((val_z>300&&val_y>300)&&(error_4>10.0||error_4<-10.0))      //弯道  不变
                       {
                           Speed_Set=50;  //最低速度 
                       } 
                       else if((val_z>260&&val_y>260)&&(error_4>10.0||error_4<-10.0))      //弯道   不变
                       {
                           Speed_Set=250;  //最低速度 
                       }
                       else if((val_z>230&&val_y>230)&&(error_4>10.0||error_4<-10.0))      //弯道   不变
                       {
                           Speed_Set=200;  //最低速度 
                       }
                       else                                                         //中速
                       {
                           Speed_Set=350; 
                       }
                       if(bridge_flag==1)
                       {
                           Speed_Set=150;
                           PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                           PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y); 
                           num_flag++;
                           if(num_flag>5)  
                           { 
                             bridge_flag=0;
                             num_flag=0;
                           }                       }
                          else    CS_HS();
                       }
                     else if(ZJ_Process_flag&&zj_flag)    //左正转9000  右反转4000  右直角
                     {
                       
                        PWM_OUT_1=9000;  
                        PWM_OUT_2=4000;
                        ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, PWM_OUT_2);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                     }
                     else if(ZJ_Process_flag&&zj_flag_1)    //右正转9000  左反转4000  左直角
                     {
                       
                        PWM_OUT_1=4000;  
                        PWM_OUT_2=9000;
                        ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, PWM_OUT_1);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                     }
                     else 
                     {
                           if(!(count_flag_1==2&&(PTD2_IN==1||PTD3_IN==1)))
                           Speed_Set=320;//250 
                           PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                           PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                     }
                  }
                  if(shizhi_flag)
                  {
                       Speed_Set=300;//250 
                       PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                       PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y); 
                  }
                  if(!ZJ_Process_flag)
                  {
                     //发、停车程序
                    if(count_flag_1==1&&PTD2_IN&&PTD3_IN)
                    { 
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //右
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //左
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                    }
                    else if(count_flag_1==2&&(PTD2_IN==1||PTD3_IN==1)&&PTA12_IN==1)
                    {
                          Speed_Set=180;//250 
                          PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                          PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y);  
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);  //右
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //左
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                    }   
                    else if(count_flag_1==2&&(PTD2_IN==0&&PTD3_IN==0)&&PTA12_IN==1)
                    { 
                          count_000=count_000+1;
                        
                          if((val_z+val_y)/2<50)
                          {
                              ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //右
                              ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                              ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //左
                              ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                          }
                          else 
                          {
                              ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //右
                              ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 8000);
                              ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 8000);  //左
                              ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                          }
                          if(count_000>50) count_000=0;
                    }
                    else if(count_flag_1==3&&PTA12_IN==1)
                    {
                              ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //右
                              ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                              ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //左
                              ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                    }
                    //发、停车程序
                    else if(angle_pwm>=DJ_zuo-0&&val_z>200&&val_y>200)              //右正转9000  左反转4000
                    {
                          PWM_OUT_1=2000;   
                          PWM_OUT_2=8000;
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);  //右
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, PWM_OUT_1);  //左
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                    }
                    else if(angle_pwm<=DJ_you+0&&val_z>200&&val_y>200)              //左正转9000  右反转4000
                    {
                          PWM_OUT_1=8000;  
                          PWM_OUT_2=2000;
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, PWM_OUT_2);   //右
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);   //左
                        
                    }
                    else
                    {
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                    }
                 }    
     }
/*中①*/if(PTA16_IN==0&&PTA17_IN==1&&PTA15_IN==1&&PTA14_IN==1&&PTA13_IN==1)  //中①速档
     {
                   if(!shizhi_flag)
                   {
                     if(! ZJ_Process_flag)
                     {
                       if(val_z<150&&val_y<150&&(-10<angle&&angle<10))               //起跑加速
                       {
                           Speed_Set=450;  
                       }
                       else if((val_z>250||val_y>250)&&(angle>20||angle<-20))       //入弯
                       {
                           Speed_Set=200;//150; 
                       }
                       else if((val_z<230||val_y<230)&&(angle>50||angle<-50))      //弯道
                       {
                           Speed_Set=260;//250; 
                       }
                       else if((val_z>300&&val_y>300)&&(error_4>10.0||error_4<-10.0))      //弯道  不变
                       {
                           Speed_Set=50;  //最低速度 
                       }
                       else if((val_z>260&&val_y>260)&&(error_4>10.0||error_4<-10.0))      //弯道   不变
                       {
                           Speed_Set=250;  //最低速度 
                       }
                       else                                                         //中速
                       {
                           Speed_Set=320; 
                       }
                       if(bridge_flag==1)
                       {
                           Speed_Set=150;
                           PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                           PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y); 
                           num_flag++;
                           if(num_flag>3)  
                           { 
                             bridge_flag=0;
                             num_flag=0;
                           }             
                       }
                           else    CS_HS();
                     }
                     else if(ZJ_Process_flag&&zj_flag)    //左正转9000  右反转4000  右直角
                     {
                       
                        PWM_OUT_1=9000;  
                        PWM_OUT_2=4000;
                        ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, PWM_OUT_2);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                     }
                     else if(ZJ_Process_flag&&zj_flag_1)    //右正转9000  左反转4000  左直角
                     {
                       
                        PWM_OUT_1=4000;  
                        PWM_OUT_2=9000;
                        ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, PWM_OUT_1);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                     }
                     else 
                     {
                           Speed_Set=300;//250 
                           PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                           PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                     }
                  }
                  if(shizhi_flag)
                  {
                       Speed_Set=300;//250 
                       PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                       PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y); 
                  }
                  if(!ZJ_Process_flag)
                  {
                    //发、停车程序
                    if(count_flag_1==1&&PTD2_IN&&PTD3_IN)
                    { 
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //右
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //左
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                    }
                    else if(count_flag_1==2&&(PTD2_IN==1||PTD3_IN==1)&&PTA12_IN==1)
                    {
                          Speed_Set=220;//250 
                          PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                          PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y);  
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);  //右
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //左
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                    }   
                    else if(count_flag_1==2&&(PTD2_IN==0&&PTD3_IN==0)&&PTA12_IN==1)
                    { 
                          count_000=count_000+1;
                        
                          if((val_z+val_y)/2<50)
                          {
                              ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //右
                              ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                              ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //左
                              ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                          }
                          else 
                          {
                              ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //右
                              ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 8000);
                              ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 8000);  //左
                              ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                          }
                          if(count_000>50) count_000=0;
                    }
                    else if(count_flag_1==3&&PTA12_IN==1)
                    {
                              ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //右
                              ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                              ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //左
                              ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                    }
                    //发、停车程序
               /*     else if(angle_pwm>=DJ_zuo-0&&val_z>200&&val_y>200)              //右正转9000  左反转4000
                    {
                          PWM_OUT_1=1000;   
                          PWM_OUT_2=9000;
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);  //右
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, PWM_OUT_1);  //左
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                    }
                    else if(angle_pwm<=DJ_you+0&&val_z>200&&val_y>200)              //左正转9000  右反转4000
                    {
                          PWM_OUT_1=9000;  
                          PWM_OUT_2=1000;
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, PWM_OUT_2);   //右
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);   //左
                        
                    }  */
                    else
                    {
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                    }
                 }    
     }
/*中②*/if(PTA15_IN==0&&PTA17_IN==1&&PTA16_IN==1&&PTA14_IN==1&&PTA13_IN==1)   //中②速档
     {
                   if(!shizhi_flag)
                   {
                     if(! ZJ_Process_flag&&!(count_flag_1==2&&(PTD2_IN==1||PTD3_IN==1)))
                     {
                       if(val_z<150&&val_y<150&&(-10<angle&&angle<10))               //起跑加速
                       {
                           Speed_Set=450;  
                       }
                       else if((val_z>250||val_y>250)&&(angle>20||angle<-20))       //入弯
                       {
                           Speed_Set=200;//150; 
                       }
                       else if((val_z<230||val_y<230)&&(angle>50||angle<-50))      //弯道
                       {
                           Speed_Set=260;//250; 
                       }
                       else if((val_z>300&&val_y>300)&&(error_4>10.0||error_4<-10.0))      //弯道  不变
                       {
                           Speed_Set=50;  //最低速度 
                       }
                       else if((val_z>260&&val_y>260)&&(error_4>10.0||error_4<-10.0))      //弯道   不变
                       {
                           Speed_Set=250;  //最低速度 
                       }
                       else                                                         //中速
                       {
                           Speed_Set=320; 
                       }
                       if(bridge_flag==1)
                       {
                           Speed_Set=100;
                           PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                           PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y); 
                           num_flag++;
                           if(num_flag>3)  
                           { 
                             bridge_flag=0;
                             num_flag=0;
                           }
                       }
                       else   CS_HS();
                     }
                     else if(ZJ_Process_flag&&zj_flag)    //左正转9000  右反转4000  右直角
                     {
                       
                        PWM_OUT_1=9000;  
                        PWM_OUT_2=4000;
                        ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, PWM_OUT_2);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                     }
                     else if(ZJ_Process_flag&&zj_flag_1)    //右正转9000  左反转4000  左直角
                     {
                       
                        PWM_OUT_1=4000;  
                        PWM_OUT_2=9000;
                        ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, PWM_OUT_1);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                     }
                     else 
                     {
                           if(!(count_flag_1==2&&(PTD2_IN==1||PTD3_IN==1)))
                           Speed_Set=320;//250 
                           PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                           PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                     }
                  }
                  if(shizhi_flag)
                  {
                       Speed_Set=300;//250 
                       PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                       PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y); 
                  }
                  if(!ZJ_Process_flag)
                  {
                    //发、停车程序
                    if(count_flag_1==1&&PTD2_IN&&PTD3_IN)
                    { 
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //右
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //左
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                    }
                    else if(count_flag_1==2&&(PTD2_IN==1||PTD3_IN==1)&&PTA12_IN==1)
                    {
                          Speed_Set=250;//250 
                          PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                          PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y);  
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);  //右
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //左
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                    }   
                    else if(count_flag_1==2&&(PTD2_IN==0&&PTD3_IN==0)&&PTA12_IN==1)
                    { 
                          count_000=count_000+1;
                        
                          if((val_z+val_y)/2<50)
                          {
                              ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //右
                              ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                              ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //左
                              ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                          }
                          else 
                          {
                              ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //右
                              ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 8000);
                              ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 8000);  //左
                              ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                          }
                          if(count_000>50) count_000=0;
                    }
                    else if(count_flag_1==3&&PTA12_IN==1)
                    {
                              ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //右
                              ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                              ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //左
                              ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                    }
                    //发、停车程序
                    else if(angle_pwm>=DJ_zuo-0&&val_z>200&&val_y>200)              //右正转9000  左反转4000
                    {
                          PWM_OUT_1=1000;   
                          PWM_OUT_2=9000;
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);  //右
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, PWM_OUT_1);  //左
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                    }
                    else if(angle_pwm<=DJ_you+0&&val_z>200&&val_y>200)              //左正转9000  右反转4000
                    {
                          PWM_OUT_1=9000;  
                          PWM_OUT_2=1000;
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, PWM_OUT_2);   //右
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);   //左
                        
                    }
                    else
                    {
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                    }
                 }    
     }
/*低*/if(PTA14_IN==0&&PTA17_IN==1&&PTA16_IN==1&&PTA15_IN==1&&PTA13_IN==1)   //低速档
     {
                   if(!shizhi_flag)
                   {
                     if(! ZJ_Process_flag)
                     {
                       if(val_z<150&&val_y<150&&(-10<angle&&angle<10))               //起跑加速
                       {
                           Speed_Set=420;  
                       }
                       else if((val_z<200||val_y<200)&&(angle>50||angle<-50))      //弯道
                       {
                           Speed_Set=230;//250; 
                       }
                       else if((val_z>230&&val_y>230)&&(error_4>10.0||error_4<-10.0))      //弯道   不变
                       {
                           Speed_Set=150;  //最低速度 
                       }
                       else                                                         //中速
                       {
                           Speed_Set=270; 
                       }   
                       if(bridge_flag==1)
                       {
                           num_flag++;
                           if(num_flag>5)  
                           { 
                             bridge_flag=0;
                             num_flag=0;
                           }
                       }
                       else   CS_HS();
                     }
                     else if(ZJ_Process_flag&&zj_flag)    //左正转9000  右反转4000  右直角
                     {
                       
                        PWM_OUT_1=9000;  
                        PWM_OUT_2=4000;
                        ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, PWM_OUT_2);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                     }
                     else if(ZJ_Process_flag&&zj_flag_1)    //右正转9000  左反转4000  左直角
                     {
                       
                        PWM_OUT_1=4000;  
                        PWM_OUT_2=9000;
                        ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, PWM_OUT_1);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                     }
                     else 
                     {
                           Speed_Set=310;//250 
                           PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                           PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                     }
                  }
                  if(shizhi_flag)
                  {
                       Speed_Set=250;//250 
                       PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                       PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y); 
                  }
                  if(!ZJ_Process_flag)
                  {
                    //发、停车程序
                    if(count_flag_1==1&&PTD2_IN&&PTD3_IN)
                    { 
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //右
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //左
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                    }
                    else if(count_flag_1==2&&(PTD2_IN==1||PTD3_IN==1)&&PTA12_IN==1)
                    {
                          Speed_Set=250;//停车前减速 
                          PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                          PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y);  
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);  //右
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //左
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                    }   
                    else if(count_flag_1==2&&(PTD2_IN==0&&PTD3_IN==0)&&PTA12_IN==1)
                    { 
                          count_000=count_000+1;
                        
                          if((val_z+val_y)/2<50)
                          {
                              ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //右
                              ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                              ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //左
                              ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                          }
                          else 
                          {
                              ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //右
                              ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 8000);
                              ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 8000);  //左
                              ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                          }
                          if(count_000>50) count_000=0;
                    }
                    else if(count_flag_1==3&&PTA12_IN==1)
                    {
                              ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //右
                              ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                              ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //左
                              ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                    }
                    //发、停车程序
                    else if(angle_pwm>=DJ_zuo-0&&val_z>200&&val_y>200)              //右正转9000  左反转4000
                    {
                          PWM_OUT_1=1000;   
                          PWM_OUT_2=9000;
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);  //右
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, PWM_OUT_1);  //左
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                    }
                    else if(angle_pwm<=DJ_you+0&&val_z>200&&val_y>200)              //左正转9000  右反转4000
                    {
                          PWM_OUT_1=9000;  
                          PWM_OUT_2=1000;
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, PWM_OUT_2);   //右
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);   //左
                        
                    }
                    else
                    {
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                    }
                 }
     }
     
/*高②*/if(PTA13_IN==0&&PTA17_IN==1&&PTA16_IN==1&&PTA15_IN==1&&PTA14_IN==1)
       {
                   if(!shizhi_flag)
                   {
                     if((angle_pwm<DJ_zuo&&angle_pwm>DJ_you)&&! ZJ_Process_flag&&!(count_flag_1==2&&(PTD2_IN==1||PTD3_IN==1)))
                     {
                       if(val_z<300&&val_y<300&&(-25<angle&&angle<25))               //起跑加速
                       {  
                             Speed_Set=550;  
                       }
                       else if(val_z<350&&val_y<350&&(-25<angle&&angle<25))               //起跑加速
                       {
                           Speed_Set=350;  
                       }
                       else if(angle>80||angle<-80)             //弯道
                       {
                           Speed_Set=300;//250;380 
                       }
                       else if((val_z>290||val_y>290)&&(error_4>10.0||error_4<-10.0)&&(angle<80&&angle>-80))      //弯道  不变
                       {
                           Speed_Set=50;  //最低速度 
                       } 
                       else if((val_z<290||val_y<290)&&(error_4>10.0||error_4<-10.0)&&(angle<80&&angle>-80))      //弯道  不变
                       {
                           Speed_Set=220;  //最低速度 
                       } 
                       else                                                         //中速
                       {
                           Speed_Set=330; 
                       }
                       if(bridge_flag==1)                    
                       {
                           Speed_Set=100;
                           PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                           PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y);
                           num_flag++;
                           if(num_flag>8)  
                           { 
                             bridge_flag=0;
                             num_flag=0;
                           }
                       }
                       else    CS_HS_1();
                     }
                     else if(ZJ_Process_flag&&zj_flag)    //左正转9000  右反转4000  右直角
                     {
                       
                        PWM_OUT_1=9000;  
                        PWM_OUT_2=4000;
                        ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, PWM_OUT_2);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                     }
                     else if(ZJ_Process_flag&&zj_flag_1)    //右正转9000  左反转4000  左直角
                     {
                       
                        PWM_OUT_1=4000;  
                        PWM_OUT_2=9000;
                        ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, PWM_OUT_1);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                     }
                     else 
                     {
                           if(!(count_flag_1==2&&(PTD2_IN==1||PTD3_IN==1)))
                           Speed_Set=320;//250 
                           PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                           PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                     }
                  }
                  if(shizhi_flag)
                  {
                       Speed_Set=300;//250 
                       PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                       PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y); 
                  }
                  if(!ZJ_Process_flag)
                  {
                     //发、停车程序
                    if(count_flag_1==1&&PTD2_IN&&PTD3_IN)
                    { 
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //右
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //左
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                    }
                    else if(count_flag_1==2&&(PTD2_IN==1||PTD3_IN==1)&&PTA12_IN==1)
                    {
                          Speed_Set=250;       //停车前减速 
                          PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                          PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y);  
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);  //右
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //左
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                    }   
                    else if(count_flag_1==2&&(PTD2_IN==0&&PTD3_IN==0)&&PTA12_IN==1)
                    { 
                          count_000=count_000+1;
                        
                          if((val_z+val_y)/2<50)
                          {
                              ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //右
                              ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                              ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //左
                              ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                          }
                          else 
                          {
                              ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //右
                              ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 8000);
                              ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 8000);  //左
                              ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                          }
                          if(count_000>50) count_000=0;
                    }
                    else if(count_flag_1==3&&PTA12_IN==1)
                    {
                              ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //右
                              ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                              ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //左
                              ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                    }
                    //发、停车程序
                    else if(angle_pwm>=DJ_zuo-0)              //右正转9000  左反转4000
                    {
                          PWM_OUT_1=2000;   
                          PWM_OUT_2=8000;
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);  //右
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //左
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                    }
                    else if(angle_pwm<=DJ_you+0)              //左正转9000  右反转4000
                    {
                          PWM_OUT_1=8000;  
                          PWM_OUT_2=2000;
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);   //右
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);   //左
                        
                    }
                    else
                    {
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                    }
                 }    
     }
/*高③*/if(PTA17_IN==0&&PTA16_IN==0&&PTA15_IN==1&&PTA14_IN==1&&PTA13_IN==1)
        {
                    if(!shizhi_flag)
                   {
                     if(! ZJ_Process_flag&&!(count_flag_1==2&&(PTD2_IN==1||PTD3_IN==1)))
                     {
                       if(val_z<200&&val_y<200&&(-10<angle&&angle<10))               //起跑加速
                       {
                           //Speed_Set=500;  
                             Speed_Set=550;  
                       }
                       else if(val_z<280&&val_y<280&&(-50<angle&&angle<50))               //起跑加速
                       {
                           Speed_Set=400;  
                       }
                       else if((val_z<270||val_y<270)&&(angle>50||angle<-50))      //弯道
                       {
                           Speed_Set=380;//250;380 
                       }

                       else if((val_z>300&&val_y>300)&&(error_4>10.0||error_4<-10.0))      //弯道  不变
                       {
                           Speed_Set=50;  //最低速度 
                       } 
                       else if((val_z>260&&val_y>260)&&(error_4>10.0||error_4<-10.0))      //弯道   不变
                       {
                           Speed_Set=250;  //最低速度 
                       }
                       else if((val_z>230&&val_y>230)&&(error_4>10.0||error_4<-10.0))      //弯道   不变
                       {
                           Speed_Set=200;  //最低速度 
                       }
                       else                                                         //中速
                       {
                           Speed_Set=360; 
                       }
                       if(bridge_flag==1)
                       {
                           Speed_Set=120;
                           PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                           PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y);
                           num_flag++;
                           if(num_flag>8)  
                           { 
                             bridge_flag=0;
                             num_flag=0;
                           }
                       }
                       else    CS_HS();
                     }
                     else if(ZJ_Process_flag&&zj_flag)    //左正转9000  右反转4000  右直角
                     {
                       
                        PWM_OUT_1=9000;  
                        PWM_OUT_2=4000;
                        ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, PWM_OUT_2);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                     }
                     else if(ZJ_Process_flag&&zj_flag_1)    //右正转9000  左反转4000  左直角
                     {
                       
                        PWM_OUT_1=4000;  
                        PWM_OUT_2=9000;
                        ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, PWM_OUT_1);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                     }
                     else 
                     {
                           if(!(count_flag_1==2&&(PTD2_IN==1||PTD3_IN==1)))   
                           Speed_Set=330;//250 
                           PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                           PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                     }
                  }
                  if(shizhi_flag)
                  {
                       Speed_Set=300;//250 
                       PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                       PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y); 
                  }
                  if(!ZJ_Process_flag)
                  {
                     //发、停车程序
                    if(count_flag_1==1&&PTD2_IN&&PTD3_IN)
                    { 
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //右
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //左
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                    }
                    else if(count_flag_1==2&&(PTD2_IN==1||PTD3_IN==1)&&PTA12_IN==1)
                    {
                          Speed_Set=200;     //停车前减速
                          PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                          PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y);  
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);  //右
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //左
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                    }   
                    else if(count_flag_1==2&&(PTD2_IN==0&&PTD3_IN==0)&&PTA12_IN==1)
                    { 
                          count_000=count_000+1;
                        
                          if((val_z+val_y)/2<50)
                          {
                              ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //右
                              ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                              ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //左
                              ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                          }
                          else 
                          {
                              ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //右
                              ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 8000);
                              ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 8000);  //左
                              ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                          }
                          if(count_000>50) count_000=0;
                    }
                    else if(count_flag_1==3&&PTA12_IN==1)
                    {
                              ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //右
                              ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                              ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //左
                              ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                    }
                    //发、停车程序
                    else if(angle_pwm>=DJ_zuo-0&&val_z>200&&val_y>200)              //右正转9000  左反转4000
                    {
                          PWM_OUT_1=2000;   
                          PWM_OUT_2=8000;
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);  //右
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, PWM_OUT_1);  //左
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                    }
                    else if(angle_pwm<=DJ_you+0&&val_z>200&&val_y>200)              //左正转9000  右反转4000
                    {
                          PWM_OUT_1=8000;  
                          PWM_OUT_2=2000;
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, PWM_OUT_2);   //右
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);   //左
                        
                    }
                    else
                    {
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                    }
                 }    
        }
   }
}

/************************以上速度控制**************************/


//-------------------------------------------------------------------------*
//@brief         示波器函数                                                *
//name：  FreeCars上位机示波器例程                                         *
//author：                                                          *
//Date:   2015-6-26                                                        *
//版权：  此代码为FreeCars上位机协议代码，允许任意使用，                   *
//        也允许用于商业用途，但请保留此段文字！                           *                                                         *
//-------------------------------------------------------------------------*

#define FreeCarsUARTPort  UART3
#define UartDataNum 17
uint8 uSendBuf[UartDataNum*2]={0};
uint8 FreeCarsDataNum=UartDataNum*2;
/*
向某个通道缓冲区填充数据
chanel：通道
data  ：数据-32768~32767
*/
void push(uint8 chanel,uint16 data)
{
    uSendBuf[chanel*2]=data/256;
    uSendBuf[chanel*2+1]=data%256;
}
/*
轮询法发送一帧数据
消耗时间与数据长度有关
不可以放在中断里面周期性调用
消耗时间计算看帮助文档
*/
void sendDataToScope(void)
{
  
    uint8 i,sum=0; 
    //使用轮询的方式发送数据，当数据未发送，程序停在此处直到发送完成
    uart_putchar (UART3,251);
    uart_putchar (UART3,109);
    uart_putchar (UART3,37);
    sum+=(251);      //全部数据加入校验
    sum+=(109);
    sum+=(37);
    for(i=0;i<FreeCarsDataNum;i++)
    {
       uart_putchar(UART3,uSendBuf[i]);
       sum+=uSendBuf[i];         //全部数据加入校验
    }
    uart_putchar (UART3,sum);
}
extern int8 z_z,y_z,D1,z_flag;
extern int16 gyro1; 
void send_to_scope()
{
   //push(0,(uint16)((val_z+val_y)/2));
  // push(1,(uint16)z_z);
  // push(2,(uint16)num_00);
  // push(3,(uint16)num_11);

   // push(4,(uint16)y_z);
   //  push(5,bridge_flag);
   //  push(7,gyro1);
   
   //  sendDataToScope();
}
/*******************以上发送函数**********************************/


/************************************************************
 *  @brief        PID函数
     v5.0
 *   @note      
***********************************************************/
int32 EE0=0;  //当前误差 
int32 EE1=0;  //前次误差
int32 EE2=0;  //前前次的误差
int32 IncPid=0;
float sp=0;
float speed_pid_1(int32 speed1,int32 speed2 ) //speed1=设定的理想速度，speed2=采集回来的反馈速度
{ 

   EE2=EE1;   //存储当前误差，用于下次计算
   EE1=EE0; 
   EE0=speed1-speed2; //增量计算 ，即速度偏差
   IncPid=(int32)(KKi*EE0+KKp*(EE0-EE1)+KKd*(EE0-2*EE1+EE2));   
   sp=sp+IncPid; 
   if(sp>=9500)  
    sp=9500;
   if(sp<0)
    sp=0; //此处给0，会导致异常停车
   return sp ;                                        
}
int32 ee0=0;  //当前误差 
int32 ee1=0;  //前次误差
int32 ee2=0;  //前前次的误差
int32 incpid=0;
float SP=0;
float speed_pid_2(int32 speed1,int32 speed2 ) //speed1=设定的理想速度，speed2=采集回来的反馈速度
 { 
   ee2=ee1;   //存储当前误差，用于下次计算
   ee1=ee0; 
   ee0=speed1-speed2; //增量计算 ，即速度偏差
   incpid=(int32)(KKi*ee0+KKp*(ee0-ee1)+KKd*(ee0-2*ee1+ee2));   
   SP=SP+incpid; 
   if(SP>=9500)  
    SP=9500;
   if(SP<0)
    SP=0; //此处给0，会导致异常停车
   return SP ;                                        
 }
