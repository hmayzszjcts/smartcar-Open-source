
#include "myLabVIEW.h"

#ifdef DMA_CH2_SendLabVIEW_Info_Enable

	LabVIEW_Info LabVIEW_info = {0};
	
#endif

//外部变量声明
extern Parameter_Info Parameter_info;

extern CCD_Info CCD1_info;
extern CCD_Info CCD2_info;
extern CCD_Info CCD3_info;

extern Car_State Car_state;
extern Car_State Car_state_Pre;

/*************************************************************************
*					零		度		偏		移  
*
*  函数名称：myLabVIEW_UART_Init
*  功能说明：与上位机通信初始化设置，波特率等
*  参数说明：
*  函数返回：无
*  修改时间：2014-06-21
*  备    注：
*************************************************************************/
void myLabVIEW_UART_Init(uint32 baud)
{
  uint8 Start[2]={0};

  LDPY_UART5_Init(baud);							//初始化串口UART5，波特率baud(115200)
  Bluetooth_Break();								//点亮蓝牙指示灯
  
  LDPY_UART5_ClearReceiveBuffer();					//清除UART5中的接收缓冲区的内容
  
  do
  {
    Start[0] = Start[1];
    Start[1] = LDPY_UART5_GetChar();
	  
  }while(('T' != Start[0]) || ('S' != Start[1]));	//当接收到的串口数据不是'SD'时，说明是蓝牙发送的乱码，所以一直读取以清空接收缓冲区
  
  if((Start[0] == 'T') && (Start[1] == 'S'))		//接收到的字符串为上位机的启动命令，则继续
  {
	  LDPY_UART5_PutChar('O');						//接收到蓝牙连接命令成功后返回"O"
  }
  
  Bluetooth_Connected();							//熄灭蓝牙连接指示灯
}

/*************************************************************************
*					零		度		偏		移  
*
*  函数名称：myLabVIEW_UART5ReceiveIsr
*  功能说明：UART5串口接收中断函数,用于在程序运行过程中接收消息
*  参数说明：
*  函数返回：无
*  修改时间：2014-06-21
*  备    注：
*************************************************************************/
void myLabVIEW_ParametersInit(  Parameter_Info *Parameter_info,
								CCD_Info *CCD_info,
								Steer_Info *Steer_info,
								Motor_Info *Motor_info,
								Speed_Info *Speed_info
							 )
{	
  	uint8 UART5_RecvData = 0;
    
	UART5_RecvData = LDPY_UART5_GetChar();//接收一个字节（帧头）
	
	if('O' == UART5_RecvData)//设定初始化参数
	{
	  	myLabVIEW_ParametersSet(Parameter_info,CCD_info,Steer_info,Motor_info,Speed_info);
	
		LDPY_UART5_PutCharArr("OK",2);//发送参数设定完成标志	
		
		LDPY_UART5_PutChar('x');
		LDPY_UART5_PutChar('x');
			  
		//--------------发送舵机PID信息给上位机---------------//
		LDPY_UART5_PutChar('P');//大写字母
		myLabVIEW_Send_Float(Steer_info->KI,Steer_info->KP_Mult);
		myLabVIEW_Send_Float(Steer_info->KI,Steer_info->KI_Mult);
		myLabVIEW_Send_Float(Steer_info->KI,Steer_info->KD_Mult);
					  
		//--------------发送电机PID信息给上位机---------------//
		LDPY_UART5_PutChar('p');//小写字母
		myLabVIEW_Send_Float(Motor_info->KP,Motor_info->KP_Mult);
		myLabVIEW_Send_Float(Motor_info->KI,Motor_info->KI_Mult);
		myLabVIEW_Send_Float(Motor_info->KD,Motor_info->KD_Mult);
					  
		//--------------返回相应的速度值---------------------//
		LDPY_UART5_PutChar('s');//小写字母		
		myLabVIEW_Send_Uint16(Speed_info->Straight_Speed);
		myLabVIEW_Send_Uint16(Speed_info->Cur_Speed);
		myLabVIEW_Send_Uint16(Speed_info->Snake_Speed);
		myLabVIEW_Send_Uint16(Speed_info->TurnRound_Speed);
		myLabVIEW_Send_Uint16(Speed_info->Obstacle_Speed);
		
		Parameter_info->Parameter_OK = 1;
	}
}

/*************************************************************************
*					零		度		偏		移  
*
*  函数名称：myLabVIEW_DebugParameters
*  功能说明：调试小车参数
*  参数说明：
*  函数返回：无
*  修改时间：2014-06-21
*  备    注：
*************************************************************************/
void myLabVIEW_DebugParameters( Parameter_Info *Parameter_info,
								CCD_Info *CCD_info,
								Steer_Info *Steer_info,
								Motor_Info *Motor_info,
								Speed_Info *Speed_info
							  )
{
	do
	{
		LDPY_UART5_PutChar('x');
		
	}while(('Q' != Parameter_info->UART_RecvData) && (0 == (UART_X->S1 & UART_S1_RDRF_MASK)));//当接收缓冲区没有值的时候
	
	switch(Parameter_info->UART_RecvData)
	{
		case 'Q'://--停车/发车
				{
					if(Stop != Car_state)//只停车，不发车
					{
						Car_state_Pre = Car_state;
						Car_state = Stop;
						
						Parameter_info->StopSpeed = Speed_info->Speed_Now;
						
						Parameter_info->StartEndLine_Flag = 1;
					}
				}
				break;//end of 'Q'
				
		case 'S':
				{  	
					myLabVIEW_GetSteerDebugNews(Steer_info);
			
					//发送舵机参数
					LDPY_UART5_PutChar('S');
					myLabVIEW_Send_Uint16((uint16)Steer_info->Steer_PWMOutputDuty);	//舵机实际PWM值
					myLabVIEW_Send_Uint16((uint16)(CCD_info->LineError[0] + 1000));//加上1000是为了转化为正数发送						//实际赛道偏差
					
					Parameter_info->Parameter_OK = 1;
				}
				break;//end of 'S'
				
		case 's'://设定当前的各种速度
				{
					myLabVIEW_GetSpeedSettingNews(Speed_info);
					
					//--------------返回相应的速度值------------//
					LDPY_UART5_PutChar('s');//小写字母		
					myLabVIEW_Send_Uint16(Speed_info->Straight_Speed);
					myLabVIEW_Send_Uint16(Speed_info->Cur_Speed);
					myLabVIEW_Send_Uint16(Speed_info->Snake_Speed);
					myLabVIEW_Send_Uint16(Speed_info->TurnRound_Speed);
					myLabVIEW_Send_Uint16(Speed_info->Obstacle_Speed);
					
					Parameter_info->Parameter_OK = 1;
				}
				break;//end of 's'
				
		case 'P':	//舵机PID参数接收
				{
					myLabVIEW_GetSteerPIDNews(Steer_info);
					
					//------------------发送舵机PID信息给上位机------------//
					LDPY_UART5_PutChar('P');
					myLabVIEW_Send_Float(Steer_info->KI,Steer_info->KP_Mult);
					myLabVIEW_Send_Float(Steer_info->KI,Steer_info->KI_Mult);
					myLabVIEW_Send_Float(Steer_info->KI,Steer_info->KD_Mult);
					
					Parameter_info->Parameter_OK = 1;
				}
				break;//end of 'P'
				
		case 'p':	//电机PID参数接收
				{
					myLabVIEW_GetMotorPIDNews(Motor_info);
					
					//发送电机PID信息给上位机
					LDPY_UART5_PutChar('p');
					myLabVIEW_Send_Float(Motor_info->KP,Motor_info->KP_Mult);
					myLabVIEW_Send_Float(Motor_info->KI,Motor_info->KI_Mult);
					myLabVIEW_Send_Float(Motor_info->KD,Motor_info->KD_Mult);
					
					Parameter_info->Parameter_OK = 1;
				}
				break;//end of 'p'
				
		default:
				break;
			
	}

	LDPY_UART5_PutChar('x');
	
//	DMA0->ERQ |= (1<<dma_init_struct.DMA_CHx);//开DMA
	
}

/*************************************************************************
*					零		度		偏		移  
*
*  函数名称：myLabVIEW_GetSteerDebugNews
*  功能说明：接收舵机的调试PWM，用于调试舵机的左右极限和中心值等
*  参数说明：
*  函数返回：无
*  修改时间：2014-06-21
*  备    注：
*************************************************************************/
void myLabVIEW_GetSteerDebugNews(Steer_Info *Steer_info)
{
  	uint8 SteerPWMDuty[2] = {0};//用于接收PWM两个字节的数据
	
	SteerPWMDuty[0] = LDPY_UART5_GetChar();//接收高字节
	SteerPWMDuty[1] = LDPY_UART5_GetChar();//接收低字节
	
  	Steer_info->Steer_PWMOutputDuty =(uint32)(((uint16)SteerPWMDuty[0] << 8) | SteerPWMDuty[1]); 
}

/*************************************************************************
*					零		度		偏		移  
*
*  函数名称：myLabVIEW_GetSteerPIDNews
*  功能说明：接收舵机的PID参数
*  参数说明：
*  函数返回：无
*  修改时间：2014-06-21
*  备    注：
*************************************************************************/
void myLabVIEW_GetSteerPIDNews(Steer_Info *Steer_info)
{
  	uint8 SteerPID_KP[3] = {0};//用于接收PID参数（各两个字节）放大倍数（一个字节）
	uint8 SteerPID_KI[3] = {0};
	uint8 SteerPID_KD[3] = {0};
	
	SteerPID_KP[0] = LDPY_UART5_GetChar();		//接收KP高字节
	SteerPID_KP[1] = LDPY_UART5_GetChar();		//接收KP低字节
	SteerPID_KP[2] = LDPY_UART5_GetChar();		//接收放大倍数
	
	SteerPID_KI[0] = LDPY_UART5_GetChar();		//接收KI高字节
	SteerPID_KI[1] = LDPY_UART5_GetChar();		//接收KI低字节
	SteerPID_KI[2] = LDPY_UART5_GetChar();		//接收放大倍数
	
	SteerPID_KD[0] = LDPY_UART5_GetChar();		//接收KD高字节
	SteerPID_KD[1] = LDPY_UART5_GetChar();		//接收KD低字节
	SteerPID_KD[2] = LDPY_UART5_GetChar();		//接收放大倍数
	
  	Steer_info->KI = (float)(((uint16)SteerPID_KP[0] << 8) | SteerPID_KP[1])/(pow(10,SteerPID_KP[2])); 
	Steer_info->KP_Mult = SteerPID_KP[2];
	Steer_info->KI = (float)(((uint16)SteerPID_KI[0] << 8) | SteerPID_KI[1])/(pow(10,SteerPID_KI[2])); 
	Steer_info->KI_Mult = SteerPID_KI[2];
	Steer_info->KI = (float)(((uint16)SteerPID_KD[0] << 8) | SteerPID_KD[1])/(pow(10,SteerPID_KD[2])); 
	Steer_info->KD_Mult = SteerPID_KD[2];
}

/*************************************************************************
*					零		度		偏		移  
*
*  函数名称：myLabVIEW_GetMotorPIDNews
*  功能说明：接收电机的PID参数
*  参数说明：
*  函数返回：无
*  修改时间：2014-06-21
*  备    注：
*************************************************************************/
void myLabVIEW_GetMotorPIDNews(Motor_Info *Motor_info)
{
  	uint8 MotorPID_KP[3] = {0};//用于接收PID参数（各两个字节）放大倍数（一个字节）
	uint8 MotorPID_KI[3] = {0};
	uint8 MotorPID_KD[3] = {0};
	
	MotorPID_KP[0] = LDPY_UART5_GetChar();		//接收KP高字节
	MotorPID_KP[1] = LDPY_UART5_GetChar();		//接收KP低字节
	MotorPID_KP[2] = LDPY_UART5_GetChar();		//接收放大倍数
	
	MotorPID_KI[0] = LDPY_UART5_GetChar();		//接收KI高字节
	MotorPID_KI[1] = LDPY_UART5_GetChar();		//接收KI低字节
	MotorPID_KI[2] = LDPY_UART5_GetChar();		//接收放大倍数
	
	MotorPID_KD[0] = LDPY_UART5_GetChar();		//接收KD高字节
	MotorPID_KD[1] = LDPY_UART5_GetChar();		//接收KD低字节
	MotorPID_KD[2] = LDPY_UART5_GetChar();		//接收放大倍数
	
  	Motor_info->KP = (float)(((uint16)MotorPID_KP[0] << 8) | MotorPID_KP[1])/(pow(10,MotorPID_KP[2])); 
	Motor_info->KP_Mult = MotorPID_KP[2];
	Motor_info->KI = (float)(((uint16)MotorPID_KI[0] << 8) | MotorPID_KI[1])/(pow(10,MotorPID_KI[2])); 
	Motor_info->KI_Mult = MotorPID_KI[2];
	Motor_info->KD = (float)(((uint16)MotorPID_KD[0] << 8) | MotorPID_KD[1])/(pow(10,MotorPID_KD[2])); 
  	Motor_info->KD_Mult = MotorPID_KD[2];
	
}

/*************************************************************************
*					零		度		偏		移  
*
*  函数名称：myLabVIEW_GetSpeedSettingNews
*  功能说明：接收各种路况下的速度设定
*  参数说明：
*  函数返回：无
*  修改时间：2014-06-21
*  备    注：
*************************************************************************/
void myLabVIEW_GetSpeedSettingNews(Speed_Info *Speed_info)
{
  	uint8 Speed[10] = {0};//用于接收设定的速度（两个字节）
	
	Speed[0] = LDPY_UART5_GetChar();			//接收直道速度高字节
	Speed[1] = LDPY_UART5_GetChar();			//接收直道速度低字节
	
	Speed[2] = LDPY_UART5_GetChar();			//接收普通弯道速度高字节
	Speed[3] = LDPY_UART5_GetChar();			//接收普通弯道速度低字节	
	
	Speed[4] = LDPY_UART5_GetChar();			//接收小S弯道速度高字节
	Speed[5] = LDPY_UART5_GetChar();			//接收小S弯道速度低字节	
	
	Speed[6] = LDPY_UART5_GetChar();			//接收调头区域速度高字节
	Speed[7] = LDPY_UART5_GetChar();			//接收调头区域速度低字节
	
	Speed[8] = LDPY_UART5_GetChar();			//接收路障速度高字节
	Speed[9] = LDPY_UART5_GetChar();			//接收路障速度低字节
	
	Speed_info->Straight_Speed  = ((uint16)Speed[0] << 8) | Speed[1];
	Speed_info->Cur_Speed 	    = ((uint16)Speed[2] << 8) | Speed[3];
	Speed_info->Snake_Speed     = ((uint16)Speed[4] << 8) | Speed[5];
	Speed_info->TurnRound_Speed = ((uint16)Speed[6] << 8) | Speed[7];
	Speed_info->Obstacle_Speed  = ((uint16)Speed[8] << 8) | Speed[9];
  
}

/*************************************************************************
*					零		度		偏		移  
*
*  函数名称：myLabVIEW_Send_Uint16
*  功能说明：发送双字节数据
*  参数说明：
*  函数返回：无
*  修改时间：2014-06-21
*  备    注：先发送低8位，再发送高8位
*************************************************************************/
void myLabVIEW_Send_Uint16(uint16 num)
{
  char x;
  uint16 uint16_num = num;
  
  x = uint16_num & 0xFF;//低8位
  LDPY_UART5_PutChar(x);
  
  x = (uint16_num & 0xFF00)>>8;//高8位
  LDPY_UART5_PutChar(x);
}

/*************************************************************************
*					零		度		偏		移   
*
*  函数名称：myLabVIEW_Send_Uint32
*  功能说明：发送四字节数据
*  参数说明：
*  函数返回：无
*  修改时间：2014-06-21
*  备    注：
*************************************************************************/
void myLabVIEW_Send_Uint32(uint32 num)
{
  char x;
  uint32 uint32_num = num;
  
  x = uint32_num & 0xFF;//低8位
  LDPY_UART5_PutChar(x);
  
  x = (uint32_num & 0xFF00)>>8;//中下8位
  LDPY_UART5_PutChar(x);
  
  x = (uint32_num & 0xFF0000)>>8;//中上8位
  LDPY_UART5_PutChar(x);
  
  x = (uint32_num & 0xFF000000)>>8;//高8位
  LDPY_UART5_PutChar(x);
  
}

/*************************************************************************
*					零		度		偏		移   
*
*  函数名称：myLabVIEW_Send_Float
*  功能说明：将float型的数据发送给上位机
*  参数说明：输入为要发送的float型数据和放大倍数
*  函数返回：无
*  修改时间：2014-06-21
*  备    注：
*************************************************************************/
void myLabVIEW_Send_Float(float Float_In,uint8 Mult)
{
	  uint16 Data = 0;
	  
	  Data = (uint16)(Float_In*pow(10,Mult)); 
	  
	  myLabVIEW_Send_Uint16(Data);			//发送放大后数据
	  LDPY_UART5_PutChar(Mult);				//发送放大倍数
  
}

/*************************************************************************
*					零		度		偏		移  
*
*  函数名称：myLabVIEW_ParameterSet
*  功能说明：设置小车的参数，包括PID，CCD高度，前瞻，曝光时间,
*  	     舵机的初始PID,电机的初始PID各种路况的速度等,
*	     并将设置信息发送给上位机。
*  参数说明：
*  函数返回：无
*  修改时间：2014-06-21
*  备    注：
*************************************************************************/
void myLabVIEW_ParametersSet(Parameter_Info *Parameter_info,CCD_Info *CCD_info,Steer_Info *Steer_info,Motor_Info *Motor_info,Speed_Info *Speed_info)
{   
	uint8 SpeedCalculate_Enable = 0;			//存放电机开/闭环开关
	
	uint8 Speed[10] = {0};						//接收设定的速度
  	
	uint8 SteerPID_KP[3] = {0};					//接收舵机PID参数（各两个字节）放大倍数（一个字节）
	uint8 SteerPID_KI[3] = {0};
	uint8 SteerPID_KD[3] = {0};
	
	uint8 MotorPID_KP[3] = {0};					//接收电机PID参数（各两个字节）放大倍数（一个字节）
	uint8 MotorPID_KI[3] = {0};
	uint8 MotorPID_KD[3] = {0};
	
	SpeedCalculate_Enable = LDPY_UART5_GetChar();	//接收速度计算开关
	
	SteerPID_KP[0] = LDPY_UART5_GetChar();			//接收KP高字节
	SteerPID_KP[1] = LDPY_UART5_GetChar();			//接收KP低字节
	SteerPID_KP[2] = LDPY_UART5_GetChar();			//接收放大倍数
	
	SteerPID_KI[0] = LDPY_UART5_GetChar();			//接收KI高字节
	SteerPID_KI[1] = LDPY_UART5_GetChar();			//接收KI低字节
	SteerPID_KI[2] = LDPY_UART5_GetChar();			//接收放大倍数
		
	SteerPID_KD[0] = LDPY_UART5_GetChar();			//接收KD高字节
	SteerPID_KD[1] = LDPY_UART5_GetChar();			//接收KD低字节
	SteerPID_KD[2] = LDPY_UART5_GetChar();			//接收放大倍数
	
	MotorPID_KP[0] = LDPY_UART5_GetChar();			//接收KP高字节
	MotorPID_KP[1] = LDPY_UART5_GetChar();			//接收KP低字节
	MotorPID_KP[2] = LDPY_UART5_GetChar();			//接收放大倍数
	
	MotorPID_KI[0] = LDPY_UART5_GetChar();			//接收KI高字节
	MotorPID_KI[1] = LDPY_UART5_GetChar();			//接收KI低字节
	MotorPID_KI[2] = LDPY_UART5_GetChar();			//接收放大倍数
	
	MotorPID_KD[0] = LDPY_UART5_GetChar();			//接收KD高字节
	MotorPID_KD[1] = LDPY_UART5_GetChar();			//接收KD低字节
	MotorPID_KD[2] = LDPY_UART5_GetChar();			//接收放大倍数
	
	Speed[0] = LDPY_UART5_GetChar();				//接收直道速度高字节
	Speed[1] = LDPY_UART5_GetChar();				//接收直道速度低字节
	
	Speed[2] = LDPY_UART5_GetChar();				//接收普通弯道速度高字节
	Speed[3] = LDPY_UART5_GetChar();				//接收普通弯道速度低字节	
	
	Speed[4] = LDPY_UART5_GetChar();				//接收小S弯道速度高字节
	Speed[5] = LDPY_UART5_GetChar();				//接收小S弯道速度低字节	
	
	Speed[6] = LDPY_UART5_GetChar();				//接收调头区域速度高字节
	Speed[7] = LDPY_UART5_GetChar();				//接收调头区域速度低字节
	
	Speed[8] = LDPY_UART5_GetChar();				//接收路障速度高字节
	Speed[9] = LDPY_UART5_GetChar();				//接收路障速度低字节
	
  	Steer_info->KI = (float)(((uint16)SteerPID_KP[0] << 8) | SteerPID_KP[1])/(pow(10,SteerPID_KP[2])); 
	Steer_info->KP_Mult = SteerPID_KP[2];
	Steer_info->KI = (float)(((uint16)SteerPID_KI[0] << 8) | SteerPID_KI[1])/(pow(10,SteerPID_KI[2])); 
	Steer_info->KI_Mult = SteerPID_KI[2];
	Steer_info->KI = (float)(((uint16)SteerPID_KD[0] << 8) | SteerPID_KD[1])/(pow(10,SteerPID_KD[2])); 
	Steer_info->KD_Mult = SteerPID_KD[2];
	
	Motor_info->KP = (float)(((uint16)MotorPID_KP[0] << 8) | MotorPID_KP[1])/(pow(10,MotorPID_KP[2])); 
	Motor_info->KP_Mult = MotorPID_KP[2];
	Motor_info->KI = (float)(((uint16)MotorPID_KI[0] << 8) | MotorPID_KI[1])/(pow(10,MotorPID_KI[2])); 
	Motor_info->KI_Mult = MotorPID_KI[2];
	Motor_info->KD = (float)(((uint16)MotorPID_KD[0] << 8) | MotorPID_KD[1])/(pow(10,MotorPID_KD[2])); 
  	Motor_info->KD_Mult = MotorPID_KD[2];
	
	Speed_info->Straight_Speed  = ((uint16)Speed[0] << 8) | Speed[1];
	Speed_info->Cur_Speed 	    = ((uint16)Speed[2] << 8) | Speed[3];
	Speed_info->Snake_Speed     = ((uint16)Speed[4] << 8) | Speed[5];
	Speed_info->TurnRound_Speed = ((uint16)Speed[6] << 8) | Speed[7];
	Speed_info->Obstacle_Speed  = ((uint16)Speed[8] << 8) | Speed[9];
 
	if(1 == SpeedCalculate_Enable)//电机闭环
	{
		Speed_info->SpeedCalculate_Enable = TRUE;
	}
	else//电机开环
	{
		Speed_info->SpeedCalculate_Enable =	FALSE;
	}
}

/*************************************************************************
*					零		度		偏		移  
*
*  函数名称：myLabVIEW_SendCCDValueOfTwoCCDs
*  功能说明：发送CCD的相关数据给上位机，包括CCD数据，曝光时间，平均电压等
*  参数说明：
*  函数返回：无
*  修改时间：2014-06-21
*  备    注：
*************************************************************************/
void myLabVIEW_SendCCDValueOfTwoCCDs(CCD_Info *CCD1_info,CCD_Info *CCD2_info)
{
	uint8 ii;
	
	/*-----------------------发送CCD1采集的数据-----------------------*/
	LDPY_UART5_PutChar('C');//帧头，大写C
	for(ii = 0; ii < 128; ii++)
	{	
		LDPY_UART5_PutChar((uint8)(CCD1_info->Pixel[ii] >> 4));	//发送滤波后的数据
	}
	
	/*-----------------------发送CCD2采集的数据-----------------------*/
	LDPY_UART5_PutChar('c');//帧头，小写c
	for(ii = 0; ii < 128; ii++)
	{	
		LDPY_UART5_PutChar((uint8)(CCD2_info->Pixel[ii] >> 4));	//发送滤波后的数据
	}
	
	/*--------------------发送CCD1数据处理后的各种值--------------------*/
	LDPY_UART5_PutChar('R');							//帧头
	LDPY_UART5_PutChar(CCD1_info->LeftLine[0]);			//左边界点
	LDPY_UART5_PutChar(CCD1_info->RightLine[0]);		//右边界点
	LDPY_UART5_PutChar(CCD1_info->CentralLine[0] + 64);	//中线点
	LDPY_UART5_PutChar(CCD1_info->LineError[0] + 1000);	//中点偏差，加上100是为了转换为正数发送，上位机应该减去64

	/*--------------------发送CCD2数据处理后的各种值--------------------*/
	LDPY_UART5_PutChar('r');							//帧头
	LDPY_UART5_PutChar(CCD2_info->LeftLine[0]);			//左边界点
	LDPY_UART5_PutChar(CCD2_info->RightLine[0]);		//右边界点
	LDPY_UART5_PutChar(CCD2_info->CentralLine[0] + 64);	//中线点
	LDPY_UART5_PutChar(CCD2_info->LineError[0] + 1000);	//中点偏差，加上100是为了转换为正数发送，上位机应该减去64
	
	LDPY_UART5_PutChar('O');							//发送数据保存命令
	
}

/*************************************************************************
*					零		度		偏		移  
*
*  函数名称：myLabVIEW_SendCCDBianaryValue
*  功能说明：发送CCD二值化后的相关数据给上位机，包括CCD数据，曝光时间，平均电压等
*  参数说明：
*  函数返回：无
*  修改时间：2014-06-21
*  备    注：压缩后的128个数据先发送低字节(0)，再发送高字节(127)
*************************************************************************/
void myLabVIEW_SendCCDBianaryValue(CCD_Info *CCD1_info,CCD_Info *CCD2_info,CCD_Info *CCD3_info)
{
	uint8 ii = 0;
	
	//压缩CCD二值化后的数据
	for(ii = 0; ii < 16; ii++)
	{
		CCD1_info->CCD_PhotoValue[ii] = 	(CCD1_info->PixelBinary[7 + 8*ii] << 7)
										  | (CCD1_info->PixelBinary[6 + 8*ii] << 6)
										  | (CCD1_info->PixelBinary[5 + 8*ii] << 5)
										  | (CCD1_info->PixelBinary[4 + 8*ii] << 4)
										  | (CCD1_info->PixelBinary[3 + 8*ii] << 3)
										  | (CCD1_info->PixelBinary[2 + 8*ii] << 2)
										  | (CCD1_info->PixelBinary[1 + 8*ii] << 1)
										  | (CCD1_info->PixelBinary[0 + 8*ii])
										;
	
		CCD2_info->CCD_PhotoValue[ii] = 	(CCD2_info->PixelBinary[7 + 8*ii] << 7)
										  | (CCD2_info->PixelBinary[6 + 8*ii] << 6)
										  | (CCD2_info->PixelBinary[5 + 8*ii] << 5)
										  | (CCD2_info->PixelBinary[4 + 8*ii] << 4)
										  | (CCD2_info->PixelBinary[3 + 8*ii] << 3)
										  | (CCD2_info->PixelBinary[2 + 8*ii] << 2)
										  | (CCD2_info->PixelBinary[1 + 8*ii] << 1)
										  | (CCD2_info->PixelBinary[0 + 8*ii])
										;
		
		CCD3_info->CCD_PhotoValue[ii] = 	(CCD3_info->PixelBinary[7 + 8*ii] << 7)
										  | (CCD3_info->PixelBinary[6 + 8*ii] << 6)
										  | (CCD3_info->PixelBinary[5 + 8*ii] << 5)
										  | (CCD3_info->PixelBinary[4 + 8*ii] << 4)
										  | (CCD3_info->PixelBinary[3 + 8*ii] << 3)
										  | (CCD3_info->PixelBinary[2 + 8*ii] << 2)
										  | (CCD3_info->PixelBinary[1 + 8*ii] << 1)
										  | (CCD3_info->PixelBinary[0 + 8*ii])
										;
	}
	
	//-------------------发送3个CCD的压缩后的数据--------------------//
	//CCD1
	LDPY_UART5_PutChar('G');
	LDPY_UART5_PutChar(CCD1_info->CCD_PhotoValue[0]);
	LDPY_UART5_PutChar(CCD1_info->CCD_PhotoValue[1]);
	LDPY_UART5_PutChar(CCD1_info->CCD_PhotoValue[2]);
	LDPY_UART5_PutChar(CCD1_info->CCD_PhotoValue[3]);
	LDPY_UART5_PutChar(CCD1_info->CCD_PhotoValue[4]);
	LDPY_UART5_PutChar(CCD1_info->CCD_PhotoValue[5]);
	LDPY_UART5_PutChar(CCD1_info->CCD_PhotoValue[6]);
	LDPY_UART5_PutChar(CCD1_info->CCD_PhotoValue[7]);
	LDPY_UART5_PutChar(CCD1_info->CCD_PhotoValue[8]);
	LDPY_UART5_PutChar(CCD1_info->CCD_PhotoValue[9]);
	LDPY_UART5_PutChar(CCD1_info->CCD_PhotoValue[10]);
	LDPY_UART5_PutChar(CCD1_info->CCD_PhotoValue[11]);
	LDPY_UART5_PutChar(CCD1_info->CCD_PhotoValue[12]);
	LDPY_UART5_PutChar(CCD1_info->CCD_PhotoValue[13]);
	LDPY_UART5_PutChar(CCD1_info->CCD_PhotoValue[14]);
	LDPY_UART5_PutChar(CCD1_info->CCD_PhotoValue[15]);
	
	//CCD2
	LDPY_UART5_PutChar('g');
	LDPY_UART5_PutChar(CCD2_info->CCD_PhotoValue[0]);
	LDPY_UART5_PutChar(CCD2_info->CCD_PhotoValue[1]);
	LDPY_UART5_PutChar(CCD2_info->CCD_PhotoValue[2]);
	LDPY_UART5_PutChar(CCD2_info->CCD_PhotoValue[3]);
	LDPY_UART5_PutChar(CCD2_info->CCD_PhotoValue[4]);
	LDPY_UART5_PutChar(CCD2_info->CCD_PhotoValue[5]);
	LDPY_UART5_PutChar(CCD2_info->CCD_PhotoValue[6]);
	LDPY_UART5_PutChar(CCD2_info->CCD_PhotoValue[7]);
	LDPY_UART5_PutChar(CCD2_info->CCD_PhotoValue[8]);
	LDPY_UART5_PutChar(CCD2_info->CCD_PhotoValue[9]);
	LDPY_UART5_PutChar(CCD2_info->CCD_PhotoValue[10]);
	LDPY_UART5_PutChar(CCD2_info->CCD_PhotoValue[11]);
	LDPY_UART5_PutChar(CCD2_info->CCD_PhotoValue[12]);
	LDPY_UART5_PutChar(CCD2_info->CCD_PhotoValue[13]);
	LDPY_UART5_PutChar(CCD2_info->CCD_PhotoValue[14]);
	LDPY_UART5_PutChar(CCD2_info->CCD_PhotoValue[15]);
	
	//CCD3
	LDPY_UART5_PutChar('H');
	LDPY_UART5_PutChar(CCD3_info->CCD_PhotoValue[0]);
	LDPY_UART5_PutChar(CCD3_info->CCD_PhotoValue[1]);
	LDPY_UART5_PutChar(CCD3_info->CCD_PhotoValue[2]);
	LDPY_UART5_PutChar(CCD3_info->CCD_PhotoValue[3]);
	LDPY_UART5_PutChar(CCD3_info->CCD_PhotoValue[4]);
	LDPY_UART5_PutChar(CCD3_info->CCD_PhotoValue[5]);
	LDPY_UART5_PutChar(CCD3_info->CCD_PhotoValue[6]);
	LDPY_UART5_PutChar(CCD3_info->CCD_PhotoValue[7]);
	LDPY_UART5_PutChar(CCD3_info->CCD_PhotoValue[8]);
	LDPY_UART5_PutChar(CCD3_info->CCD_PhotoValue[9]);
	LDPY_UART5_PutChar(CCD3_info->CCD_PhotoValue[10]);
	LDPY_UART5_PutChar(CCD3_info->CCD_PhotoValue[11]);
	LDPY_UART5_PutChar(CCD3_info->CCD_PhotoValue[12]);
	LDPY_UART5_PutChar(CCD3_info->CCD_PhotoValue[13]);
	LDPY_UART5_PutChar(CCD3_info->CCD_PhotoValue[14]);
	LDPY_UART5_PutChar(CCD3_info->CCD_PhotoValue[15]);

}
/*---------------------------end of myLabVIEW.c-------------------------------------*/
