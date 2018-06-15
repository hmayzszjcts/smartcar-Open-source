/*
* @file       Servo motor control.h
  * @brief      舵机控制相关函数实现
  * @author     xiao_fang
  * @version    v1.0
  * @date       2017-3-16
*/
#ifndef __SERVO_MOTOR_CONTROL_H__
#define __SERVO_MOTOR_CONTROL_H__



void Servo_pid_init(void);
float Servo_pid_cal(float position);            //舵机PID控制
uint8 circle_deal(uint16 * sensor_value , float position, uint16 * motor_speed);         //圆环处理
uint8 ramp_deal(uint16 * sensor_value , float position , uint16 * motor_speed);          //坡道处理
#endif