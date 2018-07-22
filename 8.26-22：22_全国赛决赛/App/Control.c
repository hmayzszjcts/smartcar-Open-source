#include "Motor_Control.h"
#include "math.h"



        /****************************    其他控制变量    ****************************/ 
char Set = 0,Speed_Set=0;
uint8   BangBang_Flag = 0;        //在增量式PID里
uint8 	Run_Flag  = 0;                    //起跑标志
uint8 Ramp_Flag=0;
uint16 Ramp_Time=0;
uint16 Ramp_Delay=60;       //坡道延时时间
int16 Ramp_Speed=185;       //坡道速度
extern uchar Foresight;
extern int32 S3010_Duty;
        /****************************        结束        ****************************/ 
int8    Point_Num=0;
        /****************************    起跑识别变量    ****************************/ 
uint8   Game_End = 1;
uint8   Stop_Flag = 0;
uint8   Go_Flag = 0;
uint8   zhidao_Flag=0;
uint8   Annulus_Check=0;
uint8   Obstacle_Check=0;
        /****************************        结束        ****************************/ 

        /****************************    差速调节变量    ****************************/ 
float g_dirControl_P = 3.3;		//方向P
float g_dirControl_D = 10;		//方向D
int16  g_nDirectionError;  		//方向偏差
float g_fDirectionError_dot;	//方向偏差微分
float g_fDirectionControlOut;	//方向控制输出

int8  CS_Point;
int32 CS_Duty;      //       //4320  4740 5180
        /****************************        结束        ****************************/ 

        /****************************   进出弯识别变量   ****************************/ 
//uint8   Went_Curve=0;     //进弯道标志位
//uint8   Out_Curve=0;      //出弯道标志位
//uint8   Straight_Time=0;    //直道时间常数
//uint8   Point_iE=0;         //中线偏差
//uint8   Out_Curve_Time=0;   //出弯时间常数
//uint8   Went_Curve_Time=0;  //进弯时间常数
        /****************************        结束        ****************************/ 
        
        /****************************    速度控制变量    ****************************/ 
int16 Starting_Time=300;              //起跑延时时间2S
int8 Starting2_Time=0;
int32   Left_MOTOR_Duty  = 0;
int32   Right_MOTOR_Duty  = 0;
int32   Left_MOTOR_Speed = 0;         //FTM1  正值
int32   Right_MOTOR_Speed = 0;         //FTM2 负值
float   Left_Speed = 0;         
float   Right_Speed = 0; 


int16 Speed;
int16 Max_Speed=0;
int16 Min_Speed=0;
int16 High_Speed=0;

int16 Mid_Speed,Left_High_Speed,Right_High_Speed;


        /****************************    差速相关变量    ****************************/ 
int16 CS_Flag = 1568;//1568 数值越大减速越提前
int16   error_0;
int16    a;
int16 Speed_iError=0;
uint8    CS_Left=11,CS_Right=11;
float   Temp_Orr;         
float   Temp_Add;
int32    Angle;
float    CS_P_Right,CS_P_Left;
        /****************************        结束        ****************************/ 


float speed_error[47]={ 1,   1.0139,  1.0279, 1.0419, 1.0559, 1.0700, 1.0840,    //7
                      1.0981, 1.1124, 1.1266, 1.1410, 1.1554, 1.1700, 1.1846,     //7
                      1.1994, 1.2143, 1.2293, 1.2445, 1.2599, 1.2754, 1.2911,     //7
                      1.3070, 1.3232, 1.3395, 1.3561, 1.3730, 1.4076, 1.4253,     //7
                      1.4434, 1.4618, 1.4806, 1.4998, 1.5195, 1.5396, 1.5601,     //7
                      1.5812, 1.6028, 1.6249, 1.6477, 1.6712, 1.6953, 1.7203,     //7
                      1.7460, 1.7724, 1.800, 1.900, 2.000};   //5

        /****************************        结束        ****************************/ 

/******** 速度控制 *********/
void Adjust_Speed(void)
{
//    Speed = 175;
//    Max_Speed = Speed*1.3;
//    Min_Speed = Speed;
//    
  //   High_Speed = Speed;
 //    High_Speed = (Foresight-30)*(Foresight-30)*(Max_Speed-Min_Speed)/CS_Flag + Min_Speed;
        //if (Foresight>20)Foresight=20;
//        High_Speed = (int16)((20-Foresight)*(20-Foresight)*(Max_Speed-Min_Speed))/400;
//        High_Speed = Max_Speed - High_Speed;
    /****************************      速度1档      ****************************/  
        if (Speed_Set==1)
        {
            if (Foresight >= 18)
            {
                High_Speed = 180;
            }
            else if (Foresight >= 16)
            {
                High_Speed = 182;
            }
            else if (Foresight >= 14)
            {
                High_Speed = 185;
            }
            else if (Foresight >= 12)
            {
                High_Speed = 189;
            }
            else if (Foresight >= 10)
            {
                High_Speed = 191;
            }
            else if (Foresight >= 8)
            {
                High_Speed = 193;
            }
            else if (Foresight >= 5)
            {
                High_Speed = 195;
            }
            else if (Foresight >= 4)
            {
                High_Speed = 200;
            }
            else if (Foresight >= 3)
            {
                High_Speed = 210;
            }
            else if (Foresight >= 2)
            {
                High_Speed = 220;
            }
            else
            {
                High_Speed = 240;
            }
            CS_P_Right = 0.35;  //差速系数
            CS_P_Left = 0.35;
            Set = 1;            //电机PID
        }
        /****************************      速度2档      ****************************/ 
        if (Speed_Set==2)
        {
            High_Speed = 160;
            CS_P_Right = 0.45;  //差速系数
            CS_P_Left = 0.45;
            Set = 1;            //电机PID
        }
        /****************************      速度3档      ****************************/ 
        if (Speed_Set==3)
        {
            High_Speed = 170;
            CS_P_Right = 0.45;  //差速系数
            CS_P_Left = 0.45;
            Set = 1;            //电机PID
        }
        /****************************      速度4档      ****************************/ 
        if (Speed_Set==4)
        {
            High_Speed = 180;
            CS_P_Right = 0.33;  //差速系数
            CS_P_Left = 0.33;
            Set = 1;            //电机PID
        }
        /****************************      速度5档      ****************************/  
        if (Speed_Set==5)
        {
            High_Speed = 200;
            CS_P_Right = 0.35;  //差速系数
            CS_P_Left = 0.35;
            Set = 1;            //电机PID
        }
        /****************************      速度6档      ****************************/ 
        if (Speed_Set==6)
        {
            High_Speed = 202;
            CS_P_Right = 0.35;  //差速系数
            CS_P_Left = 0.35;
            Set = 1;            //电机PID
        }
        /****************************      速度7档      ****************************/ 
        if (Speed_Set==7)
        {
            
            High_Speed = 205;
            CS_P_Right = 0.35;  //差速系数
            CS_P_Left = 0.35;
            Set = 1;            //电机PID
        }
        /****************************      速度8档      ****************************/ 
        if (Speed_Set==8)
        {
            
            High_Speed = 207;
            CS_P_Right = 0.35;  //差速系数
            CS_P_Left = 0.35;
            Set = 1;            //电机PID
        }    
        /****************************************************************************/  
        /****************************      坡道检测      ****************************/  
        /****************************************************************************/ 
//        MPU6050_GetData(&GYRO, &ACC);	// 读取陀螺仪数据
//        Data_Filter();
//
//        if ( GYRO_Real.Y>150 && !Ramp_Flag && !Ramp_Time)
//        {
//                Ramp_Flag=1;
//        }
//        if (Ramp_Flag)
//        {
//                High_Speed=Ramp_Speed;
//        }
//        if (Ramp_Time>Ramp_Delay)
//        { 
//                Ramp_Flag=0;
//        }
//        if (Ramp_Time>150)
//        { 
//                Ramp_Time=0;
//        } 
        
        
        /****************************************************************************/  
        /****************************      起跑控制      ****************************/  
        /****************************************************************************/         
        if (!Go_Flag)
        {
                High_Speed=0;  
        }
        else
        {

        }
    

        /****************************************************************************/  
        /****************************    出弯入弯检测    ****************************/  
        /****************************************************************************/          
//        if (Point>40)   Point_iE=Point-40; else Point_iE=40-Point;
//        if (Went_Curve) Out_Curve_Time++;  
//        if (Out_Curve)  Went_Curve_Time++; 
//        if (Point_iE<5) Straight_Time++;
//        
//        if (Straight_Time>5 && Point_iE>8)
//        {
//                Went_Curve=1;
//                Straight_Time=0;
//        }
//        if (Went_Curve && Went_Curve_Time>30 && Point_iE<15)
//        {
//                Went_Curve=0;
//                Went_Curve_Time=0;
//                Out_Curve=1;
//        }
//        if (Out_Curve_Time>30 || Point_iE<8)
//        {
//                Out_Curve=0;
//        }
        
   //     save_var(VAR2, Foresight);
        
}

void S3010_Control(void)
{
        S3010_Duty = PlacePID_Control(&S3010_PID,Point_Mid,Point);	//使用位置式PID解算	
		S3010_Duty = range_protect(S3010_Duty, S3010_RIGHT, S3010_LEFT);          //舵机占空比限幅保护
        
		ftm_pwm_duty(S3010_FTM, S3010_CH, S3010_Duty);			//修改舵机占空比
}
/******** 电机控制 *********/
void MOTOR_Control(void)
{
        CS_Point = Motor_Point_Weight(4);		// 正值 图像向下平移	必须为偶数
        CS_Duty = PlacePID_CS(&CS_PID,Point_Mid,CS_Point);	//使用位置式PID解算	
		CS_Duty = range_protect(CS_Duty, S3010_RIGHT, S3010_LEFT);          //舵机占空比限幅保护
        Angle = S3010_MID - CS_Duty;
        
//        Angle = S3010_MID - S3010_Duty;
//        save_var(VAR5, S3010_Duty);
//        save_var(VAR6, Angle);
        /****************************      起跑检测      ****************************/ 
        Starting_Check();  
        /****************************      速度调节      ****************************/ 
        Adjust_Speed();         

        /****************************************************************************/  
        /****************************    舵机角度差速    ****************************/  
        /****************************************************************************/ 
       
        if(Angle>0)         //右拐
        {
           a=(int16)(45*(Angle)*1.0/(S3010_LEFT - S3010_MID)) ;  //angle_max   45
           if(a>45)  a=45;
           if(a<0)   a=0;
           Left_Speed = High_Speed ;
           Right_Speed = High_Speed - (High_Speed/45)*a*CS_P_Right;
           Left_High_Speed = Left_Speed;
           Right_High_Speed = Right_Speed; 
        }
        else              //左拐
        {
           a=(int16)(45*(-Angle)*1.0/(S3010_MID - S3010_RIGHT)) ;  //angle_max   45
           if(a>45)  a=45;
           if(a<0)   a=0;
           Left_Speed = High_Speed - (High_Speed/45)*a*CS_P_Left;
           Right_Speed = High_Speed;
           Left_High_Speed = Left_Speed;
           Right_High_Speed = Right_Speed; 

        }
        
//        Left_High_Speed = High_Speed + g_fDirectionControlOut;
////        if(Left_High_Speed<0)Left_High_Speed=0;
////        else if(Left_High_Speed>2*High_Speed)Left_High_Speed=2*High_Speed;
//           Right_High_Speed = High_Speed - g_fDirectionControlOut;
////           if(Right_High_Speed<0)Right_High_Speed=0;
////        else if(Right_High_Speed>2*High_Speed)Right_High_Speed=2*High_Speed;
        
        /****************************************************************************/  
        /****************************    舵机角度加差速    ****************************/  
        /****************************************************************************/ 
       
//        if(Angle>0)         //右拐
//        {
//           a=(int16)(45*(Angle)*1.0/(S3010_LEFT - S3010_MID)) ;    //angle_max   45
//           if(a>45)  a=45;
//           if(a<0)   a=0;
//           Left_Speed = High_Speed + (High_Speed/45)*a*0.4;
//           Right_Speed = High_Speed;
//           Left_High_Speed = Left_Speed;
//           Right_High_Speed = Right_Speed; 
//        }
//        else              //左拐
//        {
//           a=(int16)(45*(-Angle)*1.0/(S3010_MID - S3010_RIGHT)) ;  //angle_max   45
//           if(a>45)  a=45;
//           if(a<0)   a=0;
//           Left_Speed = High_Speed;
//           Right_Speed = High_Speed + (High_Speed/45)*a*0.4;;
//           Left_High_Speed = Left_Speed;
//           Right_High_Speed = Right_Speed; 
//
//        }
        /****************************************************************************/  
        /****************************      建模减差速      ****************************/  
        /****************************************************************************/   
        

//         if(Angle>0)
//        {
//           a=(int16)(45*(Angle)*1.0/(S3010_LEFT - S3010_MID)) ;   //angle_max   45
//           if(a>45)  a=45;
//           if(a<0)   a=0;
//           Temp_Orr = 45 - (tan((a*3.14)/180) * 15.4);     //40常数越大入弯提前差速
//           Temp_Add = 45 + (tan((a*3.14)/180) * 15.4);
//           Left_Speed = High_Speed;
//           Right_Speed = High_Speed * Temp_Orr / Temp_Add*1.0;    //1.0常数越大差速越大
//           Left_High_Speed = Left_Speed;
//           Right_High_Speed = Right_Speed; 
//        }
//        else
//        {
//            a=(int16)(45*(-Angle)*1.0/(S3010_MID - S3010_RIGHT)) ; //angle_max   45
//           if(a>45)  a=45;
//           if(a<0)   a=0;
//           Temp_Orr =45 - (tan((a*3.14)/180) * 15.4);
//           Temp_Add = 45 + (tan((a*3.14)/180) * 15.4);
//           Left_Speed = High_Speed * Temp_Orr / Temp_Add*1.0;
//           Right_Speed = High_Speed;
//           Left_High_Speed = Left_Speed;
//           Right_High_Speed = Right_Speed; 
//
//        }
        
         /****************************************************************************/  
        /****************************      建模加减差速      ****************************/  
        /****************************************************************************/   
        
//         if(Angle>0)
//        {
//           a=(int16)(45*(Angle)*1.0/(S3010_LEFT - S3010_MID)) ;   //angle_max   45
//           if(a>45)  a=45;
//           if(a<0)   a=0;
//           Temp_Orr = tan((a*3.14)/180) * 15.4 / 40;
//           Left_Speed = 1.0 * High_Speed * (1.0 + Temp_Orr); //第一个常数可以加大差速，第二个常数可以提前差速
//           Right_Speed = 1.0 * High_Speed * (1.0 - Temp_Orr);
//           Left_High_Speed = Left_Speed;
//           Right_High_Speed = Right_Speed; 
//        }
//        else
//        {
//          a=(int16)(45*(-Angle)*1.0/(S3010_MID - S3010_RIGHT)) ;  //angle_max   45
//          if(a>45)  a=45;
//           if(a<0)   a=0;
//           Temp_Orr = tan((a*3.14)/180) * 15.4 / 40;
//           Left_Speed = 1.0 * High_Speed * (1.0 - Temp_Orr); 
//           Right_Speed = 1.0 * High_Speed * (1.0 + Temp_Orr);
//           Left_High_Speed = Left_Speed;
//           Right_High_Speed = Right_Speed; 
//        } 
        /****************************************************************************/  
        /****************************       PID调速      ****************************/  
        /****************************************************************************/  
//        Right_MOTOR_Speed = -ftm_quad_get(FTM1);//获取FTM1 正交解码 的脉冲数(负数表示反方向)  
//        ftm_quad_clean(FTM1);   	//正交解码寄存器清零
//        //左编码器
//        Left_MOTOR_Speed = ftm_quad_get(FTM2);//获取FTM2 正交解码 的脉冲数(负数表示反方向)   
//        ftm_quad_clean(FTM2);   	//正交解码寄存器清零
//        /****************************************************************************/  
//        /****************************      速度过滤      ****************************/  
//        /****************************************************************************/   
//         Left_MOTOR_Speed      = (Left_MOTOR_Speed>400?400:Left_MOTOR_Speed); 
//         Right_MOTOR_Speed     = (Right_MOTOR_Speed>400?400:Right_MOTOR_Speed);
        Left_MOTOR_Duty += PID_Realize(&Left_MOTOR_PID, Left_MOTOR_Speed, Left_High_Speed);//使用串级增量式PID进行调节
        Right_MOTOR_Duty += PID_Realize(&Right_MOTOR_PID, Right_MOTOR_Speed, Right_High_Speed);//使用串级增量式PID进行调节

        if (Left_MOTOR_Duty>0)
        {
          Left_MOTOR_Duty = range_protect(Left_MOTOR_Duty, 0, 900);	//限幅保护
          //电机正转
          ftm_pwm_duty(MOTOR_FTM, Left_MOTOR1_PWM,0);	//占空比最大990！！！
          ftm_pwm_duty(MOTOR_FTM, Left_MOTOR2_PWM,Left_MOTOR_Duty);	   		//占空比最大990！！！

        }
        else
        {
          Left_MOTOR_Duty = range_protect(Left_MOTOR_Duty, -900, 0);
          //电机反转
          ftm_pwm_duty(MOTOR_FTM, Left_MOTOR1_PWM,-Left_MOTOR_Duty);	   			//占空比最大990！！！
          ftm_pwm_duty(MOTOR_FTM, Left_MOTOR2_PWM,0);	//占空比最大990！！！
        }
        if(Right_MOTOR_Duty>0)
        {
          Right_MOTOR_Duty = range_protect(Right_MOTOR_Duty, 0, 900);	//限幅保护
          //电机正转
          ftm_pwm_duty(MOTOR_FTM, Right_MOTOR1_PWM,0);	//占空比最大990！！！
          ftm_pwm_duty(MOTOR_FTM, Right_MOTOR2_PWM,Right_MOTOR_Duty);	   		//占空比最大990！！！
        }
        else
        {
          Right_MOTOR_Duty = range_protect(Right_MOTOR_Duty, -900, 0);	//限幅保护
          //电机反转
          ftm_pwm_duty(MOTOR_FTM, Right_MOTOR1_PWM,-Right_MOTOR_Duty);	   			//占空比最大990！！！
          ftm_pwm_duty(MOTOR_FTM, Right_MOTOR2_PWM,0);	//占空比最大990！！！
        } 
}

/******* 电机速度测量 ********/
//int32 var[4];

void speed_measure(void)
{
	//右编码器
        Right_MOTOR_Speed = -ftm_quad_get(FTM1);//获取FTM1 正交解码 的脉冲数(负数表示反方向)  
        ftm_quad_clean(FTM1);   	//正交解码寄存器清零
        //左编码器
        Left_MOTOR_Speed = ftm_quad_get(FTM2);//获取FTM2 正交解码 的脉冲数(负数表示反方向)   
        ftm_quad_clean(FTM2);   	//正交解码寄存器清零
        /****************************************************************************/  
        /****************************      速度过滤      ****************************/  
        /****************************************************************************/   
         Left_MOTOR_Speed      = (Left_MOTOR_Speed>400?400:Left_MOTOR_Speed); 
         Right_MOTOR_Speed     = (Right_MOTOR_Speed>400?400:Right_MOTOR_Speed);
        /****************************************************************************/  
        /**************************** 虚拟上位机显示速度 ****************************/  
        /****************************************************************************/   
//        var[0]=Right_MOTOR_Speed;
//        var[1]=Left_MOTOR_Speed;
//        var[2]=Left_High_Speed;
//        var[3]=Right_High_Speed;
//        vcan_sendware(var, sizeof(var));
//             DirectionControl(Point);
	MOTOR_Control();
}
void Starting_Check(void)
{
        if (Starting_Time==0 && !Go_Flag && !Stop_Flag)
        {
                Go_Flag=1;
                Starting_Time=50;  //起跑开始关闭环路检测
        }
        if (Starting_Time==0 && Go_Flag && !Annulus_Check)
        {
                Annulus_Check=1;
                Obstacle_Check=1;
        }
        if (Stop_Flag && Go_Flag)
        {
              if (Starting2_Time==0)   
              {
                  Starting2_Time=40;  //检测到起跑线以后延时停车              
              }
              

              if (Starting2_Time==10)
              {
                Go_Flag=0;
            //    Starting_Time=-10;
              }
        }

}


void DirectionControl(uchar Point)
{
	static float g_nDirectionErrorTemp[7];
		
	g_nDirectionError = (int16)Point-40;//方向偏差（80到-80）
	g_nDirectionError = (g_nDirectionError>=30?30:g_nDirectionError);//限幅
	g_nDirectionError = (g_nDirectionError<=-30?-30:g_nDirectionError);
	 
	g_nDirectionErrorTemp[5] = g_nDirectionErrorTemp[4];
	g_nDirectionErrorTemp[4] = g_nDirectionErrorTemp[3];
	g_nDirectionErrorTemp[3] = g_nDirectionErrorTemp[2];
	g_nDirectionErrorTemp[2] = g_nDirectionErrorTemp[1];
	g_nDirectionErrorTemp[1] = g_nDirectionErrorTemp[0];
	g_nDirectionErrorTemp[0] = g_nDirectionError;
	g_fDirectionError_dot = g_nDirectionErrorTemp[0]-g_nDirectionErrorTemp[3];//偏差微分
	g_fDirectionError_dot = (g_fDirectionError_dot>30?30:g_fDirectionError_dot);//微分限幅
	g_fDirectionError_dot = (g_fDirectionError_dot<-30?-30:g_fDirectionError_dot);
	
	//关联速度的动态PD
/*
	if(g_fSpeedFilter<2000)	   {g_dirControl_P =180 ;g_dirControl_D = 300;}
	else if(g_fSpeedFilter<2000){g_dirControl_P =220 ;g_dirControl_D = 330;}
	else if(g_fSpeedFilter<2100){g_dirControl_P =250 ;g_dirControl_D = 340;}
	else if(g_fSpeedFilter<2200){g_dirControl_P =300 ;g_dirControl_D = 360;}
	else if(g_fSpeedFilter<2300){g_dirControl_P =320 ;g_dirControl_D = 380;}
	else if(g_fSpeedFilter<2400){g_dirControl_P =360 ;g_dirControl_D = 410;}
	else if(g_fSpeedFilter<2500){g_dirControl_P =380 ;g_dirControl_D = 430;}
	else if(g_fSpeedFilter<2600){g_dirControl_P =410 ;g_dirControl_D = 450;}
	else if(g_fSpeedFilter<2700){g_dirControl_P =420 ;g_dirControl_D = 470;}
	else if(g_fSpeedFilter<2800){g_dirControl_P =430 ;g_dirControl_D = 500;}
	else if(g_fSpeedFilter<2900){g_dirControl_P =440 ;g_dirControl_D = 550;}
	else if(g_fSpeedFilter<3000){g_dirControl_P =450 ;g_dirControl_D = 600;}
	else if(g_fSpeedFilter>3000){g_dirControl_P =470 ;g_dirControl_D = 750;}
	else if(g_fSpeedFilter>3100){g_dirControl_P =470 ;g_dirControl_D = 800;}
*/	
	
	g_fDirectionControlOut = g_nDirectionError*g_dirControl_P + g_fDirectionError_dot*g_dirControl_D;//转向控制输出
//	g_fDirectionControlOut = g_nDirectionError*float05 + g_fDirectionError_dot*float06;//PD转向

//	g_fDirectionControlOut = (g_fDirectionControlOut > 350? 350: g_fDirectionControlOut);//转向输出限幅
//	g_fDirectionControlOut = (g_fDirectionControlOut < -350? -350: g_fDirectionControlOut);	
}


/******** 限幅保护 *********/
int32 range_protect(int32 duty, int32 min, int32 max)//限幅保护
{
	if (duty >= max)
	{
		return max;
	}
	if (duty <= min)
	{
		return min;
	}
	else
	{
		return duty;
	}
}