#include "include.h"
uint16 CCD_Buff[128]={0};
uint8  CCD_Offset;           //暗电压
uint16 Max_Value=0,Brightness;

void  delay200ns()
{
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");//共25个 一个asm("nop")是8ns;
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
}
void CCD_Exposure()    //曝光 下一次图像捕获时刻与该函数执行的时刻与差值为曝光时间
{
    CCD_SI=1;        
    delay200ns();
    CCD_CLK=1; 
    delay200ns();
    CCD_SI=0;    
    delay200ns();
    CCD_CLK=0;    
    for(uint8 i=0; i<129; i++)
    {
        delay200ns();
        CCD_CLK=1; 
        delay200ns();
        CCD_CLK=0; 
    } 
}
void CCD_Capture()
{
  uint8  i=0; 
  CCD_SI=1;
  delay200ns();
  CCD_CLK=1;  
  delay200ns();
  CCD_SI=0;
  delay200ns();
  //采集第一个点
  CCD_Buff[0]=adc_once(ADC0_SE8,ADC_12bit);
  Max_Value=CCD_Buff[0];
  CCD_CLK=0;
  //采集第2~128个点
  for(i=1; i<128; i++)
  {
          delay200ns();
          CCD_CLK=1; 
          delay200ns();
          CCD_Buff[i]=adc_once(ADC0_SE8,ADC_12bit);
  
        if((Right-Left>20)&&Leftlastfind&&Rightlastfind)             //上次左右边线都找了那么应该在左右5个点搜索最高亮度，把其他亮度抑制掉
       {
         if((i>Left)&&(i<Right))      if(Max_Value<CCD_Buff[i]) Max_Value=CCD_Buff[i]; 
       } 
       else
       {
          if(Max_Value<CCD_Buff[i])  Max_Value=CCD_Buff[i];
       }
      CCD_CLK=0;     
   }
  //发送第129个clk作为结束
  delay200ns();
  CCD_CLK = 1; 
  delay200ns();
  CCD_CLK = 0;
}
 //做归一化处理(将采样值限制在0-127)
void CCD_Normalization()
{
  uint8 i=0;
  float ratio;
  int temp;
  if(Max_Value>CCD_Offset)
  ratio=127.0/(Max_Value-CCD_Offset);
  else  ratio=0;
  for(i=0; i<128; i++)
  {
    if(CCD_Buff[i]>CCD_Offset)
    temp=CCD_Buff[i]-CCD_Offset;
    else temp=0;
    CCD_Buff[i]=(int)(temp*ratio);
    if(CCD_Buff[i]>126) CCD_Buff[i]=126;
    CCD_Draw_Buff[i]=CCD_Buff[i];
  }
  CCD_Draw_Buff[130]=BYTE0(Max_Value);
  CCD_Draw_Buff[131]=BYTE1(Max_Value); 
}