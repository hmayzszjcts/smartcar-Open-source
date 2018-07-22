#ifndef _CCD_H_
#define _CCD_H_
#include "common.h"

#define  CCD_CLK  PTB1_OUT 
#define  CCD_SI   PTB2_OUT
extern uint16 CCD_Buff[128];
extern uint16 Max_Value,Brightness;
extern uint8  CCD_Offset; 
void CCD_Normalization();
void CCD_Capture();
void CCD_Exposure(); 

#endif