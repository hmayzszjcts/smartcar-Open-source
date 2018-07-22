#include "common.h"
#include "include.h"

uint8 CCD_BUFF[TSL1401_MAX*3][TSL1401_SIZE];    //定义存储接收CCD图像的数组  
int16 CCD_Value[TSL1401_MAX][TSL1401_SIZE];     //存储差分值，用于巡线

struct TSL1401 ccd1;
extern uint8 ramp;//坡道值

/**************************

   ccd 参 数 初 始 化

**************************/
void CCD_init(void)
{
  ccd1.THRESHOLD = 17;   //判断跳变沿阈值，固定17
  ccd1.CCD_Offset = 10;  //改，暗电压值
  ccd1.CROSS = 70;       //改，平均值，判断十字  
}
/************************************************

        Filter 简 单 的 滤 波 程 序

           中 位 值 平 均 滤 波

*************************************************/
void Filter(uint8 *last, uint8 *news, uint16 len)
{
  uint8 i;
  
  for(i = 1; i < len - 1 ; i++)
  {
    if( (last[i]==last[i-1]) ||(last[i]==last[i+1]) ||(last[i+1]==last[i-1]))
      news[i] = (last[i-1] + last[i] + last[i+1]) / 3;
    else if(last[i] > last[i-1])
    {
      if(last[i] > last[i+1])
      {
        if(last[i-1] > last[i+1])
          news[i] = last[i-1];
        else news[i] = last[i+1];
      }
      else news[i] = last[i];
    }
    else
    {
      if(last[i] < last[i+1])
      {
        if(last[i-1] < last[i+1])
          news[i] = last[i-1];
        else news[i] = last[i+1];
      }
      else news[i] = last[i];
    }
  }   
}
/***************************************

             限 制 最 大 值 

****************************************/
void maxvar(uint8 *buf,uint16 len,uint8  maxval,struct TSL1401 *ccd)
{
  while(len--)
  {
    buf[len] = buf[len] - ccd->CCD_Offset;//减去暗电压值
    if(buf[len] > maxval)
    {
        buf[len]= maxval;
    }
  }
}
/***************************************

          限 制 最 大 值 自 适 应

****************************************/
void maxvar_a(uint8 *buf,uint16 len,uint8  maxval,struct TSL1401 *ccd)
{
  uint8 i=0;
  uint8 max=0;
 //在可用区间寻找可用最大值
 if(ccd->flag_edge[1] == 11 && ccd->width > 10)
  {
    for(i = 0;i < len;i++)
    {
      if(i > ccd->left_Edge[1] && i < ccd->right_Edge[1])
        if(max < buf[i])
          max = buf[i];
    }
  }
  else if(ccd->flag_edge[1] == 1)
  {
    for(i = 0;i < len;i++)
    {
      if(i < ccd->right_Edge[1])
        if(max < buf[i])
          max = buf[i];
    }
  }
  else if(ccd->flag_edge[1] == 10)
  {
     for(i = 0;i < len;i++)
    {
      if(i > ccd->left_Edge[1])
        if(max < buf[i])
          max = buf[i];     
    }
  }
  else
  {
    for(i = 0;i < len;i++)
    {
      if(max < buf[i])
        max = buf[i];
    }
  }
  //判断最大值是否溢出，是否大于127
  if(max > maxval) //如果溢出，适应到127之内
  {
    int ratio;
    int temp;
    if(max > ccd->CCD_Offset)
      ratio = 12700 / (max - ccd->CCD_Offset);
    else
      ratio = 0;
    
    for(i = 0;i < len;i++)
    {
      if(buf[i] > ccd->CCD_Offset)
        temp = (buf[i] - ccd->CCD_Offset);
      else
        temp = 0;
      buf[i] = (uint8)(temp * ratio / 100);
      if(buf[i] > maxval)
        buf[i] = maxval;
    }
  }
  else  //如果没有溢出，正常减去暗电压值
  {
    while(len--)
    {
      buf[len] = buf[len] - ccd->CCD_Offset;//减去暗电压值
      if(buf[len] > maxval)
      {
          buf[len]= maxval;
      }
    }
  }
  
}
/***********************************************

     *  @brief   计 算 差 分 绝 对 值

 ***********************************************/
void abs_diff(int16 *dst,uint8 *src,uint16 len)
{
    int8 tmp;
    uint16 lentmp = len;

    while((--lentmp) > 3)       //仅循环 len-3 次
    {
        tmp = *(src+3)- *src;   //前一点减去后一点的差
        *dst = tmp;             //把差值赋值给buff【2和3】
        src++;
        dst++;
    }
    // 最后3点配置为 0             
    *(dst++) = 0;*(dst++) = 0;*dst = 0;
}
/******************************************

        查 找 采 集 回 电 压 值

    最 大 值，最 小 值，以 及 平 均 值

********************************************/
void findMax(uint8 *buf,struct TSL1401 *ccd)
{
  uint8 i=0;
  uint8 max,min,aver;
  uint32 sum;
  max = 0;
  min = 100;
  sum = 0;
  for(i = 10;i < 119;i++)
  {
    if(buf[i]>max)
      max = buf[i];
    if(buf[i]<min)
      min = buf[i];
    sum = sum + buf[i];
  }
  aver = (uint8)(sum / 108);
  ccd->aver = aver;
  ccd->max  = max;
  ccd->min  = min;
}
/**********************************************

          边 沿 查 找，跟 踪 法

***********************************************/
void searchEdge(int16 *bin,struct TSL1401 *ccd)
{
  uint8 i;
  ccd->cross[0] = 0;
  //10次从中间向两边寻找边线，保证后面的正确性
  if(ccd->search<10)
  {
    for(i=70;i>=10;i--)
    {
      if(bin[i] > ccd->THRESHOLD)
      {
        ccd->left_Edge[0] = i;
        break;
      }
      else if(i == 10)
      {
        ccd->left_Edge[0] = 0;
      }
    }
    for(i=50;i<=118;i++)
    {
      if(bin[i] < -ccd->THRESHOLD)
      {
        ccd->right_Edge[0] = i;
        break; 
      }
      else if(i == 118)
      {
        ccd->right_Edge[0] = 0;
      }
    }
    ccd->search++;
  }
  else//正常搜线，如果没有其中一边，其中一边的值置零
  {
   //如果上次中线在一定范围内，从上一次中线向左寻找左边，向右寻找右边
    if(ccd->mid_Line[1]> 5 && ccd->mid_Line[1] < 122)
    {
      for(i = ccd->mid_Line[1] + 0;i >= 5;i--)
      {
        if(bin[i] > ccd->THRESHOLD 
           && CCD_BUFF[1*TSL1401_MAX+0][i-4]<CCD_BUFF[1*TSL1401_MAX+0][i+9] 
           && CCD_BUFF[1*TSL1401_MAX+0][i-5]<CCD_BUFF[1*TSL1401_MAX+0][i+10])
        {
          ccd->left_Edge[0] = i;
          break;
        }
        else if(i == 5)
        {
          ccd->left_Edge[0] = 0;
        }
      }
      for(i = ccd->mid_Line[1] - 0;i<=122;i++)
      {
         if(bin[i] < -ccd->THRESHOLD 
            && CCD_BUFF[1*TSL1401_MAX+0][i+4]<CCD_BUFF[1*TSL1401_MAX+0][i-9] 
            && CCD_BUFF[1*TSL1401_MAX+0][i+5]<CCD_BUFF[1*TSL1401_MAX+0][i-10])
        {
          ccd->right_Edge[0] = i;
          break; 
        }
        else if(i == 122)
        {
          ccd->right_Edge[0] = 0;
        }
      }
    }
    //上一次中线在最左边，从最左边向右一定范围寻找右边线，在一小范围寻找左边线
    //且不会有左边，所以不找左边
    else if(ccd->mid_Line[1] <= 5)
    {
      ccd->left_Edge[0] = 0;
      if(ccd->right_Edge[1]==0)
      {
        for(i = 5;i<=(ccd->width/2+ccd->width/5);i++)//55
        {
          if(bin[i] < -ccd->THRESHOLD 
              && CCD_BUFF[1*TSL1401_MAX+0][i+9]<CCD_BUFF[1*TSL1401_MAX+0][i-4] 
              && CCD_BUFF[1*TSL1401_MAX+0][i+10]<CCD_BUFF[1*TSL1401_MAX+0][i-5])
          {
            ccd->right_Edge[0] = i;
            break; 
          }
          else if(i == (ccd->width/2+ccd->width/5))
          {
            ccd->right_Edge[0] = 0;
          }
        }
      }
      else
      {
        for(i = 5;i<=(ccd->right_Edge[1]+ccd->width/2+ccd->width/8);i++)
        {
          if(bin[i] < -ccd->THRESHOLD 
              && CCD_BUFF[1*TSL1401_MAX+0][i+9]<CCD_BUFF[1*TSL1401_MAX+0][i-4] 
              && CCD_BUFF[1*TSL1401_MAX+0][i+10]<CCD_BUFF[1*TSL1401_MAX+0][i-5])
          {
            ccd->right_Edge[0] = i;
            break; 
          }
          else if(i == (ccd->right_Edge[1]+ccd->width/2+ccd->width/8))
          {
            ccd->right_Edge[0] = 0;
          }
        }
      }
    }
    //上一次中线在最右边，从最右边一定内寻找左边线，在小范围内寻找右边线
    //且不会有右边，不找右边
    else if(ccd->mid_Line[1] >= 122)
    {
      if(ccd->left_Edge[1]==0)
      {
        for(i = 122;i >=(122-ccd->width/2-ccd->width/5);i--)//71
        {
          if(bin[i] > ccd->THRESHOLD 
             && CCD_BUFF[1*TSL1401_MAX+0][i-9]<CCD_BUFF[1*TSL1401_MAX+0][i+4] 
             && CCD_BUFF[1*TSL1401_MAX+0][i-10]<CCD_BUFF[1*TSL1401_MAX+0][i+5])
          {
            ccd->left_Edge[0] = i;
            break;
          }
          else if(i == (122-ccd->width/2-ccd->width/5))
          {
            ccd->left_Edge[0] = 0;
          }
        }
      }
      else
      {
        for(i = 122;i >= (ccd->left_Edge[1]-ccd->width/2-ccd->width/8);i--)
        {
          if(bin[i] > ccd->THRESHOLD 
             && CCD_BUFF[1*TSL1401_MAX+0][i-9]<CCD_BUFF[1*TSL1401_MAX+0][i+4] 
             && CCD_BUFF[1*TSL1401_MAX+0][i-10]<CCD_BUFF[1*TSL1401_MAX+0][i+5])
          {
            ccd->left_Edge[0] = i;
            break;
          }
          else if(i == (ccd->left_Edge[1]-ccd->width/2-ccd->width/8))
          {
            ccd->left_Edge[0] = 0;
          }
        }
      }
      ccd->right_Edge[0] = 0;
    }
  }
  //设置边线标记位
  if(ccd->left_Edge[0]!=0&&ccd->right_Edge[0]!=0)//都找到
  {
    ccd->flag_edge[0] = 11;
  } 
  else if(ccd->left_Edge[0]==0&&ccd->right_Edge[0]!=0)//只有右边
  {
    ccd->flag_edge[0] = 01;
  }
  else if(ccd->left_Edge[0]!=0&&ccd->right_Edge[0]==0)//只有左边
  {
    ccd->flag_edge[0] = 10;
  }
  else  
  {
    ccd->flag_edge[0] = 0;
    //如果没有边线，并且平均值大于一定值，十字标志为1，否则十字标志为0
    ccd->cross[0] = 0;
    if(ccd->aver > ccd->CROSS /*&& ramp==0*/)//这个值需要调节//加上坡道条件，防止坡道看天
    {
      ccd->cross[0] = 1;
    }
  }
}
/*****************************************

  根 据 查 找 到 的 边 沿，计 算 中 线 值

******************************************/
void CaculateMid_Line(struct TSL1401 *ccd)                
{
  uint8 i;
  
  if(ccd->flag_edge[0] == 11)//都找的到
  {
    //赛道宽度大于一定为正确，否则错误
    if( ccd->right_Edge[0]-ccd->left_Edge[0] > 25)
    {
      ccd->mid_Line[0] = (uint8)( (ccd->left_Edge[0]+ccd->right_Edge[0])/2 );
      ccd->width = ccd->right_Edge[0] - ccd->left_Edge[0];//计算赛道宽度，用于补线
    }
    else
    {
      ccd->flag_edge[0] = 0;
      ccd->mid_Line[0] = ccd->mid_Line[1];
    }
  }
  else if(ccd->flag_edge[0] == 10)//只找到左边
  {
    if((ccd->left_Edge[0] + ccd->width/2) < 122)
      ccd->mid_Line[0] = (uint8)(ccd->left_Edge[0] + ccd->width/2);
    else 
      ccd->mid_Line[0] = 122;
  }
  else if(ccd->flag_edge[0] == 01)//只找到右边
  {
    if( (ccd->right_Edge[0]-ccd->width/2) > 5)
      ccd->mid_Line[0] = (uint8)(ccd->right_Edge[0] - ccd->width/2);
    else
      ccd->mid_Line[0] = 5;
  }
  else if(ccd->cross[0] == 1)//十字
  {
    if(ccd->mid_Line[1]>61&&ccd->mid_Line[1]<65)
    {
      ccd->mid_Line[0] = ccd->mid_Line[1];
    }
    else
    {
      ccd->mid_Line[0] = 63;
    }
  }
  else 
  {
    if(ccd->flag_edge[2] == 1 && ccd->flag_edge[1] == 1 && ccd->flag_edge[2] == 1)
    {
      ccd->mid_Line[0] = 5;
    }
    else if(ccd->flag_edge[2] == 10 && ccd->flag_edge[1] == 10 && ccd->flag_edge[2] == 10)
    {
      ccd->mid_Line[0] = 122;
    }
    else
    {
      ccd->mid_Line[0] = ccd->mid_Line[1];
    }
  }

  if(ccd->search == 10 && ccd->cross[0] == 0)
  {
    //如果本次中线和上次中线误差太大，为错误，舍弃
    if( ccd->mid_Line[0]-ccd->mid_Line[1] > 50)
    {
      ccd->flag_edge[0] = 0;
      ccd->mid_Line[0] = ccd->mid_Line[1];
    }
    if( ccd->mid_Line[0]-ccd->mid_Line[1] < -50)
    {
      ccd->flag_edge[0] = 0;
      ccd->mid_Line[0] = ccd->mid_Line[1];
    }
  }
 
  //存储前50次位置信息
  for(i=49;i>0;i--)
  {
    ccd->flag_edge[i] = ccd->flag_edge[i-1];
    ccd->cross[i] = ccd->cross[i-1];
    
    ccd->left_Edge[i] = ccd->left_Edge[i-1];
    ccd->right_Edge[i] = ccd->right_Edge[i-1];
    ccd->mid_Line[i] = ccd->mid_Line[i-1];
  }
}
/*******************************************

  障碍——计算障碍位置，并给出当前中线位置

********************************************/
void Obstacle_search(int16 *bin,struct TSL1401 *ccd)
{
  uint8 i;
  
  uint8 ob_r,ob_l,ob_flag;
  ob_r = ob_l = ob_flag = 0;
  
  if( ccd->width > 25 && ccd->width < 40 && ccd->flag_edge[0] == 11)
  {
    //边线左右在一定范围内，在左右边均寻找障碍
    if(ccd1.left_Edge[0] > 25 && ccd1.right_Edge[0] < 102)
    {
      //如果左边线到左边界宽度大于右边线到右边界宽度，则在左边寻找障碍
      if(ccd1.left_Edge[0] >= (127-ccd1.right_Edge[0]) )
      {
        for(i = ccd1.left_Edge[0] - 5;i >= 10;i--)//从左线到左边界往右一定处寻找障碍右边
        {
          if(bin[i] < -ccd->THRESHOLD)
          {
            ob_r = i;
            break;
          }
        }
        
        if(ob_r == 10)//如果没找到，说明左侧无障碍
        {
          ob_flag = 0;
        }
        else  //如果找到，再往左一点寻找障碍左边
        {
          
          for(i = ob_r - 1;i >= 3;i--)
          {
            if(bin[i] > ccd->THRESHOLD)
            {
              ob_l = i;
              break;
            }
          }
          if(ob_l == 3)
          {
            ob_flag = 0;
          }
          else
          {
            if( (ccd1.left_Edge[0] - ob_l) > 12 && (ccd1.left_Edge[0] - ob_l) < 30
               && (ccd1.left_Edge[0]-ob_r) > (ob_r - ob_l+3) )//再次判断，障碍left到赛道left距离是否符合
            {
              ob_flag = 10;
            }
            else
            {
              ob_flag = 0;
            }
          }
        }
        
        if(ob_flag == 0)//如果左侧没有找到障碍，继续在右侧寻找
        {
          ob_r = ob_l = ob_flag = 0;//标志清零
          
          for( i = ccd->right_Edge[0] + 5 ;i <= 118;i++)
          {
            if(bin[i] > ccd->THRESHOLD)
            {
              ob_l = i;
              break;
            }
          }
          
          if(ob_l == 118)
          {
            ob_flag = 0;
          }
          else
          {
            for( i = ob_l + 1 ;i <= 124;i++) 
            {
              if(bin[i] < -ccd->THRESHOLD)
              {
                ob_r = i;
                break;
              }
            }
            
            if(ob_l == 124)
            {
              ob_flag = 0;
            }
            else
            {
              if( (ob_r - ccd1.right_Edge[0]) > 12 &&  (ob_r - ccd1.right_Edge[0]) < 30
                 && (ob_l - ccd1.right_Edge[0]) > (ob_r - ob_l + 3) )//再次判断，障碍right到赛道right距离是否符合
              {
                ob_flag = 01;
              }
              else
              {
                ob_flag = 0;
              }
            }
          }
        }
        else//左侧找到不做处理
        {
          ;
        }
        
      }
      //如果左边线到左边界宽度小于右边线到右边界宽度，则在右边寻找障碍
      else 
      {
        for( i = ccd->right_Edge[0] + 5 ;i <= 118;i++)
        {
          if(bin[i] > ccd->THRESHOLD)
          {
            ob_l = i;
            break;
          }
        }
        
        if(ob_l == 118)
        {
          ob_flag = 0;
        }
        else
        {
          for( i = ob_l + 1 ;i <= 124;i++) 
          {
            if(bin[i] < -ccd->THRESHOLD)
            {
              ob_r = i;
              break;
            }
          }
          
          if(ob_l == 124)
          {
            ob_flag = 0;
          }
          else
          {
            if( (ob_r - ccd1.right_Edge[0]) > 12 &&  (ob_r - ccd1.right_Edge[0]) < 30
                 && (ob_l - ccd1.right_Edge[0]) > (ob_r - ob_l + 3) )//再次判断，障碍right到赛道right距离是否符合
            {
              ob_flag = 01;
            }
            else
            {
              ob_flag = 0;
            }
          }
          
        }
        
        if(ob_flag == 0)//如果右侧没有找到障碍，继续在左侧寻找
        {
          ob_r = ob_l = ob_flag = 0;//标志清零
         
          for(i = ccd1.left_Edge[0] - 5;i >= 10;i--)//从左线到左边界往右一定处寻找障碍右边
          {
            if(bin[i] < -ccd->THRESHOLD)
            {
              ob_r = i;
              break;
            }
          }
          
          if(ob_r == 10)//如果没找到，说明左侧无障碍
          {
            ob_flag = 0;
          }
          else  //如果找到，再往左一点寻找障碍左边
          {
            
            for(i = ob_r - 1;i >= 3;i--)
            {
              if(bin[i] > ccd->THRESHOLD)
              {
                ob_l = i;
                break;
              }
            }
            
            if(ob_l == 3)
            {
              ob_flag = 0;
            }
            else
            {
              if( (ccd1.left_Edge[0] - ob_l) > 12 && (ccd1.left_Edge[0] - ob_l) < 30
                 && (ccd1.left_Edge[0]-ob_r) > (ob_r - ob_l+3) )//再次判断，障碍left到赛道left距离是否符合
              {
                ob_flag = 10;
              }
              else
              {
                ob_flag = 0;
              }
            }
            
          }
          
        }
        else//右侧找到不做处理
        {
          ;
        }

      }
      
    }
    //左边线小于30，障碍只能在右侧
    else if(ccd1.left_Edge[0] <= 25 && ccd1.right_Edge[0] < 102)
    {
      ob_r = ob_l = ob_flag = 0;//标志清零
        
      for( i = ccd->right_Edge[0] + 5 ;i <= 118;i++)
      {
        if(bin[i] > ccd->THRESHOLD)
        {
          ob_l = i;
          break;
        }
      }
      
      if(ob_l == 118)
      {
        ob_flag = 0;
      }
      else
      {
        for( i = ob_l + 1 ;i <= 124;i++) 
        {
          if(bin[i] < -ccd->THRESHOLD)
          {
            ob_r = i;
            break;
          }
        }
        
        if(ob_l == 124)
        {
          ob_flag = 0;
        }
        else
        {
          if( (ob_r - ccd1.right_Edge[0]) > 12 &&  (ob_r - ccd1.right_Edge[0]) < 30
                 && (ob_l - ccd1.right_Edge[0]) > (ob_r - ob_l + 3) )//再次判断，障碍right到赛道right距离是否符合 
          {
            ob_flag = 01;
          }
          else
          {
            ob_flag = 0;
          }
        }
        
      }
    }
    //右边线大于100，障碍只能在左侧
    else if(ccd1.left_Edge[0] > 25 && ccd1.right_Edge[0] >= 102)
    {
      for(i = ccd1.left_Edge[0] - 5;i >= 10;i--)//从左线到左边界往右一定处寻找障碍右边
      {
        if(bin[i] < -ccd->THRESHOLD)
        {
          ob_r = i;
          break;
        }
      }
      
      if(ob_r == 10)//如果没找到，说明左侧无障碍
      {
        ob_flag = 0;
      }
      else  //如果找到，再往左一点寻找障碍左边
      {
        
        for(i = ob_r - 1;i >= 3;i--)
        {
          if(bin[i] > ccd->THRESHOLD)
          {
            ob_l = i;
            break;
          }
        }
        if(ob_l == 3)
        {
          ob_flag = 0;
        }
        else
        {
          if( (ccd1.left_Edge[0] - ob_l) > 12 && (ccd1.left_Edge[0] - ob_l) < 30
             && (ccd1.left_Edge[0]-ob_r) > (ob_r - ob_l+3) )//再次判断，障碍left到赛道left距离是否符合
          {
            ob_flag = 10;
          }
          else
          {
            ob_flag = 0;
          }
        }
      }
    }
    
    if(ob_flag == 10)
    {
      ccd->obstacle = 10;
    }
    else if(ob_flag == 01)
    {
      ccd->obstacle = 01;
    }
    else
    {
      ccd->obstacle = 0;
    }
  }
  else
  {
    ccd1.obstacle = 0;
  }
}
