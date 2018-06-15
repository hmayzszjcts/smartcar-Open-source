#include "roundadout.h"
#include "control.h"
#include "string.h"
#include "path.h"
#include "fun.h"

/***************************************************************/
int ring_left_flag=0;
int ring_right_flag=0;
int ring_weight_change_flag=0;
int ring_fill_line_flag=0;
int ring_maxjiao_flag=0;
int maxjiao_num=0;
int normal_ring_flag=0;
int out_max_ring_jiao_flag=0;
int ring_hight_flag=0;
int out_ring_ok_flag=0;
int under_flag=0;
int guai_shizi_ring_flag=0;
int out_dasi_flag=0;
int out_ring_flag=0;
int press_flag=0;

int black_point_start=0;

int recognition_B_num=0;//环路尺寸 黑点数量

int start_line_ring_flag=0;//
/***************************************************************/
void recognition_size();//识别环岛大小
int line_lockup(uint16 line[],int line_val)
{
  int num=0;
  int flag=0;
  int ret=0;
  int total=0,av=0;
  for(num=5;num<=45;num++)
  {
    total=total+line[num];
  }
  av=total/(45-5);//平均数     动态的东西  动态方法解决
  for(num=5;num<=45;num++)
  {
    if(abs(line[num]-av)<6)
    {
        flag++;
    }
  
  }
  if(flag>30)//符合标准的行数
  {
      ret=1;
  }
  else
  {
      ret=0;  
  }
  return ret;
}


int get_line_av(uint16 line[],int start,int end,int av_val,int diff_val)//算一定范围内的平均值
{
  int i=start;
  int hang_total=0;
  int ret=0;
    for(;i<end;i++)
    {
        hang_total=hang_total+line[i];
    }
    ret=hang_total/(end-start);
    if(abs(ret-av_val)<diff_val)
    {
        ret=1;
    }
    else 
    {
        ret=0;
    }
    return ret;   

}





uint8 pick_pos(uint16 ary[])
{
  uint8 i=0;
  uint8 num=0;
  for(;num<10;num++)
  {
    if(ary[num]==1&&ary[num+1]==1&&ary[num+2]==1)//先判断是否丢线
    {
        return 0;  
    }
  }

    for(;i<50;i++)
    {   
        if(ary[i]!=40&&((ary[i+1]-ary[i]>5)||(ary[i]<5&&ary[i+1]<5)))
        {
            return i;
        }   
    }
  
  return i;
   
}
uint8 pick_pos1(uint16 ary[])
{
  uint8 i=0;
  uint8 num=0;
  for(;num<10;num++)
  {
    if(left_pos[num]==79&&left_pos[num+1]==79&&left_pos[num+2]==79)//先判断是否丢线
    {
        return 0;  
    }
  }

    for(;i<50;i++)
    {   
        if(left_pos[i]!=40&&((left_pos[i+1]-left_pos[i]>5)||(left_pos[i]<5&&left_pos[i+1]<5)))
        {
            return i;
        }   
    }
  
  return i;
   
}
int get_val(int start,int end,uint16 a[],int flag)
{

  	int i, j, k;
	int ret = 0;
    int total=0,av=0;
    if(ring_right_flag)//右侧环路  出环路的时候是50
    {
         for(i=start;i<end; i++)//求平均值
        {
          if(a[i]!=0)//丢行跳出
          {
                total=total+a[i];   
          }
          else //不丢行就累加
          {
                 break; 
          }        
        }
        end=i;
        i=0;
        
        for(i=start;i<end;i++)//右侧环路找折点
        {
            if((a[i]<a[i+2]-1)||(a[i]==40&&a[i+1]==40&&a[i+2]==40))
            {
              ret=i;//返回第一个折点的行数
                break;
            }
        }

    }
    else if(ring_left_flag)//出环路的时候是  0  
    {
        for(i=start;i<end; i++)//求平均值
        {
          if(a[i]!=0)//丢行跳出
          {
                total=total+a[i];   
          }
          else //不丢行就累加
          {
                 break; 
          }        
        }
        end=i;
        i=0;
        
        for(i=start;i<end;i++)//右侧环路找折点
        {
            if((a[i]-1>a[i+2])||(a[i]==40&&a[i+1]==40&&a[i+2]==40))
            {
              ret=i;//返回第一个折点的行数
                break;
            }
            if(i==0)//说明没有找到拐点
            {
               ret=end; 
            }
        }
    
    }   

	return ret;
}

int get_line_weight(int start,int end)
{
  int num=0;
  int weight=0;
  int zero_num=0;
    for(num=start;num<end;num++)
    {
      if(line_weight[num]==0)
      {
        //zero_num++;
        weight=weight+80;
      }
      else 
      {
        weight=weight+line_weight[num];
      }
        
    }
//    weight=weight/(end-start-zero_num);
      weight=weight/(end-start);
    return weight;
}

int right_ring_find_again()//再次寻找右侧环路
{
    int ring_hang_start=8,ring_hang_end=45;//寻找拐点的起始行
    int i=0,j=0;
    int diu_hang_num=0;
    int tuqi_point_hang=0;
    int flag_num=0;
    int anther_diuhang_num=0;
    for(i=ring_hang_start;i<ring_hang_end;i++)
    {
        if(right_pos[i]==1&&right_pos[i+2]==1&&right_pos[i+4]==1)//有五行丢线
        {
            diu_hang_num=i;
            diu_hang_num=diu_hang_num+4;
            break;
        }    
    }
    ring_hang_start=8,ring_hang_end=45;
    if(diu_hang_num>8)//
    {
           for(i=ring_hang_start;i<ring_hang_end;i++)
          {
              if((right_pos[i]<=right_pos[i+3]&&right_pos[i+3]>=right_pos[i+6])&&(right_pos[i]!=1&&right_pos[i+3]!=1&&right_pos[i+6]!=1))//有五行丢线   增  减
//            if((right_pos[i]<right_pos[i+3]&&right_pos[i+3]>right_pos[i+8])&&(right_pos[i]!=1||right_pos[i+3]!=1||right_pos[i+8]!=1))//有五行丢线   增  减
              {
                  tuqi_point_hang=i+2;//前后小中间大的点行数
                  break;
              }    
          }
    }
    if(tuqi_point_hang<=40&&tuqi_point_hang!=0)//只要  再加上另一侧判断直线  就可以断定斜入环，或者前面的标志位漏判断了
    {
        for(i=tuqi_point_hang;i>8;i--)
        {
            if(right_pos[i]==1)
            {
                flag_num=i;//保存个数  通过个数判断是 入环  还是出环    
                break;
            }        
        }        
    }
    if(flag_num!=0&tuqi_point_hang-flag_num>10)
    {
      for(j=tuqi_point_hang;j<50;j++)//
      {
          if(right_pos[j]<10)
          {
            anther_diuhang_num++;          //入环多出环少     小环多    大环少
          }
      }        
    }  
 
    if(flag_num!=0&tuqi_point_hang-flag_num>10&&line_lockup(Bline_left,129)==1&&anther_diuhang_num>=4)
    {
        ring_right_flag=1;
        zhidao_count_flag=0;//直道标志位置0
    }   
        
    return ring_right_flag;
    
}



int left_ring_find_again()//再次寻找zuo侧环路
{
    int ring_hang_start=8,ring_hang_end=45;//寻找拐点的起始行
    int i=0,j=0;
    int diu_hang_num=0;
    int tuqi_point_hang=0;
    int flag_num=0;
    int anther_diuhang_num=0;
    for(i=ring_hang_start;i<ring_hang_end;i++)
    {
        if(left_pos[i]==79&&left_pos[i+2]==79&&left_pos[i+4]==79)//有五行丢线
        {
            diu_hang_num=i;
            diu_hang_num=diu_hang_num+4;
            break;
        }    
    }
    ring_hang_start=8,ring_hang_end=45;
    if(diu_hang_num>8)//
    {
           for(i=ring_hang_start;i<ring_hang_end;i++)
          {
              if((left_pos[i]>=left_pos[i+3]&&left_pos[i+3]<=left_pos[i+6])&&(left_pos[i]!=79&&left_pos[i+3]!=79&&left_pos[i+6]!=79))//有五行丢线   增  减
//            if((right_pos[i]<right_pos[i+3]&&right_pos[i+3]>right_pos[i+8])&&(right_pos[i]!=1||right_pos[i+3]!=1||right_pos[i+8]!=1))//有五行丢线   增  减
              {
                  tuqi_point_hang=i+2;//前后小中间大的点行数
                  break;
              }    
          }
    }
    if(tuqi_point_hang<=40&&tuqi_point_hang!=0)//只要  再加上另一侧判断直线  就可以断定斜入环，或者前面的标志位漏判断了
    {
        for(i=tuqi_point_hang;i>8;i--)
        {
            if(left_pos[i]==79)
            {
                flag_num=i;//保存个数  通过个数判断是 入环  还是出环    
                break;
            }        
        }        
    }
    if(flag_num!=0&tuqi_point_hang-flag_num>10)
    {
      for(j=tuqi_point_hang;j<50;j++)//
      {
          if(left_pos[j]>=72)
          {
            anther_diuhang_num++;          //入环多出环少     小环多    大环少
          }
      }        
    }  
 
    if(flag_num!=0&tuqi_point_hang-flag_num>10&&line_lockup(Bline_right,129)==1&&anther_diuhang_num>=4)
    {
        ring_left_flag=1;
        zhidao_count_flag=0;//直道标志位置0
    }   
        
    return ring_left_flag;
    
}





int  start_run_wupan(int hang)
{
    int ret=0;
    int num=35;
    int flag=0;
    for(num;num<45;num++)//固定行刷新列   有拐点 误判
    {
        if(get_img_point(hang,num)!=0)
        {
            flag++;
        }    
    }
    if(flag>=3)
    {
        ret=1;
    }
    return ret;
}


int ring_hang_pos=0,ring_lie_pos=0;
void round_deal()
{
  
  int i=0;
  int ring_hang_start=8,ring_hang_end=45;//寻找拐点的起始行
  int left_guai_point=0,right_guai_point=0;//拐点行数
  int left_guai_point_lie=0,right_guai_point_lie=0;//拐点行数
  int tiaobian_left=0,tiaobian_right=0;
  
  int left_w_flag=0,right_w_flag=0;
  int left_w_flag_1=0,right_w_flag_1=0;
  
  /********************************右侧-拐点直线************************************/
  for(i=ring_hang_start;i<ring_hang_end;i++)
  {
      if(right_pos[i]!=40&&(right_pos[i]-2>(right_pos[i+3])||(right_pos[i]<4&&right_pos[i+1]<4&&right_pos[i+2]<4)))
      {
        right_guai_point=i;//保存右侧拐点行坐标     
        break;       
      }   
  }
  if(right_guai_point==8)//说明不在环路上或者环路漏判断
  {
       right_ring_find_again();//歪入环或者 漏判 
  }
  if(right_guai_point>=8&&right_guai_point<30)//找到拐点后，查看另一侧是否为直线(限制拐点出现的范围)
  {  
    
     if(start_run_wupan(right_guai_point))//防止起跑线误判
    {
        ring_right_flag=0;
        ring_right_flag=0;
        return ;//起跑线误判
    }        
    
    for(;i<55;i++)//找到拐点以后，找拐点上面的跳变点
    {//固定列扫描行
        if(get_img_point(i,right_pos[right_guai_point])==0&&get_img_point(i+1,right_pos[right_guai_point])!=0)
        {
            black_point_start=i;//保存黑点
            tiaobian_right=i-right_guai_point;
            break;
        }            
    }
    if(tiaobian_right>15&&tiaobian_right<27)// 9  28
    {
      
        for(i=right_pos[right_guai_point];i>=1;i--)//固定行，向有拐点的一侧扫描列
        {//第一次拐点的位置向同一侧找，没有黑点就说明是环岛的拐点，有黑点说明拐点比较缓慢是大弯道
            if(get_img_point(right_guai_point+2,i)==0)
            {
                right_w_flag++;
            } 
            if(get_img_point(right_guai_point+3,i)==0)
            {
                right_w_flag_1++;
            } 
        }
        if(right_w_flag>=5&&right_w_flag_1>=5)
        {
           //在右侧找到拐点，然后寻找左侧是否为直线
          if(line_lockup(Bline_left,129)==1)
          {            
              ring_hang_pos=right_guai_point;//保存下来
              ring_lie_pos=right_pos[ring_hang_pos];
              zhidao_count_flag=0;//直道标志位置0
              ring_right_flag=1;
          }  
          else//之前找到的拐点无效
          {
                right_guai_point=0;
                
          }
        }      
    
    }  
    
  } 
 
  /********************************左侧-拐点直线************************************/
  if(right_guai_point==0||right_guai_point==8)//未找到对的拐点
  {
      i=0;
      for(i=ring_hang_start;i<ring_hang_end;i++)
      {
          if(left_pos[i]!=40&&((left_pos[i]<left_pos[i+3]-2)||(left_pos[i]>76&&left_pos[i+1]>76&&left_pos[i+2]>76)))
          {
            left_guai_point=i;//保存左侧拐点行坐标
            break;       
          }   
      }
      if(left_guai_point==8)//说明不在环路上或者环路漏判断
      {
        left_ring_find_again();//歪入环或者 漏判 
      }
      if(left_guai_point>=8&&left_guai_point<30)//找到拐点后，查看另一侧是否为直线(限制拐点出现的范围)
      {        
        
        if(start_run_wupan(right_guai_point))//防止起跑线误判
        {
            ring_right_flag=0;
            ring_right_flag=0;
            return ;//起跑线误判
        }                       
        
        for(;i<55;i++)//找到拐点以后，找拐点上面的跳变点
        {//固定列扫描行
            if(get_img_point(i,left_pos[left_guai_point])==0&&get_img_point(i+1,left_pos[left_guai_point])!=0)
            {   
                black_point_start=i;//保存黑点
                tiaobian_left=i-left_guai_point;//找到跳变点与拐点的距离
                break;
            }
        }
        
        if(tiaobian_left>15&&tiaobian_left<27)//跳变点距离限定范围  9  28
        {
          for(i=left_pos[left_guai_point];i<=79;i++)//固定行，向有拐点的一侧扫描列
          {//第一次拐点的位置向同一侧找，没有黑点就说明是环岛的拐点，有黑点说明拐点比较缓慢是大弯道
                if(get_img_point(left_guai_point+2,i)==0)
                {
                    left_w_flag++;//拐点同侧白点个数
                }         
                if(get_img_point(left_guai_point+3,i)==0)
                {
                    left_w_flag_1++;
                }
          }
          if(left_w_flag>=5&&left_w_flag_1>=5)//left_w_flag_1=0,right_w_flag_1=0;
          {
                 //在左侧找到拐点，然后寻找右侧是否为直线
              if(line_lockup(Bline_right,67)==1)
              {
                recognition_size();//识别环岛大小  
                
                ring_hang_pos=left_guai_point;//保存下来
                ring_lie_pos =left_pos[ring_hang_pos];
                zhidao_count_flag=0;//直道标志位置0
                ring_left_flag=1;
              }  
              else//之前找到的拐点无效
              {
                    right_guai_point=0;
              }
          }          
       
        }
        
      }   
  }

  //左右标志区分结束
  /***************************************************************************/
  if(ring_left_flag||ring_right_flag)//环路一测拐点和一侧直线找到后   寻找 
  {//赛道正常宽度  62  63  64
    //过了拐点  赛道宽度变为  74 75 76 77
    if(get_line_weight(5,15)>70)//70
    {
        ring_weight_change_flag=1;//赛道宽度变宽了，    
    }  
  }
  else
  {
        return ;//向左 向右的环路标志都没有找到
  }
  if(ring_weight_change_flag)
  {
//        if(get_line_weight(5,15)<=68)
        if(get_line_weight(5,15)<=68&&get_line_weight(25,35)>=70)
        {
            ring_fill_line_flag=1;//赛道变窄了，说明到达了直道和圆环的切点位置
        }  
        
//        else 
//        {
////            ring_weight_change_flag=0;
////            ring_left_flag=0;
////            ring_right_flag=0;
//        }
  } 

}

void out_ring_under_find_line()
{
    int start=0,end=5;
    int hang=0,lie=0;
    for(hang=start;hang<end;hang++)
    {
      for(lie=40;lie<80;lie++)//从中间向左侧
      {
        if(get_img_point(hang,lie)==0&&get_img_point(hang,lie+1)!=0)
        {
            left_pos[hang]=lie;
        }
        else if(lie>=78)
        {
            left_pos[hang]=79;
        }
      }    
    }
    for(hang=start;hang<end;hang++)
    {
          for(lie=40;lie>0;lie--)
          {
            if(get_img_point(hang,lie)==0&&get_img_point(hang,lie-1)!=0)
            {
                right_pos[hang]=lie;
            }
            else if(lie<=2)
            {
               right_pos[hang]=1; 
            }
          
          }
    }
    
              
}



int find_out_ring_flag(int start,int end)//寻找出环路的标志
{
    int num=0,flag=0;
    int total=0,av=0;
    int hang_num=0;
    int left_num=0,right_num=0;
    
    if(Shi_zi_flag)//丢行
    {
        return 0;
    }
    
    for(hang_num=end;hang_num>start+1;hang_num--)
    {
        if(left_pos[hang_num]==79&&left_pos[hang_num-1]==79&&left_pos[hang_num-2]==79&&left_pos[start+1]==79&&left_pos[start+2]==79)//找到附近丢边并且远处不丢边的行
        {
            left_num=hang_num;
            break;
        }
    }
        
//    if(ring_left_flag&&left_num<=40);//左侧环岛清零状态ring_left_flag
if(ring_left_flag);//左侧环岛清零状态ring_left_flag
    {
      
        for(num=start;num<=end;num++)
        {
            total=total+Bline_right[num];
        }
        av=total/(end-start);//平均数     动态的东西  动态方法解决
        for(num=start;num<=end;num++)
        {
            if(abs(Bline_right[num]-av)<=10)//10
            {
                flag++;
            }
        }//17>45-7-15   17>23
        
        if(flag>(end-start-10))//符合标准的行数  38-20  
        {
            out_ring_ok_flag=1;
            ring_clear();
            return 1;
        }
        
    } 
    
    hang_num=start;
    for(hang_num=end;hang_num>start+1;hang_num--)
    {
        if(right_pos[hang_num]==1&&right_pos[hang_num-1]==1&&right_pos[hang_num-2]==1&&right_pos[start+1]==1&&right_pos[start+2]==1)//找到附近丢边并且远处不丢边的行
        {
            right_num=hang_num;
            break;
        }
    }
//    if(ring_right_flag&&right_num<=40)
     if(ring_right_flag)
    {
       for(num=start;num<=end;num++)
        {
            total=total+Bline_left[num];
        }
        av=total/(end-start);//平均数     动态的东西  动态方法解决
        for(num=start;num<=end;num++)
        {
            if(abs(Bline_left[num]-av)<=10)//
            {
                flag++;
            }
        }
        if(flag>(end-start-10))//符合标准的行数
        {
            out_ring_ok_flag=1;
            ring_clear();
            return 1;
        }

    }
    
    return 0;
}

int add_under_hang()
{
  int ret=0;
  int num=0;
  int left_total=0,right_total=0;
    for(num;num<5;num++)
    {
        if(left_pos[num]==79||left_pos[num]==40)//丢行
        {
             return 0;
        }
        else
        {
            left_total=left_total+left_pos[num];
        }
    }
    left_total=left_total/5;//71
    for(num=0;num<=5;num++)
    {
        if(right_pos[num]==1||right_pos[num]==40)//丢行
        {
             return 0;
        }
        else
        {
            right_total=right_total+right_pos[num];//13
        }
    }
    right_total=right_total/5;
    
    if(abs(left_total-71)<5&&abs(right_total-13)<5)
    {
        return 1;
    }
    return 0;

}

int find_center_pos()
{
   int ret=0;
   int hang=0;
   int lie=40;
   for(hang=0;hang<50;hang++)
   {
        if(get_img_point(hang,lie)==0&&get_img_point(hang+1,lie)!=0)
        {
            ret=hang;
        }
   }
   if(ret==0)
   {
        ret=50;
   }
   return ret;

}

void ring_clear()
{
            ring_left_flag=0;
            ring_right_flag=0;
            ring_weight_change_flag=0;
            ring_fill_line_flag=0;
            ring_maxjiao_flag=0;
            maxjiao_num=0;
            normal_ring_flag=0;
            out_max_ring_jiao_flag=0;
            ring_hight_flag=0;
            out_ring_ok_flag=0;
            under_flag=0;
            guai_shizi_ring_flag=0;
 
}



int fing_enter_pos()
{
  int num=0;
  int ret =0;
    if(ring_right_flag)//右边环路查右边
    {
        for(num=5;num<45;num++)
        {
            if(Bline_right[num]!=40&&Bline_right[num]+3<Bline_right[num+1])
            {
                ret=num;
                return  ret;           
            }
        }
    }
    else if(ring_left_flag)
    {
        for(num=5;num<45;num++)
        {
            if(Bline_left[num]!=40&&Bline_left[num]>Bline_left[num+1]+3)
            {
                ret=num;
                return  num;           
            }
        }
    }

}

int  find_diu_hang(int temp_val)
{
    int ret=0;
    int num=0;
    int flag=0;
    if(ring_right_flag)
    {
        for(num=0;num<temp_val;num++)
        {
            if(right_pos[num]==1||right_pos[num]==40)
            {
                flag++;
            }
        }
        if(flag>=temp_val&&flag>8)
        {
            ret=1;
            return ret;
        }
    }
    else if(ring_left_flag)
    {
        for(num=0;num<temp_val;num++)
        {
            if(left_pos[num]==79||left_pos[num]==40)
            {
                flag++;
            }
        }
//         if(temp_val>=24)
       if(flag>=temp_val&&flag>8)
        {
            ret=1;
            return ret;
        }
    }
    

}
//寻找出环的边线的折点
int  get_out_ring_zhedian()
{
    int hang=0;
    int fact_pos_hang=0;
    int jiaozheng_pos_hang=0;
    int ret=0;
    int lost_hang=0;int lost_hang_num=30;//从30行向上找是否有丢行的 
    if(ring_right_flag)//右环路
    {
        for(hang=8;hang<58;hang++)//先查看实际点  在哪里找不到点 说明在哪里丢行了
        {
            if((left_pos[hang]==79&&left_pos[hang+1]==79&&left_pos[hang+2]==79&&left_pos[hang+3]==79&&left_pos[hang+4]==79)||(left_pos[hang]==40&&left_pos[hang+1]==40&&left_pos[hang+2]==40)||(left_pos[hang]<left_pos[hang+2]))
            {
                fact_pos_hang=hang;//保存实际的点，这个有可能包含折点，但是可以找到空白行
                break;
            }
        }
        for(hang=8;hang<58;hang++)
        {
            if(Bline_left[hang]!=40&&(Bline_left[hang]+2<Bline_left[hang+2]))
            {
                jiaozheng_pos_hang=hang;//矫正后的数组折点
                break;
            }
        }
        if(fact_pos_hang>=jiaozheng_pos_hang)ret=jiaozheng_pos_hang;
        else ret=fact_pos_hang;//返回一个最小的，才是最真实的
          
        
    }
    else if(ring_left_flag)//左环路
    {
        for(hang=8;hang<58;hang++)//先查看实际点  在哪里找不到点 说明在哪里丢行了
        {
            if((right_pos[hang]==1&&right_pos[hang+1]==1&&right_pos[hang+2]==1&&right_pos[hang+3]==1&&right_pos[hang+4]==1)||(right_pos[hang]==40&&right_pos[hang+1]==40&&right_pos[hang+2]==40)||(right_pos[hang]>right_pos[hang+2]))
            {
                fact_pos_hang=hang;//保存实际的点，这个有可能包含折点，但是可以找到空白行
                break;
            }
        }
        for(hang=8;hang<58;hang++)
        {
            if(Bline_right[hang]!=40&&(Bline_right[hang]>Bline_right[hang+2]+2))
            {
                jiaozheng_pos_hang=hang;//矫正后的数组折点
                break;
            }
        }
        if(fact_pos_hang>=jiaozheng_pos_hang)ret=jiaozheng_pos_hang;
        else ret=fact_pos_hang;//返回一个最小的，才是最真实的
    }
    if(ret>8)
    {
        valid_line=ret;//控制折点前的有效行  避免远处的有效行偏差，影响近处的
    }
    if(fact_pos_hang==0&&jiaozheng_pos_hang==0)//两个都没找到折点，说明没有折点
    {
        ret=valid_line;//没有折点 折点行数为有效行
    }
    if(fact_pos_hang==8&&jiaozheng_pos_hang==0)//
    {
        ret=valid_line;
    }
    if(fact_pos_hang==8&&jiaozheng_pos_hang>9)
    {
        ret=valid_line;
    }
//    if(ret==0)//等于0的状况 说明有一个是零  看两个差值
//    {
//        if(abs(fact_pos_hang-jiaozheng_pos_hang)>=25)
//        {
//            if(fact_pos_hang>=jiaozheng_pos_hang)ret=fact_pos_hang;
//            else ret=jiaozheng_pos_hang;//返回一个最小的，才是最真实的
//        }
//    }
    
    return ret;

}
//int  get_out_ring_zhedian()
//{
//    int hang=0;
//    int fact_pos_hang=0;
//    int jiaozheng_pos_hang=0;
//    int ret=0;
//    int lost_hang=0;int lost_hang_num=30;//从30行向上找是否有丢行的 
//    if(ring_right_flag)//右环路
//    {
//        for(hang=8;hang<58;hang++)//先查看实际点  在哪里找不到点 说明在哪里丢行了
//        {
//            if((left_pos[hang]==79&&left_pos[hang+1]==79&&left_pos[hang+2]==79&&left_pos[hang+3]==79&&left_pos[hang+4]==79)||(left_pos[hang]==40&&left_pos[hang+1]==40&&left_pos[hang+2]==40)||(left_pos[hang]<left_pos[hang+2]))
//            {
//                fact_pos_hang=hang;//保存实际的点，这个有可能包含折点，但是可以找到空白行
//                break;
//            }
//        }
//        for(hang=8;hang<58;hang++)
//        {
//            if(Bline_left[hang]!=40&&(Bline_left[hang]+2<Bline_left[hang+2]))
//            {
//                jiaozheng_pos_hang=hang;//矫正后的数组折点
//                break;
//            }
//        }
//        if(fact_pos_hang>=jiaozheng_pos_hang)ret=jiaozheng_pos_hang;
//        else ret=fact_pos_hang;//返回一个最小的，才是最真实的
//          
//        
//    }
//    else if(ring_left_flag)//左环路
//    {
//        for(hang=8;hang<58;hang++)//先查看实际点  在哪里找不到点 说明在哪里丢行了
//        {
//            if((right_pos[hang]==1&&right_pos[hang+1]==1&&right_pos[hang+2]==1&&right_pos[hang+3]==1&&right_pos[hang+4]==1)||(right_pos[hang]==40&&right_pos[hang+1]==40&&right_pos[hang+2]==40)||(right_pos[hang]>right_pos[hang+2]))
//            {
//                fact_pos_hang=hang;//保存实际的点，这个有可能包含折点，但是可以找到空白行
//                break;
//            }
//        }
//        for(hang=8;hang<58;hang++)
//        {
//            if(Bline_right[hang]!=40&&(Bline_right[hang]>Bline_right[hang+2]+2))
//            {
//                jiaozheng_pos_hang=hang;//矫正后的数组折点
//                break;
//            }
//        }
//        if(fact_pos_hang>=jiaozheng_pos_hang)ret=jiaozheng_pos_hang;
//        else ret=fact_pos_hang;//返回一个最小的，才是最真实的
//    }
//    if(ret>8)
//    {
//        valid_line=ret;//控制折点前的有效行  避免远处的有效行偏差，影响近处的
//    }
//    if(fact_pos_hang-jiaozheng_pos_hang==0)//两个都没找到折点，说明没有折点
//    {
//        ret=valid_line;//没有折点 折点行数为有效行
//    }
//    if(ret==0)//等于0的状况 说明有一个是零  看两个差值
//    {
//        if(abs(fact_pos_hang-jiaozheng_pos_hang)>=25)
//        {
//            if(fact_pos_hang>=jiaozheng_pos_hang)ret=fact_pos_hang;
//            else ret=jiaozheng_pos_hang;//返回一个最小的，才是最真实的
//        }
//    }
//    
//    return ret;
//
//}

void recognition_size()//识别环岛大小
{
    int hang=0;
    int lie=0;
    recognition_B_num=0;
    if(ring_right_flag||ring_left_flag)//右环岛 或左侧
    {
        if(ring_right_flag){lie=right_pos[black_point_start]-1;hang=black_point_start;}
        else if(ring_left_flag){lie=left_pos[black_point_start]-1;hang=black_point_start;}
        for(;hang<60;hang++)
        {
          if(get_img_point(hang,lie)!=0)
          {
            recognition_B_num++;
          }
          if(recognition_B_num>5&&get_img_point(hang,lie)==0)//遇到白点
            break;
     }
     }
    
}

int get_guai_val()
{
    int ret=0;
    if(recognition_B_num>=12)//大环
    {
        ret=10;//打死值    
    }
    else
    {
        ret=30;
    }
    
    return ret;
}