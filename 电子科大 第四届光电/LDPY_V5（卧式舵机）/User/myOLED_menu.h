

#ifndef __MYOLED_menu_H__
#define __MYOLED_menu_H__

	#include "MK60DZ10.h"
	#include "myRemRoad.h"
	#include "myStruct.h"
	#include "myOLED.h"
	
	#define   zhi  Speed_info.Straight_Speed  
    #define   wan  Speed_info.Cur_Speed
	#define   x_s  Speed_info.Snake_Speed
	#define   Up   Speed_info.RampUp_Speed  			
	#define   Dn   Speed_info.RampDown_Speed   			
	#define   Ren  Speed_info.TurnRound_Speed
	
	#define   Agle Speed_info.Angle60_Speed
	#define   LuZ  Speed_info.Obstacle_Speed
	#define   Add3 Speed_info.SpeedAdd_3
	#define   S_K  Speed_info.HeadSteerPWMError_K
	#define   E_K  Speed_info.Error_K
	#define   D_K  Speed_info.HeadSteerPWMError_D_K
	
	#define   PH   HeadSteer_info.KPH
	#define   IH   HeadSteer_info.KIH
	
	#define   P1   Steer_info.KP1
	#define   P2   Steer_info.KP2
	#define   P3   Steer_info.KP3
	
	#define   H1   Steer_info.HeadToSteer_1
	#define   H2   Steer_info.HeadToSteer_2
	#define   H3   Steer_info.HeadToSteer_3
	
	#define   VPT  Motor_info.KPT
	#define   VP   Motor_info.KP
	#define   VD   Motor_info.KD
	#define   VI   Motor_info.KI
	
	#define   D_T  Parameter_info.DebugTime
	#define   Po_T Parameter_info.GetRampTime
	#define   Ren_T Parameter_info.GetTurnRoundTime
	#define   LuZ_T  Parameter_info.GetObstacleTime
	#define	  S_T	Parameter_info.GetStartEndLineTime
	
	#define   jl_3  Speed_info.DistanceOfSpeedMode3_Max
	#define   R_l  Speed_info.DistanceToTurn_MIN
	#define   R_L  Speed_info.DistanceToTurn_MAX
	#define   Po_S1	Speed_info.DistanceOfRampUp[0]
	#define   Po_S2	Speed_info.DistanceOfRampUp[1]
	#define   Po_X	Speed_info.DistanceOfRampDown
	
	#define   R_tp  Road_type
	#define   Ramp  Gyro_info.RampThresholdValue
	#define   Wu_x	Parameter_info.RoadInvalid_Enable
	#define   LSBR  Parameter_info.SlowBeforeTurnRound_Flag
	#define	  S_L	Parameter_info.GetStartEndLine_Flag
	#define   CCDL  Parameter_info.CCD1GetSL_Enable
 
#ifdef RemRoad_Control_Enable
 
	#define   S_Z1	RemSpeedUpDown_info.Cross_Num[0]
	#define   S_Z2	RemSpeedUpDown_info.Cross_Num[1]
	#define	  SQ1   RemSpeedUpDown_info.CrossRemDistance[0]
	#define	  SG1   RemSpeedUpDown_info.CrossRemDistance[1]
	#define	  SQ2   RemSpeedUpDown_info.CrossRemDistance[2]
	#define	  SG2   RemSpeedUpDown_info.CrossRemDistance[3]
	
	#define   R_Z1	RemSpeedUpDown_info.TurnRound_Num[0]
	#define   R_Z2	RemSpeedUpDown_info.TurnRound_Num[1]
	#define	  RQ1   RemSpeedUpDown_info.TurnRoundRemDistance[0]
	#define	  RG1   RemSpeedUpDown_info.TurnRoundRemDistance[1]
	#define	  RQ2   RemSpeedUpDown_info.TurnRoundRemDistance[2]
	#define	  RG2   RemSpeedUpDown_info.TurnRoundRemDistance[3]
	
	#define   P_D1	RemSpeedUpDown_info.Ramp_Num[0]
	#define   P_D2	RemSpeedUpDown_info.Ramp_Num[1]
	#define	  PQ1   RemSpeedUpDown_info.RampRemDistance[0]
	#define	  PG1   RemSpeedUpDown_info.RampRemDistance[1]
	#define	  PQ2   RemSpeedUpDown_info.RampRemDistance[2]
	#define	  PG2   RemSpeedUpDown_info.RampRemDistance[3]
	
	#define   Jiyi  RemSpeedUpDown_info.RemRoad_Enable
	#define   F_C   Parameter_info.Angle60_MeanValue
	#define   C_J	Parameter_info.WheelOutOfWay_Enable
	#define   P_Y   CCD2_info.CCD_ObstacleShift
	#define   RNum	Parameter_info.TurnRound_Num_MAX
	
	#define   B_T	Parameter_info.CCD1_AD_TargetValue
	#define   b_m	Parameter_info.CCD1_Binary_MIN
	#define   B_M	Parameter_info.CCD1_Binary_MAX
	#define   Sght	Parameter_info.Mode_HighSpeedDistance
	
#endif
	
	uint8_t   lcd_menu_display_init(menu * Menu);

#endif


