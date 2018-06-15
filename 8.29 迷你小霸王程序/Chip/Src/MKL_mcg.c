/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MKL_mcg.c
 * @brief      MCG PLL驱动
 * @author     山外科技
 * @version    v5.0
 * @date       2013-06-29
 */


#include "common.h"
#include "MKL_mcg.h"


mcg_cfg_t mcg_cfg[PLL_MAX] =
{
    {EXTAL_IN_MHz / (PRDIV + 1)*(VDIV + 24), PRDIV, VDIV}, //PLLUSR
    {48,    24,     0       },                          //PLL48
    {64,    24,     8       },                          //PLL64
    {66,    24,     9       },                          //PLL66
    {68,    24,     10      },                          //PLL68
    {72,    24,     12       },                          //PLL72
    {80,    24,     16       },                          //PLL80
    {96,    24,     24       },                          //PLL96*
    {128,   24,     40       },                         //PLL128
    {136,   24,     44       },                         //PLL136
    {148,   24,     50       },                         //PLL148
    {168,   24,     60       },                         //PLL168
    {220,   24,     76       },                         //PLL200
};

mcg_div_t mcg_div = {CORE_DIV, BUS_DIV};


/*!
 *  @brief      MCG 分频配置
 *  @param      PLL_e       频率设置参数
 *  @since      v5.0
 */
void mcg_div_count(PLL_e pll)
{
    uint16 clk = mcg_cfg[pll].clk;
    if( (pll > PLLUSR) && (pll < PLL_MAX) )
    {
        //设置分频
        if      (clk <= 1 * MAX_CORE_CLK)   mcg_div.core_div = 0;          // core  = MCG
        else if(clk <= 2 * MAX_CORE_CLK)   mcg_div.core_div = 1;          // core  = MCG/2
        else if(clk <= 3 * MAX_CORE_CLK)   mcg_div.core_div = 2;          // core  = MCG/3
        else if(clk <= 4 * MAX_CORE_CLK)   mcg_div.core_div = 3;          // core  = MCG/4
        else if(clk <= 5 * MAX_CORE_CLK)   mcg_div.core_div = 4;          // core  = MCG/5
        else if(clk <= 6 * MAX_CORE_CLK)   mcg_div.core_div = 5;          // core  = MCG/6
        else if(clk <= 7 * MAX_CORE_CLK)   mcg_div.core_div = 6;          // core  = MCG/7
        else if(clk <= 8 * MAX_CORE_CLK)   mcg_div.core_div = 7;          // core  = MCG/8
        else if(clk <= 9 * MAX_CORE_CLK)   mcg_div.core_div = 8;          // core  = MCG/9
        else if(clk <= 10 * MAX_CORE_CLK)   mcg_div.core_div = 9;         // core  = MCG/10
        else if(clk <= 11 * MAX_CORE_CLK)   mcg_div.core_div = 10;        // core  = MCG/11
        else if(clk <= 12 * MAX_CORE_CLK)   mcg_div.core_div = 11;        // core  = MCG/12
        else if(clk <= 13 * MAX_CORE_CLK)   mcg_div.core_div = 12;        // core  = MCG/13
        else if(clk <= 14 * MAX_CORE_CLK)   mcg_div.core_div = 13;        // core  = MCG/14
        else if(clk <= 15 * MAX_CORE_CLK)   mcg_div.core_div = 14;        // core  = MCG/15
        else                              mcg_div.core_div = 15;         // core  = MCG/16


        //bus 频率 由 sys 分频而来
        clk = mcg_cfg[pll].clk /(mcg_div.core_div+1);


        if     (clk <= 1 * MAX_BUS_CLK)   mcg_div.bus_div = 0;          // bus  = MCG
        else if(clk <= 2 * MAX_BUS_CLK)   mcg_div.bus_div = 1;          // bus  = MCG/2
        else if(clk <= 3 * MAX_BUS_CLK)   mcg_div.bus_div = 2;          // bus  = MCG/3
        else if(clk <= 4 * MAX_BUS_CLK)   mcg_div.bus_div = 3;          // bus  = MCG/4
        else if(clk <= 5 * MAX_BUS_CLK)   mcg_div.bus_div = 4;          // bus  = MCG/5
        else if(clk <= 6 * MAX_BUS_CLK)   mcg_div.bus_div = 5;          // bus  = MCG/6
        else if(clk <= 7 * MAX_BUS_CLK)   mcg_div.bus_div = 6;          // bus  = MCG/7
        else if(clk <= 8 * MAX_BUS_CLK)   mcg_div.bus_div = 7;          // bus  = MCG/8
        else if(clk <= 9 * MAX_BUS_CLK)   mcg_div.bus_div = 8;          // bus  = MCG/9
        else if(clk <= 10 * MAX_BUS_CLK)   mcg_div.bus_div = 9;         // bus  = MCG/10
        else if(clk <= 11 * MAX_BUS_CLK)   mcg_div.bus_div = 10;        // bus  = MCG/11
        else if(clk <= 12 * MAX_BUS_CLK)   mcg_div.bus_div = 11;        // bus  = MCG/12
        else if(clk <= 13 * MAX_BUS_CLK)   mcg_div.bus_div = 12;        // bus  = MCG/13
        else if(clk <= 14 * MAX_BUS_CLK)   mcg_div.bus_div = 13;        // bus  = MCG/14
        else if(clk <= 15 * MAX_BUS_CLK)   mcg_div.bus_div = 14;        // bus  = MCG/15
        else                              mcg_div.bus_div = 15;         // bus  = MCG/16

    }
}

/*!
 *  @brief      PLL超频
 *  @param      PLL_e       频率设置参数
 *  @return     超频频率（MHz）
 *  @since      v5.0
 *  @warning    此函数只能在 复位后没进行任何频率设置情况下调用，即MCG在FEI模式下才可调用
 *  Sample usage:       uint8 clk = pll_init(PLL100);        //超频
 */
uint8 pll_init(PLL_e pll)
{
    SIM_SOPT2 |= (0         //选择 PLL时钟
                  | SIM_SOPT2_PLLFLLSEL_MASK
                 );
    mcg_div_count( pll);

    SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;      //PTA18 和 PTA19 用于 晶振

    // set clock dividers to desired value
    SIM_CLKDIV1 = SIM_CLKDIV1_OUTDIV1(mcg_div.core_div) |  SIM_CLKDIV1_OUTDIV4(mcg_div.bus_div);


    //上电复位后，单片机会自动进入 FEI 模式，使用 内部参考时钟

    //FEI -> FBE
    OSC0_CR  =  ( 0
                  | OSC_CR_ERCLKEN_MASK     //使能 外部参考时钟
                  //| OSC_CR_SC2P_MASK      //配置电容
                  //| OSC_CR_SC4P_MASK      //配置电容
                  //| OSC_CR_SC8P_MASK      //配置电容
                  | OSC_CR_SC16P_MASK       //配置电容
                 );

    MCG_C2 =    ( 0
                | MCG_C2_RANGE0(2)
                | MCG_C2_EREFS0_MASK
                );

    MCG_C1 = (0
              | MCG_C1_CLKS(2)
              | MCG_C1_FRDIV(7)
              | MCG_C1_IRCLKEN_MASK
              );

    while (MCG_S & MCG_S_IREFST_MASK) {};                       //等待FLL参考时钟 为 外部参考时钟（S[IREFST]=0，表示使用外部参考时钟，）

    while ((MCG_S & MCG_S_CLKST_MASK)  != MCG_S_CLKST(0x2)) {}; //等待选择外部参考时钟

    //现在已经进入了 FBE模式

    //FBE -> PBE


    MCG_C5 = MCG_C5_PRDIV0(mcg_cfg[pll].prdiv);                      //分频， EXTAL_IN_MHz/( PRDIV+1)
    MCG_C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV0(mcg_cfg[pll].vdiv) ;    //倍频， EXTAL_IN_MHz/( PRDIV+1)  * (VDIV+24)

    while (!(MCG_S & MCG_S_PLLST_MASK)) {};                         //等待时钟源选择PLL

    while (!(MCG_S & MCG_S_LOCK0_MASK)) {};                          //等待 PLL锁了（锁相环）

    // 现在已经进入了 PBE 模式

    // PBE -> PEE
    //MCG_C1 &= ~MCG_C1_CLKS_MASK;
    MCG_C1 = MCG_C1_IRCLKEN_MASK;

    while (((MCG_S & MCG_S_CLKST_MASK) ) != MCG_S_CLKST(0x3)) {};//等待选择输出PLL

    // 现在已经进入了 PEE 模式
    
    SIM_SOPT2 |= (0         //选择 PLL时钟
              | SIM_SOPT2_PLLFLLSEL_MASK
             );

    return mcg_cfg[pll].clk;
} //pll_init


