/*!
 * @file       MK60_spi.c
 * @brief      SPI驱动函数
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
    SPI2
} SPIn_e;

//定义SPI模块片选号
typedef enum
{
    SPIn_PCS0 = 1 << 0,
    SPIn_PCS1 = 1 << 1,
    SPIn_PCS2 = 1 << 2,
    SPIn_PCS3 = 1 << 3,
    SPIn_PCS4 = 1 << 4,
    SPIn_PCS5 = 1 << 5,
} SPIn_PCSn_e;


uint32 spi_init       (SPIn_e, SPIn_PCSn_e , SPI_CFG,uint32 baud);                                        //SPI初始化，设置模式

//主机接收发送函数
void spi_mosi       (SPIn_e spin, SPIn_PCSn_e pcs,                              uint8 *modata, uint8 *midata,               uint32 len);    //SPI发送接收函数,发送databuff数据，并把接收到的数据存放在databuff里(注意，会覆盖原来的databuff)
void spi_mosi_cmd   (SPIn_e spin, SPIn_PCSn_e pcs, uint8 *mocmd , uint8 *micmd , uint8 *modata, uint8 *midata, uint32 cmdlen , uint32 len); //SPI发送接收函数,与spi_mosi相比，多了先发送cmd 缓冲区的步骤，即分开两部分发送



#endif  // __MK60_SPI_H__