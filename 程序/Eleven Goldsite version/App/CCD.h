#ifndef _CCD_H
#define _CCD_H

struct TSL1401
{
  uint8 left_Edge[50];
  uint8 right_Edge[50];
  uint8 mid_Line[50];
 /*
  等于11两边都找到，等于01只找到右边，等于10只找到左边,等于0都没有
  如果没有边线，沿用上一次标记值，如果找到边线，修改标记值
  再根据标记值确定寻找边线范围，
 */
  uint8 flag_edge[50];
  uint8 cross[50];
  uint8 obstacle;     
  uint8 max;
  uint8 min;
  uint8 aver;
  uint8 THRESHOLD;
  uint8 width;
  uint8 CCD_Offset;
  uint8 search;
  uint8 CROSS;
};

void CCD_init(void);

void Filter(uint8 *last, uint8 *news, uint16 len); //滤波

void maxvar(uint8 *buf,uint16 len,uint8  maxval,struct TSL1401 *ccd);  //限制最大值

void maxvar_a(uint8 *buf,uint16 len,uint8  maxval,struct TSL1401 *ccd);  //限制最大值

void abs_diff(int16 *dst,uint8 *src,uint16 len);   //求差分

void findMax(uint8 *buf,struct TSL1401 *ccd);     //寻找最大最小值

void searchEdge(int16 *bin,struct TSL1401 *ccd);  //寻找边沿

void Obstacle_search(int16 *bin,struct TSL1401 *ccd);

void CaculateMid_Line(struct TSL1401 *ccd);       //计算中线


#endif