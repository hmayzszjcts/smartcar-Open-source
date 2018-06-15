/*
* @file         Reed.c
  * @brief      干簧管相关函数实现
  * @author     xiao_fang
  * @version    v1.0
  * @date       2017-5-23
*/
#include "common.h"
#include "Reed.h"
#include "Buzzer.h"
#include "MKL_gpio.h"
#include "Motor.h"

extern uint8 stop_car_sign;

/*
 *  @brief      干簧管停车检测
 *  @since      v1.0
*/
void reed_detection(void)
{
    static uint16 stop_lenth_count;
    static uint8 detection_enable_sign;
    if(detection_enable_sign == 0)
    {
        stop_lenth_count += 1;
        if(stop_lenth_count > 200)         //车越过发车线以后才开始检测停车
      {
            port_init (PTA13, IRQ_FALLING | PF | ALT1);         //初始化PTA13引脚
            set_vector_handler(PORTA_VECTORn , portaIRQHandler);        //初始化外部中断
            enable_irq(PORTA_IRQn);
            detection_enable_sign = 1;
      }
    }
}

/*
 *  @brief      干簧管外部中断
 *  @since      v1.0
*/
void portaIRQHandler(void)              //负责停车的外部中断
{
    PORTA_ISFR  = (1 << 13);            //写1清中断标志位
    systick_delay_ms(1);
    if(gpio_get(PTA13) == 0)
    {
        BEEP_ON;
        motor_pwm_set(0);               //检测到停车后关闭电机
        stop_car_sign = 1;              //停车标志位置位
    }
    
}