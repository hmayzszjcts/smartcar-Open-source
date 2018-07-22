
#ifndef  __MYCCD_H__
#define  __MYCCD_H__

#include "MK60DZ10.h"
#include "LDPY_GPIO.h"
#include "LDPY_SysTick.h"
#include "LDPY_ADC.h"
#include "LDPY_DMA.h"

#include "myLabVIEW.h"
#include "myGyro.h"
#include "myStruct.h"

#define  	CCD1_CK  	PBout(16)
#define  	CCD1_SI  	PBout(11)

#define  	CCD2_CK  	PBout(8)
#define  	CCD2_SI  	PBout(7)

#define  	CCD3_CK  	PBout(5)
#define  	CCD3_SI  	PBout(4)

#define 	Photo_Center	(63)

#define 	TurnRoundSteer_Max (800)

#define 	ROAD_WIDETH_1_MAX	(90)		//CCD1 直道上占55个点
#define 	ROAD_WIDETH_1_MIN	(20)

#define 	ROAD_WIDETH_2_MAX	(90)		//CCD2 直道上占60个点
#define 	ROAD_WIDETH_2_MIN	(20)

#define 	ROAD_WIDETH_3_MAX	(80)		//CCD3 直道上占40个点
#define 	ROAD_WIDETH_3_MIN	(10)

#define 	TurnRound_Pixel_MIN2	(10)	//CCD2人字道斑马线所占点数最小值
#define		TurnRound_Pixel_MAX2	(40)	//CCD2人字道斑马线所占点数最大值

#define 	TurnRound_Pixel_MIN3	(3)		//CCD3人字道斑马线所占点数最小值
#define		TurnRound_Pixel_MAX3	(30)	//CCD3人字道斑马线所占点数最大值

#define		StartEndLine_Pixel_MIN	(5)		//起跑线的最小宽度
#define		StartEndLine_Pixel_MAX	(20)	//起跑线的最大宽度

#define 	CAR_LEFT_WHEEL_PIXEL	(58)	//CCD1指示的左边车轮点
#define 	CAR_RIGHT_WHEEL_PIXEL	(68)	//CCD1指示的右边车轮点

#define 	CCD1_DataReady_Num	(20)		//CCD1数据有效次数上限
#define 	CCD2_DataReady_Num	(20)		//CCD2数据有效次数上限
#define 	CCD3_DataReady_Num	(20)		//CCD3数据有效次数上限

void myCCD_GPIO_Init(void);
void myCCD_CCD1_Sampling(CCD_Info *CCD_info);
void myCCD_CCD2_Sampling(CCD_Info *CCD_info);
void myCCD_CCD3_Sampling(CCD_Info *CCD_info);

void myCCD_DataInit(CCD_Info *CCD_info);
void myCCD_FilterAndBinarization(	CCD_Info *CCD1_info,
									CCD_Info *CCD2_info,
									CCD_Info *CCD3_info,
									Parameter_Info *Parameter_info
								);
void myCCD_FilterAtFirst(CCD_Info *CCD1_info,CCD_Info *CCD2_info,CCD_Info *CCD3_info);
void myCCD_DataHandle(	CCD_Info *CCD1_info,
						CCD_Info *CCD2_info,
						CCD_Info *CCD3_info,
						HeadSteer_Info *HeadSteer_info,
						Speed_Info *Speed_info,
						Parameter_Info *Parameter_info,
						Road_Condition *Road_condition,
						Car_State *Car_state,
						Car_State *Car_state_Pre
					 );

//起跑的时候获得道路偏差
void myCCD_GetBeginLineError(	CCD_Info *CCD1_info,
								CCD_Info *CCD2_info,
								CCD_Info *CCD3_info,
								Speed_Info *Speed_info,
								Parameter_Info *Parameter_info
							);

//坡道预判函数
uint8_t myCCD_JudgeRampInAdvance(CCD_Info *CCD1_info,CCD_Info *CCD2_info,CCD_Info *CCD3_info);
//CCD判断下坡函数
uint8_t myCCD_JudgeRampDown(CCD_Info *CCD1_info,CCD_Info *CCD2_info,CCD_Info *CCD3_info,Road_Condition *Road_condition);

//长直道进入十字道判断
uint8_t myCCD_JudgeLongStraight(	CCD_Info *CCD1_info,
									CCD_Info *CCD2_info,
									CCD_Info *CCD3_info,
									HeadSteer_Info *HeadSteer_info
								);

//CCD1识别起跑线函数
uint8_t myCCD_CCD1_GetStartEndLine(	CCD_Info *CCD1_info,
									CCD_Info *CCD2_info,
									CCD_Info *CCD3_info,
									Road_Condition *Road_condition,
									Car_State *Car_state
								  );

//CCD搜寻中线函数
void myCCD_CCD1_GetLineError(CCD_Info *CCD1_info,Speed_Info *Speed_info);
void myCCD_CCD2_GetLineError(CCD_Info *CCD1_info,CCD_Info *CCD2_info,Speed_Info *Speed_info);
void myCCD_CCD3_GetLineError(CCD_Info *CCD3_info,Speed_Info *Speed_info);
void myCCD_ChooseSpeedMode(CCD_Info *CCD1_info,
							CCD_Info *CCD2_info,
							CCD_Info *CCD3_info,
							HeadSteer_Info *HeadSteer_info,
							Speed_Info *Speed_info,
							Parameter_Info *Parameter_info
							);

//人字道左边界相关搜索函数
int16_t myCCD_CCD2_GetLeftIntoTurnRound(CCD_Info *CCD2_info);
int16_t myCCD_CCD3_GetLeftIntoTurnRound(CCD_Info *CCD3_info);
int16_t myCCD_CCD2_GetLeftLineOutTurnRound(CCD_Info *CCD_info);

//人字道右边界相关搜索函数
int16_t myCCD_CCD2_GetRightIntoTurnRound(CCD_Info *CCD2_info);
int16_t myCCD_CCD3_GetRightIntoTurnRound(CCD_Info *CCD3_info);
int16_t myCCD_CCD2_GetRightLineOutTurnRound(CCD_Info *CCD_info);

//路障识别函数
uint8_t myCCD_GetObstacle(	CCD_Info *CCD1_info,
							CCD_Info *CCD2_info,
							Steer_Info *Steer_info
							);
				
//判断车轮是否出界				
uint8_t myCCD_JudgeWheelOutOfWay(	CCD_Info *CCD1_info,
									CCD_Info *CCD2_info,
									CCD_Info *CCD3_info,
									Speed_Info *Speed_info
								);
								
//人字道60度尖角识别
void myCCD_Get_60_Angle(CCD_Info *CCD1_info,CCD_Info *CCD2_info,CCD_Info *CCD3_info,Speed_Info *Speed_info,Parameter_Info *Parameter_info,Car_State *Car_state);

#endif

/*----------------------------end of myCCD.h------------------------------*/
