#include "ring.h"
#include  "common.h"
#include "include.h"
#include "path.h"
#include "fun.h"
#include "control.h"

int   MidPri         = 40;
int   LastLine       = 0;
float AvaliableLines = 0;
int   LeftLose       = 0;
int   RightLose      = 0; 
int   AllLose        = 0;
int   LeftLoseStart  = 0;//记录左边丢线的开始行
int   RightLoseStart = 0;//记录右边边丢线的开始行
int   WhiteStart     = 0;


/*********define for GetBlackEndParam**********/
int BlackEndMR      = 0;
int BlackEndML      = 0;
int BlackEndLL      = 0;
int BlackEndRR      = 0;
int BlackEndL       = 0;
int BlackEndM       = 0;
int BlackEndR       = 0;
int BlackEndMaxMax  = 0;
int BlackEndMax     = 0;
int DropRow         = 0;

/*********define for FindInflectionPoint()**********/

int RightInflectionPointRow=0;
int RightInflectionPointCol=0;
int LeftInflectionPointRow=0;
int LeftInflectionPointCol=0;
unsigned char RightInflectionPointFlag=0;
unsigned char LeftInflectionPointFlag=0;
unsigned char LeftInflectionPointSecondFlag=0;
unsigned char RightInflectionPointSecondFlag=0;
unsigned char LoopFlag=0;
unsigned char LoopRightOkFlag=0;
unsigned char LoopLeftOkFlag=0;
unsigned int StartRow=0;
unsigned int StartCol=0;
unsigned char MilldleBlack=0;
unsigned int LoopTop=0;
unsigned int LoopRightBorderLose=0;
unsigned int LoopLeftBorderLose=0;
int LoopBorttomFlag=0;
int LoopBorttomRow=0;
int LoopMilldleRow=0;
unsigned int LoopMilldleFlag=0;
unsigned int LoopTopRow=0;
unsigned int LoopLeft=0;
unsigned int MilldlePonit=0;
unsigned int LoopRight=0;
unsigned int LoopRightR=0;
unsigned int LoopLeftL=0;
int BigLoopLeftUp[60];
int BigLoopRightUp[60];
int BigLooptUp[80];

unsigned char LoopRightControlFlag=0;

unsigned char  openLoopFlag=0;//是否开启圆环标志位

unsigned int  process_flag=0;

unsigned int  enter_loop=0;//进入环路
unsigned int  out_loop=0;//出环路

int get_img_point(uint16 h,uint16 w);//声明
//找左右拐点，识别环
//一直都要开着
int left_diuxian_num=0;
int right_diuxian_num=0;
int xianzhi_daxiao(int a[],int num)
{
    int i=0;
    int ret=0;
    for(;i<6;i++)
    {
        if(a[i]!=0&&a[i]>=num)
        {
            ret++;
        }    
    }
    return ret;
}
//void FindInflectionPoint()//向右 调参中
//{
//	char i=0;
//	int distance=0;
//	int g_point_flag=0;
//    int num_line_i=0;
//    int left_total_none_num=0,right_total_none_num=0;
//    int project_i=0;
//    int LoopBorttomRow_arrary[10]={0};
//	//变量清零
//	RightInflectionPointRow=0;//右边的拐点的行坐标
//	RightInflectionPointCol=0;//右边的拐点的列坐标
//	LeftInflectionPointRow=0;//左边的拐点的行坐标
//	LeftInflectionPointCol=0;//左边的拐点的列坐标
//	RightInflectionPointFlag=0;//找到左拐点的标志位
//	LeftInflectionPointFlag=0;//找到右拐点的标志位
//
//	StartRow=0;//从底部往上扫描圆环中间的黑圆的起始行
//	StartCol=0;//从底部往上扫描圆环中间的黑圆的起始列
//
//	LoopBorttomFlag=0;//找到圆环中间的黑圆的底部的标志位
//	LoopBorttomRow=0;//圆环中间的黑圆的底部的行坐标
//	LoopMilldleRow=0;//圆环中间的黑圆的顶部的行坐标
//	LoopMilldleFlag=0;//找到圆环中间的黑圆的顶部的标志位
//	LoopTopRow=0;//找到外环的顶部
//	MilldleBlack=0;//圆环中间的黑圆的顶部和圆环中间的黑圆的底部的行坐标的平均值
//	LoopFlag=0;//找到环形赛道类型的标志
//   
//
//	//左拐点
//	for(i=3;i<=32;i++) //35 30
//	{
//		if((left_pos[i]!=40)&&(left_pos[i]!=79)) 
//		{     
//			if((left_pos[i]-left_pos[i+1]<0))//找到拐点
//			{
//				LeftInflectionPointRow=i;//记录该拐点的行           
//				LeftInflectionPointCol=left_pos[i];//记录该拐点的列           
//				LeftInflectionPointFlag=1;//标记找到左拐点              
//				break;//找到退出                                  
//			}
//		}                                                                                                                                                                                                                                            
//	} 
//
//	//右拐点 
//	for(i=3;i<=32;i++)//不能扫描太远，否则会误判//35 30
//	{
//		if((right_pos[i]!=40)&&(right_pos[i]!=1)) //连续三行不丢线
//		{     
//			if((right_pos[i]-right_pos[i+1]>0))//找到右边线有拐点
//			{         
//				RightInflectionPointRow=i;//记录拐点的行
//				RightInflectionPointCol=right_pos[i];//记录拐点的列
//				RightInflectionPointFlag=1;//标记找到左拐点              
//				break;//找到退出
//			}      
//		} 
//	}
//	//在十字有可能误判成环路 误判的时候左右拐点的坐标 大概在2附近 和69附近
//	//
//	//可以同时找到两个拐点，开始识别环路(或许可以考虑一下，如果只找到一个拐点的情况，这样就能更容易识别到环路)
//
//	if(LeftInflectionPointFlag&&RightInflectionPointFlag)//同时找到两个拐点
//	{
//		StartCol=(unsigned int)((LeftInflectionPointCol+RightInflectionPointCol)/2);// 取左右拐点的列坐标平均值    
//		StartRow=(unsigned int)((LeftInflectionPointRow+RightInflectionPointRow)/2);//取左右拐点的行坐标的平均值    
////        if(StartRow<35)
////        {
//            g_point_flag=1;
////        }
////        else 
////        {
////            g_point_flag=0;
////            shizi_wait=0;
////        }	
//
//		for(i=StartRow;i<60&&g_point_flag;i++)//固定一列（StartCol）从开始的行往上扫描，寻找环路中间的圆的特征（找到一个白到黑的跳变，然后多行黑的然后到白）
//		{
//			if(!get_img_point(i,StartCol)&&get_img_point(i+1,StartCol))//跳变点的位置
//			{
//				LoopBorttomRow=i;//记录第一次跳变的行 （此时找到了白变黑的位置并记录）
//				distance=LoopBorttomRow-StartRow;
//				if(distance>8&&distance<35&&LoopBorttomRow<50)
//				{
//					LoopBorttomFlag=1;//置位标志   ;           
//					break; //到此已经找到两个拐点和一个跳变点
//				}
//				else if(LoopBorttomRow>52&&StartRow<6)
//				{
//				//           Shi_zi_flag=1;
//				//           break; 
//				}
//
//			}     
//		} 
//		//以上找到了 拐点（就是赛道宽度突变的位置） 白黑 黑白两个跳变点的位置 并且记录了
//		/********************************************************************/    
//		if(LoopBorttomFlag)//在这之前已经找到圆环的两个跳变点--判断赛道宽度的突变
//		{//
//			if(ABS(left_pos[StartRow+2]-right_pos[StartRow+2])-35>=4//4,8这两个参数需要调整
//			&&ABS(left_pos[StartRow+4]-right_pos[StartRow+4])-40>=8)//赛道宽度突变
//			{
//				LoopFlag=1;//环形赛道标志位置
//                loop_guaidian_flag=1;//说明找到两个拐点
//				LoopRightControlFlag=1;//得到环路标志位就给方向
//				enter_loop=1;
//			}                           
//		}
//	}
//    else //如果没有找到拐点，说明拐点消失，现在是底部丢线，前面一团黑
//    {
//        for(num_line_i=0;num_line_i<58;num_line_i++)
//        {
//            if(left_pos[num_line_i]==79)
//            {
//                left_total_none_num=left_total_none_num+num_line_i;
//                project_i++;
//            }
//            else
//            {
//                if(project_i>3)
//                {
//                    left_diuxian_num=left_total_none_num/project_i;
//                    break;
//                }
//            }            
//        }
//         project_i=0;
//        /******************************************************/
//        for(num_line_i=0;num_line_i<58;num_line_i++)
//        {
//            if(right_pos[num_line_i]==1)
//            {
//                right_total_none_num=right_total_none_num+num_line_i;
//                project_i++;
//            }
//            else
//            {
//                if(project_i>3)
//                {
//                    right_diuxian_num=right_total_none_num/project_i;
//                    break;
//                }
//            }            
//        }        
//        /****************************************************/
//        if(abs(right_diuxian_num-left_diuxian_num)<2)//底部丢线找到
//        {
//            //现在找前面一堆黑
//            for(num_line_i=0;num_line_i<40;num_line_i++)
//            {
//                if(!get_img_point(num_line_i,40)&&get_img_point(num_line_i+1,40))
//                {
//                    LoopBorttomRow=num_line_i;
//                    if(LoopBorttomRow<=34)//限定黑点位置 34
//                    {
//                        LoopFlag=1;
//                        break;
//                    }
//                }
//            }   
//            i=0;
//            /***********************************************/ 
//            if(LoopFlag&&LoopBorttomRow>=26)//太大了要考虑写入十字 有可能误判 25
//            {//如果判断是环路，在进一步判断
//                for(num_line_i=LoopBorttomRow;num_line_i<59;num_line_i++)//行
//                {
//                     for(StartCol=20;StartCol<61;StartCol=StartCol+10)//列  20 30 40 50 60
//                    {
//                        if(!get_img_point(num_line_i,StartCol)&&get_img_point(num_line_i+1,StartCol))
//                        {
//                            LoopBorttomRow_arrary[i++]=num_line_i;
//                            if(i>4)
//                            {
//                                break;
//                            }
//                      
//                        }                    
//                    }                
//                }  
//                //xianzhi_daxiao(int a[],int num)
//                if(xianzhi_daxiao(LoopBorttomRow_arrary,50)>=4)
//                {
//                    LoopFlag=0;
//                }
//                else if(abs(abs(LoopBorttomRow_arrary[1]-LoopBorttomRow_arrary[2])-abs(LoopBorttomRow_arrary[4]-LoopBorttomRow_arrary[3]))<4)
//                {
//                    LoopFlag=1;
//                }
//                else if(abs(LoopBorttomRow_arrary[4]-LoopBorttomRow_arrary[2])>=10)
//                {
//                    LoopFlag=0;
//                }
//            }
//            
//       }
//   
//    }
//}
void FindInflectionPoint()//向左没问题 向右 有误判
{
	char i=0;
	int distance=0;
	int g_point_flag=0;
    int num_line_i=0;
    int left_total_none_num=0,right_total_none_num=0;
    int project_i=0;
    int LoopBorttomRow_arrary[10]={0};
	//变量清零
	RightInflectionPointRow=0;//右边的拐点的行坐标
	RightInflectionPointCol=0;//右边的拐点的列坐标
	LeftInflectionPointRow=0;//左边的拐点的行坐标
	LeftInflectionPointCol=0;//左边的拐点的列坐标
	RightInflectionPointFlag=0;//找到左拐点的标志位
	LeftInflectionPointFlag=0;//找到右拐点的标志位

	StartRow=0;//从底部往上扫描圆环中间的黑圆的起始行
	StartCol=0;//从底部往上扫描圆环中间的黑圆的起始列

	LoopBorttomFlag=0;//找到圆环中间的黑圆的底部的标志位
	LoopBorttomRow=0;//圆环中间的黑圆的底部的行坐标
	LoopMilldleRow=0;//圆环中间的黑圆的顶部的行坐标
	LoopMilldleFlag=0;//找到圆环中间的黑圆的顶部的标志位
	LoopTopRow=0;//找到外环的顶部
	MilldleBlack=0;//圆环中间的黑圆的顶部和圆环中间的黑圆的底部的行坐标的平均值
	LoopFlag=0;//找到环形赛道类型的标志
   

	//左拐点
	for(i=3;i<=32;i++) //35 30
	{
		if((left_pos[i]!=40)&&(left_pos[i]!=79)) 
		{     
			if((left_pos[i]-left_pos[i+1]<0))//找到拐点
			{
				LeftInflectionPointRow=i;//记录该拐点的行           
				LeftInflectionPointCol=left_pos[i];//记录该拐点的列           
				LeftInflectionPointFlag=1;//标记找到左拐点              
				break;//找到退出                                  
			}
		}                                                                                                                                                                                                                                            
	} 

	//右拐点 
	for(i=3;i<=32;i++)//不能扫描太远，否则会误判//35 30
	{
		if((right_pos[i]!=40)&&(right_pos[i]!=1)) //连续三行不丢线
		{     
			if((right_pos[i]-right_pos[i+1]>0))//找到右边线有拐点
			{         
				RightInflectionPointRow=i;//记录拐点的行
				RightInflectionPointCol=right_pos[i];//记录拐点的列
				RightInflectionPointFlag=1;//标记找到左拐点              
				break;//找到退出
			}      
		} 
	}
	//在十字有可能误判成环路 误判的时候左右拐点的坐标 大概在2附近 和69附近
	//
	//可以同时找到两个拐点，开始识别环路(或许可以考虑一下，如果只找到一个拐点的情况，这样就能更容易识别到环路)

	if(LeftInflectionPointFlag&&RightInflectionPointFlag)//同时找到两个拐点
	{
		StartCol=(unsigned int)((LeftInflectionPointCol+RightInflectionPointCol)/2);// 取左右拐点的列坐标平均值    
		StartRow=(unsigned int)((LeftInflectionPointRow+RightInflectionPointRow)/2);//取左右拐点的行坐标的平均值    
//        if(StartRow<35)
//        {
            g_point_flag=1;
//        }
//        else 
//        {
//            g_point_flag=0;
//            shizi_wait=0;
//        }	

		for(i=StartRow;i<60&&g_point_flag;i++)//固定一列（StartCol）从开始的行往上扫描，寻找环路中间的圆的特征（找到一个白到黑的跳变，然后多行黑的然后到白）
		{
			if(!get_img_point(i,StartCol)&&get_img_point(i+1,StartCol))//跳变点的位置
			{
				LoopBorttomRow=i;//记录第一次跳变的行 （此时找到了白变黑的位置并记录）
				distance=LoopBorttomRow-StartRow;
				if(distance>8&&distance<35&&LoopBorttomRow<50)
				{
					LoopBorttomFlag=1;//置位标志   ;           
					break; //到此已经找到两个拐点和一个跳变点
				}
				else if(LoopBorttomRow>52&&StartRow<6)
				{
				//           Shi_zi_flag=1;
				//           break; 
				}

			}     
		} 
		//以上找到了 拐点（就是赛道宽度突变的位置） 白黑 黑白两个跳变点的位置 并且记录了
		/********************************************************************/    
		if(LoopBorttomFlag)//在这之前已经找到圆环的两个跳变点--判断赛道宽度的突变
		{//
			if(ABS(left_pos[StartRow+2]-right_pos[StartRow+2])-35>=4//4,8这两个参数需要调整
			&&ABS(left_pos[StartRow+4]-right_pos[StartRow+4])-40>=8)//赛道宽度突变
			{
				LoopFlag=1;//环形赛道标志位置
                loop_guaidian_flag=1;//说明找到两个拐点
				LoopRightControlFlag=1;//得到环路标志位就给方向
				enter_loop=1;
			}                           
		}
	}
    else //如果没有找到拐点，说明拐点消失，现在是底部丢线，前面一团黑
    {
        for(num_line_i=0;num_line_i<58;num_line_i++)
        {
            if(left_pos[num_line_i]==79)
            {
                left_total_none_num=left_total_none_num+num_line_i;
                project_i++;
            }
            else
            {
                if(project_i>3)
                {
                    left_diuxian_num=left_total_none_num/project_i;
                    break;
                }
            }            
        }
         project_i=0;
        /******************************************************/
        for(num_line_i=0;num_line_i<58;num_line_i++)
        {
            if(right_pos[num_line_i]==1)
            {
                right_total_none_num=right_total_none_num+num_line_i;
                project_i++;
            }
            else
            {
                if(project_i>3)
                {
                    right_diuxian_num=right_total_none_num/project_i;
                    break;
                }
            }            
        }        
        /****************************************************/
        if(abs(right_diuxian_num-left_diuxian_num)<2)//底部丢线找到
        {
            //现在找前面一堆黑
            for(num_line_i=0;num_line_i<40;num_line_i++)
            {
                if(!get_img_point(num_line_i,40)&&get_img_point(num_line_i+1,40))
                {
                    LoopBorttomRow=num_line_i;
                    if(LoopBorttomRow<=34)//限定黑点位置 34
                    {
                        LoopFlag=1;
                        break;
                    }
                }
            }   
            i=0;
            /***********************************************/
            if(LoopBorttomRow>=25)//太大了要考虑写入十字 有可能误判
            {
                for(num_line_i=LoopBorttomRow;num_line_i<59;num_line_i++)//行
                {
                     for(StartCol=20;StartCol<61;StartCol=StartCol+10)//列  20 30 40 50 60
                    {
                        if(!get_img_point(num_line_i,StartCol)&&get_img_point(num_line_i+1,StartCol))
                        {
                            LoopBorttomRow_arrary[i++]=num_line_i;
                            if(i>4)
                            {
                                break;
                            }
                      
                        }                    
                    }                
                }  
                //xianzhi_daxiao(int a[],int num)
                if(xianzhi_daxiao(LoopBorttomRow_arrary,50)>=3)
                {
                    LoopFlag=0;
                }
                else if(abs(abs(LoopBorttomRow_arrary[1]-LoopBorttomRow_arrary[2])-abs(LoopBorttomRow_arrary[2]-LoopBorttomRow_arrary[3]))<4)
                {
                    LoopFlag=1;
                }
                else if(abs(LoopBorttomRow_arrary[4]-LoopBorttomRow_arrary[2])>=10)
                {
                    LoopFlag=0;
                }
            }
            
       }
   
    }
}



///*
//x [0-80]
//y [0-60]
//*/
int xielv_k[80]={0};
void two_point_xielv(int hang[],int num)
{
    int lie=0;
    int k=0;
    for(lie=10;lie<=70-num;lie++)
    {
//      if(hang[lie]!=0)
//      {
        k=(hang[lie]-hang[lie+num])/num;
        xielv_k[lie]=k;
//      }         
    }    
}
void FindInflectionPoint_RR()
{
	char i=0;
	int distance=0;
	int g_point_flag=0;
//    int num_line_i=0;
//    int left_total_none_num=0,right_total_none_num=0;
//    int project_i=0;
//    int LoopBorttomRow_arrary[10]={0};
//    int w_h_point[80]={0},lie=0,hang=0;
//    int temp_zeor=0,temp_zeor_2=0;
//    int guaidian_lie=0;
//    int radio_out_loop_1[20],radio_out_loop_2[20];
	//变量清零
	RightInflectionPointRow=0;//右边的拐点的行坐标
	RightInflectionPointCol=0;//右边的拐点的列坐标
	LeftInflectionPointRow=0;//左边的拐点的行坐标
	LeftInflectionPointCol=0;//左边的拐点的列坐标
	RightInflectionPointFlag=0;//找到左拐点的标志位
	LeftInflectionPointFlag=0;//找到右拐点的标志位

	StartRow=0;//从底部往上扫描圆环中间的黑圆的起始行
	StartCol=0;//从底部往上扫描圆环中间的黑圆的起始列

	LoopBorttomFlag=0;//找到圆环中间的黑圆的底部的标志位
	LoopBorttomRow=0;//圆环中间的黑圆的底部的行坐标
	LoopMilldleRow=0;//圆环中间的黑圆的顶部的行坐标
	LoopMilldleFlag=0;//找到圆环中间的黑圆的顶部的标志位
	LoopTopRow=0;//找到外环的顶部
	MilldleBlack=0;//圆环中间的黑圆的顶部和圆环中间的黑圆的底部的行坐标的平均值
	LoopFlag=0;//找到环形赛道类型的标志
   

	//左拐点
	for(i=3;i<=32;i++) //35 30
	{
		if((left_pos[i]!=40)&&(left_pos[i]!=79)) 
		{     
			if((left_pos[i]-left_pos[i+1]<0))//找到拐点
			{
				LeftInflectionPointRow=i;//记录该拐点的行           
				LeftInflectionPointCol=left_pos[i];//记录该拐点的列           
				LeftInflectionPointFlag=1;//标记找到左拐点              
				break;//找到退出                                  
			}
		}                                                                                                                                                                                                                                            
	} 

	//右拐点 
	for(i=3;i<=32;i++)//不能扫描太远，否则会误判//35 30
	{
		if((right_pos[i]!=40)&&(right_pos[i]!=1)) //连续三行不丢线
		{     
			if((right_pos[i]-right_pos[i+1]>0))//找到右边线有拐点
			{         
				RightInflectionPointRow=i;//记录拐点的行
				RightInflectionPointCol=right_pos[i];//记录拐点的列
				RightInflectionPointFlag=1;//标记找到左拐点              
				break;//找到退出
			}      
		} 
	}
	//在十字有可能误判成环路 误判的时候左右拐点的坐标 大概在2附近 和69附近
	//
	//可以同时找到两个拐点，开始识别环路(或许可以考虑一下，如果只找到一个拐点的情况，这样就能更容易识别到环路)

	if(LeftInflectionPointFlag&&RightInflectionPointFlag)//同时找到两个拐点
	{
		StartCol=(unsigned int)((LeftInflectionPointCol+RightInflectionPointCol)/2);// 取左右拐点的列坐标平均值    
		StartRow=(unsigned int)((LeftInflectionPointRow+RightInflectionPointRow)/2);//取左右拐点的行坐标的平均值    
        g_point_flag=1;
		for(i=StartRow;i<60&&g_point_flag;i++)//固定一列（StartCol）从开始的行往上扫描，寻找环路中间的圆的特征（找到一个白到黑的跳变，然后多行黑的然后到白）
		{
			if(!get_img_point(i,StartCol)&&get_img_point(i+1,StartCol))//跳变点的位置
			{
				LoopBorttomRow=i;//记录第一次跳变的行 （此时找到了白变黑的位置并记录）
				distance=LoopBorttomRow-StartRow;
				if(distance>8&&distance<35&&LoopBorttomRow<50)
				{
					LoopBorttomFlag=1;//置位标志   ;           
					break; //到此已经找到两个拐点和一个跳变点
				}
				else if(LoopBorttomRow>52&&StartRow<6)
				{
				//           Shi_zi_flag=1;
				//           break; 
				}

			}     
		} 
		//以上找到了 拐点（就是赛道宽度突变的位置） 白黑 黑白两个跳变点的位置 并且记录了
		/********************************************************************/    
		if(LoopBorttomFlag)//在这之前已经找到圆环的两个跳变点--判断赛道宽度的突变
		{//
			if(ABS(left_pos[StartRow+2]-right_pos[StartRow+2])-35>=4//4,8这两个参数需要调整
			&&ABS(left_pos[StartRow+4]-right_pos[StartRow+4])-40>=8)//赛道宽度突变
			{
				LoopFlag=1;//环形赛道标志位置
                loop_guaidian_flag=1;//说明找到两个拐点
				LoopRightControlFlag=1;//得到环路标志位就给方向
				enter_loop=1;
			}                           
		}
	}
//    else //如果没有找到拐点，说明拐点消失，现在是底部丢线，前面一团黑
//    {
//        for(num_line_i=0;num_line_i<58;num_line_i++)
//        {
//            if(left_pos[num_line_i]==79)
//            {
//                left_total_none_num=left_total_none_num+num_line_i;
//                project_i++;
//            }
//            else
//            {
//                if(project_i>3)
//                {
//                    left_diuxian_num=left_total_none_num/project_i;
//                    break;
//                }
//            }            
//        }
//         project_i=0;
//        /******************************************************/
//        for(num_line_i=0;num_line_i<58;num_line_i++)
//        {
//            if(right_pos[num_line_i]==1)
//            {
//                right_total_none_num=right_total_none_num+num_line_i;
//                project_i++;
//            }
//            else
//            {
//                if(project_i>3)
//                {
//                    right_diuxian_num=right_total_none_num/project_i;
//                    break;
//                }
//            }            
//        }        
//        /****************************************************/
//        if(abs(right_diuxian_num-left_diuxian_num)<2)//底部丢线找到
//        {
//            //现在找前面一堆黑
//            for(lie=20;lie<=60;lie++)
//            {
//              for(hang=10;hang<=55;hang++)
//              {
//                if(!get_img_point(hang,lie)&&get_img_point(hang+1,lie))
//                {
//                  w_h_point[lie]=hang;//收集跳变点
//                  if(w_h_point[lie]<35)
//                  {
//                    temp_zeor++;
//                  }
//                  if(w_h_point[lie]>45)
//                  {
//                    temp_zeor_2++;
//                  }
//                  break;
//                }
//              }
//            }
//            for(lie=20;lie<58;lie++)
//            {
//                if(w_h_point[lie]<w_h_point[lie+1])//找斜率拐点
//                {
//                  guaidian_lie=lie;        
//                  break;
//                }
//            }
////            i=0;
////            for(lie=20;lie<guaidian_lie;lie=lie+5)
////            {
////                radio_out_loop_1[i++]=regression((uint16 *)w_h_point,20,lie);
////            }
//            
//            //radio_out_loop_2=regression((uint16 *)w_h_point,guaidian_lie,60);
//            if(temp_zeor>30)//入环
//            {
//                LoopFlag=1;
//            }
//            else if(temp_zeor_2>9)
//            {
//                LoopFlag=0;
//            }
//            
////            for(project_i=0;project_i<80;project_i++)//
////            {
////                if(w_h_point[project_i]!=0)
////                {
////                    temp_zeor=project_i;//找到第一个不是0的
////                }
////                else
////                {
////                    if(temp_zeor>30)
////                    {
////                        break;
////                    }
////                }            
////            }
//            /***************************************************///求两点斜率
//           // two_point_xielv(w_h_point,3);
//            
//           
//            
//       }
//   
//    }
}




       

