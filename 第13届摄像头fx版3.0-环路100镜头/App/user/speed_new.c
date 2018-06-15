#include "common.h"
#include "include.h"
#include "speed_new.h"
#include  "info_deal_save.h"
#include "control.h"
#include "usart_file.h"
#include "direction.h"
#include "ring.h"
#include "path.h"
#include "math.h"
#include "my_cfg.h"
#include "roundadout.h"
struct motor pidl={0};
struct motor pidr={0};

/*************************
电机PID初始化
*************************/
void PID_init(void)
{
	//左电机PID初始化
	pidl.P = speed_P;
	pidl.I = speed_I;
	pidl.D = speed_D;
}
void MOTOR_init()
{
	PID_init();
	//电机以及舵机初始化
	ftm_pwm_init(MOTOR_FTM, MOTOR1_PWM,MOTOR_HZ,0);      //初始化 电机 PWM
	ftm_pwm_init(MOTOR_FTM, FTM_CH7,MOTOR_HZ,0);      //初始化 电机 PWM
	ftm_quad_init(FTM1);                                 //FTM1 正交解码初始化（所用的管脚可查 port_cfg.h ）
}
/******************************
        获取脉冲数
********************************/
void Getpluse(void)
{
	pidl.pluse = -ftm_quad_get(FTM1);    //获取FTM1 正交解码 的脉冲数A8 A9
	ftm_quad_clean(FTM1);               //清空脉冲计数器计算值（马上清空，这样才能保证计数值准确）
}

/********************************
    电机PID计算
**********************************/
void motorPID111()
{
    int16 error0,error1,error2;
    float P,I,D;

    error0 = error1 = error2 = 0;
    error0 = pidl.setPoint - (pidl.pluse);//I    
    error1 = error0 - pidl.lastError;//P
    error2 = error0 - 2*pidl.lastError + pidl.preError;//D
    pidl.preError = pidl.lastError;
    pidl.lastError = error0;
  
    P=pidl.P;    I=pidl.I;      D=pidl.D;
    pidl.PWM+= (int16)(P*error1 + I*error0 + D*error2);    
    //if(error0>60) pidl.PWM = 9999;
    //if(error0<-35) pidl.PWM = -9999;

    
    
    if(pidl.PWM > 9999)  pidl.PWM = 9999;
    if(pidl.PWM < -9999   )  pidl.PWM = -9999;

}

void motor_out()//1 3 方向
{       
  
   if(pidl.PWM >= 0)//
   {
       M1(pidl.PWM);//D5
       M2(10000);//D7
   }
   else
   {   
      M1(-pidl.PWM);
      M2(0); 
   }   
    
}




/********************************
    设置电机目标输出
*********************************/
int Vspeed=0;
int more_speed=0;
/*
速度控制方案：
max_sp    最大速度
min_sp    最小速度
e         不同公式用于不同误差
way_num   方案编号
*/
int speed_way(int max_sp,int min_sp,int diff)
{

    int more_speed=0;
//////////////  /**********************根据误差计算速度***********************************************/
//////////////    //公式=最高速度-误差的平方*速度差/最大误差的平方  得到的就是期望值速度
//////////////    //           more_speed=((abs(even_diff*even_diff))*(max_speed-min_speed))/(img_max_error*img_max_error);
//////////////    //           Vspeed=(int)(max_speed - more_speed);//用2次方程调参
//////////////  /***********************根据有效行计算速度******************************************/
////////////////             more_speed=((abs(img_error_v*img_error_v))*(max_speed-min_speed))/(900);//33*33
////////////////             Vspeed=(int)(max_speed - more_speed);//用2次方程调参
//////////////
////////////////      if(zhidao_count_flag==2)//直道
////////////////     {
////////////////         max_sp=L_zhidao_speed;
////////////////         min_sp=L_zhidao_speed-5;
////////////////     }
////////////////     else if(zhidao_count_flag==1)//短直道
////////////////     {
////////////////          max_sp=L_zhidao_speed-20;
////////////////          min_sp=L_zhidao_speed-30;
////////////////     }
////////////////     more_speed=((abs(even_diff*even_diff))*(max_sp-min_sp))/(33*33);//33是图像的最大误差,
////////////////     Vspeed=(int)(max_sp - more_speed);//用2次方程调参
////////////////    
////////////////    return Vspeed;
//      if(zhidao_count_flag==2)//直道
//     {
//        max_sp=L_zhidao_speed;
//        min_sp=L_zhidao_speed-5;
//         Vspeed=L_zhidao_speed;
//           Vspeed=180;   //218  180
//     }
//    else if(zhidao_count_flag==1)//短直道
//     {
//          max_sp=L_zhidao_speed-20;
//         min_sp=L_zhidao_speed-30;
//         Vspeed=L_zhidao_speed-30;
//          Vspeed=150;   //188  160
//
//    }
////     more_speed=((abs(even_diff*even_diff))*(max_sp-min_sp))/(33*33);//33是图像的最大误差,
////     Vspeed=(int)(max_sp - more_speed);//用2次方程调参
//    else
//     {
//        Vspeed=112;//100
//     }
    if(gl_zhangai_flag)//障碍标志
    {
//        Vspeed=CD_speed-30; //95  
       Vspeed=80; //95  
    } 
    else if(ring_left_flag||ring_right_flag)//环岛优先级最高
    {
         Vspeed=90;//环路速度不变  100很稳  10圈没问题（72米赛道  3个环路）
    }
    else  if(zhidao_count_flag==2)//直道
    {
//        Vspeed=180;
       Vspeed=L_zhidao_speed; //长直道速度       
    }
    else if(zhidao_count_flag==1)//短直道
    {
//         Vspeed=L_zhidao_speed-20; //短直道速度 zhidao_speed 
          Vspeed=zhidao_speed; //短直道速度 zhidao_speed 
    }
     else
    {
        Vspeed=CD_speed;//全程速度（最低速度）
    }
   
    return Vspeed;

}

int delay_stop_car=20;
void set_ideal_speed(int16 error)
{
    int max_speed=zhidao_speed,min_speed=CD_speed;
    int speed_diff_mark=0;//误差限速
    int img_max_error=33;//图像最大误差
    int img_error_v=57-valid_line;
    if(img_error_v>img_max_error)
    {
        img_error_v=img_max_error;
    }
    Vspeed=speed_way(max_speed,min_speed,valid_line); 

    if(stop_car_flag==1)//停车标志位
    {
        zhidao_speed=0;
        CD_speed=0;
        Vspeed=0;
  
    }    
    if((stop_car_flag==1))
    {
      delay_stop_car--;   
      if(delay_stop_car<1)
      {
        delay_stop_car=0;    
      }
      if(!(delay_stop_car))
      {
        max_speed=zhidao_speed=0;
        min_speed=CD_speed=0; 
      }     
    }
//	if(ramp_flag==1&&(abs(even_diff-0)<=2))//坡道标志
//	{
//		max_speed=CD_speed;//全局最低速度
//		min_speed=CD_speed;//全局最低速度
//	}
//	if(gl_zhangai_flag)//障碍标志
//	{
//	    max_speed=CD_speed-10;
//	    min_speed=CD_speed-10;
//	}                                               


     pidl.setPoint=(int16)Vspeed;

}


void control_speed()
{
    set_ideal_speed(jiaodu_num);
    Getpluse();
    motorPID111();
    motor_out();

}
