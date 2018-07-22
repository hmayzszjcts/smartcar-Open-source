#include "common.h"
#include "include.h"

extern struct TSL1401 ccd1;

struct s3010 spid;
struct motor pidl;
struct motor pidr;

extern uint32 dial_ccd,ramp_straight,speed_dial;  //记录拨码开关数值

int16 speed,diff_speed;
uint8 brake_flag;

extern int16 a,b,c,d,g;    //速度设定参数，以及刹车系数

uint8 qqqqq;

/*************************
  舵机、电机PID初始化
*************************/
void PID_init(void)
{
  diff_speed = 53;//差速因子
  //直道速度设定
  pidl.setPoint = pidr.setPoint = speed = 180;
  //舵机PID初始化
  spid.P = 89;
  spid.D = 300;
  spid.angle = 725;
  //左电机PID初始化
  pidl.PWM = 0;
  pidl.P = 25;
  pidl.I = 95;
  pidl.D = 0;
  //右电机PID初始化
  pidr.PWM = 0;
  pidr.P = 25;
  pidr.I = 95;
  pidr.D = 0;
  
  qqqqq = 13;
}
//方向和速度控制
extern uint8 cross_FLAG;
/**************************
      方 向 控 制
***************************/
extern uint8 ramp;//坡道值
void direction_Control(void)
{
  
  uint8 error_flag;
  if(ccd1.flag_edge[0] == 10)
  {
    if(ccd1.mid_Line[0] > (63+qqqqq) ) error_flag=0;
    else
    {
      if(ccd1.flag_edge[1] == 10 && ccd1.flag_edge[2]==10 && ccd1.flag_edge[3]==10
         && ccd1.flag_edge[4]==10 && ccd1.flag_edge[5]==10 && ccd1.flag_edge[6]==10
         && ccd1.flag_edge[7]==10 && ccd1.flag_edge[8]==10 && ccd1.flag_edge[9]==10
         && ccd1.cross[4]==0&& ccd1.cross[5]==0&& ccd1.cross[6]==0
         && ccd1.cross[7]==0&& ccd1.cross[8]==0&& ccd1.cross[9]==0
         && ccd1.cross[10]==0&& ccd1.cross[11]==0&& ccd1.cross[12]==0
         && ccd1.cross[13]==0&& ccd1.cross[14]==0&& ccd1.cross[15]==0
         && ccd1.cross[16]==0&& ccd1.cross[17]==0&& ccd1.cross[18]==0
           )
      {
        error_flag=10;
      }
      else
      {
        error_flag=0;
      }
    }
  }
  else if(ccd1.flag_edge[0] == 1)
  {
    if(ccd1.mid_Line[0] < (63-qqqqq) ) error_flag=0;
    else
    {
      if(ccd1.flag_edge[1] == 1 && ccd1.flag_edge[2]==1 && ccd1.flag_edge[3]==1
         && ccd1.flag_edge[4]==1 && ccd1.flag_edge[5]==1 && ccd1.flag_edge[6]==1
         && ccd1.flag_edge[7]==1 && ccd1.flag_edge[8]==1 && ccd1.flag_edge[9]==1
         && ccd1.cross[4]==0&& ccd1.cross[5]==0&& ccd1.cross[6]==0
         && ccd1.cross[7]==0&& ccd1.cross[8]==0&& ccd1.cross[9]==0
         && ccd1.cross[10]==0&& ccd1.cross[11]==0&& ccd1.cross[12]==0
         && ccd1.cross[13]==0&& ccd1.cross[14]==0&& ccd1.cross[15]==0
         && ccd1.cross[16]==0&& ccd1.cross[17]==0&& ccd1.cross[18]==0
           )
      {
        error_flag=1;
      }
      else
      {
        error_flag=0;
      }
    }
  }
  else 
  {
    error_flag=0;
  }
  
  if( (ramp > 50 && ramp < 95) && ramp_straight==1 ) //舵机坡道打直，可选择
  {
    led(LED_MAX,LED_ON);
    ftm_pwm_duty(S3010_FTM, S3010_CH,725);  
  }
  else
  {
    if(error_flag == 1)
    {
      //FENG = 0;
      led(LED_MAX,LED_ON);
      S3010AngleL(&ccd1);
    }
    else if(error_flag == 10)
    {
      //FENG = 0;
      led(LED_MAX,LED_ON);
      S3010AngleR(&ccd1);
    }
    else
    {
      //FENG = 1;
      led(LED_MAX,LED_OFF);
      S3010Angle(&ccd1);
    }
  }
}

/************************************************************************
              速  度  控  制
  if bend road,look last N times and speed now
  {
    if N times are straight and speed now is too fast,need speed cut
    else if speed now is normal,don't need speed cut,normal contorl speed
  }
  else if straight,look N times and speed now
  {
    if N times are bend road and speed now are too low,need speed up
    else if speed now is normal,don't need speed cut,normal contorl speed
  }
  else if 圆环
**************************************************************************/

int16 Cp,Kp,Vspeed;

void speed_Contorl(void)
{
  if(ccd1.flag_edge[4]==11 && ccd1.flag_edge[3]==11 && ccd1.flag_edge[2]==10 && ccd1.flag_edge[1]==10)
  {
    Cp=0;
    Kp=0;
    Vspeed = speed;
  }
  else if(ccd1.flag_edge[3] == 10 && ccd1.flag_edge[2]==10 && ccd1.flag_edge[1]==10)
  {
    Cp++;
    Vspeed = speed + (int16)( Cp*70/100 );
  }
  else if( ccd1.flag_edge[4] == 10 && ccd1.flag_edge[3] == 10 && ccd1.flag_edge[2]==0 && ccd1.flag_edge[1]==0 
            && ccd1.cross[2] != 1 && ccd1.cross[1] != 1 )
  {
    Cp++;
    Vspeed = speed + (int16)( Cp*70/100 );
  }
  else if(ccd1.flag_edge[3] == 10 && ccd1.flag_edge[2]==10 && ccd1.flag_edge[1]==0 && ccd1.cross[1]==1)
  {
    Cp=0;
    Kp=0;
    Vspeed = speed;
  }
  /////////////////////////////
  else if(ccd1.flag_edge[4]==11 && ccd1.flag_edge[3]==11 && ccd1.flag_edge[2]==1 && ccd1.flag_edge[1]==1)
  {
    Cp=0;
    Kp=0;
    Vspeed = speed;
  }
  else if(ccd1.flag_edge[3] == 1 && ccd1.flag_edge[2]==1 && ccd1.flag_edge[1]==1)
  {
    Cp++;
    Vspeed = speed + (int16)( Cp*70/100 );
  }
  else if( ccd1.flag_edge[4] == 1 && ccd1.flag_edge[3] == 1 && ccd1.flag_edge[2]==0 && ccd1.flag_edge[1]==0 
            && ccd1.cross[2] != 1 && ccd1.cross[1] != 1 )
  {
    Cp++;
    Vspeed = speed + (int16)( Cp*70/100 );
  }
  else if(ccd1.flag_edge[3] == 1 && ccd1.flag_edge[2]==1 && ccd1.flag_edge[1]==0 && ccd1.cross[1]==1)
  {
    Cp=0;
    Kp=0;
    Vspeed = speed;
  }
  /////////////////////////////
  else if(ccd1.flag_edge[4] == 10 && ccd1.flag_edge[3] == 10 && ccd1.flag_edge[2]==11 && ccd1.flag_edge[1]==11)
  {
    Cp=0;
    Kp=0;
    Vspeed = speed;
  }
  else if(ccd1.flag_edge[3] == 11 && ccd1.flag_edge[2]==11 && ccd1.flag_edge[1]==11
           && ccd1.cross[4]==0 && ccd1.cross[5]==0 && ccd1.cross[6]==0
           && ccd1.cross[7]==0 && ccd1.cross[8]==0 && ccd1.cross[9]==0
           && ccd1.cross[10]==0 && ccd1.cross[11]==0 && ccd1.cross[12]==0
            )
  {
    Kp++;
    Vspeed = speed + g - (int16)( Kp*70/100 );
    
    if(Vspeed < speed)
      Vspeed = speed;
  }
  else if( ccd1.flag_edge[4] == 11 && ccd1.flag_edge[3] == 11 && ccd1.flag_edge[2]==0 && ccd1.flag_edge[1]==0
            && ccd1.cross[2] != 1 && ccd1.cross[1] != 1 )
  {
    Kp=0;
    Cp=0;
    Vspeed = speed;
  }
  /////////////////////////////////////
  else
  {
    Vspeed = speed;
  }
  
  
  if(ramp >= 60 && ramp <= 100) //上坡减速
  {
    pidl.setPoint = (int16)(speed * 85/100);
    pidr.setPoint = (int16)(speed * 85/100);
  }
  else if(ramp > 15 && ramp < 60) //下坡加速
  {
    SetPoint(&ccd1,90);
  }
  else
  {
    if(brake_flag > 0)
    {
      brake_flag--;

      SetPointLL(&ccd1,a);
    }
    else
    { 
      if(ccd1.flag_edge[0]==1 && ccd1.flag_edge[1]==1 && ccd1.flag_edge[2]==1 
          && ccd1.flag_edge[3]==1 && ccd1.flag_edge[4]==11
          && ccd1.flag_edge[5]==11 && ccd1.flag_edge[6]==11 && ccd1.flag_edge[7]==11
          && ccd1.flag_edge[8]==11 && ccd1.flag_edge[9]==11 && ccd1.flag_edge[10]==11
          && ccd1.flag_edge[11]==11 && ccd1.flag_edge[12]==11 && ccd1.flag_edge[13]==11
          && ccd1.flag_edge[14]==11 && ccd1.flag_edge[15]==11 && ccd1.flag_edge[16]==11
          && ccd1.flag_edge[17]==11 && ccd1.flag_edge[18]==11 && ccd1.flag_edge[19]==11 
          && ccd1.flag_edge[20]==11 && ccd1.flag_edge[21]==11 && ccd1.flag_edge[22]==11 
          && ccd1.flag_edge[23]==11 && ccd1.flag_edge[24]==11 && ccd1.flag_edge[25]==11 
          && ccd1.flag_edge[26]==11 && ccd1.flag_edge[30]==11 && ccd1.flag_edge[31]==11 
          && ccd1.flag_edge[32]==11 && ccd1.flag_edge[24]==11 && ccd1.flag_edge[25]==11 
          && ccd1.flag_edge[26]==11 && ccd1.flag_edge[30]==11 && ccd1.flag_edge[31]==11
          && ccd1.flag_edge[32]==11 && ccd1.flag_edge[33]==11 && ccd1.flag_edge[34]==11 
          && ccd1.flag_edge[35]==11 && ccd1.flag_edge[36]==11 && ccd1.flag_edge[37]==11 
          && ccd1.flag_edge[38]==11 && ccd1.flag_edge[39]==11 && ccd1.flag_edge[40]==11 
          && ccd1.flag_edge[41]==11 && ccd1.flag_edge[42]==11 && ccd1.flag_edge[43]==11 
          && ccd1.flag_edge[44]==11 && ccd1.flag_edge[45]==11 && ccd1.flag_edge[46]==11
          && ccd1.flag_edge[47]==11 && ccd1.flag_edge[48]==11 && ccd1.flag_edge[49]==11

          && ABS(ccd1.mid_Line[11]-64)<10 && ABS(ccd1.mid_Line[12]-64)<10 && ABS(ccd1.mid_Line[13]-64)<10
          && ABS(ccd1.mid_Line[14]-64)<10 && ABS(ccd1.mid_Line[15]-64)<10 && ABS(ccd1.mid_Line[16]-64)<10
          && ABS(ccd1.mid_Line[17]-64)<10 && ABS(ccd1.mid_Line[18]-64)<10 && ABS(ccd1.mid_Line[19]-64)<10
          && ABS(ccd1.mid_Line[20]-64)<10 && ABS(ccd1.mid_Line[21]-64)<10 && ABS(ccd1.mid_Line[22]-64)<10
          && ABS(ccd1.mid_Line[23]-64)<10 && ABS(ccd1.mid_Line[24]-64)<10 && ABS(ccd1.mid_Line[25]-64)<10
          && ABS(ccd1.mid_Line[26]-64)<10 && ABS(ccd1.mid_Line[27]-64)<10 && ABS(ccd1.mid_Line[28]-64)<10
          && ABS(ccd1.mid_Line[29]-64)<10 && ABS(ccd1.mid_Line[30]-64)<10 && ABS(ccd1.mid_Line[31]-64)<10
          && ABS(ccd1.mid_Line[32]-64)<10 && ABS(ccd1.mid_Line[33]-64)<10 && ABS(ccd1.mid_Line[34]-64)<10
          && ABS(ccd1.mid_Line[35]-64)<10 && ABS(ccd1.mid_Line[36]-64)<10 && ABS(ccd1.mid_Line[37]-64)<10
          && ABS(ccd1.mid_Line[38]-64)<10 && ABS(ccd1.mid_Line[39]-64)<10 && ABS(ccd1.mid_Line[40]-64)<10
          && ABS(ccd1.mid_Line[41]-64)<10 && ABS(ccd1.mid_Line[42]-64)<10 && ABS(ccd1.mid_Line[43]-64)<10
          && ABS(ccd1.mid_Line[44]-64)<10 && ABS(ccd1.mid_Line[45]-64)<10 && ABS(ccd1.mid_Line[46]-64)<10
          && ABS(ccd1.mid_Line[47]-64)<10 && ABS(ccd1.mid_Line[48]-64)<10 && ABS(ccd1.mid_Line[49]-64)<10
         )
      {
        if(speed_dial == 1)
        {
          brake_flag = 45;
          SetPointLL(&ccd1,a-15);
        }
        else
        {
          brake_flag = 30;
          SetPointLL(&ccd1,a);
        }
      }
      else if(ccd1.flag_edge[0]==10 && ccd1.flag_edge[1]==10 && ccd1.flag_edge[2]==10 
               && ccd1.flag_edge[3]==10 && ccd1.flag_edge[4]==11
               && ccd1.flag_edge[5]==11 && ccd1.flag_edge[6]==11 && ccd1.flag_edge[7]==11
               && ccd1.flag_edge[8]==11 && ccd1.flag_edge[9]==11 && ccd1.flag_edge[10]==11
               && ccd1.flag_edge[11]==11 && ccd1.flag_edge[12]==11 && ccd1.flag_edge[13]==11
               && ccd1.flag_edge[14]==11 && ccd1.flag_edge[15]==11 && ccd1.flag_edge[16]==11
               && ccd1.flag_edge[17]==11 && ccd1.flag_edge[18]==11 && ccd1.flag_edge[19]==11 
               && ccd1.flag_edge[20]==11 && ccd1.flag_edge[21]==11 && ccd1.flag_edge[22]==11 
               && ccd1.flag_edge[23]==11 && ccd1.flag_edge[24]==11 && ccd1.flag_edge[25]==11 
               && ccd1.flag_edge[26]==11 && ccd1.flag_edge[30]==11 && ccd1.flag_edge[31]==11 
               && ccd1.flag_edge[32]==11 && ccd1.flag_edge[24]==11 && ccd1.flag_edge[25]==11 
               && ccd1.flag_edge[26]==11 && ccd1.flag_edge[30]==11 && ccd1.flag_edge[31]==11
               && ccd1.flag_edge[32]==11 && ccd1.flag_edge[33]==11 && ccd1.flag_edge[34]==11 
               && ccd1.flag_edge[35]==11 && ccd1.flag_edge[36]==11 && ccd1.flag_edge[37]==11 
               && ccd1.flag_edge[38]==11 && ccd1.flag_edge[39]==11 && ccd1.flag_edge[40]==11 
               && ccd1.flag_edge[41]==11 && ccd1.flag_edge[42]==11 && ccd1.flag_edge[43]==11 
               && ccd1.flag_edge[44]==11 && ccd1.flag_edge[45]==11 && ccd1.flag_edge[46]==11
               && ccd1.flag_edge[47]==11 && ccd1.flag_edge[48]==11 && ccd1.flag_edge[49]==11 
                 
               && ABS(ccd1.mid_Line[11]-64)<10 && ABS(ccd1.mid_Line[12]-64)<10 && ABS(ccd1.mid_Line[13]-64)<10
               && ABS(ccd1.mid_Line[14]-64)<10 && ABS(ccd1.mid_Line[15]-64)<10 && ABS(ccd1.mid_Line[16]-64)<10
               && ABS(ccd1.mid_Line[17]-64)<10 && ABS(ccd1.mid_Line[18]-64)<10 && ABS(ccd1.mid_Line[19]-64)<10
               && ABS(ccd1.mid_Line[20]-64)<10 && ABS(ccd1.mid_Line[21]-64)<10 && ABS(ccd1.mid_Line[22]-64)<10
               && ABS(ccd1.mid_Line[23]-64)<10 && ABS(ccd1.mid_Line[24]-64)<10 && ABS(ccd1.mid_Line[25]-64)<10
               && ABS(ccd1.mid_Line[26]-64)<10 && ABS(ccd1.mid_Line[27]-64)<10 && ABS(ccd1.mid_Line[28]-64)<10
               && ABS(ccd1.mid_Line[29]-64)<10 && ABS(ccd1.mid_Line[30]-64)<10 && ABS(ccd1.mid_Line[31]-64)<10
               && ABS(ccd1.mid_Line[32]-64)<10 && ABS(ccd1.mid_Line[33]-64)<10 && ABS(ccd1.mid_Line[34]-64)<10
               && ABS(ccd1.mid_Line[35]-64)<10 && ABS(ccd1.mid_Line[36]-64)<10 && ABS(ccd1.mid_Line[37]-64)<10
               && ABS(ccd1.mid_Line[38]-64)<10 && ABS(ccd1.mid_Line[39]-64)<10 && ABS(ccd1.mid_Line[40]-64)<10
               && ABS(ccd1.mid_Line[41]-64)<10 && ABS(ccd1.mid_Line[42]-64)<10 && ABS(ccd1.mid_Line[43]-64)<10
               && ABS(ccd1.mid_Line[44]-64)<10 && ABS(ccd1.mid_Line[45]-64)<10 && ABS(ccd1.mid_Line[46]-64)<10
               && ABS(ccd1.mid_Line[47]-64)<10 && ABS(ccd1.mid_Line[48]-64)<10 && ABS(ccd1.mid_Line[49]-64)<10
               )
      {
        if(speed_dial == 1)
        {
          brake_flag = 45;
          SetPointLL(&ccd1,a-15);
        }
        else
        {
          brake_flag = 30;
          SetPointLL(&ccd1,a);
        }
      }
      else if(ccd1.mid_Line[0] > 53 && ccd1.mid_Line[0] < 73)
      {
        SetPoint(&ccd1,d);
      }
      /*else if(ccd1.mid_Line[0]>40 && ccd1.mid_Line[0]<86)
      {
        SetPointLow(&ccd1,a);
      }*/
      else
      {
        SetPointLow(&ccd1,a);
      }
    }
  }
      
  Getpluse();       //获取编码器值
  
  if( (ramp >= 70 && ramp <= 100) || (brake_flag > 0 && speed_dial == 1) )///坡道带反转速度控制
  {
    motorPID111();  //电机PID计算,带反转
  }
  else
  {
    motorPID();     //电机PID计算,反转与否，看刹车系数
  }
  
  if(pidl.PWM >= 0)
  {
    M1(pidl.PWM);
    M2(0);
  }
  else
  {
    M2( ABS(pidl.PWM) );
    M1(0);
  }
  if(pidr.PWM >= 0)
  {
    M3(pidr.PWM);
    M4(0);
  }
  else
  {
    M4( ABS(pidr.PWM) );
    M3(0);
  }
}

/*******************************************
  calculatec string engine angle
********************************************/
void S3010Angle(struct TSL1401 *ccd)
{  
  spid.errorlast = spid.error;
   
  int16 error;
  error = 63 - ccd->mid_Line[0];
   
  //计算本次偏差
  spid.error = (int32)(error*error*error/600 + error*185/100);
    
  spid.angle = (uint32)(0.01*(spid.P*spid.error+spid.D*(spid.error-spid.errorlast))+725); 
  
  if(spid.angle > RightMaxAngle) //如果给的角度值大于舵机向右转的极限值，就赋予右极限角度
     spid.angle =  RightMaxAngle;
  if(spid.angle < LeftMaxAngle)  //如果给的角度值小于舵机向左转的极限值，就赋予左极限角度
     spid.angle =  LeftMaxAngle;
  
  //give 舵机 new angle
  ftm_pwm_duty(S3010_FTM, S3010_CH, spid.angle);  
}
/*******************************************
  calculatec string engine angle
********************************************/
void S3010AngleL(struct TSL1401 *ccd)
{  
  spid.errorlast = spid.error;
   
  int16 error;
  error = 63 - ccd->mid_Line[0];
   
  //计算本次偏差
  spid.error = (int32)(error*error*error/600 + error*185/100);
    
  spid.angle = (uint32)(0.01*(spid.P*spid.error+spid.D*(spid.error-spid.errorlast))+725); 
  
  if(spid.angle > RightMaxAngle) //如果给的角度值大于舵机向右转的极限值，就赋予右极限角度
     spid.angle =  RightMaxAngle;
  if(spid.angle < 780)  //如果给的角度值小于舵机向左转的极限值，就赋予左极限角度
     spid.angle =  780;
  
  //give 舵机 new angle
  ftm_pwm_duty(S3010_FTM, S3010_CH, spid.angle);  
}
/*******************************************
  calculatec string engine angle
********************************************/
void S3010AngleR(struct TSL1401 *ccd)
{  
  spid.errorlast = spid.error;
   
  int16 error;
  error = 63 - ccd->mid_Line[0];
   
  //计算本次偏差
  spid.error = (int32)(error*error*error/600 + error*185/100);
    
  spid.angle = (uint32)(0.01*(spid.P*spid.error+spid.D*(spid.error-spid.errorlast))+725); 
  
  if(spid.angle > 670) //如果给的角度值大于舵机向右转的极限值，就赋予右极限角度
     spid.angle = 670;
  if(spid.angle < LeftMaxAngle)  //如果给的角度值小于舵机向左转的极限值，就赋予左极限角度
     spid.angle =  LeftMaxAngle;
  
  //give 舵机 new angle
  ftm_pwm_duty(S3010_FTM, S3010_CH, spid.angle);  
}

void S3010AngleOB(struct TSL1401 *ccd,int8 a)
{
  spid.errorlast = spid.error;
   
  int16 error;
  error = 63 - (ccd->mid_Line[0] + a);
   
  //计算本次偏差
  spid.error = (int32)(error*error*error/600 + error*185/100);
    
  spid.angle = (uint32)(0.01*( (spid.P+10) *spid.error+spid.D*(spid.error-spid.errorlast))+725); 
  
  if(spid.angle > RightMaxAngle) //如果给的角度值大于舵机向右转的极限值，就赋予右极限角度
     spid.angle =  RightMaxAngle;
  if(spid.angle < LeftMaxAngle)  //如果给的角度值小于舵机向左转的极限值，就赋予左极限角度
     spid.angle =  LeftMaxAngle;
  
  //give 舵机 new angle
  ftm_pwm_duty(S3010_FTM, S3010_CH, spid.angle);  
}
/******************************
        获取脉冲数
********************************/
void Getpluse(void)
{
  pidl.pluse = ftm_quad_get(FTM2);    //获取FTM2 正交解码 的脉冲数
  pidr.pluse = ftm_quad_get(FTM1);    //获取FTM1 正交解码 的脉冲数
  
  ftm_quad_clean(FTM2);
  ftm_quad_clean(FTM1);               //清空脉冲计数器计算值（马上清空，这样才能保证计数值准确）
}
/********************************
    设置电机目标输出
*********************************/
void SetPoint(struct TSL1401 *ccd,int16 all)
{
  int16 error;
  error = 63 - ccd->mid_Line[0];
  
  int16 left_speed,right_speed;
  left_speed = right_speed = 0;
  
  if((error <= -7) || (error >= 7))//小车处于拐弯处
  {
    if(error <= -7)                     //右拐
    {
      left_speed  =  (Vspeed * 90/100) - (error*diff_speed/100);
      right_speed = (Vspeed * 88/100) + (error*diff_speed/100);
      //left_speed  = speed * (1 - (14*error*(45/ccd->width))/(45*45) );
      //right_speed  = speed * (1 + (14*error*(45/ccd->width))/(45*45) );
      if(right_speed < 0 )
      {
        right_speed = (speed * 75/100);
      }
    } 
    else                                     //左拐
    {
      left_speed  =  (Vspeed * 88/100) - (error*diff_speed/100);
      right_speed = (Vspeed * 90/100) + (error*diff_speed/100);
      //left_speed  = speed * (1 - (14*error*(45/ccd->width))/(45*45) );
      //right_speed  = speed * (1 + (14*error*(45/ccd->width))/(45*45) );
      if(left_speed < 0)
      {
        left_speed = (Vspeed * 75/100);
      }
    }
  }
  else if((error<=3) && (error>=-3))//小车处于直线
  {
    left_speed   = Vspeed * all/100;
    right_speed  = Vspeed * all/100;
  }
  else
  {
    left_speed   = Vspeed;
    right_speed  = Vspeed;
  }
  
  pidl.setPoint = (int16)left_speed;
  pidr.setPoint = (int16)right_speed;
}

void SetPointLow(struct TSL1401 *ccd,int16 all)
{
  int16 error;
  error = 63 - ccd->mid_Line[0];
  
  int16 left_speed,right_speed;
  left_speed = right_speed = 0;
  
  if((error <= -7) || (error >= 7))//小车处于拐弯处
  {
    if(error <= -7)                     //右拐
    {
      left_speed  =  (Vspeed * 90/100) - (error*diff_speed/100);
      right_speed = (Vspeed * 80/100) + (error*diff_speed/100);
      //left_speed  = speed * (1 - (14*error*(45/ccd->width))/(45*45) );
      //right_speed  = speed * (1 + (14*error*(45/ccd->width))/(45*45) );
      if(right_speed < 0 )
      {
        right_speed = (Vspeed * 70/100);
      }
    } 
    else                                     //左拐
    {
      left_speed  = (Vspeed * 80/100) - (error*diff_speed/100);
      right_speed = (Vspeed * 90/100) + (error*diff_speed/100);
      //left_speed  = speed * (1 - (14*error*(45/ccd->width))/(45*45) );
      //right_speed  = speed * (1 + (14*error*(45/ccd->width))/(45*45) );
      if(left_speed < 0)
      {
        left_speed = (Vspeed * 70/100);
      }
    }
  }
  else
  {
    left_speed   = Vspeed * all/100;
    right_speed  = Vspeed * all/100;
  }
  
  pidl.setPoint = (int16)left_speed;
  pidr.setPoint = (int16)right_speed;
}


void SetPointLL(struct TSL1401 *ccd,int16 all)
{
  int16 error;
  error = 63 - ccd->mid_Line[0];
  
  int16 left_speed,right_speed;
  left_speed = right_speed = 0;
  
  if((error <= -7) || (error >= 7))//小车处于拐弯处
  {
    if(error <= -7)                     //右拐
    {
      left_speed  =  (Vspeed * 85/100) - (error*diff_speed/100);
      right_speed = (Vspeed * 60/100) + (error*diff_speed/100);
      //left_speed  = speed * (1 - (14*error*(45/ccd->width))/(45*45) );
      //right_speed  = speed * (1 + (14*error*(45/ccd->width))/(45*45) );
      if(right_speed < 0 )
      {
        right_speed = (speed * 60/100);
      }
    } 
    else                                     //左拐
    {
      left_speed  = (Vspeed * 60/100) - (error*diff_speed/100);
      right_speed = (Vspeed * 85/100) + (error*diff_speed/100);
      //left_speed  = speed * (1 - (14*error*(45/ccd->width))/(45*45) );
      //right_speed  = speed * (1 + (14*error*(45/ccd->width))/(45*45) );
      if(left_speed < 0)
      {
        left_speed = (Vspeed * 60/100);
      }
    }
  }
  else
  {
    left_speed   = Vspeed * all/100;
    right_speed  = Vspeed * all/100;
  }
  
  pidl.setPoint = (int16)left_speed;
  pidr.setPoint = (int16)right_speed;
}
/********************************
    电机PID计算
**********************************/
void motorPID()
{
  int16 error0,error1,error2;
  
  //左电机
  error0 = error1 = error2 = 0;
  error0 = pidl.setPoint - pidl.pluse;//I
  error1 = error0 - pidl.lastError;//P
  error2 = error0 - 2*pidl.lastError + pidl.preError;//D
  pidl.preError = pidl.lastError;
  pidl.lastError = error0;
  
  pidl.out = (int16)(pidl.P*error1 + pidl.I*error0 + pidl.D*error2);
  pidl.PWM += pidl.out; 
  
  if(error0 > 10)   pidl.PWM = 9999;
  if(error0 < -10)  pidl.PWM = 500;
  
  if(pidl.PWM > 9999)  pidl.PWM = 9999;
  if(pidl.PWM < 500   )  pidl.PWM = 500;
  
  //右电机
  error0 = error1 = error2 = 0;
  error0 = pidr.setPoint - pidr.pluse;//I
  error1 = error0 - pidr.lastError;//P
  error2 = error0 - 2*pidr.lastError + pidr.preError;//D
  pidr.preError = pidr.lastError;
  pidr.lastError = error0;
  
  pidr.out = (int16)(pidr.P*error1 + pidr.I*error0 + pidr.D*error2);
  pidr.PWM += pidr.out; 
  
  if(error0 > 10)   pidr.PWM = 9999;
  if(error0 < -10)  pidr.PWM = 500;
  
  if(pidr.PWM > 9999)  pidr.PWM = 9999;
  if(pidr.PWM < 500   )  pidr.PWM = 500;
}

/********************************
    电机PID计算
**********************************/
void motorPID111()
{
  int16 error0,error1,error2;
  
  //左电机
  error0 = error1 = error2 = 0;
  error0 = pidl.setPoint - pidl.pluse;//I
  error1 = error0 - pidl.lastError;//P
  error2 = error0 - 2*pidl.lastError + pidl.preError;//D
  pidl.preError = pidl.lastError;
  pidl.lastError = error0;
  
  pidl.out = (int16)(pidl.P*error1 + pidl.I*error0 + pidl.D*error2);
  pidl.PWM += pidl.out; 
  
  if(error0 > 10)   pidl.PWM = 9999;
  if(error0 < -10)  pidl.PWM = -7777;
  
  if(pidl.PWM > 9999)  pidl.PWM = 9999;
  if(pidl.PWM < -7777   )  pidl.PWM = -7777;
  
  //右电机
  error0 = error1 = error2 = 0;
  error0 = pidr.setPoint - pidr.pluse;//I
  error1 = error0 - pidr.lastError;//P
  error2 = error0 - 2*pidr.lastError + pidr.preError;//D
  pidr.preError = pidr.lastError;
  pidr.lastError = error0;
  
  pidr.out = (int16)(pidr.P*error1 + pidr.I*error0 + pidr.D*error2);
  pidr.PWM += pidr.out; 
  
  if(error0 > 10)   pidr.PWM = 9999;
  if(error0 < -10)  pidr.PWM = -7777;
  
  if(pidr.PWM > 9999)  pidr.PWM = 9999;
  if(pidr.PWM < -7777   )  pidr.PWM = -7777;
}

