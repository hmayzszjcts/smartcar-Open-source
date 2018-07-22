/******************************************/
/*
  1、舵机小于中值时向右打角
  2、舵机大于中值时向左打角
  3、差速按照实际的内外侧轮的速度差值16PI计算
  4、计算公式  外轮目标速度加速=内轮速度*(1+0.8*tan(内轮可打的最大角度*(实际舵机输出pwm-舵机打直时的PWM输出)/舵机PWM从中值到最大值的差值))（向左打角时的差速情况）
  5、tan为单精度浮点型
*/
/******************************************/
#ifndef _CHASU_H
#define _CHASU_H


#define Speed_duty              //设定速度
#define Speed_R                 //右轮的设定速度
#define Speed_L                 //左轮的设定速度
#define Speed_R_Now             //右轮的实际速度
#define Speed_L_Now             //左轮的实际速度
#define angle_max             45      //内轮可打的最大角度
#define angle_pwm_center      1482    //舵机打直时的PWM输出
#define angle_pwm_error       200     //舵机PWM从中值到最大值的差值
#define angle_pwm_l           1502    //向左打角时后轮开始差速的阀值
#define angle_pwm_r           1462    //向右打角时后轮开始差速的阀值

void CHASU(void);              //初始化函数

#endif  /*  _CHASU_H  */