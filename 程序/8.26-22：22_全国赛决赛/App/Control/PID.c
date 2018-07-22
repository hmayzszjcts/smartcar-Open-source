#include "PID.h"


PID S3010_PID, Left_MOTOR_PID, Right_MOTOR_PID, CS_PID;	//定义舵机和电机的PID参数结构体
//int32 S3010[2][4] = {{17, 0, 23, 25},{ 20, 1, 0, 19}}; //0-2.55, 1-2.
//int32 S3010[5][4] = {{ 16, 0, 23, 20},{18, 0, 23, 20},{ 20, 0, 23, 20},{ 12, 0, 23, 20},{ 20, 0, 23, 20},}; //0-2.55, 1-2.

int32 S3010[5][4] = {{ 17, 0, 23, 20},{18, 0, 23, 20},{ 20, 0, 23, 20},{ 12, 0, 23, 20},{ 20, 0, 23, 20},}; //0-2.55, 1-2.


/* PID 的 I 项没有使用，只用 PD6此处 I 仅作为速度模式的标号，对PID无影响 */					
//当前误差，定义为寄存器变量，只能用于整型和字符型变量，提高运算速度
int32 CS[2][4] = {{ 17, 0, 23, 20},{18, 0, 23, 20}};

//float   Left_MOTOR[3] = {7.8, 0.96, 3.6};	//电机PID
//float   Right_MOTOR[3] = {7.8, 0.96, 3.6};	//电机PID
//
float   Left_MOTOR[3] = {7.8, 2.96, 3.6};	//电机PID
float   Right_MOTOR[3] = {7.8, 2.96, 3.6};	//电机PID

//float   Left_MOTOR[3] = {10.0, 2.5, 1.6};	//电机PID
//float   Right_MOTOR[3] = {10.0, 2.5, 1.6};	//电机PID

//float   Left_MOTOR[3] = {5.8, 2.96, 2.6};	//电机PID
//float   Right_MOTOR[3] = {5.8, 2.96, 2.6};	//电机PID

//float   Left_MOTOR[3] = {3.5, 2.8, 0.2};	//电机PID
//float   Right_MOTOR[3] = {3.5, 2.8, 0.2};	//电机PID

//float   Left_MOTOR[3] = {2.5, 4.5, 0.8};	//电机PID
//float   Right_MOTOR[3] = {2.5, 4.5, 0.8};	//电机PID

//float   Left_MOTOR[3] = {2.5, 0.5, 0.8};	//电机PID
//float   Right_MOTOR[3] = {2.5, 0.5, 0.8};	//电机PID
//位置式PID参数初始化
void PlacePID_Init(PID *sptr)
{
	sptr->SumError = 0;
	sptr->LastError = 0;	//Error[-1]
	sptr->PrevError = 0;	//Error[-2]
}
//增量式PID参数初始化
void IncPID_Init(PID *sptr, float *MOTOR_PID)
{
	sptr->SumError = 0;
	sptr->LastError = 0;	//Error[-1]
	sptr->PrevError = 0;	//Error[-2]	
	sptr->LastSpeed = 0;
        sptr->KP = *MOTOR_PID;
        sptr->KI = *(MOTOR_PID+1);
        sptr->KD = *(MOTOR_PID+2);
        
}

//位置式差速控制
int32 PlacePID_CS(PID *sprt, int32 NowPiont, int32 SetPoint)
{
	//定义为寄存器变量，只能用于整型和字符型变量，提高运算速度
	int32 iError,	//当前误差
		  Actual;	//最后得出的实际输出值
	float	Kp;		//动态P

	
	iError = SetPoint - NowPiont;	//计算当前误差
	Kp = 1.0 * (iError*iError) / CS[Set][T] + CS[Set][P];	//P值与差值成二次函数关系
	
	Actual = Kp * iError + CS[Set][D] * (iError - sprt->LastError);//只用PD
	
	sprt->LastError = iError;		//更新上次误差

	
	return S3010_MID-Actual;
}

//位置式PID舵机控制
int32 PlacePID_Control(PID *sprt, int32 NowPiont, int32 SetPoint)
{
	//定义为寄存器变量，只能用于整型和字符型变量，提高运算速度
	int32 iError,	//当前误差
		  Actual;	//最后得出的实际输出值
	float	Kp;		//动态P

	
	iError = SetPoint - NowPiont;	//计算当前误差
	Kp = 1.0 * (iError*iError) / S3010[Set][T] + S3010[Set][P];	//P值与差值成二次函数关系
	
	Actual = Kp * iError + S3010[Set][D] * (iError - sprt->LastError);//只用PD
	
	sprt->LastError = iError;		//更新上次误差

	
	return S3010_MID-Actual;
}
int32 PID_Cascade(PID *sptr, int32 ActualSpeed, int32 SetSpeed)
{
	register int32 iError,	    //当前误差
					Increase;	//最后得出的实际增量
	iError = SetSpeed - ActualSpeed;//计算当前误差
	
	Increase =  -sptr->KP * (ActualSpeed - sptr->LastSpeed)	//加速度
			   + sptr->KI * iError
			   + sptr->KD * (iError - 2 * sptr->LastError + sptr->PrevError);
	
	sptr->PrevError = sptr->LastError;	//更新前次误差
	sptr->LastError = iError;		  	//更新上次误差
	sptr->LastSpeed = ActualSpeed;		//更新上次速度
	
//	if (iError < -100)
//	{
//		Increase -= 400;
//	}
//	else if (iError < -80)
//	{
//		Increase -= 300;
//	}
//	else if (iError < -50)
//	{
//		Increase -= 250;
//	}
//	else if (iError < -30)
//	{
//		Increase -= 200;
//	}	
	return Increase;
        
}	


//增量式PID电机控制    
int32 PID_Realize(PID *sptr, int32 ActualSpeed, int32 SetSpeed)
{
	//当前误差，定义为寄存器变量，只能用于整型和字符型变量，提高运算速度
//	register int32 iError,	    //当前误差
//					Increase;	//最后得出的实际增量
	
  	int32 iError,	    //当前误差
					Increase;	//最后得出的实际增量
  
	iError = SetSpeed - ActualSpeed;//计算当前误差
	
	Increase = sptr->KP * (iError - sptr->LastError)
			 + sptr->KI * iError
			 + sptr->KD * (iError - 2 * sptr->LastError + sptr->PrevError);
	sptr->PrevError = sptr->LastError;	//更新前次误差
	sptr->LastError = iError;		  	//更新上次误差
	
//	if (iError < -50)
//	{
//		BangBang_Flag = 1;
//	}
//	else if (iError < -50 && BangBang_Flag)
//	{
//		BangBang_Flag = 0;
//		Increase -= 300;
//	}
//    if (iError > 100)
//	{
//		Increase +=  800;
//	}
//    else if (iError > 50)
//    {
//        Increase +=  400;
//    }
////	else if (iError < -40)
////	{
////		BangBang_Flag = 1;
////		Increase -= 200;
////	}
//
//        
//        if (iError > 100)
//	{
//		Increase +=  600;
//	}
////	else if ((iError > 40) && (BangBang_Flag == 1))
////	{
////		BangBang_Flag = 0;
////		Increase +=  400;
////	}
//	else if (iError > 50)
//	{
//		Increase += 300;
//                BangBang_Flag = 1;
//	}
	
	return Increase;
}



