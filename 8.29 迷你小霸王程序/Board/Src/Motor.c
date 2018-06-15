/*
* @file         Motor.c
  * @brief      电机相关函数实现
  * @author     xiao_fang
  * @version    v1.0
  * @date       2017-3-13
*/
#include "common.h"
#include "Motor.h"
#include "MKL_TPM.h"
#include "Switch.h"

extern  int32 var[5];
/*
 *  @brief      初始化电机
 *  @since      v1.0
 *  精度1000
 *  初始化时电机停转
*/
void motor_init(void)
{
  tpm_pwm_init(TPM2, TPM_CH0, MOTOR_FRE, 0);    //PTA1
  tpm_pwm_init(TPM2, TPM_CH1, MOTOR_FRE, 0);    //PTA2
}


/*!
 *  @brief      设置电机PWM
 *  pwm_duty     舵机PWM值
 *  motor        要设置的电机
 *  @since      v2.0
 */
void motor_pwm_set(int32 pwm_duty)
{
  if(pwm_duty > 0)
  {
      if(pwm_duty > 9999)
      {
          pwm_duty = 9999;
      }
      tpm_pwm_duty(TPM2, TPM_CH0, pwm_duty);
      tpm_pwm_duty(TPM2, TPM_CH1, 0);
  }
  if(pwm_duty < 0)
  {
      if(pwm_duty < -9999)
      {
          pwm_duty = -9999;
      }
      tpm_pwm_duty(TPM2, TPM_CH0, 0);
      tpm_pwm_duty(TPM2, TPM_CH1, ABS(pwm_duty));
  }
  if(pwm_duty == 0)
  {
      tpm_pwm_duty(TPM2, TPM_CH0, 0);
      tpm_pwm_duty(TPM2, TPM_CH1, 0);
  }
}


 

