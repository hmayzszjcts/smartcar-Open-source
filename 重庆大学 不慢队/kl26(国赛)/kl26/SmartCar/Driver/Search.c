#include "include.h"
#define left 0
#define right 1
int Max_Peak,Left,Right;
int leftfind,rightfind;    //左右边沿都找到了，但是不能直接赋值的那种
float Middle_Err;
int   CCD_Diff[128],RisingEdgeIndex,FallingEdgeIndex;
uint8 RisingEdgeCnt,FallingEdgeCnt,RisingEdge[5],FallingEdge[5];

float  Previous_Width[15];
float  Previous_Angle[15];
float  Previous_Error[15]; 
int    Both_Line_find_cnt=0;

uint8 state_change=0;

uint8 Hill_Flag=0;

uint8 Leftlast,Rightlast,Leftlastfind=0,Rightlastfind=0;
int Reference_Width=50;             //参考宽度
float Normal_Width=0;                 //正常赛道的宽度
int  Right_Or_Left;
int Turn_To_Angle=0;
uint8 Strong_Turn_Cnt=0,Strong_Turn_En=0;
float Character_Distance=0;
float Last_Singleine_Position;
float Last_Angle_Turn_Position=0;
int Turn_State;
int Turn_State_Cnt=0;


float Last_Hill_Distance=0;


uint8 Single_Out_Detect=0;

int Turn_State1To2_Left,Turn_State1To2_Right;
uint8 Refind_Cnt=0;

uint8 Strong_Turn_Angle;
uint8 Strong_Turn_Speed;

uint8 Barri_Turn_Angle;
uint8 Barri_Turn_Speed;

uint8 curve_to_cross=0;

uint8 RoadType;
uint8 RoadTypeConfirm;             //赛道类型确认
uint8 Threshold;                 //阈值
 
uint8 RoadTypeSwitch=40;

void RoadType_Distinguish()
{
  uint8 i=0;
  int left_c,right_c;
  static float middle_last;
  Max_Peak=0;
  RisingEdgeCnt=0;
  FallingEdgeCnt=0;
  for(i=0;i<5;i++)
  {
    RisingEdge[i]=0;
    FallingEdge[i]=0;  
  }
  for(i=3;i<128;i++)
  {
    CCD_Diff[i]= CCD_Buff[i]- CCD_Buff[i-3];
    if(ABS(CCD_Diff[i])>Max_Peak) Max_Peak=ABS(CCD_Diff[i]); //求出最大的差分值
  }
  
  for(i=4;i<127;i++)
  {
    if((CCD_Diff[i]>=CCD_Diff[i-1])&&(CCD_Diff[i]>CCD_Diff[i+1])&&(CCD_Diff[i]>Threshold)) //寻找正的峰值，左边线
    {
      if(RisingEdgeCnt<5)    //一行图像最多有5个上跳沿
      { 
       RisingEdge[RisingEdgeCnt]=i;
       RisingEdgeCnt++;  
      }
    } 
    if((CCD_Diff[i]<CCD_Diff[i-1])&&(CCD_Diff[i]<=CCD_Diff[i+1])&&(CCD_Diff[i]<-Threshold))  //寻找负的峰值，右边线
    {
      if(FallingEdgeCnt<5)    //一行图像最多有5个下跳沿
      {
       FallingEdge[FallingEdgeCnt]=i;
       FallingEdgeCnt++;
      }
    }
  }
  if(Distance-Last_Singleine_Position>0.5&&RoadType==0) Prejudge();     //赛道类型预判
  switch(RoadType)
  {
   case 0:
    Character_Distance=0;
    Normal();
    
    Is_Straight_Angle_Turn();  //判断是否是弯入直角 
    Is_Barraicade();  
    
     if(RoadType==1)
    { 
      curve_to_cross=0;
    }
    else Is_CrossRoad();
    Single_Out_Detect=1;
    Is_Single_Line();      //检测边线外部是否有线
    Single_Out_Detect=0;
    ////////////////////////////////////////////////////////////////////////////////////////////????????????????????????????????????????????????????????????????????
    if(((Rightlastfind==0)||(Leftlastfind==0))&&(Distance-Last_Hill_Distance<2)&&(RoadTypeSwitch&64))
    {
      
      Character_Distance=Distance-1;   
      state_change=0;     
      RoadType=4;
      Turn_State=1;  
      Refind_Cnt=0;
      Strong_Turn_En=1;
      Strong_Turn_Cnt=0;
      Turn_Angle_Integral=0;    
      Turn_State1To2_Left=Left;
      Turn_State1To2_Right=Right;
     
      if(Leftlastfind==0)
      {
        Right_Or_Left=1; //左直角 
      }
      else  Right_Or_Left=-1; //右直角 
      
    }
      

   break;
   case 1:
    CrossRoad();
    middle_last=Middle_Err;
   break;
   case 2:
    Barraicade();
   break;
   case 3:
    SingleLine(); 
   break;
   case 4:
    RightAngleTurn();
   break;
   case 5:
    Hill(); 
   break;
   default:
    Normal(); 
   break;
  }
  
  if(Distance<0.4)
  {
    RoadType=0;
    Last_Singleine_Position=0;
  }
  
  if(Stop==0&&RoadType!=0)
    TEST_HIGH;
  else 
    TEST_LOW;
  
   CCD_Draw_Buff[128]=Left-1;
   CCD_Draw_Buff[129]=Right; 
   CCD_Draw_Buff[132]=BYTE0(Max_Peak);
   CCD_Draw_Buff[133]=BYTE1(Max_Peak);
   CCD_Draw_Buff[134]=Leftlastfind;
   CCD_Draw_Buff[135]=Rightlastfind;
   CCD_Draw_Buff[136]=RoadType; 

   Leftlast=Left;              // 更新边界线
   Rightlast=Right;            //更新边界线
   left_c=Left-1;              //减去2修正差分误差
   right_c=Right;
   
     //判断一下是否为坡道
   if(RoadType!=4)Is_Hill(); 
   
   if(Rightlastfind||Leftlastfind)
   {
     Middle_Err=64-(right_c+left_c)/2.0+1;  //除以二会损失精度      
     if(ABS(Middle_Err-middle_last)>2&&(curve_to_cross==0))
     {
       if(RoadType!=4&&RoadType!=2)
       {
         if(ABS(Middle_Err-middle_last)>3)
         {
          if(Middle_Err>middle_last) Middle_Err=middle_last+1;
          else Middle_Err=middle_last-1;
         }
         else
         {
          if(Middle_Err>middle_last) Middle_Err=middle_last+2;
          else Middle_Err=middle_last-2;
         }
       }
     }
     middle_last=Middle_Err;
   } 
   
   //更新队列 
   Push_And_Pull(Previous_Width,15,(float)(Right-Left));
   Push_And_Pull(Previous_Angle,15,Car_Angle);
   Push_And_Pull(Previous_Error,15,Middle_Err);
   
   if(Rightlastfind&&Leftlastfind)
   {
     if(Both_Line_find_cnt<20)Both_Line_find_cnt++;
   }
   else Both_Line_find_cnt=0;
   
   
   if(Rightlastfind&&Leftlastfind&&(Right-Left>20))   //两边边线都能够找到，计算赛道的平均亮度
   {
     Brightness_Calculate();
   }
}
void Prejudge()   //预判函数
{
   if(RisingEdgeCnt==0&&FallingEdgeCnt==0)//说明没有跳边沿，全白
   {
      RoadType=1;
      curve_to_cross=0;
      Character_Distance=Distance;
   }
   else
   {
       Is_Single_Line();
   }
}
void Normal()
{
  uint8 tempwidth;
  if(Leftlastfind==0&&Rightlastfind==0)    //上次没有边线
  {
    Find_Bothine();
    if(Leftlastfind==0&&Rightlastfind==0)
    {
       if(FindBoundary(left))
       {
        Left=RisingEdge[RisingEdgeIndex];
        Leftlastfind=1; 
        if(Left<34&&RoadType==4)Leftlastfind=0;
       }
       else  Leftlastfind=0;
       if(FindBoundary(right))
       {
        Right=FallingEdge[FallingEdgeIndex];
        Rightlastfind=1;
        if(Right>84&&RoadType==4)Rightlastfind=0;
       }
       else  Rightlastfind=0; 
       if(Leftlastfind&&Rightlastfind)
       {
        if(Right<Left)
        {
         Leftlastfind=0;
         Rightlastfind=0;
        }
       }
    }
  }
  else                                    //上次至少找到一边
  {  
    //////////////////////////////
   if(Leftlastfind)						//上次找到了左线
   {
    if(FindNearest(left,Leftlast))		//这次能找到左线
    {
      Left=RisingEdge[RisingEdgeIndex];
      Leftlastfind=1;			
      if(Rightlastfind==0)				//上次找到左线，这次找到左线，上次没找到右线
      {
       if(FallingEdgeCnt)					//上次找到左线，这次找到左线，上次没找到右线，这次找到右线
       {
         for(FallingEdgeIndex=0;FallingEdgeIndex<FallingEdgeCnt;FallingEdgeIndex++)
         { 
            rightfind=0;   //
            if(FallingEdge[FallingEdgeIndex]>Left)         //找到潜在的边线了
           {  
             tempwidth = FallingEdge[FallingEdgeIndex]-Left;
             if(ABS(tempwidth-Reference_Width)>=2)       //右线出现的太左了，慢慢靠近
             {
               if(tempwidth>Reference_Width)   Reference_Width=Reference_Width+1;   //潜在的赛道边线比参考宽度宽太多
               else                         
               {
                 Reference_Width=Reference_Width-1;   //潜在的赛道边线比参考宽度窄太多
                 if((RoadType==0)&&(Distance-Last_Singleine_Position<0.5))
                 Reference_Width=Reference_Width-1;
               }
               Right=Left+Reference_Width;
               Rightlastfind = 0;
             }
             else 
             {
              Right=FallingEdge[FallingEdgeIndex];
              Rightlastfind=1;
             }
             rightfind=1;   //找到边线了，但是还不能立马赋值
             break;
           }
         } 
           if(Rightlastfind==0&&rightfind==0)   //虽然说有上升沿，但是该上升沿无效
          {
            if(CarSpeed>2.5)          //在车模的速度足够块的情况下才这样弄
            {
             if(Left+Reference_Width<=118)  //虚拟出来的右边线要往外靠，但是不能靠得太狠了
             {
              Reference_Width++;
             }
             else 
             {
              Reference_Width=118-Left;
             }
            }
           Right=Left+Reference_Width;
          }
       }
       else                        //没有下跳沿                                  
       {
                if(CarSpeed>2.5)   
              {
               if(Left+Reference_Width<=118)  //虚拟出来的右边线要往外靠，但是不能靠得太狠了
               {
                Reference_Width++;
               }
               else 
               {
                Reference_Width=118-Left;
               } 
              }
              Right=Left+Reference_Width;
       }
      }
    }
    else  //上次能找到左线，这次没能找到左线
    {
       Leftlastfind=0;   //丢线 
        /*           跳转到十字           */   
      if(FindBoundary(left))
     {
      if(RisingEdge[RisingEdgeIndex]<Leftlast)  //这次没能够找到与上次相邻的边界，但是有边界却比现有的边界宽
      {
        if(Rightlastfind==0&&((RoadType!=5)&&(RoadType!=4)))   //上次没有找到右边界，这次左边界又往外靠，那么就是遇到十字了
        {
         RoadType=1; 
         Character_Distance=Distance; 
        }
      }
      else if(RisingEdge[RisingEdgeIndex]<Leftlast+8)   //这次的边界比上次更靠近中央，但是他们的差值不大于8
      {
     //  Left=RisingEdge[RisingEdgeIndex];
      // Leftlastfind=1;                 //未丢线		 
      }
     } 
      else                     //不能找到左边界
     {
       if(Rightlastfind==0&&(RoadType!=4))  
       {
        RoadType=1;
        Character_Distance=Distance;
       }
     }
    }
   }
   ///////////////////////////////
   if(Rightlastfind)					//上次找到了右线
   {
    if(FindNearest(right,Rightlast))	//这次能找到右线
    {
      Right=FallingEdge[FallingEdgeIndex];
      Rightlastfind=1;
      if(Leftlastfind==0)				//上次找到了右线,这次能找到右线,上次找不到左线
      {
       if(RisingEdgeCnt>0)				//上次找到了右线,这次能找到右线,上次找不到左线,这次找到了左线
       {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
	 for(RisingEdgeIndex=RisingEdgeCnt-1;RisingEdgeIndex>=0;RisingEdgeIndex--)
         {
           leftfind=0;
           if(RisingEdge[RisingEdgeIndex]<Right)         //找到潜在的边线了
           {
             tempwidth = Right-RisingEdge[RisingEdgeIndex];
             if (ABS(tempwidth-Reference_Width)>=2)       //右线出现的太左了，慢慢靠近
             {
              if(tempwidth>Reference_Width)   Reference_Width=Reference_Width+1;   //潜在的赛道边线比参考宽度宽太多
              else                          
              {
                Reference_Width=Reference_Width-1;   //潜在的赛道边线比参考宽度窄太多
                if((RoadType==0)&&(Distance-Last_Singleine_Position<0.5))
                Reference_Width=Reference_Width-1;   //潜在的赛道边线比参考宽度窄太多
              }
              Left=Right-Reference_Width;
              Leftlastfind = 0;
             }
             else 
             {
               Left=RisingEdge[RisingEdgeIndex];
               Leftlastfind=1;
             }
            leftfind=1;
            break;
           }          
         }
            if(Leftlastfind==0&&leftfind==0)   //虽然说有上升沿，但是该上升沿无效
            {
              if(CarSpeed>2.5)              //速度限制，防止出线
                {
                 if(Right-Reference_Width>=10)  //虚拟出来的右边线要往外靠，但是不能靠得太狠了
                 {
                    Reference_Width++;
                 }
                 else 
                 {
                    Reference_Width=Right-10;
                 } 
                }
               Left=Right-Reference_Width;
            }
       }
       else                               //没有上升沿哦亲
       { 
         if(CarSpeed>2.5)              //速度限制，防止出线
          {
           if(Right-Reference_Width>=10)  //虚拟出来的右边线要往外靠，但是不能靠得太狠了
           {
               Reference_Width++;
           }
           else 
           {
              Reference_Width=Right-10;
           } 
          }
         Left=Right-Reference_Width;
       }
      }
    }
    else  //上次能找到，这次没能找到与上次相邻近的线
    {
      Rightlastfind=0;   //丢线
     /*           跳转到十字           */
     if(FindBoundary(right))   //能够找到右边界
     {
      if(FallingEdge[FallingEdgeIndex]>Rightlast)  //这次没能够找到与上次相邻的边界，但是有边界却比现有的边界宽
      {
        if(Leftlastfind==0&&((RoadType!=5)&&(RoadType!=4)))   //上次没有找到左边界，这次右边界又往外靠，那么就是遇到十字了
        {
         RoadType=1; 
         Character_Distance=Distance;
        }
      }
      else if(FallingEdge[FallingEdgeIndex]>Rightlast-8)  //这次的边界比上次更靠近中央，但是他们的差值不大于8
      {
    //    Right=FallingEdge[FallingEdgeIndex];
     //   Rightlastfind=1;           //其实是未丢线
      }
     }
     else                     //不能找到右边界
     {
        if(Leftlastfind==0&&(RoadType!=4))   //上次没有找到左边界，这次右边界又往外靠，那么就是遇到十字了
       {
         RoadType=1;
         Character_Distance=Distance;
       }
     }
    }
   }
   //都能找到边线，更新参考宽度
   if(Rightlastfind&&Leftlastfind)
   {
     
     Reference_Width=Right-Left; 
     if(Reference_Width<35)Reference_Width=35;
   }
  }
}
void CrossRoad()
{
   static uint8 Normal_Cnt;
   static uint8 Cross_Road_Cnt=0;
  
    //缓慢归零，车子回正  
  if(curve_to_cross==0)
  {  
    Rightlastfind=0;
    Leftlastfind=0; 
  }
  else
  {  
    if(Rightlastfind)
    {
        if(FindNearest(right,Rightlast))		//这次能找到左线
       {
        Right=FallingEdge[FallingEdgeIndex];
        Rightlastfind=1;
        Left=Right-Reference_Width;
       }
       else 
       {
         Rightlastfind=0;
         //curve_to_cross=0;
       }
    }
    else if(Leftlastfind)
    {
      if(FindNearest(left,Leftlast))		//这次能找到左线
     {
      Left=RisingEdge[RisingEdgeIndex];    
      Leftlastfind=1;
      Right=Reference_Width+Left;
     }
     else 
     {
       Leftlastfind=0;
      // curve_to_cross=0;
     }
    }
  }
  
    if(ABS(Middle_Err)>1)
    {
      if(curve_to_cross==0)
      {
       if(Middle_Err>0)Middle_Err=Middle_Err-1.2;     
       if(Middle_Err<0)Middle_Err=Middle_Err+1.2;
      }
    }
    else Middle_Err=0;
    
    if((curve_to_cross==0)&&Is_Black_Line()&&(Distance-Character_Distance<0.15))
    {
          RoadType=4;
          Turn_To_Angle=0;//??????????????????????????????????
          Strong_Turn_Cnt=0;
          Strong_Turn_En=0;
          Turn_State=0;
          Turn_Angle_Integral=0;
    }
    

    
    if((Stop==0)&&(Distance-Character_Distance>0.7)&&Protect&&(curve_to_cross==0))                             //跑出去了
    {
      Stop=true;
      CarStopedJustNow=true; 
    }
    
  
    
    
    
    if(curve_to_cross==0||(curve_to_cross==1&&Distance-Character_Distance>0.5)) Find_Bothine();
    
    if(Rightlastfind&&Leftlastfind)//能够找到两边边界
    {
     if(Right-Left<65&&Cross_Road_Cnt>4)     //宽度有效
     {
        /*                跳转到直角弯                 */
        if(Distance>1&&(Distance-Character_Distance<0.15))
        {
          RoadType=4;
          Turn_Angle_Integral=0;
          Turn_To_Angle=0;
          Strong_Turn_Cnt=0;
          Strong_Turn_En=0;
          Turn_State=0;                       
        }
        else
        { 
          Normal_Cnt++;
          if(Normal_Cnt>=3)
          {
           RoadType=0;
           curve_to_cross=0;
          /*            跳回正常道路            */
          }
         
        }
     }
     else 
     {
      Rightlastfind=0;
      Leftlastfind=0;
      Normal_Cnt=0;
     }
    }
    else Normal_Cnt=0;
    
     if(curve_to_cross&&(Distance-Character_Distance>0.1)&&(Distance-Character_Distance<0.32))
    {
       Is_Single_Line();//说明出现误识别了；
    }

    if(RoadType!=1)
    {
      Cross_Road_Cnt=0;
    }
    else
    {
      if(Cross_Road_Cnt<10)Cross_Road_Cnt++; 
      
      if(Cross_Road_Cnt>3&&Cross_Road_Cnt<8&&curve_to_cross==0)/////////////////////////////////改
      {
        Leftlastfind=0;
        Rightlastfind=0;
        Is_Single_Line();//说明出现误识别了；
      }
      
    }
}
void Barraicade()
{
  int i;
  
   if(Right_Or_Left==left) //障碍在左边
   {
     if(FindNearest(right,Rightlast))
     {
      Right=FallingEdge[FallingEdgeIndex];
      Rightlastfind=1;
     }
     else Rightlastfind=0;
     if(Distance-Character_Distance<0.25)
     {
       Leftlastfind=0;
       if(RisingEdgeCnt>0)  
       {
        for(i=RisingEdgeCnt-1;i>=0;i--)
        {
         if(RisingEdge[i]+10<Right) 
         {
          Left=RisingEdge[i]; 
          Leftlastfind=1;
          break;
         }
        }
       }
     }
     else
     {
       if(FindNearest(left,Leftlast))
       {
        Left=RisingEdge[RisingEdgeIndex];
       Leftlastfind=1;
       }
       else Leftlastfind=0;
     }
     
     if(Leftlastfind&&Rightlastfind)Reference_Width=Right-Left;
     else
     {
      Left=Right-Reference_Width;
     }
   }
   else   //障碍在右边
   { 
     if(FindNearest(left,Leftlast))
     {
      Left=RisingEdge[RisingEdgeIndex];
      Leftlastfind=1;
     }
     else Leftlastfind=0;
     if(Distance-Character_Distance<0.25)
     {
       Rightlastfind=0;
       if(FallingEdgeCnt>0)  
       {
         for(i=0;i<FallingEdgeCnt;i++)
          {
           if(FallingEdge[i]>Left+10) 
           {
            Right=FallingEdge[i]; 
            Rightlastfind=1;
            break;
           }
          }
       }
     }
     else
     {
        if(FindNearest(right,Rightlast))
       {
        Right=FallingEdge[FallingEdgeIndex];
        Rightlastfind=1;
       }
       else Rightlastfind=0;
     }
     
     if(Leftlastfind&&Rightlastfind)Reference_Width=Right-Left;
     else
     {
       Right=Left+Reference_Width;
     }
   }
   Turn_State_Cnt++;
   if(Distance-Character_Distance>0.5)
   RoadType=0;
}
void SingleLine()
{
  int i=0,j=0;
  static int lostcnt;
  static int SharpTurn_Lost=0;
  
  Leftlastfind=0;
  Rightlastfind=0;
  
  for(i=Left-5;i<=Left+5;i++)           
  {
    if((CCD_Diff[i]<CCD_Diff[i-1])&&(CCD_Diff[i]<=CCD_Diff[i+1])&&(CCD_Diff[i]<-Threshold))  //寻找负的峰值
    {   
      Left=i;
      Leftlastfind=1;
      lostcnt=0;
      break;
    }
  }
  
  for(j=Right-5;j<=Right+5;j++)
  {
    if((CCD_Diff[j]>=CCD_Diff[j-1])&&(CCD_Diff[j]>CCD_Diff[j+1])&&(CCD_Diff[j]>Threshold)) //寻找正的峰值
    {
       Right=j;
       Rightlastfind=1;
       lostcnt=0;
       break;
    }
  }
  
  if((Leftlastfind==0)&&(Rightlastfind==0)) ///////////////注意！！！！！
  {     
      lostcnt++;
    
      if(lostcnt>=3) 
      {
            
        if(((Distance-Character_Distance)>0.1)&&((Distance-Character_Distance)<0.4)&&(ABS(Middle_Err)>10))
        {
         SharpTurn_Lost=1;
        }
        
        if(Distance-Character_Distance<0.2)           //距离判定说明是误判，应该转到直角里面                                   /////////不起作用吧？？？？？？？？？？？？？？
        {
          RoadType=4;
          Turn_Angle_Integral=0;
          Turn_To_Angle=0;
          Strong_Turn_Cnt=0;
          Strong_Turn_En=0;
          Turn_State=0;             
        }
        else                                          //确实是单线而且现在是出单线的情况了
        {
           
           if(SharpTurn_Lost==0) 
          {                      
              for(i=Left-10;(i>Left-40)&&(i>0);i--)           
            {           
              if((CCD_Diff[i]>=CCD_Diff[i-1])&&(CCD_Diff[i]>CCD_Diff[i+1])&&(CCD_Diff[i]>Threshold*2))  //寻找正的峰值
              {   
                Left=i;
                Leftlastfind=1;
                break;
              }
            }
            
            
             for(j=Right+10;(j<Right+40)&&(j<125);j++)
            {
              if((CCD_Diff[j]<CCD_Diff[j-1])&&(CCD_Diff[j]<=CCD_Diff[j+1])&&(CCD_Diff[j]<-Threshold*2)) //寻找负的峰值
              {
                 Right=j;
                 Rightlastfind=1;
                 break;
              }
            }
          
            if(Rightlastfind||Leftlastfind) 
            {
             if(Rightlastfind==0) Right=(64-(int)Middle_Err)*2-Left;
             if(Leftlastfind==0) Left=(64-(int)Middle_Err)*2-Right;
             RoadType=0;
             Last_Singleine_Position=Distance;
             lostcnt=0; 
            }
          }
          else
          {  
            if(Middle_Err<0)
            {
              for(i=Left;i<120;i++)         
             {
              if((CCD_Diff[i]<CCD_Diff[i-1])&&(CCD_Diff[i]<=CCD_Diff[i+1])&&(CCD_Diff[i]<-Threshold))  //寻找负的峰值
              {
                for(j=i+2;(j<=i+10)&&(j<125);j++) 
                {
                  if((CCD_Diff[j]>=CCD_Diff[j-1])&&(CCD_Diff[j]>CCD_Diff[j+1])&&(CCD_Diff[j]>Threshold)) //寻找正的峰值
                  {
                    Left=i;
                    Leftlastfind=1;
                    Right=j;
                    Rightlastfind=1;
                    SharpTurn_Lost=0;
                    lostcnt=0;
                  }
                }
              }
             }
            }          
            if(Middle_Err>0)
            {
              for(i=Right;i>1;i--)         
             {
              if((CCD_Diff[i]<CCD_Diff[i-1])&&(CCD_Diff[i]<=CCD_Diff[i+1])&&(CCD_Diff[i]<-Threshold))  //寻找负的峰值
              {
                for(j=i+2;j<=i+10;j++) 
                {
                  if((CCD_Diff[j]>=CCD_Diff[j-1])&&(CCD_Diff[j]>CCD_Diff[j+1])&&(CCD_Diff[j]>Threshold)) //寻找正的峰值
                  {
                    Left=i;
                    Leftlastfind=1;
                    Right=j;
                    Rightlastfind=1;
                    SharpTurn_Lost=0;
                    lostcnt=0;
                  }
                }
              }
             }
            }
          }
        }
      }
    }
  
   if(Leftlastfind&&Rightlastfind) ///////////////注意！！！！！
  {
   if(ABS(Right-Left)>12)
   {
     RoadType=0;
     Last_Singleine_Position=Distance;
     Leftlastfind=0;
     Rightlastfind=0;
     Find_Bothine();
   }
  }
   
   if(RoadType!=3) SharpTurn_Lost=0;
  
}

///////////////////////////////////////////////////////直角弯道////////////////
void RightAngleTurn()
{ 
  static uint8 tuofengDisapear=0;
  static uint8 Lastine,Lastine_R,Refind_Cnt_R;
  uint8 refind=0;
  static int Cross_cnt=0;
  uint8 j=0;
  state_change=0;
  if(Distance-Character_Distance<0.1) tuofengDisapear=0;
  /***********************状态0,两边都能找到线********************/
  if(Turn_State==0)
  {   
     if(Turn_To_Angle!=0)
     {
       if(Turn_To_Angle==1)  //右转入直角
       { 
          if(tuofengDisapear==0) Cross_cnt++;
          if(FindNearest(left,Leftlast))
         {
            Left=RisingEdge[RisingEdgeIndex];
            Leftlastfind=1;         
            if(Rightlastfind==0&&Cross_cnt<=10&&(tuofengDisapear==0))
            {
              if(FallingEdgeCnt==0)
              {
                RoadType=0;
                Cross_cnt=0; 
              }     
              else  if(FallingEdge[FallingEdgeCnt-1]<Left)  
              {   
                RoadType=0;
                Cross_cnt=0; 
              }
            }
         }
         else
         { 
           Cross_cnt=0;
           Leftlastfind=0;
           if(Distance-Character_Distance>0.1)  tuofengDisapear=1; //边沿有跳变，驼峰消失        
           if(Distance-Character_Distance<0.60)
           {
             for(RisingEdgeIndex=RisingEdgeCnt-1;RisingEdgeIndex>=0;RisingEdgeIndex--)
             {
              if(((RisingEdge[RisingEdgeIndex]>=Left)&&(RisingEdge[RisingEdgeIndex]-Left<20))||ABS(RisingEdge[RisingEdgeIndex]-Left)<10)  
              {
                  Leftlastfind=1;
                  Left=RisingEdge[RisingEdgeIndex];
                  break;
              }
             }
             Character_Distance=Distance-0.1;
           }           
         }
         //处理右边线
                                                                                    ///////////////////////
         if(Rightlastfind==0)
         {
           if(RisingEdgeCnt>=1)
           {
               for(FallingEdgeIndex=FallingEdgeCnt-1;FallingEdgeIndex>=0;FallingEdgeIndex--)
              {
                 if(FallingEdge[FallingEdgeIndex]-Left>35)
                 {
                   if(Rightlastfind==1)         //说明在驼峰之外还有边界
                   {
                     refind=1;
                     break;
                   }
                   else
                   {
                    Right=FallingEdge[FallingEdgeIndex];
                    Rightlastfind=1; 
                   }
                 }
              }
           }
           if(refind==0&&tuofengDisapear==0)
           {
            Right=Left+Reference_Width; 
            Refind_Cnt=0; 
            Rightlastfind=0;
           }
           else
           {
            if(Rightlastfind==1)
            {
             Refind_Cnt++;
             if(Refind_Cnt>=3) 
             {
               Character_Distance=Distance-0.1;
               Rightlastfind=1;
             }
             else  Rightlastfind=0;
            }
           }
         }
         else
         {
            if(FindNearest(right,Rightlast))
           {
               Right=FallingEdge[FallingEdgeIndex];
               Rightlastfind=1;
           }
           else Rightlastfind=0;
         }                                                                           ///////////////////////
         
       }
       else                  //左转入直角
       {
         if(tuofengDisapear==0) Cross_cnt++;
         //处理右边线
         if(FindNearest(right,Rightlast))
         {
          Right=FallingEdge[FallingEdgeIndex];      
          Rightlastfind=1;
          
          
          if(Leftlastfind==0&&Cross_cnt<=10&&(tuofengDisapear==0))
          {
            if(RisingEdgeCnt==0)
            {
              RoadType=0;
              Cross_cnt=0; 
            }     
            else  if(RisingEdge[0]>Right)  
            {   
              RoadType=0;
              Cross_cnt=0; 
            }
          }
  
         }
         else 
         {  
            Cross_cnt=0;
           Rightlastfind=0; 
           if(Distance-Character_Distance>0.1)  tuofengDisapear=1; //边沿有跳变，驼峰消失
           if(Distance-Character_Distance<0.60)
           {
             for(FallingEdgeIndex=0;FallingEdgeIndex<FallingEdgeCnt;FallingEdgeIndex++)
             {
              if(((FallingEdge[FallingEdgeIndex]<Right)&&(Right-FallingEdge[FallingEdgeIndex]<20))||(ABS(Right-FallingEdge[FallingEdgeIndex])<10))
              {
                  Rightlastfind=1;
                  Right=FallingEdge[FallingEdgeIndex];
                  break;
              }
             }
             Character_Distance=Distance-0.1;
           }  
         } 
         //处理左边线
         if(Leftlastfind==0)
         {
           if(RisingEdgeCnt>=1)
           {
               for(RisingEdgeIndex=0;RisingEdgeIndex<=RisingEdgeCnt-1;RisingEdgeIndex++)
              {
                 if(Right-RisingEdge[RisingEdgeIndex]>35)
                 {
                   if(Leftlastfind==1)         //说明在驼峰之外还有边界
                   {
                     refind=1;
                     break;
                   }
                   else
                   {
                    Left=RisingEdge[RisingEdgeIndex];
                    Leftlastfind=1; 
                   }
                 }
              }
           }
           if(refind==0&&tuofengDisapear==0)
           {
            Left=Right-Reference_Width; 
            Refind_Cnt=0; 
            Leftlastfind=0;
           }
           else
           {
            if(Leftlastfind==1)
            {
             Refind_Cnt++;
             if(Refind_Cnt>=3) 
             {
               Character_Distance=Distance-0.1;
                Leftlastfind=1;
             }
             else  Leftlastfind=0;
            }
           }
         }
         ////////左边线已经从新找到
         else
         {
            if(FindNearest(left,Leftlast))
           {
               Left=RisingEdge[RisingEdgeIndex];
               Leftlastfind=1;
           }
           else Leftlastfind=0;
         }
       }
     }
  //////////////////////////////////////////////////
     else                   //正入直角
     {
       if(Distance-Character_Distance>0.1)
       {  
         Leftlastfind=0;
         Rightlastfind=0;
         if(RisingEdgeCnt!=0) 
         {
            for(RisingEdgeIndex=RisingEdgeCnt-1;RisingEdgeIndex>=0;RisingEdgeIndex--)
            {
               if(RisingEdge[RisingEdgeIndex]<64)
               {
                  Leftlastfind=1;
                  Left=RisingEdge[RisingEdgeIndex];
                  break;
               }
            }
         }       
        if(FallingEdgeCnt!=0)  
        {
           for(FallingEdgeIndex=0;FallingEdgeIndex<FallingEdgeCnt;FallingEdgeIndex++)
           {
              if(FallingEdge[FallingEdgeIndex]>64)
              {
                  Rightlastfind=1;
                  Right=FallingEdge[FallingEdgeIndex];
                 break;
              }
           }
         }
         Reference_Width=50;
         if(Rightlastfind==1&&Leftlastfind==0)
         {
           Left=Right-Reference_Width;
         }
         if(Rightlastfind==0&&Leftlastfind==1)
         {
           Right=Left+Reference_Width;
         }
       }
     }
    //////////////////////////      
    if(Distance-Character_Distance>1.60)
    {
      RoadType=0;
    }
    
    if((Distance-Character_Distance>0.80)&&(Leftlastfind==0||Rightlastfind==0))
    {
      Turn_State=1;
      Refind_Cnt=0;
      state_change=1;
      Strong_Turn_En=1;
      Turn_Angle_Integral=0;
      Turn_State1To2_Left=Left;
      Turn_State1To2_Right=Right;
      if(Leftlastfind==0)
      {
        Right_Or_Left=1; //左直角 
      }
      else
      Right_Or_Left=-1;  //右直角
    }
  }
  
  
  /********************状态1,进入了直角，强拐*********************/
  if((Turn_State==1)&&(state_change==0))          //状态一 丢失了一条边线
  {
    Cross_cnt=0;
    if(Right_Or_Left==1)
    {   
        Rightlastfind=0;
        Leftlastfind=0;
        
        
       for(j=Turn_State1To2_Left-10;j>5;j--)
      {
         if((CCD_Diff[j]>=CCD_Diff[j-1])&&(CCD_Diff[j]>CCD_Diff[j+1])&&(CCD_Diff[j]>Threshold)) //寻找正的峰值
         {          
          Left=j;
          Leftlastfind=1; 
          break;   
         }
       }
        
       if(Leftlastfind)             //左直角，重新找到左边线了
      {
                     
          for(j=Left;j<100;j++)
          {
             if((CCD_Diff[j]<CCD_Diff[j-1])&&(CCD_Diff[j]<=CCD_Diff[j+1])&&(CCD_Diff[j]<-Threshold))  //寻找负的峰值
            {
             Rightlastfind=1;
             Right=j;
             break;
            }
          }
         
         if(Lastine<=Left)
         {
           Refind_Cnt++;
           if(Refind_Cnt>=3)
           {
            Turn_State=2;
            state_change=1;
           }
         }
         else   Refind_Cnt=0;
         Lastine=Left;  
      }
      else    Refind_Cnt=0;
      
       if(Rightlastfind==0)              //
      {        
        for(j=Turn_State1To2_Left;j<100;j++)
        {
           if((CCD_Diff[j]<CCD_Diff[j-1])&&(CCD_Diff[j]<=CCD_Diff[j+1])&&(CCD_Diff[j]<-Threshold))  //寻找负的峰值
          {
           Rightlastfind=1;
           Right=j;
           break;
          }
        }
        if(Rightlastfind)
        {
         if(Lastine_R>Right)
         {
           Refind_Cnt_R++;
           if(Refind_Cnt_R>=3)
           {
            Turn_State=2;
            state_change=1;
           }
           else Rightlastfind=0;
         }
         else
         {
           Refind_Cnt_R=0;
           Rightlastfind=0;
         }
         Lastine_R=Right;  
        }
        else   Refind_Cnt_R=0;
      }
    }
    else  //右直角                              有点问题哦
    {  
      Leftlastfind=0;
       Rightlastfind=0;
       
       for(j=Turn_State1To2_Right;j<125;j++)
      {
         if((CCD_Diff[j]<CCD_Diff[j-1])&&(CCD_Diff[j]<=CCD_Diff[j+1])&&(CCD_Diff[j]<-Threshold))  //寻找负的峰值
        {
         Rightlastfind=1;
         Right=j;
         break;
        }
      }
       
       if(Rightlastfind)             //右直角，重新找到右边线了
      { 
          for(j=Right;j>20;j--)
          {
             if((CCD_Diff[j]>=CCD_Diff[j-1])&&(CCD_Diff[j]>CCD_Diff[j+1])&&(CCD_Diff[j]>Threshold)) //寻找正的峰值
            {
             Leftlastfind=1;
             Left=j;
             break;
            }
          }
        
         if(Lastine_R>=Right)
         {
           Refind_Cnt_R++;
           if(Refind_Cnt_R>=3)
           {
            Turn_State=2;
            state_change=1;
           }
         }
         else Refind_Cnt_R=0;
         Lastine_R=Right;  
       }
      else  Refind_Cnt_R=0;
     
      if(Leftlastfind==0)
     {
       for(j=Turn_State1To2_Right;j>5;j--)
      {
         if((CCD_Diff[j]>=CCD_Diff[j-1])&&(CCD_Diff[j]>CCD_Diff[j+1])&&(CCD_Diff[j]>Threshold)) //寻找正的峰值
         {          
          Left=j;
          Leftlastfind=1; 
          break;   
         }
       }
       
       if(Leftlastfind)
      {
       if(Lastine<Left)
       {
         Refind_Cnt++;
         if(Refind_Cnt>=3)
         {
          Turn_State=2;
          state_change=1;
         }
         else Leftlastfind=0;
       }
       else
       {
         Refind_Cnt=0;
         Leftlastfind=0;
       }
       Lastine=Left;  
      }
       else Refind_Cnt=0;
     }
    }
    
    if(state_change==1)
    {
       if(Strong_Turn_Cnt<20)     //防止过早地跳到Turn_State2中
       {
         Turn_State=1;
       }
    }
        
    Middle_Err=0;
    
  }
  
  static uint8 Width_Decrease_cnt;
  static int Widthast;
  int width;
  
  if((Turn_State==2)&&(state_change==0))
  {
    Normal();
    RoadType=4;
    
    if(Distance-Character_Distance>2.0)
    {
        if(Leftlastfind&&Rightlastfind)        //斜着出直角
      {
        width=Right-Left;
        
        if(Widthast!=0)
        {
          if(Widthast-width>=2)
          {
           Width_Decrease_cnt++; 
          }
          else
          {
           Width_Decrease_cnt=0;  
          }
          if(Width_Decrease_cnt==2||(Widthast-width>=3))
          {
            Turn_State=3;
            state_change=1;
            Character_Distance=Distance;
            Leftlastfind=0;
            Rightlastfind=0;
          } 
        }
        Widthast=width;
      }
    
     if(Leftlastfind==0||Rightlastfind==0) //直出直角
     {  
       Turn_State=3;
       state_change=1;
       Character_Distance=Distance;
       Leftlastfind=0;
       Rightlastfind=0;
     }
    }
    else
    {
      Widthast=0;
      Width_Decrease_cnt=0;
    }
  }
  if((Turn_State==3)&&(state_change==0))  //
  {
      if(Middle_Err>2) Middle_Err=Middle_Err-0.5;
      if(Middle_Err<-2) Middle_Err=Middle_Err+0.5;
      if(Distance-Character_Distance>0.2)
      {
        Find_Bothine();
        if(Leftlastfind&&Rightlastfind)
        {
         if((Right-Left>25)&&(Right-Left<60)&&(ABS((Right+Left)/2-64))<15)
         {
           RoadType=0;
           Turn_State=0;
           Refind_Cnt=0;
         }
        }
      } 
      if(Distance-Character_Distance>0.3)
      {
         RoadType=0;
        Turn_State=0;
        Refind_Cnt=0;
      }
      if(RoadType==0) Last_Angle_Turn_Position=Distance;
      
  }
    if(Distance-Last_Angle_Turn_Position<0.5)RoadType=0;
    
    CCD_Draw_Buff[137]=Turn_State;
}
void Hill()
{
  //////////用来在坡道上搜索线的变量/////////
  uint8 i=0,j=0,threshold_t;
  static uint8 lost_cnt;
  ////////////////////////////
  static uint8  Confirm_Cnt=0;  
   if(RoadTypeConfirm==0)  //还没确认
  {
    Normal(); //调用normal函数来搜线 
    if(Confirm_Cnt<5)      //用5个周期来确认
    {
      if(Leftlastfind==0||Rightlastfind==0||ABS(Middle_Err)>5)   //有一条边没找到说明这个其实是弯道
      {
       RoadType=0; 
       Confirm_Cnt=0;
      } 
     else Confirm_Cnt++;
    }
    else RoadTypeConfirm=1;     //赛道类型确认了，确实是坡道 
    lost_cnt=0;
  }
  else                                  //已经判断是坡道了
  { 
    Leftlastfind=0;
    Rightlastfind=0;
    threshold_t=Threshold;
    while((Leftlastfind==0)&&(threshold_t*1.5>Threshold))
    {
       for(i=Leftlast-6;i<=Leftlast+6;i++) ////////////////////////////////////////////////////////////////////////////////////////////???????????????????????????         
      {
        if((CCD_Diff[i]>=CCD_Diff[i-1])&&(CCD_Diff[i]>CCD_Diff[i+1])&&(CCD_Diff[i]>threshold_t))  //寻找负的峰值
        {  
          Left=i;
          Leftlastfind=1;
        }
      }
      threshold_t=threshold_t-2;
    } 
    threshold_t=Threshold;
    while((Rightlastfind==0)&&(threshold_t*1.5>Threshold))
    {
      for(j=Rightlast+6;j>=Rightlast-6;j--)
      {
        if((CCD_Diff[j]<CCD_Diff[j-1])&&(CCD_Diff[j]<=CCD_Diff[j+1])&&(CCD_Diff[j]<-threshold_t)) //寻找正的峰值
        {
           Right=j;
           Rightlastfind=1;
        }
      }
      threshold_t=threshold_t-2;
    }
    
    if((Leftlastfind==1)&&(Rightlastfind==0))/////////////////////////////////////??????????????????????????????????????????????????????????????????????????????
    {
       for(j=Left+10;(j<=Left+50)&&(j<120);j++)
      {
        if((CCD_Diff[j]<CCD_Diff[j-1])&&(CCD_Diff[j]<=CCD_Diff[j+1])&&(CCD_Diff[j]<-Threshold)) //寻找正的峰值
        {
           Right=j;
           Rightlastfind=1;
           break;
        }
      }
    }
      
    if((Leftlastfind==0)||(Rightlastfind==1)) 
    {
       for(i=Right-10;(i>=Right-50)&&(i>10);i--) ////////////////////////////////////////////////////////////////////////////////////////////???????????????????????????         
      {
        if((CCD_Diff[i]>=CCD_Diff[i-1])&&(CCD_Diff[i]>CCD_Diff[i+1])&&(CCD_Diff[i]>threshold_t))  //寻找负的峰值
        {  
          Left=i;
          Leftlastfind=1;
          break;
        }
      }
    }
    
    if(Leftlastfind==0||Rightlastfind==0)  ////////////////////////////////////////////////////////////////////////////////////////////???????????????????????????????????
    {
       if((Distance-Character_Distance)>3) 
     {
       if(Is_Black_Line())
       {
          RoadType=4;
          Turn_To_Angle=0;//??????????????????????????????????
          Strong_Turn_Cnt=0;
          Strong_Turn_En=0;
          Turn_State=0;
          Turn_Angle_Integral=0;
       }
     }
      
      lost_cnt++;
      if(Right-Left>50||(lost_cnt>=3&&Right-Left>30)||(lost_cnt>=5&&Right-Left<20))
      {
        RoadType=0;
        Confirm_Cnt=0;
        RoadTypeConfirm=0;
      }
    }
    else lost_cnt=0;
  }
   if((Distance-Character_Distance)>3.3)   //假设坡道长度不超过3m
     /*            跳回正常道路            */
   {
     Last_Hill_Distance=Distance;
     RoadType=0;
     Hill_Flag=1;
     Confirm_Cnt=0;
     RoadTypeConfirm=0;
   }
   

   
}
uint8 FindNearest(uint8 mode,uint8 lastegde)
{
  uint8 find=0;
  if(mode==left)
  {
     if(RisingEdgeCnt!=0)  
     {
      for(RisingEdgeIndex=0;RisingEdgeIndex<RisingEdgeCnt;RisingEdgeIndex++)
      {
       if(ABS(lastegde-RisingEdge[RisingEdgeIndex])<=3)//3
       {
         find=1;
         break;
       }
      }
     }
  }
  else
  {
    if(FallingEdgeCnt!=0)  
   {
     for(FallingEdgeIndex=0;FallingEdgeIndex<FallingEdgeCnt;FallingEdgeIndex++)
     {
       if(ABS(lastegde-FallingEdge[FallingEdgeIndex])<=3)//3
       {
         find=1;
         break;
       }
     }
   }
  }
  return find;
}
uint8 FindBoundary(uint8 mode)    //寻找最靠近中心的边界
{
  uint8 find=0;
  if(mode==left)
  { 
   if(RisingEdgeCnt!=0)  
   { 
    for(RisingEdgeIndex=RisingEdgeCnt-1;RisingEdgeIndex>=0;RisingEdgeIndex--)
    {
     if(RisingEdge[RisingEdgeIndex]<75)
     {
       find=1;
       break;
     }
     if(RisingEdgeIndex==0)  break;
    }
   }
  }
   else
  {
    if(FallingEdgeCnt!=0)  
    {
     for(FallingEdgeIndex=0;FallingEdgeIndex<FallingEdgeCnt;FallingEdgeIndex++)
     {
      if(FallingEdge[FallingEdgeIndex]>53)
      {
       find=1;
       break;
      }
     }
    }
  }
  return find;
}

//坡道检测
void Is_Hill()
{
   static int Width_Increase_Cnt=0,Straight_Cnt; 
   float slope;
   float Angle_Change=0;
   int i;
   
   Angle_Change = Previous_Angle[0]+Previous_Angle[1]+Previous_Angle[2]-Previous_Angle[3]-Previous_Angle[4]-Previous_Angle[5];
   Angle_Change=Angle_Change/3;
   
   if((Stop==0)&&(Distance>1)&&(ABS(Middle_Err)<=3)&&(RoadType!=5)&&Rightlastfind&&Leftlastfind&&(Angle_Change>-1))    //偏差小于一定值 在直道，用于判定坡道。跑动距离大于0.5，再判断值，防止起跑误判
   {
       Straight_Cnt++;
       if(Straight_Cnt>14)                //计算连续15次的值                             //前15个状态一直是直道
       {
          Width_Increase_Cnt=0;
        
          slope=10* Slope_Calculate(0,8,Previous_Error); 
                    
          for(i=0;i<10;i++)                        
         {
           Width_Increase_Cnt+=Previous_Width[i]-Previous_Width[i+1];
         }
          
         if((Width_Increase_Cnt>=5)&&(ABS(slope)<2))      //只要宽度增加的次数大于5且变化的角度大于-3度则认为找打了坡道  排除由于车模角度减小带来的干扰
         {
           RoadType=5;
           RoadTypeConfirm=0;
           Character_Distance=Distance;
         } 
       }
   }
   else
   {
     Angle_Change=0;
     Straight_Cnt=0;
   }     
}

void Is_Barraicade()
{
  int left_edge,right_edge,left_edge_find,right_edge_find;
  int i;
  float half_width=0;
  half_width=Previous_Width[0]/2;
  if(Leftlastfind&&Rightlastfind)
  {
    right_edge_find=0;
    left_edge_find=0;
    if(FallingEdgeCnt>0)
    {
      for(i=0;i<FallingEdgeCnt;i++)
      {
       if((FallingEdge[i]>Leftlast)&&(FallingEdge[i]+7<Rightlast)) 
       {
        left_edge=FallingEdge[i]; 
        left_edge_find=1;
        break;
       }
      }
    }  
    if(RisingEdgeCnt>0)  
   {
    for(i=RisingEdgeCnt-1;i>=0;i--)
    {
       if((RisingEdge[i]<Rightlast)&&(RisingEdge[i]-7>Leftlast)) 
       {
        right_edge=RisingEdge[i]; 
        right_edge_find=1;
        break;
       }
    }
   }
   
   if(right_edge_find&&left_edge_find)
   {
     if(((right_edge-left_edge)*1.0/Previous_Width[0])>0.2)  //大于宽度的1/5 本来是1/4
     {
       RoadType=2;
       Turn_State_Cnt=0;        
       Turn_Angle_Integral=0;
       Turn_State=0;
       Character_Distance=Distance;  
       if(Rightlast-right_edge>half_width) Right_Or_Left=left; //左边出现了障碍
       if(left_edge-Leftlast>half_width)   Right_Or_Left=right;  //右边出现了障碍 
     }
   }
   
   else if(left_edge_find)
   {
     
   }
   else if(right_edge_find)
   {
     
   }
 }
}

void Is_Straight_Angle_Turn()  
{
  //斜着入直角，能搜到两条边线的变量
  
  static int Width_Change_Cnt_L=0,Width_Old_R;
  static int Pre_Width_All_L=0;
  int Width_All_L,Width_Low_L;
  
  static int Width_Change_Cnt_R=0,Width_Old_L;
  static int Pre_Width_All_R=0;
  int Width_All_R,Width_Low_R;

  
  uint8 find=0; 
  Turn_To_Angle=0;
  //斜着入直角，不能搜到两条边线的变量
  int i=0;
  static uint8  leftfind_t=0,rightfind_t=0;
  static uint8  leftlostcnt=0,rightlostcnt=0;

  
  /*********************两边都找到线了*******************/
 if(Leftlastfind&&Rightlastfind&&(Right-Left<60)&&(RoadTypeSwitch&8))  //两边都找到线了并且有斜着入直角
  {
    if((RisingEdgeIndex<RisingEdgeCnt-1)&&(RisingEdge[RisingEdgeIndex+1]<120))  //边线右边有多的上升沿
    {
      Width_All_R=RisingEdge[RisingEdgeIndex+1]-Left;
      Width_Low_R=RisingEdge[RisingEdgeIndex+1]-Right;
      if((Pre_Width_All_R>Width_All_R)&&(Width_Low_R<30)&&(Width_Low_R>=6))//Left-Line_Out>8
      {
        if(Width_Change_Cnt_R==0) Width_Old_R=Right-Left;
        Width_Change_Cnt_R++;
        if(Width_Change_Cnt_R>=5)
        {
          if(Width_Old_R-(Right-Left)>5)
          {
            if(FallingEdgeIndex<FallingEdgeCnt-1)
            {
               Right=FallingEdge[FallingEdgeIndex+1];
               Rightlastfind=1;
            }
            
            else 
            {
              Right=126;
              Reference_Width= Right-Left;
              Rightlastfind=0;
            }  
            find=1;
            Turn_To_Angle=1;
            Width_Change_Cnt_R=0;
          }
        }
      }
      else 
      {
        Width_Change_Cnt_R=0;
      }
      Pre_Width_All_R=Width_All_R;
    }
    
    if((FallingEdgeIndex>=1)&&(FallingEdge[FallingEdgeIndex-1]>10))   //边线左边有多的下降沿
    {
      Width_All_L=Right-FallingEdge[FallingEdgeIndex-1];
      Width_Low_L=Left-FallingEdge[FallingEdgeIndex-1];
      if((Pre_Width_All_L>Width_All_L)&&(Width_Low_L<30)&&(Width_Low_L>=6))        //Left-Line_Out>8
      {
        if(Width_Change_Cnt_L==0) Width_Old_L=Right-Left;
        Width_Change_Cnt_L++;
        if(Width_Change_Cnt_L>=5)
        {
          if(Width_Old_L-(Right-Left)>5)
          {
            if(RisingEdgeIndex>=1)
            {
               Left=RisingEdge[RisingEdgeIndex-1]; 
               Leftlastfind=1;
            }
            else 
            {
              Left=10;
              Reference_Width= Right-Left;
              Leftlastfind=0;
            }     
            find=1;
            Turn_To_Angle=-1;
            Width_Change_Cnt_L=0;
          }
        }
      }
      else
      {
        Width_Change_Cnt_L=0;
      }
      Pre_Width_All_L= Width_All_L;
    }
  }
  /**********************至少有一边能找到***********************/
  else if((Leftlastfind||Rightlastfind)&&(Right-Left<55)&&(Right<110)&&(Left>18))  //有误判啊啊
  {
    if(Leftlastfind==0)
    {
      if(leftfind_t==0&&leftlostcnt<6)
      {
          for(i=Left;i<Right;i++)
         {
           if(CCD_Draw_Buff[i]>60)//亮度的一半
           break;
         }
         if(i>Left+10&&i+10<Right)
         {
          if(ABS(i-Left)>10)  //与上次的差值
          find=1;  
         }
      }
    }
     if(Rightlastfind==0)
    {
      if(rightfind_t==0&&rightlostcnt<6)
      {
          for(i=Right;i>Left;i--)
         {
           if(CCD_Draw_Buff[i]>60)//亮度的一半
           break;
         }
         if(i>Left+10&&i+10<Right)
         {
          if(ABS(i-Right)>10)  //与上次的差值
          find=1;  
         }
      }
    }
    
  }
 
 
 if(Leftlastfind==0&&Rightlastfind==0)
 {
   if(Is_Black_Line())
   {
    RoadType=4;
    Strong_Turn_Cnt=0;
    Turn_To_Angle=0;
    Turn_Angle_Integral=0;
    Strong_Turn_En=0;
    Turn_State=0;  
    Character_Distance=Distance;
   }
 }
 
 //////////////////////
  if(find==1)   //找到是直角了
  {
    RoadType=4;
    Strong_Turn_Cnt=0;
    Strong_Turn_En=0;
    Turn_Angle_Integral=0;
    Turn_State=0;  
    Character_Distance=Distance;
  }
  leftfind_t=Leftlastfind;
  rightfind_t=Rightlastfind;
  if(leftfind_t==0)
  {
    if(leftlostcnt<10)
      leftlostcnt++;
  }
  else leftlostcnt=0;
  
  if(rightfind_t==0)
  {
     if(rightlostcnt<10) 
     rightlostcnt++;
  }
  else rightlostcnt=0;
  
  
   if(ABS(Middle_Err)<5&&(Last_Angle_Turn_Position>2)&&(Distance-Last_Angle_Turn_Position<1.2)&&(Distance-Last_Angle_Turn_Position>0.7)&&(RoadTypeSwitch&1))//
  {
   if(((Leftlastfind&&Rightlastfind)==0)&&(Leftlastfind||Rightlastfind))
   {
     Character_Distance=Distance-1;
    
     state_change=0;
     
      RoadType=4;
      Turn_State=1;  
      Refind_Cnt=0;
      Strong_Turn_En=1;
      Strong_Turn_Cnt=0;
      Turn_Angle_Integral=0;
      
      Turn_State1To2_Left=Left;
      Turn_State1To2_Right=Right;
     
      if(Leftlastfind==0)
      {
        Right_Or_Left=1; //左直角 
      }
      else  Right_Or_Left=-1; //右直角 
   }  
  }  
}

void Is_CrossRoad(void)  //斜入十字   //深色背景适用！
{
   static int Width_Change_Cnt_L;
   static int Pre_Width_Out_L,Pre_Width_All_L;
   int Width_Out_L,Line_Out_L,Width_All_L;
  
   static int Width_Change_Cnt_R;
   static int Pre_Width_Out_R,Pre_Width_All_R;        
   int Width_Out_R,Line_Out_R,Width_All_R;
   
   static uint8 All_Find=0;
   
   if(Leftlastfind&&Rightlastfind)  //两边都找到线了
  {
    if((RisingEdgeIndex<RisingEdgeCnt-1)&&(RoadTypeSwitch&4))                //右边有多的上升沿
    { 
      Line_Out_R=RisingEdge[RisingEdgeIndex+1];
      Width_Out_R=Line_Out_R-Right;
      Width_All_R=Line_Out_R-Left;
      if(Width_Out_R<6)  //白色背景
      {
        if(RisingEdgeIndex<RisingEdgeCnt-2)  
        {
          Line_Out_R=RisingEdge[RisingEdgeIndex+2];
          Width_Out_R=Line_Out_R-Right;
          Width_All_R=Line_Out_R-Left;
        }
      }
      if(Width_Out_R>8)
      {  
        if((Pre_Width_Out_R>Width_Out_R)&&(Width_All_R<=Pre_Width_All_R))  //
        {
           Width_Change_Cnt_R++;
        }
        else  Width_Change_Cnt_R=0;
      }
      Pre_Width_Out_R= Width_Out_R;
      Pre_Width_All_R= Width_All_R;
    }
    else 
    {
      Width_Change_Cnt_R=0;
    }
    
    if((FallingEdgeIndex>=1)&&(RoadTypeSwitch&4))    //左边有多的下降沿 
    {
      Line_Out_L=FallingEdge[FallingEdgeIndex-1];
      Width_Out_L=Left-Line_Out_L;
      Width_All_L=Right-Line_Out_L;
      if(Width_Out_L<6)  //白色背景
      {
        if(FallingEdgeIndex>=2)  //白色背景
        {
         Line_Out_L=FallingEdge[FallingEdgeIndex-2];
         Width_Out_L=Left-Line_Out_L;
         Width_All_L=Right-Line_Out_L;
        }
      }
      if(Width_Out_L>8)
      {  
        if((Pre_Width_Out_L>Width_Out_L)&&(Pre_Width_All_L>=Width_All_L))  //
        {
           Width_Change_Cnt_L++;
        }
        else  Width_Change_Cnt_L=0;
      }
      Pre_Width_Out_L= Width_Out_L;
      Pre_Width_All_L= Width_All_L;
    }
    else
    {
      Width_Change_Cnt_L=0;
    }
    All_Find=1;
   }
   else                       
   {    
     if(Width_Change_Cnt_R>=5||Width_Change_Cnt_L>=5)
     {
      if(RoadTypeSwitch&4) curve_to_cross=1;
       RoadType=1; 
       Character_Distance=Distance;
     }
     if(All_Find==1)          
     { 
       Width_Change_Cnt_L=0;
       Width_Change_Cnt_R=0;
     }  
     else 
     {
      if(Leftlastfind&&(Left<60))
      {
         if((FallingEdgeCnt==0)||FallingEdge[FallingEdgeCnt-1]<Left)  //右边没有多的线了
        {
          if(Left<Leftlast) Width_Change_Cnt_L++;         
          else  Width_Change_Cnt_L=0;
        }
        else Width_Change_Cnt_L=0;
      }
      if(Rightlastfind&&(Right>70))
      {
        if((RisingEdgeCnt==0)||RisingEdge[0]>Right)  //左边没有多的线了
        {
          if(Right>Rightlast)    Width_Change_Cnt_R++;
          else  Width_Change_Cnt_R=0;
        }
        else Width_Change_Cnt_R=0;
      }
     }
     All_Find=0;
   }  
}
void Is_Single_Line()
{
  uint8 i=0,j=0,threshold=45;
  uint8 find=0;
  
  uint8 Single_Line_Detected=0;
  
  static int Width_Change_Cnt;
  static int Pre_Width_In,Preine_Out;
  int Width_In,Line_Out;
  
  static int Width_Change_Cnt_R;
  static int Pre_Width_In_R,Preine_Out_R;
  int Width_In_R,Line_Out_R;
  
  static int Single_Line_Cnt=0;
  
  
  Single_Line_Detected=0;
  
  
   if(Leftlastfind&&Rightlastfind&&Single_Out_Detect&&(RoadTypeSwitch&2))  //两边都找到线了并且有斜入单线
  {
    if((RisingEdgeIndex<RisingEdgeCnt-1)&&(Middle_Err<-12))  //右弯入单线
    {
      Width_In_R=Right-Left;
      Line_Out_R=RisingEdge[RisingEdgeIndex+1];
      if((Pre_Width_In_R>Width_In_R)&&(Line_Out_R<=Preine_Out_R)&&(Line_Out_R-Right<=6)&&Width_In_R<45)
      {
        if(ABS(CCD_Buff[Line_Out_R+3]-CCD_Buff[Right-3])<25)
        Width_Change_Cnt_R++;
        if(Width_Change_Cnt_R>=4)//4
        {
            find=1;
            i=Right;
            j=Line_Out_R;
        }
      }
      else 
      { 
        if((Pre_Width_In_R==Width_In_R)&&(Line_Out_R<=Preine_Out_R)&&(Line_Out_R-Right<=6)&&Width_In_R<45)
        Width_Change_Cnt_R=Width_Change_Cnt_R-1;
        else Width_Change_Cnt_R=0;
      }
      Pre_Width_In_R= Width_In_R;
      Preine_Out_R=Line_Out_R;
    }
    
    if((FallingEdgeIndex>=1)&&(Middle_Err>12))  //左弯入单线
    {
      Width_In=Right-Left;
      Line_Out=FallingEdge[FallingEdgeIndex-1];
       if((Pre_Width_In>Width_In)&&(Line_Out>=Preine_Out)&&(Left-Line_Out<=6)&&Width_In<45)
      {
        if(ABS(CCD_Buff[Line_Out-3]-CCD_Buff[Left+2])<25)
        Width_Change_Cnt++;
        if(Width_Change_Cnt>=4)//4
        {
            find=1;
            i=Line_Out;
            j=Left;
        }
      }   
      else 
      {
        if((Pre_Width_In==Width_In)&&(Line_Out>=Preine_Out)&&(Left-Line_Out<=6)&&Width_In<45)
        Width_Change_Cnt=Width_Change_Cnt-1;
        else Width_Change_Cnt=0;
      }
      Pre_Width_In=Width_In;
      Preine_Out=Line_Out;
    }
  }
  
  
 if(Single_Out_Detect==0)
 { 
  for(i=10;i<116;i++)         
  {
    if((CCD_Diff[i]<CCD_Diff[i-1])&&(CCD_Diff[i]<=CCD_Diff[i+1])&&(CCD_Diff[i]<-threshold))  //寻找负的峰值
    {
      for(j=i+2;j<=i+10;j++) 
      {
        if((CCD_Diff[j]>=CCD_Diff[j-1])&&(CCD_Diff[j]>CCD_Diff[j+1])&&(CCD_Diff[j]>threshold)) //寻找正的峰值
        { 
          if(ABS(CCD_Diff[i]+CCD_Diff[j])<30)  //峰值的绝对差小于一定值
          {
            
               if(Leftlastfind&&Rightlastfind)  //情况一，上次两边都找到了线
               {
                 if((i>Left+10)&&(j+10<Right))
                 {
                    find=1;
                 }
               }
                if(Leftlastfind&&Rightlastfind==0)  //情况二，上次只有一边找到线
               {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
                 if((i-Left>10)&&(i-Left<40))
                 {
                    find=1;
                 }
               }
                if(Leftlastfind==0&&Rightlastfind)  //情况三，上次只有一边找到线了
               {
                    if((Right-j>10)&&(Right-j<40))
                   {
                      find=1;
                   }
               }
               if(Leftlastfind==0&&Rightlastfind==0)  //情况四，上次两边都没找到线
               {
     
                    find=1;
               }
              
              Single_Line_Detected=1;
               
          }
          break;
        }
      }
    }
    if(find) break;
   }
  
  
  
    if((find==0)&&(Single_Line_Detected)&&(RoadTypeSwitch&2)&&((i-j)<=5))       //情况五，明显是单线
   {
     Single_Line_Cnt++;
     if(Single_Line_Cnt>3)
     { 
       Single_Line_Cnt=0;
       find=1;
     }
   }
   else  Single_Line_Cnt=0; 
  
  }
  
  
      if(find)
    {
       Single_Line_Cnt=0;
       Left=i;
       Right=j;
       RoadType=3;
       Leftlastfind=1;
       Rightlastfind=1;
       Character_Distance=Distance;
    }
   
   
}
void Brightness_Calculate()   //计算赛道中心亮度
{
  uint8 i;
  uint32 tempi=0;
  float tempf=0;
  for(i=Left;i<=Right;i++)
  {
    tempi+=CCD_Buff[i];
  }
  tempf=tempi*1.0/(Right-Left);
  
  tempf=tempf*(Max_Value-CCD_Offset)/127.0+CCD_Offset;
  Brightness=(int)(Brightness*0.95+tempf*0.05);
  CCD_Draw_Buff[140]=BYTE0(Brightness);
  CCD_Draw_Buff[141]=BYTE1(Brightness);  
}
void Find_Bothine()
{
    uint8 i=0,j=0,find=0;
    for(j=60;j>20;j--)
    {
       if((CCD_Diff[j]>=CCD_Diff[j-1])&&(CCD_Diff[j]>CCD_Diff[j+1])&&(CCD_Diff[j]>Threshold)) //寻找正的峰值
      {
       break;
      }
    }
     for(i=70;i<110;i++)
     {
        if((CCD_Diff[i]<CCD_Diff[i-1])&&(CCD_Diff[i]<=CCD_Diff[i+1])&&(CCD_Diff[i]<-Threshold))  //寻找负的峰值
        {
          break;
        }
     }
    if(j>25&&i<105)
    {
      find=1;
       Left=j;
       Right=i;
       Leftlastfind=1;
       Rightlastfind=1;  
    }
    else
    {    
      for(j=10;j<80;j++)         
      {
         if((CCD_Diff[j]>=CCD_Diff[j-1])&&(CCD_Diff[j]>CCD_Diff[j+1])&&(CCD_Diff[j]>Threshold)) //寻找正的峰值
        {
          for(i=j+1;i<=120;i++) 
          {
            if((CCD_Diff[i]<CCD_Diff[i-1])&&(CCD_Diff[i]<=CCD_Diff[i+1])&&(CCD_Diff[i]<-Threshold))  //寻找负的峰值
            {
              if(i-j>25)
              {
                 find=1;
                 Left=j;
                 Right=i;
                 Leftlastfind=1;
                 Rightlastfind=1; 
              }
              break;
            }
          }
        }
        if(find) break;
      } 
    }
    if(find==0)
    {
      Leftlastfind=0;
      Rightlastfind=0; 
    }
}
uint8 Is_Black_Line()
{
  uint8 i,j=0;
  uint32 tempi=0;
  float tempf=0;
  for(i=54;i<74;i++)  //计算中间几个点的亮度值
  {
   if(CCD_Buff[i]>10)  tempi+=CCD_Buff[i];
  }
  tempf=tempi*1.0/20;
  
  tempi=(int)(tempf*(Max_Value-CCD_Offset)/127.0+CCD_Offset);
  
  if(tempi<Brightness/2)
  {
    j=1;
  }
  else j=0;
  return j;
}
void Push_And_Pull(float *buff,int len,float newdata)
{
 int i;
 for(i=len-1;i>0;i--)
 {
   *(buff+i)=*(buff+i-1);
 }
   *buff=newdata; 
}