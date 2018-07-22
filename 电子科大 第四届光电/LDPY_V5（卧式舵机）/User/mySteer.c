

#include "mySteer.h"

	Steer_Info Steer_info = {0};
	HeadSteer_Info HeadSteer_info = {0};
	
	extern CCD_Info CCD2_info;
	extern CCD_Info CCD3_info;
	
	extern Parameter_Info Parameter_info;
	
	extern Road_Condition Road_condition;
	
	#ifdef DMA_CH2_SendLabVIEW_Info_Enable
		
		extern LabVIEW_Info LabVIEW_info;
	
	#endif

/*************************************************************************
*					零		度		偏		移  
*
*  函数名称：mySteer_HeadSteerDataInit
*  功能说明：摇头舵机参数初始化
*  参数说明：
*  函数返回：无
*  修改时间：2014-06-29
*  备    注：
*************************************************************************/
void mySteer_HeadSteerDataInit(HeadSteer_Info *HeadSteer_info)
{
	uint8 ii = 0;
	
	HeadSteer_info->KPH = 30.0;
	HeadSteer_info->KIH = 1.40;
	
	//摇头舵机频率-200Hz	
	HeadSteer_info->Steer_Center = 5000;
	HeadSteer_info->Steer_LeftMAX = 4200;//2000是 -90 度,4250
	HeadSteer_info->Steer_RightMAX = 5800;//7800是 +90 度,5890
	
	HeadSteer_info->Steer_PWMOutputDuty = HeadSteer_info->Steer_Center;
	
	for(ii = 0; ii < 10; ii++)
	{
		HeadSteer_info->SteerPWM_Error[ii] = 0;
		HeadSteer_info->SteerPWM_Error_D[ii] = 0;
	}
	
	FTM2->CONTROLS[1].CnV = HeadSteer_info->Steer_PWMOutputDuty;
}

/*************************************************************************
*					零		度		偏		移  
*
*  函数名称：mySteer_DataInit
*  功能说明：转向舵机参数初始化
*  参数说明：
*  函数返回：无
*  修改时间：2014-06-29
*  备    注：
*************************************************************************/
void mySteer_DataInit(Steer_Info *Steer_info)
{
	uint8_t ii = 0;
	
 	Steer_info->KP1 = 20;
	Steer_info->KP2 = 10;
	Steer_info->KP3 = 6;
	
	Steer_info->KI = 0;
	
	Steer_info->HeadToSteer_1 = 1;
	Steer_info->HeadToSteer_2 = 1.05;
	Steer_info->HeadToSteer_3 = 0.6;
	
	Steer_info->KP_Mult = 0;
	Steer_info->KD_Mult = 0;
	Steer_info->KI_Mult = 0;

	//转向舵机频率-200Hz	
	Steer_info->Steer_Center = 5040;
	Steer_info->Steer_RightMAX = 4100;
	Steer_info->Steer_LeftMAX = 5980;
	
	for(ii = 0; ii < 10; ii++)
	{
		Steer_info->SteerPWM_Error[ii] = 0;
	}
	
	Steer_info->Steer_PWMOutputDuty = Steer_info->Steer_Center;
	
	Steer_info->RampUp_SteerBackToCenter_Flag = 0;
	
	FTM2->CONTROLS[0].CnV = Steer_info->Steer_PWMOutputDuty;
}

/*************************************************************************
*					零		度		偏		移  
*
*  函数名称：mySteer_HeadSteerControl
*  功能说明：摇头舵机增量式PID计算及摇头舵机的控制
*  参数说明：
*  函数返回：无
*  修改时间：2014-07-07
*  备    注：
***************************************w**********************************/
void mySteer_HeadSteerControl(	CCD_Info *CCD1_info,
								CCD_Info *CCD2_info,
								CCD_Info *CCD3_info,
								HeadSteer_Info *HeadSteer_info,
								Speed_Info *Speed_info,
								Car_State *Car_state,
								Car_State *Car_state_Pre
							)
{
	float HeadSteerError = 0.0;//摇头舵机需要调节的偏差
	
	if(Turn_Round != *Car_state)//不处于人字道时控制摇头
	{
		if(1 != CCD1_info->LossLine_Flag)//CCD1没有丢线
		{	
			if(1 != CCD2_info->Cross_Flag)//CCD2没有处于十字道
			{
				switch(Speed_info->SpeedMode)//根据档位选择使用相应CCD的偏差
				{
					case 1://CCD1
						{
							HeadSteerError =  HeadSteer_info->KPH*(CCD1_info->LineError[0] - CCD1_info->LineError[1])
											+ HeadSteer_info->KIH*CCD1_info->LineError[0];
							
							#ifdef DMA_CH2_SendLabVIEW_Info_Enable
								
								LabVIEW_info.HeadSteer_KP_Item = HeadSteer_info->KPH*(CCD1_info->LineError[0] - CCD1_info->LineError[1]);
								LabVIEW_info.HeadSteer_KI_Item = HeadSteer_info->KIH*CCD1_info->LineError[0];
							
							#endif
						}
						break;
					case 2://CCD2
						{
							HeadSteerError =  HeadSteer_info->KPH*(CCD2_info->LineError[0] - CCD2_info->LineError[1])
											+ HeadSteer_info->KIH*CCD2_info->LineError[0];
							
							#ifdef DMA_CH2_SendLabVIEW_Info_Enable
								
								LabVIEW_info.HeadSteer_KP_Item = HeadSteer_info->KPH*(CCD2_info->LineError[0] - CCD2_info->LineError[1]);
								LabVIEW_info.HeadSteer_KI_Item = HeadSteer_info->KIH*CCD2_info->LineError[0];
							
							#endif
						}
						break;
					case 3://CCD2
						{
							HeadSteerError =  10*(CCD3_info->LineError[0] - CCD3_info->LineError[1])
											+ 0.8*CCD3_info->LineError[0];
							
							#ifdef DMA_CH2_SendLabVIEW_Info_Enable
								
								LabVIEW_info.HeadSteer_KP_Item = 10*(CCD2_info->LineError[0] - CCD2_info->LineError[1]);
								LabVIEW_info.HeadSteer_KI_Item = 0.8*CCD2_info->LineError[0];
							
							#endif
						}
						break;
					default:
						break;
				}
				
				//更新摇头舵机的输出值
				HeadSteer_info->Steer_PWMOutputDuty += HeadSteerError;
			}
			else//CCD2十字道丢线
			{
				if (CCD3_info->CCD_Ready_Num >= 1)//CCD3数据有效
				{
					HeadSteerError = 10*(CCD3_info->LineError[0] - CCD3_info->LineError[1])
								   + 0.8*CCD3_info->LineError[0];

					//更新摇头舵机的输出值
					HeadSteer_info->Steer_PWMOutputDuty += HeadSteerError;
					
					#ifdef DMA_CH2_SendLabVIEW_Info_Enable
							
							LabVIEW_info.HeadSteer_KP_Item = 10*(CCD3_info->LineError[0] - CCD3_info->LineError[1]);
							LabVIEW_info.HeadSteer_KI_Item = 0.8*CCD3_info->LineError[0];
						
					#endif
				}
				else if (0 == CCD1_info->LossLine_Flag)//CCD3数据无效,CCD1数据有效
				{
					HeadSteerError =  HeadSteer_info->KPH*(CCD1_info->LineError[0] - CCD1_info->LineError[1])
									+ HeadSteer_info->KIH*CCD1_info->LineError[0];
					
					//更新摇头舵机的输出值
					HeadSteer_info->Steer_PWMOutputDuty += HeadSteerError;
					
					#ifdef DMA_CH2_SendLabVIEW_Info_Enable
							
							LabVIEW_info.HeadSteer_KP_Item = HeadSteer_info->KPH*(CCD1_info->LineError[0] - CCD1_info->LineError[1]);
							LabVIEW_info.HeadSteer_KI_Item = HeadSteer_info->KIH*CCD1_info->LineError[0];
						
					#endif
				}
				else if(1 == Speed_info->DistanceOf1Cm_Flag)//CCD3数据无效，CCD1数据无效，则摇头舵机逐渐回正
				{
					if(HeadSteer_info->SteerPWM_Error[0] >= 40)
					{
						HeadSteer_info->Steer_PWMOutputDuty -= 40;
					}
					else if(HeadSteer_info->SteerPWM_Error[0] <= -40)
					{
						HeadSteer_info->Steer_PWMOutputDuty += 40;
					}
				}
			}
		}
		else//CCD1全丢线
		{	
			if((1 == CCD1_info->LossLine_Flag) && ((Ramp_Up == Road_condition) || (Ramp_Down == Road_condition)))//坡道丢线
			{
				HeadSteerError = 0;
				CCD1_info->LineError[0] = 0;
				CCD1_info->LineError[1] = 0;
				
				HeadSteer_info->Steer_PWMOutputDuty = HeadSteer_info->Steer_Center;//CCD1丢线之后舵机摇头舵机回正直走
			}
			else if(1 == CCD1_info->Cross_Flag)//CCD1十字道丢线
			{
				Speed_info->SpeedMode = 2;//十字道用2档速度通过
				
				if (0 == CCD2_info->LossLine_Flag)//CCD2没有丢线
			{
					HeadSteerError =  HeadSteer_info->KPH*(CCD2_info->LineError[0] - CCD2_info->LineError[1])
									+ HeadSteer_info->KIH*CCD2_info->LineError[0];
					
					//更新摇头舵机的输出值
					HeadSteer_info->Steer_PWMOutputDuty += HeadSteerError;
					
					#ifdef DMA_CH2_SendLabVIEW_Info_Enable
							
							LabVIEW_info.HeadSteer_KP_Item = HeadSteer_info->KPH*(CCD2_info->LineError[0] - CCD2_info->LineError[1]);
							LabVIEW_info.HeadSteer_KI_Item = HeadSteer_info->KIH*CCD2_info->LineError[0];
						
					#endif
					
				}
				else if (CCD3_info->CCD_Ready_Num >= 1)//CCD2数据无效,CCD3数据有效
				{
					HeadSteerError =  10*(CCD3_info->LineError[0] - CCD3_info->LineError[1])
									+ 0.8*CCD3_info->LineError[0];
					
					//更新摇头舵机的输出值
					HeadSteer_info->Steer_PWMOutputDuty += HeadSteerError;
					
					#ifdef DMA_CH2_SendLabVIEW_Info_Enable
							
							LabVIEW_info.HeadSteer_KP_Item = 10*(CCD3_info->LineError[0] - CCD3_info->LineError[1]);
							LabVIEW_info.HeadSteer_KI_Item = 0.8*CCD3_info->LineError[0];
						
					#endif
				}
				else if(1 == Speed_info->DistanceOf1Cm_Flag)//CCD3数据无效，CCD2数据无效，则摇头舵机逐渐回正
				{
					if(HeadSteer_info->SteerPWM_Error[0] >= 40)
					{
						HeadSteer_info->Steer_PWMOutputDuty -= 40;
					}
					else if(HeadSteer_info->SteerPWM_Error[0] <= -40)
					{
						HeadSteer_info->Steer_PWMOutputDuty += 40;
					}
				}
			}
		}
	}//end of if(TurnRound != *Car_state)
	 
	/*-------------------------遇限削弱和抗积分饱和处理----------------------*/
	if((HeadSteerError > 0) && (HeadSteer_info->Steer_PWMOutputDuty >= HeadSteer_info->Steer_RightMAX))
	{
		HeadSteerError = 0;
	}
	else if((HeadSteerError < 0) && (HeadSteer_info->Steer_PWMOutputDuty <= HeadSteer_info->Steer_LeftMAX))
	{
		HeadSteerError = 0;
	}
	//-------------------------遇限削弱和抗积分饱处理结束----------------------//
	
	//--------------------------摇头舵机的最后总限幅---------------------------//
	if(HeadSteer_info->Steer_PWMOutputDuty < HeadSteer_info->Steer_LeftMAX)//摇头超过左极限
	{
		HeadSteer_info->Steer_PWMOutputDuty = HeadSteer_info->Steer_LeftMAX;
	}
	else if(HeadSteer_info->Steer_PWMOutputDuty > HeadSteer_info->Steer_RightMAX)//摇头超过右极限
	{
		HeadSteer_info->Steer_PWMOutputDuty = HeadSteer_info->Steer_RightMAX; 
	}
	
	//摇头舵机输出
	FTM2->CONTROLS[1].CnV = HeadSteer_info->Steer_PWMOutputDuty;
	
	if(1 == Speed_info->DistanceOf1Cm_Flag)//1cm移动一次队列
	{
		Speed_info->DistanceOf1Cm_Flag = 0;//清零1cm到标记
		
		//移动摇头舵机偏差队列
		HeadSteer_info->SteerPWM_Error[9] = HeadSteer_info->SteerPWM_Error[8];
		HeadSteer_info->SteerPWM_Error[8] = HeadSteer_info->SteerPWM_Error[7];
		HeadSteer_info->SteerPWM_Error[7] = HeadSteer_info->SteerPWM_Error[6];
		HeadSteer_info->SteerPWM_Error[6] = HeadSteer_info->SteerPWM_Error[5];
		HeadSteer_info->SteerPWM_Error[5] = HeadSteer_info->SteerPWM_Error[4];
		HeadSteer_info->SteerPWM_Error[4] = HeadSteer_info->SteerPWM_Error[3];
		HeadSteer_info->SteerPWM_Error[3] = HeadSteer_info->SteerPWM_Error[2];
		HeadSteer_info->SteerPWM_Error[2] = HeadSteer_info->SteerPWM_Error[1];
		HeadSteer_info->SteerPWM_Error[1] = HeadSteer_info->SteerPWM_Error[0];
		
		//移动摇头舵机偏差率队列
		HeadSteer_info->SteerPWM_Error_D[9] = HeadSteer_info->SteerPWM_Error_D[8];
		HeadSteer_info->SteerPWM_Error_D[8] = HeadSteer_info->SteerPWM_Error_D[7];
		HeadSteer_info->SteerPWM_Error_D[7] = HeadSteer_info->SteerPWM_Error_D[6];
		HeadSteer_info->SteerPWM_Error_D[6] = HeadSteer_info->SteerPWM_Error_D[5];
		HeadSteer_info->SteerPWM_Error_D[5] = HeadSteer_info->SteerPWM_Error_D[4];
		HeadSteer_info->SteerPWM_Error_D[4] = HeadSteer_info->SteerPWM_Error_D[3];
		HeadSteer_info->SteerPWM_Error_D[3] = HeadSteer_info->SteerPWM_Error_D[2];
		HeadSteer_info->SteerPWM_Error_D[2] = HeadSteer_info->SteerPWM_Error_D[1];
		HeadSteer_info->SteerPWM_Error_D[1] = HeadSteer_info->SteerPWM_Error_D[0];
		
		Steer_info.RampUp_SteerBackToCenter_Flag = 1;//转向舵机回正标记
	}
	
	//更新摇头舵机输出角度偏差队列(用占空比表示)
	HeadSteer_info->SteerPWM_Error[0] = HeadSteer_info->Steer_PWMOutputDuty - HeadSteer_info->Steer_Center;
	
	//更新摇头舵机的角度偏差率
	HeadSteer_info->SteerPWM_Error_D[0] = HeadSteer_info->SteerPWM_Error[0] - HeadSteer_info->SteerPWM_Error[9];
	
}

/*************************************************************************
*					零		度		偏		移  
*
*  函数名称：mySteer_DirectionControl
*  功能说明：转向舵机位置式PID计算及转向舵机的控制
*  参数说明：
*  函数返回：无
*  修改时间：2014-06-21
*  备    注：根据摇头舵机的打角以及偏差综合计算得到转向舵机的打角
*			且转向舵机打角与摇头舵机打角成比例关系，再利用余差来
*			纠正打角
*************************************************************************/
void mySteer_DirectionControl(CCD_Info *CCD1_info,CCD_Info *CCD2_info,CCD_Info *CCD3_info,HeadSteer_Info *HeadSteer_info,Steer_Info *Steer_info,Speed_Info *Speed_info)
{
	float LineError = 0.0;
	float HeadToSteer_Temp = 0;
	
	//初始高速距离减弱校正
	if(Parameter_info.Mode_HighSpeedDistance <= 0)
	{
		switch(Speed_info->SpeedMode)
		{
			case 1://CCD1
					LineError = Steer_info->KP1*CCD1_info->LineError[0];
					Bell_ON;
					break;
			case 2://CCD2
					LineError = Steer_info->KP2*CCD2_info->LineError[0];
					break;
			case 3://CCD2
					LineError = Steer_info->KP3*CCD3_info->LineError[0];
					break;
			default:
					break;
		}
	}

	//------------------------更新转向舵机的输出角度---------------------------//
	//上坡
	if(Ramp_Up == Road_condition)
	{
		if(1 != CCD1_info->LossLine_Flag)//CCD1没有丢线
		{
			Steer_info->Steer_PWMOutputDuty = (uint32)(Steer_info->Steer_Center - Steer_info->HeadToSteer_1*HeadSteer_info->SteerPWM_Error[1] - 0.4*LineError);
		}
		else if((1 == Steer_info->RampUp_SteerBackToCenter_Flag) && (1 == CCD1_info->LossLine_Flag))//CCD1已经丢线,转向舵机逐渐回正直走
		{
			//清零上坡回正标记放在舵机偏差队列移动之后
			
			if(Steer_info->Steer_PWMOutputDuty > Steer_info->Steer_Center)
			{	
				if(Steer_info->Steer_PWMOutputDuty > Steer_info->Steer_Center + 20)
				{
					Steer_info->Steer_PWMOutputDuty -= 20;
				}
				else
				{
					Steer_info->Steer_PWMOutputDuty = Steer_info->Steer_Center;
				}
			}
			else if(Steer_info->Steer_PWMOutputDuty < Steer_info->Steer_Center)
			{
				if(Steer_info->Steer_PWMOutputDuty < Steer_info->Steer_Center - 20)
				{
					Steer_info->Steer_PWMOutputDuty += 20;
				}
				else
				{
					Steer_info->Steer_PWMOutputDuty = Steer_info->Steer_Center;
				}
			
			}
		}
	}
	else if ((Ramp_Down == Road_condition) && (1 != CCD1_info->LossLine_Flag))//下坡,CCD1搜到两边的线,舵机不能够打角过大
	{
		Steer_info->Steer_PWMOutputDuty = (uint32)(Steer_info->Steer_Center - HeadSteer_info->SteerPWM_Error[1] - 0.4*LineError);
	
		if(Steer_info->SteerPWM_Error[0] > 400)
		{
			Steer_info->Steer_PWMOutputDuty = Steer_info->Steer_Center + 400;
		}
		else if(Steer_info->SteerPWM_Error[0] < -400)
		{
			Steer_info->Steer_PWMOutputDuty = Steer_info->Steer_Center - 400;
		}
	
	}
	else if((1 == CCD2_info->Cross_Flag) || (1 == CCD1_info->Cross_Flag))//CCD2进入十字道
	{		
		LineError = 0;
		
		Steer_info->Steer_PWMOutputDuty = (uint32_t)(Steer_info->Steer_Center - Steer_info->HeadToSteer_3*HeadSteer_info->SteerPWM_Error[1]);
	
		//发送临时变量
		#ifdef DMA_CH2_SendLabVIEW_Info_Enable
						
			LabVIEW_info.Steer_Head_Item = HeadSteer_info->SteerPWM_Error[1];
			LabVIEW_info.Steer_Err_Item = LineError;
					
		#endif
	
	}
	else//正常赛道:K*摇头舵机的输出角度 	+ 	LineError
	{
		switch(Speed_info->SpeedMode)
		{
			case 1://使用CCD1
				{
					//限制摇头舵机传给转向舵机的角度
					HeadToSteer_Temp = Steer_info->HeadToSteer_1*HeadSteer_info->SteerPWM_Error[1];

					Steer_info->Steer_PWMOutputDuty = (uint32)(Steer_info->Steer_Center - HeadToSteer_Temp - LineError);
					
					//发送临时变量
					#ifdef DMA_CH2_SendLabVIEW_Info_Enable
									
						LabVIEW_info.Steer_Head_Item = HeadToSteer_Temp;
						LabVIEW_info.Steer_Err_Item = LineError;
					
					#endif
				}
				break;
			case 2://使用CCD2
				{
					//限制摇头舵机传给转向舵机的角度
					if(Parameter_info.Mode_HighSpeedDistance <= 0)
					{
						HeadToSteer_Temp = Steer_info->HeadToSteer_2*HeadSteer_info->SteerPWM_Error[1];
					}
					else
					{
						HeadToSteer_Temp = 0.8*Steer_info->HeadToSteer_2*HeadSteer_info->SteerPWM_Error[1];
					}
						
					Steer_info->Steer_PWMOutputDuty = (uint32)(Steer_info->Steer_Center - HeadToSteer_Temp - LineError);
				
					//发送临时变量
					#ifdef DMA_CH2_SendLabVIEW_Info_Enable
									
						LabVIEW_info.Steer_Head_Item = HeadToSteer_Temp;
						LabVIEW_info.Steer_Err_Item = LineError;
					
					#endif
				}	
				break;
			case 3://使用CCD3
				{
					if(Parameter_info.Mode_HighSpeedDistance <= 0)
					{
						HeadToSteer_Temp = Steer_info->HeadToSteer_3*HeadSteer_info->SteerPWM_Error[1];
					}
					else
					{
						HeadToSteer_Temp = 0.8*Steer_info->HeadToSteer_3*HeadSteer_info->SteerPWM_Error[1];
					}
					
					
					Steer_info->Steer_PWMOutputDuty = (uint32)(Steer_info->Steer_Center - HeadToSteer_Temp - LineError);
				
					//发送临时变量
					#ifdef DMA_CH2_SendLabVIEW_Info_Enable
									
						LabVIEW_info.Steer_Head_Item = Steer_info->HeadToSteer_3*HeadSteer_info->SteerPWM_Error[1];
						LabVIEW_info.Steer_Err_Item = LineError;
					
					#endif
				}
				break;
			default:
					break;
		}
	}
	
	//--------------------------转向舵机的最后总限幅---------------------------//
	if(Steer_info->Steer_PWMOutputDuty > Steer_info->Steer_LeftMAX)	//超过左极限
	{
	 	Steer_info->Steer_PWMOutputDuty = Steer_info->Steer_LeftMAX;
	}
	else if(Steer_info->Steer_PWMOutputDuty < Steer_info->Steer_RightMAX)//超过右极限
	{
	 	Steer_info->Steer_PWMOutputDuty = Steer_info->Steer_RightMAX; 
	}
	
	//移动转向舵机偏差队列
	if(1 == Steer_info->RampUp_SteerBackToCenter_Flag)
	{
		Steer_info->RampUp_SteerBackToCenter_Flag = 0;	//清零上坡回正标记
		
		Steer_info->SteerPWM_Error[9] = Steer_info->SteerPWM_Error[8];
		Steer_info->SteerPWM_Error[8] = Steer_info->SteerPWM_Error[7];
		Steer_info->SteerPWM_Error[7] = Steer_info->SteerPWM_Error[6];
		Steer_info->SteerPWM_Error[6] = Steer_info->SteerPWM_Error[5];
		Steer_info->SteerPWM_Error[5] = Steer_info->SteerPWM_Error[4];
		Steer_info->SteerPWM_Error[4] = Steer_info->SteerPWM_Error[3];
		Steer_info->SteerPWM_Error[3] = Steer_info->SteerPWM_Error[2];
		Steer_info->SteerPWM_Error[2] = Steer_info->SteerPWM_Error[1];
		Steer_info->SteerPWM_Error[1] = Steer_info->SteerPWM_Error[0];
	}
	
	//更新转向舵机偏差队列
	Steer_info->SteerPWM_Error[0] = Steer_info->Steer_PWMOutputDuty - Steer_info->Steer_Center;
	
	//转向舵机输出
	FTM2->CONTROLS[0].CnV = Steer_info->Steer_PWMOutputDuty;
}

/*************************************************************************
*					零		度		偏		移  
*
*  函数名称：mySteer_Init
*  功能说明：摇头舵机，转向舵机的初始化
*  参数说明：
*  函数返回：无
*  修改时间：2014-06-21
*  备    注：
*************************************************************************/
void mySteer_Init(void)
{
	PORTB->PCR[18]=PORT_PCR_MUX(3);				//B18:FTM2_CH0
	PORTB->PCR[19]=PORT_PCR_MUX(3);				//B19:FTM2_CH1
	
	SIM->SCGC3|=SIM_SCGC3_FTM2_MASK;			//使能时钟
			
	FTM2->MODE|=FTM_MODE_WPDIS_MASK;			//写保护禁止
			
	//--------------------CnSC通道状态控制----------------------------//
	//------------------边沿对齐PWM先高后低	
	FTM2->CONTROLS[0].CnSC |= FTM_CnSC_ELSB_MASK;			//左对齐，先高后低
	FTM2->CONTROLS[0].CnSC &=~FTM_CnSC_ELSA_MASK; 			//
	FTM2->CONTROLS[0].CnSC |= FTM_CnSC_MSB_MASK;			//模式选着边沿对齐模式
	
	FTM2->CONTROLS[1].CnSC |= FTM_CnSC_ELSB_MASK;			//左对齐，先高后低
	FTM2->CONTROLS[1].CnSC &=~FTM_CnSC_ELSA_MASK; 			//
	FTM2->CONTROLS[1].CnSC |= FTM_CnSC_MSB_MASK;			//模式选着边沿对齐模式

	//------------------FTM2状态控制-------------------------------
	//[7]TOF定时器溢出标志
	//[6]TOFIE定时器溢出中断使能
	//[5]CPWMS=0 计数器加法计数
	//[4:3]CLKS=01 system clock
	//[2:0]PS 预分频比=2^PS			
	FTM2->SC=0x0D;					//中断禁止,时钟源，分频4，递增计数

	//-------------------特性模式选着-----------------------------
  	FTM2->MODE &=~ 1;					//使能FTM2
			
	//-------------------输出屏蔽寄存器---------------------------
	FTM2->OUTMASK =~ (FTM_OUTINIT_CH0OI_MASK | FTM_OUTINIT_CH1OI_MASK);	//通道0,1输出，其它禁止
	FTM2->OUTINIT |=0x01;
	
	//-------------------正交解码状态控制-------------------------
	FTM2->QDCTRL &=~ FTM_QDCTRL_QUADEN_MASK;	//禁止正交解码模式
			
	//-------------------通道联合功能-----------------------------
	FTM2->COMBINE = 0 ;					//双边沿捕捉禁止，不级联
			
	FTM2->CNTIN = 0 ;					//计数器初始值 0
	FTM2->MOD = 15626;					//计数器终止值,即舵机控制周期设定(15625-200Hz)
	
	//-------------------计数值-------------------------------------
	FTM2->CONTROLS[0].CnV = 5000;		//高电平时间,初始化舵机时的占空比(5000/15625)
	FTM2->CONTROLS[1].CnV = 5000;
		
	FTM2->CNT = 0;						//计数器归0
}

/*------------------------------end of mySteer.c--------------------------*/
