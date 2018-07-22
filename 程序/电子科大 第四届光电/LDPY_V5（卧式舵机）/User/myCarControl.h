

#ifndef __MYCARCONTROL_H__
#define __MYCARCONTROL_H__

#include "myIncludes.h"

void myCarControl_Ready_Go(void);
void myCarControl_CalDistanceToTurnRound(Speed_Info *Speed_info);
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
							);

#endif

/*-----------------------end of myControl.h---------------------------------*/
