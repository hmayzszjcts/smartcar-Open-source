/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       main.c
 * @brief      山外KL26 平台主程序
 * @author     山外科技
 * @version    v5.2
 * @date       2014-10-26
 */

#include "common.h"
#include "include.h"

extern uint16 sensor_value[6];
extern uint16 normalization_threshold[6];
extern float position_accumulative[15];


int32 var[5];                                           //发送到上位机的数据
float position = 0.0f;                                 //位置偏差
float pwm = 0.0f;                                      //舵机占空比增量
uint32 pwm_duty_now = 0;                                //舵机占空比
uint8 loss_line = 0;                                    //丢线标志位
uint8 loss_line_lock =0;                                //丢线锁
uint8 circle_sign = 0;                                  //圆环标志
uint8 ramp_sign = 0;                                    //坡道标志
uint8 stop_car_sign = 0;                                //停车标志

uint16 motor_speed = 0;                            //当前电机速度
int16 speed_set = 250;                            //电机目标速度
uint8 speed_rank = 0;                            //速度等级


void pit2IRQHandler(void);                             //定时器中断函数声明


/*!
 *  @brief      main函数
 *  @since      v5.2
 */
void main()
{
    int16 gx, gy, gz,ax,ay,az; 
//    Sensor_init();
//    Gyroscope_init();
//    servo_motor_init();
//    switch_button_init();
//    motor_init();
//    encoder_init();
//    Servo_pid_init();
//    flash_24c02_init();
//    Senser_normalization_threshold_get();
//    Senser_normalization_threshold_read(normalization_threshold);       //读取归一化阀值
//    speed_stall_select();               //速度档位选择
//    NVIC_SetPriority(PORTA_IRQn,0);                     //为了防止漏过停车线，将停车用的外部中断定为最高优先级
//    NVIC_SetPriority(PIT_IRQn,1);
//    pit_init_ms(PIT0,5);                                //初始化定时器中断，10ms
//    set_vector_handler(PIT_VECTORn , pit2IRQHandler);   //初始化定时器中断，10ms
//    enable_irq(PIT_IRQn);
    MPU_Init();
    while(1)
    {
        systick_delay_ms(20);
        MPU_Get_Gyroscope(&gx,&gy,&gz);
        MPU_Get_Accelerometer(&ax,&ay,&az);
        printf("%5d   %5d   %5d    %5d    %5d    %5d    %5d\r\n",gx ,gy ,gz,ax,ay,az,MPU_Get_Temperature());
    }
}

void pit2IRQHandler(void)
{
      encoder_get(&motor_speed);
      reed_detection();         //干簧管停车检测
      Sensor_value_get(sensor_value);
      Senser_normalization(sensor_value);
//      printf("%5d %5d %5d %5d %5d %5d\r\n",sensor_value[0],sensor_value[1],sensor_value[2],sensor_value[3],sensor_value[4],sensor_value[5]);
//      printf("%5d  %d\r\n",sensor_value[0]+sensor_value[2]+sensor_value[3]+sensor_value[5],(int32)(position * 100));      
      position = cal_deviation(sensor_value);
      position = position_filter(position);
      pwm = Servo_pid_cal(position);
      ramp_sign = ramp_deal(sensor_value,position,&motor_speed);                     //坡道处理
      if(ramp_sign == 0)                                                //在坡道上不进行圆环处理
      {
          circle_sign = circle_deal(sensor_value,position,&motor_speed);             //圆环检测
      } 
      /***圆环处理***/
     if(circle_sign == 0)    //没有检测到圆环，丢线处理原值
       {
            if(ramp_sign == 0)
            {
                loss_line_deal(sensor_value , position , LEFT_LOSE_LINE_THRESHOLD , RIGHT_LOSE_LINE_THRESHOLD);
            }            
        }
      else if(circle_sign == 2)    //如果车已经入圆，使用圆内的丢线处理
      {
          loss_line_deal(sensor_value , position , LEFT_LOSE_LINE_THRESHOLD - 10 , RIGHT_LOSE_LINE_THRESHOLD - 10);
       }
      switch(loss_line)
      {
          case 0:
              pwm_duty_now = servo_motor_pwm_set(SERVO_MID - (int)(pwm)); 
              break;
              
          case 1:
              if(circle_sign == 2)
              {
                  if(((switch_read())&(0x40)) == 0)
                  {
                      position = POSITION_BOUND;
                      pwm_duty_now = servo_motor_pwm_set(SERVO_RIGHT_DEAD_ZONE);
                  }
                  else
                  {
                      position = (-(POSITION_BOUND));
                      pwm_duty_now = servo_motor_pwm_set(SERVO_LEFT_DEAD_ZONE);
                  }
              }
              else
              {
                  loss_line_lock = 1;
                  position = POSITION_BOUND;
                  pwm_duty_now = servo_motor_pwm_set(SERVO_RIGHT_DEAD_ZONE);
              }
              break;
              
          case 2:
              if(circle_sign == 2)
              {
                  if(((switch_read())&(0x40)) == 0)
                  {
                      position = POSITION_BOUND;
                      pwm_duty_now = servo_motor_pwm_set(SERVO_RIGHT_DEAD_ZONE);
                  }
                  else
                  {
                      position = (-(POSITION_BOUND));
                      pwm_duty_now = servo_motor_pwm_set(SERVO_LEFT_DEAD_ZONE);                     
                  }
              }
              else
              {
                  loss_line_lock = 1;
                  position = (-(POSITION_BOUND));
                  pwm_duty_now = servo_motor_pwm_set(SERVO_LEFT_DEAD_ZONE);
              }
              break;
      }
      if((((switch_read()) & (0x80)) == 0) | (stop_car_sign == 1))
      {
          motor_pwm_set(0);
      }
      else
      {
          motor_pwm_set((int32)speed_set);
      }         
//      vcan_scope((uint8_t *)var,sizeof(var));         //虚拟示波器发送数据
      PIT_Flag_Clear(PIT0); 
}
