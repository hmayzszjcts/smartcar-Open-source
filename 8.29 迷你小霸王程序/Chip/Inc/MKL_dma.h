/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_dma.h
 * @brief      DMA函数库
 * @author     山外科技
 * @version    v5.0
 * @date       2013-09-01
 */


#ifndef _MKL_DMA_H_
#define _MKL_DMA_H_


typedef enum
{
    /*        禁用通道            */
    Channel_Disabled        = 0,

    /*        UART            */
    DMA_UART0_Rx            = 2,
    DMA_UART0_Tx            = 3,
    DMA_UART1_Rx            = 4,
    DMA_UART1_Tx            = 5,
    DMA_UART2_Rx            = 6,
    DMA_UART2_Tx            = 7,

    /*        I2S            */
    DMA_I2S0_Rx             = 14,
    DMA_I2S0_Tx             = 15,

    /*        SPI            */
    DMA_SPI0_Rx             = 16,
    DMA_SPI0_Tx             = 17,
    DMA_SPI1_Rx             = 18,
    DMA_SPI1_Tx             = 19,

    /*        I2C            */
    DMA_I2C0                = 22,
    DMA_I2C1                = 23,

    /*        FTM            */
    DMA_TPM0_CH0            = 24,
    DMA_TPM0_CH1            = 25,
    DMA_TPM0_CH2            = 26,
    DMA_TPM0_CH3            = 27,
    DMA_TPM0_CH4            = 28,
    DMA_TPM0_CH5            = 29,


    DMA_FTM1_CH0            = 32,
    DMA_FTM1_CH1            = 33,

    DMA_FTM2_CH0            = 34,
    DMA_FTM2_CH1            = 35,


    /*     ADC/DAC/CMP/CMT    */
    DMA_ADC0                = 40,

    DMA_CMP0                = 42,

    DMA_DAC0                = 45,



    DMA_PORTA              = 49,

    DMA_PORTC              = 51,
    DMA_PORTD              = 52,

    DMA_FTM0_OVER           = 54,
    DMA_FTM1_OVER           = 55,
    DMA_FTM2_OVER           = 56,

    DMA_TSI                 = 57,

    DMA_Always_EN1          = 60,
    DMA_Always_EN2          = 61,
    DMA_Always_EN3          = 62,
    DMA_Always_EN4          = 63,

} DMA_sources;



typedef enum
{
    DADDR_RECOVER = 0,        //恢复目的地址
    DADDR_KEEPON  = 1,        //目的地址保持不变

} DMA_cfg;

typedef enum      //DMA每次传输字节数
{
    DMA_BYTE1 = 1,
    DMA_BYTE2 = 2,
    DMA_BYTE4 = 0,
} DMA_BYTEn;


typedef enum
{
    DMA_CH0,
    DMA_CH1,
    DMA_CH2,
    DMA_CH3,
} DMA_CHn;



#define  DMA_IRQ_EN(DMA_CHn)    do{DMA_DCR(DMA_CHn)  |= DMA_DCR_EINT_MASK;enable_irq((IRQn_t)((IRQn_t)DMA_CHn + DMA0_IRQn));}while(0)        //允许DMA通道传输完成中断
#define  DMA_IRQ_DIS(DMA_CHn)   do{DMA_DCR(DMA_CHn)  &= ~DMA_DCR_EINT_MASK;enable_irq((IRQn_t)((IRQn_t)DMA_CHn + DMA0_IRQn));}while(0)       //禁止DMA通道传输完成中断


#define  DMA_IRQ_CLEAN(DMA_CHn) DMA_DSR_BCR(DMA_CHn)|= DMA_DSR_BCR_DONE_MASK                                                                    //清除通道传输中断标志位

#define  DMA_EN(DMA_CHn)        DMAMUX0_CHCFG(DMA_CHn) |= DMAMUX_CHCFG_ENBL_MASK     //使能通道硬件DMA请求
#define  DMA_DIS(DMA_CHn)       DMAMUX0_CHCFG(DMA_CHn) &= ~DMAMUX_CHCFG_ENBL_MASK       //禁止通道硬件DMA请求

//#define  DMA_CHn_DIS(DMA_CHn)   DMAMUX_CHCFG_REG(DMAMUX_BASE_PTR,DMA_CHn) &= DMAMUX_CHCFG_SOURCE(Channel_Disabled)  //禁用通道



//初始化DMA，使得PORT端口数据通过DMA传输到BUFF缓冲区
extern void dma_portx2buff_init(DMA_CHn, void *SADDR, void *DADDR, PTXn_e, DMA_BYTEn, uint32 count);



//DMA 重新配置，传输完毕后，一些参数会改变，需要重新赋值
static inline void dma_repeat(DMA_CHn CHn,void *SADDR, void *DADDR,uint32 count)
{
    DMA_IRQ_CLEAN(CHn);
    DMA_DSR_BCR(CHn) =   (0
                         | DMA_DSR_BCR_BCR(count)              //传输数目
                         );

    DMA_SAR(CHn) =    (uint32)SADDR;                      // 设置  源地址
    DMA_DAR(CHn) =    (uint32)DADDR;                      // 设置目的地址

    DMA_EN(DMA_CH0);                        //使能DMA 硬件请求
    DMA_DCR(CHn) |=  DMA_DCR_ERQ_MASK;

}

#endif  //_MKL_DMA_H_