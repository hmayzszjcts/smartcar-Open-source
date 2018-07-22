
#ifndef __MYREMROAD_H__
#define __MYREMROAD_H__

#include "MK60DZ10.h"
#include "LDPY_TypeChange.h"
#include "myLED.h"
#include "myStruct.h"

void myRemRoad_DataInit(RemSpeedUpDown_Info *RemSpeedUpDown_info);
void myRemRoad_DataSetting(RemSpeedUpDown_Info *RemSpeedUpDown_info);
uint8_t myRemRoad_ControlSpeed(RemSpeedUpDown_Info *RemSpeedUpDown_info,Parameter_Info *Parameter_info);

#endif

/*---------------------------------------end of myRemRoad.h------------------------------------*/
