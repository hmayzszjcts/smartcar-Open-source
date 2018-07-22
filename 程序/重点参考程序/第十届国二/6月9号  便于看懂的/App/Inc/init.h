#ifndef  _INIT_H
#define  _INIT_H  

#define MOTOR_FTM   FTM0
#define MOTOR1_PWM  FTM_CH1      
#define MOTOR2_PWM  FTM_CH2
#define MOTOR3_PWM  FTM_CH3
#define MOTOR4_PWM  FTM_CH4
#define S3010_FTM   FTM2
#define S3010_CH    FTM_CH0
#define S3010_HZ    100
#define  MOTOR_HZ   20*1000    //(20*1000)

extern void FTM_init();
extern void CESU();
extern void dianji_pid();



#endif
