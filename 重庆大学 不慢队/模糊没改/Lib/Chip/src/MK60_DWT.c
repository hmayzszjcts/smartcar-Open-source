/*!
 * @file       MK60_dwt.h
 * @brief      flash函数库
 */

#include "common.h"

#define  DWT_CLK_HHZ                        (core_clk_khz )
#define  DEM_CR_TRCENA_MASK                 (1 << 24)
#define  DWT_CTRL_CYCCNTENA                 (1 <<  0)

/*!
 *  @brief      延时(us)
 *  @param      us      延时时间
 *  @since      v5.0
 */
void dwt_delay_us(uint32 us)
{
    uint32 st,et,ts;
    DEMCR            |=  DEM_CR_TRCENA_MASK;        //使能 调试时钟 （默认已经打开）
    DWT_CTRL         |=  DWT_CTRL_CYCCNTENA;

    st = DWT_CYCCNT;
    ts =  us * (DWT_CLK_HHZ /(1000));
    et = st + ts;
    if(et < st)
    {
        //溢出，需要转动一周
        while(DWT_CYCCNT > et);      //等待 DWT_CYCCNT 溢出 返回0
    }

    while(DWT_CYCCNT < et);      //等待 DWT_CYCCNT 到底计数值
}

/*!
 *  @brief      延时(ms)
 *  @param      ms      延时时间
 *  @since      v5.0
 */
void dwt_delay_ms(uint32 ms)
{
    while(ms--)
    {
        dwt_delay_us(1000);
    }
}