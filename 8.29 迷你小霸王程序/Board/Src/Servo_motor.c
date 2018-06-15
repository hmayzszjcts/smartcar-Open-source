/*
  * @file       Servo motor.c
  * @brief      舵机相关函数实现
  * @author     xiao_fang
  * @version    v1.0
  * @date       2017-3-12
*/

#include "common.h"
#include "Servo_motor.h"
#include  "MKL_TPM.h"


/*
 *  @brief      初始化舵机
 *  @since      v1.0
 *  舵机接在PTA12，频率50~330HZ，精度10000
 *  初始化时舵机对中
*/
void servo_motor_init(void)
{
  tpm_pwm_init( TPM1, TPM_CH0, SERVO_FRE, SERVO_MID);
  
}

/*!
 *  @brief      设置舵机PWM
 *  @param      pwm_duty        舵机PWM值
 *  @since      v2.0
 */
uint32 servo_motor_pwm_set(uint32 pwm_duty)
{
  if(pwm_duty < SERVO_RIGHT_DEAD_ZONE)
  {
    pwm_duty = SERVO_RIGHT_DEAD_ZONE;
  }
  if(pwm_duty > SERVO_LEFT_DEAD_ZONE)
  {
    pwm_duty = SERVO_LEFT_DEAD_ZONE;
  }
  tpm_pwm_duty( TPM1, TPM_CH0, pwm_duty);
  return pwm_duty;
}