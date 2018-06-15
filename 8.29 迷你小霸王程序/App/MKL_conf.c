/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       VCAN_MKL_conf.c
 * @brief      山外KL26 平台配置功能实现文件
 * @author     山外科技
 * @version    v5.0
 * @date       2013-06-26
 */

#include    "common.h"
#include    "stdio.h"
#include    "MKL_uart.h"
#include    "VCAN_LED.h"
#include    "MKL_SysTick.h"

/*!
 *  @brief      断言失败所执行的函数
 *  @param      file    文件路径地址
 *  @param      line    行数
 *  @since      v5.0
 *  Sample usage:       assert_failed(__FILE__, __LINE__);
 */
const char ASSERT_FAILED_STR[] = "断言失败，故障位置位于文件%s的第%d行。\r\n";

void assert_failed(char *file, int line)
{
    while (1)
    {
        //死循环等待程序员检测为何断言失败
        printf(ASSERT_FAILED_STR, file, line);          //通过串口提示断言失败
        lightwave();            //闪烁LED灯提示断言失败
        systick_delay_ms(50);
    }
}

/*!
 *  @brief      重定义printf 到串口
 *  @param      ch      需要打印的字节
 *  @param      stream  数据流
 *  @since      v5.0
 *  @note       此函数由编译器自带库里的printf所调用
 */
int fputc(int ch, FILE *stream)
{
    uart_putchar(VCAN_PORT, (char)ch);
    return(ch);
}


#ifdef  DEBUG
// VECTOR_TABLE end
static char vector_str[][9];
#endif

/*!
 *  @brief      默认中断服务函数
 *  @since      v5.0
 *  @note       此函数写入中断向量表里，不需要用户执行
 */
void default_isr(void)
{
#define VECTORNUM                     (*(volatile uint32_t*)(0xE000ED04))

#ifdef  DEBUG

    uint8 vtr = VECTORNUM;
    DEBUG_PRINTF("\r\n****中断错误，已进入默认中断。发生错误的中断号为%d*****\r\n%s Interrupt", vtr, vector_str[vtr]);
    lightwave();            //闪烁LED灯提示断言失败
    systick_delay_ms(50);

#endif
    return;
}

#ifdef DEBUG
//这个表是输出中断错误信息用的
char vector_str[][9] =
{
    /******  Cortex-M4 Processor Exceptions Numbers ****************************************************************/
    "RES",                         //0
    "RES",
    "NMI"               , /*!< 2 Non Maskable Interrupt                              */
    "HardFault"                 , /*!< 3 Hard Fault                                          */
    "RES"                          ,
    "RES"                          ,
    "RES"                          ,
    "RES"                          , //7
    "RES"                          ,
    "RES"                          , //9
    "RES"                          ,
    "SVCall"                    , /*!< 11 SV Call Interrupt                        */
    "RES"              ,
    "RES",
    "PendSV"                    , /*!< 14 Pend SV Interrupt                        */
    "SysTick"                   , /*!< 15 System Tick Interrupt                    */
    /******  Kinetis 60 specific Interrupt Numbers **********************************************************************/
    "DMA0"                      , // DMA Channel 0 Transfer Complete
    "DMA1"                      , // DMA Channel 1 Transfer Complete
    "DMA2"                      , // DMA Channel 2 Transfer Complete
    "DMA3"                      , // DMA Channel 3 Transfer Complete
    "DMA4"                      , // DMA Channel 4 Transfer Complete
    "RES"                       ,
    "FTFA"                      , // FTFA Interrupt
    "LVD_LVW"                   , // Low Voltage Detect, Low Voltage Warning
    "LLW"                       , // Low Leakage Wakeup
    "I2C0"                      , // I2C0 interrupt
    "I2C1"                      , // I2C1 interrupt
    "SPI0"                      , // SPI0 Interrupt
    "SPI1"                      , // SPI1 Interrupt
    "UART0"                     , // UART0 status/error interrupt
    "UART1"                     , // UART1 status/error interrupt
    "UART2"                     , // UART2 status/error interrupt
    "ADC0"                      , // ADC0 interrupt

    "CMP0"                      , // CMP0 interrupt

    "TPM0"                      , // TPM0 fault, overflow and channels interrupt
    "TPM1"                      , // TPM1 fault, overflow and channels interrupt
    "TPM2"                      , // TPM2 fault, overflow and channels interrupt

    "RTC"                       , // RTC interrupt
    "RTC_Sec"                   , // RTC seconds interrupt
    "PIT"                      , // PIT timer interrupt

    "I2S0"                      , // I2S0 Interrupt
    "USB0"                      , // USB0 interrupt
    "DAC0"                      , // DAC0 interrupt

    "TSI0"                      , // TSI0 Interrupt
    "MCG"                       , // MCG Interrupt

    "LPTMR"                     , // LPTimer interrupt
    "RES"                       , //
    "PTA"                       , // Port A interrupt
    "PTC_PTD"                   , // Port C & D interrupt
};
#endif