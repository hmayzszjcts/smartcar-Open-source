
#ifndef __MYSTRUCT_H__
#define __MYSTRUCT_H__

#include "MK60DZ10.h"
#include "math.h"
#include "LDPY_TypeChange.h"

/*-----------------------选择使用各个功能辅助模块-------------------*/
//#define LabVIEW_Control_Enable	(1)					//使用上位机
//#define	DMA_Enable (2)								//开DMA
//#define   Remote_Control_Enable		(3)				//遥控器控制使能
#define   RemRoad_Control_Enable	(4)	//记忆算法使能,注意，此算法不能关闭，否则可能导致坡道问题
//#define   PROTECT_CAR_ENABLE		(5)				//出界保护
//#define   GetRoadCondition_Enable	(6)				//获取赛道类型

/*-----------------------DMA通道选择----------------------------------*/
//#define DMA_CH0_Enable	(1)//发送CCD图像
//#define DMA_CH1_Enable	(2)//不发送图像,只发送舵机，电机，陀螺仪，及路况信息 
//#define DMA_CH2_Enable 	(3)//发送压缩图像或者临时变量项，两者不能同时发送

/*-----------------------DMA发送数据选择------------------------------*/
//#define DMA_CH2_SendImage_Enable			(4)//CH2通道复用功能1：发送压缩图像
//#define DMA_CH2_SendLabVIEW_Info_Enable		(5)//CH2通道复用功能2：发送临时变量项
			
#define 	SPEED_FIFO_LENGTH		(20)
#define     Line_SIZE   			(45)		//存储左右边界线，中线等的队列长度
#define 	GYRO_LENGTH				(16)		//存储陀螺仪的AD电压值
#define 	ANGLE_LENGTH			(20)		//60度尖角队列长度

/*--------------------DMA发送缓冲区大小-------------------------*/
#define  image_SendBuff_size   (358)//定义缓冲区的大小，单位字节

#define MAX(a,b) (((a)>(b))?(a):(b))	//求大者
#define MIN(a,b) (((a)<(b))?(a):(b))	//求小者

typedef enum Car_State_	//-------------------------------行车状态
{
	Go_Forward,		//正常前进
	Turn_Round,		//调头
	Stop			//停车
}Car_State;

typedef enum Road_Condition_//---------------------------路况
{
	NeedJudge,			//需再判断
	Straight,			//直道
	Cross,				//十字道
	Obstacle,			//路障
	Ramp_Up,			//上坡
	Ramp_Down			//下坡	
}Road_Condition;

typedef enum Road_Type_	//--------------------------------赛道种类(手动设定是否包含坡道，人字，路障)
{
	None,					//纯竞速，没有坡道，人字，路障(预赛可能用上)
	Ramp_Only,				//只有坡道(预赛可能用上)
	All						//全部类型都有
}Road_Type;

typedef enum Car_Mode_//----------------------------------------选择速度模式(比赛时速度等级不同)
{
	High2,
	High1,
	Safe,
	Low1,
	Low2,
	SelfDef,				//自定义速度
	CheckCCD,				//调试(前瞻)模式
	CheckSL,				//调试起跑线模式
	CheckRamp				//调试坡道模式
}Car_Mode;

typedef struct CCD_Info_//--------------------------------CCD数据
{
	uint16_t PixelOri[2][128];      	// CCD原始值
	uint16_t Pixel[128];				// CCD滤波后值
	uint16_t PixelAfterMult[128];		// CCD放大后值
	uint16_t PixelBinary[128];			// CCD二值化值
	uint8_t  CCD_PhotoValue[16];		// CCD二维压缩图像存储值
	
	uint16_t AD_MAX[4];					//AD最大值
	uint16_t AD_MIN[4];					//AD最小值
	
	int16_t CCD_PhotoCenter;			//图像的中心点
	int16_t CCD_ObstacleShift;			//路障的时候图像偏移点数
	
	int16_t  LeftLine[Line_SIZE];		//左边界队列
	int16_t  CentralLine[Line_SIZE];	//中线队列
	int16_t  RightLine[Line_SIZE];		//右边界队列
	int16_t  LineError[Line_SIZE];		//偏差队列
	int16_t  LineError_D[Line_SIZE];	//偏差D队列
	int16_t  RoadWidth[10];				//路宽队列
	int16_t  Angle_LeftFifo[ANGLE_LENGTH];	//60度尖角队列
	int16_t  Angle_RightFifo[ANGLE_LENGTH];	//60度尖角队列
	
	int16_t LeftLossLinePixel;			//记录丢左边线时左边的点
	int16_t RightLossLinePixel;			//记录丢右边线时右边的点
	
	uint8_t LeftLoseLine_Flag;
	uint8_t RightLoseLine_Flag;
	
	int16_t  RoadWidthOfStraight;		//直道的路宽
	
	uint8_t  InvalidPixel_Num;			//CCD图像无效点数
	uint8_t  CCD_Ready_Num;				//CCD数据有效次数
	
	uint8_t AddLine_Flag;				//补线标记
	uint8_t LossLine_Flag;				//CCD丢线标志
	uint8_t Cross_Flag;					//十字道标志
	uint8_t RoadInvalid_Flag;			//左边赛道无效标记，防止窜道
	
}CCD_Info;

typedef struct Gyro_Info_//------------------------------------------------陀螺仪
{
	int16_t	GyroscopeZero;						//记录陀螺仪-加速度计的零点电压AD值
	int16_t	Gyroscope_Fifo[GYRO_LENGTH];		//存储陀螺仪-加速度计电压AD值
	int32_t Gyro_Sum;
	
	int16_t RampThresholdValue;	
	uint8_t RampUpDown_Num;		//记录检测到陀螺仪电压突变的次数
	
}Gyro_Info;

typedef struct Speed_Info_//-------------------------------------------------速度
{
	int16_t Straight_Speed;					//直道速度
	int16_t Cur_Speed;				 		//弯道速度
	int16_t Snake_Speed;					//小S速度
	int16_t TurnRound_Speed;				//调头速度
	int16_t Obstacle_Speed;					//路障速度
	int16_t RampUp_Speed;					//上坡道速度
	int16_t RampDown_Speed;					//下坡道速度
	int16_t Angle60_Speed;					//60度尖角速度
	
	int16_t SpeedAdd_3;						//3档的直道最高速度增加值
	float SpeedMode3Add;					//3档速度增加值
	int16_t SpeedMode3Acc_Counter;			//使用2档，但使用3档速度档时的条件有效距离(cm)
	uint8_t CurSpeedAcc_Flag;				//出弯道加速标记
	float CurSpeedAcc;						//弯道加速值
	
	int16_t Speed_Now;						//当前速度
	int16_t TargetSpeed[2];					//目标速度
	int16_t SpeedRAcc;						//加速度值
	
	uint8_t SpeedMode;						//速度模式选择
	uint8_t LastSpeedMode;					//上一次的速度模式
	
	int8_t  Speed_Direction;				//速度方向
	int16_t EncoderData;					//编码器寄存器的值
	int16_t SpeedFifo[SPEED_FIFO_LENGTH];	//速度队列
	int16_t SpeedError;						//速度偏差
	int16_t LastSpeedError;					//上一次的速度偏差
	
	int32_t DistanceTo1CmCount;				//1cm脉冲计数
	uint8_t DistanceOf1Cm_Flag;				// 1cm到了标记
	int16_t DistanceOfCross_Counter;		//十字道1cm距离计数
	int16_t DistanceOfTurnRound_Counter;	//人字道1cm距离计数
	int16_t DiatanceToTurnRound;			//识别到人字道时距离尖角的距离
	int16_t DistanceBeginTurn;				//开始人字转角时的距离
	int16_t DistanceToTurn_MIN;				//人字道开转的最小距离
	int16_t DistanceToTurn_MAX;				//人字道开转的最大距离
	int16_t DistanceOfRampUp[2];			//上坡距离
	int16_t DistanceOfRampDown;				//下坡距离
	int16_t DistanceOfRampUp_Counter;		//上坡道1cm距离计数
	int16_t DistanceOfObstacle_Counter;		//路障1cm距离计数
	int16_t DistanceOfSpeedMode3_Counter;	//3档高速1cm距离计数
	int16_t DistanceOfSpeedMode3_Max;		//使用3档的最大高速距离
	int16_t DistanceOfRoadInvalid_Counter;	//道路无效最长距离,超过距离则强制有效
	int16_t DistanceOfCCD1GetSL_Counter;	//CCD1预判起跑线后的最长检测距离
	int16_t DistanceOf60Angle_Counter;		//人字道60度尖角预判后的最长人字检测距离
	
	uint8_t SpeedCalculate_Enable;			//计算速度使能
	
	int16_t Error_K;
	int16_t Error_D_K;
	int32_t HeadSteerPWMError_K;
    int32_t HeadSteerPWMError_D_K;
	
}Speed_Info;

typedef struct HeadSteer_Info_//------------------------------------摇头舵机
{
	float KPH;
	float KIH;

	int32_t Steer_Center;				
	int32_t Steer_LeftMAX;				
	int32_t Steer_RightMAX;			

	int32_t Steer_PWMOutputDuty;

	int32_t SteerPWM_Error[10];
	int32_t SteerPWM_Error_D[10];
  
}HeadSteer_Info;

typedef struct Steer_Info_//--------------------------------------舵机
{
	float KP1;
	float KP2;
	float KP3;
	float KI;

	float HeadToSteer_1;
	float HeadToSteer_2;
	float HeadToSteer_3;

	uint8 KP_Mult;				//PID放大倍数
	uint8 KD_Mult;
	uint8 KI_Mult;

	int32_t Steer_Center;				
	int32_t Steer_LeftMAX;				
	int32_t Steer_RightMAX;			

	int32_t Steer_PWMOutputDuty;	
	int32_t SteerPWM_Error[10];

	uint8_t RampUp_SteerBackToCenter_Flag;

}Steer_Info;

typedef struct Motor_Info_//-----------------------------------------------电机
{
	float KPT;
	float KP;
	float KI;
	float KD;

	uint8 KP_Mult;				//PID放大倍数
	uint8 KI_Mult;
	uint8 KD_Mult;

	int32 PWMOutputDuty;				
  
}Motor_Info;

typedef struct Parameter_Info_//------------------------------------------一些参数
{
	uint8 Parameter_OK;		
	uint8 UART_RecvData;				//用于串口数据接收
	float StartEndLine_Fifo[10][2];		//起跑线队列,10行2列，第一列记录红外管标号，第二列记录采集时间
	
	float AD_MultK_f;
	float CCD1_AD_TargetValue;			//CCD电压放大的目标值
	int16_t CCD1_Binary_MAX;			//CCD阈值上限
	int16_t CCD1_Binary_MIN;			//CCD阈值下限

	uint8_t GetTurnRound_Enable;		//检测人字道使能(TRUE/FALSE)
	uint8_t GetRamp_Enable;				//检测坡道使能(TRUE/FALSE)
	uint8_t GetStartEndLine_Enable;		//检测起跑线使能(TRUE/FALSE)
	uint8_t GetStartEndLine_Flag;		//检测起跑线标记（1/0）
	uint8_t GetObstacle_Enable;			//检测路障使能(TRUE/FALSE)
	uint8_t RoadInvalid_Enable;			//检测道路有效无效判断(TRUE/FALSE)
	uint8_t GetStartEndLine_SameLight_Enable;//起跑线检测同侧红外管使能
	uint8_t CCD1GetSL_Enable;			//CCD1识别起跑线使能
	uint8_t WheelOutOfWay_Enable;		//判断轮子出界使能
	
	int16_t RampJgeInAdvce_Num;			//坡道预判计数
	int16_t TurnRound_OK_Num;			//人字道确认次数,连续几次检测到人字道才认为是人字道
	int16_t TurnRound_Num_MAX;			//人字道最大确认次数
	int16_t Obstacle_OK_Num;			//路障判断次数计数，连续几次检测到路障才认为是路障
	
	int32_t Angle60_MeanValue;			//人字道尖角判断时另一条边界的方差最大值
	
	int16_t Mode_HighSpeedDistance;		//高速距离
	
	float	Time;						//行驶时间(秒)
	float   GetStartEndLineTime;		//起跑线检测倒计时，超过该时间才检测起跑线
	float	GetTurnRoundTime;			//开始检测人字道的时间
	float	GetRampTime;				//开始检测坡道的时间
	float   GetObstacleTime;			//开始检测路障的时间
	float	DebugTime;					//调试时间(秒)
	float	LoseLineTime;				//丢线停车时间
	int32_t RoadDistance;				//行驶距离(厘米)
	int16_t StopSpeed;					//停车前一刻速度

	uint8_t CCD_SampleOK_Flag;  		//CCD采样完成标志
	uint8_t StartEndLine_Flag; 			//检测到起跑线标志

	uint8_t	LeftIntoTurnRound_Flag;		//左边入人字道
	uint8_t RightIntoTurnRound_Flag;	//右边入人字道
	uint8_t CCD2OrCCD3_GetTurnRound;	//指示是CCD2还是CCD3检测到人字道
	
	uint8_t LeftOrRightObstacle_Flag;	//路障标记(左边路障/右边路障)
	
	int32_t OLED_NoAction_Counter;		//按键没反应计数
	uint8_t OLED_NoAction_Flag;			//按键没有反应标志
	uint8_t RampReady_Distance;			//预判坡道超过该距离还未判断到坡道则清除预判标记，防止预判错误时长时间减速
	uint8_t RampReady_FLag;				//坡道预判标记
	uint8_t SlowBeforeTurnRound_Flag;	//人字前后速度不同设定标记
	uint8_t LongStraight_Flag;
	uint8_t CCD1_GetedStartEndLine_Flag;//CCD1识别到起跑线标记
	uint8_t	Left60Angle_Flag;			//左边60度尖角标记
	uint8_t Right60Angle_Flag;			//右边60度尖角标记
  
}Parameter_Info;

#ifdef  RemRoad_Control_Enable

typedef struct RemSpeedUpDown_Info_//-----------------------------------记忆算法结构体
{
	uint8_t Ramp_Counter;				//坡道计数
	uint16_t RampSpeedUp[2][3];			//有2个坡道
	uint8_t Ramp_Num[2];
	int16_t RampRemDistance[4];
	
	uint8_t Cross_Counter;				//十字道计数
	uint16_t CrossSpeedUp[16][3];		//有16个十字道
	uint8_t Cross_Num[2];
	int16_t CrossRemDistance[4];
	
	uint8_t TurnRound_Counter;			//人字道计数
	uint16_t TurnRoundSpeedUp[2][3];	//有2个人字道
	uint8_t TurnRound_Num[2];
	int16_t TurnRoundRemDistance[4];
	
	uint8_t RemRoad_Enable;				//记忆算法使能（TRUE/FALSE）
}RemSpeedUpDown_Info;

#endif

typedef struct Flash_Info_//------------------------------------------------Flash操作
{
	float   Flash_Array_1[100];
	float   Flash_Array_2[100];
	float   Flash_Array_3[100];
  
}Flash_Info;

/*************************DMA所需参数配置部分********************************/

#ifdef LabVIEW_Control_Enable

typedef struct LabVIEW_Info_
{
	float HeadSteer_KP_Item;					//摇头舵机KP项数据	
	float HeadSteer_KI_Item;					//摇头舵机KI项数据
	
	float Steer_Head_Item;						//转向舵机摇头项数据
	float Steer_Err_Item;						//转向舵机偏差项数据
	
	float Motor_KP_Item;						//电机KP项数据
	float Motor_KI_Item;						//电机KI项数据
	float Motor_KD_Item;						//电机KD项数据
		
}LabVIEW_Info;

#endif

typedef struct DMA_Required_Variable
{
	uint8_t CCD_Image[image_SendBuff_size];	
	uint8_t ThreeCCDs_Image[420];
}DMA_Required_Variable;

typedef struct 
{
	uint8_t  enter_exit;
	uint16_t choice_flag;
	uint8_t  add_sub;
	uint8_t  enter_exit_temp;
	uint8_t  Clear;
	uint8_t  Ready_Go;
	
}menu;

#endif

/*-----------------------------end of myStruct.h----------------------------------*/	
