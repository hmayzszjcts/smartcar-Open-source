#ifndef  _INIT_H
#define  _INIT_H  

#define MOTOR_FTM   FTM0
#define MOTOR1_PWM  FTM_CH1    
#define MOTOR2_PWM  FTM_CH2
#define MOTOR3_PWM  FTM_CH3
#define MOTOR4_PWM  FTM_CH4

#define S3010_FTM   FTM3
#define S3010_CH    FTM_CH5
#define S3010_HZ    60//100
#define  MOTOR_HZ   20*1000    //(20*1000)

//舵机中值：    880
//左打死  :     990
//右打死：      775
//舵机中值：    845
//左打死  :     953
//右打死：      736
//舵机中值：    847
//左打死  :     958
//右打死：      736
//舵机中值：    845
//左打死  :     953
//右打死：      736
#define    DJ_cen      845
#define    DJ_zuo      953
#define    DJ_you      736
typedef struct
{
    uint16 m;
    uint16 n;
}SW_t;
extern void FTM_init();
extern void get_speed();
extern void Speed_Contorl();
extern void  JianChe();
extern float speed_pid_1(int32 speed1,int32 speed2 ); //speed1=设定的理想速度，speed2=采集回来的反馈速度
extern float speed_pid_2(int32 speed1,int32 speed2 ); //speed1=设定的理想速度，speed2=采集回来的反馈速度
extern void send_to_scope();
extern void  DangWei_();
#endif
