#include "Handle.h"





//大环路的检测，还有是否初始时左右边界的赋值，赋值后是否可以，需要验证，留个记号






#define Display_OFF_ON 1    //补线显示开关，1为开 0为关
#define SSSSSSS_OFF_ON 1    //小S检测开关，1为开 0为关

uint8 colour[2] = {255, 0},black=1,white=0; //0 和 1 分别对应的颜色
//注：山外的摄像头 0 表示 白色，1表示 黑色
////////////////////0       2       4       6       8




uchar Width_Max[61]={5,5,5,5,5,5,  5,5,5,5,5,
                        5,5,5,5,5,  5,5,5,5,5,                         
                         12,12,15,15,17,  17,18,18,20,20,                
                             23,23,25,25,26,  26,29,29,31,31,             
                                33,33,35,35,36,  36,38,38,41,41,          
                                    42,42,43,43,46, 46,48,48,50,50};


//uchar Weight[60] = {1,  1,  1,  1,  1,  2,  3,  4,  4,  4,       //50
//                    5,  6,  9, 10, 11, 12, 13, 15, 12, 13,     //40
//                   11, 12,  6,  5,  4,  3,  4,  3,  3,  2,      //30
//                    1,  1,  1,  1,  1,  1,  1,  1,  1,  1, //20
//                    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
//                    1,  1,  1,  1,  1,  1,  1,  1,  1,  1};	//加权平均参数

//uchar Weight[60] = {1,  1,  4,  1,  6,  2,  6,  4,  6,  4,       //50
//                    5,  6,  9, 10, 11, 12, 13, 15, 12, 13,     //40
//                   11, 12,  6,  5,  4,  3,  4,  3,  3,  2,      //30
//                    1,  1,  1,  1,  1,  1,  1,  1,  1,  1, //20
//                    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
//                    1,  1,  1,  1,  1,  1,  1,  1,  1,  1};	//加权平均参数

//uchar Weight[60] = {1,  1,  2,  1,  4,  2,  4,  4,  4,  4,       //50
//                    5,  6,  9, 10, 11, 12, 13, 15, 12, 13,     //40
//                   11, 12,  6,  5,  4,  3,  4,  3,  3,  2,      //30
//                    1,  1,  1,  1,  1,  1,  1,  1,  1,  1, //20
//                    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
//                    1,  1,  1,  1,  1,  1,  1,  1,  1,  1};	//加权平均参数







uchar Weight[60] = {  1,  2,  1,  4,  2,  4,  4,  4,  4,       //50比赛
                    5,  6,  9, 10, 11, 12, 13, 15, 12, 13,     //40
                   11, 12,  6,  5,  4,  3,  4,  3,  3,  2,      //30
                    1,  1,  1,  1,  1,  1,  1,  1,  1,  1, //20
                    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
                    1,  1,  1,  1,  1,  1,  1,  1,1,  1,  1};	//加权平均参数




//uchar Weight[60] = {  1,  2,  1,  4,  2,  4,  4,  4,  4,       //50比赛
//                    5,  6,  9, 10, 11, 12, 13, 13, 12, 11,     //40
//                   11, 10,  6,  5,  4,  3,  4,  3,  3,  2,      //30
//                    1,  1,  1,  1,  1,  1,  1,  1,  1,  1, //20
//                    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
//                    1,  1,  1,  1,  1,  1,  1,  1,1,  1,  1};	//加权平均参数







//uchar Weight[60] = {  2,  1,  4,  2,  4,  4,  4,  4,       //50
//                    5,  6,  9, 10, 11, 12, 13, 15, 12, 13,     //40
//                   11, 12,  6,  5,  4,  3,  4,  3,  3,  2,      //30
//                    1,  1,  1,  1,  1,  1,  1,  1,  1,  1, //20
//                    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
//                    1,  1,  1,  1,  1,  1,  1,  1,1, 1,   1,  1};	//加权平均参数
//
//uchar Weight[60] = {  1,  1,  2,  3,  3,  4,  4,  4,  5,  6,     //50
//                      9, 10, 11, 12, 13, 15, 12, 13,  11,  10,  //40
//                      9,  8,  7,  6,  5,  4,  3,  3,  2,  1, //30
//                      1,  1,  1,  1,  1,  1,  1,  1,  1,  1,//20
//                      1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
//                      1,  1,  1,  1,  1,  1,  1,  1,  1,  1};	//加权平均参数
 


//uchar Weight[60] = {48, 47, 46, 45, 44,   43, 42, 41, 40, 38,
//                    37, 36, 35, 34, 33,   32, 31, 30, 29, 28,
//				    27, 26, 25, 24, 23,   22, 21, 20, 19, 18,
//                    17, 16, 15, 14, 13,   12, 11, 10, 9,  8,
//					7,  6,   5,  5,  5,    5,  5,  5, 5,  5,
//					5,  5,   5,  5,  5,    5,  5,  5, 5,  5};	//加权平均参数

//uchar Weight[60] = {  1, 1, 2,  3,  4,  4,  4,  5,  6,      //50
//                    9, 10, 11, 12, 13, 15, 12, 13, 11, 12,    //40
//                    6,  5,  4,  3,  4,  3,  3,  2,   1,  1,    //30
//                    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,//20
//                    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
//                    1,  1,  1,  1,  1,1,   1,  1,  1,  1,  1};	//加权平均参数

//uchar Weight[60] = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5,      //50
//                    3, 3, 3, 3, 3, 3, 3, 3, 3, 3,    //40
//                    3, 3, 3, 3, 3, 3, 3, 3, 3, 3,    //30
//                    
//                    3, 3, 3, 3, 3, 3, 3, 3, 3, 3,//20
//                    5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
//                    1,  1,  1,  1,  1,  1,  1,  1,  1,  1};	//加权平均参数
//uchar Weight[60] = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5,      //50
//                    5, 5, 5, 5, 5, 5, 5, 5, 5, 5,    //40
//                    5, 5, 5, 5, 5, 5, 5, 5, 5, 5,    //30
//                    5, 5, 5, 5, 5, 5, 5, 5, 5, 5,//20
//                    5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
//                    1,  1,  1,  1,  1,  1,  1,  1,  1,  1};	//加权平均参数

//uchar Weight[60] = {  3,  4,  4,  4,  5,  6,  9, 10, 11,  12,    //50
//                      13, 15, 12, 13, 11, 12, 6,  5,  4,   3, //40
//                       4,  3,  3,  2,  1,  1,  1,  1,  1,  1,  //30
//                       1,  1,  1,  1,  1,  1,  1,  1,  1,  1,//20
//                       1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
//                       1,  1,  1,  1,  1,  1,  1,  1,  1,  1};	//加权平均参数

//uchar Weight[60] = {                  5,  6,  9, 10, 11,  12,    //50
//                      13, 15, 12, 13, 11, 12, 6,  5,  4,   3, //40
//                       4,  3,  3,  2,  1,  1,  1,  1,  1,  1,  //30
//                       1,  1,  1,  1,  1,  1,  1,  1,  1,  1,//20
//                       1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
//                       1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
//                       1,  1,  1,  1                       };	//加权平均参数

//uchar Weight[60] = {1,  1,  1,  1,  1,  1,  2,  2,  3,    3,  4,  4,  5,  5,
//                    6,  6,  4,  4,  2,    2,  1,  1,  1,  1,
//				    1,  1,  1,  1,  1,    1,  1,  1,  1,  1,
//                    1,  1,  1,  1,  1,    1,  1,  1,  1,  1,
//					1,  1,  1,  1,  1,    1,  1,  1,  1,  1,
//					1,  1,  1,  1,  1,    1,              };	//加权平均参数

//uchar Weight[60] = {1,  1,  1,  1,  1,  1,  1,  2,  2,  3,    3,  4,  4,  5,  5,
//                    6,  6,  4,  4,  2,    2,  1,  1,  1,  1,
//				    1,  1,  1,  1,  1,    1,  1,  1,  1,  1,
//                    1,  1,  1,  1,  1,    1,  1,  1,  1,  1,
//					1,  1,  1,  1,  1,    1,  1,  1,  1,  1,
//					1,  1,  1,  1,  1               };	//加权平均参数

uchar Left_Line[62], Right_Line[62], Mid_Line[62];	// 原始左右边界数据
uchar Left_Add_Line[62], Right_Add_Line[62];		// 左右边界补线数据
uchar Left_Add_Flag[62], Right_Add_Flag[62];		// 左右边界补线标志位
uchar Width_Real[62];	// 实际赛道宽度
uchar Width_Add[62];	// 补线赛道宽度
uchar Width_Min;		// 最小赛道宽度

uchar S_temp;
int8 Ramp_Error[10];    //坡道宽度误差

uchar Ek_compar;         //两行中间值与两行的中行的Mid_Line的差值
uchar Foresight,Last_Foresight,Ramp_Flag_Last=0;	// 前瞻，用于速度控制
uchar Out_Side = 0;	// 丢线控制
uchar Line_Count;	// 记录成功识别到的赛道行数

uchar Left_Add_Start, Right_Add_Start;	// 左右补线起始行坐标
uchar Left_Add_Stop, Right_Add_Stop;	// 左右补线结束行坐标
float Left_Ka = 0, Right_Ka = 0;
float Left_Kb = 1, Right_Kb = 79;	// 最小二乘法参数

uchar Left_Hazard_Flag, Right_Hazard_Flag, Hazard_Flag;	// 左右障碍物标志
uint8 Hazard_Time;
uchar Left_Max, Right_Min;

uchar Starting_Line_Flag = 0;	// 起跑线标志位
uchar Starting_Line_Times = 0;	// 起跑线标志位
uchar Starting_Line_Time = 0;	// 起跑线标志位
char Last_Point = 40;

/************** 环路相关变量 *************/
uchar Annu_OFF_ON=0;
uchar Annulus_Flag=0;
uchar Annulus_Mid=0;
uchar Annulus_Left;
uchar Annulus_Right;
uchar Annulus_Count[4] = {0, 1, 0, 1};	         // 1右环路，0左环路
uint16 Annulus_Delay[4] = {120, 130, 120, 130};	// 环路延时时间
uchar Annulus_Times = 0;	// 环路次数
uchar Annulus2_Delay = 0;
uint16 Annulus_DelayMs = 0;	// 检测到环路后延时一段时间后自动清除环路标志位

uint8  Annulus_Nums=0;
uint8  Annulus_Small=0;
uchar Annulus_Mode = 0;	

uchar Annulus_Num_Count=0;
/************** 环路相关变量 *************/


void Annulus_Control(void)
{

		Annulus_Right = Annulus_Count[Annulus_Times];
		Annulus_Left = !Annulus_Right;
		if (Annulus_DelayMs>Annulus_Delay[Annulus_Times])
                {
                   Annulus_Flag=0;
                   Annulus_DelayMs=0;
                   Annulus_Times++;
                   if (Annulus_Times > 3)
                   {
			Annulus_Times = 0;
                        
                   }
                }
		

}


void Annulus2_Control(void)
{
	Annulus_Right = Annulus_Count[Annulus_Times++];
	Annulus_Left = !Annulus_Right;
	
	Annulus_Mode = 1;	// 开始进入环路
	Annulus_Flag = 1;	// 环路标志位置位
	
    
    led(LED_MAX,LED_ON);// 打开LED指示灯		全部初始化成功
    
	if (Annulus_Times >= 4)
	{
		Annulus_Times = 0;
	}
}

/*
*	环路图像处理算法
*
*	说明：环路专用，不含其他元素
*/
void Annulus2_Handle(uchar *data)
{
	uchar i;	// 控制行
	uchar res;
	
	Line_Count = 0;	// 赛道行数复位
	
	Left_Add_Start = 0;		// 复位补线起始行坐标
	Right_Add_Start = 0;
	Left_Add_Stop = 0;
	Right_Add_Stop = 0;
	
	/***************************** 第一行特殊处理 *****************************/
	res = First_Line_Handle(data);
	if (res == 0)
	{
		Out_Side++;	// 丢线
		return;
	}
	if (Left_Add_Start || Right_Add_Start)
	{
		Width_Min = 60;
	}
	/*************************** 第一行特殊处理结束 ***************************/
	
	for (i = 59; i >= 15;)
	{
		i -= 2;
		
		if (!data[i*80 + Mid_Line[i+2]])//前2行中点在本行为黑点，赛道结束
		{ 
			i += 2;
			break;
		}
		else
		{
			Traversal_Mid_Line(i, data, Mid_Line[i+2], 1, 78, Left_Line, Right_Line, Left_Add_Line, Right_Add_Line);
			/************************** 补线检测开始 **************************/
			if (Width_Real[i] >= Width_Min)	// 赛道宽度变宽,进入环路
			{	
				if (Annulus_Left)	// 左环路
				{
					Left_Add_Line[i] = Left_Line[i];
                                        Right_Add_Line[i]=Left_Add_Line[i] + (Width_Max[i]+11);  //加数变宽
                                        Right_Add_Line[i]=range_protect(Right_Add_Line[i],0,79);
//                    Right_Add_Line[i]=Left_Line[i] + (Width_Min-1);
					
				}
				else				// 右环路
				{
					Right_Add_Line[i] = Right_Line[i];
                                        Left_Add_Line[i]=range_protect((Right_Line[i] - (Width_Max[i]+11)),0,79);
//                    Left_Add_Line[i]=Right_Line[i] - (Width_Min-1);
				}
			}
			/************************** 补线检测结束 **************************/
			Mid_Line[i] = (Right_Add_Line[i] + Left_Add_Line[i]) / 2;
			Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	
			if (Width_Add[i] < Width_Min)
			{
				Width_Min = Width_Add[i];
			}
			if (Width_Add[i] <= 6)
			{
				i += 2;
				break;
			}
		}
	}
	Line_Count = i;

#if   Display_OFF_ON	
	for (i = 61; i >= Line_Count+2;)
	{
		i -= 2;
		/**//************************* 上位机显示边界 *************************/
		/**/data[i*80 + Left_Add_Line[i] + 2] = 0;	// 上位机显示补线后的左边界，不用时屏蔽
		/**/data[i*80 + Right_Add_Line[i] - 2] = 0;	// 上位机显示补线后的右边界，不用时屏蔽
		/**/data[i*80 + Mid_Line[i]] = 0;			// 上位机显示中线，不用时屏蔽
//		/**/data[i*80 + Left_Line[i] + 2] = 0;		// 上位机显示原始左边界，不用时屏蔽
//		/**/data[i*80 + Right_Line[i] - 2] = 0;		// 上位机显示原始右边界，不用时屏蔽
		/**//************************* 上位机显示边界 *************************/
	}
#endif	
	/**************************** 环路出入状态判断 ****************************/
	if (Annulus_Left)	// 左环路
	{
		if (Right_Add_Flag[59])		// 最底行需要补线
		{
            Annulus_Num_Count++;
            if(Annulus_Num_Count>0)
            {
                if (Annulus_Mode == 1)	// 上次状态为开始进入环路
                {
                    Annulus_Mode = 2;	// 当前状态为进入环路中
                }
                if (Annulus_Mode == 3)	// 上次状态为已成功进入环路
                {
                    if (Left_Add_Line[59] > Left_Add_Line[Line_Count+4])	// 左边界是向左延伸的，消除进环路误判
                    {
                        Annulus_Mode = 4;	// 当前状态为出环路中
                        Annulus2_Delay = 15;	// 最少延时1s
                    }
                }
            }
		}
		else if(!Right_Add_Flag[59])					// 最底行不需要补线
		{
            Annulus_Num_Count=0;
			if (Annulus_Mode == 2)	// 上次状态为进入环路中
			{
				Annulus_Mode = 3;	// 当前状态为已成功进入环路，准备出环路
			}
			if (Annulus_Mode == 4)	// 上次状态为出环路中
			{
				if (!Annulus2_Delay)	// 最少保持1s环路专用算法，防止误判
				{
					Annulus_Mode = 0;	// 本次状态为成功出环路，准备下次进入环路
					Annulus_Flag = 0;	// 清除环路状态标志位
                    Annulus_Small=0; 
                    
                    led(LED_MAX,LED_OFF);// 打开LED指示灯		全部初始化成功
                    
				}
			}
		}
	}
	else	// 右环路
	{
		if (Left_Add_Flag[59])		// 最底行需要补线
		{
            Annulus_Num_Count++;
            if(Annulus_Num_Count>0)
            {
                if (Annulus_Mode == 1)	// 上次状态为开始进入环路
                {
                    Annulus_Mode = 2;	// 当前状态为进入环路中
                }
                if (Annulus_Mode == 3)	// 上次状态为已成功进入环路
                {
                    if (Right_Add_Line[59] < Right_Add_Line[Line_Count+4])	// 右边界是向右延伸的，消除进环路误判
                    {
                        Annulus_Mode = 4;	// 当前状态为出环路中
                        Annulus2_Delay = 15;	// 最少延时1s
                    }
                }
            }
		}
		else if(!Left_Add_Flag[59])						// 最底行不需要补线
		{
            Annulus_Num_Count=0;
			if (Annulus_Mode == 2)	// 上次状态为进入环路中
			{
				Annulus_Mode = 3;	// 当前状态为已成功进入环路，准备出环路
			}
			if (Annulus_Mode == 4)	// 上次状态为出环路中
			{
				if (!Annulus2_Delay)	// 本次状态为成功出环路，准备下次进入环路
				{
					Annulus_Mode = 0;	// 本次状态为成功出环路，准备下次进入环路
					Annulus_Flag = 0;	// 清除环路状态标志位
                    Annulus_Small =0; 
                    
                    led(LED_MAX,LED_OFF);// 打开LED指示灯		全部初始化成功
				}
			}
		}
	}
	/************************** 环路出入状态判断结束 **************************/
}





/****************** 新算法 ******************/

/*
*	图像算法参数初始化
*
*	说明：仅影响第一行特殊处理
*/
void Image_Para_Init(void)
{	
	Mid_Line[61] = 40;
	Left_Line[61] = 1;
	Right_Line[61] = 79;
	Left_Add_Line[61] = 1;
	Right_Add_Line[61] = 79;
	Width_Real[61] = 78;
	Width_Add[61] = 78;
}




/*
*	图像处理算法
*
*	说明：处理普通图像，包括十字、障碍
*/
void Image_Handle(uchar *data)
{
	uchar i;	// 控制行
    uint8 j;
	uchar res,temps;	// 用于结果状态判断
	float Result;	// 用于结果状态判断
    uchar Add_Start_Max;
	uchar Weight_Left, Weight_Right;
	uchar Mid_Left, Mid_Right;
	uchar Limit_Left, Limit_Right;
	
	Line_Count = 0;	// 赛道行数复位
	Starting_Line_Times=0;
        Starting_Line_Time=0;
        Starting_Line_Flag=0;
	Left_Hazard_Flag = 0;	// 复位左右障碍物标志位
	Right_Hazard_Flag = 0;
	Hazard_Flag=0;
    
       
     
     
	Left_Add_Start = 0;		// 复位补线起始行坐标
	Right_Add_Start = 0;
	Left_Add_Stop = 0;
	Right_Add_Stop = 0;
	
	/***************************** 第一行特殊处理 *****************************/
	res = First_Line_Handle(data);
	if (res == 0)
	{
		Out_Side = 1;	// 丢线
		return;
	}
	Out_Side = 0;
	Line_Count = 59;
	/*************************** 第一行特殊处理结束 ***************************/
	
	for (i = 59; i >= 19;)	// 仅处理前40行图像，隔行后仅处理20行数据
	{
		i -= 2;	// 隔行处理，减小单片机负荷
		
		if (!data[i*80 + Mid_Line[i+2]])//前2行中点在本行为黑点，可能是赛道结束，也可能是环路
		{
                /**************************** 起跑线检测开始 ****************************/
                 if (i>30 && Annulus_Check && i<59)
                { 
                        Starting_Line_Times=0;
                        Starting_Line_Time=0;
                        if (Last_Point >35 && Last_Point < 45)
                        {
                                for(j=Left_Line[i+2];j<Right_Line[i+2];j++)
                                {
                                        
                                        if (Starting_Line_Time==0)
                                        {
                                                if(!data[80*(i-2)+j])
                                                {
                                                        Starting_Line_Times++;                                                        
                                                        Starting_Line_Time=1;
                                                }     
                                        }
                                        else
                                        {
                                               if(data[80*(i-2)+j])
                                                {
                                                        Starting_Line_Time=0;
                                                } 
                                        }
                                }
                                if(Starting_Line_Times>5 && Starting_Line_Times<15)
                                {
                                                Starting_Line_Flag=1;                                                
                                }

                        }
                  
                }
                

       if(Annu_OFF_ON==0)
       {
                                   /**************************** 苛刻的圆环检测，环境影响大时使用 ****************************/
                    if (Width_Real[i+2]>30 && Mid_Line[i+2] > 28 && Mid_Line[i+2] <52 && i<55 && Right_Add_Flag[i+2] && Left_Add_Flag[i+2])
                    {
                        if(((Width_Real[i+4] - Width_Real[i+6]) >=0) && ((Width_Real[i+4] - Width_Real[i+6]) <=6) && ((Left_Line[i+6] - Left_Line[i+4]) >=0) 
                            && ((Left_Line[i+6] - Left_Line[i+4]) <=6)&& ((Right_Line[i+4] - Right_Line[i+6])<=6) && ((Right_Line[i+4] - Right_Line[i+6])>=0) && Right_Add_Start && Left_Add_Start)
                        {
                            if(Annulus_Check && Last_Point<50 && Last_Point>30)
                            { 
                                    Starting_Line_Times=0;
                                    Starting_Line_Time=0;
                                    for(j=Left_Line[i+2];j<Right_Line[i+2];j++)
                                    {                                     
                                            if (Starting_Line_Time==0)
                                            {
                                                    if(data[80*(i-2)+j])
                                                    {
                                                            Starting_Line_Times++;
                                                            if (Starting_Line_Times>=3)
                                                            {
                                                                Starting_Line_Times=0;
                                                                Starting_Line_Time=1;
                                                            }                                                       
                                                    }     
                                            }
                                            else if(Starting_Line_Time==1)
                                            {
                                                   if(!data[80*(i-2)+j])
                                                    {
                                                            Starting_Line_Times++;
                                                            if (Starting_Line_Times>=3)
                                                            {
                                                                Starting_Line_Times=0;
                                                                Starting_Line_Time=2;
                                                            }                                                       
                                                    }                                                                                                    
                                            }
                                            else if(Starting_Line_Time==2)
                                            {
                                                   if(data[80*(i-2)+j])
                                                    {
                                                            Starting_Line_Times++;
                                                            if (Starting_Line_Times>=3)
                                                            {
                                                                Starting_Line_Times=0;
                                                                Starting_Line_Time=3;
                                                            }                                                       
                                                    } 
                                            }
                                    }
                                    if (Starting_Line_Time==3)
                                    {
                                      /********** 检测大小圆环程序 ***********/      
                                       Annulus_Nums=0;
                                       for(j=Left_Line[i+2];j<Right_Line[i+2];j++)
                                       {
                                            if(!data[80*(i-2)+j])
                                            {
                                                    Annulus_Nums++;
                                            }
                                       }
                                       if (Annulus_Nums>10 && Annulus_Nums<35)
                                       {
                                            Annulus_Small=1;
                                       }
                                       /********** 检测结束 ***********/  
                                          Annulus2_Control();    
                                    }
                                    Starting_Line_Time=0;
                               
                                }
                        }
                  }
        //环路检测结束
       }
       else
       {
                        /**************************** 简单的圆环检测，环境影响小时使用 ****************************/
                        if (Width_Real[i+2]>30 && i<50 && Right_Add_Flag[i+2] && Left_Add_Flag[i+2] && Right_Add_Start && Left_Add_Start && !Right_Add_Stop && !Left_Add_Stop)
                        {
                                if (Left_Add_Start >= Right_Add_Start)
                                {
                                        Add_Start_Max = Left_Add_Start;
                                }
                                else
                                {
                                        Add_Start_Max = Right_Add_Start;
                                }
                                if((Width_Real[i+2] > Width_Real[Add_Start_Max]) && ((Width_Real[i+2] - Width_Real[i+4]) <=10) && ((Left_Line[i+4] - Left_Line[i+2]) <=5) && ((Right_Line[i+2] - Right_Line[i+4])<=5))
                                {
                                        if(Annulus_Check)
                                        {
                                               
                                                /********** 检测大小圆环程序 ***********/      
                                               Annulus_Nums=0;
                                               for(j=Left_Line[i+2];j<Right_Line[i+2];j++)
                                               {
                                                    if(!data[80*(i-2)+j])
                                                    {
                                                            Annulus_Nums++;
                                                    }
                                               }
                                               if (Annulus_Nums>10 && Annulus_Nums<35)
                                               {
                                                    Annulus_Small=1;
                                               }
                                                /********** 检测结束 ***********/  
                                                Annulus2_Control();
                                        }
                                }
                       }
       }

			break;
		}
        else	// 使用前2行中点向两边扫描边界
		{
			Traversal_Mid_Line(i, data, Mid_Line[i+2], 1, 79, Left_Line, Right_Line, Left_Add_Line, Right_Add_Line);
		}


			
		/**************************** 补线检测开始 ****************************/
		if (Width_Real[i] > Width_Min+1)//&&(Left_Add_Flag[i]||Right_Add_Flag[i]))	// 赛道宽度变宽，可能是十字或环路
		{
			// 使用特殊算法进行突变点检测，杜绝十字漏判
//			Result = Calculate_Angle(Left_Add_Line[i+4], Left_Add_Line[i+2], Left_Add_Line[i]);	// 突变点检测
//			if (Result <= -3)	// -1：135°
//			{
//				Left_Add_Flag[i] = 1;
//			}
//			Result = Calculate_Angle(Right_Add_Line[i+4], Right_Add_Line[i+2], Right_Add_Line[i]);	// 突变点检测
//			if (Result >= 3)	// 1：135°
//			{
//				Right_Add_Flag[i] = 1;
//			}
//			if (i >= 55)	// 距离较近
//			{
//				if (Left_Add_Line[i] <  Left_Add_Line[i+2] && Right_Add_Line[i] >= Right_Add_Line[i+2])
//				{
//					Left_Add_Flag[i] = 1;	// 强制认定为需要补线
//				}
//				if (Right_Add_Line[i] > Right_Add_Line[i+2] && Left_Add_Line[i] <=  Left_Add_Line[i+2])
//				{
//					Right_Add_Flag[i] = 1;	// 强制认定为需要补线
//				}
//			}
                        
                       ////////////////////////////////////////////////
             if (Left_Add_Line[i] <=  Left_Add_Line[i+2])
			{
				if (!Left_Add_Flag[i])
				{
					Left_Add_Flag[i] = 1;	// 强制认定为需要补线
				}
			}
			if (Right_Add_Line[i] >= Right_Add_Line[i+2])
			{
				if (!Right_Add_Flag[i])
				{
					Right_Add_Flag[i] = 1;	// 强制认定为需要补线
				}
			}
			
			if (Left_Add_Flag[i] || Right_Add_Flag[i])
			{
				if (Left_Add_Stop || Right_Add_Stop)
				{
					break;
				}
			}
		}
		/**************************** 补线检测结束 ****************************/
		
		/**************************** 障碍检测开始 ****************************/
		else
		{
                /**************************** 起跑线检测开始 ****************************/
                if (i>30 && Annulus_Check && i<59)
                { 
                        Starting_Line_Times=0;
                        Starting_Line_Time=0;
                        if (Last_Point >35 && Last_Point < 45)
                        {
                                for(j=Left_Line[i+2];j<Right_Line[i+2];j++)
                                {
                                        
                                        if (Starting_Line_Time==0)
                                        {
                                                if(!data[80*(i-2)+j])
                                                {
                                                        Starting_Line_Times++;                                                        
                                                        Starting_Line_Time=1;
                                                }     
                                        }
                                        else
                                        {
                                               if(data[80*(i-2)+j])
                                                {
                                                        Starting_Line_Time=0;
                                                } 
                                        }
                                }
                                if(Starting_Line_Times>5 && Starting_Line_Times<15)
                                {
                                                Starting_Line_Flag=1;                                                
                                }

                        }
                  
                }
                /**************************** 起跑线检测结束 ****************************/
             if (i < 57 && i > 25 && Width_Real[i] < 50 && Annulus_Check && !Starting_Line_Flag && (Width_Real[i+2]-Width_Real[i])>6)
			{
				if (Right_Add_Line[i+2] - Right_Add_Line[i]<4 && (Left_Add_Line[i] > (Left_Add_Line[i+2] + 6)) && !Left_Add_Start)	// 本行与前一行左边界有较大突变且没有补线
				{        
                    temps=0;
                    res=0;
                    for (j=Left_Add_Line[i+2];j<Left_Add_Line[i]+1;j++) //j<可以优化
                    {
                        if(temps==0)
                        {
                            if(data[80*(i-2) + j] && !data[80*(i-2) + j+1])//搜索白点
                            {
                                temps=j;
                            }   
                        }
                        else
                        {
                            if(!data[80*(i-2) + j])
                            {
                                res++;
                            }
                        }
                    }
                    if(res>5 && res<15)
                    {
                        Hazard_Flag=temps;
                    }
                    if(Hazard_Flag && Left_Add_Line[i] - Hazard_Flag > 4) //后面判断可以不要
                    {
                        Left_Hazard_Flag = i;	// 障碍物标志位置位
//                        save_var(VAR3, Left_Hazard_Flag);
                    }

				}
				else if (Left_Add_Line[i] - Left_Add_Line[i+2] <4 && Right_Add_Line[i] < Right_Add_Line[i+2] - 6 && !Right_Add_Start)	// 本行与前一行右边界有较大突变且没有补线
				{
                    temps=0;
                    res=0;
                    for (j=Right_Add_Line[i+2];j>Right_Add_Line[i]-1;j--) //j<可以优化
                    {
                        if(temps==0)
                        {
                            if(data[80*(i-2) + j] && !data[80*(i-2) + j-1])//搜索白点
                            {
                                temps=j;
                            }   
                        }
                        else
                        {
                            if(!data[80*(i-2) + j])
                            {
                                res++;
                            }
                        }
                    }
                    if(res>5 && res<15)
                    {
                        Hazard_Flag=temps;
                    }
                    if(Hazard_Flag && Hazard_Flag - Right_Add_Line[i]> 4) //后面判断可以不要
                    {
                        Right_Hazard_Flag = i;	// 障碍物标志位置位
//                        save_var(VAR3, Right_Hazard_Flag);
                    }
				}
			}
		}
		/**************************** 障碍检测结束 ****************************/
		
		/*************************** 第一轮补线开始 ***************************/
		if (Left_Add_Flag[i])	// 左侧需要补线
		{
			if (i >= 55)	// 前三行补线不算
			{
				if (!Left_Add_Start)
				{
					Left_Add_Start = i;	// 记录补线开始行
					Left_Ka = 0;
					Left_Kb = Left_Add_Line[i];
				}
				Left_Add_Line[i] = Calculate_Add(i, Left_Ka, Left_Kb);	// 使用前一帧图像左边界斜率补线
			}
			else
			{
				if (!Left_Add_Start)	// 之前没有补线
				{
					Left_Add_Start = i;	// 记录左侧补线开始行
					Curve_Fitting(&Left_Ka, &Left_Kb, &Left_Add_Start, Left_Add_Line, Left_Add_Flag, 1);	// 使用两点法拟合直线
				}
				Left_Add_Line[i] = Calculate_Add(i, Left_Ka, Left_Kb);	// 补线完成
			}
		}
		else
		{
			if (Left_Add_Start)	// 已经开始补线
			{
				if (!Left_Add_Stop && !Left_Add_Flag[i+2])
				{
					if (Left_Add_Line[i] >= Left_Add_Line[i+2])
					{
						Left_Add_Stop = i;	// 记录左侧补线结束行
					}
				}
			}
		}
		
		if (Right_Add_Flag[i])	// 右侧需要补线
		{
			if (i >= 55)	// 前三行补线不算
			{
				if (!Right_Add_Start)
				{
					Right_Add_Start = i;	// 记录补线开始行
					Right_Ka = 0;
					Right_Kb = Right_Add_Line[i];
				}
				Right_Add_Line[i] = Calculate_Add(i, Right_Ka, Right_Kb);	// 使用前一帧图像右边界斜率补线
			}
			else
			{
				if (!Right_Add_Start)	// 之前没有补线
				{
					Right_Add_Start = i;	// 记录右侧补线开始行
					Curve_Fitting(&Right_Ka, &Right_Kb, &Right_Add_Start, Right_Add_Line, Right_Add_Flag, 2);	// 使用两点法拟合直线
				}
				Right_Add_Line[i] = Calculate_Add(i, Right_Ka, Right_Kb);	// 补线完成
			}
		}
		else
		{
			if (Right_Add_Start)	// 已经开始补线
			{
				if (!Right_Add_Stop && !Right_Add_Flag[i+2])
				{
					if (Right_Line[i] <= Right_Line[i+2])
					{
						Right_Add_Stop = i;	// 记录右侧补线结束行
					}
				}
			}
		}
		/*************************** 第一轮补线结束 ***************************/
		
		Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	// 重新计算赛道宽度
		if ((Left_Add_Flag[i] && Right_Add_Flag[i]) || (!Left_Add_Flag[i] && !Right_Add_Flag[i]))
		{
			Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	// 重新计算赛道宽度
			Mid_Line[i] = (Right_Add_Line[i] + Left_Add_Line[i]) / 2;	// 计算中线
		}
		else
		{
			Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	// 重新计算赛道宽度
			Mid_Line[i] = Mid_Line[i+2];
		}
		if (Width_Add[i] < Width_Min)
		{
			Width_Min = Width_Add[i];	// 更新最小赛道宽度
		}

		Line_Count = i;	// 记录成功识别到的赛道行数
                if(Left_Add_Stop&&Right_Add_Stop)    //补线结束后又要补线这不补
                {
                    if(Left_Add_Flag[i]||Right_Add_Flag[i])
                    {
                        break;
                        Line_Count = i+2;
                    }
                }
	
//        Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	// 重新计算赛道宽度
//		if ((Left_Add_Flag[59] && Left_Add_Start && !Left_Add_Stop) && (Right_Add_Flag[59] && Right_Add_Start && !Right_Add_Stop))
//		{
//			Mid_Line[i] = Mid_Line[i+2];
//		}
//		else
//		{
//			Mid_Line[i] = (Right_Add_Line[i] + Left_Add_Line[i]) / 2;	// 计算中线
//		}
//		if (Width_Add[i] < Width_Min)
//		{
//			Width_Min = Width_Add[i];	// 更新最小赛道宽度
//		}
//
//		Line_Count = i;	// 记录成功识别到的赛道行数
//                if(Left_Add_Stop&&Right_Add_Stop)    //补线结束后又要补线这不补
//                {
//                    if(Left_Add_Flag[i]||Right_Add_Flag[i])
//                    {
//                        break;
//                        Line_Count = i+2;
//                    }
//                }
	}
	/*************************** 第二轮补线修复开始 ***************************/
	if (!Annulus_Flag)	// 不是环路
	{
		if (Left_Add_Start)		// 左边界需要补线
		{
			Line_Repair(Left_Add_Start, Left_Add_Stop, data, Left_Line, Left_Add_Line, Left_Add_Flag, 1);
		}
		if (Right_Add_Start)	// 右边界需要补线
		{
			Line_Repair(Right_Add_Start, Right_Add_Stop, data, Right_Line, Right_Add_Line, Right_Add_Flag, 2);
		}
	}

	/*************************** 第二轮补线修复结束 ***************************/
	
	/****************************** 中线修复开始 ******************************/
	Mid_Line_Repair(Line_Count, data);
	/****************************** 中线修复结束 ******************************/
    
    if(Ramp_Continue_Flag)  //开启坡道检测
    {
        Ramp_Check();
    }    
}

/*
*	中线平均
*
*	说明：不做特殊处理，直接取均值
*/
uchar Point_Average(void)
{
	uchar i, Half_Line;
	int32 Point, Sum = 0;
	static uchar Last_Point = 40;
	
	if (Out_Side || Line_Count >= 55)	// 出界或者摄像头图像异常
	{
		Point  = Last_Point;			// 使用上次目标点
	}
	else
	{
		Half_Line = 59-(59-Line_Count)/2;	// 计算有效行的中间行
		if (Half_Line%2 == 0)
		{
			Half_Line += 1;
		}
		if (Left_Max + 12 < Right_Min)
		{
			Point = (Left_Max + Right_Min) / 2;
		}
		else
		{
			if (Line_Count <= 21)
			{
				Line_Count = 21;
			}
			for (i = 61; i >= Line_Count; )
			{
				i -= 2;
				Sum += Mid_Line[i];
			}
			Point = Sum / ((61-Line_Count)/2);	// 对中线求平均
			if (Left_Add_Line[Half_Line] >= 20)				// 太偏左
			{
				Point += (Left_Add_Line[Half_Line]-20);	// 适当补偿
			}
			if (Right_Add_Line[Half_Line] <= 60)				// 太偏右
			{
				Point -= (60-Right_Add_Line[Half_Line]);	// 适当补偿
			}
		}
		
		Point = Point*0.9 + Last_Point*0.1;	// 低通滤波
		Point = range_protect(Point, 1, 79);		// 限幅，防止补偿溢出
		
		/*** 障碍物特殊情况处理 ***/
//		if (Left_Hazard_Flag)			//左侧有障碍物且需要补线，即使误判也不会造成影响
//		{
//			Point = Mid_Line[Left_Hazard_Flag]+6;	//使用障碍物出现的那一行中点作为目标点
//			if (Left_Hazard_Flag < 40)
//			{
//				Point += 3;
//			}
//		}
//		else if (Right_Hazard_Flag)	//右测有障碍物且需要补线，即使误判也不会造成影响
//		{
//			Point = Mid_Line[Right_Hazard_Flag]-6;//使用障碍物出现的那一行中点作为目标点
//			if (Right_Hazard_Flag < 40)
//			{
//				Point -= 5;
//			}
//		}
		Last_Point = Point;	// 更新上次目标点
	}
	
	return Point;
}

/*
*	最小二乘法计算斜率
*
*	说明：返回斜率作为转向控制量,结果放大100倍
*/
int32 Least_Squares(void)
{
	int32 Ave_x, Ave_y, Sum1, Sum2;
	uchar i;
	float f_Slope;
	int32 i_Slope;
	static int32 i_Last_Slope = 0;
	
	for (i = 61; i >= Line_Count; )
	{
		i -= 2;
	}
	i_Last_Slope = i_Slope;
	
	return i_Slope;
}

/*
*	斜率计算
*
*	说明：使用斜率作为转向控制量，返回斜率的倒数
*/
float Slope_Weight(uchar *Mid)
{
	float Slope;
	
	Slope = 1.0 * (Mid[Line_Count] - 40) / (60-Line_Count);
	
	return Slope;
}

/*
*	赛道角度计算
*
*	说明：返回结果为 右：0：180°，1：135°，2：90°，>= 1可能为直角突变
*					 左：0：180°，-1：135°，-2：90°，<= -1可能为直角突变
*/
char Calculate_Angle(uchar Point_1, uchar Point_2, uchar Point_3)
{
	char K1, K2,Result;
	
	K1 = Point_2 - Point_1;
	K2 = Point_3 - Point_2;
	
	Result = (K2 - K1);
	
	return Result;
}

/*
*	两点法求直线
*
*	说明：拟合直线 y = Ka * x + Kb   Mode == 1代表左边界，Mode == 2代表右边界
*/
void Curve_Fitting(float *Ka, float *Kb, uchar *Start, uchar *Line, uchar *Add_Flag, uchar Mode)
{
	*Start += 4;
	if (Add_Flag[*Start] == 2)
	{
		if (*Start <= 51)
		{
			*Start += 2;
		}
		*Ka = 1.0*(Line[*Start+4] - Line[*Start]) / 4;
		if (Mode == 2)
		{
			if (*Ka < 0)
			{
				*Ka = 0;
			}
		}
		if (Mode == 1)
		{
			if (*Ka > 0)
			{
				*Ka = 0;
			}
		}
	}
	else
	{
		*Ka = 1.0*(Line[*Start+4] - Line[*Start]) / 4;
	}
	*Kb = 1.0*Line[*Start] - (*Ka * (*Start));
}

/*
*	两点法求直线
*
*	说明：拟合直线 y = Ka * x + Kb   Mode == 1代表左边界，Mode == 2代表右边界
*/
void Curve2_Fitting(float *Ka, float *Kb, uchar Start,uchar End, uchar *Line, uchar Mode)
{
        if (Mode==1)
        {
             *Ka = 1.0*((Line[Start]+21) - Line[End]) / (Start-End);
             *Kb = 1.0*Line[End] - (*Ka * End);   
        }
        else
        {
              *Ka = 1.0*((Line[Start]-21) - Line[End]) / (Start-End);
              *Kb = 1.0*Line[End] - (*Ka * End);   
        }
	
}

/*
*	计算补线坐标
*
*	说明：使用两点法计算拟合出的补线坐标
*/
uchar Calculate_Add(uchar i, float Ka, float Kb)	// 计算补线坐标
{
	float res;
	uchar Result;
	
	res = i * Ka + Kb;
	Result = range_protect((int32)res, 1, 79);
	
	return Result;
}

/*
*	尽头搜索
*
*	说明：从某一点开始竖直向上搜索，返回最远行坐标
*/
uchar Limit_Scan(uchar i, uchar *data, uchar Point)
{
	for ( ; i >= 20; i--)
	{
		if (!data[80*i + Point])	// 搜索到黑点
		{
			break;
		}
	}
	
	return i;	// 返回最远行坐标
}

/*
*	第一行特殊处理
*
*	说明：先使用第60行中点作为第59行(第一行)搜线起始位置，成功搜索到左右边界后
 	将第59行中点赋值给第60行便于下一帧图像使用。如果第60行中点在本行为黑点，再
 	分别使用左遍历和右遍历的方法搜索边界，以赛道宽度较大的结果作为第59行边界，
 	若仍然搜索不到边界或数据异常认为出界，出界返回0
*/
uchar First_Line_Handle(uchar *data)
{
	uchar i;	// 控制行
	uchar Weight_Left, Weight_Right;	// 左右赛道宽度
	uchar Mid_Left, Mid_Right;
	Image_Para_Init();
	i = 59;

    
                        
	if (!data[i*80 + Mid_Line[61]])	// 第61行中点在第59行为黑点
	{
		Weight_Left = Traversal_Left(i, data, &Mid_Left, 1, 79);	// 从左侧搜索边界
		Weight_Right = Traversal_Right(i, data, &Mid_Right, 1, 79);// 从右侧搜索边界
		if (Weight_Left >= Weight_Right && Weight_Left)	// 左赛道宽度大于右赛道宽度且不为0
		{
			Traversal_Left_Line(i, data, Left_Line, Right_Line);	// 使用左遍历获取赛道边界
		}
		else if (Weight_Left < Weight_Right && Weight_Right)
		{
			Traversal_Right_Line(i, data, Left_Line, Right_Line);	// 使用右遍历获取赛道边界
		}
		else	// 说明没查到
		{
			return 0;
		}
	}
	else
	{
		Traversal_Mid_Line(i, data, Mid_Line[i+2], 1, 79, Left_Line, Right_Line, Left_Add_Line, Right_Add_Line);	// 从前一行中点向两边扫描
	}
	
      
	Left_Line[61] = Left_Line[59];
	Right_Line[61] = Right_Line[59];
	Left_Add_Line[61] = Left_Add_Line[59];
	Right_Add_Line[61] = Right_Add_Line[59];
	if (Left_Add_Flag[59] && Right_Add_Flag[59])
	{
		Mid_Line[59] = Mid_Line[61];
	}
	else
	{
		Mid_Line[59] = (Right_Line[59] + Left_Line[59]) / 2;
		Mid_Line[61] = Mid_Line[59];	// 更新第60行虚拟中点，便于下一帧图像使用
	}
	Width_Real[61] = Width_Real[59];
	Width_Add[61] = Width_Add[59];
	Width_Min = Width_Add[59];
	
	return 1;
}

/*
*	从左侧开始搜索边界，返回赛道宽度
*
*	说明：本函数仅仅作为探测赛道使用，仅返回赛道宽度，不保存边界数据
*/
uchar Traversal_Left(uchar i, uchar *data, uchar *Mid, uchar Left_Min, uchar Right_Max)
{
	uchar j, White_Flag = 0;
	uchar Left_Line = Left_Min, Right_Line = Right_Max;
	
	for (j = Left_Min; j <= Right_Max; j++)	// 边界坐标 1到79
	{
		if (!White_Flag)	// 先查找左边界
		{
			if (data[i*80 + j])	// 检测到白点
			{
				Left_Line = j;	// 记录当前j值为本行左边界
				White_Flag = 1;	// 左边界已找到，必有右边界
				
				continue;	// 结束本次循环，进入下次循环
			}
		}
		else
		{
			if (!data[i*80 + j])//检测黑点
			{
				Right_Line = j-1;//记录当前j值为本行右边界
				
				break;	// 左右边界都找到，结束循环
			}
		}
	}
	
	if (!White_Flag)	// 未找到左右边界
	{
		return 0;
	}
	else
	{
		*Mid = (Right_Line + Left_Line) / 2;
		
		return (Right_Line - Left_Line);
	}
}

/*
*	从右侧开始搜索边界，返回赛道宽度
*
*	说明：本函数仅仅作为探测赛道使用，仅返回赛道宽度，不保存边界数据
*/
uchar Traversal_Right(uchar i, uchar *data, uchar *Mid, uchar Left_Min, uchar Right_Max)
{
	uchar j, White_Flag = 0;
	uchar Left_Line = Left_Min, Right_Line = Right_Max;
	
	for (j = Right_Max; j >= Left_Min; j--)	// 边界坐标 1到79
	{
		if (!White_Flag)	// 先查找右边界
		{
			if (data[i*80 + j])	// 检测到白点
			{
				Right_Line = j;	// 记录当前j值为本行右边界
				White_Flag = 1;	// 右边界已找到，必有左边界
				
				continue;	// 结束本次循环，进入下次循环
			}
		}
		else
		{
			if (!data[i*80 + j])	//检测黑点
			{
				Left_Line = j+1;	//记录当前j值为本行左边界
				
				break;	// 左右边界都找到，结束循环
			}
		}
	}
	
	if (!White_Flag)	// 未找到左右边界
	{
		return 0;
	}
	else
	{
		*Mid = (Right_Line + Left_Line) / 2;
			
		return (Right_Line - Left_Line);
	}
}

/*
*	从左侧开始搜索边界，保存赛道边界，返回1成功 0失败
*
*	说明：本函数使用后将保存边界数据
*/
uchar Traversal_Left_Line(uchar i, uchar *data, uchar *Left_Line, uchar *Right_Line)
{
	uchar j, White_Flag = 0;
	
	Left_Line[i] = 1;
	Right_Line[i] = 79;
	
	for (j = 1; j < 80; j++)	// 边界坐标 1到79
	{
		if (!White_Flag)	// 先查找左边界
		{
			if (data[i*80 + j])	// 检测到白点
			{
				Left_Line[i] = j;	// 记录当前j值为本行左边界
				White_Flag = 1;		// 左边界已找到，必有右边界
				
				continue;	// 结束本次循环，进入下次循环
			}
		}
		else
		{
			if (!data[i*80 + j])	//检测黑点
			{
				Right_Line[i] = j-1;//记录当前j值为本行右边界
				
				break;	// 左右边界都找到，结束循环
			}
		}
	}
	if (White_Flag)
	{
		Left_Add_Line[i] = Left_Line[i];
		Right_Add_Line[i] = Right_Line[i];
		Width_Real[i] = Right_Line[i] - Left_Line[i];
		Width_Add[i] = Width_Real[i];
	}
	
	return White_Flag;	// 返回搜索结果
}

/*
*	从右侧开始搜索边界，保存赛道边界，返回1成功 0失败
*
*	说明：本函数使用后将保存边界数据
*/
uchar Traversal_Right_Line(uchar i, uchar *data, uchar *Left_Line, uchar *Right_Line)
{
	uchar j, White_Flag = 0;
	
	Left_Line[i] = 1;
	Right_Line[i] = 79;
	
	for (j = 79; j > 0; j--)	// 边界坐标 1到79
	{
		if (!White_Flag)	// 先查找右边界
		{
			if (data[i*80 + j])	// 检测到白点
			{
				Right_Line[i] = j;	// 记录当前j值为本行右边界
				White_Flag = 1;		// 右边界已找到，必有左边界
				
				continue;	// 结束本次循环，进入下次循环
			}
		}
		else
		{
			if (!data[i*80 + j])	//检测黑点
			{
				Left_Line[i] = j+1;//记录当前j值为本行左边界
				
				break;	// 左右边界都找到，结束循环
			}
		}
	}
	if (White_Flag)
	{
		Left_Add_Line[i] = Left_Line[i];
		Right_Add_Line[i] = Right_Line[i];
		Width_Real[i] = Right_Line[i] - Left_Line[i];
		Width_Add[i] = Width_Real[i];
	}
	
	return White_Flag;	// 返回搜索结果
}

/*
*	从中间向两边搜索边界
*
*	说明：本函数使用后将保存边界数据
*/
void Traversal_Mid_Line(uchar i, uchar *data, uchar Mid, uchar Left_Min, uchar Right_Max, uchar *Left_Line, uchar *Right_Line, uchar *Left_Add_Line, uchar *Right_Add_Line)
{
	uchar j;
	
	Left_Add_Flag[i] = 1;	// 初始化补线标志位
	Right_Add_Flag[i] = 1;
	
////	Left_Min = range_protect(Left_Min, 1, 79);	// 限幅，防止出错
//	if (Left_Add_Flag[i+2])
//	{
//		Left_Min = range_protect(Left_Add_Line[i+2]-10, 1, 79);
//	}
////	Right_Max = range_protect(Right_Max, 1, 79);
//	if (Right_Add_Flag[i+2])
//	{
//		Right_Max = range_protect(Right_Add_Line[i+2]+10, 1, 79);
//	}
	
	Right_Line[i] = Right_Max;
	Left_Line[i] = Left_Min;	// 给定边界初始值
	
	for (j = Mid; j >= Left_Min+1; j--)	// 以前一行中点为起点向左查找边界
	{
		if (!data[i*80 + j]&&!data[i*80 + j-1])	// 检测到黑点
		{
			Left_Add_Flag[i] = 0;	//左边界不需要补线，清除标志位
			Left_Line[i] = j;	//记录当前j值为本行实际左边界
			Left_Add_Line[i] = j;	// 记录实际左边界为补线左边界
			
			break;
		}
	}
	for (j = Mid; j <= Right_Max-1; j++)	// 以前一行中点为起点向右查找右边界
	{
		if (!data[i*80 + j]&&!data[i*80 + j+1])	//检测到黑点
		{
			Right_Add_Flag[i] = 0;		//右边界不需要补线，清除标志位
			Right_Line[i] = j;	//记录当前j值为本行右边界
			Right_Add_Line[i] = j;	// 记录实际右边界为补线左边界
			
			break;
		}
	}
	if (Left_Add_Flag[i])	// 左边界需要补线
	{
		if (i >= 55)	// 前6行
		{
			Left_Add_Line[i] = Left_Line[59];	// 使用底行数据
		}
		else                     ///
		{
			Left_Add_Line[i] = Left_Add_Line[i+2];	// 使用前2行左边界作为本行左边界
		}
	}
	if (Right_Add_Flag[i])	// 右边界需要补线
	{
		if (i >= 55)	// 前6行
		{
			Right_Add_Line[i] = Right_Line[59];	// 使用底行数据
		}
		else         //////
		{
			Right_Add_Line[i] = Right_Add_Line[i+2];	// 使用前2行右边界作为本行右边界
		}
	}
	Width_Real[i] = Right_Line[i] - Left_Line[i];			// 计算实际赛道宽度
	Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	// 计算补线赛道宽度
	
}

/*
*	补线修复
*
*	说明：有始有终才使用，直接使用两点斜率进行补线
*/
/*
*	补线修复
*
*	说明：有始有终才使用，直接使用两点斜率进行补线
*/
void Line_Repair(uchar Start, uchar Stop, uchar *data, uchar *Line, uchar *Line_Add, uchar *Add_Flag, uchar Mode)
{
	float res;
	uchar i;	// 控制行
	float Ka, Kb;
	
	if ((Mode == 1) && (Right_Add_Start <= Stop) && Start <= 53)	// 左边界补线
	{
		if (Start <= 57)
		{
			Start +=2;
		}
		for (i = Start+2; i >= Stop+2;)
		{
			i -= 2;
			Line_Add[i] = range_protect(Right_Add_Line[i] - Width_Add[i+2], 1, 79); 
			Width_Add[i] = Width_Add[i+2];
		}
	}
	if ((Mode == 2) && (Left_Add_Start <= Stop) && Start <= 53)	// 右边界补线
	{
		if (Start <= 57)
		{
			Start +=2;
		}
		for (i = Start+2; i >= Stop+2;)
		{
			i -= 2;
			Line_Add[i] = range_protect(Left_Add_Line[i] + Width_Add[i+2], 1, 79); 
			Width_Add[i] = Width_Add[i+2];
		}
	}
	else
	{
		if (Stop)	// 有始有终
		{
			if ((Right_Add_Stop >= 47 && Left_Add_Stop >= 47) || (Right_Add_Stop >= 47 && Left_Add_Start <= Right_Add_Stop) || (Left_Add_Stop >= 47 && Right_Add_Start <= Left_Add_Stop))	// 只有较少行需要补线，不计算斜率，直接竖直向下补线
			{
				for (i = Stop-2; i <= 57; )
				{
					i += 2;
					Line_Add[i] = Line_Add[Stop];
				}
			}
//			else if (Right_Add_Start >= 57 && Left_Add_Start >= 57)	// 补线起始行较近，从上向下补线
//			{
//				Ka = 1.0*(Line_Add[Stop] - Line_Add[Line_Count]) / (Stop - Line_Count);
//				Kb = 1.0*Line_Add[Stop] - (Ka * Stop);
//				
//				for (i = Stop; i <= 57; )
//				{
//					i += 2;
//					res = i * Ka + Kb;
//					Line_Add[i] = range_protect((int32)res, 1, 159);
//				}
//			}
			else	// 将起始行和结束行计算斜率补线
			{
				if (Start <= 57)
				{
					Start +=2;
				}
				Ka = 1.0*(Line_Add[Start] - Line_Add[Stop]) / (Start - Stop);
				Kb = 1.0*Line_Add[Start] - (Ka * Start);
				
				for (i = Stop; i <= Start; )
				{
					i += 2;
					res = i * Ka + Kb;
					Line_Add[i] = range_protect((int32)res, 1, 79);
				}
			}
		}
	}
}


void Mid_Line_Repair(uchar count, uchar *data)
{
	int res;
	uchar i,j;	// 控制行
    Left_Max = 0;
	Right_Min = 79;
    
	for (i = 59; i >= count;i-=2)
	{
		Mid_Line[i] = (Right_Add_Line[i] + Left_Add_Line[i]) / 2;	// 计算赛道中点
		Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];		// 计算赛道宽度
        if (i>21)
        {
            if (Left_Add_Line[i] > Left_Max)
            {
                Left_Max = Left_Add_Line[i];
            }
            if (Right_Add_Line[i] < Right_Min)
            {
                Right_Min = Right_Add_Line[i];
            }	
        }
        //障碍处理
                if(Left_Hazard_Flag)
                {
                      Curve2_Fitting(&Left_Ka, &Left_Kb, 59,Left_Hazard_Flag, Left_Line, 1);
                      for (j=59;j>=Left_Hazard_Flag;j--)
                      {
                           Left_Add_Line[j] = Calculate_Add(j, Left_Ka,Left_Kb);	// 补线完成
                           Mid_Line[j] = (Right_Add_Line[j] + Left_Add_Line[j]) / 2;
                      }
                }
                if(Right_Hazard_Flag)
                {
                      Curve2_Fitting(&Right_Ka, &Right_Kb, 59,Right_Hazard_Flag, Right_Line, 2);
                      for (j=59;j>=Right_Hazard_Flag;j--)
                      {
                           Right_Add_Line[j] = Calculate_Add(j, Right_Ka, Right_Kb);	// 补线完成  
                           Mid_Line[j] = (Right_Add_Line[j] + Left_Add_Line[j]) / 2;
                      }
                
                }

#if   Display_OFF_ON	
	/**//*************************** 上位机显示边界 ***************************/
	/**/data[i*80 + Left_Add_Line[i] + 4] = 0;	// 上位机显示补线后的左边界，不用时屏蔽
	/**/data[i*80 + Right_Add_Line[i] - 4] = 0;	// 上位机显示补线后的右边界，不用时屏蔽
	/**/data[i*80 + Mid_Line[i]] = 0;			// 上位机显示中线，不用时屏蔽
	/**/data[i*80 + Left_Line[i] + 2] = 0;		// 上位机显示原始左边界，不用时屏蔽
	/**/data[i*80 + Right_Line[i] - 2] = 0;		// 上位机显示原始右边界，不用时屏蔽
	/**//*************************** 上位机显示边界 ***************************/
#endif
	}

	Mid_Line[61] = Mid_Line[59];
}

///*
//*	中线修复
//*
//*	说明：普通弯道丢线使用平移赛道方式，中点到达边界结束
//*/
//void Mid_Line_Repair(uchar count, uchar *data)
//{
//	char res;
//	uchar i,j,Left_Hazard,Right_Hazard;	// 控制行
//	float Ka, Kb;
//        
//        Left_Hazard = 0;
//        Right_Hazard = 0;
//	
//	Left_Max = Left_Add_Line[59];
//	Right_Min = Right_Add_Line[59];
//	
//	for (i = 61; i >= count+2; )
//	{
//		i -= 2;
//		if (Left_Add_Flag[i] && Right_Add_Flag[i])	// 左右都需要补线
//		{
//			Mid_Line[i] = (Right_Add_Line[i] + Left_Add_Line[i]) / 2;	// 计算赛道中点
//			Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];		// 计算赛道宽度
//		}
//		else if (Left_Add_Flag[i])	// 仅左侧需要补线
//		{
//			if (Left_Add_Line[i] == 1 && !Left_Add_Stop)	// 补到极限了，不具备参考价值
//			{
//			res = Right_Add_Line[i] - Width_Add[i+2] / 2 + (Right_Add_Line[i+2] - Right_Add_Line[i]) / 2;	// 平移赛道宽度一半
//				Width_Add[i] = 2 * (Right_Add_Line[i] - res);	// 根据右边界和中线推算赛道宽度
//				if (res <= 0)	// 中点已到达图像极限，不再具备参考价值
//				{
//					i += 2;	// 记录成功识别到的中点行数
//					break;
//				}
//			}
//			else
//			{
//				res = (Right_Add_Line[i] + Left_Add_Line[i]) / 2;		// 计算赛道中点
//				Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	// 计算赛道宽度
//			}
//			Mid_Line[i] = res;	// 更新中点
//		}
//		else if (Right_Add_Flag[i])	// 仅右侧需要补线
//		{
//			if (Right_Add_Line[i] == 79 && !Right_Add_Stop)	// 补到极限了，不具备参考价值
//			{
//				res = Left_Add_Line[i] + Width_Add[i+2] / 2 - (Left_Add_Line[i]-Left_Add_Line[i+2]) / 2;	// 平移赛道宽度一半
//				Width_Add[i] = 2 * (res - Left_Add_Line[i]);	// 根据中线和左边界推算赛道宽度
//				if (res >= 80)	// 中点已到达图像极限，不再具备参考价值
//				{
//					i += 2;	// 记录成功识别到的中点行数
//					break;
//				}
//			}
//			else
//			{
//				res = (Right_Add_Line[i] + Left_Add_Line[i]) / 2;		// 计算赛道中点
//				Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	// 计算赛道宽度
//			}
//			Mid_Line[i] = res;	// 更新中点
//		}
//                //障碍处理
//                if(Left_Hazard_Flag)
//                {
//                      Curve2_Fitting(&Left_Ka, &Left_Kb, 59,Left_Hazard_Flag, Left_Line, 1);
//                      for (j=59;j>=Left_Hazard_Flag;j--)
//                      {
//                           Left_Add_Line[j] = Calculate_Add(j, Left_Ka,Left_Kb);	// 补线完成
//                           Mid_Line[j] = (Right_Add_Line[j] + Left_Add_Line[j]) / 2;
//                      }
//                }
//                if(Right_Hazard_Flag)
//                {
//                      Curve2_Fitting(&Right_Ka, &Right_Kb, 59,Right_Hazard_Flag, Right_Line, 2);
//                      for (j=59;j>=Right_Hazard_Flag;j--)
//                      {
//                           Right_Add_Line[j] = Calculate_Add(j, Right_Ka, Right_Kb);	// 补线完成  
//                           Mid_Line[j] = (Right_Add_Line[j] + Left_Add_Line[j]) / 2;
//                      }
//                
//                }
//		if (Left_Add_Line[i] > Left_Max)
//		{
//			Left_Max = Left_Add_Line[i];
//		}
//		if (Right_Add_Line[i] < Right_Min)
//		{
//			Right_Min = Right_Add_Line[i];
//		}
//
//#if   Display_OFF_ON		
//	/**//*************************** 上位机显示边界 ***************************/
//	/**/data[i*80 + Left_Add_Line[i] + 2] = 0;	// 上位机显示补线后的左边界，不用时屏蔽
//	/**/data[i*80 + Right_Add_Line[i] - 2] = 0;	// 上位机显示补线后的右边界，不用时屏蔽
//	/**/data[i*80 + Mid_Line[i]] = 0;			// 上位机显示中线，不用时屏蔽
////	/**/data[i*80 + Left_Line[i] + 2] = 0;		// 上位机显示原始左边界，不用时屏蔽
////	/**/data[i*80 + Right_Line[i] - 2] = 0;		// 上位机显示原始右边界，不用时屏蔽
//	/**//*************************** 上位机显示边界 ***************************/
//#endif
//	}
//	Mid_Line[61] = Mid_Line[59];
//	Line_Count = i;		// 记录成功识别到的中点行数
//	
//	if (Left_Add_Stop)
//	{
//		Curve22_Fitting(&Left_Ka, &Left_Kb, Left_Add_Stop, Left_Add_Line);
//	}
//	if (Right_Add_Stop)
//	{
//		Curve22_Fitting(&Right_Ka, &Right_Kb, Right_Add_Stop, Right_Add_Line);
//	}
//}

/****************** 新算法 ******************/

/*
*	加权平均
*
*	说明：权重是乱凑的，效果不好
*/
uchar  Motor_Point_Weight(int8 Num)
{
	uchar i ,Point, Point_Mid;
    uchar Count=0;
    int8   Point_id;
	int32 Sum = 0, Weight_Count = 0;
	uchar tep=0,L_Min=40,R_Max=40;

        if (Line_Count <= 15)
            {
              Line_Count = 15;
            }
//        for (i = 61; i >= Line_Count+2; )		//使用加权平均
//		{
//			i -= 2;
//			Sum += Mid_Line[i] * Weight[59-i];
//			Weight_Count += Weight[59-i];
//		}
//		Point = range_protect(Sum / Weight_Count, 2, 78);
        Count = 60-Line_Count;
        if(Count <= Num)
        {
            
        }
        for (i = 59; i >= Line_Count-Num;i-=2)		//使用加权平均
		{	
            Point_id = i-Num;
            if (Point_id>59)Point_id=59;
			Sum += Mid_Line[Point_id] * Weight[59-i];
			Weight_Count += Weight[59-i];
		}
		Point = range_protect(Sum / Weight_Count, 2, 78);
        
        
//        for (i = 61; i >= Line_Count+2; )		//使用加权平均
//		{
//			i -= 2;
//			Sum += Mid_Line[i] * Weight[59-i];
//			Weight_Count += Weight[59-i];
//		}
         //////////////////////////////// 
#if   SSSSSSS_OFF_ON	
         S_temp=0;
         for (i = 57; i >= Line_Count+2; )		//检测小S弯道
		{
			i -= 2;
            if(tep==0)
            {
                if (Mid_Line[i]>(Mid_Line[i+4]+1))
                {
                    tep=1;
                    S_temp++;
                }
                else if ((Mid_Line[i]+1)<Mid_Line[i+4])
                {
                    tep=2;
                    S_temp++;
                }
            }
            if (tep==1)
            {
                 if ((Mid_Line[i]+1)<Mid_Line[i+2])
                {
                    tep=2;
                    S_temp++;
                }
            }
            if (tep==2)
            {
                 if (Mid_Line[i]>(Mid_Line[i+2]+1))
                {
                    tep=1;
                    S_temp++;
                }
            }
            
        }
//        save_var(VAR2, S_temp);
         ////////////////
        if (S_temp>=3 && !Annulus_Flag)
        {
             for (i = 61; i >= Line_Count+2; )		//计算小S弯道偏差
            {
                i -= 2;
                if(L_Min> Mid_Line[i])
                {
                    L_Min = Mid_Line[i];
                }
                if (R_Max < Mid_Line[i])
                {
                    R_Max = Mid_Line[i];
                }
            }
            Point = (R_Max + L_Min) / 2;
        }
#endif
        ////////////////////////
		Point = range_protect(Point, 2, 78);
		Last_Point = Point;
		

        Point_Mid = Mid_Line[Line_Count+2];

	
	return Point;
}
/*
*	加权平均
*
*	说明：权重是乱凑的，效果不好
*/
uchar Line_Num=0;
uchar Dynamic_Point_Weight(int8 Num)
{
	uchar i ,Point, Point_Mid;
    uchar Count=0;
    int8   Point_id;
	int32 Sum = 0, Weight_Count = 0;
	uchar tep=0,L_Min=40,R_Max=40;
	if (Out_Side || Line_Count >= 57)	//出界或者摄像头图像异常
	{
//        Line_Num++;
		Point  = Last_Point;
               // Speed=0;
//                if(Annulus_Check && Line_Num>=1)
//                {
//                   Stop_Flag = 1;
//                }
	}
    /********************************起跑线停车控制*******************************/
    if(Starting_Line_Flag)
    {
//            Line_Num++;
            Point  = Last_Point;
           // Speed=0;
            if(Annulus_Check)
            {
                    Stop_Flag = 1;
            }
    }
    /**********************起跑线停车控制********************/
	else
	{
        if (Line_Count <= 20)
            {
              Line_Count = 21;
            }
//        for (i = 61; i >= Line_Count+2; )		//使用加权平均
//		{
//			i -= 2;
//			Sum += Mid_Line[i] * Weight[59-i];
//			Weight_Count += Weight[59-i];
//		}
//		Point = range_protect(Sum / Weight_Count, 2, 78);
        Count = 60-Line_Count;
        if(Count <= Num)
        {
            
        }
        for (i = 59; i >= Line_Count-Num;i-=2)		//使用加权平均
		{	
            Point_id = i-Num;
            if (Point_id>59)Point_id=59;
			Sum += Mid_Line[Point_id] * Weight[59-i];
			Weight_Count += Weight[59-i];
		}
		Point = range_protect(Sum / Weight_Count, 2, 78);
        
        
//        for (i = 61; i >= Line_Count+2; )		//使用加权平均
//		{
//			i -= 2;
//			Sum += Mid_Line[i] * Weight[59-i];
//			Weight_Count += Weight[59-i];
//		}
         //////////////////////////////// 
#if   SSSSSSS_OFF_ON	
         S_temp=0;
         for (i = 57; i >= Line_Count+2; )		//检测小S弯道
		{
			i -= 2;
            if(tep==0)
            {
                if (Mid_Line[i]>(Mid_Line[i+4]+1))
                {
                    tep=1;
                    S_temp++;
                }
                else if ((Mid_Line[i]+1)<Mid_Line[i+4])
                {
                    tep=2;
                    S_temp++;
                }
            }
            if (tep==1)
            {
                 if ((Mid_Line[i]+1)<Mid_Line[i+2])
                {
                    tep=2;
                    S_temp++;
                }
            }
            if (tep==2)
            {
                 if (Mid_Line[i]>(Mid_Line[i+2]+1))
                {
                    tep=1;
                    S_temp++;
                }
            }
            
        }
//        save_var(VAR2, S_temp);
         ////////////////
        if (S_temp>=3 && !Annulus_Flag)
        {
             for (i = 61; i >= Line_Count+2; )		//计算小S弯道偏差
            {
                i -= 2;
                if(L_Min> Mid_Line[i])
                {
                    L_Min = Mid_Line[i];
                }
                if (R_Max < Mid_Line[i])
                {
                    R_Max = Mid_Line[i];
                }
            }
            Point = (R_Max + L_Min) / 2;
        }
#endif
        ////////////////////////
		Point = range_protect(Point, 2, 78);
		Last_Point = Point;
		
							/***** 使用最远行数据和目标点作为前瞻 *****/
//		if (Line_Count >= 27)
//		{
//			Point_Mid = Mid_Line[31];
//		}
//		else
//		{
//			Point_Mid = Mid_Line[Line_Count+2];
//		}
                Point_Mid = Mid_Line[Line_Count+2];
                //Ek_compar=(Mid_Line[Line_Count]+Mid_Line[Line_Count+8])>>1;
                //Ek_compar=Mid_Line[Line_Count+4]-Ek_compar;
	}
//        if (Line_Count <= 30)
//        {
//            Foresight = 0.8 * Error_Transform(Point_Mid, 40)	//使用最远行偏差和加权偏差确定前瞻
//			  + 0.2 * Error_Transform(Point,  40);//+0.1*Ek_compar;
//        }
//	else
//        {
            Foresight = 0.8 * Error_Transform(Point_Mid, 40)	//使用最远行偏差和加权偏差确定前瞻
			  + 0.2 * Error_Transform(Point,  40);//+0.1*Ek_compar;
//        }
            
           Last_Foresight=Foresight;
	
	return Point;
}

/*
*	加权平均
*
*	说明：权重是乱凑的，效果不好
*/
uchar Point_Weight(void)
{
	uchar i ,Point, Point_Mid;

	int32 Sum = 0, Weight_Count = 0;
	uchar tep=0,L_Min=40,R_Max=40;
	if (Out_Side || Line_Count >= 57)	//出界或者摄像头图像异常
	{
		Point  = Last_Point;
               // Speed=0;
                if(Annulus_Check)
                {
                   Stop_Flag = 1;
                }
	}
        if(Starting_Line_Flag)
        {
                Point  = Last_Point;
               // Speed=0;
                if(Annulus_Check)
                {
                        Stop_Flag = 1;
                }
        }
	else
	{
        if (Line_Count <= 20)
            {
              Line_Count = 21;
            }
         //////////////////////////////// 
            S_temp=0;
         for (i = 59; i >= Line_Count+2; )		//检测S弯道
		{
			i -= 2;
            if(tep==0)
            {
                if (Mid_Line[i]>Mid_Line[i+4]+2)
                {
                    tep=1;
                    S_temp++;
                }
                else if (Mid_Line[i]+2<=Mid_Line[i+4])
                {
                    tep=2;
                    S_temp++;
                }
            }
            if (tep==1)
            {
                 if (Mid_Line[i]+2<=Mid_Line[i+4])
                {
                    tep=2;
                    S_temp++;
                }
            }
            if (tep==2)
            {
                 if (Mid_Line[i]>Mid_Line[i+4]+2)
                {
                    tep=1;
                    S_temp++;
                }
            }
            
        }
//            save_var(VAR2, S_temp);
        ///////////////////////////////
		for (i = 61; i >= Line_Count+2; )		//使用加权平均
		{
			i -= 2;
			Sum += Mid_Line[i] * Weight[59-i];
			Weight_Count += Weight[59-i];
		}
		Point = range_protect(Sum / Weight_Count, 2, 78);
         ////////////////
        if (S_temp>=3)
        {
             for (i = 61; i >= Line_Count+2; )		//计算S弯道偏差
            {
                i -= 2;
                if(L_Min> Mid_Line[i])
                {
                    L_Min = Mid_Line[i];
                }
                if (R_Max < Mid_Line[i])
                {
                    R_Max = Mid_Line[i];
                }
            } 
            Point = (L_Min + R_Max) / 2;
        }
        ////////////////////////
		Point = range_protect(Point, 2, 78);
		Last_Point = Point;
		
							/***** 使用最远行数据和目标点作为前瞻 *****/
//		if (Line_Count >= 27)
//		{
//			Point_Mid = Mid_Line[31];
//		}
//		else
//		{
//			Point_Mid = Mid_Line[Line_Count+2];
//		}
                Point_Mid = Mid_Line[Line_Count+2];
                //Ek_compar=(Mid_Line[Line_Count]+Mid_Line[Line_Count+8])>>1;
                //Ek_compar=Mid_Line[Line_Count+4]-Ek_compar;
	}
//        if (Line_Count <= 30)
//        {
//            Foresight = 0.8 * Error_Transform(Point_Mid, 40)	//使用最远行偏差和加权偏差确定前瞻
//			  + 0.2 * Error_Transform(Point,  40);//+0.1*Ek_compar;
//        }
//	else
//        {
            Foresight = 0.8 * Error_Transform(Point_Mid, 40)	//使用最远行偏差和加权偏差确定前瞻
			  + 0.2 * Error_Transform(Point,  40);//+0.1*Ek_compar;
//        }
	
	return Point;
}

char Error_Transform(uchar Data, uchar Set)   //求绝对值
{
	char Error;
	
	Error = Set - Data;
	if (Error < 0)
	{
		Error = -Error;
	}
	
	return Error;
}
int32 abs_int(int32 i)//作用： 求绝对值  int32型
{
     int32 B;
      B=i;
     if(B<0) return (-B);
     else    return B;
} 
/*************函数功能********************        
          滤波采用中值滤波
          
          可优化 
*************3月13号  验证通过 ***********/  
uint8  MiddleFilter(uint8 a,uint8 b,uint8 c)
{  
  uint8 temp;
  if(a>b) 
  {
    temp=b;
    b=a;
    a=temp;
  }
  if(b>c)
  {
    temp=c;
    c=b;
    b=temp;
  }
  if(a>b)
  {
    temp=b;
    b=a;
    a=temp;
  }
  return b;
}


/*
*	两点法求直线
*
*	说明：拟合直线 y = Ka * x + Kb
*/
void Curve22_Fitting(float *Ka, float *Kb, uchar Start, uchar *Line)
{
	*Ka = 1.0*(Line[Start+4] - Line[Start]) / 4;
	*Kb = 1.0*Line[Start] - (*Ka * Start);
}



/*!
*  @brief      二值化图像解压（空间 换 时间 解压）
*  @param      dst             图像解压目的地址
*  @param      src             图像解压源地址
*  @param      srclen          二值化图像的占用空间大小
*  @since      v5.0            img_extract(img, imgbuff,CAMERA_SIZE);
*  Sample usage:
*/
void img_extract(uint8 *dst, uint8 *src, uint32 srclen)
{
  uint8 tmpsrc;
  while(srclen --)
  {
    tmpsrc = *src++;
    *dst++ = colour[ (tmpsrc >> 7 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 6 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 5 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 4 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 3 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 2 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 1 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 0 ) & 0x01 ];
  }
}

/*!
*  @brief      二值化图像压缩（空间 换 时间 压缩）
*  @param      dst             图像压缩目的地址
*  @param      src             图像压缩源地址
*  @param      srclen          二值化图像的占用空间大小
*  @since      v5.0            img_extract(img, imgbuff,CAMERA_SIZE);
*  Sample usage:
*/
void img_recontract(uint8 *dst, uint8 *src, uint32 srclen)
{
  uint8 tmpsrc;
  
  while(srclen --)
  {
    tmpsrc=0;
    if(*dst++ == colour[black])
    {
      tmpsrc = tmpsrc + 0x80;
    }
    
    if(*dst++ == colour[black])
    {
      tmpsrc = tmpsrc + 0x40;
    }
    
    if(*dst++ == colour[black])
    {
      tmpsrc = tmpsrc + 0x20;
    }
    
    if(*dst++ == colour[black])
    {
      tmpsrc = tmpsrc + 0x10;
    }
    
    if(*dst++ == colour[black])
    {
      tmpsrc = tmpsrc + 0x08;
    }
    
    if(*dst++ == colour[black])
    {
      tmpsrc = tmpsrc + 0x04;
    }
    
    if(*dst++ == colour[black])
    {
      tmpsrc = tmpsrc + 0x02;
    }
    
    if(*dst++ == colour[black])
    {
      tmpsrc = tmpsrc + 0x01;
    }
    
    *src++ = tmpsrc;
  }
}

/*!
*  @brief      取边界线
*  @param      dst             图像压缩目的地址
*  @param      src             图像压缩源地址
*  @param      srclen          二值化图像的占用空间大小
*  @since      v5.0            img_extract(img, imgbuff,CAMERA_SIZE);
*  Sample usage:
*/
void img_getline(uint8 *dst, uint8 *src, uint32 srclen)
{
  uint8 tmpsrc,buff_y,buff_x;
  
  while(srclen --)
  {
    tmpsrc=0;
    buff_x = *(dst + 80);
    buff_y = *dst++;
    *(dst-1)=colour[white];
    if(*dst != buff_y || (buff_y != buff_x && srclen > 9))
    {
      tmpsrc = tmpsrc + 0x80;
      *(dst-1)=colour[black];
    }
    
    buff_x = *(dst + 80);
    buff_y = *dst++;
    *(dst-1)=colour[white];
    if(*dst != buff_y || (buff_y != buff_x && srclen > 9))
    {
      tmpsrc = tmpsrc + 0x40;
      *(dst-1)=colour[black];
    }
    
    buff_x = *(dst + 80);
    buff_y = *dst++;
    *(dst-1)=colour[white];
    if(*dst != buff_y || (buff_y != buff_x && srclen > 9))
    {
      tmpsrc = tmpsrc + 0x20;
      *(dst-1)=colour[black];
    }
    
    buff_x = *(dst + 80);
    buff_y = *dst++;
    *(dst-1)=colour[white];
    if(*dst != buff_y || (buff_y != buff_x && srclen > 9))
    {
      tmpsrc = tmpsrc + 0x10;
      *(dst-1)=colour[black];
    }
    
    buff_x = *(dst + 80);
    buff_y = *dst++;
    *(dst-1)=colour[white];
    if(*dst != buff_y || (buff_y != buff_x && srclen > 9))
    {
      tmpsrc = tmpsrc + 0x08;
      *(dst-1)=colour[black];
    }
    
    buff_x = *(dst + 80);
    buff_y = *dst++;
    *(dst-1)=colour[white];
    if(*dst != buff_y || (buff_y != buff_x && srclen > 9))
    {
      tmpsrc = tmpsrc + 0x04;
      *(dst-1)=colour[black];
    }
    
    buff_x = *(dst + 80);
    buff_y = *dst++;
    *(dst-1)=colour[white];
    if(*dst != buff_y || (buff_y != buff_x && srclen > 9))
    {
      tmpsrc = tmpsrc + 0x02;
      *(dst-1)=colour[black];
    }
    
    buff_x = *(dst + 80);
    buff_y = *dst++;
    *(dst-1)=colour[white];
    if((*dst != buff_y && srclen % 10 !=0 ) || (buff_y != buff_x && srclen > 9))
    {
      tmpsrc = tmpsrc + 0x01;
      *(dst-1)=colour[black];
    }
    
    *src++ = tmpsrc;
  }
}

void image_processing(uchar *data)
{
    if (!Annulus_Flag)
    {
      Image_Handle(data);
    }
    else
    {
        Annulus2_Handle(data);
    }
    
}

/**************************** 坡道检测开始 ****************************/
void Ramp_Check(void)
{
    uint8 i,j,Ramp_Count,line_Count;
    uint16 Ramp_Error_Sum=0;
    Ramp_Flag = 0;
    Ramp_Count = 0;
    line_Count = 0;
    i = 0;
    
    for(j=41;j>=21;)
    {
      j -= 2;
      if(Last_Foresight<=3&&!Left_Add_Flag[j]&&!Right_Add_Flag[j])
      {
        if (Left_Line[j] >= Left_Line[j+2]&&Right_Line[j] <= Right_Line[j+2]&&Left_Line[j]>Left_Line[59]&&Right_Line[j]<Right_Line[59])	//与前一行的边界实线比较	
        {
            Ramp_Error[i]=Width_Real[j]-Width_Max[j];
            if(Ramp_Error[i]>=20)Ramp_Error[i]=0;
            else if(Ramp_Error[i]<=0)Ramp_Error[i]=0;
          if(Width_Real[j]>Width_Max[j]) Ramp_Count++;
        }
      }
      Ramp_Error_Sum+=Ramp_Error[i];
      i++;
    }
    //Mid_Line_K = 1.0*(Mid_Line[59]-Mid_Line[21])/38;
    if(Ramp_Count>=5&&Ramp_Error_Sum>=14&&Ramp_Error[8]>=(Ramp_Error[0]+3))
    {
        Ramp_Flag = 1;
    }
    else
    {
        Ramp_Flag = 0;
    }
}
/**************************** 坡道检测开始 ****************************/


