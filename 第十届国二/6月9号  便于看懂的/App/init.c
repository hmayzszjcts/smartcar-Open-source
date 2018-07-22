/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外初学论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       main.c
 * @brief      山外K60 平台主程序
 * @author     
 * @version    v5.0
 * @date       
 * author    
 */
#include "common.h"
#include "include.h"
#include  "init.h"
#include  "PID.h"
#define    KKi     1.2         // 2.8       // 0.8     //1.5
#define    KKp     0.08      //0.9
#define    KKd      2        //0.4

/*
#define    KKi     0.8
#define    KKp     7
#define    KKd     0   */

//#define    CS_P_1       8
//#define    CS_P_2       8
#define   angle_max       45

uint8    CS_P_1=9,CS_P_2=9;
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
    gpio_init(PTD0,GPI,0);
    port_init_NoALT(PTD0,PULLUP);
    gpio_init(PTD1,GPI,0);
    port_init_NoALT(PTD1,PULLUP);
    
    gpio_init(PTD6,GPI,0);
    port_init_NoALT(PTD6,PULLDOWN);
    
    //gpio_init(PTD7,GPI,0);
    //port_init_NoALT(PTD7,PULLUP);
    

    
    
    gpio_init(PTA12,GPI,0);
    port_init_NoALT(PTA12,PULLUP);
    
    
   // gpio_init(PTA14,GPO,1);
  
   
    
    ftm_quad_init(FTM1);                                     // 初始化FTM 的正交解码 功能
    ftm_quad_init(FTM2);
    ftm_pwm_init(S3010_FTM, S3010_CH,S3010_HZ, DJ_cen);     //初始化 舵机 PWM        
    ftm_pwm_init(MOTOR_FTM, MOTOR1_PWM,MOTOR_HZ,0);      //初始化 电机 PWM
    ftm_pwm_init(MOTOR_FTM, MOTOR2_PWM,MOTOR_HZ,0);      //初始化 电机 PWM
    ftm_pwm_init(MOTOR_FTM, MOTOR3_PWM,MOTOR_HZ,0);      //初始化 电机 PWM
    ftm_pwm_init(MOTOR_FTM, MOTOR4_PWM,MOTOR_HZ,0);      //初始化 电机 PWM  
  
}

/************************************************************
 *  @brief        测速函数
     v5.0
 *   @note      
 ***********************************************************/
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
uint32   error_0;
uint32  PWM_OUT_1, PWM_OUT_2;
int32   angle,     Speed_Set;    //左正右负  
int16    a;
void CS_HS()
{
  
  if(angle>0)   //zuo 弯
  {
     a=(int16)(angle_max*(angle)*1.0/(DJ_zuo-DJ_cen))-1 ;  //angle_max   45
     if(a>45)  a=0;
     if(a<0)   a=0;
     error_0=(uint32)(speed_error[a]*CS_P_1*10);
     PWM_OUT_1=(uint32)speed_pid_1(Speed_Set-error_0/2,val_z);  
     PWM_OUT_2=(uint32)speed_pid_2(Speed_Set+error_0,val_y);  
  }
   if(angle<0)   //you 弯
  {
     a=(int16)(angle_max*(-angle)*1.0/(DJ_cen-DJ_you))-1 ;
     if(a>45)  a=0;
     if(a<0)   a=0;
     error_0=(uint32)(speed_error[a]*CS_P_2*10);
     PWM_OUT_1=(uint32)speed_pid_1(Speed_Set+error_0,val_z);  
     PWM_OUT_2=(uint32)speed_pid_2(Speed_Set-error_0/2,val_y);  
  }
}


////////////////////////////////////////////////////

extern int8    ZJ_Process_flag;
extern int8    y_flag,  z_flag;
extern int32   angle_pwm;  
void Speed_Contorl()
{
      angle=angle_pwm-DJ_cen;
      if(! ZJ_Process_flag)
      {
          if(val_z<100&&val_y<100&&(-20<angle&&angle<20))               //起跑加速
          {
               Speed_Set=600;                                            
          }
          else if((val_z>300||val_y>300)&&(-40<angle&&angle<40))        //直道
          {
               Speed_Set=320;//400; 
          }
          else if((val_z>250||val_y>250)&&(angle>40||angle<-40))       //入弯
          {
               Speed_Set=150;//150; 
          }
          else if((val_z<200||val_y<200)&&(-40<angle&&angle<40))       //出弯
          {
               Speed_Set=300; 
          }
          else if((val_z<200||val_y<200)&&(angle>60||angle<-60))      //弯道
          {
               Speed_Set=200;//250; 
          }
          else                                                         //中速
          {
               Speed_Set=230; 
          }
         CS_HS();
      }
      else if(ZJ_Process_flag&&y_flag)
      {
         Speed_Set=300;
         PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
         PWM_OUT_2=(uint32)speed_pid_2(Speed_Set-400,val_y); 
      }
      else if(ZJ_Process_flag&&z_flag)
      {
         Speed_Set=300;
         PWM_OUT_1=(uint32)speed_pid_1(Speed_Set-400,val_z);  
         PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y); 
      }
      else 
      {
          Speed_Set=230; 
          PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
          PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y); 
      }
      if(tingche_flag==1)
      {
        PWM_OUT_2=0;
        PWM_OUT_1=0;
      }
        ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);
        ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
        ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
        ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
 
}

/************************速度控制**************************/
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
   if(sp>=8800)  
    sp=8800;
   if(sp<10)
    sp=10; //此处给0，会导致异常停车
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
   if(SP>=8800)  
    SP=8800;
   if(SP<10)
    SP=10; //此处给0，会导致异常停车
   return SP ;                                        
 }
