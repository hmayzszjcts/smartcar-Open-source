#include "Direction_Control.h"



        /****************************    舵机控制变量    ****************************/ 
uint8    screen_mid = 40;		  //屏幕中点
uint8    Point_Mid = 40;
uint8    Point_Left = 43;//43
uint8    Point_Right = 37;//37
uint8    Hazard_Left_Right=0;
int32    S3010_Duty = 4740;      //       //4320  4740 5180
uint8    Point;	//目标点横坐标
int8     Point_Num=0;
int8     CS_Point_Num=0;

        /****************************    差速控制变量    ****************************/ 
int16    a;
int32    Angle;
uint8    CS_Left=11,CS_Right=11;
uint8     CS_Point;
int32    CS_Duty;      //       //4320  4740 5180
float   Left_Speed = 0;         
float   Right_Speed = 0; 
float   Temp_Orr;         
float   Temp_Add;

float   CS_P_Right,CS_P_Left;

        /****************************   进出弯识别变量   ****************************/ 
//uint8   Went_Curve=0;     //进弯道标志位
//uint8   Out_Curve=0;      //出弯道标志位
//uint8   Straight_Time=0;    //直道时间常数
//uint8   Point_iE=0;         //中线偏差
//uint8   Out_Curve_Time=0;   //出弯时间常数
//uint8   Went_Curve_Time=0;  //进弯时间常数


void S3010_Control(void)
{
        Point_Error = Point-40;
        Point_Error = ABS(Point_Error);
        S3010_Duty = PlacePID_Control(&S3010_PID,Point_Mid,Point);	//使用位置式PID解算	     
		S3010_Duty = range_protect(S3010_Duty, S3010_RIGHT, S3010_LEFT);          //舵机占空比限幅保护
		ftm_pwm_duty(S3010_FTM, S3010_CH, S3010_Duty);			//修改舵机占空比
}

void CS_Control(void)
{
        CS_Point = Motor_Point_Weight(CS_Point_Num);		// 正值 图像向下平移 提前转向	必须为偶数
        CS_Duty = PlacePID_CS(&CS_PID,Point_Mid,CS_Point);	//使用位置式PID解算	
		CS_Duty = range_protect(CS_Duty, S3010_RIGHT, S3010_LEFT);          //舵机占空比限幅保护
        Angle = S3010_MID - CS_Duty;
        
//        Angle = S3010_MID - S3010_Duty;
//        save_var(VAR5, S3010_Duty);
//        save_var(VAR6, Angle);
      

        /****************************************************************************/  
        /****************************    舵机角度差速    ****************************/  
        /****************************************************************************/ 
       
        if(Angle>0)         //右拐
        {
           a=(int16)(45*(Angle)*1.0/(S3010_LEFT - S3010_MID)) ;  //angle_max   45
           if(a>45)  a=45;
           if(a<0)   a=0;
           Left_Speed = High_Speed;
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
//        Left_High_Speed = N32_Average_Filter(Left_High_Speed, Left_Value_Buf);
//        Right_High_Speed = N32_Average_Filter(Right_High_Speed, Right_Value_Buf);
        
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
        /****************************      阿克曼减差速      ****************************/  
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
        /****************************     阿克曼加减差速      ****************************/  
        /****************************************************************************/   
        
//         if(Angle>0)
//        {
//           a=(int16)(45*(Angle)*1.0/(S3010_LEFT - S3010_MID)) ;   //angle_max   45
//           if(a>45)  a=45;
//           if(a<0)   a=0;
//           Temp_Orr = tan((a*3.14)/180) * 13.4 / 40;          //15.4
//           Left_Speed = 1.0 * High_Speed * (1.0 + 0.8*Temp_Orr); //第一个常数可以加大差速，第二个常数可以提前差速
//           Right_Speed = 1.0 * High_Speed * (1.0 - 1.0*Temp_Orr);
//           Left_High_Speed = Left_Speed;
//           Right_High_Speed = Right_Speed; 
//        }
//        else
//        {
//          a=(int16)(45*(-Angle)*1.0/(S3010_MID - S3010_RIGHT)) ;  //angle_max   45
//          if(a>45)  a=45;
//           if(a<0)   a=0;
//           Temp_Orr = tan((a*3.14)/180) * 13.4 / 40;
//           Left_Speed = 1.0 * High_Speed * (1.0 - 1.0*Temp_Orr); 
//           Right_Speed = 1.0 * High_Speed * (1.0 + 0.8*Temp_Orr);
//           Left_High_Speed = Left_Speed;
//           Right_High_Speed = Right_Speed; 
//        } 
}


void Curve_Judge(void)
{
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
}


