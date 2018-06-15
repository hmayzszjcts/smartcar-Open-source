#include "include.h"
/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
* 文件名       ：L3G4200.c
* 描述         ：陀螺仪软件模拟SPI驱动程序
* 实验平台     ：凌立印象开发板
* 库版本       ：基于野火库
* 嵌入系统     ：
* 作者         ：xuxu
**********************************************************************************/


/************************************************
*  函数名称：L3g4200_Init
*  功能说明：L3G4200软件模拟spi初始化
*  参数说明：无
*  函数返回：无
*  修改时间：2014-1-14    no测试
*************************************************/
void L3G4200_Init()
{
  gpio_init(PORTB, 4,GPO,HIGH);
  gpio_init(PORTC, 6,GPO,HIGH);
  gpio_init(PORTC, 7,GPO,HIGH);
  gpio_init(PORTC,18,GPI,HIGH);
  SPI_Write_Byte(CTRL_REG1,0x0f);
  SPI_Write_Byte(CTRL_REG2,0x00);
  SPI_Write_Byte(CTRL_REG3,0x08);
  SPI_Write_Byte(CTRL_REG4,0x30);
  SPI_Write_Byte(CTRL_REG5,0x00);
}


/************************************************
*  函数名称：SPI_delay
*  功能说明：L3G4200软件模拟spi delay函数
*  参数说明：length为时间
*  函数返回：无
*  修改时间：2014-1-14    no测试
*************************************************/
void SPI_delay(signed int length)
{
  while (length >0)
    length--;
}



/************************************************
*  函数名称：SPI_Write_Byte
*  功能说明：L3G4200软件模拟spi 写字节程序
*  参数说明：RegisterAddress为寄存器地址，Command为数据或命令
*  函数返回：无
*  修改时间：2014-1-14    no测试
*************************************************/
void SPI_Write_Byte(unsigned char RegisterAddress, unsigned char Command)
{
  unsigned char	ControlValue = 0;
  unsigned short int ValueToWrite = 0;
  unsigned char	i = 0;
  ControlValue = RegisterAddress;
  SPC_H;SPI_delay(1);
  CS_H;SPI_delay(1);
  CS_L;SPI_delay(1);
  for(i=0; i<8; i++)//Write out the address
  { 	
    SPC_L;
    if(0x80 == (ControlValue & 0x80))
      SDI_H;		
    else
      SDI_L;		 
    SPI_delay(1);
    SPC_H;SPI_delay(1);
    ControlValue<<= 1;	
  }
  ValueToWrite = Command;
  for (i=0; i<8; i++)//Write out the data
  {
    SPC_L;
    if(0x80 == (ValueToWrite & 0x80))
      SDI_H;	  
    else
      SDI_L;
    SPI_delay(1);
    SPC_H;SPI_delay(1);
    ValueToWrite <<= 1;	
  }
  CS_H;SPI_delay(2);
}


/************************************************
*  函数名称：SPI_Read_Byte
*  功能说明：L3G4200软件模拟spi 读单字节程序
*  参数说明：RegisterAddress为寄存器地址
*  函数返回：ReceiveData为返回值
*  修改时间：2014-1-14    no测试
*************************************************/
unsigned char SPI_Read_Byte(unsigned char RegisterAddress)
{
  unsigned char	ControlValue = 0;
  unsigned char ReceiveData = 0;
  unsigned char	i = 0;
  ControlValue = RegisterAddress | 0x80;
  SPC_H;delay(1);	
  CS_H;delay(1);
  CS_L;delay(1);
  for(i=0; i<8; i++)//Write out the control word
  {
    SPC_L;
    if(0x80 == (ControlValue & 0x80))
      SDI_H;	  
    else
      SDI_L; 
    delay(1);
    SPC_H;
    delay(1);
    ControlValue <<= 1;	
  }
  for(i=0; i<8; i++) //Read data in
  {
    SPC_L;delay(1);
    SPC_H;
    ReceiveData <<= 1;		
    if(SDO)
      ReceiveData |= 1;	
    delay(1);
  }
  CS_H;	
  return ReceiveData;	
} 



/************************************************
*  函数名称：SPI_Read_2Byte
*  功能说明：L3G4200软件模拟spi 读双字节程序
*  参数说明：无
*  函数返回：无
*  修改时间：2014-1-14    no测试
*************************************************/
unsigned short int SPI_Read_2Byte(unsigned char RegisterAddress)
{
  unsigned char	ControlValue = 0;
  unsigned short int ReceiveData = 0;
  unsigned char	i = 0;
  ControlValue = RegisterAddress | 0xC0;
  SPC_H;SPI_delay(1);	
  CS_H;SPI_delay(1);
  CS_L;SPI_delay(1);
  for(i=0; i<8; i++)//Write out the control word
  {
    SPC_L;
    if(0x80 == (ControlValue & 0x80))
      SDI_H;	  
    else
      SDI_L; 
    SPI_delay(1);
    SPC_H;
    SPI_delay(1);
    ControlValue <<= 1;	
  }
  for(i=0; i<16; i++) //Read data in
  {
    SPC_L;SPI_delay(1);
    SPC_H;
    ReceiveData <<= 1;		
    if(SDO)
      ReceiveData |= 1;	
    SPI_delay(1);
  }
  CS_H;	
  return ReceiveData;	
} 


