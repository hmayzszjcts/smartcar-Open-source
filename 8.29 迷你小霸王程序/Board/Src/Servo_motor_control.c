/*
  * @file       Servo motor control.c
  * @brief      舵机控制相关函数实现
  * @author     xiao_fang
  * @version    v1.0
  * @date       2017-3-16
*/
#include "common.h"
#include "Servo_motor_control.h"
#include "Servo_motor.h"
#include "Senser.h"
#include "Switch.h"
#include "MKL_gpio.h"
#include "Motor.h"
#include "Buzzer.h"
#include "Fuzzy.h"
#include "OLED.h"
#include "VCAN_LED.H"
#include "Gyroscope.h"

extern uint16 sensor_value[6];
extern uint8 loss_line;
extern int16 speed_set;
extern int32 var[5];
extern float position_accumulative[15];
extern uint8 speed_rank;

extern float fuzzy_kp;          //模糊计算出的结果，KP值
extern float fuzzy_kd;          //模糊计算出的结果，KD值

struct SERVO_PID_MESSAGE              //舵机PID参数
{
	float error_now;
	float error_last;
        float error_deviation;
        float fuzzy_kp_gain;
        float fuzzy_kd_gain;
	float kp;
        float kd; 
	float output;
} Servo_pid;

/*
 *  @brief      舵机PID初始化
 *  @since      v1.0
*/
void Servo_pid_init(void)
{
     Servo_pid.error_now  = 0.0f;
     Servo_pid.error_last = 0.0f;
     Servo_pid.error_deviation = 0.0f;         //本次偏差减上次偏差的结果
     Servo_pid.kp = 0.0f;
     Servo_pid.kd = 0.0f;
     /*速度6500*/
     Servo_pid.fuzzy_kp_gain = 15.0f;       //模糊P要乘的比例系数
     Servo_pid.fuzzy_kd_gain = 17.0f;       //模糊D要乘的比例系数     
}

/*
 *  @brief      舵机二次函数PD控制
 *  @since      v1.0
 *  position    当前偏差
 *  pid.output  输出给舵机的占空比
*/
float Servo_pid_cal(float position)
{
	Servo_pid.error_now = position;		//计算出实际值和目标值之间的差值
        Servo_pid.error_deviation = Servo_pid.error_now - Servo_pid.error_last;         //计算本次偏差和上次偏差之间的差值
        KP_Fuzzy(Servo_pid.error_now , Servo_pid.error_deviation);      //模糊计算
        
        Servo_pid.kp = fuzzy_kp * Servo_pid.fuzzy_kp_gain;      //模糊计算的结果乘以比例系数作为最终的KP值
        Servo_pid.kd = fuzzy_kd * Servo_pid.fuzzy_kd_gain;      //模糊计算的结果乘以比例系数作为最终的Kd值
        Servo_pid.output = (Servo_pid.kp * Servo_pid.error_now) + (Servo_pid.kd * Servo_pid.error_deviation);
       
        Servo_pid.error_last = Servo_pid.error_now;
               
	return Servo_pid.output;		//返回实际控制值 
}

/*
 *  @brief      圆环处理
 *  @note       实验函数，效果未知
 *  @since      v1.0
 *　position       偏差值
*/
uint8 circle_deal(uint16 * sensor_value , float position , uint16 * motor_speed)
{
    uint16 sensor_value_add[3];         
    uint16 sensor_value_minus;
    static uint8 circle_flag;                                             //flag=0是没有检测到圆，1是检测到圆但未入圆，2是已经入圆
    static uint16 circle_count;
    sensor_value_add[0] = sensor_value[0] + sensor_value [2] + sensor_value[3] + sensor_value[5];
    sensor_value_add[1] = sensor_value[0] + sensor_value[2];
    sensor_value_add[2] = sensor_value[3] + sensor_value[5];
    sensor_value_minus = ABS(sensor_value_add[1] - sensor_value_add[2]);
//    printf("%d %d\r\n",sensor_value_add[0],sensor_value_minus);
//    printf("%d %d %d %d\r\n",sensor_value[0],sensor_value[2],sensor_value[3],sensor_value[5]);
     if(circle_flag == 0)                                                    //如果还未检测到圆
     {
         if(ABS(position)<50)
         {
             if((sensor_value_add[0] < 150) && (sensor_value_add[0] > 130))
            {
                if(sensor_value_minus < 30)
                {
                    circle_flag = 1;           //如果传感器值符合入圆条件，就将标志位设为检测到圆但未入圆
                    led(LED0,LED_ON);
                    speed_set = -1500;
                }
            }

         }
         
    }
     
     if(circle_flag == 1)                      //如果检测到圆但未入圆
     { 
         circle_count += *motor_speed;
         printf("%d\r\n",*motor_speed);
         if(((switch_read()) & (0x40)) == 0)   //根据拨码开关选择打角方向
         {
             
//              if(circle_count < 6500)       //当偏差符合这个条件就说明车未进圆
              if(ABS(sensor_value_minus) < 20)
              {
                   servo_motor_pwm_set(SERVO_LEFT_DEAD_ZONE);        //左打死
                   
              }
              else
              {
                  circle_flag = 2;                                          //车已进圆
                  circle_count = 0;
                  led(LED0,LED_OFF);
                  led(LED1,LED_ON);
                  switch(speed_rank)
                  {
                       case 0:
                           speed_set = 7000;
                           break;
                       case 1:
                           speed_set = 8000;
                           break;
                       case 2:
                           speed_set = 9000;
                           break;
                       case 3:
                           speed_set = 10000;
                           break;
                  }
              }
         }
         else
         {
//             if(circle_count < 6500)       //当偏差符合这个条件就说明车未进圆
             if(ABS(sensor_value_minus) < 20)
             {
                 servo_motor_pwm_set(SERVO_RIGHT_DEAD_ZONE);        //右打死
             }
             else
             {
                 circle_flag = 2;                  //车已进圆
                 circle_count = 0;
                 led(LED0,LED_OFF);
                 led(LED1,LED_ON);
                 switch(speed_rank)
                 {
                     case 0:
                           speed_set = 7000;
                           break;
                       case 1:
                           speed_set = 8000;
                           break;
                       case 2:
                           speed_set = 9000;
                           break;
                       case 3:
                           speed_set = 10000;
                           break;
                 }
             }
         }
     }
    if(circle_flag == 2)                                                
     {
         if(sensor_value_add[0] > 200)      
         {
              if(sensor_value_minus < 30 ) 
              {
                    circle_flag = 0; 
                    led(LED1,LED_OFF);
              }
         }        
     } 
    return circle_flag;                                                   //将当前车的状态返回              
}


/*
 *  @brief      坡道处理
 *  @note       实验函数，效果未知
 *  @since      v1.0
 *　position       偏差值
*/
uint8 ramp_deal(uint16 * sensor_value , float position, uint16 * motor_speed)
{
    static uint8 ramp_detect_sign;                                      //是否在坡上的标志位
    static uint32 ramp_count;
    static uint8 ramp_flag;                                             //flag=0是没有检测到坡，1是检测到坡，2是已上坡
     if(ramp_flag == 0)                                                    //如果还未检测到坡
     {
         
         if(position < 50)
         {
             if((sensor_value[2] > 110) && (sensor_value[3] > 110))
             {
                  ramp_flag = 1;           //如果传感器值符合坡道条件，就将标志位设为1
                  ramp_detect_sign = 1;
                  led(LED0,LED_ON);
                  led(LED1,LED_ON);
             }
         } 
    }
    
    if(ramp_flag == 1)                      //如果检测到已上坡
     {
        ramp_count += *motor_speed;
        if(ramp_count > 30000)                //延时防止误测到下坡
        {
                led(LED0,LED_OFF);
                led(LED1,LED_OFF);
                ramp_flag = 0;
                ramp_detect_sign = 0;
                ramp_count = 0;
        }
     }      
    return ramp_detect_sign;                                                   //将当前车的状态返回              
}