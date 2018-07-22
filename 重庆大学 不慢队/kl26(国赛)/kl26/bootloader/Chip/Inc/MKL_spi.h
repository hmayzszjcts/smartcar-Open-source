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


//定义主从机模式
typedef enum
{
    MASTER,    //主机模式
    SLAVE      //主机模式
} SPI_CFG;

//定义SPI模块号
typedef enum
{
    SPI0,
    SPI1,

} SPIn_e;

//定义SPI模块片选号
typedef enum
{
    SPI_PCS_NULL,           //手动控制 片选
    SPI_PCS0 = 1 << 0,
} SPI_PCSn_e;


extern uint32 spi_init(SPIn_e, SPI_PCSn_e , SPI_CFG,uint32 baud);                                        //SPI初始化，选择片选信号，设置模式，波特率
uint32 spi_set_baud (SPIn_e,uint32 baud);
extern void spi_pcs_io(SPIn_e spin, SPI_PCSn_e pcs,uint8 data);




#endif  // __MK60_SPI_H__