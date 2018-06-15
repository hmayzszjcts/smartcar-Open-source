#include "path.h"
#include "direction.h"
#include "fun.h"
#include <math.h>
#include <string.h>
#include  "MK60_FTM.h"
#include "system_MK60DZ10.h"
#include "control.h"
#include  "info_deal_save.h"
#include "ring.h"
#include "speed_new.h"
#include "roundadout.h"

uint8 guai_dian_count=0;
uint8 donw_guan_dian=0;
uint8 gl_zhidao_count=0;
uint8 gl_path_type;
int ramp_flag;
int line_weight[60]={0};

//功能 ： 得到图像中指定的点
//参数 ： h 行60  w 列80
//返回值 ： 白 0   黑 非0
int get_img_point(uint16 h,uint16 w)
{
    char * pimg=(char *)IMG_BUF;
//    uint8  ch=pimg[((img_size.W>>3)*h)+w>>3];
    uint8  ch=pimg[(IMG_W*h+w)>>3];
    ch&=(1 << (7 - (w & 0x07) ));
    return ch!=0;
//    return pimg[((img_size.W>>3)*h)+w>>3]& (1 << (7 - (w & 0x07) ));
}               
//功能 ：画一下找到的边线，用于测试
int test_draw_line(Line_struct line,uint16 color)
{
	int start=line.start_point;
	int end=line.end_point;
	Site_t site;
	for(;start<=end;start++)
	{
		site.x=line.point[start];
		site.y=IMG_H-start-1;
		LCD_point(site,color);
	}
	return 0;
}
int test_draw_point(uint16 line,uint16 x,uint16 color)
{
    Site_t site;   
    site.x=x;
    site.y=line;
    LCD_point(site,color);
    return 0;
}

/*******************************************************************************
函数名称：If_straight
函数功能: 判断小车是否在直道上方法
参数：
*******************************************************************************/
uint8 If_straight(uint8 start,uint8 end,uint8 val)
{
	uint8 line;
	uint8 count=0,line_w,guai_dian_num=0;
	uint8 guai_dian_y[10]={0};//
	int16 sum_1,sum_2;
	int last_line_x=Pick_table[start],last_index=0,guai_dian_start=Pick_table[start];
	for(line=start;line<=end;line++)
	{
		line_w=abs_sub(Pick_table[line],196/2);
		if(line_w<=val)
			count++;
		if(line<=end&&((Pick_table[line]-last_line_x)*last_index<0))
		{
		    guai_dian_y[guai_dian_num++]=line;  
		}
		if(Pick_table[line]-last_line_x!=0)
		    last_index=Pick_table[line]-last_line_x;
		last_line_x=Pick_table[line];
	}
	for(line=0;line<guai_dian_num;line++)
	{
		if(line<end-5)
		{
			if(line==guai_dian_num-1)
			{
				sum_1=Pick_table[guai_dian_y[line]]-guai_dian_start;
				sum_2=Pick_table[guai_dian_y[line]]-Pick_table[end];
				if((sum_1)*(sum_2)>0)
				{
					if(abs(sum_1)>3&&abs(sum_2)>3)
					{
						if(donw_guan_dian==0)
						donw_guan_dian=guai_dian_y[line];
						guai_dian_count++;
					}
				}
			}
			else
			{
			  sum_1=Pick_table[guai_dian_y[line]]-guai_dian_start;
			  sum_2=Pick_table[guai_dian_y[line]]-Pick_table[guai_dian_y[line+1]];
			  if((sum_1*sum_2)>0)
			  {
			      if(abs(sum_1)>=3&&abs(sum_2)>=3)
			      {
			          if(donw_guan_dian==0)
			              donw_guan_dian=guai_dian_y[line];
			          guai_dian_count++;
			      }
			  }
			}
		}
	  guai_dian_start=Pick_table[guai_dian_y[line]];
	}
	return count;
}

/*******************************************************************************
函数名称：getBlineCenter
函数功能: 获取赛道中心线
参数：无
*******************************************************************************/
void getBlineCenter()
{
  uint8 line;
  Site_t site;

  uint16 x,fill_line=FILL_LEN/2;
  
  for(line=START_Y;line<=valid_line;line++)
  {
      if(((Deal_flag[line]&DEAL_LEFT_LOST)==0)&&((Deal_flag[line]&DEAL_RIGHT_LOST)!=0))//右边线丢失(根据左边线补中线)
      {
          if(Bline_left[line]>fill_line)
          {
                Pick_table[line]=Bline_left[line]-fill_line;
          }
          else
          {       
              Pick_table[line]=0;
              Deal_flag[line]|=INVALID_LINE;
          }
      }
      else if(((Deal_flag[line]&DEAL_LEFT_LOST)!=0)&&((Deal_flag[line]&DEAL_RIGHT_LOST)==0))//左边线丢失(根据右边线补中线)
      {
            if(Bline_right[line]>fill_line)
            {
                    Pick_table[line]=Bline_right[line]+fill_line;
            }
            else
            {
              Pick_table[line]=0;
              Deal_flag[line]|=INVALID_LINE;
            }
      }
      else
      {
        Pick_table[line]=(Bline_left[line]+Bline_right[line])/2;
        fill_line=(Bline_left[line]-Bline_right[line])/2;
      }            
  }   

  if(my_debug_flag==1)
  {
      if(my_debug_flag==1)
      {
          for(line=START_Y;line<=valid_line;line++)
            test_draw_point(LCD_H-line,LCD_W-Pick_table[line]+50,0XF800);
      }
      for(x=40;x<80;x++)
      {
          site.x=x+50;
          site.y=LCD_H-valid_line;
          LCD_point(site,0XFFE0);
      }
  }
}

/*******************************************************************************
用于检测到环形以后补线
*/
void with_left_to_center()
{
  uint8 line;
  Site_t site;
  uint16 x,fill_line=FILL_LEN/2;

  for(line=START_Y;line<=valid_line;line++)
  {
    Pick_table[line]=Bline_left[line]-(fill_line)+25;//+20  15
  }

//   valid_line= pick_pos1(left_pos);//求出实际的有效行
//  if(!valid_line)//底部丢线
//  {
//    ring_maxjiao_flag=1;
//  }
//  else
//  {
//    ring_maxjiao_flag=0;//没丢线就正常补线
//
//  }
  
  if(my_debug_flag==1)
  {
      if(my_debug_flag==1)
      {
          for(line=START_Y;line<=valid_line;line++)
            test_draw_point(LCD_H-line,LCD_W-Pick_table[line]+50,0XF800);
      }
      for(x=40;x<80;x++)
      {
          site.x=x+50;
          site.y=LCD_H-valid_line;
          LCD_point(site,0XFFE0);
      }
  }


}
/************************************************************************/
void with_right_to_center()
{
  uint8 line;
  Site_t site;
  uint16 x,fill_line=FILL_LEN/2;
  uint16 temp=0; 
  int val_ring=10;//大圆环用的有效行
  if(ring_size>=48)//大圆环
  {
        val_ring;
  }
  else
  {
        val_ring=valid_line;
  }
  for(line=START_Y;line<=val_ring;line++)
  {
    temp=Bline_right[line]+(fill_line)-25;//-20  -15
    if(temp<5)
    {
        temp=2;
    }
    Pick_table[line]=temp;//20
  }
//   valid_line= pick_pos(right_pos);//求出实际的有效行
//  if(!valid_line)//底部丢线
//  {
//    ring_maxjiao_flag=1;
//  }
//  else
//  {
//    ring_maxjiao_flag=0;//没丢线就正常补线
//
//  }
      if(my_debug_flag==1)
      {
          if(my_debug_flag==1)
          {
              for(line=START_Y;line<=valid_line;line++)
                test_draw_point(LCD_H-line,LCD_W-Pick_table[line]+50,0XF800);
          }
          for(x=40;x<80;x++)
          {
              site.x=x+50;
              site.y=LCD_H-valid_line;
              LCD_point(site,0XFFE0);
          }
      }
  

}




/*******************************************************************************
函数名称：averageLvBo
函数功能: 均值滤波
参数：无
*******************************************************************************/
void averageLvBo()
{
  uint8 line;
  for(line=START_Y+2;line<=(valid_line-2);line++)
  {
	Bline_left[line]=(Bline_left[line-1]+Bline_left[line-2]+Bline_left[line]
					  +Bline_left[line+1]+Bline_left[line+2])/5;
        Bline_right[line]=(Bline_right[line-1]+Bline_right[line-2]+Bline_right[line]
					  +Bline_right[line+1]+Bline_right[line+2])/5;
  }
 
}
/*******************************************************************************
函数名称：averageLvBo
函数功能: 均值滤波
参数：无
*******************************************************************************/
void bianxian_averageLvBo()
{
  uint8 line;
  for(line=START_Y+2;line<=(valid_line-2);line++)
  {
	Pick_table[line]=(Pick_table[line-1]+Pick_table[line-2]+Pick_table[line]
					  +Pick_table[line+1]+Pick_table[line+2])/5;
  }
}
/*********************************************************************************
计算整形数组里面的所有数据的和
*/
int add_arrary(int a[],int num)
{ 
  int i=0,s=0;
  for(;i<num;i++)
  {
    s=s+a[i];
  }
  return s;
}
/*********************************************************************************
测试 像素点的横纵坐标
*/
int total=1;
void test()
{
  int x=1,y=1;
  while(total)
  {
    for(x=0;x<79;x++)
    {
        for(y=0;y<59;y++)
        {
              if(get_img_point(y,x))
              {
                printf("1");              
              }
              else
              {
                printf("0");   
              }
        
        }
        printf("\n");
  
      }
    total=0;

  }
}

/*******************************************************************************
函数名称：bDistance
函数功能: 赛道宽度法滤波 去除无效行
参数：无
*******************************************************************************/
void bDistance()
{
  uint8 line,shizi_count=0,shizi_start_line=0;
  uint16 distance=90,zhangai_max_len=46,zhang_ai_num=0;;
  uint16 line_w=0;
  uint16 line_fact_w=0;
//  int b_point_num_jin_loop=0;
  int total_line_w=0;
  int av_fact_w=0;
  
  ramp_flag=0;
  
  for(line=START_Y;line<=valid_line;line++)//START_Y
  {
      line_w=(Bline_left[line]-Bline_right[line]);
      
      line_fact_w=left_pos[line]-right_pos[line];
      
      line_weight[line]=line_w;//收集赛道宽度的数组
      
      
      total_line_w=total_line_w+line_w;
      if(((line_w>distance)||(Bline_left[line]<Bline_right[line])))
      {
        if(Pick_flag[line]==0)
        {
            Deal_flag[line] |= INVALID_LINE;
        }
        else
        {
          if((Deal_flag[line]&DEAL_LEFT_LOST)&&Bline_right[line]<Bline_right[line-1])
          {
              Deal_flag[line] |= INVALID_LINE;
              shizi_count++;
              if(shizi_start_line==0)
              {
                  shizi_start_line=line;
              }
          }
          else if((Deal_flag[line]&DEAL_RIGHT_LOST)&&Bline_left[line]>Bline_left[line-1])
          {
              Deal_flag[line] |= INVALID_LINE;
              shizi_count++;
              if(shizi_start_line==0)
              {
                  shizi_start_line=line;
              }
          }
        }
      }
      else if((line_w<zhangai_max_len)&&(line>2)&&(Pick_flag[line]==0))
      {
          if(gl_zhangai_start==0)
          {
              gl_zhangai_start=line;
              gl_zhangai_end=line;
          }
          else
          {
              if(line-gl_zhangai_end<=2)
                gl_zhangai_end=line;
          }
          zhang_ai_num++;
      }
  }
  if((valid_line-START_Y>0)&&(valid_line>52))//分母不能为0（有可能卡死）
  {
      av_fact_w=total_line_w/(valid_line-START_Y);// 坡道 按照宽度检测
      if(av_fact_w>74)//坡道检测 宽度 76
      {
//           b_point_num_jin_loop=beleved_shizi_loop();
//           if(b_point_num_jin_loop>30)
//           {           
//               valid_line=20; 
//               LoopFlag=1;
//               return ;
//           }        
//           else
//           {
//               ramp_flag=1;
//           }            
      }
  }  
  if(zhang_ai_num>7)
  {
      gl_zhangai_flag=1;//障碍 检测
  }
  if(shizi_count>8)
  {
      Shi_zi_flag=shizi_start_line;
      valid_line=Shi_zi_flag-1;
  }
}
/*******************************************************************************
函数名称：xielv_lvbo
函数功能: 斜率法滤波 去除无效行
参数：无
*******************************************************************************/
void xielv_lvbo()
{
  uint8 line;
  for(line=START_Y+1;line<=valid_line;line++)
  {
	if((abs_sub(Bline_left[line],Bline_left[line-1])>JUDGE_DIFF)
	   ||(abs_sub(Bline_right[line],Bline_right[line-1])>JUDGE_DIFF))
	{
	  Deal_flag[line] |= INVALID_LINE;
	}
        if(my_debug_flag==1)
        {
            test_draw_point(line,Bline_left[line],0xFFE0);
            test_draw_point(line,Bline_right[line],0X001F);
        }
  }
}
/*******************************************************************************
函数名称：Is_out
函数功能: 判断该场图像是否有效
参数：
*******************************************************************************/
void Is_out()
{
  if(valid_line<6)
  {
	Out_flag=1;
  }
  else
  {
	Out_flag=0;
  }
  
  
}
float squre_error(uint16 arrary[],int average,int start,int valid_line)
{
	int line = 0;
	float total_error = 0;
	float error = 0;
	for (line = start; line < valid_line; line++)
	{
		//temp[line] = (float)(arrary[line] - average)*(arrary[line] - average);
		total_error = total_error +(float)(arrary[line] - average)*(arrary[line] - average);
		
	}
	error = total_error / valid_line;
	return error;
}
/*******************************************************************************
函数名称：If_LStraight
函数功能: 判断小车是否在长直道上方法
参数：无
*******************************************************************************/
 int temp[60]={0};
int zhidao_loop_flag=0;
void If_LStraight()
{

    
    gl_zhidao_count=If_straight(START_Y,valid_line,16);//返回偏离误差范围内的行数  15  
    if(gl_zhidao_count<40)//45
    {
        zhidao_count_flag=0;
        zhidao_loop_flag=0;
    }
    else if(gl_zhidao_count<=45)//50
    {
        zhidao_count_flag=1;
    }
    else
    {
        zhidao_count_flag=2;  
    }     
    last_zhidao_flag=zhidao_count_flag;
}
void shizi_find_end_line(uint8 *max_x,uint8 *max_y)
{
    uint8 start_line=Shi_zi_flag;
    uint8 index=10;//10
    *max_y=0;
    if(start_line<START_Y+2)
    {
        start_line=START_Y+2;
    }
    for(;index<IMG_W-10;index+=1)//3 1
    {
        for(start_line=Shi_zi_flag;start_line<IMG_H-1;start_line++)
        {
            if(get_img_point(start_line,index)||get_img_point(start_line-2,index))
            {
                break;
            }
        }
        if(start_line-1>(*max_y))
        {
            *max_y=start_line-1;
            *max_x=index;
        }
        
    }
}

int shizi_wait=0;
int shizi_num_right=0;
void shizi_find_line()
{

    uint8 sz_end_line,sz_end_x;
    uint8 line,lift_x,right_x,i;
    uint8 lost_line_count=0;
    shizi_find_end_line(&sz_end_x,&sz_end_line);
    lift_x=right_x=sz_end_x;
   
    for(line=sz_end_line;line>valid_line;line--)
    {
        Deal_flag[line]=0;
        //找左边线
        if(get_img_point(line,lift_x))
        {
            for(i=1;i<IMG_DIFF;i++)
            {
                if(lift_x==0||lift_x-i==0)
                {
                    lift_x=0;                    
                    Deal_flag[line]|=DEAL_LEFT_LOST;
                    break;
                }
                if(!get_img_point(line,lift_x-i))
                {
                    lift_x=lift_x-i+1;
                    break;
                }
            }
            if(i==IMG_DIFF)
            {
                lift_x=lift_x-i+1;
                Deal_flag[line]|=DEAL_LEFT_LOST;
                lost_line_count++;
            }
        }
        else
        {
            for(i=1;i<IMG_DIFF;i++)
            {
                if(lift_x+i>=IMG_W-1)
                {
                    lift_x=IMG_W-1;
                    Deal_flag[line]|=DEAL_LEFT_LOST;
                    break;
                }
                if(get_img_point(line,lift_x+i))
                {
                    lift_x=lift_x+i-1;
                    break;
                }
            }
            if(i==IMG_DIFF)
            {
                Deal_flag[line]|=DEAL_LEFT_LOST;
                lift_x=lift_x+i-1;
                lost_line_count++;
            }
        }
        if(lift_x==0||lift_x==IMG_H-1)
          lost_line_count++;
        Bline_left[line]=lift_x;        
        left_pos[line]=Bline_left[line];//保存左边的坐标
        //找右边线
        if(get_img_point(line,right_x))
        {
            for(i=1;i<IMG_DIFF;i++)
            {
                if(right_x+i>=IMG_W-1)
                {
                    right_x=IMG_W-1;
                    Deal_flag[line]|=DEAL_RIGHT_LOST;
                    break;
                }
                if(!get_img_point(line,right_x+i))
                {
                    right_x=right_x+i-1;
                    break;
                }
            }
            if(i==IMG_DIFF)
            {
                right_x=right_x+i-1;
                Deal_flag[line]|=DEAL_RIGHT_LOST;
                lost_line_count++;
            }
        }
        else
        {
            for(i=1;i<IMG_DIFF;i++)
            {
                if(right_x==0||right_x-i==0)
                {
                    right_x=0;
                    Deal_flag[line]|=DEAL_RIGHT_LOST;
                    break;
                }
                if(get_img_point(line,right_x-i))
                {
                    right_x=right_x-i+1;
                    break;
                }
            }
            if(i==IMG_DIFF)
            {
                right_x=right_x-i+1;
                Deal_flag[line]|=DEAL_RIGHT_LOST;
                lost_line_count++;
            }
        }
        if(right_x==0||right_x==IMG_H-1)
          lost_line_count++;
        Bline_right[line]=right_x;
        //right_pos[line]=right_x;
        right_pos[line]=Bline_right[line];//保存右边的坐标
        center_pos[line]=(left_pos[line]+right_pos[line])/2;//合成中心坐标
        
        if((Deal_flag[line]&DEAL_RIGHT_LOST)&&(Deal_flag[line]&DEAL_LEFT_LOST))
        {
            Deal_flag[line]|=INVALID_LINE;
        }
        else if(!(Deal_flag[line]&DEAL_RIGHT_LOST)&&!(Deal_flag[line]&DEAL_LEFT_LOST))
        {
            lost_line_count=0;
        }
        if(lost_line_count>=10)
        {
            line+=10;
            break;
        }
    }
    ti_jiaozheng(line,sz_end_line);//矫正图像的梯形失真
    for(i=valid_line;i<=line;i++)
      Deal_flag[i]|=INVALID_LINE;
    valid_line=sz_end_line;
}

//void ring_find_line()//对称补线
//{
//    int i=0,j=0;
//    int left_total_num=0;
//    int bian_lie=0;
//    int average_left=0;
//    int av_flag=0;
//    int differ=0;
//    int valin_lie=0;
//    for(i=3;i<40;i++)
//    {
//      if(abs(Bline_left[i]-Bline_left[i+1])>4)
//      {
//          bian_lie=i;//记录
//          average_left=left_total_num/(bian_lie-3);  
//          av_flag=1;
//          break;
//      }
//      else
//      {
//          left_total_num=left_total_num+Bline_left[i];//累计
//      }    
//    }
//    
//    if(av_flag==1)
//    {
//        for(j=bian_lie;;j++)
//        {
//            if(Bline_left[j]>Bline_left[j+1])
//            {
//                valin_lie=j;
//                break;
//            }      
//        }
//        for(j=3;j<valin_lie;j++)
//        {
//            if(Bline_left[j]>average_left)
//            {
//                differ=Bline_left[j]-average_left;
//                Bline_left[j]=average_left-differ-25;
//            }        
//        }
//        if(j<35)
//        {
//             for(;j<35;j++)
//            {
//                Bline_left[j]=Bline_left[j-1]-5;                    
//            }
//        }    
//    }
//}

/*******************************************************************************
函数名称：get_even_diff
函数功能: 获取有效行内中心线偏离平均值
参数：
*******************************************************************************/
/********************************************************************************************************/
uint8 get_max_distance()
{
  //速度100以下时动态前瞻 会很稳，但是最低速度超过100时（高速模式），
  //需要固定前瞻 ，因为频繁 切换前瞻  使小车不稳
    int speed_index=0;
    speed_index=Vspeed/10;
    if(speed_index>11)
    {
        speed_index=11;
    }
    else if(speed_index<6)
    {
        speed_index=6;    
    }
//    if(CD_speed>=100||zhidao_speed>110)//高速模式
//    {
//        return max_speed_distance_array[speed_index-6];
//    }
//    else//中低速
//    {
        return max_distance_array[speed_index-6];
//    }
    
}
/*******************************************************************************
函数名称：get_even_diff
函数功能: 获取有效行内中心线偏离平均值
参数：
*******************************************************************************/
void get_even_diff_s()//
{
  uint8 line;
  uint8 v_line1=0;//清零
  float diff1=0;
  float valid_line2=diff_valid_line;//记录用多少行计算误差，大小需要调试
  //diff_valid_line=45;//宏定义的用来计算误差
  uint8 center_val=98;//中心值是98，因为之前硬件上有偏差，所以-5抵消掉，根据实际情况测(右加左减)
  int16  w1,w2;//                99
  int valid_line_diff=30;//最远最近前瞻差值
  int mix_tance=MIX_distance;  //最近前瞻
  if(gl_zhangai_flag==1&&Shi_zi_flag==0&&gl_zhangai_start>3)//出现障碍时候进入判断，改变中心值来调整转向
  {
      w1=abs(Bline_left[gl_zhangai_start+4]-Bline_left[gl_zhangai_start-1]);
      w2=abs(Bline_right[gl_zhangai_start+4]-Bline_right[gl_zhangai_start-1]);
      if(w1>w2)//障碍在左边
      {
//          center_val+=15;
         center_val+=20;
          
      }
      else
      {  
//          center_val-=15;
        center_val-=20;
          
      }
    }
    else if(stop_car_line&&valid_line<15)//近处是停止线，会找不到边线
    {
        even_diff=0;
        return ;
    }

      if(valid_line<valid_line2)//不能超过有效的行数
      {
          valid_line2=valid_line;
      }
      if(valid_line2<START_Y+valid_line_diff)
      {
          mix_tance=START_Y;
      }
      even_diff=0;//清空误差

      if(valid_line2<=mix_tance)//有效行数小于最近计算行
      {
          mix_tance=START_Y;
      }
      
        //普通的赛道误差计算
        for(line=mix_tance;line<valid_line2;line++)//从第5行到到第valid_line2行参与计算，实际对应的是计算车前方的哪一段道路，要自己调试
        {
          if((Deal_flag[line]&INVALID_LINE)==0)//没有丢线就加和
          {               
              v_line1++;//纪录加了多少行
              diff1=diff1+( (int)(Pick_table[line]-center_val));//求和
          }
        }
   //   }

      even_diff=(int)((float)(diff1)/(float)(v_line1));//取平均值
    
   if(even_diff>=33)//向左
    {
    even_diff=33;
    }
    else if(even_diff<=-33)//向右
    {
    even_diff=-33;
    }

}
/********************************************************************************************************/





