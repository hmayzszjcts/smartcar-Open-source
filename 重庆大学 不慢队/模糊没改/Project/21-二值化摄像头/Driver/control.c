#include "include.h"
//角度类变量
int16  Acc_Z,Gyro_X,Gyro_Y;
//int16  Acc_Max=8192;
//int16  Acc_Min=-8192;
int16  Acc_Max=3780;
int16  Acc_Min=-2600;
int16  Acc_Offset;
float Angle,Angle_Speed,Car_Angle=0;
int16  Gyro_Z_Offset=427,Gyro_Y_Offset=47,Gyro_X_Offset=740;
//速度类变量
float SpeedControlOutNew;
float SpeedControlOutOld;
float SpeedControlIntegral=0,Hill_Slow_Ratio;
uint8  Set_Angle;   //加速前倾角度
int   SpeedCount;
int   Speed_Filter_Times=15;    //速度平滑输出  50
float CarSpeed=0,ControlSpeed=0,AverageSpeed,SetSpeed=0,Distance;
//方向类变量
float DirectionControlOutNew;
float DirectionControlOutOld;
float Turn_Speed=0;
int   DirectionCount;
float Delt_error,Middle_Err;
float Turn_Out;
float Turn_Angle_Integral;
//模糊化系数
float  Delta_P;
float  Delta_D;
float  Fuzzy_Kp;
float  Fuzzy_Kd;
//PID控制类变量
PID PID_ANGLE,PID_SPEED,PID_TURN;

float  LeftMotorOut,RightMotorOut;   //电机输出量
float  L_DeadValue=0,R_DeadValue=0;
float  RunTime=0;                    //跑动时间计数
uint8   Starting,Stop,CarStopedJustNow;
uint8 Encoder_Disable=0;

float shuzhi;
//
void Get_Attitude()
{
  Acc_Z =Get_Z_Acc(); 
  Gyro_X= Get_X_Gyro();
  Gyro_Y= Get_Y_Gyro();            
}
//******Kalman滤波******//
//-------------------------------------------------------
static  float Q_angle=0.001, Q_gyro=0.001, R_angle=5, dt=0.004;
	//Q增大，动态响应增大
static float Pk[2][2] = { {1, 0}, {0, 1 }};
	
static float Pdot[4] ={0,0,0,0};

static float q_bias=0, angle_err, PCt_0, PCt_1, E, K_0, K_1, t_0, t_1;
//-------------------------------------------------------
void Kalman_Filter(float angle_m,float gyro_m)			
{
	Car_Angle+=(gyro_m-q_bias) * dt; ///预测值
	Pdot[0]=Q_angle - Pk[0][1] - Pk[1][0];
	Pdot[1]=- Pk[1][1];
	Pdot[2]=- Pk[1][1];
	Pdot[3]=Q_gyro;
	
	Pk[0][0] += Pdot[0] * dt;
	Pk[0][1] += Pdot[1] * dt;
	Pk[1][0] += Pdot[2] * dt;
	Pk[1][1] += Pdot[3] * dt;
	
	angle_err = angle_m -Car_Angle;///测量值-预测值
	
	PCt_0 =  Pk[0][0];
	PCt_1 =  Pk[1][0];
	
	E = R_angle + PCt_0;
	
	K_0 = PCt_0 / E; ///卡尔曼增益
	K_1 = PCt_1 / E;
	
	t_0 = PCt_0;
	t_1 = Pk[0][1];

	Pk[0][0] -= K_0 * t_0;
	Pk[0][1] -= K_0 * t_1;
	Pk[1][0] -= K_1 * t_0;
	Pk[1][1] -= K_1 * t_1;
	
	Car_Angle+= K_0 * angle_err; ///最优角度=预测值+卡尔曼增益*(测量值-预测值)
	q_bias	+= K_1 * angle_err;
	Angle_Speed = gyro_m-q_bias;
}


//角度计算与滤波
void Angle_Calculate()
{
 float xishu=0.048;
 Angle =(Acc_Z-Gyro_Z_Offset)*180.0/(Acc_Max-Acc_Min); 
 Angle_Speed=(Gyro_Y-Gyro_Y_Offset) * xishu;
 //Car_Angle+=Angle_Speed*0.005;//没有 
 Kalman_Filter(Angle,Angle_Speed);            //调用卡尔曼滤波函数
}
//角度控制函数
void Angle_Control()  
{
  PID_ANGLE.pout=PID_ANGLE.P*Car_Angle;
  PID_ANGLE.dout=PID_ANGLE.D*Angle_Speed;
  if(ABS(Angle_Speed)>30&&ABS(Angle_Speed)<80)
  {
       PID_ANGLE.dout*=(1+(ABS(Angle_Speed)-30.0)/30.0);
  } 
  PID_ANGLE.OUT= PID_ANGLE.pout+ PID_ANGLE.dout;
  shuzhi = Angle_Speed;
}
void Get_Speed()                     //5ms执行一次
{  
  int qd1_result,qd2_result;
  qd1_result = FTM_QUAD_get(FTM1); 
  qd2_result = FTM_QUAD_get(FTM2);
  
  FTM_QUAD_clean(FTM1);
  FTM_QUAD_clean(FTM2);

  Distance+=(qd1_result+qd2_result)/6100.0;  //转化为距离
  CarSpeed=CarSpeed*0.1+0.9*(qd1_result+qd2_result)*250.0/6100.0;    //求出车速转换为M/S
   if(CarSpeed>4)CarSpeed=3.5;//3.5
 
}
//速度控制量计算
void Speed_Control(void)
{  
      
  static float PreError[20]={0};
  float  SpeedError;
  uint8 i;
  //设定速度滤波
  //速度滤波，防止因为速度变化过大而车身晃动
  //if(Starting||Stop)  setspeed=0; //启动的时候把速度置为零
  
   SpeedError=SetSpeed-CarSpeed;  
   
  //求出最近20个偏差的总和作为积分项
   SpeedControlIntegral=0;
   for(i=0;i<19;i++)
   {
     PreError[i]=PreError[i+1]; 
     SpeedControlIntegral+=PreError[i];
   }
    PreError[19]=SpeedError;
    SpeedControlIntegral+=PreError[19]; 
  //速度更新
  SpeedControlOutOld=SpeedControlOutNew;
  //防止起步位移
//  if(Starting)
//  {
//   SpeedControlIntegral=-50*Distance; //没有
//   SpeedError=0;
//  } 
  SpeedControlOutNew=PID_SPEED.P*SpeedError+PID_SPEED.I*SpeedControlIntegral;   //PI控制
  SpeedControlOutNew= SpeedControlOutOld*0.9+SpeedControlOutNew*0.1;
}
//速度控制
void Speed_Control_Output(void) 
{ 
  float fValue; 
  
  fValue = SpeedControlOutNew - SpeedControlOutOld; 
  PID_SPEED.OUT = fValue * (SpeedCount+1)/Speed_Filter_Times+SpeedControlOutOld;
//    shuzhi = PID_SPEED.OUT;
}
/********************方向控制量计算***************/
void Direction_Control(void)
{
  float ratio=0.005;
  static int Calculate_Length=0;
  Turn_Speed=(Gyro_X-Gyro_X_Offset)*ratio;            
  
   
  if(RoadType==0)  //只有在普通赛道和单线上用模糊
  {
    if(Calculate_Length<8) 
    {  
     Calculate_Length++;
     Delta_P=0;
     Delt_error=0;
     Delta_D=0;
    }
    else
    {
      Delt_error=-10*Slope_Calculate(0,Calculate_Length,Previous_Error);
      Delta_P=Fuzzy( Middle_Err,Delt_error)* Fuzzy_Kp*0.1;
      Delta_D=Fuzzy( Middle_Err,Delt_error)* Fuzzy_Kd*0.1;
    }
  }
  else 
  {
    Delta_P=0;
    Delta_D=0;
    Calculate_Length=0;
  }
  
  PID_TURN.pout=(PID_TURN.P+Delta_P)*Middle_Err;
  PID_TURN.dout=(PID_TURN.D+Delta_D)*Turn_Speed;
  Turn_Out= PID_TURN.pout+ PID_TURN.dout;
  Turn_Out=Turn_Out_Filter(Turn_Out);         //转动输出滤波 
  PID_TURN.OUT=Turn_Out;

}

//电机pwm值输出
void Moto_Out() 
{
 int L_Value,R_Value;
 //static float  Forward_Safe_Angle=35;//前倾的安全角度
 static float  Forward_Safe_Angle=-25;
 static int Motor_Abnormal_Cnt=0;    //电机转速异常计数
 //int Backward_Safe_Angle=20;          //后倾的安全角度
  int Backward_Safe_Angle=20;
 float Sum;
 //速度控制输出限幅
// if(PID_SPEED.OUT>PID_ANGLE.P*Forward_Safe_Angle)//如果车子前倾，则车模的速度控制输出为正，反之为负
// PID_SPEED.OUT=PID_ANGLE.P*Forward_Safe_Angle;                       //已经倾斜到到安全角度了
// if(PID_SPEED.OUT<-PID_ANGLE.P*Backward_Safe_Angle)
// PID_SPEED.OUT=-PID_ANGLE.P*Backward_Safe_Angle;

//  if(PID_SPEED.OUT>PID_ANGLE.P*Forward_Safe_Angle)//如果车子前倾，则车模的速度控制输出为正，反之为负
//      PID_SPEED.OUT=PID_ANGLE.P*Forward_Safe_Angle;//已经倾斜到到安全角度了  PID_ANGLE.P*(Forward_Safe_Angle)
// if(PID_SPEED.OUT<-PID_ANGLE.P*Backward_Safe_Angle)
//     PID_SPEED.OUT=-PID_ANGLE.P*(Backward_Safe_Angle);   //-PID_ANGLE.P*(Backward_Safe_Angle)
 
 Sum=PID_ANGLE.OUT+PID_SPEED.OUT;//+
//
//  Sum=PID_ANGLE.OUT;
//  LeftMotorOut=Sum-PID_TURN.OUT;   //计算输出值
//  RightMotorOut=Sum+PID_TURN.OUT; 
  LeftMotorOut=Sum;   //计算输出值
  RightMotorOut=Sum; 
 //正值限幅，防止减速过大

  if(RightMotorOut>0.99)RightMotorOut=0.99;                     
  if(RightMotorOut<-0.99)RightMotorOut=-0.99; 
  if(LeftMotorOut>0.99)LeftMotorOut=0.99;                     
  if(LeftMotorOut<-0.99)LeftMotorOut=-0.99; 
  
  L_Value=(int)(3000*LeftMotorOut);            //10000 
  R_Value=(int)(3000*RightMotorOut);            //10000
 
 /////////////////////保护////////////////////////////
 if(Car_Angle<-100||Car_Angle>100)             //倒下 -50
 {
   if(Stop==false&&RunTime>2)
   {
    Stop=true;
    CarStopedJustNow=true; 
    Motor_Abnormal_Cnt=0;
   }
    L_Value=0;
    R_Value=0;
 }
// if((Distance>1)&&(Stop==false))               //              检测是否是速度异常
// {
//    if((CarSpeed<0.5)||(CarSpeed>2)) //4
//    {
//       Motor_Abnormal_Cnt++;
//       if(Motor_Abnormal_Cnt>250) //电机异常的持续时间大于500ms秒
//       {
//          Stop=true;
//          CarStopedJustNow=true; 
//       }
//    } 
//    else
//    {
//       Motor_Abnormal_Cnt=0;
//    }
// }
// if(Stop)                                //如果停止则电机不输出
// {
//    
//    //L_Value=0;
//   // R_Value=0;
// }
   //   PID_SPEED.OUT=0;
     // PID_TURN.OUT=0;
   if(L_Value>=0) //正转
  {
     FTM_PWM_Duty(FTM0,FTM_CH2,L_Value);//占空比精度为10000  20 31
     FTM_PWM_Duty(FTM0,FTM_CH0,0);
  }
  else   //反转
  {
     FTM_PWM_Duty(FTM0,FTM_CH2,0);
     FTM_PWM_Duty(FTM0,FTM_CH0,-L_Value);
  }
    if(R_Value>=0) //正转
  {
     FTM_PWM_Duty(FTM0,FTM_CH3,R_Value); 
     FTM_PWM_Duty(FTM0,FTM_CH1,0);
  }
  else   //反转
  {
     FTM_PWM_Duty(FTM0,FTM_CH3,0);
     FTM_PWM_Duty(FTM0,FTM_CH1,-R_Value);
  }
}
float  Turn_Out_Filter(float turn_out)    //转向控制输出滤波      
{
  float Turn_Out_Filtered; 
  static float Pre1_Error[4]; 
  Pre1_Error[3]=Pre1_Error[2];
  Pre1_Error[2]=Pre1_Error[1];
  Pre1_Error[1]=Pre1_Error[0];
  Pre1_Error[0]=turn_out;
  Turn_Out_Filtered=Pre1_Error[0]*0.4+Pre1_Error[1]*0.3+Pre1_Error[2]*0.2+Pre1_Error[3]*0.1;
  return Turn_Out_Filtered;
}
float  Middle_Err_Filter(float middle_err)    //中心偏差滤波      
{
  float Middle_Err_Fltered; 
  static float Pre3_Error[4]; 
  Pre3_Error[3]=Pre3_Error[2];
  Pre3_Error[2]=Pre3_Error[1];
  Pre3_Error[1]=Pre3_Error[0];
  Pre3_Error[0]=middle_err;
  Middle_Err_Fltered=Pre3_Error[0]*0.4+Pre3_Error[1]*0.3+Pre3_Error[2]*0.2+Pre3_Error[3]*0.1;
  return Middle_Err_Fltered;
}