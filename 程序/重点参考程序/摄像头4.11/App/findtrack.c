
#include  "common.h"
#include "include.h"
#include "findtrack.h"


/*********define for SearchCenterBlackline**********/

int   MiddleLine[RowMax+1];
int   RightEdge[RowMax+1];
int   LeftEdge[RowMax+1];
int   Width[RowMax+1]={2,3,3,3,4,4,5,5,6,6,
                       8,8,10,10,12,13,14,14,16,17,
                       18,18,20,20,22,22,24,24,26,26,
                       28,30,31,32,32,34,36,36,38,39,
                       41,41,43,43,45,45,47,47,49,50,
                       50,51,52,54,55,56,57,58,59,60,61};;

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
unsigned int LeftUpExitFlag=0;
unsigned int RightUpExitFlag=0;
unsigned int  ExitFlag=0;
unsigned int  LeftExitFlag=0;
unsigned int  RightExitFlag=0;;
unsigned char ClearLoopControlFlag=0;
unsigned char LoopRightControlFlag=0;
unsigned char LoopLeftControlFlag=0;
int MotivateLoopDlayFlagR=0;
int MotivateLoopDlayFlagL=0;
unsigned char InLoopLeftInflectionPointFlag=0;
unsigned int InLoopLeftInflectionPointRow=0;
unsigned char MotivateLoopDlayFlagLOK=0;
unsigned char MotivateLoopDlayFlagROK=0;
unsigned char OpenLoopExitRepairFlagL=0;
unsigned char OpenLoopExitRepairFlagR=0;
unsigned char InLoopRightInflectionPointFlag=0;
unsigned int  InLoopRightInflectionPointRow=0;
unsigned char BreakEndL=0;
unsigned char BreakEndLFlag=0;
unsigned char BreakEndR=0;
unsigned char BreakEndRFlag=0;
unsigned char BreakStartL=0;
unsigned char BreakStartLFlag=0;
unsigned char BreakStartR=0;
unsigned char BreakStartRFlag=0;

//设置中线，左线，右线的初始化值
//设置每一行对应的赛道宽度
//本文件内不调用
void SetInitVal()
{
  int i;
  
  for(i=0;i<RowMax;i++)//赋初值             
  { 
      RightEdge[i]  = ColumnMax;
      LeftEdge[i]   = 0;
      MiddleLine[i] = ColumnMax/2;
      
  }
  
} 


//全行扫描和边缘结合提取赛道的中线
//前十行全行扫描
//后面50行，根据上一行的寻线情况来决定当前行的寻线方式和起点
//外部调用
 void SearchCenterBlackline(void)
{

  int16 i         = 0;
  int16 j         = 0;
  uint8 jj        = 0;
  uint8 WhiteNum  = 0; 
  
  LeftLose        = 0;//变量清零
  RightLose       = 0;
  AllLose         = 0;
  WhiteNum        = 0;

  MiddleLine[RowMax]  = ColumnMax/2;//第60行
  LeftEdge[RowMax]    = 0;
  RightEdge[RowMax]   = ColumnMax;
  Width[RowMax]       = ColumnMax;
  
  SetInitVal();
  for(i=RowMax-1;i>=50;i--)//首先找前十行，全行扫描
  {
    if(i ==RowMax-1)//首行就以图像中心作为扫描起点
    {
       j = MidPri;//40  
    } 
    else
    {
        j = MiddleLine[i+1];//否则就以上一行中点的位置作为本行扫描起点
    }   
    if(j <= 2)
    {
        j = 2;
    } 
    while(j >= 3)//j>=3有效范围内进行搜寻 
    {
        if(img[i][j]==White_Point&& img[i][j-1]==Black_Point && img[i][j-2]==Black_Point)//从右向左找到白白黑跳变 
        {
             LeftEdge[i] =j;//找到则赋值 找不到保持原值0      
             break;//跳出本行寻线
        }
             j--;//列数往左移动
     }
     if(i==RowMax-1) //再找右边界
     {
        j = MidPri;//如果首行，从图像中心开始搜寻
     }          
     else
     {
        j = MiddleLine[i+1];//否则从上一行中心位置开始搜寻
     }
     if(j >=ColumnMax-3)//j >=ColumnMax-2有效范围内搜寻右线
     {
        j = ColumnMax-3;
     }
     while(j <= ColumnMax-3)
     {
         
        if(img[i][j]==White_Point && img[i][j+1]==Black_Point && img[i][j+2]==Black_Point)//从左向右找到白白黑跳变点
        {
               RightEdge[i] = j;//找到则赋值   找不到保持原值
               break;//跳出本行寻线
        }
               j++;//列数往右移动
     }
     if(LeftEdge[i]!=0 && RightEdge[i]!=ColumnMax)//中线判断，没有丢线
     {
          MiddleLine[i] = (LeftEdge[i] + RightEdge[i])/2;  
     }
     else if(LeftEdge[i]!=0 && RightEdge[i]==ColumnMax)//丢了右线
     {
          RightLose++;//记录只有右线丢的数量

        if( (RightEdge[i]-LeftEdge[i]) >=(RightEdge[i+1]-LeftEdge[i+1]+1))//突变
        {
                MiddleLine[i] = MiddleLine[i+1];//用上一行的中点    
        }
        else 
        {
                MiddleLine[i] = LeftEdge[i] + Width[i]/2;//正常的话就用半宽补
        }
     }
     else if(LeftEdge[i]==0 && RightEdge[i]!=ColumnMax)//丢了左线
     {
           
        if((RightEdge[i]-LeftEdge[i]) >= (RightEdge[i+1]-LeftEdge[i+1]+1))//突变      
        {
                MiddleLine[i] = MiddleLine[i+1];//用上一行   
        } 
        else 
        {
                MiddleLine[i] = RightEdge[i] - Width[i]/2;//线宽
        }
     }
     else if(LeftEdge[i]==0 && RightEdge[i]==ColumnMax)//两边都丢了的话  
     {
         AllLose++;
             
        if(i ==RowMax-1)//如果是首行就以图像中心作为中点
        {
                MiddleLine[i] = MidPri;
        }       
        else 
        {
                MiddleLine[i] = MiddleLine[i+1];//如果不是首行就用上一行的中线作为本行中点
        }             
      }
     if(MiddleLine[RowMax-1] >=70)
     {
         MidPri = 70;
     }          
     else if(MiddleLine[RowMax-1] <= 10)
     {
         MidPri = 10;
     }         
     else
     {
          MidPri = MiddleLine[RowMax-1];//记录本帧图像第59行的中线值，作为下一幅图像的59行扫描起始点
     }
    
  }   
  for(i=49; i>0; i--)//查找剩余行
  {   
    if(LeftEdge[i+1]!=0 && RightEdge[i+1]!=ColumnMax) //上一行两边都找到 启用边沿扫描     
    {
         j = ((LeftEdge[i+1]+10) >= ColumnMax-2)? ColumnMax-2:(LeftEdge[i+1]+10);//先找左边界    
         jj = ((LeftEdge[i+1]-5) <= 1)? 1:(LeftEdge[i+1]-5);        
      while(j >= jj)       
      {       
          if(img[i][j]==White_Point && img[i][j-1]==Black_Point)  
          {
               LeftEdge[i] = j;
               break;
          }   
         j--;   
      }
         j = ((RightEdge[i+1]-10) <= 1)? 1:(RightEdge[i+1]-10); //在找右边界   
         jj = ((RightEdge[i+1]+5) >= ColumnMax-2)? ColumnMax-2:(RightEdge[i+1]+5);    
      while(j <= jj)             
      {
          if(img[i][j]==White_Point&& img[i][j+1]==Black_Point) 
          {
               RightEdge[i] = j;
               break;    
          }
         j++;
      }
    }
    else if(LeftEdge[i+1]!=0 && RightEdge[i+1]==ColumnMax)//上一行只找到左边界   
    {      
         j  = ((LeftEdge[i+1]+10) >=ColumnMax-2)? ColumnMax-2:(LeftEdge[i+1]+10);//左边界用边沿扫描   
         jj = ((LeftEdge[i+1]-5) <= 1)? 1:(LeftEdge[i+1]-5);              
      while(j >= jj)   
      {     
          if(img[i][j]==White_Point && img[i][j-1]==Black_Point)
          {
               LeftEdge[i] = j;
               break;
          }
         j--;   
      }     
         j = MiddleLine[i+1];//上一行丢了右边界用全行扫描 
      if(j >= 78)
      {
           j = 78;
      }
      while(j <= ColumnMax-2)      
      {    
          if(img[i][j]==White_Point && img[i][j+1]==Black_Point)
          {
               RightEdge[i] = j;
               break;
          }
         j++;
      }
           
    }
    else if(LeftEdge[i+1]==0 && RightEdge[i+1]!=ColumnMax) //上一行只找到右边界      
    {
         j = ((RightEdge[i+1]-10) <= 1)?1:(RightEdge[i+1]-10);//边缘追踪找右边界 
         jj = ((RightEdge[i+1]+5) >= ColumnMax-2)? ColumnMax-2:(RightEdge[i+1]+5);            
       while(j <= jj)  
       {    
            if(img[i][j]==White_Point&&img[i][j+1]==Black_Point)
            {
                 RightEdge[i] = j;
                 break;
            }
           j++;     
       }   
           j = MiddleLine[i+1]; //全行扫描找左边界
        if(j < 2)
        {
             j = 2;
        }  
       while(j >= 1)  
       {  
            if(img[i][j]==White_Point && img[i][j-1]==Black_Point)       
            {
                 LeftEdge[i] = j;
                 break;
            }
           j--;      
       }           
     } 
    else if(LeftEdge[i+1]==0 && RightEdge[i+1]==ColumnMax)  //上一行没找到边界，可能是十字或者环形  
    {
        
          j = MiddleLine[i+1];   //找全行找左边界
        while(j >= 1)  
        {
             if(img[i][j]==White_Point && img[i][j-1]==Black_Point)     
             {
                  LeftEdge[i] = j;
                  break;
             }
            j--;
        }
         j = MiddleLine[i+1];   //全行找右边界   
       while(j <=ColumnMax-2)       
       {   
            if(img[i][j]==White_Point&&img[i][j+1]==Black_Point)
            {
                  RightEdge[i] = j;  
                  break;
            }
           j++;
              
       }         
    }
    if( (RightEdge[i]-LeftEdge[i]) >= (RightEdge[i+1]-LeftEdge[i+1]+1) )//不满足畸变 
    {      
          MiddleLine[i] = MiddleLine[i+1];//用上一行
    }
    else
    {
            if(LeftEdge[i]!=0 && RightEdge[i]!=ColumnMax)   
            {
                     MiddleLine[i] = (LeftEdge[i] + RightEdge[i])/2;
                     
                     //对斜出十字进行纠正
                     
                 if( MiddleLine[i]-MiddleLine[i+1]>8&&((ABS(LeftEdge[i]-LeftEdge[i+1]>3)||ABS(RightEdge[i]-RightEdge[i+1]>3)) )&& i>=30)//中线向右突变
                 {
                           uint8 ii = i;
                    
                      while(1)
                      {
                               MiddleLine[ii+1] = MiddleLine[ii]-1; 
                               ii++; 
                               
                           if( ii>=50 || (MiddleLine[ii]-MiddleLine[ii+1]<=1) )
                           {
                                 break;
                           }          
                       }
                 }
                 if( (MiddleLine[i+1]-MiddleLine[i]>8)&&((ABS(LeftEdge[i]-LeftEdge[i+1]>3)||ABS(RightEdge[i]-RightEdge[i+1]>3)))&& i>=30)
                 {
                           uint8 ii = i;
                          
                      while(1)
                      {
                               MiddleLine[ii+1] = MiddleLine[ii]+1;
                               ii++;
                               
                           if( ii>=50 || (MiddleLine[ii+1]-MiddleLine[ii]<=1) )
                           { 
                                 break;
                           }       
                       }
                  }
            }
            else if(LeftEdge[i]!=0 && RightEdge[i]==ColumnMax)//find left
            { 
                         RightLose++;
                         
                     
                 if(LeftEdge[i+1] != 0)
                 {
                               MiddleLine[i] = MiddleLine[i+1]+LeftEdge[i]-LeftEdge[i+1];
                 }        
                 else
                 {
                                MiddleLine[i]  = LeftEdge[i] + Width[i]/2;
                 }
            }
            
            else if(LeftEdge[i]==0 && RightEdge[i]!=ColumnMax)//find right
            {
                          LeftLose++;
                          
                      
                 if(RightEdge[i+1] !=ColumnMax)
                 {
                  
                                 MiddleLine[i] = MiddleLine[i+1]+RightEdge[i]-RightEdge[i+1];
                 }
                 else
                 {
                 
                                 MiddleLine[i] = RightEdge[i] - Width[i]/2;
                 }
            }
           else if(LeftEdge[i]==0 && RightEdge[i]==ColumnMax)//两边丢线    
           {      
                         WhiteNum++;
                         AllLose++;
                
                 if(WhiteNum == 1)
                 {   
                                  WhiteStart = i;
                 }         
                        MiddleLine[i] = MiddleLine[i+1];
           }
            
       }
       
    if(i == 0)             
    {            
            AvaliableLines = 60;
            LastLine  = 0;  
            break;
    }
           uint16 m = MiddleLine[i];
    if(m < 5)
    { 
            m = 5;
    }
    if(m > 75)
    {
            m = 75;
    }
    if( (LeftEdge[i]!=0 && LeftEdge[i]>=65) || (RightEdge[i]!=ColumnMax && RightEdge[i]<15) || (i>=1)&&(img[i-1][m]== Black_Point)) //最后一行              
    {
            LastLine = i;//最后一行，动态前瞻
            AvaliableLines = 60 - i;//有效行数
            break;
    }           
  }
}
 
  
 //提取图像的特征
//选取几列，从图像底部往上扫描
//内部调用
void GetBlackEndParam()//获取黑线截止行
{
  
  unsigned char LEndFlag  = 0;//标志位
  unsigned char MEndFlag  = 0;
  unsigned char REndFlag  = 0;	
  unsigned char MREndFlag = 0;
  unsigned char MLEndFlag = 0;
  unsigned char LLEndFlag = 0;
  unsigned char RREndFlag = 0;

  int i=0;

  BlackEndMR   = 0;//清零
  BlackEndML   = 0;
  BlackEndLL   = 0;
  BlackEndRR   = 0;
  BlackEndL    = 0;
  BlackEndM    = 0;
  BlackEndR    = 0;
  
  for (i = RowMax-1; i >= 3 ; i--)
  {
	if(img[i][ColumnMax/2] == White_Point && !MEndFlag )//!MEndFlag=1 //40
        {
		BlackEndM++;//中黑线截至行
        }
	else if(i > 1 && img[i-1][ColumnMax/2] == Black_Point && img[i-2][ColumnMax/2] == Black_Point)//连续两行是黑色        
        {
		MEndFlag = 1;
        }
	if(img[i][ColumnMax/4] == White_Point && !LEndFlag )//20
        {
		BlackEndL++;//左黑线截至行
        }
	else if(i > 1 && img[i-1][ColumnMax/4] == Black_Point && img[i-2][ColumnMax/4] == Black_Point)
        {
		LEndFlag = 1;
        }
	if(img[i][ColumnMax*3/4] == White_Point && !REndFlag )//60
        {
		BlackEndR++;//右黑线截至行
	}
	else if(i > 1 && img[i-1][ColumnMax*3/4] == Black_Point && img[i-2][ColumnMax*3/4] == Black_Point)
        {
		REndFlag = 1;
        }
        if(img[i][30] == White_Point && !MLEndFlag )
        {
		BlackEndML++;
        }
	else if(i > 1 && img[i-1][30] == Black_Point && img[i-2][30] == Black_Point)
        {
		MLEndFlag = 1;
        }
	if(img[i][50] == White_Point && !MREndFlag )
        {
		BlackEndMR++;
        }
	else if(i > 1 && img[i-1][50] == Black_Point && img[i-2][50] == Black_Point)
        {
		MREndFlag = 1;
	}
        if(img[i][10] == White_Point && !LLEndFlag )
        {
		BlackEndLL++;
	}
	else if(i > 1 && img[i-1][10] == Black_Point && img[i-2][10] == Black_Point)
        {
		LLEndFlag = 1;
	}
        if(img[i][70] == White_Point && !RREndFlag )
        {
		BlackEndRR++;
	}
	else if(i > 1 && img[i-1][70] == Black_Point && img[i-2][70] == Black_Point)
        {
		RREndFlag = 1;
	}
   }
        
    
        BlackEndMax =MAX(BlackEndL,BlackEndM);//取大值
        BlackEndMax =MAX(BlackEndMax,BlackEndR);
        BlackEndMaxMax =MAX(BlackEndMax,BlackEndMR);
        BlackEndMaxMax =MAX(BlackEndMax,BlackEndML);
        BlackEndMaxMax =MAX(BlackEndMax,BlackEndLL);
        BlackEndMaxMax =MAX(BlackEndMax,BlackEndRR);
        if(BlackEndMaxMax>=60)
        {
            BlackEndMaxMax=58;
        }
        DropRow=60-BlackEndMaxMax;//封顶的行数      
 }
 


#if 1

//找左右拐点，识别环
//一直都要开着

void FindInflectionPoint()
{
  
 char i=0;
 char i_i=0;
 char i_i_i=0;
  
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

  //右拐点
  
  for(i=59;i>=25;i--)//不能扫描太远，否则会误判
  {
     if(RightEdge[i]!=ColumnMax&&RightEdge[i-1]!=ColumnMax&&RightEdge[i+1]!=ColumnMax) //连续三行不丢线
     {
     
      if((RightEdge[i]-RightEdge[i+1]<=0)&&(RightEdge[i-1]-RightEdge[i])>0)//找到右边线有拐点
       {
         
           RightInflectionPointRow=i;//记录拐点的行
           RightInflectionPointCol=RightEdge[i];//记录拐点的列
           RightInflectionPointFlag=1;//标记找到右拐点
           break;//退出for
       }
      
       } 
  }
  
    //左拐点
   for(i=59;i>25;i--) 
   {
     if(LeftEdge[i]!=0&&LeftEdge[i-1]!=0&&LeftEdge[i+1]!=0)  
     {
     
      if((LeftEdge[i]-LeftEdge[i+1]>=0)&&(LeftEdge[i-1]-LeftEdge[i]<0))//找到拐点
       {
           LeftInflectionPointRow=i;//记录该拐点的行
           
           LeftInflectionPointCol=LeftEdge[i];//记录该拐点的列
           
           LeftInflectionPointFlag=1;//标记找到左拐点
              
           break;//找到退出
       }
       }                                                                                                                                                                                                                                            
   } 
  
  //可以同时找到两个拐点，开始识别环路(或许可以考虑一下，如果只找到一个拐点的情况，这样就能更容易识别到环路)
  
 if(LeftInflectionPointFlag&&RightInflectionPointFlag)//同时找到两个拐点
  {
     StartCol=(unsigned int)((LeftInflectionPointCol+RightInflectionPointCol)/2);// 取左右拐点的列坐标平均值
    
     StartRow=(unsigned int)((LeftInflectionPointRow+RightInflectionPointRow)/2);//取左右拐点的行坐标的平均值
    
    for(i=StartRow;i>=8;i--)//固定一列（StartCol）从开始的行往上扫描，寻找环路中间的圆的特征（找到一个白到黑的跳变，然后多行黑的然后到白）
    {
      if(img[i][StartCol]==White_Point&&img[i-1][StartCol]==Black_Point)
      {
 
         LoopBorttomRow=i-1;//记录第一次跳变的行

         LoopBorttomFlag=1;//置位标志
         
         break;
      }
      
    }
    
    if(LoopBorttomFlag)//如果找到了一个跳变
    {
         i_i=i-2;//从上一个跳变的行开始，再向上寻找黑->白跳变
        
           while(i_i>=8)
           {
               if(img[i_i][StartCol]==Black_Point&&img[i_i-1][StartCol]==White_Point)
               {
                 
                 
                    LoopMilldleRow=i_i-1;//记录第二个跳变的行坐标
                    LoopMilldleFlag=1;//标志位置位
                    break; //退出while   
                     
                }
               
                i_i--; 
               
              }   
         }
      
    
   if(LoopMilldleFlag)//找到圆环中间的黑圆的顶部
    {
        i_i_i=i_i-1;
        
       while(i_i_i>8)
       {
          if(img[i_i_i][StartCol]==White_Point&&img[i_i_i-1][StartCol]==Black_Point)//由白到黑跳变，找到
          {
             LoopTopRow=i_i_i-1;
             
               if(LoopBorttomRow-LoopMilldleRow>=4&&AllLose<1&&(LoopMilldleRow-LoopTopRow)<=5)//判断一下圆的黑点数
                {
                 
                  
                  if(ABS(LeftEdge[StartRow-2]-RightEdge[StartRow-2])-Width[StartRow-2]>=6&&ABS(LeftEdge[StartRow-3]-RightEdge[StartRow-3])-Width[StartRow-3]>=10&&ABS(LeftEdge[StartRow-4]-RightEdge[StartRow-4])-Width[StartRow-4]>=12)//赛道宽度突变
                  {
                     LoopFlag=1;//环形赛道标志位置
                     break;
                  }
                }
                
          }
          
          i_i_i--;
       }
     }
   }     
}
//底下几行丢线，面向一团黑圆


#endif



#if 1

//寻找环形出口
//找到圆的4个切点
//将中间的黑块横切成两块  

void FindLoopExit()

{
     int i,j;
     unsigned char i_i;
     unsigned char RightScanFlag;
     unsigned char LeftScanFlag;
      
     LoopLeft=0;
     MilldlePonit=0;
     LoopRight=0;
     LoopRightR=0;
     LoopLeftL=0;
     LeftUpExitFlag=0;
     RightUpExitFlag=0;
     ExitFlag=0;
     RightExitFlag=0;
     LeftExitFlag=0;
     RightScanFlag=0;
     LeftScanFlag=0;
     
    
  if(LoopFlag)
  {
    
    //从黑圆的中间往两边寻找跳变
    
     MilldlePonit=LoopBorttomRow-((LoopBorttomRow-LoopMilldleRow)/2);
     
     for(j=StartCol;j<76;j++)
     {
       if(img[MilldlePonit][j]==Black_Point&&img[MilldlePonit][j+1]==Black_Point&&img[MilldlePonit][j+2]==White_Point)
       {
       
           LoopRight=j+1;
           //break;
       }
       
       unsigned char j_j=j+2;
       
       if(LoopRight)
       {
       
         while(j_j<76)
         {
           if(img[MilldlePonit][j_j]==White_Point&&img[MilldlePonit][j_j+1]==White_Point&&img[MilldlePonit][j_j+2]==Black_Point)
           {
               LoopRightR=j_j+2;

               break;
           }
           
           j_j++;
         }
       
      }
      
      if(LoopRightR)
      {
         break;
      
      }
    }
     
     for(j=StartCol;j>3;j--)
     {
        if(img[MilldlePonit][j]==Black_Point&&img[MilldlePonit][j-1]==Black_Point&&img[MilldlePonit][j-2]==White_Point)
       {
       
           LoopLeft=j-1;
           //break;
       }
       
          unsigned char jj=j-2;
       
       if(LoopLeft)
       {
       
         while(jj>3)
         {
           if(img[MilldlePonit][jj]==White_Point&&img[MilldlePonit][jj-1]==White_Point&&img[MilldlePonit][jj-2]==Black_Point)
           {
               LoopLeftL=jj-2;
               break;
           }
           
           jj--;
         }
       
        }
       
       if(LoopLeftL)
       {
          break;
       }
     }
     
     //先从上面找出口
     
    
     for(i=0;i<80;i++)
     {
     
       BigLooptUp[i]=0;
     }
     
     for(j=LoopLeft-10;j<LoopRight+10;j++)
     {
        for(i=MilldlePonit;i>5;i--)
        {
          if(img[i][j]==White_Point&&img[i-1][j]==Black_Point)
          {
           BigLooptUp[j]=i-1;
           break;
          }
  
        }
     
     }
     
    for(j=2;j<75;j++)
     {
       
       if(BigLooptUp[j]!=0&&BigLooptUp[j+1]!=0&&BigLooptUp[j-1]!=0)
       {
          if(ABS(BigLooptUp[j+1]-BigLooptUp[j])>=2&&ABS(BigLooptUp[j+1]-BigLooptUp[j-1])>=2)
          {

              ExitFlag=j;
              break;

          }
          else
          {
            ExitFlag=0;
          }
          
         }
   
      }
     
     if(ExitFlag!=0)
     {
     
         if(ExitFlag>=StartCol)
         {
           RightUpExitFlag=1;
         
         }
         else if(ExitFlag<StartCol)
         {
         
          LeftUpExitFlag=1;
         }
     
       }
     
 //如果上面没有找到出口，就看看有没有丢一边 
     
     
        for(i=0;i<60;i++)
        {
     
          BigLoopLeftUp[i]=0;
          BigLoopRightUp[i]=0;
        }
    
 if((!LeftUpExitFlag)&&(!RightUpExitFlag)&&LoopFlag&&LoopLeftControlFlag==0&&LoopRightControlFlag==0)//这里改过
 {
      
       if(StartCol>=39)
       {
         LeftScanFlag=1;//标志往左边找出口
         
        for(i=StartRow;i>LoopMilldleRow-5;i--)
        { 
         
         for(j=LoopLeft-2;j>1;j--)
         {
           
           if(img[i][j+1]==White_Point)//先判断前一行是不是白的
           {
           
           if(img[i][j]==White_Point&&img[i][j-1]==Black_Point)//由白到黑跳变
           {
           
             BigLoopLeftUp[i]=j-1;
             break;
           }
           
           }
           
           else
             
           {
              break;
           
           }
         }
         
         
       }
       
      for(i=StartRow-1;i>LoopMilldleRow-5;i--)
         {
           
            if(BigLoopLeftUp[i]!=0&&BigLoopLeftUp[i-1]==0)
            {
             
                 BreakStartL=BigLoopLeftUp[i];
                 BreakStartLFlag=1;
                 break;
             }
            else
            {
               BreakStartL=0;
               BreakStartLFlag=0;
            }
         }

            if(BreakStartLFlag)
            {
              
              i_i=i-1;
            
            while(i_i>LoopMilldleRow-5)
              
            {
              if(BigLoopLeftUp[i_i]==0&&BigLoopLeftUp[i_i-1]!=0)
               {
              
                 BreakEndL=BigLoopLeftUp[i-1];
                 BreakEndLFlag=1;
                 break;
               }
              
              else
              {
                
                 BreakEndL=0;
                 BreakEndLFlag=0;
              
              }
            
              i_i--;
            }
            
            }
                 
              if(BreakEndLFlag&&BreakStartLFlag) 
              {
                   if(ABS(BreakStartL-BreakEndL>=6))
                   {
                       LeftExitFlag=1;
                       //break;
                   }
              
              }       
     }
        
       
      else if(StartCol<39)
      {
        RightScanFlag=1;//标志往右边找出口
        
       for(i=StartRow;i>LoopMilldleRow-5;i--)
       {
         
         for(j=LoopRight+2;j<79;j++)
         {
           
           if(img[i][j-1]==White_Point)
           {
             
           if(img[i][j]==White_Point&&img[i][j+1]==Black_Point)
           {
           
             BigLoopRightUp[i]=j-1;
             break;
           }
           }
           else
           {
              break;
           
           }
         
         }
       }
       
       for(i=StartRow-1;i>LoopMilldleRow-5;i--)
         {
           
            if(BigLoopRightUp[i]!=0&&BigLoopRightUp[i-1]==0)
            {
             
                 BreakStartR=BigLoopRightUp[i];
                 BreakStartRFlag=1;
                 break;
             }
            else
            {
               BreakStartR=0;
               BreakStartRFlag=0;
            }
         }
            
            if(BreakStartRFlag)
            {
              
                  i_i=i-1;
                
                while(i_i>LoopMilldleRow-5)
                  
                {
                  if(BigLoopRightUp[i_i]==0&&BigLoopRightUp[i_i-1]!=0)
                   {
                  
                     BreakEndR=BigLoopLeftUp[i-1];
                     BreakEndRFlag=1;
                     break;
                   }
                  
                  else
                  {
                    
                     BreakEndR=0;
                     BreakEndRFlag=0;
                  
                  }
                
                  i_i--;
                }
            
            }
                 
              if(BreakEndRFlag&&BreakStartRFlag) 
              {
                   if(ABS(BreakStartR-BreakEndR>=6))
                   {
                       RightExitFlag=1;
                       //break;
                   }
              
              }          
        
 }
 
 
 if(RightScanFlag&&!RightExitFlag)//如果找了右边是否有出口，并且在右边没有找到出口
        {
            LeftExitFlag=1;//默认出口在左边
        }
        
        else if(LeftScanFlag&&!LeftExitFlag)
        {
           RightExitFlag=1;
        
        }

    }    
    
   }
 
}

#endif


#if 1

//环路的控制
//外部调用

void LoopControl()
{

  static int x,y;

  
   if((LeftUpExitFlag||LeftExitFlag)&&!(RightUpExitFlag||RightExitFlag)&&LoopLeftControlFlag==0&&LoopRightControlFlag==0)//出口在左边
   {
     x++;
     if(x>=3)//滤波
     {
       x=0; 
       LoopLeftControlFlag=1;//获取环路左边的控制标志位
     }
   }
  
   
   if(!(LeftUpExitFlag||LeftExitFlag)&&(RightUpExitFlag||RightExitFlag)&&LoopLeftControlFlag==0&&LoopRightControlFlag==0)
   {
     
     y++;
    if(y>=3)
     {
       LoopRightControlFlag=1;//获取环路右边的控制标志位
       y=0;
     }
   
    
   }
           
}

#endif


#if 1

//环形赛道入口补线

void LoopRepair()
{
   int i;

  
   if(LoopRightControlFlag)//如果获得了出口在右边的控制信号
   {
     
      for(i=58;i>15;i--)
     {

        MiddleLine[i]=RightEdge[i]-Width[i]/2;
     
      }
      
      
     if(LeftEdge[58]==0&&LeftEdge[57]==0&&LeftEdge[56]==0)//底部几行全部丢线
     
     {
  
       ClearLoopControlFlag=1;//清掉环道控制标志
       
        for(i=58;i>15;i--)
      {

        MiddleLine[i]=RightEdge[i]-Width[i]/2;
       }
   
    }

    if(ClearLoopControlFlag&&LeftEdge[45]!=0&&LeftEdge[46]!=0)//清标志
    {
      // gpio_set   (PTB19, 0);
       LoopRightControlFlag=0;
       LoopLeftControlFlag=0;
       ClearLoopControlFlag=0;
       MotivateLoopDlayFlagR=1;
    }
   
   
  }
  
  //左边
  
  
  else if(LoopLeftControlFlag)
  {
     for(i=58;i>15;i--)
     {
        MiddleLine[i]=LeftEdge[i]+Width[i]/2;
     
      }
      
      
     if(RightEdge[58]==80&&RightEdge[57]==80&&RightEdge[56]==80)
     
     {
  
       ClearLoopControlFlag=1;
       
        for(i=58;i>15;i--)
      {

        MiddleLine[i]=LeftEdge[i]+Width[i]/2;
       }
   
    }
 
   

    if(ClearLoopControlFlag&&RightEdge[45]!=80&&RightEdge[46]!=80)
    {
       LoopRightControlFlag=0;
       LoopLeftControlFlag=0;
       ClearLoopControlFlag=0;
       MotivateLoopDlayFlagL=1;//延时开始
    }
   
 
  }

}




#endif

//环路出口补线处理

void LoopExitRepair()
{
  
  int i;
  static unsigned char  ClearInLoopLeftInflectionPointFlag=0;
  static unsigned char  ClearInLoopRightInflectionPointFlag=0;
  static unsigned char  OpenLoopExitRepairNumR=0;
  static unsigned char  OpenLoopExitRepairNumL=0;
  static unsigned char  LastInLoopLeftInflectionPointFlag=0;
  static unsigned char  LastLastInLoopLeftInflectionPointFlag=0;
  static unsigned char  LastInLoopRightInflectionPointFlag=0;
  static unsigned char  LastLastInLoopRightInflectionPointFlag=0;
  static unsigned char  InLoopDlay=0;
  
  
  if(MotivateLoopDlayFlagL&&OpenLoopExitRepairFlagL==0&&MotivateLoopDlayFlagR==0)//在环道里面,关了环道识别和十字识别
  {
        OpenLoopExitRepairNumL++;//开个小延时
          
       if(OpenLoopExitRepairNumL>=6)//延时够了
        {
           OpenLoopExitRepairNumL=0;
           OpenLoopExitRepairFlagL=1;//在环路里面打开出口寻找函数

        }
      
   }
 
  if(OpenLoopExitRepairFlagL)//左边
  {
    
     LastLastInLoopLeftInflectionPointFlag=LastInLoopLeftInflectionPointFlag; 
     LastInLoopLeftInflectionPointFlag=InLoopLeftInflectionPointFlag;
   
      
     for(i=59;i>20;i--) //前面40行
   {
     
     if(LeftEdge[i]!=0&&LeftEdge[i-1]!=0&&LeftEdge[i+1]!=0)      
     {
         if((LeftEdge[i]-LeftEdge[i+1]>=0)&&(LeftEdge[i-1]-LeftEdge[i]<=-1))
        
       {
         
         InLoopLeftInflectionPointRow=i-1;
         InLoopLeftInflectionPointFlag=1;
         break;
        
       }
       else
         
       {
       
         InLoopLeftInflectionPointRow=0;
         
         InLoopLeftInflectionPointFlag=0;
       }
       
       }
       
     }

     if(InLoopLeftInflectionPointFlag=1&&(InLoopLeftInflectionPointRow>=25))
     {

        ClearInLoopLeftInflectionPointFlag=1;
        
     }
     
     if(ClearInLoopLeftInflectionPointFlag)
     {
         gpio_set (PTB19, 1);
         
          for(i=58;i>15;i--)
        {

         MiddleLine[i]=LeftEdge[i]+Width[i]/2;
        
        }
        
       InLoopDlay++;//启动延时保险清标志
        
        if(InLoopDlay>=100)//如果这麽久还没满足清标志的条件,强制清标志
      {
         gpio_set (PTB19, 0);  
         ClearInLoopLeftInflectionPointFlag=0;
         LastLastInLoopLeftInflectionPointFlag=0;
         LastInLoopLeftInflectionPointFlag=0;
         MotivateLoopDlayFlagR=0;
         MotivateLoopDlayFlagL=0;
         InLoopDlay=0;
         OpenLoopExitRepairFlagL=0;
         OpenLoopExitRepairFlagR=0;
 
       }
   
     
     }
  
    if(ClearInLoopLeftInflectionPointFlag&&(LastInLoopLeftInflectionPointFlag&&LastLastInLoopLeftInflectionPointFlag&&!InLoopLeftInflectionPointFlag))
    {
         gpio_set (PTB19, 0);  
         ClearInLoopLeftInflectionPointFlag=0;
         LastLastInLoopLeftInflectionPointFlag=0;
         LastInLoopLeftInflectionPointFlag=0;
         MotivateLoopDlayFlagR=0;
         MotivateLoopDlayFlagL=0;
         InLoopDlay=0;
         OpenLoopExitRepairFlagL=0;
         OpenLoopExitRepairFlagR=0;
     }
    
    }

  //右边
  if(MotivateLoopDlayFlagR&&OpenLoopExitRepairFlagR==0&&MotivateLoopDlayFlagL==0)//在环道里面,关了环道识别和十字识别
  {
    
     OpenLoopExitRepairNumR++;//开个小延时
      
   if(OpenLoopExitRepairNumR>=6)
    {
       OpenLoopExitRepairNumR=0;
       OpenLoopExitRepairFlagR=1;
    }
  
  }
  
  
  
  if(OpenLoopExitRepairFlagR)
  {
    
     LastLastInLoopRightInflectionPointFlag=LastInLoopRightInflectionPointFlag; 
     LastInLoopRightInflectionPointFlag=InLoopRightInflectionPointFlag;

   for(i=59;i>20;i--) 
    {
     
     if(RightEdge[i]!=80&&RightEdge[i-1]!=80&&RightEdge[i+1]!=80)      
     {
       if(RightEdge[i]-RightEdge[i+1]<=0&&(RightEdge[i-1]-RightEdge[i]>=1))//找到拐点
       {
         InLoopRightInflectionPointRow=i-1;
         
         InLoopRightInflectionPointFlag=1;
         break;
       }
       else
         
        {
       
         InLoopRightInflectionPointRow=0;
         
         InLoopRightInflectionPointFlag=0;
        }
       }
       
     }

     if(InLoopRightInflectionPointFlag=1&&(InLoopRightInflectionPointRow>=25))
     {
       
        ClearInLoopRightInflectionPointFlag=1;
 
     }
     if(ClearInLoopRightInflectionPointFlag)
     {
        gpio_set (PTB19, 1);
       
       for(i=58;i>15;i--)
        {

         MiddleLine[i]=RightEdge[i]-Width[i]/2;
         
        }
       
       InLoopDlay++;//启动延时保险清标志
       
      if(InLoopDlay>=100)//如果这麽久还没满足清标志的条件,强制清标志
      {
        
         gpio_set (PTB19, 0);
         ClearInLoopRightInflectionPointFlag=0;
         LastInLoopRightInflectionPointFlag=0;
         LastLastInLoopRightInflectionPointFlag=0;
         InLoopDlay=0;
         MotivateLoopDlayFlagR=0;
         MotivateLoopDlayFlagL=0;
         OpenLoopExitRepairFlagL=0;
         OpenLoopExitRepairFlagR=0;
      }
     
     }
     
  
    if(ClearInLoopRightInflectionPointFlag&&(LastInLoopRightInflectionPointFlag&&LastLastInLoopRightInflectionPointFlag&&(!InLoopRightInflectionPointFlag)))
      
    {
         gpio_set (PTB19, 0);
         ClearInLoopRightInflectionPointFlag=0;
         LastInLoopRightInflectionPointFlag=0;
         LastLastInLoopRightInflectionPointFlag=0;
         InLoopDlay=0;
         MotivateLoopDlayFlagR=0;
         MotivateLoopDlayFlagL=0;
         OpenLoopExitRepairFlagL=0;
         OpenLoopExitRepairFlagR=0;
      }
    
    }           
}



       

