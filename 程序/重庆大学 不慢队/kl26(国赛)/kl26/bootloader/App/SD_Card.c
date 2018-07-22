#include "include.h"


#define  SD_CS PTE4_OUT

uint8   SD_Type = 0;
void   SD_DisSelect();
uint8   SD_GetResponse(uint8 Response);
uint8   SD_RecvData(uint8*buf,uint16 len);
uint8   SD_Select();

uint8   SD_WaitReady();
uint8  SPI_SendReceiveData(uint8 TxData);



void SD_SPI_HighSpeed()
{
  spi_set_baud (SPI1,1000*1000);	
}

void SD_DisSelect()
{
  SD_CS=1; //取消片选
  SPI_SendReceiveData(0xff);   //提供额外的8个时钟
}
uint8 SD_WaitReady()
{
  uint32 t = 0;
  do
  {
    if(SPI_SendReceiveData(0xff) == 0xff) return 0;
    t++;
  }while(t < 0xffffff);
  return 1;
}

uint8 SD_GetResponse(uint8 Response)
{
  uint16 Count=0xFFFF;//等待次数	   						  
  while((SPI_SendReceiveData(0XFF)!=Response)&&Count) Count--;//等待得到准确的回应  	  
  if(Count==0)return MSD_RESPONSE_FAILURE;//得到回应失败   
  else return MSD_RESPONSE_NO_ERROR;//正确回应	
}
//从sd卡读取一个数据包的内容
//buf:数据缓存区
//len:要读取的数据长度.
//返回值:0,成功;其他,失败;	
uint8 SD_RecvData(uint8*buf,uint16 len)
{			  	  
   if(SD_GetResponse(0xFE)) return 1;//等待SD卡发回数据起始令牌0xFE
    while(len--)//开始接收数据
    {
      *buf=SPI_SendReceiveData(0xFF);
       buf++;
    }
    //下面是2个伪CRC（dummy CRC）
    SPI_SendReceiveData(0xFF);
    SPI_SendReceiveData(0xFF);									  					    
    return 0;//读取成功
}
//向SD卡发送一个命令
//输入: uint8 cmd   命令 
//      u32 arg  命令参数
//      uint8 crc   crc校验值	   
//返回值:SD卡返回的响应															  
uint8 SD_SendCmd(uint8 cmd, uint32 arg, uint8 crc) 
{
    uint8 r1;	
    uint8 Retry=0;
    SD_DisSelect();//取消片选
    SD_CS=0; //片选
    SPI_SendReceiveData(cmd | 0x40);//分别写入命令
    SPI_SendReceiveData(arg >> 24);
    SPI_SendReceiveData(arg >> 16);
    SPI_SendReceiveData(arg >> 8);
    SPI_SendReceiveData(arg);	  
    SPI_SendReceiveData(crc); 
    Retry=0X1F;
    do
    {
     r1=SPI_SendReceiveData(0xFF);
    }
    while((r1&0X80)&&Retry--);
    //返回状态值;
    return r1;
}
uint8 SD_Initialize(void)
{
    uint8 r1;           //存放SD卡的返回值
    uint16 retry;       //用来进行超时计数
    uint16 i;
    uint8 buff[4];
    uint8 success=0;
    
    SD_DisSelect();
    for(i=0;i<10;i++)SPI_SendReceiveData(0XFF);//发送最少74个脉冲
    retry=20;
    do
    {
       r1=SD_SendCmd(CMD0,0,0x95);//进入IDLE状态
       SD_DisSelect();            //取消片选
    }
    while((r1!=0X01)&&retry--);
    SD_Type=0;//默认无卡
    if(r1==0X01)
    {
      if(SD_SendCmd(CMD8,0x1AA,0x87)==1)//SD V2.0
      {
        retry=0XFFFE;
        do
        {
            SD_SendCmd(CMD55,0,1);	       //发送CMD55
            r1=SD_SendCmd(CMD41,0x40000000,1);//发送CMD41
        }while(r1&&retry--);
        if(r1==0)success=1;  //初始化成功了
        r1 = SD_SendCmd(CMD58, 0, 1);
        if(r1==0)
        {
          buff[0] =SPI_SendReceiveData(0xFF);
          buff[1] =SPI_SendReceiveData(0xFF);
          buff[2] =SPI_SendReceiveData(0xFF);
          buff[3] =SPI_SendReceiveData(0xFF); 
          SD_DisSelect();//取消片选
          if(buff[0]&0x40)SD_Type = SD_TYPE_V2HC; //检查CCS
          else SD_Type = SD_TYPE_V2;
        } 
      }
    }
    SD_DisSelect();
    SD_SPI_HighSpeed();//高速	
    return success;//其他错误
}
//读SD卡
//buf:数据缓存区
//sector:扇区
//cnt:扇区数
//返回值:0,ok;其他,失败.
uint8 SD_ReadDisk(uint8*buf,uint32 sector,uint8 cnt)
{
  uint8 r1;
  if(SD_Type!=SD_TYPE_V2HC)sector<<= 9;//转换为字节地址
  if(cnt==1)
  {
    r1=SD_SendCmd(CMD17,sector,0X01);//读命令
    if(r1==0)//指令发送成功
    {
      r1=SD_RecvData(buf,512);//接收512个字节	   
    }
  }
  else
  {
    r1=SD_SendCmd(CMD18,sector,0X01);//连续读命令
    do
    {
      r1=SD_RecvData(buf,512);//接收512个字节	 
      buf+=512;  
    }while(--cnt && r1==0); 	
    SD_SendCmd(CMD12,0,0X01);	//发送停止命令
  }   
  SD_DisSelect();//取消片选
  return r1;
}
uint8 SPI_SendReceiveData(uint8 TxData)
{
  uint16 retry=0;
  SPI_MemMapPtr spi_ptr=SPI1_BASE_PTR;
  
  while(!(SPI_S_REG(spi_ptr)&SPI_S_SPTEF_MASK)) //发送寄存器非空等待
  {
    retry++;
    if(retry>200)return 0;
  }
  SPI_DL_REG(SPI1_BASE_PTR)=TxData;                     //直接读取数据来清空接收缓冲区

  retry=0;
 
  while(!(SPI_S_REG(spi_ptr)& SPI_S_SPRF_MASK))  //检测接收缓冲区非空
  {
    retry++;
    if(retry>2000)return 0;
  } 	                                    //直接读取数据来清空接收缓冲区
 return   SPI_DL_REG(SPI1_BASE_PTR);		
}

      