/*
						
                         \\\|///
                       \\  - -  //
                        (  @ @  )
+---------------------oOOo-(_)-oOOo-------------------------+
|                                           				|
|             飞思卡尔智能车-光电组-零度偏移             |
|              												|
|                              Oooo                         |
+-----------------------oooO--(   )-------------------------+
                       (   )   ) /
                        \ (   (_/
                         \_)     

*/

#include "myIncludes.h"

	 Parameter_Info  Parameter_info = {0};
	 Road_Condition  Road_condition = NeedJudge;
	 Road_Condition  Road_condition_Item = NeedJudge;
	 Car_State  	 Car_state = Go_Forward;
	 Car_State 		 Car_state_Pre = Go_Forward;
	 
	 Car_Mode Car_mode = SelfDef;//初始化为安全速度
	 
	 Road_Type 		Road_type = All;//默认赛道种类为全有(即：坡道,人字和路障)
	 
	 menu Menu = {0};
	 
	 uint16_t StopType = 0;

	 extern CCD_Info CCD1_info;
     extern CCD_Info CCD2_info;
	 extern CCD_Info CCD3_info;
	 
	 extern Steer_Info Steer_info;
	 extern HeadSteer_Info HeadSteer_info;
	 
	 extern Motor_Info Motor_info;
	 extern Speed_Info Speed_info;
	 
	 extern Gyro_Info Gyro_info;
	    
	 extern DMA_Required_Variable DMA_Variable_1;
	 extern DMA_InitTypeDef dma_init_struct;
	 extern Flash_Info Flash_info;
	 extern RemSpeedUpDown_Info RemSpeedUpDown_info;
	 
void myMain_ParametersInit(Parameter_Info *Parameter_info);
void myMain_ChangeRoadType(Road_Type *Road_type,Parameter_Info *Parameter_info);
void myMain_TimeControl(Car_Mode *Car_mode,
							CCD_Info *CCD1_info,
							CCD_Info *CCD2_info,
							CCD_Info *CCD3_info,
							Speed_Info *Speed_info,
							Parameter_Info *Parameter_info,
							Car_State *Car_state,
							Car_State *Car_state_Pre
						   );

/*************************************************************************
*					零		度		偏		移  
*  函数名称: main
*  功能说明: main函数
*  参数说明:	
*            
*  函数返回:
*  修改时间:2014-06-23
*  备	注:
*************************************************************************/
int main(void)
{ 		
	LDPY_SysTickInit();	
	LDPY_ADC0_Init();
	LDPY_ADC1_Init();
	LDPY_PIT0_Init();	
	LDPY_Flash_Init();
	
	myLED_Init();
	myOLED_Init();
	myKey_Init();
	myCCD_GPIO_Init();
	mySteer_Init();
	myMotor_Init();
	myMotor_EncoderInit();
	
/*--------------------数据初始化------------------------*/
	myMain_ParametersInit(&Parameter_info);					//一些特殊参数初始化
	myCCD_DataInit(&CCD1_info);								//CCD1参数初始化
	myCCD_DataInit(&CCD2_info);								//CCD2参数初始化
	myCCD_DataInit(&CCD3_info);								//CCD3参数初始化
	mySteer_HeadSteerDataInit(&HeadSteer_info);				//摇头舵机参数初始化
	mySteer_DataInit(&Steer_info);							//转向舵机参数初始化
	myMotor_DataInit(&Motor_info);							//电机参数初始化
	myMotor_SpeedDataInit(&Speed_info);						//速度参数初始化
	myGyro_DataInit(&Gyro_info);							//陀螺仪参数初始化
	
	CCD1_info.InvalidPixel_Num = 20;
	
	#ifdef RemRoad_Control_Enable
	
		myRemRoad_DataInit(&RemSpeedUpDown_info);
		
	#endif
	
/*--------------------------------------------------------*/

	Bell_ON;
	LDPY_Delay_ms(150);
	Bell_OFF;
	
/*--------------------------上位机控制-----------------------------*/
#ifdef LabVIEW_Control_Enable

	LDPY_UART5_DisableIrq();				//关闭串口中断
	
	myLabVIEW_UART_Init(115200);			//上位机初始化
	Parameter_info.Parameter_OK = 0;
	do
	{
		myLabVIEW_ParametersInit(	&Parameter_info,
									&CCD1_info,
									&Steer_info,
									&Motor_info,
									&Speed_info
								);
	}while(0 == Parameter_info.Parameter_OK);			//等待初始化参数完成
	
	Parameter_info.Parameter_OK = 0;
	
	LDPY_UART5_EnableIrq();						//打开串口中断,接收调试参数
	
#endif
//end of LabVIEW_Control_Enable
/*-----------------------------------------------------------------*/
	
#ifdef DMA_Enable
	//DMA初始化
	LDPY_DMA_InitAndChoice(&dma_init_struct,&DMA_Variable_1,&CCD1_info);
#endif
//end of DMA_Enable
	
	//把初始化的参数写进Flash
	//write_Data2Flash(&Parameter_info,&CCD1_info,&CCD2_info,&HeadSteer_info,&Steer_info,&Speed_info,&Flash_info,&Motor_info);
	
	//从Flash读取参数
	read_DataFromFlash(&Parameter_info,&CCD1_info,&CCD2_info,&HeadSteer_info,&Steer_info,&Speed_info,&Flash_info,&Motor_info);
	
	while(lcd_menu_display_init(&Menu))
	{
		if(1 == Parameter_info.OLED_NoAction_Flag)//当按键没有反应的时候计时退出
		{
			if(Parameter_info.OLED_NoAction_Counter > 0)
			{
				Parameter_info.OLED_NoAction_Counter--;
			}
			else if(Parameter_info.OLED_NoAction_Counter <= 0)
			{
				break;//直接跳出OLED等待
			}
		}
	}

	//将参数写入Flash
	write_Data2Flash(&Parameter_info,&CCD1_info,&CCD2_info,&HeadSteer_info,&Steer_info,&Speed_info,&Flash_info,&Motor_info);


	//根据手动设定的赛道种类来开启相关的赛道类型识别(坡道，人字，路障)
	myMain_ChangeRoadType(&Road_type,&Parameter_info);
	
	#ifdef RemRoad_Control_Enable
	
		myRemRoad_DataSetting(&RemSpeedUpDown_info);
	
	#endif
	
	myCarControl_Ready_Go();				//发车延时
	
	if(1 == Parameter_info.GetStartEndLine_Flag)//需要检测起跑线时初始化起跑线检测
	{
		myKey_StartEndLineInit();				
	}
	
	LDPY_PIT0_us(3000);						//控制周期设定
	
	//起跑时的初次数据采集和处理
	myCCD_GetBeginLineError(	&CCD1_info,
								&CCD2_info,
								&CCD3_info,
								&Speed_info,
								&Parameter_info
							);

	while(1)
	{	
		if(1 == Parameter_info.CCD_SampleOK_Flag)			//数据采集完成
		{
			Parameter_info.CCD_SampleOK_Flag = 0;			//清除数据采集完成标志
			
			myCarControl_CarControl(&CCD1_info,
									&CCD2_info,
									&CCD3_info,
									&HeadSteer_info,
									&Steer_info,
									&Motor_info,
									&Speed_info,
									&Parameter_info,
									&RemSpeedUpDown_info,
									&Road_condition,
									&Car_state,
									&Car_state_Pre
									);	//(耗时500us)

			//----------------------------时间控制----------------------------//
			if(Stop != Car_state)//计算行驶的时间
			{
				myMain_TimeControl(	&Car_mode,
									&CCD1_info,
									&CCD2_info,
									&CCD3_info,
									&Speed_info,
									&Parameter_info,
									&Car_state,
									&Car_state_Pre
								  );
			}
		}//end of if(1 == Parameter_info.CCD_SampleOK_Flag)
	}//end of while(1)	
}

/*************************************************************************
*					零		度		偏		移  
*  函数名称: myMain_ParameterInit
*  功能说明: 一些特殊参数初始化函数
*  参数说明:	
*            
*  函数返回:
*  修改时间:2014-06-21
*  备	注:
*************************************************************************/
void myMain_ParametersInit(Parameter_Info *Parameter_info)
{
	uint8_t ii = 0;
	
	Parameter_info->Parameter_OK = 0;
	Parameter_info->UART_RecvData = 0;
	
	for(ii = 0; ii < 10; ii++)
	{
		Parameter_info->StartEndLine_Fifo[ii][0] = 0;		//记录红外管标号
		Parameter_info->StartEndLine_Fifo[ii][1] = 0;		//记录中断时间
	}
	
	Parameter_info->AD_MultK_f = 1.0;
	Parameter_info->CCD1_AD_TargetValue = 1400;
	Parameter_info->CCD1_Binary_MAX = 1000;
	Parameter_info->CCD1_Binary_MIN = 700;
	
	
	Parameter_info->GetTurnRound_Enable = TRUE;		//默认识别人字道
	Parameter_info->GetRamp_Enable = TRUE;			//默认检测坡道
	Parameter_info->GetStartEndLine_Enable = TRUE;	//默认检测起跑线
	Parameter_info->GetObstacle_Enable = TRUE;		//默认检测路障
	Parameter_info->RoadInvalid_Enable = TRUE;		//默认检测道路是否有效，防止窜道
	Parameter_info->GetStartEndLine_Flag = 1;		//默认检测起跑线,标记1
	Parameter_info->GetStartEndLine_SameLight_Enable = FALSE;//起跑线检测同侧红外管使能
	Parameter_info->CCD1GetSL_Enable = FALSE;		//CCD1识别起跑线使能
	Parameter_info->WheelOutOfWay_Enable = FALSE;	
	
	Parameter_info->Mode_HighSpeedDistance = 400.0;			//默认400cm的高速距离
	Parameter_info->OLED_NoAction_Counter = 20000;
	
	Parameter_info->RampJgeInAdvce_Num = 0;
	Parameter_info->TurnRound_OK_Num = 0;
	Parameter_info->TurnRound_Num_MAX = 3;
	Parameter_info->Obstacle_OK_Num = 0;
	
	Parameter_info->Angle60_MeanValue = 10;
	
	Parameter_info->Time = 0;	
	Parameter_info->GetStartEndLineTime = 5;			//默认5秒内不检测起跑线
	Parameter_info->GetTurnRoundTime = 2;				//默认2秒内不检测人字道
	Parameter_info->GetRampTime = 2;					//默认2秒内不检测坡道
	Parameter_info->GetObstacleTime = 2;				//默认2秒内不检测路障
	Parameter_info->DebugTime = 0;						//默认调试 0 秒,即比赛模式，不定时
	Parameter_info->LoseLineTime = 0;				
	Parameter_info->RoadDistance = 0;		
	Parameter_info->StopSpeed = 0;
	
	Parameter_info->CCD_SampleOK_Flag = 0;
	Parameter_info->StartEndLine_Flag = 0;
	
	Parameter_info->LeftIntoTurnRound_Flag = 0;
	Parameter_info->LeftIntoTurnRound_Flag = 0;
	Parameter_info->CCD2OrCCD3_GetTurnRound = 0;
	
	Parameter_info->LeftOrRightObstacle_Flag = 0;
	
	Parameter_info->OLED_NoAction_Flag = 1;		
	Parameter_info->RampReady_Distance = 0;
	Parameter_info->RampReady_FLag = 0;
	Parameter_info->SlowBeforeTurnRound_Flag = 0;	//默认全程高速，此标记置1则在识别到人字之前使用低速模式
	Parameter_info->LongStraight_Flag = 0;			//长直道标记
	Parameter_info->CCD1_GetedStartEndLine_Flag = 0;
	Parameter_info->Left60Angle_Flag = 0;
	Parameter_info->Right60Angle_Flag = 0;
}

/*************************************************************************
*					零		度		偏		移  
*  函数名称: myMain_SetCarMode
*  功能说明: 设置比赛时的小车模式（高速，安全速度，低速）
*  参数说明:	
*            
*  函数返回:
*  修改时间:2014-07-12
*  备	注:
*************************************************************************/
void myMain_SetCarMode(	Car_Mode *Car_mode,
						HeadSteer_Info *HeadSteer_info,
						Steer_Info *Steer_info,
						Motor_Info *Motor_info,
						Speed_Info *Speed_info
					  )
{
	switch(*Car_mode)//选择速度模式
	{
		case High2://最高速
			{
				
			}
			break;
		case High1:
			{
				
			}
			break;
		case Safe:
			{
				
			}
			break;
		case Low1:
			{
				
			}
			break;
		case Low2:
			{
				
			}
			break;
		default:
			break;
	}
}

/*************************************************************************
*					零		度		偏		移  
*  函数名称: myMain_ChangeRoadType
*  功能说明: 改变赛道种类
*  参数说明:	
*            
*  函数返回:
*  修改时间:2014-07-08
*  备	注:
*************************************************************************/
void myMain_ChangeRoadType(Road_Type *Road_type,Parameter_Info *Parameter_info)
{
	switch(*Road_type)
	{
		case None://全竞速，没有坡道，人字和路障(预赛可能用上)
			Parameter_info->GetRamp_Enable = FALSE;
			Parameter_info->GetTurnRound_Enable = FALSE;
			Parameter_info->GetObstacle_Enable = FALSE;
			break;
		case Ramp_Only://只有坡道(预赛可能用上)
			Parameter_info->GetRamp_Enable = FALSE;
			Parameter_info->GetTurnRound_Enable = FALSE;
			Parameter_info->GetObstacle_Enable = TRUE;
			break;
		case All://全都有
			Parameter_info->GetRamp_Enable = TRUE;
			Parameter_info->GetTurnRound_Enable = TRUE;
			Parameter_info->GetObstacle_Enable = TRUE;
			break;
		default:
			break;
	}
}

/*************************************************************************
*					零		度		偏		移  
*  函数名称: myMain_TimeControl
*  功能说明: 各种时间相关的控制
*  参数说明:	
*            
*  函数返回:无
*  修改时间:2014-07-22
*  备	注:此函数每3ms调用一次，故可用作时间控制函数
*************************************************************************/
void myMain_TimeControl(	Car_Mode *Car_mode,
							CCD_Info *CCD1_info,
							CCD_Info *CCD2_info,
							CCD_Info *CCD3_info,
							Speed_Info *Speed_info,
							Parameter_Info *Parameter_info,
							Car_State *Car_state,
							Car_State *Car_state_Pre
					   )
{
	//计算行驶的时间
	Parameter_info->Time += 0.003;
	
	#ifndef DMA_CH0_Enable
		//CCD1连续丢线时进行丢线保护
		if(    (1 == CCD1_info->LossLine_Flag)
			&& (1 == CCD2_info->LossLine_Flag)
			&& (0 == CCD1_info->Cross_Flag)
			&& (Ramp_Up != Road_condition)
			&& (Ramp_Down != Road_condition)
			&& (Turn_Round != *Car_state)
			&& (CheckCCD != *Car_mode))
		{
			if(Parameter_info->LoseLineTime < 1)//CCD1连续丢线1秒则停车保护
			{
				Parameter_info->LoseLineTime += 0.003;
			}
			else
			{
				*Car_state = Stop;
				
				Parameter_info->StartEndLine_Flag = 1;
				
				StopType = 9;
			}
		}
		else
		{
			Parameter_info->LoseLineTime = 0;//一旦CCD1再次寻到边界则清零丢线计时
		}
	#endif
	
	//------------------------------调试控制模式---------------------------//
	//若调试时间为零则进入正常行驶状态，不进行调试。检查CCD时不计时。
	if(    (CheckCCD != *Car_mode)
		&& (0 != Parameter_info->DebugTime) 
		&& (Parameter_info->Time >= Parameter_info->DebugTime))//调试时间到,停车
	{
		if(Stop != *Car_state)
		{
			*Car_state_Pre = *Car_state;
			*Car_state = Stop;
			
			Parameter_info->StopSpeed = Speed_info->Speed_Now;
			
			Parameter_info->StartEndLine_Flag = 1;
			
			StopType = 8;
		}
	}
	else if(CheckCCD == *Car_mode)//检查CCD模式
	{
		myOLED_Dec4(7,10,CCD3_info->LeftLine[0]);
		myOLED_Dec4(7,50,CCD3_info->CentralLine[0]);
		myOLED_Dec4(7,90,CCD3_info->RightLine[0]);
		
		myOLED_Dec4(6,10,CCD2_info->LeftLine[0]);
		myOLED_Dec4(6,50,CCD2_info->CentralLine[0]);
		myOLED_Dec4(6,90,CCD2_info->RightLine[0]);
		
		myOLED_Dec4(5,10,CCD1_info->LeftLine[0]);
		myOLED_Dec4(5,50,CCD1_info->CentralLine[0]);
		myOLED_Dec4(5,90,CCD1_info->RightLine[0]);
		
		myOLED_Dec4(4,10,Speed_info->SpeedFifo[0]);				//用于显示编码器是否正常工作
		myOLED_Decimals(4,50,Parameter_info->AD_MultK_f);		//用于显示CCD的放大倍数
		
		myOLED_CCDwave(CCD1_info,CCD2_info,CCD3_info);
		
		//处于调试CCD模式时电机恒定占空比
		FTM0->CONTROLS[6].CnV = 2000;
		FTM0->CONTROLS[7].CnV = 0;
	}
	else if(CheckRamp == *Car_mode)
	{
		//显示当前路况（是否为坡道，4 - 上坡，5 - 下坡）
		myOLED_String(6,1,"RoadCndition");
		myOLED_Dec(6,90,Road_condition);
		
		//显示坡道距离记录
		myOLED_String(5,1,"RampDistance");
		myOLED_Dec4(5,90,Speed_info->DistanceOfRampUp_Counter);
		
		//显示上坡是否预判成功
		myOLED_String(4,1,"RampUp_YuPan");
		myOLED_Dec(4,90,Parameter_info->RampReady_FLag);
		
		//显示下坡是否预判成功
		myOLED_String(3,1,"RampDn_YuPan");
		myOLED_Dec(3,90,myCCD_JudgeRampDown(CCD1_info,CCD2_info,CCD3_info,&Road_condition));
		
		//处于调试坡道模式时关闭电机
		FTM0->CONTROLS[6].CnV = 0;
		FTM0->CONTROLS[7].CnV = 0;
	}
}

/*------------------------end of myMain.c---------------------------------*/		
