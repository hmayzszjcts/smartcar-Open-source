
#include "myGyro.h"

Gyro_Info Gyro_info = {0};

extern CCD_Info CCD2_info;
extern CCD_Info CCD3_info;

extern Speed_Info Speed_info;
extern Parameter_Info Parameter_info;

extern RemSpeedUpDown_Info RemSpeedUpDown_info;

/*************************************************************************
*					零		度		偏		移                             
*
*  函数名称:myGyro_DataInit
*  功能说明:陀螺仪/加速度计数据初始化
*  参数说明:
*  函数返回:
*  修改时间:2014-06-26
*  备    注：
*************************************************************************/
void myGyro_DataInit(Gyro_Info *Gyro_info)
{
	uint8_t ii = 0;
	
	Gyro_info->GyroscopeZero = 2208;				//陀螺仪零点电压
	
	for(ii = 0; ii < GYRO_LENGTH; ii++)
	{
		Gyro_info->Gyroscope_Fifo[ii] = 2208;
	}
	
	Gyro_info->Gyro_Sum = 0;
	Gyro_info->RampThresholdValue = 250;
	
	Gyro_info->RampUpDown_Num = 0;
	
}

/*************************************************************************
*					零		度		偏		移                             
*
*  函数名称:myGyro_GetRamp
*  功能说明:采集陀螺仪/加速度计
*  参数说明:
*  函数返回:
*  修改时间:2014-07-11
*  备    注：
*************************************************************************/	
Road_Condition myGyro_GetRamp(CCD_Info *CCD1_info,Gyro_Info *Gyro_info,Speed_Info *Speed_info,Road_Condition *Road_condition)
{
	uint8_t ii = 0;

	Gyro_info->Gyro_Sum = 0;
	
	//采集陀螺仪端口的AD电压值放在了CCD采集中断里面
	//如果液晶显示太多东西的话会死机于此处采集陀螺仪AD值，估计与CCD采集中断有冲突
	
	//移动陀螺仪AD值队列
	for(ii = GYRO_LENGTH - 1; ii > 0; ii--)
	{
		Gyro_info->Gyroscope_Fifo[ii] = Gyro_info->Gyroscope_Fifo[ii - 1];
	}
	
	//求最新的5次AD值之和
	for(ii = 0; ii < 5; ii++)
	{
		Gyro_info->Gyro_Sum += Gyro_info->Gyroscope_Fifo[ii];
	}
	
	Gyro_info->Gyro_Sum = Gyro_info->Gyro_Sum / 5;
	
	//检测上坡
	if((Ramp_Up != *Road_condition) && (Ramp_Down != *Road_condition)
		&& (0 == CCD1_info->LeftLoseLine_Flag) && (0 == CCD1_info->RightLoseLine_Flag)
		&& ((int16_t)(Gyro_info->Gyro_Sum - Gyro_info->GyroscopeZero) < -Gyro_info->RampThresholdValue))
	{
		if((Gyro_info->RampUpDown_Num < 20) && (1 == Speed_info->DistanceOf1Cm_Flag))//连续检测到上坡
		{
			Gyro_info->RampUpDown_Num++;
		}
		
		if((Gyro_info->RampUpDown_Num >= 15) 
			|| ((1 == Parameter_info.RampReady_FLag) && (Gyro_info->RampUpDown_Num >= 5)))//当有坡道预判的时候允许次数减少
		{
			Gyro_info->RampUpDown_Num = 0;//清零坡道判断次数
			Parameter_info.RampReady_FLag = 0;//清零坡道预判标记
			
			Parameter_info.GetRamp_Enable = FALSE;//已经处于坡道则在一定距离内不再检测坡道
			Parameter_info.GetTurnRound_Enable = FALSE;//坡道不检测人字道
			Parameter_info.GetStartEndLine_Enable = FALSE;//坡道不检测起跑线

			//可以在此处记录记忆算法坡道后高速模式的初始距离
			if(RemSpeedUpDown_info.Ramp_Counter < 2)
			{
				RemSpeedUpDown_info.Ramp_Counter++;
			}
			
			RemSpeedUpDown_info.RampSpeedUp[RemSpeedUpDown_info.Ramp_Counter - 1][0] = Parameter_info.RoadDistance;//记录此时的总路程
			
			
			return Ramp_Up;
		}
		else 
		{
			return NeedJudge;
		}
	}
	//检测下坡
	else if((Ramp_Up == *Road_condition) && (Speed_info->DistanceOfRampUp_Counter >= Speed_info->DistanceOfRampUp[RemSpeedUpDown_info.Ramp_Counter - 1] - 20)
			&& (((int16_t)(Gyro_info->Gyro_Sum - Gyro_info->GyroscopeZero) > 200) || (TRUE == myCCD_JudgeRampDown(CCD1_info,&CCD2_info,&CCD3_info,Road_condition))))
	{
		if((Gyro_info->RampUpDown_Num < 6) && (1 == Speed_info->DistanceOf1Cm_Flag))//连续检测到下坡
		{
			Gyro_info->RampUpDown_Num++;
		}
		
		if(Gyro_info->RampUpDown_Num >= 5)
		{
			Gyro_info->RampUpDown_Num = 0;//清零坡道判断计数
		
			return Ramp_Down;
		}
		else //没有检测到下坡则一直是上坡
		{
			return Ramp_Up;
		}
	}
	//下坡的时候检测坡底
	else if((Ramp_Down == *Road_condition) 
			&& (Speed_info->DistanceOfRampUp_Counter >= Speed_info->DistanceOfRampUp[RemSpeedUpDown_info.Ramp_Counter - 1] + 30)
			&& ((int16_t)(Gyro_info->Gyro_Sum - Gyro_info->GyroscopeZero) < -Gyro_info->RampThresholdValue))
	{
		if((Gyro_info->RampUpDown_Num < 6) && (1 == Speed_info->DistanceOf1Cm_Flag))//连续检测到坡底
		{
			Gyro_info->RampUpDown_Num++;
		}
		
		if(Gyro_info->RampUpDown_Num >= 5)
		{
			Gyro_info->RampUpDown_Num = 0;//清零坡道判断计数
		
			Speed_info->DistanceOfRampUp_Counter = Parameter_info.RoadDistance;//记录此时总的行驶距离
			
			return NeedJudge;
  		}
		else //没有检测到坡底则一直是下坡
		{
			return *Road_condition;
		}
	}
	else
	{
		Gyro_info->RampUpDown_Num = 0;//清零坡道判断计数
		
		return *Road_condition;//其他情况返回原来的路况类型
	}

}

/*-------------------------------end of myGyro.c-------------------------------*/
