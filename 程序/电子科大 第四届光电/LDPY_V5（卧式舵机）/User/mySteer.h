
#ifndef __MYSTEER_H__
#define __MYSTEER_H__

#include "myStruct.h"
#include "myOLED.h"
#include "myLED.h"
#include "myCCD.h"

#define 	HeadSteer_LeftMAX2	(4340)
#define		HeadSteer_RightMAX2	(5800)

#define 	HeadSteer_LeftMAX3	(4340)
#define		HeadSteer_RightMAX3	(5800)

void mySteer_Init(void);

void mySteer_DataInit(Steer_Info *Steer_info);
void mySteer_HeadSteerDataInit(HeadSteer_Info *HeadSteer_info);

void mySteer_HeadSteerControl(	CCD_Info *CCD1_info,
								CCD_Info *CCD2_info,
								CCD_Info *CCD3_info,
								HeadSteer_Info *HeadSteer_info,
								Speed_Info *Speed_info,
								Car_State *Car_state,
								Car_State *Car_state_Pre
							);
void mySteer_DirectionControl(CCD_Info *CCD1_info,CCD_Info *CCD2_info,CCD_Info *CCD3_info,HeadSteer_Info *HeadSteer_info,Steer_Info *Steer_info,Speed_Info *Speed_info);

#endif

/*---------------------------------end of mySteer.h-------------------------------*/
