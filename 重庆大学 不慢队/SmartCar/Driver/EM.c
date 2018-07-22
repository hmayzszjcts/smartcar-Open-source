#include "include.h"

uint16 Max_Value;
float left_adc,right_adc;
static float Middle_Err_Last;
uint16 Inductor_Adc[16];
float ADC_Filter(float middle_err);
void  delay()
{
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
}

static float pre_left_adc[10]={0};
static float pre_right_adc[10]={0};
static float left_adc_max=0;
static float right_adc_max=0;
void EM_ADC()
{
  uint8 i, j;
  long  ADC_Sum[2];
  ADC_Sum[0]=0;
  ADC_Sum[1]=0;
  //采集50次
  #define FILTER_N 10
  float filter_temp, filter_sum_L = 0, filter_sum_R = 0;
  float filter_buf_L[FILTER_N];
  float filter_buf_R[FILTER_N];
  for(i = 0; i < FILTER_N; i++) {
    delay();
    filter_buf_L[i] = adc_once(ADC1_SE12,ADC_12bit);
    delay();
    filter_buf_R[i] = adc_once(ADC1_SE15,ADC_12bit);
  }
  
  // 采样值从小到大排列（冒泡法）
  for(j = 0; j < FILTER_N - 1; j++) {
    for(i = 0; i < FILTER_N - 1 - j; i++) {
      if(filter_buf_L[i] > filter_buf_L[i + 1]) {
        filter_temp = filter_buf_L[i];
        filter_buf_L[i] = filter_buf_L[i + 1];
        filter_buf_L[i + 1] = filter_temp;
      }
    }
  }
  
  for(j = 0; j < FILTER_N - 1; j++) {
    for(i = 0; i < FILTER_N - 1 - j; i++) {
      if(filter_buf_R[i] > filter_buf_R[i + 1]) {
        filter_temp = filter_buf_R[i];
        filter_buf_R[i] = filter_buf_R[i + 1];
        filter_buf_R[i + 1] = filter_temp;
      }
    }
  }
  
  // 去除最大最小极值后求平均
  for(i = 1; i < FILTER_N - 1; i++) 
  {
    filter_sum_L += filter_buf_L[i];
    filter_sum_R += filter_buf_R[i];
  }
  left_adc = filter_sum_L / (FILTER_N - 2);
  right_adc =filter_sum_R/ (FILTER_N - 2);

  /*
  for(i=0; i<100; i++)
  {
     delay();
     ADC_Sum[0]+=adc_once(ADC1_SE12,ADC_12bit);
     delay();
     ADC_Sum[1]+=adc_once(ADC1_SE15,ADC_12bit);
  } 
 
  left_adc  = ADC_Sum[0]/5.0;
  right_adc = ADC_Sum[1]/5.0;
  */
   //left_adc  = (left_adc-4000.0)/(14000.0-4000.0);
   //right_adc = (right_adc-3600.0)/(23000.0-3600.0);
   for(i=0; i<9; i++)
  {
      pre_left_adc[i]=pre_left_adc[i+1];
      pre_right_adc[i]=pre_right_adc[i+1];
  }
  pre_left_adc[9]=left_adc;
  pre_right_adc[9]=right_adc;
  
  Inductor_Adc[0]=left_adc;
  Inductor_Adc[1]=right_adc;
  Middle_Err=50*(left_adc-right_adc)/(left_adc+right_adc);
  
  //Middle_Err=50*(left_adc-right_adc);
  /*
  Middle_Err_Last=Middle_Err;
  float slope_L,slope_R;
  if(left_adc/10000.0<0.4 || right_adc/10000.0<0.4) //采样值过小
  {
     slope_L=Slope_Calculate(0,9,pre_left_adc);//计算左边斜率
     slope_R=Slope_Calculate(0,9,pre_right_adc);//计算右边斜率
     if(slope_L<-10&&slope_R<-10)//左边在减小而右边也在减小，说明拐出线了,此时中心偏差不可采信,沿用上一个
     {
       Middle_Err=Middle_Err_Last;
     }
  }
  
  if(left_adc/10000.0<0.4 && right_adc/10000.0<0.4) //丢线
  {
   if(Stop==false&&RunTime>2)               //记录当因为超出安全倾斜范围时车子停下来这一一状态
   {
    Stop=true;
    CarStopedJustNow=true;
   }
  }
  */
   Middle_Err=ADC_Filter(Middle_Err);
}

float ADC_Filter(float middle_err)
{
  #define FILTER_N 10
  float Middle_Err_Fltered; 
  static float Pre3_Error[10]; 
  Pre3_Error[9]=Pre3_Error[8];
  Pre3_Error[8]=Pre3_Error[7];
  Pre3_Error[7]=Pre3_Error[6];
  Pre3_Error[6]=Pre3_Error[5];
  Pre3_Error[5]=Pre3_Error[4];
  Pre3_Error[4]=Pre3_Error[3];
  Pre3_Error[3]=Pre3_Error[2];
  Pre3_Error[2]=Pre3_Error[1];
  Pre3_Error[1]=Pre3_Error[0];
  Pre3_Error[0]=middle_err;
  int i, j;
  float filter_temp, filter_sum = 0;
  float filter_buf[FILTER_N];
  for(i = 0; i < FILTER_N; i++) {
    filter_buf[i] = Pre3_Error[i];
  }
  
  // 采样值从小到大排列（冒泡法）
  for(j = 0; j < FILTER_N - 1; j++) {
    for(i = 0; i < FILTER_N - 1 - j; i++) {
      if(filter_buf[i] > filter_buf[i + 1]) {
        filter_temp = filter_buf[i];
        filter_buf[i] = filter_buf[i + 1];
        filter_buf[i + 1] = filter_temp;
      }
    }
  }
  
  // 去除最大最小极值后求平均
  for(i = 1; i < FILTER_N - 1; i++) filter_sum += filter_buf[i];
  return filter_sum / (FILTER_N - 2);
}

float  Middle_Err_Filter(float middle_err)    //中心偏差滤波      
{
  float Middle_Err_Fltered; 
  static float Pre3_Error[8]; 
  Pre3_Error[7]=Pre3_Error[6];
  Pre3_Error[6]=Pre3_Error[5];
  Pre3_Error[5]=Pre3_Error[4];
  Pre3_Error[4]=Pre3_Error[3];
  Pre3_Error[3]=Pre3_Error[2];
  Pre3_Error[2]=Pre3_Error[1];
  Pre3_Error[1]=Pre3_Error[0];
  Pre3_Error[0]=middle_err;
  Middle_Err_Fltered=Pre3_Error[0]*0.3+Pre3_Error[1]*0.25+Pre3_Error[2]*0.15+Pre3_Error[3]*0.1+Pre3_Error[4]*0.08+Pre3_Error[5]*0.05+Pre3_Error[6]*0.04+Pre3_Error[7]*0.03;
  return Middle_Err_Fltered;
}
