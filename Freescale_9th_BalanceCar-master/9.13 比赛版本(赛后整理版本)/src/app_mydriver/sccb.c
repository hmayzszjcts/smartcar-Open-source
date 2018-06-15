#include "include.h"
/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：sccb.c
 * 描述         ：数字摄像头OV7620 sccb配置
 * 实验平台     ：凌立印象开发板
 * 库版本       ：基于野火库
 * 嵌入系统     ：
 * 作者         ：xuxu
**********************************************************************************/






/*************************************************************************
*                             岱默科技DEMOK Kinetis开发小组
*
*  函数名称：sccb_init
*  功能说明：初始化SCCB  其中SCL接PTD10 SDA接PTD11
*  参数说明：无
*  函数返回：无
*
*************************************************************************/
//-------初始化PE11  PE12为GPIO--------//
void sccb_init(void)
{
  gpio_init(PORTE,11, GPO, HIGH); 
  gpio_init(PORTE,12, GPO, HIGH); 
}

/************************************************************************
*                             岱默科技DEMOK Kinetis开发小组
*
*  函数名称：sccb_wait
*  功能说明：SCCB延时，不应太小
*  参数说明：无
*  函数返回：无
*
*************************************************************************/
void sccb_wait(void)
{
  unsigned int i;
  for( i=0; i<2000; i++)
  {
    asm ("nop");
  }
}
/************************************************************************
*                             岱默科技DEMOK Kinetis开发小组
*
*  函数名称：sccb_start
*  功能说明：SCCB启动位
*  参数说明：无
*  函数返回：无
*
*************************************************************************/
void sccb_start(void)
{
  SCL_OUT;
  SDA_OUT;
 
  SDA_HIGH;
  //sccb_wait();
  SCL_HIGH;
  sccb_wait();
  SDA_LOW;
  sccb_wait();
  SCL_LOW;
}

/************************************************************************
*                             岱默科技DEMOK Kinetis开发小组
*
*  函数名称：sccb_stop
*  功能说明：SCCB停止位
*  参数说明：无
*  函数返回：无
*
*************************************************************************/
void sccb_stop(void)
{
  SCL_OUT;
  SDA_OUT;
  
  SDA_LOW;
  sccb_wait();
  SCL_HIGH;
  sccb_wait();
  SDA_HIGH;
  sccb_wait();
}

/************************************************************************
*                             岱默科技DEMOK Kinetis开发小组
*
*  函数名称：sccb_sendByte
*  功能说明：在SCCB总线上发送一个字节
*  参数说明：data 要发送的字节内容
*  函数返回：无
*
*************************************************************************/
unsigned char sccb_sendByte(unsigned char data)
{
  unsigned char i;
  v ack;
  SDA_OUT;
  for( i=0; i<8; i++)
  {
    if(data & 0x80)
      SDA_HIGH;
    else 
      SDA_LOW;
    data <<= 1;
    sccb_wait();
    SCL_HIGH;
    sccb_wait();
    SCL_LOW;
    sccb_wait();
  }
  SDA_HIGH;
  SDA_IN;
  sccb_wait();
  SCL_HIGH;
  sccb_wait();
  ack = SDA_DATA;
  SCL_LOW;
  sccb_wait();
  return ack;
}


/************************************************************************
*                             岱默科技DEMOK Kinetis开发小组
*
*  函数名称：sccb_regWrite
*  功能说明：通过SCCB总线向指定设备的指定地址发送指定内容
*  参数说明：device---设备号  读写有区别
*            address---写数据的寄存器
*            data---写的内容
*  函数返回：ack=1未收到应答(失败)    ack=0收到应答(成功)
*
*************************************************************************/
unsigned char sccb_regWrite(unsigned char device,unsigned char address,unsigned char data)
{
  unsigned char i;
  unsigned char ack;
  for( i=0; i<20; i++)
  {
    sccb_start();
    ack = sccb_sendByte(device);
    if( ack == 1 )
    {
      sccb_stop();
      continue;
    }
    
    ack = sccb_sendByte(address);
    if( ack == 1 )
    {
      sccb_stop();
      continue;
    }
    
    ack = sccb_sendByte(data);
    if( ack == 1 )
    {
      sccb_stop();
      continue;
    }
    
    sccb_stop();
    if( ack == 0 ) break;
  }
  return ack;
}


/************************************************************************
*                             岱默科技DEMOK Kinetis开发小组
*
*  函数名称：sccb_refresh
*  功能说明：sccb寄存器刷新
*  参数说明：无
*  函数返回：ACK=1未收到应答(失败)    ACK=0收到应答(成功)
*
*************************************************************************/
unsigned char sccb_refresh()
{
    //-----------SCCB恢复默认出厂设置----------//
    //--PCLK:73ns   HREF:63.6us   VSYN:16.64ms--//
    //--默认隔行扫描，全分辨率为640*480，采集VSYN分辨率是640*240  
    //--在每两个HREF之间有640个PCLK
    //--在每两个VSYN之间有240个HREF    
    //sccb_regWrite(0x42,0x11,0x00);   
    //sccb_regWrite(0x42,0x14,0x04);
    //sccb_regWrite(0x42,0x28,0x20);
    //---------------------------------------//
    unsigned char ack1,ack2,ack3;
    unsigned char ACK = 1;
    for(u8 sccb_time=0; sccb_time<20; sccb_time++)
    {
     
      ack1 = sccb_regWrite(0x42,0x11,0x03);    //地址0X11-中断四分频(640*240)           PCLK:292ns   HREF:254.4us   VSYN:66.7ms
      ack2 = sccb_regWrite(0x42,0x14,0x24);    //地址0X14-QVGA(320*120)                 PCLK:584ns   HREF:508.8us   VSYN:66.7ms
      ack3 = sccb_regWrite(0x42,0x28,0x20);    //地址0X28-连续采集模式(320*240)         PCLK:584ns   HREF:254.4us   VSYN:66.7ms
      
      if( (ack1 == 0) && (ack2 == 0) && (ack3 == 0)) 
      {
        gpio_set (PORTE, 0, 0);  //写成功  灯亮 
        sccb_wait();
        ACK = 0;
        break;
      }
      
      else
      {
        gpio_set (PORTE, 0, 1);  //写失败  灯灭
        sccb_wait();
        ACK = 1;
        continue;
      }
    }
    return ACK;
}