
#ifndef __MYLABVIEW_H__
#define __MYLABVIEW_H__

#include "LDPY_UART.h"
#include "LDPY_DMA.h"

#include "myStruct.h"
#include "math.h"
#include "myMotor.h"
#include "mySteer.h"
#include "myCCD.h"

#define Bluetooth_Break()		LED3_ON
#define Bluetooth_Connected() 	LED3_OFF

void myLabVIEW_UART_Init(uint32 baud);
void myLabVIEW_ParametersInit(Parameter_Info *Parameter_info,
					CCD_Info *CCD_info,
					Steer_Info *Steer_info,
					Motor_Info *Motor_info,
					Speed_Info *Speed_info
					);

void myLabVIEW_GetSteerDebugNews(Steer_Info *Steer_info);
void myLabVIEW_GetSteerPIDNews(Steer_Info *Steer_info);
void myLabVIEW_GetMotorPIDNews(Motor_Info *Motor_info);
void myLabVIEW_GetSpeedSettingNews(Speed_Info *Speed_info);
void myLabVIEW_GetSpeedNews(Speed_Info *Speed_info);

void myLabVIEW_Send_Uint16(uint16 num);
void myLabVIEW_Send_Uint32(uint32 num);
void myLabVIEW_Send_Float(float Float_In,uint8 Mult);

void myLabVIEW_DebugParameters(Parameter_Info *Parameter_info,CCD_Info *CCD_info,Steer_Info *Steer_info,Motor_Info *Motor_info,Speed_Info *Speed_info);

void myLabVIEW_ParametersSet(Parameter_Info *Parameter_info,CCD_Info *CCD_info,Steer_Info *Steer_info,Motor_Info *Motor_info,Speed_Info *Speed_info);	//参数设定

void myLabVIEW_SendCCDValue(CCD_Info *CCD_info);						//发送CCD数据
void myLabVIEW_SendCCDValueOfTwoCCDs(CCD_Info *CCD1_info,CCD_Info *CCD2_info);	//发送双CCD数据
void myLabVIEW_SendCCDBianaryValue(CCD_Info *CCD1_info,CCD_Info *CCD2_info,CCD_Info *CCD3_info);

#endif

/*---------------------------end of myLabVIEW.h-----------------------------*/
