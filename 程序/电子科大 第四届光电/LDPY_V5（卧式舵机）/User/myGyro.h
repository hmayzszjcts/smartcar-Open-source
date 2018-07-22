
#ifndef __MYGYRO_H__
#define __MYGYRO_H__

#include "MK60DZ10.h"
#include "LDPY_ADC.h"
#include "myCCD.h"
#include "myStruct.h"

void myGyro_GPIOInit(void);
void myGyro_DataInit(Gyro_Info *Gyro_info);
Road_Condition myGyro_GetRamp(CCD_Info *CCD1_info,Gyro_Info *Gyro_info,Speed_Info *Speed_info,Road_Condition *Road_condition);

#endif

/*----------------------------end of myGyro.h------------------------------*/
