#include "include.h"

int read_buff[6];

void I2C_Start(void);
void I2C_Stop(void);
void I2C_Ack(void);
void I2C_NoAck(void);
void I2C_SendByte(uint8);
uint8 I2C_ReceiveByte(void);
void I2C_Delay(uint16);

/*
 * I2C_Init
 * 初始化I2C所需引脚
 */
void I2C_Init(void)
{ 
 gpio_init(PTB22,GPO,1);                     //初始化SCL管脚
 gpio_init(PTB23,GPO,1);                     //初始化SDA管脚
 MPU3050_Init();
 MMA8451_Init();
}
/*
 * I2C_WriteReg
 * 写I2C设备寄存器
 */
void I2C_WriteReg(uint8 dev_addr,uint8 reg_addr , uint8 data)
{		
  I2C_Start();
  I2C_SendByte(dev_addr<<1);         
  I2C_SendByte(reg_addr );   
  I2C_SendByte(data);   
  I2C_Stop(); 
}
//读寄存器
uint8 I2C_ReadByte(uint8 dev_addr,uint8 reg_addr)
{
  uint8 data;
  I2C_Start();
  I2C_SendByte( dev_addr<<1); 
  I2C_SendByte( reg_addr ); 
  I2C_Start();	
  I2C_SendByte((dev_addr<<1)+1); 
  data= I2C_ReceiveByte();
  I2C_NoAck();
  I2C_Stop();
  return data;
}

//读寄存器
int16 I2C_ReadWord(uint8 dev_addr,uint8 reg_addr)
{
  char h,l;
  I2C_Start();
  I2C_SendByte( dev_addr<<1); 
  I2C_SendByte( reg_addr); 
  I2C_Start();	
  I2C_SendByte((dev_addr<<1)+1); 
  h= I2C_ReceiveByte();
  I2C_Ack();
  l= I2C_ReceiveByte();
  I2C_NoAck();
  I2C_Stop();
  return (h<<8)+l;
}

void I2C_ReadGryo(uint8 dev_addr,uint8 reg_addr,int16 *x,int16 *y)
{
  char h,l;
  I2C_Start();
  I2C_SendByte( dev_addr<<1); 
  I2C_SendByte( reg_addr); 
  I2C_Start();	
  I2C_SendByte((dev_addr<<1)+1); 
  h= I2C_ReceiveByte();
  I2C_Ack();
  l= I2C_ReceiveByte();
  I2C_Ack();
  *x=(h<<8)+l;
  h= I2C_ReceiveByte();
  I2C_Ack();
  l= I2C_ReceiveByte();
  I2C_Ack();
  h= I2C_ReceiveByte();
  I2C_Ack();
  l= I2C_ReceiveByte();
  I2C_NoAck();
  *y=(h<<8)+l;
  I2C_Stop();
}
/*
 * I2C_Start
 * I2C起始信号，内部调用
 */
void I2C_Start(void)
{
  I2C_SDA_OUT();
  I2C_DELAY();
  I2C_DELAY();
  I2C_SDA_O=1; 
  I2C_SCL=1;
  I2C_DELAY();
  I2C_DELAY();
  I2C_SDA_O=0; 
  I2C_DELAY();
  I2C_DELAY();
  I2C_SCL=0;
  I2C_DELAY();
  I2C_DELAY();
}

/*
 * I2C_Stop
 * I2C停止信号，内部调用
 */
 void I2C_Stop(void)
{ 
    I2C_SDA_O=0;
    I2C_SCL=0; 
  I2C_DELAY();
  I2C_DELAY();
    I2C_SCL=1;
  I2C_DELAY();
  I2C_DELAY();
    I2C_SDA_O=1;
  I2C_DELAY();
  I2C_DELAY();
    I2C_SCL=0;
}

/*
 * I2C_Stop
 * I2C应答信号，内部调用
 */
void I2C_Ack(void)
{	
  I2C_SCL=0;
  I2C_DELAY();
  
  I2C_SDA_O=0;
  I2C_DELAY();
  
  I2C_SCL=1;
  I2C_DELAY();
  
  I2C_SCL=0;
  I2C_DELAY();
}

/*
 * I2C_NoAck
 * I2C无应答信号，内部调用
 */
 void I2C_NoAck(void)
{	
  I2C_SCL=0;
  I2C_DELAY();
  I2C_SDA_O=1;
  I2C_DELAY();
  I2C_SCL=1;
  I2C_DELAY();
  I2C_SCL=0;
  I2C_DELAY();
}


/*
 * I2C_SendByte
 * I2C发送数据，内部调用
 */
void I2C_SendByte(uint8 data) 
{
  uint8 i=8;
  while(i--)
  {
    I2C_SCL=0;
    if(data&0x80)
      I2C_SDA_O=1; 
    else 
      I2C_SDA_O=0;   
    data<<=1;
    I2C_DELAY();
    I2C_SCL=1;
    I2C_DELAY();
  }
  I2C_SCL=0;
  I2C_DELAY();
  I2C_SDA_I=1;
  I2C_DELAY();
  I2C_SCL=1; 
  I2C_DELAY();
  I2C_SCL=0;
}

/*
 * I2C_SendByte
 * I2C接收数据，内部调用
 */
 uint8 I2C_ReceiveByte(void)  
{ 
  uint8 i=8;
  uint8 ReceiveByte=0;
  
  I2C_SDA_O=1;	
  I2C_SDA_IN();	
  
  while(i--)
  {
    ReceiveByte<<=1;      
    I2C_SCL=0;
    I2C_DELAY();
    I2C_SCL=1;
    I2C_DELAY();	
    if(I2C_SDA_I)
    {
      ReceiveByte|=0x01;
    }
    
  }
  I2C_SDA_OUT();
  I2C_SCL=0;
  
  return ReceiveByte;
}

/*
 * I2C_SendByte
 * I2C延时函数，内部调用
 */
void I2C_Delay(uint16 i)
{	
  while(i) 
    i--; 
}

/******************获得Z轴加速度******************/
int16 Get_Z_Acc()
{
  int16 temp;
  temp=I2C_ReadWord(I2C_MMA8451_ADR,0X05);
  return temp;
}
/*****************获得y轴角速度*****************/
int16 Get_Y_Gyro()
{
  int16 temp; 
  temp=I2C_ReadWord(I2C_MPU3050_ADR,0X1f);
  return temp;
}
/*****************获得x轴角速度*****************/
int16 Get_X_Gyro()
{
  int16 temp;
  temp=I2C_ReadWord(I2C_MPU3050_ADR,0X1D);
  return temp;
}

void MPU3050_Init()
{
 I2C_WriteReg(I2C_MPU3050_ADR,0x3E , 0X80); //复位MPU3050
 DELAY_MS(5);
 I2C_WriteReg(I2C_MPU3050_ADR,0x15,  0);    //采样不分频
 DELAY_MS(5);
 I2C_WriteReg(I2C_MPU3050_ADR,0x16 , 0x11);  //采样速率8kHz&&采样范围±1000°
 DELAY_MS(5);
 I2C_WriteReg(I2C_MPU3050_ADR,0x17 , 0);    //不产生中断
 DELAY_MS(5);
 I2C_WriteReg(I2C_MPU3050_ADR,0x3E , 0X00); //启动MPU3050
}

void MMA8451_Init()
{ 
 I2C_WriteReg(I2C_MMA8451_ADR,0x0E, 1);  //0为+-2g，1为+-4g，2为+-8g
 DELAY_MS(5);
 I2C_WriteReg(I2C_MMA8451_ADR,0x0F, 0X10);  // 使能低通滤波
 DELAY_MS(5);
 I2C_WriteReg(I2C_MMA8451_ADR,0x2A , 1); //0x2a为控制寄存器1 激活mma8451
 DELAY_MS(5);
}
	
