#include "include.h"

#define Variable_ADDR   2
#define Para_ADDR   102
#define CCD_ADDR   212
uint8   SD_Buff[512]; 
uint8   SD_OK=0;
uint8   SaveData=0;
uint16  Block_Index=0;
uint8   Zone_Index=0;
uint8   Zone_Read_Index=0;
uint8   Block_Index_Receiving;
uint8   SD_Type = 0;
void   SD_DisSelect();
uint8   SD_GetResponse(uint8 Response);
uint8   SD_RecvData(uint8*buf,uint16 len);
uint8   SD_Select();
void   SD_SPI_HighSpeed();
void   SD_SPI_LowSpeed();
uint8   SD_WaitReady();
uint8  SD_WriteDisk(uint8*buf,uint32 sector);
uint8  SPI_SendReceiveData(uint8 TxData);


//用来在第一个页面存储一共有多少页有用数据
void  Write_Information()
{
  SD_Buff[0]=BYTE0(Block_Index);  
  SD_Buff[1]=BYTE1(Block_Index);
  SD_WriteDisk(SD_Buff,BLOCK_BEGIN);//第一个扇区用来记录重要的信息
  Block_Index=1;                       //其他数据只能从第二个扇区写
}
//用来存储运行数据
void Record()
{
//用来更新缓存，每10ms执行一次
  float temp;
  uint8 i;
  SD_Buff[0]=BYTE0(Block_Index);         
  SD_Buff[1]=BYTE1(Block_Index); 
 
   SD_WriteDisk(SD_Buff,BLOCK_BEGIN+Block_Index);
   LED_BLUE_TURN;                          //LED灯指示
   Block_Index++;
}

void SD_SPI_LowSpeed()
{
  
   spi_init(SPI1,SPIn_PCS0, MASTER,200*1000);
}

void SD_SPI_HighSpeed()
{
  spi_init(SPI1,SPIn_PCS0, MASTER,10000*1000);
}

void SD_DisSelect()
{
  SD_CS_HIGH;
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
    SD_CS_LOW; //片选
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
    
     gpio_init (SD_CS, GPO,0);
    
    SD_SPI_LowSpeed();	//设置到低速模式 
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
uint8 SD_ReadDisk(uint8*buf,uint32 sector)
{
  uint8 r1;
  if(SD_Type!=SD_TYPE_V2HC)sector<<= 9;//转换为字节地址
  r1=SD_SendCmd(CMD17,sector,0X01);//读命令
  if(r1==0)//指令发送成功
  {
    r1=SD_RecvData(buf,512);//接收512个字节	   
  }
  SD_DisSelect();//取消片选
  return r1;
}



//写SD卡
//buf:数据缓存区
//sector:起始扇区
//cnt:扇区数
//返回值:0,ok;其他,失败.
uint8 SD_WriteDisk(uint8*buf,uint32 sector)
{
  uint8 r1;
  uint16 t;
  uint16 retry;
  if(SD_Type!=SD_TYPE_V2HC)sector <<= 9;//转换为字节地址
  if(SD_Type!=SD_TYPE_MMC)
  {
    SD_SendCmd(CMD55,0,0X01);	
    SD_SendCmd(CMD23,1,0X01);//发送指令	
  }
  r1=SD_SendCmd(CMD24,sector,0X01);//单块读命令
  if(r1==0)
  {
    
    SD_CS_LOW;
    
    SPI_SendReceiveData(0XFF);
    SPI_SendReceiveData(0XFF);
    SPI_SendReceiveData(0XFF);
    
    SPI_SendReceiveData(0XFE);//开始写

    for(t=0;t<512;t++)SPI_SendReceiveData(buf[t]);//提高速度,减少函数传参时间
    
    SPI_SendReceiveData(0xFF);//忽略crc
    SPI_SendReceiveData(0xFF);
    t=SPI_SendReceiveData(0xFF);//接收响应
    if((t&0x1F)!=0x05)return 2;//响应错误	
    
   }						 	 
    retry = 0;
    while(!SPI_SendReceiveData(0xff))
    {
      retry++;
      if(retry>0xfffe) //如果长时间写入没有完成，报错退出
      {
          SD_CS_HIGH;
          return 1; //写入超时返回1
      }
    }
  SD_DisSelect();//取消片选
  return 1;//
}
uint8 SPI_SendReceiveData(uint8 TxData)
{
  

  uint8 temp;
  SPI_PUSHR_REG(SD_SPI)  = 0
               |SPI_PUSHR_CTAS(0)
               |TxData;
  
  while(!(SPI_SR_REG(SD_SPI)& SPI_SR_TCF_MASK));
  SPI_SR_REG(SD_SPI) |= SPI_SR_TCF_MASK ;               
  
  while(!(SPI_SR_REG(SD_SPI)& SPI_SR_RFDF_MASK)); 
  temp = (uint8)(SPI_POPR_REG(SD_SPI) & 0xff);    
  
  
  SPI_SR_REG(SD_SPI)|= SPI_SR_RFDF_MASK;                
  return temp;
 
 
 
}

      