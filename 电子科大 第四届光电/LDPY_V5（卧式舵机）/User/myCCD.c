

#include "myCCD.h"

 CCD_Info CCD1_info = {0};
 CCD_Info CCD2_info = {0};
 CCD_Info CCD3_info = {0};
 
 extern Parameter_Info Parameter_info;
 extern HeadSteer_Info HeadSteer_info;
 extern Steer_Info Steer_info;
 extern Motor_Info Motor_info;
 extern Speed_Info Speed_info;
 extern Road_Condition Road_condition;
 extern Gyro_Info Gyro_info;
 
 extern Car_State Car_state;
 extern Car_State Car_state_Pre;
 
 extern Car_Mode Car_mode;
 
 extern uint16_t StopType;
 
 extern DMA_Required_Variable DMA_Variable_1;
 
/*************************************************************************
*					零		度		偏		移                           
*
*  函数名称:myCCD_DataInit(CCD_Info * CCD_info)
*  功能说明:CCD 数据初始化
*  参数说明:
*  函数返回:
*  修改时间:2014-06-21
*  备    注：
*************************************************************************/	
void myCCD_DataInit(CCD_Info * CCD_info)
{
	uint8_t ii = 0, jj = 0;
	 
	for (ii = 0; ii < 2; ii++)
	{
		for (jj = 0; jj < 20; jj++)
		{
			CCD_info->PixelOri[ii][jj] = 0;
			CCD_info->PixelOri[ii][127 - jj] = 0;
		}
			
		for (jj = 20; jj < 107; jj++)
		{
			CCD_info->PixelOri[ii][jj] = 2000;
		}
	}
	
	for (ii = 0; ii < 128; ii++)
	{
		CCD_info->Pixel[ii] = 2000;
		CCD_info->PixelAfterMult[ii] = 2000;
		CCD_info->PixelBinary[ii] = 0;
	}
	
	CCD_info->CCD_PhotoCenter = 63;
	CCD_info->CCD_ObstacleShift = 2;					//默认偏移两个点
	
	for (ii = 0; ii < Line_SIZE; ii++)
	{
	    CCD_info->LeftLine[ii] = 0;
		CCD_info->RightLine[ii] = 0;
		CCD_info->CentralLine[ii] = Photo_Center;
		CCD_info->LineError[ii] = 0;
		CCD_info->LineError_D[ii] = 0;
	}
	
	CCD_info->LeftLossLinePixel = 0;
	CCD_info->RightLossLinePixel = 0;
	CCD_info->LeftLoseLine_Flag = 0;
	CCD_info->RightLoseLine_Flag = 0;
	
	for (ii = 0; ii < 10; ii++)
	{
		CCD_info->RoadWidth[ii] = 50;
	}
	
	CCD_info->RoadWidthOfStraight = 70;
	
	CCD_info->LeftLine[0] = 28;
	CCD_info->RightLine[0] = 100;
	
	for (ii = 0; ii < 4; ii++)
	{
		CCD_info->AD_MAX[ii] = 1200;
		CCD_info->AD_MIN[ii] = 0;
	}
	
	for (ii = 0; ii < 16; ii++)
	{
		CCD_info->CCD_PhotoValue[ii] = 0;
	}
	
	for(ii = 0; ii < ANGLE_LENGTH; ii++)
	{
		CCD_info->Angle_LeftFifo[ii] = 20;
		CCD_info->Angle_RightFifo[ii] = 20;
	}

	CCD_info->InvalidPixel_Num = 15;	//图像无效点数初始化为15
	CCD_info->CCD_Ready_Num = 0;
	
	CCD_info->AddLine_Flag = 0;			//补线标记
	CCD_info->LossLine_Flag = 0;		//丢线标记
	CCD_info->Cross_Flag = 0;			//十字道标记
	CCD_info->RoadInvalid_Flag = 0;
}
 
/*************************************************************************
*					零		度		偏		移                                
*
*  函数名称:myCCD_DataHandle
*  功能说明:CCD数据处理
*  参数说明:
*  函数返回:无
*  修改时间:2014-06-26
*  备    注: 
*************************************************************************/	
void myCCD_DataHandle(	CCD_Info *CCD1_info,
						CCD_Info *CCD2_info,
						CCD_Info *CCD3_info,
						HeadSteer_Info *HeadSteer_info,
						Speed_Info *Speed_info,
						Parameter_Info *Parameter_info,
						Road_Condition *Road_condition,
						Car_State *Car_state,
						Car_State *Car_state_Pre
				   )
{
	uint8_t ii = 0;
	
	//数据滤波和二值化
	myCCD_FilterAndBinarization(CCD1_info,CCD2_info,CCD3_info,Parameter_info);

	//CCD搜边线和偏差计算
	myCCD_CCD1_GetLineError(CCD1_info,Speed_info);
	myCCD_CCD2_GetLineError(CCD1_info,CCD2_info,Speed_info);
	myCCD_CCD3_GetLineError(CCD3_info,Speed_info);
	
	//移动60度尖角队列
	if(1 == Speed_info->DistanceOf1Cm_Flag)
	{
		//移动队列
		for(ii = ANGLE_LENGTH - 1; ii > 0; ii--)
		{
			CCD3_info->Angle_LeftFifo[ii] = CCD3_info->Angle_LeftFifo[ii - 1];
			CCD3_info->Angle_RightFifo[ii] = CCD3_info->Angle_RightFifo[ii - 1];
		}
	}
		
	//识别人字道60度尖角
	if(	   (TRUE == Parameter_info->GetTurnRound_Enable)
		&& (Parameter_info->Time >= Parameter_info->GetTurnRoundTime)
		&& (1 != CCD1_info->Cross_Flag)
		&& (1 != CCD2_info->Cross_Flag))//十字道不识别60度尖角
	{
		//判断60度尖角
		myCCD_Get_60_Angle(CCD1_info,CCD2_info,CCD3_info,Speed_info,Parameter_info,Car_state);
	}

	//选择档位(选择使用CCD1，CCD2还是CCD3)
	myCCD_ChooseSpeedMode(CCD1_info,CCD2_info,CCD3_info,HeadSteer_info,Speed_info,Parameter_info);
	
	//判断直道，用于检测起跑线
	if(TRUE == Parameter_info->GetStartEndLine_SameLight_Enable)
	{
		for(ii = 0; ii < 10; ii++)
		{
			if((fabs(HeadSteer_info->SteerPWM_Error[ii]) > 500)
				|| (fabs(CCD2_info->LineError[ii]) > 16)
				|| (0 == CCD3_info->CCD_Ready_Num)
				|| (1 == CCD1_info->LossLine_Flag))
			{
				break;
			}
		}
		
		if(10 == ii)
		{
			Parameter_info->LongStraight_Flag = 1;
		}
		else 
		{
			Parameter_info->LongStraight_Flag = 0;
		}
	}
	
	#ifdef DMA_CH1_Enable//开DMA，发送偏差等参数

		LDPY_DMA_UpdateFifo_Parameters(CCD1_info,&Steer_info,&Motor_info,Speed_info,Road_condition);
		
		DMA0->ERQ |= (1 << DMA_CH1);
	
	#endif
	//end of DMA_CH1_Enable
	
	#ifdef DMA_CH2_Enable//开DMA,发送压缩图像
		#ifdef DMA_CH2_SendImage_Enable
			if (1 == Speed_info->DistanceOf1Cm_Flag)//1cm发送一次
			{
				LDPY_DMA_UpdateFifo_PhotoValue(CCD1_info,CCD2_info,CCD3_info);
			
				DMA0->ERQ |= (1 << DMA_CH2);
			}
		#endif
	#endif
	//end of DMA_CH2_Enable
			
	/*---------------------------CCD1识别起跑线-------------------------------------------------------*/
	if(    (Stop != *Car_state)
		&& (Ramp_Up != *Road_condition)
		&& (Ramp_Down != *Road_condition)
		&& (1 != Parameter_info->CCD1_GetedStartEndLine_Flag)
		&& (1 == Parameter_info->GetStartEndLine_Flag)
		&& (TRUE == Parameter_info->GetStartEndLine_Enable)
		&& (TRUE == Parameter_info->CCD1GetSL_Enable)
		&& (Parameter_info->Time >= Parameter_info->GetStartEndLineTime)
		&& (TRUE == myCCD_CCD1_GetStartEndLine(CCD1_info,CCD2_info,CCD3_info,Road_condition,Car_state)))
	{
		Parameter_info->CCD1_GetedStartEndLine_Flag = 1;//CCD1预判起跑线成功
	}
	else if((1 == Parameter_info->CCD1_GetedStartEndLine_Flag))//CCD1起跑线预判成功后应该及时清除，防止误判
	{
		if((1 == CCD2_info->LossLine_Flag)
			|| (1 == CCD2_info->AddLine_Flag)
			|| (1 == CCD3_info->LossLine_Flag)
			|| (fabs(CCD2_info->LineError[0]) > 20))
		{
			Parameter_info->CCD1_GetedStartEndLine_Flag = 0;
			Speed_info->DistanceOfCCD1GetSL_Counter = 0;
		}
	}
	
	/*---------------------------在摇头舵机打角很小的时候进行坡道,路障判断-----------------------*/
	//判断坡道(包括上坡和下坡)
	if(	   ((Go_Forward == *Car_state) && (TRUE == Parameter_info->GetRamp_Enable) && (Parameter_info->Time >= Parameter_info->GetRampTime))
		|| ((Ramp_Up == *Road_condition) || (Ramp_Down == *Road_condition)))//检测上坡，或者在上坡的时候检测下坡和坡底
	{
		*Road_condition = myGyro_GetRamp(CCD1_info,&Gyro_info,Speed_info,Road_condition);
	}
	
	//判断路障
	if(	(Go_Forward == *Car_state)
		&& (Ramp_Up != *Road_condition) && (Ramp_Down != *Road_condition) && (Obstacle != *Road_condition)
		&& (TRUE == Parameter_info->GetObstacle_Enable)
		&& (Parameter_info->Time >= Parameter_info->GetObstacleTime)
		&& (0 != myCCD_GetObstacle(CCD1_info,CCD2_info,&Steer_info)))//不处于坡道的时候进行路障判断
	{
		Parameter_info->LeftOrRightObstacle_Flag = myCCD_GetObstacle(CCD1_info,CCD2_info,&Steer_info);

		Parameter_info->GetObstacle_Enable = FALSE;//识别到路障后不再识别路障
		
		Speed_info->DistanceOfObstacle_Counter = 80;//80cm的过路障距离
		
		*Road_condition = Obstacle;//路障判断成功
	}
} 

/*************************************************************************
*					零		度		偏		移                              
*
*  函数名称:myCCD_ChooseSpeedMode		
*  功能说明: 选择使用哪一个CCD
*  参数说明:
*  函数返回:无
*  修改时间:2014-07-05
*  备    注:
*************************************************************************/	
void myCCD_ChooseSpeedMode(	CCD_Info *CCD1_info,
							CCD_Info *CCD2_info,
							CCD_Info *CCD3_info,
							HeadSteer_Info *HeadSteer_info,
							Speed_Info *Speed_info,
							Parameter_Info *Parameter_info
						  )
{	

	uint8_t OldSpeedMode = Speed_info->SpeedMode;//记录当前的速度模式
	
	Speed_info->SpeedMode = 1;//默认为1档
	
	/*------------------------------选择CCD----------------------------------*/
	if(1 == OldSpeedMode)//处于低速档(1档)
	{
		if (   (CCD2_info->CCD_Ready_Num >= 3)
			&& (0 == CCD1_info->LossLine_Flag))//CCD2有效
		{
			Speed_info->SpeedMode = 2;//使用2档
			
			if(	   (0 == CCD2_info->LossLine_Flag)
				&& (0 == CCD2_info->AddLine_Flag)
				&& (CCD3_info->CCD_Ready_Num >= 15)
				&& (FALSE == myCCD_JudgeWheelOutOfWay(CCD1_info,CCD2_info,CCD3_info,Speed_info)))//CCD2有两条边才切换到3档
			{
				Speed_info->SpeedMode = 3;//使用3档
			}
		}
	}
	else//处于高速档(2档，3档)
	{
		if (CCD2_info->CCD_Ready_Num >= 3)//CCD2有效,且补线正确,CCD1有效，防止窜道
		{
			Speed_info->SpeedMode = 2;//使用2档
			
			//处于2档
			if(    (2 == OldSpeedMode)
				&& (0 == CCD1_info->AddLine_Flag)
				&& (0 == CCD2_info->AddLine_Flag)
				&& (CCD3_info->CCD_Ready_Num >= 15)
				&& (FALSE == myCCD_JudgeWheelOutOfWay(CCD1_info,CCD2_info,CCD3_info,Speed_info)))//原来处于2档，则由2档换至3档条件要严格
			{
				Speed_info->SpeedMode = 3;//使用3档
			}
			//处于3档
			else if(   (CCD3_info->CCD_Ready_Num >= 15)
					&& (FALSE == myCCD_JudgeWheelOutOfWay(CCD1_info,CCD2_info,CCD3_info,Speed_info)))//原来处于3档,则3档使用条件放松
			{
				Speed_info->SpeedMode = 3;//使用3档
			}
		}
	}
	
	if(Speed_info->SpeedMode != OldSpeedMode)//当速度模式与上一次不同的时候改变
	{
		Speed_info->LastSpeedMode = OldSpeedMode;//记录上一次的速度模式
	}
	
	if((Ramp_Up == Road_condition) && (Speed_info->DistanceOfRampUp_Counter >= 35))//上坡超过35cm后自动认为丢线
	{
		CCD1_info->LossLine_Flag = 1;//CCD1强制丢线
		
		CCD1_info->CCD_Ready_Num = 0;
		CCD2_info->CCD_Ready_Num = 0;
		CCD3_info->CCD_Ready_Num = 0;
	}
	
	//利用CCD进行长直道进入坡道时预判坡道换挡减速
	if( (Go_Forward == Car_state)
		&& (TRUE == Parameter_info->GetRamp_Enable)
		&& (Parameter_info->Time >= Parameter_info->GetRampTime)
		&& (HeadSteer_info->SteerPWM_Error[0] < 500)
		&& (TRUE == myCCD_JudgeRampInAdvance(CCD1_info,CCD2_info,CCD3_info)))
	{
		
		if((Parameter_info->RampJgeInAdvce_Num < 30) && (1 == Speed_info->DistanceOf1Cm_Flag))
		{
			Parameter_info->RampJgeInAdvce_Num++;
		}
		
		if(Parameter_info->RampJgeInAdvce_Num >= 10)//连续10厘米预判成功
		{
			Parameter_info->RampReady_FLag = 1;//坡道预判成功
			
			if(3 == Speed_info->SpeedMode)
			{
				Speed_info->SpeedMode = 2;
			}
			
			LED3_ON;
		}
	}
	else
	{
		Parameter_info->RampJgeInAdvce_Num = 0;//清零坡道预判计数
	}
	
}

/*************************************************************************
*					零		度		偏		移                              
*
*  函数名称:myCCD_JudgeRampInAdvance		
*  功能说明:利用3个CCD进行长直道入坡道的坡道预判
*  参数说明:
*  函数返回:无
*  修改时间:2014-08-13
*  备    注: 坡道路宽连续变宽并且大于在直道上的路宽可以作为坡道
*************************************************************************/	
uint8_t myCCD_JudgeRampInAdvance(CCD_Info *CCD1_info,CCD_Info *CCD2_info,CCD_Info *CCD3_info)
{
	uint8_t ii = 0;
	
	int16_t CCD1_Left = 0,CCD1_Right = 0;
	int16_t CCD2_Left = 0,CCD2_Right = 0;
	int16_t CCD3_Left = 0,CCD3_Right = 0;

	ii = 63;
	
	if((1 == CCD1_info->PixelBinary[ii]) && (1 == CCD2_info->PixelBinary[ii]) && (1 == CCD3_info->PixelBinary[ii]))
	{
		//判断CCD3路宽是否符合
		ii = 63;
		
		//判断左边界
		while(ii > CCD3_info->InvalidPixel_Num)
		{
			if(0 == CCD3_info->PixelBinary[ii])
			{
				CCD3_Left = ii;
				break;
			}
			
			ii--;
		}
		
		ii = 63;
		
		//判断右边界
		while(ii < 127 - CCD3_info->InvalidPixel_Num)
		{
			if(0 == CCD3_info->PixelBinary[ii])
			{
				CCD3_Right = ii;
				break;
			}
			
			ii++;
		}
		
		//判断CCD1
		if((0 != CCD3_Left) && (0 != CCD3_Right) && (CCD3_Right - CCD3_Left > 50) && (CCD3_Right - CCD3_Left < ROAD_WIDETH_3_MAX))
		{
			//判断CCD2路宽是否符合
			ii = 63;
			
			//判断左边界
			while(ii > CCD2_info->InvalidPixel_Num)
			{
				if(0 == CCD2_info->PixelBinary[ii])
				{
					CCD2_Left = ii;
					break;
				}
				
				ii--;
			}
			
			ii = 63;
			
			//判断右边界
			while(ii < 127 - CCD2_info->InvalidPixel_Num)
			{
				if(0 == CCD2_info->PixelBinary[ii])
				{
					CCD2_Right = ii;
					break;
				}
				
				ii++;
			}
			
			//判断CCD2
			if((0 != CCD2_Left) && (0 != CCD2_Right) && (CCD2_Right - CCD2_Left > 66) && (CCD2_Right - CCD2_Left < ROAD_WIDETH_2_MAX))
			{
				//判断CCD1路宽是否符合
				ii = 63;
				
				//判断左边界
				while(ii > CCD1_info->InvalidPixel_Num)
				{
					if(0 == CCD1_info->PixelBinary[ii])
					{
						CCD1_Left = ii;
						break;
					}
					
					ii--;
				}
				
				ii = 63;
				
				//判断右边界
				while(ii < 127 - CCD1_info->InvalidPixel_Num)
				{
					if(0 == CCD1_info->PixelBinary[ii])
					{
						CCD1_Right = ii;
						break;
					}
					
					ii++;
				}
				
				//判断CCD1
				if((0 != CCD1_Left) && (0 != CCD1_Right) && (CCD1_Right - CCD1_Left > ROAD_WIDETH_1_MIN) && (CCD1_Right - CCD1_Left < ROAD_WIDETH_1_MAX))
				{	
					return TRUE;
				}
				else
				{
					return FALSE;
				}
			}
			else
			{
				return FALSE;
			}
		}
		else 
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
}

/*************************************************************************
*					零		度		偏		移                              
*
*  函数名称:myCCD_JudgeRampDown	
*  功能说明:利用3个CCD进行下坡判断
*  参数说明:
*  函数返回:无
*  修改时间:2014-08-14
*  备    注: 3个CCD都回到正常状态则可以作为下坡的标志
*************************************************************************/	
uint8_t myCCD_JudgeRampDown(CCD_Info *CCD1_info,CCD_Info *CCD2_info,CCD_Info *CCD3_info,Road_Condition *Road_condition)
{
	uint8_t ii = 0;
	
	int16_t CCD1_Left = 0,CCD1_Right = 0;
	int16_t CCD2_Left = 0,CCD2_Right = 0;
	int16_t CCD3_Left = 0,CCD3_Right = 0;

	ii = 63;
	
	if((Ramp_Up == *Road_condition) && (1 == CCD1_info->PixelBinary[ii]) && (1 == CCD2_info->PixelBinary[ii]) && (1 == CCD3_info->PixelBinary[ii]))
	{
		//判断CCD3路宽是否符合
		ii = 63;
		
		//判断左边界
		while(ii > CCD3_info->InvalidPixel_Num)
		{
			if(0 == CCD3_info->PixelBinary[ii])
			{
				CCD3_Left = ii;
				break;
			}
			
			ii--;
		}
		
		ii = 63;
		
		//判断右边界
		while(ii < 127 - CCD3_info->InvalidPixel_Num)
		{
			if(0 == CCD3_info->PixelBinary[ii])
			{
				CCD3_Right = ii;
				break;
			}
			
			ii++;
		}
		
		//判断CCD1
		if((0 != CCD3_Left) && (0 != CCD3_Right) && (CCD3_Right - CCD3_Left > ROAD_WIDETH_3_MIN) && (CCD3_Right - CCD3_Left < ROAD_WIDETH_3_MAX))
		{
			//判断CCD2路宽是否符合
			ii = 63;
			
			//判断左边界
			while(ii > CCD2_info->InvalidPixel_Num)
			{
				if(0 == CCD2_info->PixelBinary[ii])
				{
					CCD2_Left = ii;
					break;
				}
				
				ii--;
			}
			
			ii = 63;
			
			//判断右边界
			while(ii < 127 - CCD2_info->InvalidPixel_Num)
			{
				if(0 == CCD2_info->PixelBinary[ii])
				{
					CCD2_Right = ii;
					break;
				}
				
				ii++;
			}
			
			//判断CCD2
			if((0 != CCD2_Left) && (0 != CCD2_Right) && (CCD2_Right - CCD2_Left > ROAD_WIDETH_2_MIN) && (CCD2_Right - CCD2_Left < ROAD_WIDETH_2_MAX))
			{
				//判断CCD1路宽是否符合
				ii = 63;
				
				//判断左边界
				while(ii > CCD1_info->InvalidPixel_Num)
				{
					if(0 == CCD1_info->PixelBinary[ii])
					{
						CCD1_Left = ii;
						break;
					}
					
					ii--;
				}
				
				ii = 63;
				
				//判断右边界
				while(ii < 127 - CCD1_info->InvalidPixel_Num)
				{
					if(0 == CCD1_info->PixelBinary[ii])
					{
						CCD1_Right = ii;
						break;
					}
					
					ii++;
				}
				
				//判断CCD3
				if((0 != CCD1_Left) && (0 != CCD1_Right) && (CCD1_Right - CCD1_Left > ROAD_WIDETH_1_MIN) && (CCD1_Right - CCD1_Left < ROAD_WIDETH_1_MAX))
				{	
					return TRUE;
				}
				else
				{
					return FALSE;
				}
			}
			else
			{
				return FALSE;
			}
		}
		else 
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
}

/*************************************************************************
*					零		度		偏		移                              
*
*  函数名称:myCCD_JudgeStraightIntoCross	
*  功能说明:利用3个CCD进行长直道入十字道的判断
*  参数说明:
*  函数返回:无
*  修改时间:2014-07-05
*  备    注:
*************************************************************************/	
uint8_t myCCD_JudgeLongStraight(	CCD_Info *CCD1_info,
									CCD_Info *CCD2_info,
									CCD_Info *CCD3_info,
									HeadSteer_Info *HeadSteer_info
								)
{
	uint8_t ii = 0;
	
	//长直道判断
	for(ii = 1; ii < 31; ii++)
	{
		if(fabs(CCD3_info->LineError[ii]) >= 10)
		{
			break;
		}
		else if(fabs(CCD2_info->LineError[ii]) >= 10)
		{
			break;
		}
		else if(fabs(CCD1_info->LineError[ii]) >= 10)
		{
			break;
		}
	}
	
	if((31 == ii) && (fabs(HeadSteer_info->SteerPWM_Error[0]) < 200))//是长直道
	{	
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*************************************************************************
*					零		度		偏		移                              
*
*  函数名称:myCCD_CCD1_GetLineError		
*  功能说明:CCD1搜寻中线
*  参数说明:
*  函数返回:无
*  修改时间:2014-06-21
*  备    注: 从上次的中点往两边搜索低电平
*************************************************************************/	
void myCCD_CCD1_GetLineError(CCD_Info *CCD1_info,Speed_Info *Speed_info)
{	
	int16_t CentralLinePixel_Now = CCD1_info->CentralLine[0];//从上次的中点开始搜线
	int16_t LinePixel_Temp = CentralLinePixel_Now;
	
	int16_t Left_temp=0, Right_temp=0;
	uint8_t ii = 0;
	
	//清零标记
	CCD1_info->LossLine_Flag = 0;
	CCD1_info->Cross_Flag  = 0;
	CCD1_info->AddLine_Flag = 0;
	CCD1_info->LeftLoseLine_Flag = 1;
	CCD1_info->RightLoseLine_Flag = 1;

	if ((CentralLinePixel_Now < 2 + CCD1_info->InvalidPixel_Num) && (CCD1_info->RightLine[0] > CCD1_info->LeftLine[0]))
	{
		CentralLinePixel_Now = 2 + CCD1_info->InvalidPixel_Num;
		
		//判断赛道是否有效
		if((TRUE == Parameter_info.RoadInvalid_Enable)
			&& (Ramp_Up != Road_condition) 
			&& (Ramp_Down != Road_condition))//坡道不判定赛道无效
		{
			if(	   (0 == CCD1_info->RoadInvalid_Flag)
				&& (1 == CCD1_info->PixelBinary[CentralLinePixel_Now - 5]) 
				&& (0 == CCD1_info->PixelBinary[CentralLinePixel_Now]) 
				&& (0 == CCD1_info->PixelBinary[CentralLinePixel_Now + 5]))
			{
				CCD1_info->RoadInvalid_Flag = 1;
			}
			else if(   (1 == CCD1_info->PixelBinary[CentralLinePixel_Now - 5])
					&& (1 == CCD1_info->PixelBinary[CentralLinePixel_Now])
					&& (0 == CCD1_info->PixelBinary[CentralLinePixel_Now + 5]))
			{
				CCD1_info->RoadInvalid_Flag = 0;
				
				Speed_info->DistanceOfRoadInvalid_Counter = 0;//清零道路无效距离
			}
		}
		else 
		{
			CCD1_info->RoadInvalid_Flag = 0;
		}
	}
	else if ((CentralLinePixel_Now > 125 - CCD1_info->InvalidPixel_Num) && (CCD1_info->RightLine[0] > CCD1_info->LeftLine[0]))
	{
		CentralLinePixel_Now = 125 - CCD1_info->InvalidPixel_Num;
		
		//判断赛道是否有效
		if((TRUE == Parameter_info.RoadInvalid_Enable) 
			&& (Ramp_Up != Road_condition) 
			&& (Ramp_Down != Road_condition))//坡道不判定赛道无效
		{
			if(    (0 == CCD1_info->RoadInvalid_Flag)
				&& (0 == CCD1_info->PixelBinary[CentralLinePixel_Now - 5]) 
				&& (0 == CCD1_info->PixelBinary[CentralLinePixel_Now]) 
				&& (1 == CCD1_info->PixelBinary[CentralLinePixel_Now + 5]))
			
			{
				CCD1_info->RoadInvalid_Flag = 2;
			}
			else if(   (0 == CCD1_info->PixelBinary[CentralLinePixel_Now - 5])
					&& (1 == CCD1_info->PixelBinary[CentralLinePixel_Now])
					&& (1 == CCD1_info->PixelBinary[CentralLinePixel_Now + 5]))
			{
				CCD1_info->RoadInvalid_Flag = 0;
				
				Speed_info->DistanceOfRoadInvalid_Counter = 0;//清零道路无效距离
			}
		}
		else 
		{
			CCD1_info->RoadInvalid_Flag = 0;
		}
	}
	else if (CCD1_info->LeftLine[0] >= CCD1_info->RightLine[0])
	{
		CentralLinePixel_Now = Photo_Center;
		
		//左右边界出现异常，用上一次的左右边界值来替代
		CCD1_info->LeftLine[0] = CCD1_info->LeftLine[1];
		CCD1_info->RightLine[0] = CCD1_info->RightLine[1];
	}

	if((Ramp_Up == Road_condition) || (Ramp_Down == Road_condition))//坡道从中间开始搜线
	{
		LinePixel_Temp = CCD1_info->CCD_PhotoCenter;
	}
	else
	{
		LinePixel_Temp = CentralLinePixel_Now;
	}
	
	/*------------------左边线------------------------*/
	if (0 == CCD1_info->RoadInvalid_Flag)//赛道有效时才进行搜线
	{		
		ii = LinePixel_Temp;
		
		while(ii > CCD1_info->InvalidPixel_Num)
		{
			if(1 == CCD1_info->PixelBinary[ii])//搜到高电平“1”
			{
				break;
			}
			
			ii--;
		}
		
		if(ii != CCD1_info->InvalidPixel_Num)//已经搜索到高电平
		{
			while (ii > CCD1_info->InvalidPixel_Num)
			{
				 if (0 == CCD1_info->PixelBinary[ii])//寻 1 -> 0 跳变
				 {
					 Left_temp = ii;
					 break;
				 }
				 else
				 {
					 ii--;//只有当前点的二值化数据为 1 时才继续往下搜寻下降沿，否则停止搜线
				 }
			}
		}
	}
	else
	{
		CCD1_info->LossLine_Flag = 1;
	}

	//左边丢线
	if (0 == Left_temp)
	{ 
		Left_temp = CCD1_info->InvalidPixel_Num; 
	}	
			
   /*-------------------右边线------------------------*/	
	if(0 == CCD1_info->RoadInvalid_Flag)//赛道有效时才进行搜线
	{
		ii = LinePixel_Temp + 1;
		
		while(ii < 127 - CCD1_info->InvalidPixel_Num)
		{
			if(1 == CCD1_info->PixelBinary[ii])//搜到高电平“1”
			{
				break;
			}
			
			ii++;
		}
		
		if(ii != 127 - CCD1_info->InvalidPixel_Num)//已经搜索到高电平
		{
			while (ii < (127 - CCD1_info->InvalidPixel_Num))
			{
				 if (0 == CCD1_info->PixelBinary[ii])//寻 1 -> 0 跳变
				 {
					 Right_temp = ii;
					 break;
				 }
				 else
				 {
					 ii++;//只有当前点的二值化数据为 1 时才继续往下搜寻下降沿，否则停止搜线
				 }
			}
		}
	}
	else
	{
		CCD1_info->LossLine_Flag = 1;
	}

	//右边丢线
	if (0 == Right_temp) 
	{
		Right_temp = 127 - CCD1_info->InvalidPixel_Num;
	}	
	
	/*------------------------------移动队列---------------------------------*/
	for (ii = Line_SIZE - 1; ii > 0; ii--)
	{
		CCD1_info->LeftLine[ii]    = CCD1_info->LeftLine[ii - 1];
		CCD1_info->RightLine[ii]   = CCD1_info->RightLine[ii - 1];
		CCD1_info->CentralLine[ii] = CCD1_info->CentralLine[ii - 1];
		CCD1_info->LineError[ii]   = CCD1_info->LineError[ii - 1];
		CCD1_info->LineError_D[ii] = CCD1_info->LineError_D[ii - 1];
	}
	
	//移动AD_MAX,AD_MIN队列
	if(1 == Speed_info->DistanceOf1Cm_Flag)
	{
		CCD1_info->AD_MAX[3] = CCD1_info->AD_MAX[2];
		CCD1_info->AD_MAX[2] = CCD1_info->AD_MAX[1];
		CCD1_info->AD_MAX[1] = CCD1_info->AD_MAX[0];
		
		CCD1_info->AD_MIN[3] = CCD1_info->AD_MIN[2];
		CCD1_info->AD_MIN[2] = CCD1_info->AD_MIN[1];
		CCD1_info->AD_MIN[1] = CCD1_info->AD_MIN[0];
	}
	
	/*------------------------------普通弯道往左补线-------------------------------*/	
	if ((Left_temp <= CCD1_info->InvalidPixel_Num) && (Right_temp < 127 - CCD1_info->InvalidPixel_Num))//限制右边界范围
	{
		CCD1_info->AddLine_Flag = 1;
		
		CCD1_info->LeftLine[0]  = Right_temp -  CCD1_info->RoadWidth[0];
		CCD1_info->RightLine[0] = Right_temp;

		if (((CCD1_info->LeftLine[0] + CCD1_info->RightLine[0]) >> 1) > CCD1_info->CCD_PhotoCenter)//防止在弯道入十字的时候补线错误
		{
			CCD1_info->LeftLine[0] = CCD1_info->LeftLine[1];
			CCD1_info->RightLine[0] = CCD1_info->RightLine[1];
		}
		else if ((1 == Speed_info->DistanceOf1Cm_Flag) 
				&& (CCD1_info->CCD_Ready_Num < CCD1_DataReady_Num))//正常补线,1cm到了,清零1cm标志放在更新摇头舵机偏差队列之后
		{
			CCD1_info->CCD_Ready_Num++;//CCD1数据有效次数自加
		}
		
		//搜寻人字道
		if ((TRUE == Parameter_info.GetTurnRound_Enable) 
			&& (Parameter_info.Time >= Parameter_info.GetTurnRoundTime) 
			&& (fabs(Steer_info.SteerPWM_Error[0]) < TurnRoundSteer_Max)
			&& (TRUE == myCCD_CCD2_GetRightIntoTurnRound(&CCD2_info)))
		{
			if(Parameter_info.TurnRound_OK_Num < Parameter_info.TurnRound_Num_MAX + 3)
			{
				Parameter_info.TurnRound_OK_Num++;
			}
			
			if(Parameter_info.TurnRound_OK_Num >= Parameter_info.TurnRound_Num_MAX + 2)
			{
				Parameter_info.TurnRound_OK_Num = 0;
				
				Parameter_info.GetTurnRound_Enable = FALSE;//已经处于人字道则不再检测人字道
					
				Car_state_Pre = Car_state;
				Car_state = Turn_Round;
				
				//从右边进入人字道
				Parameter_info.LeftIntoTurnRound_Flag = 0;
				Parameter_info.RightIntoTurnRound_Flag = 1;
				
				Parameter_info.CCD2OrCCD3_GetTurnRound = 2;//CCD2识别到了人字道
			}
		}
		else
		{
			Parameter_info.TurnRound_OK_Num = 0;
		}
	}
	/*------------------------------普通弯道往右补线-------------------------------*/	
	else if ((Right_temp >= 127 - CCD1_info->InvalidPixel_Num) && (Left_temp > CCD1_info->InvalidPixel_Num))//限制左边界范围
	{	
		CCD1_info->AddLine_Flag = 1;
		
		CCD1_info->RightLine[0] = Left_temp + CCD1_info->RoadWidth[0];
		CCD1_info->LeftLine[0]  = Left_temp;
		
		if (((CCD1_info->LeftLine[0] + CCD1_info->RightLine[0]) >> 1) < CCD1_info->CCD_PhotoCenter)//防止在弯道入十字的时候补线错误
		{
			CCD1_info->LeftLine[0] = CCD1_info->LeftLine[1];
			CCD1_info->RightLine[0] = CCD1_info->RightLine[1];
		}
		else if ((1 == Speed_info->DistanceOf1Cm_Flag) 
				&& (CCD1_info->CCD_Ready_Num < CCD1_DataReady_Num))//正常补线,1cm到了,清零1cm标志放在更新摇头舵机偏差队列之后
		{
			CCD1_info->CCD_Ready_Num++;//CCD1数据有效次数自加
		}
		
		//搜寻人字道
		if ((TRUE == Parameter_info.GetTurnRound_Enable) 
			&& (Parameter_info.Time >= Parameter_info.GetTurnRoundTime)
			&& (fabs(Steer_info.SteerPWM_Error[0]) < TurnRoundSteer_Max)
			&& (TRUE == myCCD_CCD2_GetLeftIntoTurnRound(&CCD2_info)))
		{
			if(Parameter_info.TurnRound_OK_Num < Parameter_info.TurnRound_Num_MAX + 3)
			{
				Parameter_info.TurnRound_OK_Num++;
			}
			
			if(Parameter_info.TurnRound_OK_Num >= Parameter_info.TurnRound_Num_MAX + 2)
			{
				Parameter_info.TurnRound_OK_Num = 0;
				
				Parameter_info.GetTurnRound_Enable = FALSE;//已经处于人字道则不再检测人字道
					
				Car_state_Pre = Car_state;
				Car_state = Turn_Round;
				
				//从左边进入人字道
				Parameter_info.LeftIntoTurnRound_Flag = 1;
				Parameter_info.RightIntoTurnRound_Flag = 0;
				
				Parameter_info.CCD2OrCCD3_GetTurnRound = 2;//CCD2识别到了人字道
			}
		}	
		else
		{
			Parameter_info.TurnRound_OK_Num = 0;
		}
	}	
	/*------------------------------十字道和正常赛道-------------------------------*/			 
	else if (0 == CCD1_info->AddLine_Flag)//没有进行补线
	{
		//正常赛道
		if ((CCD1_info->InvalidPixel_Num < Left_temp) && (Right_temp < 127 - CCD1_info->InvalidPixel_Num))
		{
			CCD1_info->LeftLine[0] = Left_temp;
			CCD1_info->RightLine[0] = Right_temp;
			
			if ((1 == Speed_info->DistanceOf1Cm_Flag) && (CCD1_info->CCD_Ready_Num < CCD1_DataReady_Num))//1cm到了,清零1cm标志放在更新摇头舵机偏差队列之后
			{
				CCD1_info->CCD_Ready_Num++;//CCD1数据有效次数自加
			}
			
			CCD1_info->LeftLoseLine_Flag = 0;
			CCD1_info->RightLoseLine_Flag = 0;
		}
		else//可能是十字道,坡道和全黑丢线,停止更新左右边界值
		{
			CCD1_info->LossLine_Flag = 1;
			
			//判断十字道，十字道不清除CCD1的有效次数
			if(	   (0 == CCD1_info->RoadInvalid_Flag) 
				&& ((Ramp_Up != Road_condition) && (Ramp_Down != Road_condition))
				&& ((1 == CCD1_info->PixelBinary[LinePixel_Temp]) 
				&& (1 == CCD1_info->PixelBinary[LinePixel_Temp - 1]) 
				&& (1 == CCD1_info->PixelBinary[LinePixel_Temp + 1])))//不是坡道则为十字道或者全黑丢线
			{
				CCD1_info->Cross_Flag = 1;
			}
			else
			{
				CCD1_info->CCD_Ready_Num = 0;
			}
		}
	}
	
	/*------------------------------更新队列-----------------------------------------*/
	if ((1 != CCD1_info->LossLine_Flag) && (CCD1_info->RightLine[0] - CCD1_info->LeftLine[0]) >= ROAD_WIDETH_1_MIN)//限制路宽
	{
		CCD1_info->CentralLine[0] = (CCD1_info->LeftLine[0] + CCD1_info->RightLine[0]) >> 1;
		CCD1_info->LineError[0] = CCD1_info->CentralLine[0] - CCD1_info->CCD_PhotoCenter;
		CCD1_info->LineError_D[0] = CCD1_info->LineError[0] - CCD1_info->LineError[8]; 
		
		if ((Ramp_Up == Road_condition) || (Ramp_Down == Road_condition))//坡道使用CCD1，不允许舵机有很大的打角且不允许偏差突变
		{
			if((fabs(CCD1_info->CentralLine[0] - CCD1_info->CentralLine[1]) > 5)
				|| (fabs(CCD1_info->LineError[0]) > 15))
			{
				CCD1_info->LeftLine[0] = CCD1_info->LeftLine[1];
				CCD1_info->RightLine[0] = CCD1_info->RightLine[1];
				
				CCD1_info->CentralLine[0] = (CCD1_info->LeftLine[0] + CCD1_info->RightLine[0]) >> 1;
				CCD1_info->LineError[0] = CCD1_info->CentralLine[0] - Photo_Center;
				CCD1_info->LineError_D[0] = CCD1_info->LineError[0] - CCD1_info->LineError[8]; 
				
				CCD1_info->LossLine_Flag = 1;//偏差有突变时认为CCD1丢线
			}
		}
		
		if (((Right_temp - Left_temp) <= ROAD_WIDETH_1_MAX) && (1 == Speed_info->DistanceOf1Cm_Flag))//路宽符合规定再更新路宽，1cm更新一次
		{
			CCD1_info->RoadWidth[9] = CCD1_info->RoadWidth[8];
			CCD1_info->RoadWidth[8] = CCD1_info->RoadWidth[7];
			CCD1_info->RoadWidth[7] = CCD1_info->RoadWidth[6];
			CCD1_info->RoadWidth[6] = CCD1_info->RoadWidth[5];
			CCD1_info->RoadWidth[5] = CCD1_info->RoadWidth[4];
			CCD1_info->RoadWidth[4] = CCD1_info->RoadWidth[3];
			CCD1_info->RoadWidth[3] = CCD1_info->RoadWidth[2];
			CCD1_info->RoadWidth[2] = CCD1_info->RoadWidth[1];
			CCD1_info->RoadWidth[1] = CCD1_info->RoadWidth[0];
			
			CCD1_info->RoadWidth[0] = CCD1_info->RightLine[0] - CCD1_info->LeftLine[0];
		}
	}
}

/*************************************************************************
*					零		度		偏		移                             
*
*  函数名称:myCCD_CCD2_GetLineError		
*  功能说明:CCD2搜寻中线,求得偏差
*  参数说明:
*  函数返回:无
*  修改时间:2014-06-17
*  备    注: 从上次的中点往两边搜索低电平
*************************************************************************/	
void myCCD_CCD2_GetLineError(CCD_Info *CCD1_info,CCD_Info *CCD2_info,Speed_Info *Speed_info)
{
	int16_t CentralLinePixel_Now = CCD2_info->CentralLine[0];//从上次的中点开始搜线
	int16_t LinePixel_Temp = CentralLinePixel_Now;
	
	int16_t Left_temp=0, Right_temp=0;
	uint8_t ii = 0;
	
	//清零相关标记
	CCD2_info->Cross_Flag = 0;
	CCD2_info->AddLine_Flag = 0;
	CCD2_info->LeftLoseLine_Flag = 1;
	CCD2_info->RightLoseLine_Flag = 1;
	
	if ((CentralLinePixel_Now < 2 + CCD2_info->InvalidPixel_Num) && (CCD2_info->RightLine[0] > CCD2_info->LeftLine[0]))
	{
		CentralLinePixel_Now = 2 + CCD2_info->InvalidPixel_Num;
		
		if((TRUE == Parameter_info.RoadInvalid_Enable) 
			&& (Ramp_Up != Road_condition) 
			&& (Ramp_Down != Road_condition) 
			&& (1 != CCD1_info->Cross_Flag))//坡道和十字道不判定道路无效
		{
			if((0 == CCD2_info->RoadInvalid_Flag) 
				&& (1 == CCD2_info->PixelBinary[CentralLinePixel_Now - 5])
				&& (0 == CCD2_info->PixelBinary[CentralLinePixel_Now])
				&& (0 == CCD2_info->PixelBinary[CentralLinePixel_Now + 5]))
			{
				CCD2_info->RoadInvalid_Flag = 1;
			}
			else if((1 == CCD2_info->PixelBinary[CentralLinePixel_Now - 5])
				&& (1 == CCD2_info->PixelBinary[CentralLinePixel_Now])
				&& (0 == CCD2_info->PixelBinary[CentralLinePixel_Now + 5]))
			{
				CCD2_info->RoadInvalid_Flag = 0;
			}
		}
		else 
		{
			CCD2_info->RoadInvalid_Flag = 0;
		}
	}
	else if ((CentralLinePixel_Now > 125 - CCD2_info->InvalidPixel_Num) && (CCD2_info->RightLine[0] > CCD2_info->LeftLine[0]))
	{
		CentralLinePixel_Now = 125 - CCD2_info->InvalidPixel_Num;
		
		if((TRUE == Parameter_info.RoadInvalid_Enable) 
			&& (Ramp_Up != Road_condition) 
			&& (Ramp_Down != Road_condition) 
			&& (1 != CCD1_info->Cross_Flag))//坡道和十字道不判定道路无效
		{
			if((0 == CCD2_info->RoadInvalid_Flag) 
				&& (0 == CCD2_info->PixelBinary[CentralLinePixel_Now - 5])
				&& (0 == CCD2_info->PixelBinary[CentralLinePixel_Now])
				&& (1 == CCD2_info->PixelBinary[CentralLinePixel_Now + 5]))
			{
				CCD2_info->RoadInvalid_Flag = 2;
			}
			else if((0 == CCD2_info->PixelBinary[CentralLinePixel_Now - 5])
				&& (1 == CCD2_info->PixelBinary[CentralLinePixel_Now])
				&& (1 == CCD2_info->PixelBinary[CentralLinePixel_Now + 5]))
			{
				CCD2_info->RoadInvalid_Flag = 0;
			}
		}
		else 
		{
			CCD2_info->RoadInvalid_Flag = 0;
		}
	}
	else if (CCD2_info->LeftLine[0] >= CCD2_info->RightLine[0])
	{
		CentralLinePixel_Now = Photo_Center;
		
		//左右边界出现异常，用上一次的边界值来代替
		CCD2_info->LeftLine[0] = CCD2_info->LeftLine[1];
		CCD2_info->RightLine[0] = CCD2_info->RightLine[1];
	}

	if((Ramp_Up == Road_condition) || (Ramp_Down == Road_condition) || (1 == CCD1_info->Cross_Flag)
		|| ((CCD2_info->CCD_Ready_Num < 3) && (Turn_Round == Car_state_Pre)))//坡道，十字道和出人字道，从中间往两边搜线
	{
		LinePixel_Temp = 63;
	}
	else 
	{
		LinePixel_Temp = CentralLinePixel_Now;
	}
	
	/*------------------左边线------------------------*/
	if(0 == CCD2_info->RoadInvalid_Flag)
	{
		ii = LinePixel_Temp;
		
		while(ii > CCD2_info->InvalidPixel_Num)
		{
			if(1 == CCD2_info->PixelBinary[ii])//搜到高电平“1”
			{
				break;
			}
			
			ii--;
		}
		
		if(ii != CCD2_info->InvalidPixel_Num)//已经搜索到高电平
		{
			while (ii > CCD2_info->InvalidPixel_Num)
			{
				 if (0 == CCD2_info->PixelBinary[ii])//寻 1 -> 0 跳变
				 {
					 Left_temp = ii;
					 break;
				 }
				 else
				 {
					 ii--;//只有当前点的二值化数据为 1 时才继续往下搜寻下降沿，否则停止搜线
				 }
			}
		}
	}
	
	//左边丢线
	if (0 == Left_temp) 
	{
		Left_temp = CCD2_info->InvalidPixel_Num;
	}	
			
    /*-------------------右边线------------------------*/
	if(0 == CCD2_info->RoadInvalid_Flag)
	{
		ii = LinePixel_Temp + 1;
		
		while(ii < 127 - CCD2_info->InvalidPixel_Num)
		{
			if(1 == CCD2_info->PixelBinary[ii])//搜到高电平“1”
			{
				break;
			}
			
			ii++;
		}
		
		if(ii != 127 - CCD2_info->InvalidPixel_Num)//已经搜索到高电平
		{
			while (ii < (127 - CCD2_info->InvalidPixel_Num))
			{
				 if (0 == CCD2_info->PixelBinary[ii])//寻 1 -> 0 跳变
				 {
					 Right_temp = ii ;
					 break;
				 }
				 else
				 {
					 ii++;//只有当前点的二值化数据为 1 时才继续往下搜寻下降沿，否则停止搜线
				 }
			}
		}
	}
	//右边丢线
	if (0 == Right_temp) 
	{
		Right_temp = 127 - CCD2_info->InvalidPixel_Num;
	}	
	
	//------------------------------移动队列----------------------------------------//
	for (ii = Line_SIZE - 1; ii > 0; ii--)
	{
		CCD2_info->LeftLine[ii] = CCD2_info->LeftLine[ii - 1];
		CCD2_info->RightLine[ii] = CCD2_info->RightLine[ii - 1];
		CCD2_info->CentralLine[ii] = CCD2_info->CentralLine[ii - 1];
		CCD2_info->LineError[ii] = CCD2_info->LineError[ii - 1];
		CCD2_info->LineError_D[ii] = CCD2_info->LineError_D[ii - 1];
	}
	
	//----------------------------------------CCD2往左补线------------------------------------//
	if ((Left_temp <= CCD2_info->InvalidPixel_Num) && (Right_temp < 127 - CCD2_info->InvalidPixel_Num))
	{ 
		CCD2_info->AddLine_Flag = 1;
		
		if(	   (1 == CCD1_info->RoadInvalid_Flag)
			|| (1 == CCD2_info->LossLine_Flag)
		    || ((Right_temp - (CCD2_info->RoadWidth[0] >> 1)) > CCD2_info->CCD_PhotoCenter)
		  )//CCD1进行补线时或者CCD1全丢线或者CCD2丢过线则CCD2停止补线
		{
			CCD2_info->LossLine_Flag = 1;		//置位CCD2丢线标记
			
			CCD2_info->CCD_Ready_Num = 0;
		}
		else
		{
			CCD2_info->LeftLine[0] = Right_temp - CCD2_info->RoadWidth[0];
			CCD2_info->RightLine[0] = Right_temp;		

			if((1 == Speed_info->DistanceOf1Cm_Flag) && (CCD2_info->CCD_Ready_Num < CCD2_DataReady_Num))
			{
				CCD2_info->CCD_Ready_Num++;		
			}
			
			CCD2_info->LossLine_Flag = 0;			//清零CCD2丢线标记
		}
		
		
		//利用CCD3搜寻人字道
		if ((TRUE == Parameter_info.GetTurnRound_Enable) && (Parameter_info.Time >= Parameter_info.GetTurnRoundTime))
		{
			if (   (0 == CCD1_info->LossLine_Flag)
				&& (0 == CCD1_info->AddLine_Flag)
				&& (CCD2_info->LeftLossLinePixel >= 20)
				&& (fabs(Steer_info.SteerPWM_Error[0]) < TurnRoundSteer_Max)
				)//CCD2左边丢线(人字道丢线时LeftLine[0] >= 20)，右边不丢线,CCD1没有丢线并且摇头舵机没有打死角
			{
				if(Speed_info->DiatanceToTurnRound <= 0)//没有记录到人字道尖角的距离
				{
					Speed_info->DiatanceToTurnRound = 50;//记录此时到人字道尖角的距离,CCD2前瞻50cm
				}

				if (TRUE == myCCD_CCD3_GetRightIntoTurnRound(&CCD3_info))
				{
					if(Parameter_info.TurnRound_OK_Num < Parameter_info.TurnRound_Num_MAX)
					{
						Parameter_info.TurnRound_OK_Num++;
					}
					
					if(Parameter_info.TurnRound_OK_Num >= Parameter_info.TurnRound_Num_MAX)
					{
						Parameter_info.GetTurnRound_Enable = FALSE;//已经处于人字道则不再检测人字道
						
						Car_state_Pre = Car_state;
						Car_state = Turn_Round;
						
						//从右边进入人字道
						Parameter_info.LeftIntoTurnRound_Flag = 0;
						Parameter_info.RightIntoTurnRound_Flag = 1;
						
						Parameter_info.CCD2OrCCD3_GetTurnRound = 3;//CCD3识别到了人字道
					}
				}
				else
				{
					Parameter_info.TurnRound_OK_Num = 0;
				}
			}
		}
	}
	//---------------------------------------CCD2往右补线-------------------------------------//
	else if ((CCD2_info->InvalidPixel_Num < Left_temp) && (Right_temp >= 127 - CCD2_info->InvalidPixel_Num))//左边不丢线，右边丢线(人字道丢线时RightLine[0] <= 90)
	{
		CCD2_info->AddLine_Flag = 1;
		
		if(    (1 == CCD1_info->RoadInvalid_Flag)
			|| (1 == CCD2_info->LossLine_Flag)
			|| ((Left_temp + (CCD2_info->RoadWidth[0] >> 1)) < CCD2_info->CCD_PhotoCenter)
		  )//CCD1进行补线时或者CCD1全丢线或者CCD2丢过线则CCD2停止补线
		{
			CCD2_info->LossLine_Flag = 1;			//置位CCD2丢线标记
			
			CCD2_info->CCD_Ready_Num = 0;
		}
		else 
		{
			CCD2_info->RightLine[0] = Left_temp + CCD2_info->RoadWidth[0];
			CCD2_info->LeftLine[0]  = Left_temp;	

			if((1 == Speed_info->DistanceOf1Cm_Flag) && (CCD2_info->CCD_Ready_Num < CCD2_DataReady_Num))
			{
				CCD2_info->CCD_Ready_Num++;		
			}			
			
			CCD2_info->LossLine_Flag = 0;			//清零CCD2丢线标记
		}
		
		//利用CCD3搜寻人字道
		if ((TRUE == Parameter_info.GetTurnRound_Enable) && (Parameter_info.Time >= Parameter_info.GetTurnRoundTime))
		{	
			if (   (0 == CCD1_info->LossLine_Flag)
				&& (0 == CCD1_info->AddLine_Flag)
				&& (CCD2_info->RightLossLinePixel <= 110)
				&& (fabs(Steer_info.SteerPWM_Error[0]) < TurnRoundSteer_Max)
				)//CCD2右边丢线(人字道丢线时右边界 <= 110)，左边不丢线,CCD1没有丢线并且摇头舵机没有打死角
			{
				if(Speed_info->DiatanceToTurnRound <= 0)//没有记录到人字道尖角的距离
				{
					Speed_info->DiatanceToTurnRound = 50;//记录此时到人字道尖角的距离,CCD2前瞻50cm
				}
				
				if (TRUE == myCCD_CCD3_GetLeftIntoTurnRound(&CCD3_info))
				{
					if(Parameter_info.TurnRound_OK_Num < Parameter_info.TurnRound_Num_MAX)
					{
						Parameter_info.TurnRound_OK_Num++;
					}
					
					if(Parameter_info.TurnRound_OK_Num >= Parameter_info.TurnRound_Num_MAX)
					{
						Parameter_info.TurnRound_OK_Num = 0;
						
						Parameter_info.GetTurnRound_Enable = FALSE;//已经处于人字道则不再检测人字道
						
						Car_state_Pre = Car_state;
						Car_state = Turn_Round;
						
						//从左边进入人字道
						Parameter_info.LeftIntoTurnRound_Flag = 1;
						Parameter_info.RightIntoTurnRound_Flag = 0;
						
						Parameter_info.CCD2OrCCD3_GetTurnRound = 3;//CCD3识别到了人字道
					}
				}
				else
				{
					Parameter_info.TurnRound_OK_Num = 0;
				}
			}
		}
	}
	else 
	{
		//---------------------------------普通赛道-----------------------------------------//
		if (   (CCD2_info->InvalidPixel_Num < Left_temp) && (Right_temp < 127 - CCD2_info->InvalidPixel_Num)
			&& ((Right_temp - Left_temp) >= ROAD_WIDETH_2_MIN))//限制路宽
		{
			CCD2_info->LeftLine[0] = Left_temp;
			CCD2_info->RightLine[0] = Right_temp;
			
			if (   (1 == Speed_info->DistanceOf1Cm_Flag)
				&& (0 == CCD1_info->RoadInvalid_Flag)
				&& (CCD2_info->CCD_Ready_Num < CCD2_DataReady_Num))//1cm到了，清除次标志是在控制摇头舵机之后
			{
				CCD2_info->CCD_Ready_Num++;//CCD1没有补线并且道路有效时，CCD2数据有效次数自加，防止窜道
			}
			
			//记录每次的两边点，一旦丢线，则可以通过判断此点来决定是否人字道搜线
			CCD2_info->LeftLossLinePixel = Left_temp;
			CCD2_info->RightLossLinePixel = Right_temp;
			
			CCD2_info->LeftLoseLine_Flag = 0;
			CCD2_info->RightLoseLine_Flag = 0;
			
			CCD2_info->LossLine_Flag = 0;
		}
		//---------------------------------十字道-----------------------------------------//
		else if((Left_temp <= CCD2_info->InvalidPixel_Num) && (Right_temp >= 127 - CCD2_info->InvalidPixel_Num))
		{
			if(    (1 == CCD2_info->PixelBinary[LinePixel_Temp]) 
				&& (1 == CCD2_info->PixelBinary[LinePixel_Temp - 1]) 
				&& (1 == CCD2_info->PixelBinary[LinePixel_Temp + 1]))
			{
				CCD2_info->Cross_Flag = 1;
			}
			else
			{
				CCD2_info->CCD_Ready_Num = 0;			//清零CCD2数据有效次数
			}
			
			CCD2_info->LossLine_Flag = 1;
		}
	}
	
	/*------------------------------更新队列--------------------------------*/
	if ((0 == CCD2_info->LossLine_Flag) && ((CCD2_info->RightLine[0] - CCD2_info->LeftLine[0]) <= ROAD_WIDETH_2_MAX))//CCD2数据有效
	{		
		CCD2_info->CentralLine[0] = (CCD2_info->LeftLine[0] + CCD2_info->RightLine[0] ) >> 1;	
		CCD2_info->LineError[0] = CCD2_info->CentralLine[0] - CCD2_info->CCD_PhotoCenter; 
		CCD2_info->LineError_D[0] = CCD2_info->LineError[0] - CCD2_info->LineError[8];  
		
		if (1 == Speed_info->DistanceOf1Cm_Flag)//更新CCD2的路宽,1cm更新一次
		{
			CCD2_info->RoadWidth[9] = CCD2_info->RoadWidth[8];
			CCD2_info->RoadWidth[8] = CCD2_info->RoadWidth[7];
			CCD2_info->RoadWidth[7] = CCD2_info->RoadWidth[6];
			CCD2_info->RoadWidth[6] = CCD2_info->RoadWidth[5];
			CCD2_info->RoadWidth[5] = CCD2_info->RoadWidth[4];
			CCD2_info->RoadWidth[4] = CCD2_info->RoadWidth[3];
			CCD2_info->RoadWidth[3] = CCD2_info->RoadWidth[2];
			CCD2_info->RoadWidth[2] = CCD2_info->RoadWidth[1];
			CCD2_info->RoadWidth[1] = CCD2_info->RoadWidth[0];
			
			CCD2_info->RoadWidth[0] = CCD2_info->RightLine[0] -  CCD2_info->LeftLine[0];
		}
	}
}

/*************************************************************************
*					零		度		偏		移                              
*
*  函数名称:myCCD_CCD3_GetLineError		
*  功能说明:CCD3搜寻中线,求得偏差,识别人字道
*  参数说明:
*  函数返回:无
*  修改时间:2014-06-17
*  备    注: 从上次的中点往两边搜索低电平
*************************************************************************/	
void myCCD_CCD3_GetLineError(CCD_Info *CCD3_info,Speed_Info *Speed_info)
{
	int16_t CentralLinePixel_Now = CCD3_info->CentralLine[0];//从上次的中点开始搜线
	int16_t LinePixel_Temp = CentralLinePixel_Now;
	
	int16_t Left_temp=0, Right_temp=0;
	uint8_t ii = 0;
	
	//清零十字道标记
	CCD3_info->Cross_Flag= 0;
	CCD3_info->LeftLoseLine_Flag = 1;
	CCD3_info->RightLoseLine_Flag = 1;

	if ((CentralLinePixel_Now < 2 + CCD3_info->InvalidPixel_Num) && (CCD3_info->RightLine[0] > CCD3_info->LeftLine[0]))
	{
		CentralLinePixel_Now = 2 + CCD3_info->InvalidPixel_Num;
	}
	else if ((CentralLinePixel_Now > 125 - CCD3_info->InvalidPixel_Num) && (CCD3_info->RightLine[0] > CCD3_info->LeftLine[0]))
	{
		CentralLinePixel_Now = 125 - CCD3_info->InvalidPixel_Num;
	}
	else if (CCD3_info->LeftLine[0] >= CCD3_info->RightLine[0])
	{
		CentralLinePixel_Now = Photo_Center;
		
		//左右边界出现异常，用上一次的边界值来代替
		CCD3_info->LeftLine[0] = CCD3_info->LeftLine[1];
		CCD3_info->RightLine[0] = CCD3_info->RightLine[1];
	}
	
	if((1 == CCD1_info.Cross_Flag) || (1 == CCD2_info.Cross_Flag)
		|| (Ramp_Up == Road_condition) || (Ramp_Down == Road_condition)
		|| (Turn_Round == Car_state_Pre))//坡道和十字道,从中间往两边搜线
	{
		LinePixel_Temp = 63;
	}
	else 
	{
		LinePixel_Temp = CentralLinePixel_Now;
	}
	
	/*------------------左边线------------------------*/		
	ii = LinePixel_Temp;
	
	while(ii > CCD3_info->InvalidPixel_Num)
	{
		if(1 == CCD3_info->PixelBinary[ii])//搜到高电平“1”
		{
			break;
		}
		
		ii--;
	}
	
	if(ii != CCD3_info->InvalidPixel_Num)//已经搜索到高电平
	{
		while (ii > CCD3_info->InvalidPixel_Num)
		{
			 if (0 == CCD3_info->PixelBinary[ii])//寻 1 -> 0 跳变
			 {
				 Left_temp = ii ;
				 break;
			 }
			 else
			 {
				 ii--;//只有当前点的二值化数据为 1 时才继续往下搜寻下降沿，否则停止搜线
			 }
		} 
	}

	//左边丢线
	if (0 == Left_temp) 
	{
		Left_temp = CCD3_info->InvalidPixel_Num; 
	}	
			
    /*-------------------右边线------------------------*/	
	ii = LinePixel_Temp + 1;
		
	while(ii < 127 - CCD3_info->InvalidPixel_Num)
	{
		if(1 == CCD3_info->PixelBinary[ii])//搜到高电平“1”
		{
			break;
		}
		
		ii++;
	}
	
	if(ii != 127 - CCD3_info->InvalidPixel_Num)//已经搜索到高电平
	{
		while (ii < (127 - CCD3_info->InvalidPixel_Num))
		{
			 if (0 == CCD3_info->PixelBinary[ii])//寻 1 -> 0 跳变
			 {
				 Right_temp = ii ;
				 break;
			 }
			 else
			 {
				 ii++;//只有当前点的二值化数据为 1 时才继续往下搜寻下降沿，否则停止搜线
			 }
		}
	}

	//右边丢线
	if (0 == Right_temp) 
	{
		Right_temp = 127 - CCD3_info->InvalidPixel_Num;
	}	
	
	//------------------------------移动队列----------------------------------------//
	for (ii = Line_SIZE - 1; ii > 0; ii--)
	{
		CCD3_info->CentralLine[ii] = CCD3_info->CentralLine[ii - 1];
		CCD3_info->LineError[ii] = CCD3_info->LineError[ii - 1];
		CCD3_info->LineError_D[ii] = CCD3_info->LineError_D[ii - 1];
	}
	
	if(1 == Speed_info->DistanceOf1Cm_Flag)
	{
		for (ii = Line_SIZE - 1; ii > 0; ii--)
		{
			CCD3_info->LeftLine[ii] = CCD3_info->LeftLine[ii - 1];
			CCD3_info->RightLine[ii] = CCD3_info->RightLine[ii - 1];
		}
	}
	
	//CCD3两边都不丢线才认为数据有效
	if ((CCD3_info->InvalidPixel_Num < Left_temp) && (Right_temp < 127 - CCD3_info->InvalidPixel_Num)
		&& ((Right_temp - Left_temp) >= ROAD_WIDETH_3_MIN) 
		&& ((Right_temp - Left_temp) <= ROAD_WIDETH_3_MAX))//限制路宽
	{
		CCD3_info->LeftLine[0] = Left_temp;
		CCD3_info->RightLine[0] = Right_temp;
		
		if ((1 == Speed_info->DistanceOf1Cm_Flag) && (CCD3_info->CCD_Ready_Num < CCD3_DataReady_Num))//1cm到了,清除1cm标志是更新摇头舵机偏差队列之后
		{
			CCD3_info->CCD_Ready_Num++;//CCD3数据有效次数自加
		}
		
		CCD3_info->LeftLossLinePixel = Left_temp;
		CCD3_info->RightLossLinePixel = Right_temp;
		CCD3_info->LeftLoseLine_Flag = 0;
		CCD3_info->RightLoseLine_Flag = 0;
		
		CCD3_info->LossLine_Flag = 0;
	}
	else
	{
		CCD3_info->LossLine_Flag = 1;
		CCD3_info->CCD_Ready_Num = 0;//清除CCD3数据有效次数
		
		//-------------------------------用于判断人字道尖角----------------------------//
		//左边丢线
		if((CCD3_info->InvalidPixel_Num >= Left_temp) && (Right_temp < 127 - CCD3_info->InvalidPixel_Num))
		{
			CCD3_info->RightLossLinePixel = Right_temp;
			CCD3_info->RightLoseLine_Flag = 0;
		}
		//右边丢线
		else if((CCD3_info->InvalidPixel_Num < Left_temp) && (Right_temp >= 127 - CCD3_info->InvalidPixel_Num))
		{
			CCD3_info->LeftLossLinePixel = Left_temp;
			CCD3_info->LeftLoseLine_Flag = 0;
		}
		//判断是否进入十字道
		else if(((Left_temp <= CCD3_info->InvalidPixel_Num) && (Right_temp >= 127 - CCD3_info->InvalidPixel_Num))
			&& (1 == CCD3_info->PixelBinary[LinePixel_Temp])
			&& (1 == CCD3_info->PixelBinary[LinePixel_Temp - 1])
			&& (1 == CCD3_info->PixelBinary[LinePixel_Temp + 1]))//CCD3全丢线并且全白
		{
			CCD3_info->Cross_Flag = 1;
		}
	}
	
	/*------------------------------更新队列--------------------------------*/
	if (0 == CCD3_info->LossLine_Flag)//CCD3数据有效
	{
		CCD3_info->CentralLine[0] = (CCD3_info->LeftLine[0] + CCD3_info->RightLine[0]) >> 1;	
		CCD3_info->LineError[0]   = CCD3_info->CentralLine[0] - CCD3_info->CCD_PhotoCenter;  
		CCD3_info->LineError_D[0] = CCD3_info->LineError[0] - CCD3_info->LineError[8]; 
		
		if(1 == Speed_info->DistanceOf1Cm_Flag)//1cm更新一次
		{
			CCD3_info->RoadWidth[9] = CCD3_info->RoadWidth[8];
			CCD3_info->RoadWidth[8] = CCD3_info->RoadWidth[7];
			CCD3_info->RoadWidth[7] = CCD3_info->RoadWidth[6];
			CCD3_info->RoadWidth[6] = CCD3_info->RoadWidth[5];
			CCD3_info->RoadWidth[5] = CCD3_info->RoadWidth[4];
			CCD3_info->RoadWidth[4] = CCD3_info->RoadWidth[3];
			CCD3_info->RoadWidth[3] = CCD3_info->RoadWidth[2];
			CCD3_info->RoadWidth[2] = CCD3_info->RoadWidth[1];
			CCD3_info->RoadWidth[1] = CCD3_info->RoadWidth[0];
			
			CCD3_info->RoadWidth[0]   = CCD3_info->RightLine[0] -  CCD3_info->LeftLine[0];
		}
	}
}

/*************************************************************************
*					零		度		偏		移                            
*
*  函数名称:myCCD_CCD1_GetStartEndLine
*  功能说明:CCD 识别直道和小S路段
*  参数说明:
*  函数返回:
*  修改时间:2014-07-23
*  备    注：CCD2,CCD3都存在两条边并且CCD1从左往右搜寻到以下类型为起跑线
*			①2个上升沿和2个下降沿
*			②3个上升沿和3个下降沿
*************************************************************************/
uint8_t myCCD_CCD1_GetStartEndLine(	CCD_Info *CCD1_info,
									CCD_Info *CCD2_info,
									CCD_Info *CCD3_info,
									Road_Condition *Road_condition,
									Car_State *Car_state
								  )
{
	uint8_t ii = CCD1_info->InvalidPixel_Num;//从左边无效点开始往右边搜寻起跑线
	int16_t Up_1 = 0, Down_1 = 0, Up_2 = 0;
	
	//只当CCD2，CCD3都存在两条边界，不在人字道，不在坡道时才检测起跑线
	if(    (0 == CCD2_info->AddLine_Flag)
		&& (0 == CCD2_info->Cross_Flag)
		&& (CCD2_info->CCD_Ready_Num >= 6)
		&& (CCD3_info->CCD_Ready_Num >= 16))
	{
		while(ii < (127 - CCD1_info->InvalidPixel_Num))
		{
			if(0 == CCD1_info->PixelBinary[ii])//搜寻到低电平
			{
				ii += 1;
				
				while(ii < (127 - CCD1_info->InvalidPixel_Num))
				{
					if(1 == CCD1_info->PixelBinary[ii])//搜寻到上升沿1
					{
						Up_1 = ii;
						
						ii += 1;
						
						while(ii < (127 - CCD1_info->InvalidPixel_Num))
						{
							if(0 == CCD1_info->PixelBinary[ii])//搜寻到下降沿1
							{
								Down_1 = ii;
								
								ii += 1;
								
								while(ii < (127 - CCD1_info->InvalidPixel_Num))
								{
									if(1 == CCD1_info->PixelBinary[ii])//搜寻到上升沿2
									{
										Up_2 = ii;
										
										ii += 1;
										
										while(ii < (127 - CCD1_info->InvalidPixel_Num))
										{
											if(0 == CCD1_info->PixelBinary[ii])//搜寻到下降沿2
											{
												if(((Down_1 - Up_1) >= StartEndLine_Pixel_MIN) && ((Down_1 - Up_1) <= StartEndLine_Pixel_MAX)
													&& ((Up_2 - Down_1) >= StartEndLine_Pixel_MIN) && ((Up_2 - Down_1) <- StartEndLine_Pixel_MAX))
												{
													return TRUE;
												}
												else
												{
													return TRUE;//检测到起跑线
												}
											}
											
											ii++;
										}
									
										return FALSE;
									}
									
									ii++;
								}
							
								return FALSE;
							}
							
							ii++;
						}
					
						return FALSE;
					}
					
					ii++;
				}
			
				return FALSE;
			}
			
			ii++;
		}
		
		return FALSE;//检测到起跑线，停车
	}
	else
	{
		return FALSE;
	}
}

/*************************************************************************
*					零		度		偏		移                          
*
*  函数名称:myCCD_CCD2_GetLeftIntoTurnRound
*  功能说明:搜索左边进人字道
*  参数说明:
*  函数返回:	TRUE - 搜索到人字道
*				FALSE - 未搜索到人字道
*
*  修改时间:2014-06-21
*  备    注:从最左边往最右边搜索Up_1,Down_1,Up_2,Down_2
*************************************************************************/	
int16_t myCCD_CCD2_GetLeftIntoTurnRound(CCD_Info *CCD2_info)
{
	uint8 Up_1 = 0, Down_1 = 0, Up_2 = 0;
	uint8  LeftLinePixel_Now = 18;
	
	while (LeftLinePixel_Now < 111)
	{
		if (0 == CCD2_info->PixelBinary[LeftLinePixel_Now])//搜索到低电平
		{
			LeftLinePixel_Now += 1;
			
			while (LeftLinePixel_Now < 111)//继续搜索Up_1
			{
				if (1 == CCD2_info->PixelBinary[LeftLinePixel_Now])//搜索到Up_1
				{
					Up_1 = LeftLinePixel_Now;	//得到Up_1
					LeftLinePixel_Now += 1;
					
					while (LeftLinePixel_Now < 111)//继续搜索Down_1
					{
						if (0 == CCD2_info->PixelBinary[LeftLinePixel_Now])//得到Down_1
						{
							Down_1 = LeftLinePixel_Now;
							LeftLinePixel_Now += 1;
							
							while (LeftLinePixel_Now < 111)//继续搜索Up_2
							{
								if (1 == CCD2_info->PixelBinary[LeftLinePixel_Now])//得到Up_2
								{
									Up_2 = LeftLinePixel_Now;
									LeftLinePixel_Now += 1;
									
									while (LeftLinePixel_Now < 111)//继续搜索Down_2
									{
										if (0 == CCD2_info->PixelBinary[LeftLinePixel_Now])//得到Down_2
										{
											//判断是否是人字道
											if (((int)(Down_1 - Up_1) > TurnRound_Pixel_MIN2) && ((int)(Down_1 - Up_1) < TurnRound_Pixel_MAX2)
												&& ((int)(Up_2 - Down_1) > TurnRound_Pixel_MIN2) && ((int)(Up_2 - Down_1) < TurnRound_Pixel_MAX2))
											{
												return TRUE;//得到人字道
											}
											else
												return FALSE;//得到了Up_1,Down_1,Up_2,Down_2,但不符合人字道特征
										}
										
										LeftLinePixel_Now++;
									}
									
									return FALSE;//只得到了Up_1,Down_1,Up_2
								}

								LeftLinePixel_Now++;
							}
							
							return FALSE;//只得到了Up_1,Down_1
						}
						
						LeftLinePixel_Now++;
					}
					
					return FALSE;//只得到了Up_1
				}
				
				LeftLinePixel_Now++;
			}//end of while(LeftLinePixel_Now < 119)
			
			return FALSE;//没有搜索到Up_1,全为低电平
		}//end of if(0 == CCD2_info->PixelBinary[LeftLinePixel_Now])

		LeftLinePixel_Now++;
	}
	
	return FALSE;//没有搜索到低电平则全为高电平
}

/*************************************************************************
*					零		度		偏		移                          
*
*  函数名称:myCCD_CCD3_GetLeftLineIntoTurnRound
*  功能说明:搜索左边进人字道
*  参数说明:
*  函数返回:	TRUE - 搜索到人字道
*				FALSE - 未搜索到人字道
*
*  修改时间:2014-06-30
*  备    注:从最左边往最右边搜索Up_1,Down_1,Up_2,Down_2
*************************************************************************/	
int16_t myCCD_CCD3_GetLeftIntoTurnRound(CCD_Info *CCD3_info)
{
	uint8 Up_1 = 0, Down_1 = 0, Up_2 = 0;
	uint8  LeftLinePixel_Now = 25;
	
	while (LeftLinePixel_Now < 103)
	{
		if (0 == CCD3_info->PixelBinary[LeftLinePixel_Now])//搜索到低电平
		{
			LeftLinePixel_Now += 1;
			
			while (LeftLinePixel_Now < 103)//继续搜索Up_1
			{
				if (1 == CCD3_info->PixelBinary[LeftLinePixel_Now])//搜索到Up_1
				{
					Up_1 = LeftLinePixel_Now;	//得到Up_1
					LeftLinePixel_Now += 1;
					
					while (LeftLinePixel_Now < 103)//继续搜索Down_1
					{
						if (0 == CCD3_info->PixelBinary[LeftLinePixel_Now])//得到Down_1
						{
							Down_1 = LeftLinePixel_Now;
							LeftLinePixel_Now += 1;

							while (LeftLinePixel_Now < 103)//继续搜索Up_2
							{
								if (1 == CCD3_info->PixelBinary[LeftLinePixel_Now])//得到Up_2
								{
									Up_2 = LeftLinePixel_Now;
									LeftLinePixel_Now += 1;
									
									while (LeftLinePixel_Now < 103)//继续搜索Down_2
									{
										if (0 == CCD3_info->PixelBinary[LeftLinePixel_Now])//得到Down_2
										{
											//判断是否是人字道
											if (((int)(Down_1 - Up_1) > TurnRound_Pixel_MIN3) && ((int)(Down_1 - Up_1) < TurnRound_Pixel_MAX3)
												&& ((int)(Up_2 - Down_1) > TurnRound_Pixel_MIN3) && ((int)(Up_2 - Down_1) < TurnRound_Pixel_MAX3))
											{
												return TRUE;//得到人字道
											}
											else
											{
												return FALSE;//得到了Up_1,Down_1,Up_2,Down_2,但不符合人字道特征
											}
										}
										
										LeftLinePixel_Now++;
									}
									
									return FALSE;//只得到了Up_1,Down_1,Up_2
								}

								LeftLinePixel_Now++;
							}
							
							return FALSE;//只得到了Up_1,Down_1
						}
						
						LeftLinePixel_Now++;
					}
					
					return FALSE;//只得到了Up_1
				}
				
				LeftLinePixel_Now++;
			}//end of while(LeftLinePixel_Now < 119)
			
			return FALSE;//没有搜索到Up_1,全为低电平
		}//end of if(0 == CCD2_info->PixelBinary[LeftLinePixel_Now])

		LeftLinePixel_Now++;
	}
	
	return FALSE;//没有搜索到低电平则全为高电平
}

/*************************************************************************
*					零		度		偏		移  
*
*  函数名称：myCCD_CCD2_GetLeftLineOutTurnRound	
*  功能说明：提取出人字道时的跑道左边界线函数
*  参数说明：传入参数为CCD_info 
*  函数返回：右边界对应的曝光点
*  修改时间：2014-08-05
*  备    注： 	用于在人字道调头出人字道的时候搜寻跑道左边界，
*				从最左边开始搜索到最左边，进行两次搜索，一次搜索上升沿，
*				一次搜索下降沿，当搜索到上升沿并且上升沿在限定范围内时
*				认为是左边界，如果搜索到一条上升沿和一条下降沿并且下降沿
*				和上升沿之间的距离为正常跑道宽度则认为是跑道
*************************************************************************/
int16_t myCCD_CCD2_GetLeftLineOutTurnRound(CCD_Info *CCD_info)
{
  	uint8 LeftLinePixel_Old = CCD_info->InvalidPixel_Num;
	
	int LeftLinePixel_Now = CCD_info->InvalidPixel_Num;
	int RightLinePixel_Now = CCD_info->InvalidPixel_Num;
	
	//往右边搜索低电平
	while (LeftLinePixel_Old < 127 - CCD_info->InvalidPixel_Num)
	{
		if (0 == CCD_info->PixelBinary[LeftLinePixel_Old])//搜索到低电平
		{
			LeftLinePixel_Old += 1;//移动到下一个点
			
			while (LeftLinePixel_Old < 127 - CCD_info->InvalidPixel_Num)//继续往右边搜索
			{
				if (1 == CCD_info->PixelBinary[LeftLinePixel_Old])//搜索到上升沿
				{
					LeftLinePixel_Now = LeftLinePixel_Old;
					LeftLinePixel_Old += 1;
					
					while (LeftLinePixel_Old < 127 - CCD_info->InvalidPixel_Num)//继续搜索下降沿
					{
						if (0 == CCD_info->PixelBinary[LeftLinePixel_Old])//搜索到下降沿
						{
							RightLinePixel_Now = LeftLinePixel_Old;
							
							if(((int)(RightLinePixel_Now - LeftLinePixel_Now) > ROAD_WIDETH_1_MIN) && ((int)(RightLinePixel_Now - LeftLinePixel_Now) < ROAD_WIDETH_1_MAX))
							{
								return LeftLinePixel_Now;
							}
						}
						
						LeftLinePixel_Old++;//继续往右边搜索
					}
					
					if (LeftLinePixel_Now > Photo_Center)//没有搜索到下降沿,只搜到左边界
					{
						return LeftLinePixel_Now;
					}
					else
						return -1;
				}
			  
				LeftLinePixel_Old++;//继续往右边搜索
			}
			
			return -1;//没有搜索到高电平
		}
		else 
			return -1;
	}

	return -1;//没有得到上升沿
}

/*************************************************************************
*					零		度		偏		移                             
*
*  函数名称:myCCD_CCD2_GetRightIntoTurnRound		
*  功能说明:搜索右边进入人字道
*  参数说明:
*  函数返回:	1 - 搜索到人字道
*
*  修改时间:2014-06-21
*  备    注:从最右边往最左边搜索Up_1,Down_1,Up_2,Down_2
*************************************************************************/	
int16_t myCCD_CCD2_GetRightIntoTurnRound(CCD_Info *CCD2_info)
{
	uint8 Up_1 = 127, Down_1 = 127, Up_2 = 127;	
	uint8  RightLinePixel_Now = 111;
	
	while (RightLinePixel_Now > 18)
	{
		if (0 == CCD2_info->PixelBinary[RightLinePixel_Now])//搜索到低电平
		{
			RightLinePixel_Now -= 1;
			
			while (RightLinePixel_Now > 18)//继续搜索Up_1
			{
				if (1 == CCD2_info->PixelBinary[RightLinePixel_Now])//搜索到Up_1
				{
					Up_1 = RightLinePixel_Now;	//得到Up_1
					RightLinePixel_Now -= 1;
					
					while (RightLinePixel_Now > 18)//继续搜索Down_1
					{
						if (0 == CCD2_info->PixelBinary[RightLinePixel_Now])//得到Down_1
						{
							Down_1 = RightLinePixel_Now;
							RightLinePixel_Now -= 1;
							
							while (RightLinePixel_Now > 18)//继续搜索Up_2
							{
								if (1 == CCD2_info->PixelBinary[RightLinePixel_Now])//得到Up_2
								{
									Up_2 = RightLinePixel_Now;
									RightLinePixel_Now -= 1;
									
									while (RightLinePixel_Now > 18)//继续搜索Down_2
									{
										if (0 == CCD2_info->PixelBinary[RightLinePixel_Now])//得到Down_2
										{
											//判断是否是人字道
											if (((int)(Up_1 - Down_1) > TurnRound_Pixel_MIN2) && ((int)(Up_1 - Down_1) < TurnRound_Pixel_MAX2)
												&& ((int)(Down_1 - Up_2) > TurnRound_Pixel_MIN2) && ((int)(Down_1 - Up_2) < TurnRound_Pixel_MAX2))
											{
												return TRUE;//得到人字道
											}
											else
											{
												return FALSE;//得到了Up_1,Down_1,Up_2,Down_2,但是不符合人字道特征
											}
										}
										
										RightLinePixel_Now--;
									}//end of while (RightLinePixel_Now > 10)
									
									return FALSE;//只得到了Up_1,Down_1,Up_2
								}//end of if (1 == CCD_info->PixelBinary[RightLinePixel_Now])

								RightLinePixel_Now--;
							}//end of while (RightLinePixel_Now > 10)
							
							return FALSE;//只得到了Up_1,Down_1

						}//end of if (0 == CCD_info->PixelBinary[RightLinePixel_Now])
						
						RightLinePixel_Now--;
					}//end of while (RightLinePixel_Now > 10)
					
					return FALSE;//只得到了Up_1
				}//end of if (1 == CCD_info->PixelBinary[RightLinePixel_Now])
				
				RightLinePixel_Now--;
			}//end of while (RightLinePixel_Now > 10)
			
			return FALSE;//没有得到Up_1全为低电平
		}//end of if (0 == CCD_info->PixelBinary[RightLinePixel_Now])

		RightLinePixel_Now--;
	}//end of while (RightLinePixel_Now > 10)
	
	return FALSE;//没有搜索到低电平
}

/*************************************************************************
*					零		度		偏		移                             
*
*  函数名称:myCCD_CCD3_GetRightIntoTurnRound		
*  功能说明:搜索右边进入人字道
*  参数说明:
*  函数返回:	1 - 搜索到人字道
*
*  修改时间:2014-06-30
*  备    注:从最右边往最左边搜索Up_1,Down_1,Up_2,Down_2
*************************************************************************/	
int16_t myCCD_CCD3_GetRightIntoTurnRound(CCD_Info *CCD3_info)
{
	uint8 Up_1 = 127, Down_1 = 127, Up_2 = 127;	
	uint8  RightLinePixel_Now = 103;
	
	while (RightLinePixel_Now > 25)
	{
		if (0 == CCD3_info->PixelBinary[RightLinePixel_Now])//搜索到低电平
		{
			RightLinePixel_Now -= 1;
			
			while (RightLinePixel_Now > 25)//继续搜索Up_1
			{
				if (1 == CCD3_info->PixelBinary[RightLinePixel_Now])//搜索到Up_1
				{
					Up_1 = RightLinePixel_Now;	//得到Up_1
					RightLinePixel_Now -= 1;
					
					while (RightLinePixel_Now > 25)//继续搜索Down_1
					{
						if (0 == CCD3_info->PixelBinary[RightLinePixel_Now])//得到Down_1
						{
							Down_1 = RightLinePixel_Now;
							RightLinePixel_Now -= 1;
							
							while (RightLinePixel_Now > 25)//继续搜索Up_2
							{
								if (1 == CCD3_info->PixelBinary[RightLinePixel_Now])//得到Up_2
								{
									Up_2 = RightLinePixel_Now;
									RightLinePixel_Now -= 1;
									
									while (RightLinePixel_Now > 25)//继续搜索Down_2
									{
										if (0 == CCD3_info->PixelBinary[RightLinePixel_Now])//得到Down_2
										{
											//判断是否是人字道
											if (((int)(Up_1 - Down_1) > TurnRound_Pixel_MIN3) && ((int)(Up_1 - Down_1) < TurnRound_Pixel_MAX3)
												&& ((int)(Down_1 - Up_2) > TurnRound_Pixel_MIN3) && ((int)(Down_1 - Up_2) < TurnRound_Pixel_MAX3))
											{
												return TRUE;//得到人字道
											}
											else
											{
												return FALSE;//得到了Up_1,Down_1,Up_2,Down_2,但是不符合人字道特征
											}
										}
										
										RightLinePixel_Now--;
									}//end of while (RightLinePixel_Now > 10)
									
									return FALSE;//只得到了Up_1,Down_1,Up_2
								}//end of if (1 == CCD_info->PixelBinary[RightLinePixel_Now])

								RightLinePixel_Now--;
							}//end of while (RightLinePixel_Now > 10)
							
							return FALSE;//只得到了Up_1,Down_1

						}//end of if (0 == CCD_info->PixelBinary[RightLinePixel_Now])
						
						RightLinePixel_Now--;
					}//end of while (RightLinePixel_Now > 10)
					
					return FALSE;//只得到了Up_1
				}//end of if (1 == CCD_info->PixelBinary[RightLinePixel_Now])
				
				RightLinePixel_Now--;
			}//end of while (RightLinePixel_Now > 10)
			
			return FALSE;//没有得到Up_1全为低电平
		}//end of if (0 == CCD_info->PixelBinary[RightLinePixel_Now])

		RightLinePixel_Now--;
	}//end of while (RightLinePixel_Now > 10)
	
	return FALSE;//没有搜索到低电平
}

/*************************************************************************
*					零		度		偏		移  
*
*  函数名称：myCCD_CCD2_GetRightLineOutTurnRound	(二值化)
*  功能说明：提取出人字道时的跑道右边界线函数
*  参数说明：传入参数为CCD_info 
*  函数返回：右边界对应的曝光点
*  修改时间：2014-08-05
*  备    注： 	用于在人字道调头出人字道的时候搜寻跑道右边界，
*				从最右边开始搜索到最左边，进行两次搜索，一次搜索上升沿，
*				一次搜索下降沿，当搜索到上升沿并且上升沿在限定范围内时
*				认为是右边界，如果搜索到一条上升沿和一条下降沿并且下降沿
*				和上升沿之间的距离为正常跑道宽度则认为是跑道
*************************************************************************/
int16_t myCCD_CCD2_GetRightLineOutTurnRound(CCD_Info *CCD_info)
{
  	uint8 RightLinePixel_Old = 127 - CCD_info->InvalidPixel_Num;
	
	int LeftLinePixel_Now = CCD_info->InvalidPixel_Num;
	int RightLinePixel_Now = CCD_info->InvalidPixel_Num;
	
	//往左边搜索低电平
	while (RightLinePixel_Old > CCD_info->InvalidPixel_Num)
	{
		if (0 == CCD_info->PixelBinary[RightLinePixel_Old])//搜索到低电平
		{
			RightLinePixel_Old -= 1;
			
			while (RightLinePixel_Old > CCD_info->InvalidPixel_Num)
			{
				if (1 == CCD_info->PixelBinary[RightLinePixel_Old])//搜索到上升沿
				{
					RightLinePixel_Now = RightLinePixel_Old;
					RightLinePixel_Old -= 1;
					
					while (RightLinePixel_Old > CCD_info->InvalidPixel_Num)//继续搜索下降沿
					{
						if (0 == CCD_info->PixelBinary[RightLinePixel_Old])//搜索到下降沿
						{
							LeftLinePixel_Now = RightLinePixel_Old;
							
							if (((int)(RightLinePixel_Now - LeftLinePixel_Now) >= ROAD_WIDETH_1_MIN) && ((int)(RightLinePixel_Now - LeftLinePixel_Now) <= ROAD_WIDETH_1_MAX))
							{
								return RightLinePixel_Now;
							}
						}
						
						RightLinePixel_Old--;//继续往左边搜索
					}
					
					if (RightLinePixel_Now < Photo_Center)//没有搜索到下降沿,只搜到右边界
					{
						return RightLinePixel_Now;
					}
					else
						return -1;
				}
				  
				RightLinePixel_Old--;//继续往左边搜索
			}
			
			return -1;//没有搜索到高电平
		}
		else 
			return -1;
	}

	return -1;//没有得到上升沿
}

/*************************************************************************
*					零		度		偏		移                            
*
*  函数名称:myCCD_GetObstacle
*  功能说明:CCD 识别路障
*  参数说明:
*  函数返回:
*  修改时间:2014-07-14
*  备    注：CCD2路宽突然变窄，然后CCD1路宽也变窄，并且此时CCD1和CCD2的
*			一边边界点相差不大，则这边边界为路障，而另外一边为正常边界
*************************************************************************/
uint8_t myCCD_GetObstacle(	CCD_Info *CCD1_info,
							CCD_Info *CCD2_info,
							Steer_Info *Steer_info
						)
{
	if((0 == CCD1_info->LossLine_Flag) 
		&& (0 == CCD2_info->LossLine_Flag) 
		&& (fabs(Steer_info->SteerPWM_Error[0]) < 600)
		&& (fabs(CCD1_info->LineError[0]) < 16))//CCD1,CCD2没有丢边,且摇头很小
	{
		if((int16_t)(CCD2_info->RoadWidth[0] - CCD2_info->RoadWidth[6]) < -8)//CCD2路宽有突变
		{
			if((CCD2_info->LeftLine[0] - CCD1_info->LeftLine[0] >= 6)
				&& (fabs(CCD1_info->RightLine[0] - CCD2_info->RightLine[0]) < 6))//CCD1和CCD2左边界相同，右边界不同
			{
				return 1;//路障在左边
			}
			else if((CCD1_info->RightLine[0] - CCD2_info->RightLine[0] >= 6)
					&& (fabs(CCD2_info->LeftLine[0] - CCD1_info->LeftLine[0]) < 6))//CCD1和CCD2右边界相同，左边界不同
			{
				return 2;//路障在右边
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

/*************************************************************************
*					零		度		偏		移                            
*
*  函数名称:myCCD_JudgeWheelOutOfWay
*  功能说明:CCD1 判断是轮子是否会出界
*  参数说明:
*  函数返回:出界(TRUE)/不出界(FALSE)
*  修改时间:2014-08-07
*  备    注：利用CCD1来判断使用3档时当前偏差是否会导致轮子出界
*************************************************************************/
uint8_t myCCD_JudgeWheelOutOfWay(	CCD_Info *CCD1_info,
									CCD_Info *CCD2_info,
									CCD_Info *CCD3_info,
									Speed_Info *Speed_info
								)
{
	if(TRUE == Parameter_info.WheelOutOfWay_Enable)
	{
		//检查右轮是否出界
		if((CCD2_info->LineError[0] < 0)
			&& (CCD1_info->RightLine[0] < CAR_RIGHT_WHEEL_PIXEL))
		{
			CCD3_info->CCD_Ready_Num = 5;

			return TRUE;
		}
		//检查左轮是否出界
		else if((CCD2_info->LineError[0] > 0)
				&& (CCD1_info->LeftLine[0] > CAR_LEFT_WHEEL_PIXEL))
		{
			CCD3_info->CCD_Ready_Num = 5;

			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
}

/*************************************************************************
*					零		度		偏		移                          
*
*  函数名称:myCCD_Get_60_Angle
*  功能说明:搜索人字道60度尖角
*  参数说明:
*  函数返回:	TRUE - 搜索到尖角
*				FALSE - 未搜索到尖角
*
*  修改时间:2014-08-12
*  备    注:
*************************************************************************/	
void myCCD_Get_60_Angle(CCD_Info *CCD1_info,CCD_Info *CCD2_info,CCD_Info *CCD3_info,Speed_Info *Speed_info,Parameter_Info *Parameter_info,Car_State *Car_state)
{
	uint8_t ii = 0,LineTemp = 63;
	
	int16_t Up = 0,Down = 0;
	
	int32_t LeftOrRightLine_Sum = 0;
	int32_t LeftOrRightLine_Ava = 0;
	
	if(0 == CCD3_info->LossLine_Flag)
	{
		LineTemp = CCD3_info->CentralLine[0];
	}
	
	ii = LineTemp;
	
	if((1 == CCD3_info->PixelBinary[ii]) 
		&& (0 == CCD1_info->LeftLoseLine_Flag)
		&& (0 == CCD1_info->RightLoseLine_Flag)
		&& (0 == CCD2_info->LeftLoseLine_Flag)
		&& (0 == CCD2_info->RightLoseLine_Flag))
	{
		//往左边搜索凹槽
		while(ii > CCD3_info->InvalidPixel_Num)
		{
			if(0 == CCD3_info->PixelBinary[ii])//搜索到下降沿
			{
				Down = ii;
				
				break;
			}
				
			ii--;
		}
		
		if(ii > CCD3_info->InvalidPixel_Num)//已经搜索到下降沿
		{
			ii -= 1;
				
			while(ii > CCD3_info->InvalidPixel_Num)
			{
				if(1 == CCD3_info->PixelBinary[ii])//搜索到上升沿
				{
					Up = ii;
					
					if((Down - Up) < 40)
					{
						CCD3_info->Angle_LeftFifo[0] = Down - Up;//更新角度队列
					}
					
					break;
				}
				
				ii--;
			}
		}
		
		//往右边搜索凹槽
		ii = LineTemp;
		
		while(ii < 127 - CCD3_info->InvalidPixel_Num)
		{
			if(0 == CCD3_info->PixelBinary[ii])//搜索到下降沿
			{
				Down = ii;
				
				break;
			}
				
			ii++;
		}
		
		if(ii < 127 - CCD3_info->InvalidPixel_Num)
		{
			ii += 1;
				
			while(ii < 127 - CCD3_info->InvalidPixel_Num)
			{
				if(1 == CCD3_info->PixelBinary[ii])//搜索到上升沿
				{
					Up = ii;
					
					if((Up - Down) < 40)
					{
						CCD3_info->Angle_RightFifo[0] = Up - Down;//更新角度队列
					}
					
					break;
				}
				
				ii++;
			}
		}	
	}

	//判断60度尖角
	if((1 == CCD3_info->LossLine_Flag) 
		&& (0 == Parameter_info->Left60Angle_Flag) 
		&& (0 == Parameter_info->Right60Angle_Flag))
	{
		//判断左边尖角
		if((1 == CCD3_info->LeftLoseLine_Flag) && (0 == CCD3_info->RightLoseLine_Flag))
		{
			for(ii = 0; ii < 3; ii++)
			{
				if((CCD3_info->Angle_LeftFifo[ii + 3] - CCD3_info->Angle_LeftFifo[ii]) <= 0)
				{
					break;
				}
			}
			
			if(3 == ii)
			{
				//------------------------判断左边界是否有太大变化-----------------------//
				LeftOrRightLine_Sum = CCD3_info->LeftLine[0] + CCD3_info->LeftLine[1]
									+ CCD3_info->LeftLine[2] + CCD3_info->LeftLine[3]
									+ CCD3_info->LeftLine[4] + CCD3_info->LeftLine[5];
				
				//求左边界平均值
				LeftOrRightLine_Ava = LeftOrRightLine_Sum / 6;
				
				//求左边界方差
				LeftOrRightLine_Sum = (CCD3_info->LeftLine[0] - LeftOrRightLine_Ava)*(CCD3_info->LeftLine[0] - LeftOrRightLine_Ava)
									+ (CCD3_info->LeftLine[1] - LeftOrRightLine_Ava)*(CCD3_info->LeftLine[1] - LeftOrRightLine_Ava)
									+ (CCD3_info->LeftLine[2] - LeftOrRightLine_Ava)*(CCD3_info->LeftLine[2] - LeftOrRightLine_Ava)
									+ (CCD3_info->LeftLine[3] - LeftOrRightLine_Ava)*(CCD3_info->LeftLine[3] - LeftOrRightLine_Ava)
									+ (CCD3_info->LeftLine[4] - LeftOrRightLine_Ava)*(CCD3_info->LeftLine[4] - LeftOrRightLine_Ava)
									+ (CCD3_info->LeftLine[5] - LeftOrRightLine_Ava)*(CCD3_info->LeftLine[5] - LeftOrRightLine_Ava);
				
				//判断左边界方差是否在正常范围内
				if(LeftOrRightLine_Sum < Parameter_info->Angle60_MeanValue)
				{
					//判断尖角成功
					Parameter_info->Left60Angle_Flag = 1;
					Parameter_info->Right60Angle_Flag = 0;
					
					Speed_info->DistanceOf60Angle_Counter = 80;
					
					LED5_ON;
				}
			}
		}
		//判断右边尖角
		else if((1 == CCD3_info->RightLoseLine_Flag) && (0 == CCD3_info->LeftLoseLine_Flag))
		{
			for(ii = 0; ii < 3; ii++)
			{
				if((CCD3_info->Angle_RightFifo[ii + 3] - CCD3_info->Angle_RightFifo[ii]) <= 0)
				{
					break;
				}
			}
			
			if(3 == ii)
			{
				//------------------------判断右边界是否有太大变化-----------------------//
				LeftOrRightLine_Sum = CCD3_info->RightLine[0] + CCD3_info->RightLine[1]
									+ CCD3_info->RightLine[2] + CCD3_info->RightLine[3]
									+ CCD3_info->RightLine[4] + CCD3_info->RightLine[5];
				
				//求右边界平均值
				LeftOrRightLine_Ava = LeftOrRightLine_Sum / 6;
				
				//求右边界方差
				LeftOrRightLine_Sum = (CCD3_info->RightLine[0] - LeftOrRightLine_Ava)*(CCD3_info->RightLine[0] - LeftOrRightLine_Ava)
									+ (CCD3_info->RightLine[1] - LeftOrRightLine_Ava)*(CCD3_info->RightLine[1] - LeftOrRightLine_Ava)
									+ (CCD3_info->RightLine[2] - LeftOrRightLine_Ava)*(CCD3_info->RightLine[2] - LeftOrRightLine_Ava)
									+ (CCD3_info->RightLine[3] - LeftOrRightLine_Ava)*(CCD3_info->RightLine[3] - LeftOrRightLine_Ava)
									+ (CCD3_info->RightLine[4] - LeftOrRightLine_Ava)*(CCD3_info->RightLine[4] - LeftOrRightLine_Ava)
									+ (CCD3_info->RightLine[5] - LeftOrRightLine_Ava)*(CCD3_info->RightLine[5] - LeftOrRightLine_Ava);
				
				//判断右边界方差是否在正常范围内
				if(LeftOrRightLine_Sum < Parameter_info->Angle60_MeanValue)
				{
					//判断尖角成功
					Parameter_info->Left60Angle_Flag = 0;
					Parameter_info->Right60Angle_Flag = 1;
					
					Speed_info->DistanceOf60Angle_Counter = 80;
					
					LED5_ON;
				}
			}
		}
	}
}

/*************************************************************************
*					零		度		偏		移                            
*
*  函数名称:myCCD_GetRoadCondition
*  功能说明:不依赖于搜线方式的赛道类型判断
*  参数说明:
*  函数返回:当前赛道类型
*  修改时间:2014-08-13
*  备    注：该方法不依赖于搜线方式来判断当前的赛道类型，是从图像直接
*			获得的赛道类型。
*************************************************************************/
#ifdef GetRoadCondition_Enable

Road_Condition myCCD_GetRoadCondition(	CCD_Info *CCD1_info,
										CCD_Info *CCD2_info,
										CCD_Info *CCD3_info,
										Speed_Info *Speed_info
									 )
{
	uint8_t ii = 0;
	
	//更新CCD1的左右边界和中心点
	CCD1_info->Rn_Left[0] = CCD1_info->LeftLine[0];
	CCD1_info->Rn_Right[0] = CCD1_info->RightLine[0];
	
	//更新CCD2的左右边界和中心点
	CCD2_info->Rn_Left[0] = CCD2_info->LeftLine[0];
	CCD2_info->Rn_Right[0] = CCD2_info->RightLine[0];
	
	
	
	return NeedJudge;
}

#endif
/*************************************************************************
*					零		度		偏		移                            
*
*  函数名称:myCCD_GPIO_Init(void)
*  功能说明:CCD IO初始化
*  参数说明:
*  函数返回:
*  修改时间:
*  备    注：
*************************************************************************/	
void myCCD_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//CCD1_CK	B16
	GPIO_InitStructure.GPIO_Direct=GPIO_Direct_Out;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_PU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_16;
	LDPY_GPIO_Init(PORTB,&GPIO_InitStructure);
	
	//CCD1_SI  	B11	
	GPIO_InitStructure.GPIO_Direct=GPIO_Direct_Out;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_PU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	LDPY_GPIO_Init(PORTB,&GPIO_InitStructure);	
		
	//CCD2_CK	B8
	GPIO_InitStructure.GPIO_Direct=GPIO_Direct_Out;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_PU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
	LDPY_GPIO_Init(PORTB,&GPIO_InitStructure);
	
	//CCD2_SI  	B7	
	GPIO_InitStructure.GPIO_Direct=GPIO_Direct_Out;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_PU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
	LDPY_GPIO_Init(PORTB,&GPIO_InitStructure);	
		
	//CCD3_CK	B5
	GPIO_InitStructure.GPIO_Direct=GPIO_Direct_Out;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_PU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;
	LDPY_GPIO_Init(PORTB,&GPIO_InitStructure);
	
	//CCD3_SI  	B4	
	GPIO_InitStructure.GPIO_Direct=GPIO_Direct_Out;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_PU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
	LDPY_GPIO_Init(PORTB,&GPIO_InitStructure);	
}
/*************************************************************************
*					零		度		偏		移                                
*
*  函数名称:myCCD_CCD1_Sampling
*  功能说明:CCD1采样
*  参数说明:
*  函数返回:无
*  修改时间:
*  备    注: 
*************************************************************************/		
void myCCD_CCD1_Sampling(CCD_Info *CCD_info)
{
	uint8_t i;

	CCD1_SI=1;						//CCD复位
	LDPY_Delay_10ns(3);
	CCD1_CK=1;						//
	LDPY_Delay_10ns(20);
	CCD1_SI=0;	
	LDPY_Delay_10ns(20);

	LDPY_Delay_10ns(3000);				//延时以保证前几个点平滑
	
	for (i = 0; i < 128; i++)
	{
		 
		CCD1_CK=1;							//下降沿AO输出电压值
		LDPY_Delay_10ns(5);
		 
		CCD_info->PixelOri[0][i] = LDPY_ADC1_GetValue(1); 	//ADC采样

		CCD1_CK=0;
		LDPY_Delay_10ns(5);
	}
	 
	CCD1_CK=1;
	LDPY_Delay_10ns(20);
	CCD1_CK=0;
	LDPY_Delay_10ns(20);
}
/*************************************************************************
*					零		度		偏		移                             
*
*  函数名称:myCCD_CCD2_Sampling
*  功能说明:CCD2采样
*  参数说明:
*  函数返回:无
*  修改时间:
*  备    注: 
*************************************************************************/		
void myCCD_CCD2_Sampling(CCD_Info *CCD_info)
{
	uint8_t i;
	
	CCD2_SI=1;						//CCD复位
	LDPY_Delay_10ns(3);
	CCD2_CK=1;						
	LDPY_Delay_10ns(20);
	CCD2_SI=0;	
	LDPY_Delay_10ns(20);
	
	LDPY_Delay_10ns(3000);				//延时以保证前几个点平滑
	
	for (i = 0; i < 128; i++)
	{
		 
		CCD2_CK=1;						//下降沿AO输出电压值
		LDPY_Delay_10ns(5);
		 
		CCD_info->PixelOri[0][i] = LDPY_ADC1_GetValue(2); //ADC采样

		CCD2_CK=0;
		LDPY_Delay_10ns(5);
	}
	 
	CCD2_CK=1;
	LDPY_Delay_10ns(20);
	CCD2_CK=0;
	LDPY_Delay_10ns(20);
}	

/*************************************************************************
*					零		度		偏		移                             
*
*  函数名称:CCD3_Sampling
*  功能说明:CCD3采样
*  参数说明:
*  函数返回:无
*  修改时间:
*  备    注: 
*************************************************************************/		
	void myCCD_CCD3_Sampling(CCD_Info *CCD_info)
	{
		uint8_t ii;
		
		CCD3_SI=1;						//CCD复位
		LDPY_Delay_10ns(3);
		CCD3_CK=1;						//
		LDPY_Delay_10ns(20);
		CCD3_SI=0;	
		LDPY_Delay_10ns(20);
		
		LDPY_Delay_10ns(3000);				//延时以保证前几个点平滑
		
		for (ii = 0; ii < 128; ii++)
		 {
			 
			 CCD3_CK=1;				//下降沿AO输出电压值
			 LDPY_Delay_10ns(10);
			 
			 CCD_info->PixelOri[0][ii] = LDPY_ADC0_GetValue(3); //ADC采样

			 CCD3_CK=0;
			 LDPY_Delay_10ns(10);
		 }
		 CCD3_CK=1;
		 LDPY_Delay_10ns(20);
		 CCD3_CK=0;
		 LDPY_Delay_10ns(20);
	}	

/*************************************************************************
*					零		度		偏		移                               
*
*  函数名称:myCCD_Filter
*  功能说明:CCD数据滤波和二值化
*  参数说明:
*  函数返回:无
*  修改时间:2014-05-21
*  备    注:暗电流的AD值约为1000，不放大的时候为高 2000，低 400
*************************************************************************/		
void myCCD_FilterAndBinarization(CCD_Info *CCD1_info,CCD_Info *CCD2_info,CCD_Info *CCD3_info,Parameter_Info *Parameter_info)
{
	uint8 ii = 0;
	float CentralLine_ExposureVoltage = 0;
	uint32 AD1_MAX = 0;
	uint32 AD1_MIN = 4095;
	uint32 AD_MultK = 16;
	
	uint16_t BinaryEdgeValue = 800;

	//进行CCD数据采集滤波
	for (ii = 0; ii < 128; ii++)//60us
	{
		CCD1_info->Pixel[ii] = (CCD1_info->PixelOri[0][ii] + CCD1_info->PixelOri[1][ii]) >> 1;			
		CCD2_info->Pixel[ii] = (CCD2_info->PixelOri[0][ii] + CCD2_info->PixelOri[1][ii]) >> 1;
		CCD3_info->Pixel[ii] = (CCD3_info->PixelOri[0][ii] + CCD3_info->PixelOri[1][ii]) >> 1;
		
		//移动CCD1原始数据队列
		CCD1_info->PixelOri[1][ii] = CCD1_info->PixelOri[0][ii];
		
		//移动CCD2原始数据队列
		CCD2_info->PixelOri[1][ii] = CCD2_info->PixelOri[0][ii];
		
		//移动CCD3原始数据队列
		CCD3_info->PixelOri[1][ii] = CCD3_info->PixelOri[0][ii];
		
		//去CCD1暗电流
		if (CCD1_info->Pixel[ii] > 1000)
		{
		   CCD1_info->Pixel[ii] = CCD1_info->Pixel[ii] - 1000;
		}
	    else
		{
		   CCD1_info->Pixel[ii] = 0;
		}
		
		//去CCD2暗电流
	    if (CCD2_info->Pixel[ii] > 1000)
		{
		   CCD2_info->Pixel[ii] = CCD2_info->Pixel[ii] - 1000;
		}
	    else
		{
		   CCD2_info->Pixel[ii] = 0;
		}
		
		//去CCD3暗电流
	    if (CCD3_info->Pixel[ii] > 1000)
		{
		   CCD3_info->Pixel[ii] = CCD3_info->Pixel[ii] - 1000;
		}
	    else
		{
		   CCD3_info->Pixel[ii] = 0;
		}
	}

	//如果上次的中线点在允许范围内且不处于人字道，十字道，坡道则可以用来进行动态放大
	if ((Turn_Round != Car_state)
		&& ((Ramp_Up != Road_condition) && (Ramp_Down != Road_condition))
		&& (1 != CCD1_info->Cross_Flag)
		&& (CCD1_info->CentralLine[0] > 50) && (CCD1_info->CentralLine[0] < 76))
	{
		//用上一次的放大后的原始值来计算放大倍数
		CentralLine_ExposureVoltage = 	(	CCD1_info->Pixel[CCD1_info->CentralLine[0]]
										  +	CCD1_info->Pixel[CCD1_info->CentralLine[0] - 1]
										  +	CCD1_info->Pixel[CCD1_info->CentralLine[0] - 2]
										  +	CCD1_info->Pixel[CCD1_info->CentralLine[0] + 1]
										  +	CCD1_info->Pixel[CCD1_info->CentralLine[0] + 2]
										)/5;
		
		Parameter_info->AD_MultK_f = Parameter_info->CCD1_AD_TargetValue / CentralLine_ExposureVoltage;
	}
	
	//防止放大倍数过小和过大(1400/3096 = 0.45,1400/400 = 3.5)
	if (Parameter_info->AD_MultK_f >  3.5)
	{
		Parameter_info->AD_MultK_f = 3.5;
	}
	else if (Parameter_info->AD_MultK_f < 0.45)
	{
		Parameter_info->AD_MultK_f = 0.45;
	}
	
	//把放大倍数放大为整型数据
	AD_MultK = (uint32_t)(Parameter_info->AD_MultK_f*16);

	//进行CCD1 CCD2 CCD3数据放大
	for (ii = 0; ii < 128; ii++)//80us
	{		
		CCD1_info->Pixel[ii] = (CCD1_info->Pixel[ii] >> 4)*AD_MultK;//对CCD1进行放大
		CCD2_info->Pixel[ii] = (CCD2_info->Pixel[ii] >> 4)*AD_MultK;//对CCD2进行放大
		CCD3_info->Pixel[ii] = (CCD3_info->Pixel[ii] >> 4)*AD_MultK;//对CCD3进行放大
		
		//对CCD1放大后的AD值进行限幅
		if (CCD1_info->Pixel[ii] > 4095)
		{
			CCD1_info->Pixel[ii] = 4095;
		}
		
		//对CCD2放大后的AD值进行限幅
		if (CCD2_info->Pixel[ii] > 4095)
		{
			CCD2_info->Pixel[ii] = 4095;
		}
		
		//对CCD3放大后的AD值进行限幅
		if (CCD3_info->Pixel[ii] > 4095)
		{
			CCD3_info->Pixel[ii] = 4095;
		}
		
		if ((ii >= 16) && (ii <= 112))
		{
			if (AD1_MAX < CCD1_info->Pixel[ii])		//找出CCD1放大后的AD最大值
			{
				AD1_MAX = CCD1_info->Pixel[ii];
			}
			
			if (AD1_MIN > CCD1_info->Pixel[ii])		//找出CCD1放大后的AD最小值
			{
				AD1_MIN = CCD1_info->Pixel[ii];
			}
		}
	}

	//更新AD_MAX,AD_MIN队列
	CCD1_info->AD_MAX[0] = AD1_MAX;
	CCD1_info->AD_MIN[0] = AD1_MIN;

	//计算CCD1二值化的分界值
	BinaryEdgeValue = (		CCD1_info->AD_MAX[3] + CCD1_info->AD_MAX[2] + CCD1_info->AD_MAX[1] + CCD1_info->AD_MAX[0]
						  + CCD1_info->AD_MIN[3] + CCD1_info->AD_MIN[2] + CCD1_info->AD_MIN[1] + CCD1_info->AD_MIN[0]
					  ) >> 3;

	if(CheckCCD == Car_mode)
	{
		myOLED_Dec(4,90,BinaryEdgeValue);
	}
	
	//限制二值化边界值的最大值最小值(阈值在750左右)
	if (BinaryEdgeValue < Parameter_info->CCD1_Binary_MIN)
	{
		BinaryEdgeValue = Parameter_info->CCD1_Binary_MIN;
	}
	else if (BinaryEdgeValue > Parameter_info->CCD1_Binary_MAX)
	{
		BinaryEdgeValue = Parameter_info->CCD1_Binary_MAX;
	}
	
	//CCD数据的二值化
	for (ii = 0; ii < 128; ii++)
	{
		CCD1_info->PixelBinary[ii] = (CCD1_info->Pixel[ii] >= BinaryEdgeValue) ? 1 : 0;//CCD1数据二值化
		CCD2_info->PixelBinary[ii] = (CCD2_info->Pixel[ii] >= BinaryEdgeValue) ? 1 : 0;//CCD2数据二值化
		CCD3_info->PixelBinary[ii] = (CCD3_info->Pixel[ii] >= BinaryEdgeValue) ? 1 : 0;//CCD3数据二值化
	}
}

/*************************************************************************
*					零		度		偏		移                            
*
*  函数名称:myCCD_FilterAtFirst
*  功能说明:CCD数据头几次移动
*  参数说明:
*  函数返回:无
*  修改时间:2014-06-21
*  备    注:
*************************************************************************/	
void myCCD_FilterAtFirst(CCD_Info *CCD1_info,CCD_Info *CCD2_info,CCD_Info *CCD3_info)
{
	int16_t ii = 0;
	
	//初始化移动CCD数据数组
	for (ii = 0; ii < 128; ii++)
	{
		//移动CCD1原始数据队列
		CCD1_info->PixelOri[1][ii] = CCD1_info->PixelOri[0][ii];
		
		//移动CCD2原始数据队列
		CCD2_info->PixelOri[1][ii] = CCD2_info->PixelOri[0][ii];
		
		//移动CCD3原始数据队列
		CCD3_info->PixelOri[1][ii] = CCD3_info->PixelOri[0][ii];
	}
}

/*************************************************************************
*					零		度		偏		移                           
*
*  函数名称:myCCD_GetBeginLineError
*  功能说明:CCD数据头几次采集,处理和移动
*  参数说明:
*  函数返回:无
*  修改时间:2014-06-26
*  备    注:
*************************************************************************/	
void myCCD_GetBeginLineError(	CCD_Info *CCD1_info,
								CCD_Info *CCD2_info,
								CCD_Info *CCD3_info,
								Speed_Info *Speed_info,
								Parameter_Info *Parameter_info
							)
{
	uint8_t ii = 0;
	uint32_t Gyro_Sum = 0;
	
	//CCD数据的头几次滤波处理
	ii = 5;
	
	do
	{
		if (1 == Parameter_info->CCD_SampleOK_Flag)
		{
			Parameter_info->CCD_SampleOK_Flag = 0;
			
			myCCD_FilterAtFirst(CCD1_info,CCD2_info,CCD3_info);
			
			ii--;
		}
	}while(ii > 0);
	
	//起跑时的偏差计算和移动
	ii = Line_SIZE;
	
	do
	{
		if (1 == Parameter_info->CCD_SampleOK_Flag)
		{
			Parameter_info->CCD_SampleOK_Flag = 0;
			
			//数据滤波 二值化
			myCCD_FilterAndBinarization(CCD1_info,CCD2_info,CCD3_info,Parameter_info);

			//CCD搜边线和偏差计算
			myCCD_CCD1_GetLineError(CCD1_info,Speed_info);
			myCCD_CCD2_GetLineError(CCD1_info,CCD2_info,Speed_info);
			myCCD_CCD3_GetLineError(CCD3_info,Speed_info);
			
			ii--;
		}
	}while(ii > 0);

	CCD1_info->RoadWidthOfStraight = CCD1_info->RightLine[0] - CCD1_info->LeftLine[0];
	CCD2_info->RoadWidthOfStraight = CCD2_info->RightLine[0] - CCD2_info->LeftLine[0];
	CCD3_info->RoadWidthOfStraight = CCD3_info->RightLine[0] - CCD3_info->LeftLine[0];
	
	//设定初始化档位
	if((0 == CCD1_info->LossLine_Flag) && (0 == CCD2_info->LossLine_Flag))
	{
		CCD1_info->CCD_Ready_Num = CCD1_DataReady_Num;
		CCD2_info->CCD_Ready_Num = CCD2_DataReady_Num;
	}
	
	Speed_info->SpeedMode = 2;//初始化为2档
	
	//采集陀螺仪零点
	for(ii = 0; ii < GYRO_LENGTH; ii++)//连续采集16次求平均值
	{
		Gyro_info.Gyroscope_Fifo[ii] = LDPY_ADC1_ENC();
		
		Gyro_Sum += Gyro_info.Gyroscope_Fifo[ii];
	}
	
	//更新零点
	Gyro_info.GyroscopeZero = Gyro_Sum >> 4;
}

/*---------------------------end of myCCD.c-----------------------------*/
