
#include "myCarControl.h"

extern DMA_InitTypeDef dma_init_struct;

extern uint16_t StopType;
extern Car_Mode Car_mode;

#ifdef RemRoad_Control_Enable
	
	extern RemSpeedUpDown_Info RemSpeedUpDown_info;
	
#endif

#ifdef DMA_CH2_SendLabVIEW_Info_Enable
	
	extern LabVIEW_Info LabVIEW_info;
	
#endif

	extern DMA_Required_Variable DMA_Variable_1;

/*************************************************************************
*					零		度		偏		移   
*  函数名称: myCarControl_CarControl
*  功能说明: 小车综合控制函数
*  参数说明:
*  函数返回:
*  修改时间:2014-07-04
*  备	注:
*************************************************************************/
void myCarControl_CarControl(	CCD_Info *CCD1_info,
								CCD_Info *CCD2_info,
								CCD_Info *CCD3_info,
								HeadSteer_Info *HeadSteer_info,
								Steer_Info *Steer_info,
								Motor_Info *Motor_info,
								Speed_Info *Speed_info,
								Parameter_Info *Parameter_info,
								RemSpeedUpDown_Info *RemSpeedUpDown_info,
								Road_Condition *Road_condition,
								Car_State *Car_state,
								Car_State *Car_state_Pre
							)
{	
	Bell_OFF;

	//编码器更新速度和距离
	myMotor_UpdateEncoder(CCD1_info,CCD2_info,Motor_info,Speed_info,Parameter_info,RemSpeedUpDown_info,Road_condition,Car_state,Car_state_Pre);
	
	//更新偏差和计算速度
	switch(*Car_state)
	{
		case Go_Forward://正常前进
			{
				//计算偏差
				myCCD_DataHandle(CCD1_info,CCD2_info,CCD3_info,HeadSteer_info,Speed_info,Parameter_info,Road_condition,Car_state,Car_state_Pre);
				
				//计算目标速度
				switch(*Road_condition)//根据不同的赛道类型给定不同速度
				{
					case Ramp_Up://上坡
						{
							Speed_info->SpeedMode = 1;//上坡道使用1档
							Speed_info->TargetSpeed[0] = Speed_info->RampUp_Speed;
						}
						break;
					case Ramp_Down://下坡
						{
							if(3 == Speed_info->SpeedMode)
							{
								Speed_info->SpeedMode = 2;
							}
							
							Speed_info->TargetSpeed[0] = Speed_info->RampDown_Speed;
						}
						break;
					case Obstacle://路障
						{
							if(3 == Speed_info->SpeedMode)//如果是3档则换为2档
							{
								Speed_info->LastSpeedMode = Speed_info->SpeedMode;
								Speed_info->SpeedMode = 2;
							}
							
							//路障时图像中心点偏移
							if(1 == Parameter_info->LeftOrRightObstacle_Flag)//左边路障
							{
								CCD2_info->CCD_PhotoCenter = 63 - CCD2_info->CCD_ObstacleShift;//左移图像中心点
							}
							else if(2 == Parameter_info->LeftOrRightObstacle_Flag)//右边路障
							{
								CCD2_info->CCD_PhotoCenter = 63 + CCD2_info->CCD_ObstacleShift;//右移图像中心点
							}
						
							Speed_info->TargetSpeed[0] = Speed_info->Obstacle_Speed;
							
							Bell_ON;
						}
						break;
					default://其他赛道类型(除开人字道)
						{
							myMotor_CalculateTargetSpeed(CCD1_info,CCD2_info,CCD3_info,HeadSteer_info,Steer_info,Speed_info);

						}
						break;
				}
			}
			break;//end of Go_Forward
			
		case Turn_Round://调头
			{
				Bell_ON;
				
				Parameter_info->Left60Angle_Flag = 0;
				Parameter_info->Right60Angle_Flag = 0;
				
				//若标记了人字道前为低速模式，则在人字道之后清零该标记
				if(1 == Parameter_info->SlowBeforeTurnRound_Flag)
				{
					Parameter_info->SlowBeforeTurnRound_Flag = 0;
				}
				
				if(3 == Parameter_info->CCD2OrCCD3_GetTurnRound)
				{
					LED4_ON;
				}
				else if(2 == Parameter_info->CCD2OrCCD3_GetTurnRound)
				{
					LED3_ON;
				}
				
				Speed_info->SpeedMode = 2;//默认使用CCD2
				
				CCD1_info->CCD_Ready_Num = 0;
				CCD2_info->CCD_Ready_Num = 0;
				CCD3_info->CCD_Ready_Num = 0;
				
				Parameter_info->GetRamp_Enable = FALSE;//确保人字道不识别坡道
				
				//CCD数据滤波和二值化
				myCCD_FilterAndBinarization(CCD1_info,CCD2_info,CCD3_info,Parameter_info);
				
				//根据此时的速度计算到尖角多少距离转角能准确过人字道
				myCarControl_CalDistanceToTurnRound(Speed_info);
				
				if(1 == Parameter_info->LeftIntoTurnRound_Flag)//从左边入人字道
				{
					//当达到转向距离(或者CCD2识别到人字道)时再转向
					if((Speed_info->DistanceOfTurnRound_Counter >= Speed_info->DistanceBeginTurn) || (2 == Parameter_info->CCD2OrCCD3_GetTurnRound))
					{
						CCD2_info->LineError[0] = 20;//强制摇头到右极限
						
						HeadSteer_info->Steer_PWMOutputDuty = HeadSteer_info->Steer_RightMAX;
					}
					
					if((HeadSteer_info->SteerPWM_Error[0] > 600) &&(Speed_info->DistanceOfTurnRound_Counter > 60))//60cm后开始巡线
					{
						if(-1 != myCCD_CCD2_GetLeftLineOutTurnRound(CCD2_info))//已经搜寻到左边界
						{
							*Car_state = Go_Forward;
							*Car_state_Pre = Turn_Round;
							
							CCD2_info->LeftLine[0] = myCCD_CCD2_GetLeftLineOutTurnRound(CCD2_info);
							CCD2_info->RightLine[0] = CCD2_info->LeftLine[0] + CCD2_info->RoadWidth[0];
							CCD2_info->CentralLine[0] = (CCD2_info->LeftLine[0] + CCD2_info->RightLine[0]) >> 1;
							
							//清零人字道标记
							Parameter_info->LeftIntoTurnRound_Flag = 0;
							Parameter_info->RightIntoTurnRound_Flag = 0;
						}
					}
				}
				else if(1 == Parameter_info->RightIntoTurnRound_Flag)//从右边入人字道
				{
					//当达到转向距离(或者CCD2识别到人字道)时再转向
					if((Speed_info->DistanceOfTurnRound_Counter >= Speed_info->DistanceBeginTurn) || (2 == Parameter_info->CCD2OrCCD3_GetTurnRound))
					{
						CCD2_info->LineError[0] = -20;//强制摇头到左极限
						
						HeadSteer_info->Steer_PWMOutputDuty = HeadSteer_info->Steer_LeftMAX;
					}
					
					if((HeadSteer_info->SteerPWM_Error[0] < -600) && (Speed_info->DistanceOfTurnRound_Counter > 60))//60cm后开始巡线
					{
						if(-1 != myCCD_CCD2_GetRightLineOutTurnRound(CCD2_info))//已经搜寻到右边界
						{
							*Car_state = Go_Forward;
							*Car_state_Pre = Turn_Round;
							
							CCD2_info->RightLine[0] = myCCD_CCD2_GetRightLineOutTurnRound(CCD2_info);
							CCD2_info->LeftLine[0] = CCD2_info->RightLine[0] - CCD2_info->RoadWidth[0];
							CCD2_info->CentralLine[0] = (CCD2_info->LeftLine[0] + CCD2_info->RightLine[0]) >> 1;
							
							//清零人字道标记
							Parameter_info->LeftIntoTurnRound_Flag = 0;
							Parameter_info->RightIntoTurnRound_Flag = 0;
						}
					}
				}		
	
				//设定人字调头路口速度
				Speed_info->TargetSpeed[0] = Speed_info->TurnRound_Speed;//设定为转向速度
			}
			break;//end of Turn_Round
			
		case Stop://停车
			{
				if(3 == Speed_info->SpeedMode)
				{
					Speed_info->SpeedMode = 2;//停车使用2档并减弱校正
				}
				
				Steer_info->HeadToSteer_2 = 0.6;
				Steer_info->KP2 = 6;
				
				Parameter_info->GetTurnRound_Enable = FALSE;//停车以后不检测人字道

				if(	   (0 == Speed_info->SpeedFifo[0]) 
					&& (0 == Speed_info->SpeedFifo[1])
					&& (0 == Speed_info->SpeedFifo[2]))//停车完成后显示相关数据
				{
					myOLED_Dec(6,10,StopType);//显示起跑线检测停车时红外管的标号
					
					myOLED_Dec(5,10,Parameter_info->RoadDistance);//显示行驶距离

					myOLED_Decimals(4,10,Parameter_info->Time);//显示行驶时间
				}
				
				//计算偏差
				myCCD_DataHandle(CCD1_info,CCD2_info,CCD3_info,HeadSteer_info,Speed_info,Parameter_info,Road_condition,Car_state,Car_state_Pre);
				
				//目标速度给零
				Speed_info->TargetSpeed[0] = 0;
				
				Parameter_info->StartEndLine_Flag = 1;//置位起跑线检测到标记,相当于检测到起跑线
			}
			break;//end of Stop
			
		default :
			break;
		
	}//end of switch(*Car_state)

	//不是调试模式才控制舵机和电机
	if((CheckCCD != Car_mode) && (CheckSL != Car_mode))
	{
		//控制摇头舵机
		mySteer_HeadSteerControl(CCD1_info,CCD2_info,CCD3_info,HeadSteer_info,Speed_info,Car_state,Car_state_Pre);
		
		//控制转向舵机
		mySteer_DirectionControl(CCD1_info,CCD2_info,CCD3_info,HeadSteer_info,Steer_info,Speed_info);
		
		//控制电机
		if(CheckRamp != Car_mode)
		{
			myMotor_Control(Motor_info,Speed_info,Parameter_info);
		}
	}
	
#ifdef DMA_CH0_Enable//开DMA，发送CCD图像
	
	LDPY_DMA_SendThreeCCDsImage(&DMA_Variable_1,CCD1_info,CCD2_info,CCD3_info);
	
	DMA0->ERQ |= (1 << DMA_CH0);
	
#endif
//end of DMA_CH0_Enable
	
//发送临时变量给上位机
#ifdef DMA_CH2_SendLabVIEW_Info_Enable
		
	LDPY_DMA_SendTempVar(&LabVIEW_info,&DMA_Variable_1,CCD1_info,CCD2_info,CCD3_info,HeadSteer_info,Steer_info,Motor_info,Speed_info);
	
	DMA0->ERQ |= (1 << DMA_CH2);
	
#endif
	
}

/*************************************************************************
*					零		度		偏		移  
*  函数名称: myCarControl_CalDistanceToTurnRound
*  功能说明: 根据当前速度计算到人字道尖角多远处可以转角
*  参数说明:	
*  函数返回:
*  修改时间:2014-07-07
*  备	注:
*************************************************************************/
void myCarControl_CalDistanceToTurnRound(Speed_Info *Speed_info)
{	
	//计算舵机打死角时的距离   = 当前到尖角的距离 - K*当前的速度
	Speed_info->DistanceBeginTurn = Speed_info->DiatanceToTurnRound - (0.80*Speed_info->Speed_Now);
	
	//检查参数的正确性
	if(Speed_info->DistanceToTurn_MIN > Speed_info->DistanceToTurn_MAX)//若是最小限定值大于最大限定值
	{
		Speed_info->DistanceToTurn_MIN = Speed_info->DistanceToTurn_MAX;//以最大限定值为准
	}
	
	//对人字道开始转角的距离限幅
    if(Speed_info->DistanceBeginTurn > Speed_info->DistanceToTurn_MAX)//最多到了尖角才转角
	{
		Speed_info->DistanceBeginTurn = Speed_info->DistanceToTurn_MAX;
	}
	else if(Speed_info->DistanceBeginTurn < Speed_info->DistanceToTurn_MIN)//最少5cm后转角
	{
		Speed_info->DistanceBeginTurn = Speed_info->DistanceToTurn_MIN;
	}
}

/*************************************************************************
*					零		度		偏		移  
*  函数名称: myCarControl_Ready_Go
*  功能说明: 发车延时
*  参数说明:         
*  函数返回:
*  修改时间:2014-06-21
*  备	注:
*************************************************************************/
void myCarControl_Ready_Go(void)
{	   
	 LDPY_Delay_s(1);
	 Bell_ON;
	 LDPY_Delay_s(1);
	 Bell_OFF;	 

	 myOLED_Clear();
}

/*----------------------------end of myCarControl.c-----------------------*/
