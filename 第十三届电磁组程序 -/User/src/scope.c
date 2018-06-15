/*********************************************************************************************************************
 * @file       		scope.c
 *  				虚拟示波器
 ********************************************************************************************************************/
#include "scope.h"

short int OutData[8]={0};

unsigned short CRC_CHECK(unsigned char *Buf,unsigned char CRC_CNT)  //循环冗余校验
{
	unsigned short CRC_Temp;
	unsigned char i,j;
	CRC_Temp=0xffff;
	for(i=0;i<CRC_CNT;i++)
	{	CRC_Temp^=Buf[i];  //按位异或a=a^b(反正是校验的一种方法)
	for(j=0;j<8;j++)
	{
	    if(CRC_Temp&0x01)
		CRC_Temp=(CRC_Temp>>1)^0xa001;
	    else
		CRC_Temp=CRC_Temp>>1;
	}
	}
	return (CRC_Temp);
}

void DataOutPut(void){  //传八路数据
	short int temp[8]={0};  
	unsigned short int temp1[8]={0};
	unsigned char databuf[18]={0};
	unsigned char i;
	unsigned short CRC16=0;
	for(i=0;i<8;i++)
	{   temp[i]=(short int)OutData[i];
	    temp1[i]=(unsigned short int)temp[i];		
	}
	for(i=0;i<8;i++)
	{   databuf[i*2]=(unsigned char)(temp1[i]%256);		
	    databuf[(i*2)+1]=(unsigned char)(temp1[i]/256);		
	}
	CRC16=CRC_CHECK(databuf,16);
	databuf[16]=CRC16%256;
	databuf[17]=CRC16/256;
	for(i=0;i<10;i++)
	{ uart_putchar(uart1, (char)databuf[i]);
	}
}
/*!
 *  @brief      山外多功能调试助手上位机，虚拟示波器显示函数
 *  @param      wareaddr    波形数组起始地址
 *  @param      waresize    波形数组占用空间的大小
 */
void Send_Scope(void *wareaddr, uint32_t waresize)
{
#define CMD_WARE     3
    uint8_t cmdf[2] = {CMD_WARE, ~CMD_WARE};    //串口调试 使用的前命令
    uint8_t cmdr[2] = {~CMD_WARE, CMD_WARE};    //串口调试 使用的后命令

    uart_putbuff(uart1, cmdf, sizeof(cmdf));    //先发送前命令
    uart_putbuff(uart1, (uint8_t *)wareaddr, waresize);    //发送数据
    uart_putbuff(uart1, cmdr, sizeof(cmdr));    //发送后命令
}
