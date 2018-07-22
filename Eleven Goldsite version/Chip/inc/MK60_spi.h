/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_spi.c
 * @brief      SPI驱动函数
 * @author     山外科技
 * @version    v5.0
 * @date       2013-07-16
 */
#ifndef __MK60_SPI_H__
#define __MK60_SPI_H__

/**
 *  @brief 主从机模式
 */
typedef enum
{
    MASTER,    //主机模式
    SLAVE      //从机模式
} SPI_CFG;

/**
 *  @brief SPI模块号
 */
typedef enum
{
    SPI0,
    SPI1,
    SPI2
} SPIn_e;

/**
 *  @brief SPI模块片选号
 */
typedef enum
{
    SPI_PCS0 = 1 << 0,
    SPI_PCS1 = 1 << 1,
    SPI_PCS2 = 1 << 2,
    SPI_PCS3 = 1 << 3,
    SPI_PCS4 = 1 << 4,
    SPI_PCS5 = 1 << 5,
} SPI_PCSn_e;


extern uint32 spi_init       (SPIn_e, SPI_PCSn_e , SPI_CFG,uint32 baud);                                        //SPI初始化，选择片选信号，设置模式，波特率
uint32 spi_set_baud (SPIn_e,                      uint32 baud);

//主机接收发送函数
extern void spi_mosi       (SPIn_e, SPI_PCSn_e pcs,                              uint8 *modata, uint8 *midata,               uint32 len);    //SPI发送接收函数,发送databuff数据，并把接收到的数据存放在databuff里(注意，会覆盖原来的databuff)
extern void spi_mosi_cmd   (SPIn_e, SPI_PCSn_e pcs, uint8 *mocmd , uint8 *micmd , uint8 *modata, uint8 *midata, uint32 cmdlen , uint32 len); //SPI发送接收函数,与spi_mosi相比，多了先发送cmd 缓冲区的步骤，即分开两部分发送



#endif  // __MK60_SPI_H__