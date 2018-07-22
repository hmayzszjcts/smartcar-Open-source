
#ifndef __MYMOTOR_H__
#define __MYMOTOR_H__

#include "myOLED.h"
#include "myStruct.h"
#include "myCCD.h"

#define MOTOR_PWM_OUTPUT_MAX 	(9950)
#define MOTOR_PWM_OUTPUT_MIN 	(-9950)

#define MOTOR_PWM_OUTPUT_DUTY 	(2000)	//电机恒定占空比大小

void myMotor_Init(void);
void myMotor_EncoderInit(void);
void myMotor_DataInit(Motor_Info *Motor_info);
void myMotor_SpeedDataInit(Speed_Info *Speed_info);

void myMotor_UpdateEncoder(	CCD_Info *CCD1_info,
							CCD_Info *CCD2_info,
							Motor_Info *Motor_info,
							Speed_Info *Speed_info,
							Parameter_Info *Parameter_info,
							RemSpeedUpDown_Info *RemSpeedUpDown_info,
							Road_Condition *Road_condtion,
							Car_State *Car_state,
							Car_State *Car_state_Pre
						  );

void myMotor_CalculateTargetSpeed(	CCD_Info *CCD1_info,
									CCD_Info *CCD2_info,
									CCD_Info *CCD3_info,
									HeadSteer_Info *HeadSteer_info,
									Steer_Info *Steer_info,
									Speed_Info *Speed_info
								 );

int32 myMotor_PIDCalculate(	Speed_Info *Speed_info,
							Motor_Info *Motor_info,
							Parameter_Info * Parameter_info
						  );

void myMotor_Control(	Motor_Info *Motor_info,
						Speed_Info *Speed_info,
						Parameter_Info * Parameter_info		
					);

#endif

/*---------------------------------end of myMotor.h------------------------------*/
