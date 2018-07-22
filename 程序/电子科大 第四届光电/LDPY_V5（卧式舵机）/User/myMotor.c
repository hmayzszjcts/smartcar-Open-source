

#include "myMotor.h"

  Speed_Info	Speed_info = {0};
  Motor_Info    Motor_info = {0};
  
  extern CCD_Info CCD3_info;
  extern HeadSteer_Info HeadSteer_info;
  
  extern Gyro_Info Gyro_info;
  
  extern Car_State Car_state;
  extern Car_State Car_state_Pre;
  
  extern Parameter_Info Parameter_info;
  extern Road_Type Road_type;
  
  extern Road_Condition Road_condition;
  
 #ifdef DMA_CH2_SendLabVIEW_Info_Enable

	extern LabVIEW_Info LabVIEW_info;
	
#endif

/*************************************************************************
*				零	度	偏	移  
*
*  函数名称：myMotor_DataInit
*  功能说明：电机相关参数的初始化
*  参数说明：
*  函数返回：无
*  修改时间：2014-06-21
*  备    注：
*************************************************************************/
void myMotor_DataInit(Motor_Info *Motor_info)
{
	Motor_info->KPT = 900;
	Motor_info->KP = 300.0;
	Motor_info->KI = 100;
	Motor_info->KD = 8.0;
	
	Motor_info->KP_Mult = 0;
	Motor_info->KI_Mult = 0;
	Motor_info->KD_Mult = 0;
	
	Motor_info->PWMOutputDuty = 0;		
}

/*************************************************************************
*					零		度		偏		移  
*
*  函数名称：myMotor_SpeedDataInit
*  功能说明：速度相关参数的初始化,单位为脉冲数
*  参数说明：
*  函数返回：无
*  修改时间：2014-06-21
*  备    注：
*************************************************************************/
void myMotor_SpeedDataInit(Speed_Info *Speed_info)
{
	uint8_t  ii = 0;
	
  	Speed_info->Straight_Speed = 74;				//直道速度
	Speed_info->Cur_Speed = 68;				 		//弯道速度
	Speed_info->Snake_Speed = 72;					//小S速度
	Speed_info->TurnRound_Speed = 50;				//调头速度
	Speed_info->Obstacle_Speed = 70;				//路障速度
	Speed_info->RampUp_Speed = 30;					//上坡道速度
	Speed_info->RampDown_Speed = 60;				//下坡道速度
	Speed_info->Angle60_Speed = 50;					//60度尖角速度
	
	Speed_info->Speed_Now = 0;
	Speed_info->TargetSpeed[0] = 0;
	Speed_info->TargetSpeed[1] = 0;
	Speed_info->SpeedError = 0;
	Speed_info->LastSpeedError = 0;
	Speed_info->SpeedRAcc = 0;
	
	Speed_info->Speed_Direction = 0;
	Speed_info->EncoderData = 0;
	
	for(ii = 0; ii < SPEED_FIFO_LENGTH; ii++)
	{
		Speed_info->SpeedFifo[ii] = 0;
	}
	
	Speed_info->SpeedMode = 1;						//默认为1档
	Speed_info->LastSpeedMode = 1;
	
	Speed_info->SpeedAdd_3 = 20;					//3档直道最高速度增加值
	Speed_info->SpeedMode3Add = 0;					//3档速度增加值
	Speed_info->SpeedMode3Acc_Counter = 0;
	Speed_info->CurSpeedAcc_Flag = 0;				//出弯道加速标记清零
	Speed_info->CurSpeedAcc = 0;					//出弯道加速值
	
	Speed_info->Error_K = 18;
	Speed_info->Error_D_K = 18;
	Speed_info->HeadSteerPWMError_K = 750;
	Speed_info->HeadSteerPWMError_D_K = 300;

	Speed_info->DistanceTo1CmCount = 0;				//1cm脉冲计数
	Speed_info->DistanceOf1Cm_Flag = 0;				//1cm到了标志
	Speed_info->DistanceOfCross_Counter = 0;
	Speed_info->DistanceOfTurnRound_Counter = 0;
	Speed_info->DiatanceToTurnRound = 0;			//识别人字道时距离尖角的距离
	Speed_info->DistanceBeginTurn = 0;				
	Speed_info->DistanceToTurn_MIN = 0;
	Speed_info->DistanceToTurn_MAX = 20;
	Speed_info->DistanceOfRampUp[0] = 100;
	Speed_info->DistanceOfRampUp[1] = 100;
	Speed_info->DistanceOfRampDown = 60;
	Speed_info->DistanceOfRampUp_Counter = 0;
	Speed_info->DistanceOfObstacle_Counter = 0;	
	Speed_info->DistanceOfSpeedMode3_Counter = 0;	
	Speed_info->DistanceOfSpeedMode3_Max = 800;		//3档高速距离最长为8m
	Speed_info->DistanceOfRoadInvalid_Counter = 0;
	Speed_info->DistanceOfCCD1GetSL_Counter = 0;
	Speed_info->DistanceOf60Angle_Counter = 0;
	
	Speed_info->SpeedCalculate_Enable = TRUE;		//默认闭环
}

/*************************************************************************
*					零		度		偏		移  
*
*  函数名称：myMotor_UpdateEncoder
*  功能说明：编码器相关的数据更新
*  参数说明：
*  函数返回：无
*  修改时间：2014-06-21
*  备    注：
*************************************************************************/
void myMotor_UpdateEncoder(	CCD_Info *CCD1_info,
							CCD_Info *CCD2_info,
							Motor_Info *Motor_info,
							Speed_Info *Speed_info,
							Parameter_Info *Parameter_info,
							RemSpeedUpDown_Info *RemSpeedUpDown_info,
							Road_Condition *Road_condition,
							Car_State *Car_state,
							Car_State *Car_state_Pre
						  )
{
	uint8_t ii = 0;
	
	//计算脉冲数
	Speed_info->SpeedFifo[0] = 0 - Speed_info->EncoderData;
	
	//编码器限幅
	if(fabs(Speed_info->SpeedFifo[0]) > 165)//3ms采样周期对应的实际速度为 8m/s
	{
		Speed_info->SpeedFifo[0] = Speed_info->SpeedFifo[1];
	}
	
	//1cm脉冲累加
	Speed_info->DistanceTo1CmCount += Speed_info->SpeedFifo[0];
	
	//------------------------距离控制----------------------------//
	if(Speed_info->DistanceTo1CmCount >= 68)//1cm到了
	{
		Speed_info->DistanceTo1CmCount -= 68;
		Speed_info->DistanceOf1Cm_Flag = 1;		//标记1cm到了
		
		//行驶距离自加
		Parameter_info->RoadDistance++;
		
		if(Speed_info->DistanceTo1CmCount >= 68)//若是速度很快，距离可能计算不准了，则应该再次计算距离
		{
			Speed_info->DistanceTo1CmCount -= 68;
			
			//行驶距离自加
			Parameter_info->RoadDistance++;
			
			//高速模式距离自减
			if(Parameter_info->Mode_HighSpeedDistance > 0)
			{
				Parameter_info->Mode_HighSpeedDistance--;
			}
			
			//3档高速距离记录
			if((3 == Speed_info->SpeedMode) && (Speed_info->DistanceOfSpeedMode3_Counter < Speed_info->DistanceOfSpeedMode3_Max))
			{
				Speed_info->DistanceOfSpeedMode3_Counter++;//3档高速距离自加
			}
			
			//CCD1预判到起跑线距离自加
			if(1 == Parameter_info->CCD1_GetedStartEndLine_Flag)
			{
				Speed_info->DistanceOfCCD1GetSL_Counter++;
			}
			
			//人字道60度尖角预判成功
			if(Speed_info->DistanceOf60Angle_Counter > 0)
			{
				Speed_info->DistanceOf60Angle_Counter--;
			}
		}
		
		//高速模式距离自减
		if(Parameter_info->Mode_HighSpeedDistance > 0)
		{
			Parameter_info->Mode_HighSpeedDistance--;
		}
		
		//赛道无效距离自加
		if(1 == CCD1_info->RoadInvalid_Flag)
		{
			Speed_info->DistanceOfRoadInvalid_Counter++;
			
			if(Speed_info->DistanceOfRoadInvalid_Counter >= 5)//超过5cm则强制有效
			{
				CCD1_info->RoadInvalid_Flag = 0;
				
				Speed_info->DistanceOfRoadInvalid_Counter = 0;//清零道路无效距离
			}
		}
		
		//CCD1预判到起跑线距离自加
		if(1 == Parameter_info->CCD1_GetedStartEndLine_Flag)
		{
			Speed_info->DistanceOfCCD1GetSL_Counter++;
			
			if(Speed_info->DistanceOfCCD1GetSL_Counter >= 50)//50cm内没有检测到起跑线则清零该预判
			{
				Speed_info->DistanceOfCCD1GetSL_Counter = 0;
				Parameter_info->CCD1_GetedStartEndLine_Flag = 0;
			}
		}
		
		//人字道60度尖角预判成功
		if(Speed_info->DistanceOf60Angle_Counter > 0)
		{
			Speed_info->DistanceOf60Angle_Counter--;
			
			if((Speed_info->DistanceOf60Angle_Counter <= 0) 
				|| (1 == CCD3_info.Cross_Flag)
				|| (1 == CCD2_info->Cross_Flag)
				|| (1 == CCD2_info->Cross_Flag))
			{
				Parameter_info->Left60Angle_Flag = 0;
				Parameter_info->Right60Angle_Flag = 0;
			}
			
			Bell_ON;
		}
		
		//3档高速距离记录
		if((3 == Speed_info->SpeedMode) && (Speed_info->DistanceOfSpeedMode3_Counter < Speed_info->DistanceOfSpeedMode3_Max))
		{
			Speed_info->DistanceOfSpeedMode3_Counter++;//3档高速距离自加
		}
		else if((2 == Speed_info->SpeedMode))//速度减下来的时候再清零3档高速距离
		{
			Speed_info->DistanceOfSpeedMode3_Counter = 0;
		}
		
		/*-------------------------------------------------------------------------------------------*/
		/*---------------------------------特殊路段的距离控制-------------------------------------*/
		/*-------------------------------------------------------------------------------------------*/
		//十字道距离控制
		if((1 == CCD2_info->Cross_Flag) 
			&& (Speed_info->DistanceOfCross_Counter <= 0) 
			&& (TRUE == RemSpeedUpDown_info->RemRoad_Enable))//记忆算法已经开启)
		{
			Speed_info->DistanceOfCross_Counter++;
		}
		else if(Speed_info->DistanceOfCross_Counter > 0)
		{
			Speed_info->DistanceOfCross_Counter++;
			
			if(Speed_info->DistanceOfCross_Counter >= 60)//60cm一次十字道计数
			{
				if(RemSpeedUpDown_info->Cross_Counter < 16)
				{
					RemSpeedUpDown_info->Cross_Counter++;
					RemSpeedUpDown_info->CrossSpeedUp[RemSpeedUpDown_info->Cross_Counter - 1][0] = Parameter_info->RoadDistance;//记录此时的赛道距离
					
					Speed_info->DistanceOfCross_Counter = 0;//清零十字道距离
				}
			}
		}
		
		//------------------------路障距离控制----------------------------//
		if(Speed_info->DistanceOfObstacle_Counter > 0)//处于路障段
		{
			Speed_info->DistanceOfObstacle_Counter--;//路障距离自减
			
			if(Speed_info->DistanceOfObstacle_Counter <= 0 )
			{
				if(Obstacle == *Road_condition)
				{
					*Road_condition = NeedJudge;//回到正常状态
				}
				
				CCD2_info->CCD_PhotoCenter = 63;
				Parameter_info->LeftOrRightObstacle_Flag = 0;
				Parameter_info->GetObstacle_Enable = TRUE;//路障距离过后开启路障的再次识别
			}
		}
		
		//------------------------坡道距离控制-----------------------------//
		//如果坡道预判成功
		if((1 == Parameter_info->RampReady_FLag) && (Ramp_Up != *Road_condition) && (Ramp_Down != *Road_condition))
		{
			if(Parameter_info->RampReady_Distance < 120)//120cm内没有判断到坡道则清除该标记
			{
				Parameter_info->RampReady_Distance++;
			}
			else
			{
				Parameter_info->RampReady_Distance = 0;
				Parameter_info->RampReady_FLag = 0;//清零坡道预判标记
			}
		}
		
		//处于坡道上时的距离控制
		if((Ramp_Up == *Road_condition) || (Ramp_Down == *Road_condition))
		{
			Speed_info->DistanceOfRampUp_Counter++;//坡道距离自加
			
			if((Ramp_Up == *Road_condition) && (Speed_info->DistanceOfRampUp_Counter >= Speed_info->DistanceOfRampUp[RemSpeedUpDown_info->Ramp_Counter - 1]))//超过上坡距离后自动回到下坡状态
			{
				*Road_condition = Ramp_Down;
				
				Gyro_info.RampUpDown_Num = 0;//清零坡道判断计数
			}
			else if((Ramp_Down == *Road_condition) && (Speed_info->DistanceOfRampUp_Counter >= (Speed_info->DistanceOfRampUp[RemSpeedUpDown_info->Ramp_Counter - 1] + Speed_info->DistanceOfRampDown)))
			{
				*Road_condition = NeedJudge;
				
				Speed_info->DistanceOfRampUp_Counter = Parameter_info->RoadDistance;//记录此时总的行驶距离
			}
		}
		else if((FALSE == Parameter_info->GetRamp_Enable) 
			&& ((int32_t)(Parameter_info->RoadDistance - Speed_info->DistanceOfRampUp_Counter) > 50))//50cm以后识别新的坡道和人字道
		{
			if((All == Road_type) && (Turn_Round != *Car_state))//只有当包含全部赛道种类的时候才开启人字道识别，否则不识别人字道
			{
				Parameter_info->GetTurnRound_Enable = TRUE;
			}
			
			//1.5m以后识别新的坡道
			if((int32_t)(Parameter_info->RoadDistance - Speed_info->DistanceOfRampUp_Counter) > 150)
			{
				Parameter_info->RampReady_Distance = 0;		//清零坡道预判距离计数
				Parameter_info->RampReady_FLag = 0;			//清零坡道预判标记
				
				Speed_info->DistanceOfRampUp_Counter = 0;
				
				Parameter_info->GetStartEndLine_Enable = TRUE;//可以检测起跑线
				
				if((All == Road_type) || (Ramp_Only == Road_type))//防止启动的时候自动开启坡道的识别
				{
					Parameter_info->GetRamp_Enable = TRUE;
				}
			}
		}
		
		//-----------------------------------人字道距离控制------------------------------//
		if (Speed_info->DiatanceToTurnRound > 0)//已经记录了距离人字道尖角的距离
		{
			Speed_info->DiatanceToTurnRound--;//到人字道尖角的距离自减
		}
		
		if((Turn_Round == *Car_state) || (Turn_Round == *Car_state_Pre))//处于人字道
		{
			Speed_info->DistanceOfTurnRound_Counter++;//人字道距离自加
			
			#ifdef RemRoad_Control_Enable
				if(Speed_info->DistanceOfTurnRound_Counter >= 150)
				{
					//可以在此处记录记忆算法人字道后高速模式的初始距离
					if(TRUE == RemSpeedUpDown_info->RemRoad_Enable)//记忆算法已经开启
					{
						if(RemSpeedUpDown_info->TurnRound_Counter < 2)
						{
							RemSpeedUpDown_info->TurnRound_Counter++;
						}
							
						RemSpeedUpDown_info->TurnRoundSpeedUp[RemSpeedUpDown_info->TurnRound_Counter - 1][0] = Parameter_info->RoadDistance;//记录此时的总路程
					}
				}
			#endif
			
			if((0 == CCD1_info->LeftLoseLine_Flag) && (0 == CCD1_info->RightLoseLine_Flag)
				&& (0 == CCD2_info->LeftLoseLine_Flag) && (0 == CCD2_info->RightLoseLine_Flag)
				&& (Speed_info->DistanceOfTurnRound_Counter > 100))//CCD1有两条边即可识别坡道
			{
				Parameter_info->GetRamp_Enable = TRUE;		//可以检测坡道
			}
			
			if(Speed_info->DistanceOfTurnRound_Counter > 200)//从识别到人字道开始200cm后可以再次检测人字道
			{
				*Car_state_Pre = Go_Forward;
				
				//清零人字道测距
				Speed_info->DiatanceToTurnRound = 0;
				Speed_info->DistanceOfTurnRound_Counter = 0;
				Speed_info->DistanceBeginTurn = 0;
				
				Parameter_info->GetTurnRound_Enable = TRUE;	//可以再次检测人字道
				
				if((Ramp_Up != *Road_condition) && (Ramp_Down != *Road_condition))
				{
					Parameter_info->GetRamp_Enable = TRUE;		//不在坡道则可以检测坡道
				}
				
			}
			else if((Turn_Round == *Car_state) && (Speed_info->DistanceOfTurnRound_Counter > 100))//人字道距离超过100cm还未寻到线则强制回正
			{
				*Car_state = Go_Forward;
				*Car_state_Pre = Turn_Round;
				
				//清零人字道标记
				Parameter_info->LeftIntoTurnRound_Flag = 0;
				Parameter_info->RightIntoTurnRound_Flag = 0;
				
				//清零识别人字道的CCD标记
				Parameter_info->CCD2OrCCD3_GetTurnRound = 0;
				
				//摇头舵机输出
				HeadSteer_info.Steer_PWMOutputDuty = HeadSteer_info.Steer_Center;
				FTM2->CONTROLS[1].CnV = HeadSteer_info.Steer_Center;
			}
		}
	}
	
	//得到车当前的实际速度(脉冲数)
	Speed_info->Speed_Now = ( 	  Speed_info->SpeedFifo[0]
								+ Speed_info->SpeedFifo[1]
								+ Speed_info->SpeedFifo[2]
							) / 3;
	
	//更新速度队列(脉冲数),元素0（最新的速度）在前面速度采集的时候更新
	for(ii = SPEED_FIFO_LENGTH - 1; ii > 0; ii--)
	{
		Speed_info->SpeedFifo[ii] = Speed_info->SpeedFifo[ii - 1];
	}
	
	//移动目标速度队列
	Speed_info->TargetSpeed[1] = Speed_info->TargetSpeed[0];
}

/*************************************************************************
*					零		度		偏		移  
*
*  函数名称：myMotor_CalculateTargetSpeed
*  功能说明：小车目标速度计算
*  参数说明：
*  函数返回：无
*  修改时间：2014-06-21
*  备    注：根据摇头舵机的打角和打角后的CCD中线余差来进行速度计算
*************************************************************************/
void myMotor_CalculateTargetSpeed(	CCD_Info *CCD1_info,
									CCD_Info *CCD2_info,
									CCD_Info *CCD3_info,
									HeadSteer_Info *HeadSteer_info,
									Steer_Info *Steer_info,
									Speed_Info *Speed_info
								 )
{
	int16_t ii = 0, LineError_MAX = 0,LineError_D_MAX = 0;
	int32_t HeadSteerPWMError_MAX = 0, HeadSteerPWMError_D_MAX = 0,SteerPWMError_MAX = 0;
	uint8_t SpeedMode = Speed_info->SpeedMode; 
	
	switch(SpeedMode)
	{
		case 1://CCD1
			break;
		case 2://CCD2
			for(ii = 0; ii < 20; ii++)
			{
				//找出CCD最近最大偏差
				if(LineError_MAX < fabs(CCD2_info->LineError[ii]))
				{
					LineError_MAX = fabs(CCD2_info->LineError[ii]);
				}
				
				//找出CCD最近最大偏差变化率
				if(LineError_D_MAX < fabs(CCD2_info->LineError_D[ii]))
				{
					LineError_D_MAX = fabs(CCD2_info->LineError_D[ii]);
				}
			}
			break;
		case 3://CCD3
			break;
		default:
			break;
	}
	
	//------------限制CCD偏差的最大值--------------------//
	if(LineError_MAX > Speed_info->Error_K)
	{
		LineError_MAX = Speed_info->Error_K;
	}
	
	//------------限制CCD偏差变化率的最大值--------------------//
	if(LineError_D_MAX > Speed_info->Error_D_K)
	{
		LineError_D_MAX = Speed_info->Error_D_K;
	}
	
	//------------找出摇头舵机最近偏差和偏差率最大值----------------//
	for(ii = 0; ii < 10; ii++)
	{
		//摇头舵机打角偏差最大值
		if(HeadSteerPWMError_MAX < fabs(HeadSteer_info->SteerPWM_Error[ii]))
		{
			HeadSteerPWMError_MAX = fabs(HeadSteer_info->SteerPWM_Error[ii]);
		}
		
		//摇头舵机打角偏差率最大值
		if(HeadSteerPWMError_D_MAX < fabs(HeadSteer_info->SteerPWM_Error_D[ii]))
		{
			HeadSteerPWMError_D_MAX = fabs(HeadSteer_info->SteerPWM_Error_D[ii]);
		}
		
		//转向舵机打角偏差最大值
		if(SteerPWMError_MAX < fabs(Steer_info->SteerPWM_Error[ii]))
		{
			SteerPWMError_MAX = fabs(Steer_info->SteerPWM_Error[ii]);
		}
	}
	
	//------------限制摇头舵机偏差最大值------------------//
	if(HeadSteerPWMError_MAX > Speed_info->HeadSteerPWMError_K)
	{
		HeadSteerPWMError_MAX = Speed_info->HeadSteerPWMError_K;
	}
	
	//-------------限制摇头舵机偏差率最大值--------------//
	if(HeadSteerPWMError_D_MAX > Speed_info->HeadSteerPWMError_D_K)
	{
		HeadSteerPWMError_D_MAX = Speed_info->HeadSteerPWMError_D_K;
	}
	
	//------------限制转向舵机偏差最大值------------------//
	if(SteerPWMError_MAX > Speed_info->HeadSteerPWMError_K)
	{
		SteerPWMError_MAX = Speed_info->HeadSteerPWMError_K;
	}
	
	//直道或者缓弯进入十字道继续使用3档
	if(	   (2 == SpeedMode) 
		&& (1 == CCD3_info->Cross_Flag) 
		&& (0 == CCD1_info->LeftLoseLine_Flag) 
		&& (0 == CCD1_info->RightLoseLine_Flag))
	{
		SpeedMode = 3;
	}
	else if(   (2 == SpeedMode)
			&& (CCD1_info->CCD_Ready_Num >= CCD1_DataReady_Num) 
			&& (CCD2_info->CCD_Ready_Num >= CCD2_DataReady_Num)
			&& (0 == CCD1_info->Cross_Flag)
			&& (0 == CCD2_info->Cross_Flag)
			&& (0 == Parameter_info.RampReady_FLag)
			&& ((0 == CCD3_info->LeftLoseLine_Flag) || (0 == CCD3_info->RightLoseLine_Flag)))//防掉轮程序启动时继续使用3档的速度
	{
	 
		Speed_info->SpeedMode3Acc_Counter++;
		
		if(Speed_info->SpeedMode3Acc_Counter >= 50)//连续有效50次才换到3档，防止抖动
		{
			SpeedMode = 3;
		}
	}
	else if((2 == SpeedMode) || (1 == SpeedMode))
	{
		Speed_info->SpeedMode3Acc_Counter = 0;
	}
	
	//弯道里面加速设定
	if((( 1== SpeedMode) || (2 == SpeedMode))
		&& (1 != Speed_info->CurSpeedAcc_Flag) && (Speed_info->Speed_Now <= Speed_info->Cur_Speed)
		&& (fabs(Steer_info->SteerPWM_Error[0]) >= 900))//转向舵机打到极限之后开始加速
	{
		Speed_info->CurSpeedAcc_Flag = 1;
	}
	else if(fabs(Steer_info->SteerPWM_Error[0]) < 100)//舵机回正则停止加速
	{
		Speed_info->CurSpeedAcc_Flag = 0;
		
		Speed_info->CurSpeedAcc = 0;//清零弯道加速值
	}
	
	switch(SpeedMode)//根据档位调整速度,一并考虑设定了人字道前为低速模式的情况
	{
		case 1://使用弯道速度匀速
			{
				if((Turn_Round != Car_state) && (Turn_Round != Car_state_Pre))//不处于人字转角处
				{
					Speed_info->TargetSpeed[0] = Speed_info->Cur_Speed - (Speed_info->Straight_Speed - Speed_info->Snake_Speed);
				}
				else
				{
					Speed_info->TargetSpeed[0] = Speed_info->TurnRound_Speed;
				}
				
				Speed_info->SpeedMode3Add = 0;//清零3档速度增加值
			}
			break;
		case 2://需要根据偏差减速
			{
				if(0 == Parameter_info.RampReady_FLag)
				{
				
				Speed_info->TargetSpeed[0] =  Speed_info->Straight_Speed
										   - (Speed_info->Straight_Speed - Speed_info->Cur_Speed)*HeadSteerPWMError_MAX/Speed_info->HeadSteerPWMError_K
										   - (Speed_info->Straight_Speed - Speed_info->Snake_Speed)*HeadSteerPWMError_D_MAX/Speed_info->HeadSteerPWMError_D_K;
				}
				else
				{
					Speed_info->TargetSpeed[0] = Speed_info->RampUp_Speed;
				}
				
				Speed_info->SpeedMode3Add = 0;//清零3档速度增加值
			}
			break;
		case 3://高速前进，并少量减速,防止换到低速档后无法正常减速
			{
				if(Speed_info->DistanceOfSpeedMode3_Counter >= Speed_info->DistanceOfSpeedMode3_Max)//达到了3档使用最长距离,则除去3档最高速度加速
				{
					//此时应该去掉直道最高加速
					Speed_info->TargetSpeed[0] =  Speed_info->Straight_Speed;
					
					Speed_info->SpeedMode3Add = 0;//清零3档速度增加值
				}
				else
				{
					if(Speed_info->SpeedMode3Add < Speed_info->SpeedAdd_3)
					{
						Speed_info->SpeedMode3Add += 1;//每个周期增加1个脉冲,逐渐增加到最高值，防止速度突变引起抖动
					}
					
					//没有进入人字道检测时间
					if(Parameter_info.Time < Parameter_info.GetTurnRoundTime)
					{
						Speed_info->TargetSpeed[0] = Speed_info->Straight_Speed + Speed_info->SpeedMode3Add
												   - Speed_info->SpeedMode3Add*HeadSteerPWMError_MAX/Speed_info->HeadSteerPWMError_K;
					}
					else//进入人字道检测时间，则去掉3档最高速度加速值
					{
						Speed_info->TargetSpeed[0] = Speed_info->Straight_Speed + (0 == Parameter_info.SlowBeforeTurnRound_Flag)*Speed_info->SpeedMode3Add
												   - ((0 == Parameter_info.SlowBeforeTurnRound_Flag)*Speed_info->SpeedMode3Add)*HeadSteerPWMError_MAX/Speed_info->HeadSteerPWMError_K;
					}
				}
			}
			break;
		default:
			break;
	}//end of switch(Speed_info->SpeedMode)
	
	if(Parameter_info.Mode_HighSpeedDistance > 0)
	{
		Speed_info->TargetSpeed[0] = Speed_info->Straight_Speed + Speed_info->SpeedAdd_3;
	}
	
	//人字道尖角判断成功,设定为人字道尖角速度
	if(((1 == Parameter_info.Left60Angle_Flag) || (1 == Parameter_info.Right60Angle_Flag))
		&& (Speed_info->TargetSpeed[0] > Speed_info->Angle60_Speed))
	{
		Speed_info->TargetSpeed[0] = Speed_info->Angle60_Speed;
	}
	
	//目标速度限幅，防止速度意外计算错误
	if(Speed_info->TargetSpeed[0] > Speed_info->Straight_Speed + Speed_info->SpeedAdd_3)
	{
		Speed_info->TargetSpeed[0] = Speed_info->Straight_Speed + Speed_info->SpeedAdd_3;
	}
	else if(Speed_info->TargetSpeed[0] < 10)
	{
		Speed_info->TargetSpeed[0] = 10;
	}
}

/*************************************************************************
*					零		度		偏		移  
*
*  函数名称：myMotor_PIDCalculate
*  功能说明：电机PID计算
*  参数说明：
*  函数返回：	当前应该输出的PWM占空比（增量式PID计算的结果）
*  				符号为正时电机正转，符号为负时电机反转
*  修改时间：2014-06-21
*  备    注：
*************************************************************************/
int32 myMotor_PIDCalculate(	Speed_Info *Speed_info,
							Motor_Info *Motor_info,
							Parameter_Info * Parameter_info)
{
  	int32 MotorPWMDuty = 0;				//最终得到的PWM占空比
	int32 SpeedDelta = 0;				//算出的偏差(脉冲数)
	uint8 ii = 0;
	
	//检测到起跑线
	if(1 == Parameter_info->StartEndLine_Flag)
	{
		Speed_info->TargetSpeed[0] = 0;
	}
	
	//计算速度偏差(脉冲数)
	Speed_info->LastSpeedError = Speed_info->SpeedError;
	Speed_info->SpeedError = Speed_info->TargetSpeed[0] - Speed_info->Speed_Now;
	
	//计算速度偏差变化率(加速度)
	Speed_info->SpeedRAcc = 0;
	
	for(ii = 0; ii < 5; ii++)
	{
		Speed_info->SpeedRAcc += Speed_info->SpeedFifo[ii];
	}
	
	for(ii = 5; ii < 10; ii++)
	{
		Speed_info->SpeedRAcc -= Speed_info->SpeedFifo[ii];
	}
	
	SpeedDelta = (int32_t)(    Motor_info->KPT*(Speed_info->TargetSpeed[0] - Speed_info->TargetSpeed[1])
							 + Motor_info->KP*(Speed_info->SpeedError - Speed_info->LastSpeedError)
							 + Motor_info->KI*(Speed_info->SpeedError)
							 - Motor_info->KD*(Speed_info->SpeedRAcc)
						  );
		
	//发送临时变量至上位机
	#ifdef DMA_CH2_SendLabVIEW_Info_Enable
	
		#ifndef DMA_CH2_SendImage_Enable
	
		LabVIEW_info.Motor_KP_Item = Motor_info->KP*(Speed_info->SpeedError - Speed_info->LastSpeedError);
		LabVIEW_info.Motor_KI_Item = Motor_info->KI*(Speed_info->SpeedError);
		LabVIEW_info.Motor_KD_Item = Motor_info->KD*(Speed_info->SpeedRAcc);
	
		#endif
	
	#endif
		
	/*-------------------------遇限削弱和抗积分饱和处理----------------------*/
	if((SpeedDelta > 0) && (Motor_info->PWMOutputDuty >= MOTOR_PWM_OUTPUT_MAX))//当前电机输出的PWM占空比已经最大且增量还为正时不再增大PWM占空比
	{
		SpeedDelta = 0;
	}
	else if((SpeedDelta < 0) && (Motor_info->PWMOutputDuty <= MOTOR_PWM_OUTPUT_MIN))//当前电机输出的PWM占空比已经最小且增量还为负时不再减小PWM占空比(包含负值)
	{
		SpeedDelta = 0;
	}
	
	//更新电机的PWM输出
	MotorPWMDuty = Motor_info->PWMOutputDuty + SpeedDelta;
	
	//--------------------------------Bang-Bang算法-----------------------------------------//
	//判断是否需要急加减速，如果需要则去除PID作用直接全力加减速
	if(Speed_info->SpeedError > 12)
	{
		MotorPWMDuty = MOTOR_PWM_OUTPUT_MAX;//直接全速前进
	}
	else if(Speed_info->SpeedError < -12)
	{
		MotorPWMDuty = MOTOR_PWM_OUTPUT_MIN;//直接全速反转刹车
	}
		
	//--------------------------PWM输出占空比最后总的限幅--------------------------------//
	if(MotorPWMDuty > MOTOR_PWM_OUTPUT_MAX)
	{
		MotorPWMDuty = MOTOR_PWM_OUTPUT_MAX;
	}
	else if(MotorPWMDuty < MOTOR_PWM_OUTPUT_MIN)
	{
		MotorPWMDuty = MOTOR_PWM_OUTPUT_MIN;
	}
	
	return MotorPWMDuty;
}

/*************************************************************************
*					零		度		偏		移  
*
*  函数名称：myMotor_Control
*  功能说明：电机PWM输出控制
*  参数说明：
*  函数返回：无
*  修改时间：2014-06-21
*  备    注：
*************************************************************************/
void myMotor_Control(	Motor_Info *Motor_info,
						Speed_Info *Speed_info,
						Parameter_Info * Parameter_info
					)
										
{
	if(TRUE == Speed_info->SpeedCalculate_Enable)//电机闭环
	{
		Motor_info->PWMOutputDuty = myMotor_PIDCalculate(Speed_info,Motor_info,Parameter_info);
	}
	else//电机开环
	{
		if(1 == Parameter_info->StartEndLine_Flag)
		{
			Motor_info->PWMOutputDuty = 0;
		}
		else
		{
			Motor_info->PWMOutputDuty = MOTOR_PWM_OUTPUT_DUTY;
		}
	}
	
	//如果倒退超过3m的距离，则自动停止电机,防止意外错误导致撞车
	if((Speed_info->Speed_Now < 0)&& (Speed_info->DistanceTo1CmCount < -20400) && (Motor_info->PWMOutputDuty < -1000))//倒退时该脉冲计数为负
	{
		Motor_info->PWMOutputDuty = 0;
	}
	
	//对电机的输出限幅
	if(Motor_info->PWMOutputDuty > 9990)
	{
		Motor_info->PWMOutputDuty = 9990;
	}
	else if(Motor_info->PWMOutputDuty < -9990)
	{
		Motor_info->PWMOutputDuty  = -9990;
	}

	//电机输出
	if(Motor_info->PWMOutputDuty > 0)//电机正转
	{
		FTM0->CONTROLS[6].CnV = Motor_info->PWMOutputDuty;
		FTM0->CONTROLS[7].CnV = 0;
	}
	else//电机反转
	{
		FTM0->CONTROLS[6].CnV = 0;
		FTM0->CONTROLS[7].CnV = -Motor_info->PWMOutputDuty;
	}
}

/*************************************************************************
*					零		度		偏		移   
*
*  函数名称：myMotor_Init
*  功能说明：电机的初始化
*  参数说明：
*  函数返回：无
*  修改时间：2014-06-21
*  备    注：
*************************************************************************/
void myMotor_Init(void)
{
	PORTD->PCR[7]=PORT_PCR_MUX(4);			//D7:FTM0_CH7
	PORTD->PCR[6]=PORT_PCR_MUX(4);			//D6:FTM0_CH7
			
	SIM->SCGC6|=SIM_SCGC6_FTM0_MASK;			//使能时钟
	
	FTM0->MODE|=FTM_MODE_WPDIS_MASK;			//写保护禁止
			
	//----------------------CnSC通道状态控制---------------------------//
	//----------------------边沿对齐PWM先高后低  		
	FTM0->CONTROLS[7].CnSC |= FTM_CnSC_ELSB_MASK;		//左对齐，先高后低
	FTM0->CONTROLS[7].CnSC &=~FTM_CnSC_ELSA_MASK; 		//
	FTM0->CONTROLS[7].CnSC |= FTM_CnSC_MSB_MASK;		//模式选着边沿对齐模式
		
	FTM0->CONTROLS[6].CnSC |= FTM_CnSC_ELSB_MASK;		//左对齐，先高后低
	FTM0->CONTROLS[6].CnSC &=~FTM_CnSC_ELSA_MASK; 		//
	FTM0->CONTROLS[6].CnSC |= FTM_CnSC_MSB_MASK;		//模式选着边沿对齐模式

	//---------------------FTM0状态控制--------------------------------
	//[7]TOF定时器溢出标志
	//[6]TOFIE定时器溢出中断使能
	//[5]CPWMS=0 计数器加法计数
	//[4:3]CLKS=01 system clock
	//[2:0]PS 预分频比=2^PS			
	FTM0->SC=0x08;		//中断禁止,时钟源，分频16，递增计数

	//---------------------特性模式选着-----------------------------------
  	FTM0->MODE &=~ 1;					//使能FTM0
			
	//---------------------输出屏蔽寄存器---------------------------------
	FTM0->OUTMASK =~ (FTM_OUTINIT_CH7OI_MASK | FTM_OUTINIT_CH6OI_MASK);	//通道7 6输出，其它禁止
	FTM0->OUTINIT |=0xC0;
	
	//---------------------正交解码状态控制-------------------------------
	FTM0->QDCTRL &=~ FTM_QDCTRL_QUADEN_MASK;	//禁止正交解码模式
		
	//---------------------通道联合功能-----------------------------------
	FTM0->COMBINE = 0 ;	//双边沿捕捉禁止，不级联
		
	FTM0->CNTIN = 0 ;					//计数器初始值 0
	FTM0->MOD = 10000;					//计数器终止值
	
	//----------------------计数值------------------------------------------
	FTM0->CONTROLS[7].CnV = 0;			//高电平时间
	FTM0->CONTROLS[6].CnV = 0;			//高电平时间
	
	FTM0->CNT = 0;						//计数器归0
}

/*************************************************************************
*					零		度		偏		移  
*
*  函数名称：myMotor_EncoderInit
*  功能说明：编码器初始化
*  参数说明：
*  函数返回：无
*  修改时间：2014-05-09
*  备    注：
*************************************************************************/
void myMotor_EncoderInit(void)
{
  
	PORTA->PCR[8]=PORT_PCR_MUX(6);			// A8为FTM_PH1A
	PORTA->PCR[9]=PORT_PCR_MUX(6);			// A9为FTM_PH1B
	
	SIM->SCGC6 |= SIM_SCGC6_FTM1_MASK;			//使能FTM1时钟
	
	FTM1->MODE |= FTM_MODE_WPDIS_MASK;			//写保护禁止
	
	FTM1->QDCTRL |= FTM_QDCTRL_QUADMODE_MASK;		//启用计数和方向编码模式
	
	FTM1->CNTIN = 0;						//计数器初始值
	FTM1->MOD = 65535;  					//计数器结束值
	
	FTM1->QDCTRL |= FTM_QDCTRL_QUADEN_MASK;		//启用FTM1正交编码模式
	FTM1->MODE |= FTM_MODE_FTMEN_MASK;			//使能FTM1
	FTM1->CNT = 0;						//计数器值清零
	
}

/*--------------------------end of myMotor.c--------------------------------*/


