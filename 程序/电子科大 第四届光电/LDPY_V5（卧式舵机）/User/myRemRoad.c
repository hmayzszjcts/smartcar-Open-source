
#include "myRemRoad.h"

RemSpeedUpDown_Info RemSpeedUpDown_info = {0};

/*************************************************************************
*					零		度		偏		移   
*  函数名称: myRemRoad_DataInit
*  功能说明: 记忆算法数据初始化函数
*  参数说明:	
*            
*  函数返回:
*  修改时间:2014-07-20
*  备	注:
*************************************************************************/
void myRemRoad_DataInit(RemSpeedUpDown_Info *RemSpeedUpDown_info)
{	
	uint8_t ii = 0;
	
	/*---------------------坡道-----------------------*/
	RemSpeedUpDown_info->Ramp_Counter = 0;							//记录坡道个数
	RemSpeedUpDown_info->RampSpeedUp[0][0] = 0;						//记录识别到第一个坡道时的路程
	RemSpeedUpDown_info->RampSpeedUp[0][1] = 0;						//坡道后，高速行驶前的正常距离
	RemSpeedUpDown_info->RampSpeedUp[0][2] = 0;						//记录第一个坡道后的高速距离
	RemSpeedUpDown_info->RampSpeedUp[1][0] = 0;						//记录识别到第二个坡道时的路程
	RemSpeedUpDown_info->RampSpeedUp[1][1] = 0;						//坡道后，高速行驶前的正常距离
	RemSpeedUpDown_info->RampSpeedUp[1][2] = 0;						//记录第二个坡道后的加速距离
	
	//坡道标号初始化
	RemSpeedUpDown_info->Ramp_Num[0] = 1;
	RemSpeedUpDown_info->Ramp_Num[1] = 1;
	
	//坡道距离初始化
	RemSpeedUpDown_info->RampRemDistance[0] = 0;
	RemSpeedUpDown_info->RampRemDistance[1] = 0;
	RemSpeedUpDown_info->RampRemDistance[2] = 0;
	RemSpeedUpDown_info->RampRemDistance[3] = 0;

	/*---------------------十字道--------------------*/
	RemSpeedUpDown_info->Cross_Counter = 0;							//记录十字道个数
	for(ii = 0; ii < 16; ii++)
	{
		RemSpeedUpDown_info->CrossSpeedUp[ii][0] = 0;
		RemSpeedUpDown_info->CrossSpeedUp[ii][1] = 0;
		RemSpeedUpDown_info->CrossSpeedUp[ii][2] = 0;
	}
	
	//设定十字路口后的加速段
	RemSpeedUpDown_info->CrossSpeedUp[1][1] = 0;//第二个十字后高速前距离
	RemSpeedUpDown_info->CrossSpeedUp[1][2] = 0;//第二个十字道后
	RemSpeedUpDown_info->CrossSpeedUp[3][1] = 0;//第四个十字后高速前距离
	RemSpeedUpDown_info->CrossSpeedUp[3][2] = 0;//第四个十字道后
	
	//十字道标号初始化
	RemSpeedUpDown_info->Cross_Num[0] = 1;
	RemSpeedUpDown_info->Cross_Num[1] = 1;
	
	//十字道距离初始化
	RemSpeedUpDown_info->CrossRemDistance[0] = 250;
	RemSpeedUpDown_info->CrossRemDistance[1] = 400;
	RemSpeedUpDown_info->CrossRemDistance[2] = 150;
	RemSpeedUpDown_info->CrossRemDistance[3] = 400;
	
	/*---------------------人字道--------------------*/
	RemSpeedUpDown_info->TurnRound_Counter = 0;						//记录人字道个数
	RemSpeedUpDown_info->TurnRoundSpeedUp[0][0] = 0;				//记录识别到第一个人字道时的路程
	RemSpeedUpDown_info->TurnRoundSpeedUp[0][1] = 0;				//人字道后,高速前的正常距离
	RemSpeedUpDown_info->TurnRoundSpeedUp[0][2] = 0;				//记录第一个人字道后的高速距离
	RemSpeedUpDown_info->TurnRoundSpeedUp[1][0] = 0;				//记录识别到第二个人字道时的路程
	RemSpeedUpDown_info->TurnRoundSpeedUp[1][1] = 0;				//人字道后,高速前的正常距离
	RemSpeedUpDown_info->TurnRoundSpeedUp[1][2] = 0;				//记录第二个人字道后的高速距离
	
	//人字道标号初始化
	RemSpeedUpDown_info->TurnRound_Num[0] = 1;
	RemSpeedUpDown_info->TurnRound_Num[1] = 1;
	
	//人字道距离初始化
	RemSpeedUpDown_info->TurnRoundRemDistance[0] = 300;
	RemSpeedUpDown_info->TurnRoundRemDistance[1] = 400;
	RemSpeedUpDown_info->TurnRoundRemDistance[2] = 300;
	RemSpeedUpDown_info->TurnRoundRemDistance[3] = 400;
	
	RemSpeedUpDown_info->RemRoad_Enable = TRUE;						//默认开启记忆算法
}	

/*************************************************************************
*					零		度		偏		移   
*  函数名称: myRemRoad_DataSetting
*  功能说明: 记忆算法数据赋值函数
*  参数说明:	
*            
*  函数返回:
*  修改时间:2014-08-18
*  备	注:
*************************************************************************/
void myRemRoad_DataSetting(RemSpeedUpDown_Info *RemSpeedUpDown_info)
{	
	/*---------------------坡道-----------------------*/
	RemSpeedUpDown_info->RampSpeedUp[RemSpeedUpDown_info->Ramp_Num[0] - 1][1] = RemSpeedUpDown_info->RampRemDistance[0];			
	RemSpeedUpDown_info->RampSpeedUp[RemSpeedUpDown_info->Ramp_Num[0] - 1][2] = RemSpeedUpDown_info->RampRemDistance[1];				
	RemSpeedUpDown_info->RampSpeedUp[RemSpeedUpDown_info->Ramp_Num[1] - 1][1] = RemSpeedUpDown_info->RampRemDistance[2];						
	RemSpeedUpDown_info->RampSpeedUp[RemSpeedUpDown_info->Ramp_Num[1] - 1][2] = RemSpeedUpDown_info->RampRemDistance[3];						

	/*---------------------十字道--------------------*/
	RemSpeedUpDown_info->CrossSpeedUp[RemSpeedUpDown_info->Cross_Num[0] - 1][1] = RemSpeedUpDown_info->CrossRemDistance[0];
	RemSpeedUpDown_info->CrossSpeedUp[RemSpeedUpDown_info->Cross_Num[0] - 1][2] = RemSpeedUpDown_info->CrossRemDistance[1];
	RemSpeedUpDown_info->CrossSpeedUp[RemSpeedUpDown_info->Cross_Num[1] - 1][1] = RemSpeedUpDown_info->CrossRemDistance[2];
	RemSpeedUpDown_info->CrossSpeedUp[RemSpeedUpDown_info->Cross_Num[1] - 1][2] = RemSpeedUpDown_info->CrossRemDistance[3];

	/*---------------------人字道--------------------*/			
	RemSpeedUpDown_info->TurnRoundSpeedUp[RemSpeedUpDown_info->TurnRound_Num[0] - 1][1] = RemSpeedUpDown_info->TurnRoundRemDistance[0];				
	RemSpeedUpDown_info->TurnRoundSpeedUp[RemSpeedUpDown_info->TurnRound_Num[0] - 1][2] = RemSpeedUpDown_info->TurnRoundRemDistance[1];				
	RemSpeedUpDown_info->TurnRoundSpeedUp[RemSpeedUpDown_info->TurnRound_Num[1] - 1][1] = RemSpeedUpDown_info->TurnRoundRemDistance[2];				
	RemSpeedUpDown_info->TurnRoundSpeedUp[RemSpeedUpDown_info->TurnRound_Num[1] - 1][2] = RemSpeedUpDown_info->TurnRoundRemDistance[3];				
}	

/*************************************************************************
*					零		度		偏		移   
*  函数名称: myRemRoad_ControlSpeed
*  功能说明: 记忆算法控制函数
*  参数说明:	
*            
*  函数返回:
*  修改时间:2014-07-10
*  备	注:
*************************************************************************/
uint8_t myRemRoad_ControlSpeed(RemSpeedUpDown_Info *RemSpeedUpDown_info,Parameter_Info *Parameter_info)
{
	//十字道加速
	if((RemSpeedUpDown_info->Cross_Counter > 0)
		&& (RemSpeedUpDown_info->CrossSpeedUp[RemSpeedUpDown_info->Cross_Counter - 1][2] > 0))//十字道计数不为零
	{
		if(RemSpeedUpDown_info->CrossSpeedUp[RemSpeedUpDown_info->Cross_Counter - 1][0]
			+ RemSpeedUpDown_info->CrossSpeedUp[RemSpeedUpDown_info->Cross_Counter - 1][1]> Parameter_info->RoadDistance)
		{
			//十字道后还有约200cm的过渡段
			return FALSE;
		}
		else if((RemSpeedUpDown_info->CrossSpeedUp[RemSpeedUpDown_info->Cross_Counter - 1][0] 
				+ RemSpeedUpDown_info->CrossSpeedUp[RemSpeedUpDown_info->Cross_Counter - 1][1]
				+ RemSpeedUpDown_info->CrossSpeedUp[RemSpeedUpDown_info->Cross_Counter - 1][2]) > Parameter_info->RoadDistance)
		{
			//此处可以设置为十字道过后的高速模式
			return TRUE;
		}
		else 
		{
			return FALSE;
		}
	}
	
	//人字道加速
	if((RemSpeedUpDown_info->TurnRound_Counter >= 1)
		&& (RemSpeedUpDown_info->TurnRoundSpeedUp[RemSpeedUpDown_info->TurnRound_Counter - 1][2] > 0))//十字道计数不为零
	{
		if(RemSpeedUpDown_info->TurnRoundSpeedUp[RemSpeedUpDown_info->TurnRound_Counter - 1][0] 
			+ RemSpeedUpDown_info->TurnRoundSpeedUp[RemSpeedUpDown_info->TurnRound_Counter - 1][1] > Parameter_info->RoadDistance)
		{
			return FALSE;
		}
		else if((RemSpeedUpDown_info->TurnRoundSpeedUp[RemSpeedUpDown_info->TurnRound_Counter - 1][0]
				+ RemSpeedUpDown_info->TurnRoundSpeedUp[RemSpeedUpDown_info->TurnRound_Counter - 1][1]
				+ RemSpeedUpDown_info->TurnRoundSpeedUp[RemSpeedUpDown_info->TurnRound_Counter - 1][2]) > Parameter_info->RoadDistance)
		{
			//人字道后高速段满足
			return TRUE;
		}
		else 
		{
			return FALSE;
		}
	}

	//上诉条件全部都不满足则返回FALSE
	return FALSE;
}

/*-----------------------------------end of myRemRoad.c----------------------------------*/
