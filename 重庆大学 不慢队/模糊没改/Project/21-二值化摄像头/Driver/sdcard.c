#include "include.h"

#define Variable_ADDR   2
#define Para_ADDR   102
#define CCD_ADDR   212
uint8   SD_Buff[1024]; 
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

uint8 Spi_Delay=3;

//用来在第一个页面存储一共有多少页有用数据
void  Write_Information()
{
 uint8 i=0;
  uint16 j=0;
  float temp=0;
  uint8 Parameter_num=14;  //14个可改参数
  
  
  
  SD_Buff[j++]=0x55;              
  SD_Buff[j++]=0xaa;
  SD_Buff[j++]=0xff;                 //数据量的字头
  SD_Buff[j++]=BYTE0(Block_Index);   //第一起始帧用来标识一共存储了多少帧数据
  SD_Buff[j++]=BYTE1(Block_Index);
  
  SD_Buff[j++]=0x55;                 //就像串口发送数据一样把这些数据按照协议的方式存在SD卡上，到时候直接发送即可，上位机便可识别
  SD_Buff[j++]=0xaa;
  SD_Buff[j++]=0xab;                  ///第一起始帧可以存放参数等不会经常改变的数据
  SD_Buff[j++]=Parameter_num;
  

  
  for(i=0;i<Parameter_num;i++)
  { 
     temp=Control_Para[i];
    SD_Buff[j++]=BYTE0(temp);
    SD_Buff[j++]=BYTE1(temp);
    SD_Buff[j++]=BYTE2(temp);
    SD_Buff[j++]=BYTE3(temp);
  }
    SD_Buff[j++]=0X0b;//帧尾
  
  SD_Buff[j++]=0xaa;                 //后面的三个用来指示该扇区的有效内容结束
  SD_Buff[j++]=0xbb;
  SD_Buff[j++]=0xcc;
  
  SD_WriteDisk(SD_Buff,BLOCK_BEGIN);  //////在指定的起始扇区写信息
  Block_Index=1;                       //其他数据只能从第二个扇区写
}
//用来存储运行数据
void Record()
{  
 //用来更新缓存，每10ms执行一次
  float temp;
  int i;
  uint16 j=0;
  uint8 Variable_num=16;
   
  int block_temp;
  
 
  SD_Buff[j++]=0x55;       //就像串口发送数据一样把这些数据按照协议的方式存在SD卡上，到时候直接发送即可
  SD_Buff[j++]=0xaa;
  SD_Buff[j++]=0xad;
  SD_Buff[j++]=Variable_num;
  for(i=0;i<Variable_num;i++)    //64个数据
  {
    temp=Variable[i];
    SD_Buff[j++]=BYTE0(temp);
    SD_Buff[j++]=BYTE1(temp);
    SD_Buff[j++]=BYTE2(temp);
    SD_Buff[j++]=BYTE3(temp);
  }
    SD_Buff[j++]=0x0d;
  
  SD_Buff[j++]=0x55;       //就像串口发送数据一样把这些数据按照协议的方式存在SD卡上，到时候直接发送即可
  SD_Buff[j++]=0xaa;
  SD_Buff[j++]=0xac;
  SD_Buff[j++]=0x8;
  
  
  
  uart_putbuff(UART_PORT, (uint8*)(&LMR[0][0]),     180); //发送边线及中线
  uart_putbuff(UART_PORT, (uint8*)(&LMR[0][0]),     20); //预留20个数据位
  my_putchar(0x0C);
  
   for(i=0;i<CAMERA_SIZE;i++)  //600
  {
    SD_Buff[j++]=*(imgbuff+i);
  }
  for(i=0;i<180;i++) //保存边线 //180
  {
    SD_Buff[j++]=*(&LMR[0][0]+i);
  }
   for(i=0;i<20;i++) //保存边线 //20
  {
     SD_Buff[j++]=0;
  }
  SD_Buff[j++]=0x0C;
  
  SD_Buff[j++]=0xaa;                 //后面的三个用来指示该扇区的有效内容结束
  SD_Buff[j++]=0xbb;
  SD_Buff[j++]=0xcc;
  LED_BLUE_OFF;
    
  block_temp=Block_Index*2;
    
  SD_WriteDisk(SD_Buff,BLOCK_BEGIN+block_temp);
  SD_WriteDisk(&(SD_Buff[512]),BLOCK_BEGIN+block_temp+1);
  LED_BLUE_ON;  
                        //LED灯指示
  Block_Index=Block_Index++;
  
  
}

void SD_SPI_LowSpeed()
{
  Spi_Delay=10;
}

void SD_SPI_HighSpeed()
{
  Spi_Delay=1;
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
    uint8 success=0;
      uint8 buff[4];
     gpio_init (SD_CS, GPO,0);
     gpio_init (SD_SCK, GPO,0);
     gpio_init (SD_SOUT, GPO,0);
     gpio_init (SD_SIN, GPI,0);
     port_init_NoALT (SD_SIN, PULLUP ); 
    
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
uint8 SD_ReadDisk(uint8*buf,uint32 sector)
{
  uint8 r1;
sector<<= 9;//转换为字节地址
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
      if(retry>0xffe) //如果长时间写入没有完成，报错退出
      {
          SD_CS_HIGH;
          return 1; //写入超时返回1
      }
    }
  SD_DisSelect();//取消片选
  return 1;//
}


void SPI_Delay(uint16 i)
{	
  while(i) 
    i--; 
}

//  spi_mosi(SPI0,SPIn_PCS0,cmd,NULL,buff,buff,1,2)
uint8 SPI_SendReceiveData(uint8 TxData)
{

      char i,temp; 

      temp=0; 

      SD_SCK_DAT=0; 

     // SPI_Delay(Spi_Delay);

      for(i=0;i<8;i++) 

      {  

        if(TxData & 0x80) 

        {  
          SD_SOUT_DAT=1; 

        }  

      else SD_SOUT_DAT=0; 

      TxData<<=1; 

      SD_SCK_DAT=1;
    //  SPI_Delay(Spi_Delay);
        
      temp<<=1;  

      if(SD_SIN_IN)temp++; 

      SD_SCK_DAT=0; 

   //   SPI_Delay(Spi_Delay);
      
      }
   return temp; 
}  
      