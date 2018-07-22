#ifndef _SEARCH_H_
#define _SEARCH_H_
#include "common.h"
extern int Max_Peak, Left,Right,leftfind,rightfind,RisingEdgeIndex,FallingEdgeIndex;
extern uint8 Threshold,RisingEdgeCnt,FallingEdgeCnt,Strong_Turn_En,Strong_Turn_Cnt,Strong_Turn_Angle,Strong_Turn_Speed,Barri_Turn_Angle,Barri_Turn_Speed,RoadTypeSwitch;
extern float  Middle_Err;
extern float Last_Singleine_Position,Last_Angle_Turn_Position;
extern uint8  RoadType,RoadTypeConfirm, Hill_Flag;              //赛道类型确认;
extern float  Character_Distance;
extern float Previous_Error[15];
extern int Right_Or_Left,Reference_Width;
extern uint8  Leftlastfind,Rightlastfind,Rightlast,Leftlast;
extern int Turn_State,Turn_State_Cnt;
void RoadType_Distinguish();
void Prejudge();
void Normal();
void CrossRoad();
void Barraicade();
void SingleLine();
void Find_Bothine();
void RightAngleTurn();
void Is_Barraicade(); 
void Is_Straight_Angle_Turn();
uint8 Is_Black_Line();
void Is_Single_Line();
void Is_CrossRoad();
void Is_Hill();
void  Brightness_Calculate();
void Hill();
uint8 FindBoundary(uint8 mode);
uint8 FindNearest(uint8 mode,uint8 lastegde);
void Push_And_Pull(float *buff,int len,float newdata);

#endif