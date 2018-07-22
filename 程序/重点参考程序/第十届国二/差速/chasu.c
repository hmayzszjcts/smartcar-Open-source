/******************************************/
/*
  1、舵机小于中值时向右打角
  2、舵机大于中值时向左打角
  3、差速按照实际的内外侧轮的速度差值16PI计算
  4、计算公式  外轮目标速度加速=内轮速度*(1+0.8*tan(内轮可打的最大角度*(实际舵机输出pwm-舵机打直时的PWM输出)/舵机PWM从中值到最大值的差值))（向左打角时的差速情况）
  5、tan为单精度浮点型
*/
/******************************************/
#include "chasu.h"
#include "math.h"

void CHASU()
{
    if(angle_pwm>=angle_pwm_r && angle_pwm=<angle_pwm_l)  //当舵机打的角度较小时 不进行差速
    {
        Speed_R=Speed_L=Speed_duty ;
    }
    else if(angle_pwm>angle_pwm_l)    //舵机的打角大于了中值时为向左转  此时右侧为外轮  左侧为内轮
    {
        Speed_L=Speed_duty ;
        Speed_R=Speed_L_Now*(1+0.8*tan(angle_max*(angle_pwm-angle_pwm_center)/angle_pwm_error)) ;
		                                              a=(int)(angle_max*(angle_pwm-angle_pwm_center)/angle_pwm_error) ;
													  error= shuzu [a]*p;
												r=	   error+setspeed;
												L=   setspeed;
    }
    else if(angle_pwm<angle_pwm_r)    //舵机打的打角小于中值时为向右转  此时右侧为内轮  左侧为外轮
    {
        Speed_R=Speed_duty ;
        Speed_L=Speed_R_Now*(1+0.8*tan(angle_max*(angle_pwm_center-angle_pwm)/angle_pwm_error)) ;
    }
}