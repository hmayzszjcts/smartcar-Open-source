/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		KEA128_spi
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.23
 * @Target core		S9KEA128AMLK
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-11-6
 ********************************************************************************************************************/


#include "KEA128_spi.h"


SPI_Type* SPIN[2] = SPI_BASES; //定义两个指针数组保存 SPIx 的地址


 
#define SPI_TX_WAIT(SPIn)	while( !( SPIN[SPIn]->S & SPI_S_SPTEF_MASK ) )  //等待发送完成
#define SPI_RX_WAIT(SPIn)   while( !( SPIN[SPIn]->S & SPI_S_SPRF_MASK ) )   //等待接收数据


//-------------------------------------------------------------------------------------------------------------------
//  @brief      SPI初始化
//  @param      spin            选择SPI模块   有spi0 spi1
//  @param      pcs             选择片选引脚
//  @param      master          选择主从模式
//  @param      baud            选择通信速率
//  @return     				返回真实波特率
//  @since      v2.0
//  Sample usage:       		uint32 baud = spi_init(spi0,USE_PCS,MASTER,1*1000*1000);              //初始化SPI,主机模式, 波特率为1M ,返回真实波特率到baud变量
//-------------------------------------------------------------------------------------------------------------------
uint32 spi_init(SPIn_e spin, SPI_PCSn_e pcs, SPI_CFG master,uint32 baud)
{
    uint8 i,j;
    uint8 sppr,spr;
    uint32 Scaler[] = {2,4,8,16,32,64,128,256,512};
    uint8  Prescaler[] = {1,2,3,4,5,6,7,8};
    uint32 min_diff, diff;
    
    if(spi0 == spin)
    {
        SIM->SCGC |= SIM_SCGC_SPI0_MASK;    //开启时钟
        //引脚复用
        if(B2 == SPI0_SCK_PIN)
        {
            SIM->PINSEL &= ~(uint32)SIM_PINSEL_SPI0PS_MASK;
        }
        else
        {
            SIM->PINSEL |= SIM_PINSEL_SPI0PS_MASK;
        }
    }
    else
    {
        SIM->SCGC |= SIM_SCGC_SPI1_MASK;    //开启时钟
        //引脚复用
        if(D0 == SPI1_SCK_PIN)
        {
            SIM->PINSEL1 &= ~(uint32)SIM_PINSEL1_SPI1PS_MASK;
        }
        else
        {
            SIM->PINSEL1 |= SIM_PINSEL1_SPI1PS_MASK;
        }
    }
    //根据主从机模式设置工作模式 最大波特率为总线时钟的二分之一
    if(master == MASTER)
    {
        min_diff = ~0;
        for(i=0; i<9; i++)
        {
            for(j=0; j<8; j++)
            {
                diff = abs(bus_clk_khz*1000/(Prescaler[j]*Scaler[i]) - baud);
                if(min_diff > diff)
                {
                    //更新最佳参数
                    sppr = j;
                    spr = i;
                    min_diff = diff;
                    
                    if(min_diff == 0)	break;//刚好匹配
                }
            }
        }
        
        SPIN[spin]->C1 = (0
                          | (pcs << SPI_C1_SSOE_SHIFT)          //配合C2_MODFEN位选择片选引脚的功能
                          | SPI_C1_SPE_MASK                     //SPI 系统使能
                          | SPI_C1_MSTR_MASK                    //SPI 模块配置为SPI 主机
                          //| SPI_C1_SPIE_MASK                  //SPRF 或MODF 为1 时请求硬件中断
                          //| SPI_C1_SPTIE_MASK                 //SPTEF 为1 时，请求硬件中断。
                          //| SPI_C1_CPOL_MASK                  //0：高电平有效SPI 时钟（空闲时为低电平） 1：低电平有效SPI 时钟（空闲时为高电平）
                          //| SPI_C1_CPHA_MASK                            //0:SPSCK 上的第一个边沿出现在数据传输的第一个周期的中间 1:SPSCK 上的第一个边沿出现在数据传输的第一个周期的开始
                          //| SPI_C1_LSBFE_MASK                 //1:SPI 串行数据传输从最低有效位开始
                          );    
        SPIN[spin]->C2 = pcs << SPI_C2_MODFEN_SHIFT;            //配合C1_SSOE位选择片选引脚的功能
        SPIN[spin]->BR = SPI_BR_SPR(spr) | SPI_BR_SPPR(sppr);   //设置波特率参数
    }
    else{}
    return (bus_clk_khz*1000/Prescaler[sppr]/Scaler[spr]);      //返回实际波特率
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      SPI发送接收函数
//  @param      spin            选择SPI模块   有spi0 spi1
//  @param      modata          发送的数据缓冲区地址(不需要接收则传 NULL)
//  @param      midata          发送数据时接收到的数据的存储地址(不需要接收则传 NULL)
//  @param      len             发送的字节数
//  @return     void				
//  @since      v2.0
//  Sample usage:       		spi_mosi(spi0,buf,buf,1);    //发送buff的内容，并接收到buf里，长度为1字节
//-------------------------------------------------------------------------------------------------------------------
void spi_mosi(SPIn_e spin, SPI_PCSn_e pcs, uint8 *modata, uint8 *midata, uint32 len)
{
    while(len)
    {
        SPI_TX_WAIT(spin);
        SPIN[spin]->D = *modata;
        SPI_RX_WAIT(spin);
        if(midata != NULL)  *midata = SPIN[spin]->D;
        else                SPIN[spin]->D;
        len--;
        modata++;
        if(midata != NULL)  midata++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      SPI发送接收函数
//  @param      spin            选择SPI模块   有spi0 spi1
//  @param      mocmd           发送的命令缓冲区地址(不需要接收则传 NULL)
//  @param      micmd           发送命令时接收到的数据的存储地址(不需要接收则传 NULL)
//  @param      modata          发送的数据缓冲区地址(不需要接收则传 NULL)
//  @param      midata          发送数据时接收到的数据的存储地址(不需要接收则传 NULL)
//  @param      cmdlen          发送命令的字节数
//  @param      len             发送数据的字节数
//  @return     void				
//  @since      v2.0
//  Sample usage:       		spi_mosi_cmd(spi0,cmd,NULL,buff,buff,1,2);    //发送cmd/buff的内容，不接收cmd发送时的数据，接收buff发送时的数据到buff里，长度分别为1、2字节
//-------------------------------------------------------------------------------------------------------------------
void spi_mosi_cmd(SPIn_e spin, SPI_PCSn_e pcs, uint8 *mocmd , uint8 *micmd , uint8 *modata , uint8 *midata, uint32 cmdlen , uint32 len)
{
    spi_mosi(spin,pcs,mocmd,micmd,cmdlen);
    spi_mosi(spin,pcs,modata,midata,len);
}


