#ifndef _CONTROL_H
#define _CONTROL_H

#define MOTOR_FTM   FTM0
#define MOTOR1_PWM  FTM_CH3
#define MOTOR2_PWM  FTM_CH4
#define MOTOR3_PWM  FTM_CH5
#define MOTOR4_PWM  FTM_CH6

#define MOTOR_HZ   10*1000//10KHZ

#define M1(duty)   ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM,duty)
#define M2(duty)   ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM,duty)
#define M3(duty)   ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM,duty)
#define M4(duty)   ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM,duty)

#define S3010_FTM   FTM3
#define S3010_CH    FTM_CH0
#define S3010_HZ    (50)

#define LeftMaxAngle  640      //右转最小 675 1.35 需要根据实际舵机打死PWM进行测试
#define RightMaxAngle 810      //左转最大 825 1.65

struct s3010
{
  uint16 P;
  uint16 D;
  int32  error;
  int32  errorlast;
  uint32 angle;
};

struct motor
{
  int16 pluse;
  int16 setPoint;
  uint16 P;
  uint16 I;
  uint16 D;
  int16 lastError;
  int16 preError;
  int16 out;
  int16 PWM;
};


void PID_init(void);

void direction_Control(void);

void S3010Angle(struct TSL1401 *ccd);        //计算舵机角度
/**小圆环舵机限幅，避免出弯不正，可选用，
   赛道中所有十字后都是单纯的小圆环可以使用，
   十字后有其他情况不行，如大圆环或者直道**/
void S3010AngleL(struct TSL1401 *ccd);       //计算舵机角度
void S3010AngleR(struct TSL1401 *ccd);       //计算舵机角度

void S3010AngleOB(struct TSL1401 *ccd,int8 a);        //计算舵机角度

void Getpluse(void);

void SetPoint(struct TSL1401 *ccd,int16 all);
void SetPointLow(struct TSL1401 *ccd,int16 all);
void SetPointLL(struct TSL1401 *ccd,int16 all);

void speed_Contorl(void);

void motorPID();      //正常PID

void motorPID111();  //带反转ＰＩＤ

#endif