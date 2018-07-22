#include "myFlashReadWrite.h"

Flash_Info Flash_info = {0};
extern Road_Type Road_type;
extern Gyro_Info Gyro_info;

#ifdef RemRoad_Control_Enable

	extern RemSpeedUpDown_Info RemSpeedUpDown_info;

#endif

/*************************************************************************
*					零		度		偏		移                           
*
*  函数名称:write_DataT2Flash
*  功能说明:给FLASH里边写入数据
*  参数说明:
*  函数返回:无
*  修改时间:2014-07-22
*  备    注：
*************************************************************************/
void write_Data2Flash(Parameter_Info * Parameter_info,
                      CCD_Info * CCD1_info,
                      CCD_Info * CCD2_info,
					  HeadSteer_Info *HeadSteer_info,
                      Steer_Info * Steer_info,
                      Speed_Info * Speed_info,
                      Flash_Info * Flash_info,
                      Motor_Info * Motor_info
                      )

{
	LDPY_Flash_SectorErase(FLASH_ARRAY_ADDR_1);  		
	LDPY_Flash_SectorErase(FLASH_ARRAY_ADDR_2);
	LDPY_Flash_SectorErase(FLASH_ARRAY_ADDR_3);

	/*---------Flash_Array_1数组的填装----------*/
	Flash_info->Flash_Array_1[0] = Motor_info->KPT;	
	Flash_info->Flash_Array_1[1] = Speed_info->SpeedAdd_3;
	Flash_info->Flash_Array_1[2] = Speed_info->DistanceToTurn_MAX;
	Flash_info->Flash_Array_1[3] = Speed_info->DistanceToTurn_MIN;
	Flash_info->Flash_Array_1[4] = Gyro_info.RampThresholdValue;
	Flash_info->Flash_Array_1[5] = Parameter_info->RoadInvalid_Enable;
	Flash_info->Flash_Array_1[6] = Steer_info->HeadToSteer_1;
	Flash_info->Flash_Array_1[7] = Steer_info->HeadToSteer_2;
	Flash_info->Flash_Array_1[8] = Parameter_info->SlowBeforeTurnRound_Flag;
	Flash_info->Flash_Array_1[9] = Speed_info->DistanceOfRampUp[0];	Flash_info->Flash_Array_1[10] = Parameter_info->GetStartEndLineTime; 
	Flash_info->Flash_Array_1[11] = Steer_info->HeadToSteer_3;
	Flash_info->Flash_Array_1[12] = Steer_info->KP3;
	Flash_info->Flash_Array_1[13] = Speed_info->DistanceOfRampUp[1];
	Flash_info->Flash_Array_1[14] = CCD2_info->CCD_ObstacleShift;
	Flash_info->Flash_Array_1[15] = Parameter_info->TurnRound_Num_MAX;
	
	Flash_info->Flash_Array_1[16] = Parameter_info->CCD1_AD_TargetValue;
	Flash_info->Flash_Array_1[17] = Parameter_info->CCD1_Binary_MAX;
	Flash_info->Flash_Array_1[18] = Parameter_info->CCD1_Binary_MIN;

	/*---------Flash_Array_2数组的填装----------*/
	Flash_info->Flash_Array_2[0] = HeadSteer_info->KPH;	
	Flash_info->Flash_Array_2[1] = HeadSteer_info->KIH;
	Flash_info->Flash_Array_2[2] = Motor_info->KP; 
	Flash_info->Flash_Array_2[3] = Motor_info->KI; 
	Flash_info->Flash_Array_2[4] = Motor_info->KD; 
	Flash_info->Flash_Array_2[5] = Speed_info->DistanceOfRampDown;
	Flash_info->Flash_Array_2[6] = Steer_info->KP1;	
	Flash_info->Flash_Array_2[7] = Steer_info->KP2;
	Flash_info->Flash_Array_2[8] = Speed_info->RampUp_Speed;	
	Flash_info->Flash_Array_2[9] = Speed_info->RampDown_Speed;  
	Flash_info->Flash_Array_2[10] = Parameter_info->GetRampTime;
	Flash_info->Flash_Array_2[11] = Parameter_info->GetTurnRoundTime;
	Flash_info->Flash_Array_2[12] = Parameter_info->GetObstacleTime;
	Flash_info->Flash_Array_2[13] = Parameter_info->CCD1GetSL_Enable;
	Flash_info->Flash_Array_2[14] = Parameter_info->Mode_HighSpeedDistance;

	/*------------Flash_Array_3数组的填装------------*/  
	Flash_info->Flash_Array_3[0] = Speed_info->HeadSteerPWMError_K; 
	Flash_info->Flash_Array_3[1] = Speed_info->HeadSteerPWMError_D_K; 
	Flash_info->Flash_Array_3[2] = Speed_info->Straight_Speed;  
	Flash_info->Flash_Array_3[3] = Speed_info->Cur_Speed; //弯道速度 
	Flash_info->Flash_Array_3[4] = Speed_info->Snake_Speed;  
	Flash_info->Flash_Array_3[5] = Speed_info->TurnRound_Speed;  
	Flash_info->Flash_Array_3[6] = Speed_info->Obstacle_Speed;
	Flash_info->Flash_Array_3[7] = Speed_info->DistanceOfSpeedMode3_Max;//使用3档的最长距离 
	Flash_info->Flash_Array_3[8] = (uint8_t)Road_type;  
	Flash_info->Flash_Array_3[9] = Parameter_info->GetStartEndLine_Flag;
	Flash_info->Flash_Array_3[10] = Speed_info->Angle60_Speed;
	Flash_info->Flash_Array_3[11] = Speed_info->Error_K;
	Flash_info->Flash_Array_3[12] = Speed_info->Error_D_K;
	
	Flash_info->Flash_Array_3[13] = RemSpeedUpDown_info.CrossRemDistance[0];
	Flash_info->Flash_Array_3[14] = RemSpeedUpDown_info.CrossRemDistance[1];
	Flash_info->Flash_Array_3[15] = RemSpeedUpDown_info.CrossRemDistance[2];
	Flash_info->Flash_Array_3[16] = RemSpeedUpDown_info.CrossRemDistance[3];
	
	Flash_info->Flash_Array_3[17] = RemSpeedUpDown_info.TurnRoundRemDistance[0];
	Flash_info->Flash_Array_3[18] = RemSpeedUpDown_info.TurnRoundRemDistance[1];
	Flash_info->Flash_Array_3[19] = RemSpeedUpDown_info.TurnRoundRemDistance[2];
	Flash_info->Flash_Array_3[20] = RemSpeedUpDown_info.TurnRoundRemDistance[3];
	
	Flash_info->Flash_Array_3[21] = RemSpeedUpDown_info.RemRoad_Enable;
	Flash_info->Flash_Array_3[22] = Parameter_info->Angle60_MeanValue;
	Flash_info->Flash_Array_3[23] = Parameter_info->WheelOutOfWay_Enable;
	Flash_info->Flash_Array_3[24] = RemSpeedUpDown_info.Cross_Num[0];
	Flash_info->Flash_Array_3[25] = RemSpeedUpDown_info.Cross_Num[1];
	Flash_info->Flash_Array_3[26] = RemSpeedUpDown_info.TurnRound_Num[0];
	Flash_info->Flash_Array_3[27] = RemSpeedUpDown_info.TurnRound_Num[1];
	Flash_info->Flash_Array_3[28] = RemSpeedUpDown_info.Ramp_Num[0];
	Flash_info->Flash_Array_3[29] = RemSpeedUpDown_info.Ramp_Num[1];
	
	//参数写入Flash
	LDPY_Flash_ByteProgram((uint32)(FLASH_ARRAY_ADDR_1),(uint32*)(Flash_info->Flash_Array_1),FLASH_ARRAY_SIZE);
	LDPY_Flash_ByteProgram((uint32)(FLASH_ARRAY_ADDR_2),(uint32*)(Flash_info->Flash_Array_2),FLASH_ARRAY_SIZE);
	LDPY_Flash_ByteProgram((uint32)(FLASH_ARRAY_ADDR_3),(uint32*)(Flash_info->Flash_Array_3),FLASH_ARRAY_SIZE);
}

/*************************************************************************
*					零		度		偏		移                            
*
*  函数名称:read_DataFromFlash
*  功能说明:从FLASH中读取数据
*  参数说明:
*  函数返回:无
*  修改时间:2014-07-22
*  备    注：
*************************************************************************/
void read_DataFromFlash(Parameter_Info * Parameter_info,
						CCD_Info * CCD1_info,
						CCD_Info * CCD2_info,
						HeadSteer_Info *HeadSteer_info,
						Steer_Info * Steer_info,
						Speed_Info * Speed_info,
						Flash_Info * Flash_info,
						Motor_Info * Motor_info
						)
{
	uint8 ii;
	
	float *ptr_1 = (float*)FLASH_ARRAY_ADDR_1;   //将起始地址指针变量赋给ptr_1
	float *ptr_2 = (float*)FLASH_ARRAY_ADDR_2;   //将起始地址指针变量赋给ptr_2 
	float *ptr_3 = (float*)FLASH_ARRAY_ADDR_3;   //将起始地址指针变量赋给ptr_3
  
	for(ii = 0; ii < 100; ii++)                    //float型的数据每次加 1 ，地址加 4 位
	{
		*((float*)Flash_info->Flash_Array_1 + ii) = *(ptr_1 + ii);    
		*((float*)Flash_info->Flash_Array_2 + ii) = *(ptr_2 + ii);
		*((float*)Flash_info->Flash_Array_3 + ii) = *(ptr_3 + ii);
	}
  
	/*---------Flash_Array_1数组的读取----------*/
	Motor_info->KPT = Flash_info->Flash_Array_1[0];	
	Speed_info->SpeedAdd_3 = (int16_t)Flash_info->Flash_Array_1[1];	
	Speed_info->DistanceToTurn_MAX = (int16_t)Flash_info->Flash_Array_1[2];
	Speed_info->DistanceToTurn_MIN = (int16_t)Flash_info->Flash_Array_1[3];
	Gyro_info.RampThresholdValue = (int16_t)Flash_info->Flash_Array_1[4];
	Parameter_info->RoadInvalid_Enable = (uint8_t)Flash_info->Flash_Array_1[5];
	Steer_info->HeadToSteer_1 = Flash_info->Flash_Array_1[6];
	Steer_info->HeadToSteer_2 = Flash_info->Flash_Array_1[7];
	Parameter_info->SlowBeforeTurnRound_Flag = Flash_info->Flash_Array_1[8];
	Speed_info->DistanceOfRampUp[0] = (int16_t)Flash_info->Flash_Array_1[9];
	Parameter_info->GetStartEndLineTime = Flash_info->Flash_Array_1[10];
	Steer_info->HeadToSteer_3 = Flash_info->Flash_Array_1[11];
	Steer_info->KP3 = Flash_info->Flash_Array_1[12];
	Speed_info->DistanceOfRampUp[1] = (int16_t)Flash_info->Flash_Array_1[13];
	CCD2_info->CCD_ObstacleShift = (int16_t)Flash_info->Flash_Array_1[14];
	Parameter_info->TurnRound_Num_MAX = (int16_t)Flash_info->Flash_Array_1[15];
	
	Parameter_info->CCD1_AD_TargetValue = Flash_info->Flash_Array_1[16];
	Parameter_info->CCD1_Binary_MAX = (int16_t)Flash_info->Flash_Array_1[17];
	Parameter_info->CCD1_Binary_MIN = (int16_t)Flash_info->Flash_Array_1[18];

	/*---------Flash_Array_2数组的读取----------*/
	HeadSteer_info->KPH = (float)Flash_info->Flash_Array_2[0];	
	HeadSteer_info->KIH = (float)Flash_info->Flash_Array_2[1];
	Motor_info->KP = (float)Flash_info->Flash_Array_2[2];  
	Motor_info->KI = (float)Flash_info->Flash_Array_2[3];  
	Motor_info->KD = (float)Flash_info->Flash_Array_2[4]; 
	Speed_info->DistanceOfRampDown = (int16_t)Flash_info->Flash_Array_2[5];
	Steer_info->KP1 = (float)Flash_info->Flash_Array_2[6];	
	Steer_info->KP2 = (float)Flash_info->Flash_Array_2[7];
	Speed_info->RampUp_Speed = Flash_info->Flash_Array_2[8];	
	Speed_info->RampDown_Speed = Flash_info->Flash_Array_2[9];
	Parameter_info->GetRampTime = Flash_info->Flash_Array_2[10];
	Parameter_info->GetTurnRoundTime = Flash_info->Flash_Array_2[11];
	Parameter_info->GetObstacleTime = Flash_info->Flash_Array_2[12];
	Parameter_info->CCD1GetSL_Enable = (uint8_t)Flash_info->Flash_Array_2[13];
	Parameter_info->Mode_HighSpeedDistance = (int16_t)Flash_info->Flash_Array_2[14];

	/*------------Flash_Array_3数组的读取------------*/ 
	Speed_info->HeadSteerPWMError_K = Flash_info->Flash_Array_3[0];
	Speed_info->HeadSteerPWMError_D_K = Flash_info->Flash_Array_3[1]; 
	Speed_info->Straight_Speed = (int16_t)Flash_info->Flash_Array_3[2]; 
	Speed_info->Cur_Speed = (int16_t)Flash_info->Flash_Array_3[3]; 
	Speed_info->Snake_Speed = (int16_t)Flash_info->Flash_Array_3[4];  
	Speed_info->TurnRound_Speed = (int16_t)Flash_info->Flash_Array_3[5]; 
	Speed_info->Obstacle_Speed = (int16_t)Flash_info->Flash_Array_3[6];
	Speed_info->DistanceOfSpeedMode3_Max = (int16_t)Flash_info->Flash_Array_3[7];
	Road_type = (Road_Type)Flash_info->Flash_Array_3[8];
	Parameter_info->GetStartEndLine_Flag = (uint8_t)Flash_info->Flash_Array_3[9];
	Speed_info->Angle60_Speed = (int16_t )Flash_info->Flash_Array_3[10];
	Speed_info->Error_K = (int16_t)Flash_info->Flash_Array_3[11];
	Speed_info->Error_D_K = (int16_t)Flash_info->Flash_Array_3[12];
	
	RemSpeedUpDown_info.CrossRemDistance[0] = Flash_info->Flash_Array_3[13];
	RemSpeedUpDown_info.CrossRemDistance[1] = Flash_info->Flash_Array_3[14];
	RemSpeedUpDown_info.CrossRemDistance[2] = Flash_info->Flash_Array_3[15];
	RemSpeedUpDown_info.CrossRemDistance[3] = Flash_info->Flash_Array_3[16];
	
	RemSpeedUpDown_info.TurnRoundRemDistance[0] = Flash_info->Flash_Array_3[17];
	RemSpeedUpDown_info.TurnRoundRemDistance[1] = Flash_info->Flash_Array_3[18];
	RemSpeedUpDown_info.TurnRoundRemDistance[2] = Flash_info->Flash_Array_3[19];
	RemSpeedUpDown_info.TurnRoundRemDistance[3] = Flash_info->Flash_Array_3[20];
	
	RemSpeedUpDown_info.RemRoad_Enable = (uint8_t)Flash_info->Flash_Array_3[21];
	Parameter_info->Angle60_MeanValue = (int16_t)Flash_info->Flash_Array_3[22];
	Parameter_info->WheelOutOfWay_Enable = (uint8_t)Flash_info->Flash_Array_3[23];
	
	RemSpeedUpDown_info.Cross_Num[0] = (uint8_t)Flash_info->Flash_Array_3[24];
	RemSpeedUpDown_info.Cross_Num[1] = (uint8_t)Flash_info->Flash_Array_3[25];
	RemSpeedUpDown_info.TurnRound_Num[0] = (uint8_t)Flash_info->Flash_Array_3[26];
	RemSpeedUpDown_info.TurnRound_Num[1] = (uint8_t)Flash_info->Flash_Array_3[27];
	RemSpeedUpDown_info.Ramp_Num[0] = (uint8_t)Flash_info->Flash_Array_3[28];
	RemSpeedUpDown_info.Ramp_Num[1] = (uint8_t)Flash_info->Flash_Array_3[29];
	
}
