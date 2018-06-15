#ifndef _L3G4200_H_
#define _L3G4200_H_


/*************************************************************/
/*******************L3G4200 SPI 端口宏定义********************/
/*************************************************************/

//不知道有么有CS线行吗？？？？？？
/*********定义使能端*********/
#define CS_L   PTB4_OUT=0      
#define CS_H   PTB4_OUT=1 
/*********定义时钟线*********/
#define SPC_L   PTC6_OUT=0      
#define SPC_H   PTC6_OUT=1 
/*********定义数据线*********/
#define SDI_L   PTC7_OUT=0     
#define SDI_H   PTC7_OUT=1  
#define SDO     PTD18_IN 


void L3G4200_Init();
void SPI_delay(signed int length);
void SPI_Write_Byte(unsigned char RegisterAddress, unsigned char Command);
unsigned char SPI_Read_Byte(unsigned char RegisterAddress);
unsigned short int SPI_Read_2Byte(unsigned char RegisterAddress);


#endif