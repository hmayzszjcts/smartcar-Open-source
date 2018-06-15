/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MKL_TPM.c
 * @brief      TPM定时器函数库
 * @author     山外科技
 * @version    v5.0
 * @date       2013-08-22
 */

/*
 * 包含头文件
 */
#include "common.h"
#include  "MKL_TPM.h"

/*
 * 定义数组
 */
TPM_MemMapPtr TPMN[3] = {TPM0_BASE_PTR, TPM1_BASE_PTR, TPM2_BASE_PTR}; //定义三个指针数组保存 TPMn_e 的地址


void tpm_port_mux(TPMn_e tpmn, TPM_CHn_e ch)
{
    switch(tpmn)
    {
    case TPM0:
        SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK;       //使能TPM0时钟
        switch(ch)
        {
        case TPM_CH0:
            if((TPM0_CH0_PIN == PTC1) || (TPM0_CH0_PIN == PTD0) )
            {
                port_init(TPM0_CH0_PIN, ALT4);
            }
            else if((TPM0_CH0_PIN == PTE24) || (TPM0_CH0_PIN == PTA3) )
            {
                port_init(TPM0_CH0_PIN, ALT3);
            }
            else
            {
                ASSERT(0);                      //设置管脚有误？
            }
            break;

        case TPM_CH1:
            if((TPM0_CH1_PIN == PTC2) || (TPM0_CH1_PIN == PTD1))
            {
                port_init(TPM0_CH1_PIN, ALT4);
            }
            else if((TPM0_CH1_PIN == PTA4) || (TPM0_CH1_PIN == PTE25) )
            {
                port_init(TPM0_CH1_PIN, ALT3);
            }
            else
            {
                ASSERT(0);                      //设置管脚有误？
            }
            break;

        case TPM_CH2:
            if((TPM0_CH2_PIN == PTC3) || (TPM0_CH2_PIN == PTD2) )
            {
                port_init(TPM0_CH2_PIN, ALT4);
            }
            else if((TPM0_CH2_PIN == PTA5) || (TPM0_CH2_PIN == PTE29) )
            {
                port_init(TPM0_CH2_PIN, ALT3);
            }
            else
            {
                ASSERT(0);                      //设置管脚有误？
            }
            break;

        case TPM_CH3:
            if((TPM0_CH3_PIN == PTC4) || (TPM0_CH3_PIN == PTD3) )
            {
                port_init(TPM0_CH3_PIN, ALT4);
            }
            else if((TPM0_CH3_PIN == PTA6) || (TPM0_CH3_PIN == PTE30))
            {
                port_init(TPM0_CH3_PIN, ALT3);
            }
            else
            {
                ASSERT(0);                      //设置管脚有误？
            }
            break;

        case TPM_CH4:
            if(TPM0_CH4_PIN == PTD4)
            {
                port_init(TPM0_CH4_PIN, ALT4);
            }
            else if((TPM0_CH4_PIN == PTC8) || (TPM0_CH4_PIN == PTA7) || (TPM0_CH4_PIN == PTE31) )
            {
                port_init(TPM0_CH4_PIN, ALT3);
            }
            else
            {
                ASSERT(0);                      //设置管脚有误？
            }
            break;

        case TPM_CH5:
            if(TPM0_CH5_PIN == PTD5)
            {
                port_init(TPM0_CH5_PIN, ALT4);
            }
            else if((TPM0_CH5_PIN == PTA0)  || (TPM0_CH5_PIN == PTC9)  ||   (TPM0_CH5_PIN == PTE26)    )
            {
                port_init(TPM0_CH5_PIN, ALT3);
            }
            else
            {
                ASSERT(0);                      //设置管脚有误？
            }
            break;

        default:
            return;
        }
        break;

    case TPM1:
        SIM_SCGC6 |= SIM_SCGC6_TPM1_MASK;       //使能TPM1时钟
        switch(ch)
        {
        case TPM_CH0:
            if((TPM1_CH0_PIN == PTE20) || (TPM1_CH0_PIN == PTA12) || (TPM1_CH0_PIN == PTB0) )
            {
                port_init(TPM1_CH0_PIN, ALT3);
            }
            else
            {
                ASSERT(0);                      //设置管脚有误？
            }
            break;


        case TPM_CH1:
            if((TPM1_CH1_PIN == PTE21) || (TPM1_CH1_PIN == PTA13) || (TPM1_CH1_PIN == PTB1) )
            {
                port_init(TPM1_CH1_PIN, ALT3);
            }
            else
            {
                ASSERT(0);                      //设置管脚有误？
            }
            break;

        default:
            return;
        }
        break;

    case TPM2:
        SIM_SCGC6 |= SIM_SCGC6_TPM2_MASK;                           //使能TPM2时钟
        switch(ch)
        {
        case TPM_CH0:
            if((TPM2_CH0_PIN == PTA1) || (TPM2_CH0_PIN == PTB2) || (TPM2_CH0_PIN == PTB18) || (TPM2_CH0_PIN == PTE22) )
            {
                port_init(TPM2_CH0_PIN, ALT3);
            }
            else
            {
                ASSERT(0);                      //设置管脚有误？
            }
            break;

        case TPM_CH1:
            if((TPM2_CH1_PIN == PTA2) || (TPM2_CH1_PIN == PTB3)|| (TPM2_CH1_PIN == PTB19)|| (TPM2_CH1_PIN == PTE23))
            {
                port_init(TPM2_CH1_PIN, ALT3);
            }
            else
            {
                ASSERT(0);                      //设置管脚有误？
            }
            break;

        default:
            return;
        }
        break;
    default:
        break;
    }
}

/*!
 *  @brief      初始化TPM 的PWM 功能
 *  @param      TPMn_e    模块号（TPM0、  TPM1、  TPM2）
 *  @param      TPM_CHn_e     通道号（CH0~CH7）
 *  @param      freq    频率（单位为Hz）
 *  @param      duty    占空比分子，占空比 = duty / TPMn_PRECISON
 *  @since      v5.0
 *  @note       同一个TPM，PWM频率是必须一样的，但占空比可不一样。共3个TPM，即可以输出3个不同频率PWM
 *  Sample usage:       tpm_pwm_init(TPM0, TPM_CH6,200, 10);    //初始化 TPM0_CH6 为 频率 200Hz 的PWM，占空比为 10/TPM0_PRECISON
 */
void tpm_pwm_init(TPMn_e tpmn, TPM_CHn_e ch, uint32 freq, uint32 duty)
{
    uint32 clk_hz ;
    uint16 mod;
    uint8  ps;
    uint16 cv;

    ASSERT( ((tpmn == TPM0)&& (ch <= TPM_CH5)) || ( (tpmn == TPM1 || tpmn == TPM2 ) && (ch <= TPM_CH1))   );  //检查传递进来的通道是否正确

    /******************* 开启时钟 和 复用IO口*******************/
    tpm_port_mux(tpmn,ch);

    /*       计算频率设置        */
    //  若 CPWMS = 1 ，即双边捕捉脉冲，则 PMW频率 =  pll频率/2 /2 /(2^预分频因子)/模数
    //  若 CPWMS = 0 ，即单边捕捉脉冲，则 PMW频率 =  pll频率/2    /(2^预分频因子)/模数
    //  EPWM的周期 ：MOD - CNTIN + 0x0001   (CNTIN 设为0)
    //  脉冲宽度：CnV - CNTIN

    //  模数 MOD < 0x10000
    //  预分频因子 PS  < 0x07
    //  预分频因子 PS 越小时，模数 mod 就越大，计数就越精准，PWM输出更为准确
    //  MOD  = clk_hz/(freq*(1 << PS)) < 0x10000  ==>  clk_hz/(freq*0x10000) < (1<< PS)  ==>  (clk_hz/(freq*0x10000) >> PS) < 1
    //  即 (((clk_hz/0x10000 )/ freq ) >> PS ) < 1

    // 以 CPWMS = 1 ，即双边捕捉脉冲为例
    clk_hz = (pll_clk_mhz * 1000 * 1000) >> 1 ; // pll频率 / 2

    mod = (clk_hz >> 16 ) / freq ;      // 临时用 mod 缓存一下
    ps = 0;
    while((mod >> ps) >= 1)             // 等 (mod >> ps) < 1 才退出 while 循环 ，即求 PS 的最小值
    {
        ps++;
    }

    ASSERT(ps <= 0x07);         // 断言， PS 最大为 0x07 ，超过此值，则 PWM频率设置过低，或 Bus 频率过高

    mod = (clk_hz >> ps) / freq;// 求 MOD 的值

    switch(tpmn)                // 初值 CNTIN 设为0 ，脉冲宽度：CnV - CNTIN ，即 CnV 就是 脉冲宽度了。
    {
        // EPWM的周期 ： MOD - CNTIN + 0x0001 == MOD - 0 + 1
        // 则 CnV = (MOD - 0 + 1) * 占空比 = (MOD - 0 + 1) * duty/ TPM_PRECISON
    case TPM0:
        cv = (duty * (mod - 0 + 1)) / TPM0_PRECISON;
        break;

    case TPM1:
        cv = (duty * (mod - 0 + 1)) / TPM1_PRECISON;
        break;

    case TPM2:
        cv = (duty * (mod - 0 + 1)) / TPM2_PRECISON;
        break;

    default:
        break;
    }

    /******************** 选择输出模式为 边沿对齐PWM *******************/
    //通道状态控制，根据模式来选择 边沿或电平
    TPM_CnSC_REG(TPMN[tpmn], ch) &= ~TPM_CnSC_ELSA_MASK;
    TPM_CnSC_REG(TPMN[tpmn], ch)  = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK;
    // MSnB:MSnA = 1x       边沿对齐PWM
    // ELSnB:ELSnA = 10     先高后低
    // ELSnB:ELSnA = 11     先低后高

    /******************** 配置时钟和分频 ********************/
    SIM_SOPT2 &= ~ SIM_SOPT2_TPMSRC_MASK;
    SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1);        //     00 Clock disabled  01 MCGFLLCLK clock, or MCGPLLCLK/2  10 OSCERCLK clock   11 MCGIRCLK clock

    TPM_SC_REG(TPMN[tpmn])    = ( 0
                                  //| TPM_SC_CPWMS_MASK         //0：上升沿计数模式 ，1： 跳变沿计数模式选择 （注释了表示 0）
                                  | TPM_SC_PS(ps)             //分频因子，分频系数 = 2^PS
                                  | TPM_SC_CMOD(1)
                                );
    TPM_MOD_REG(TPMN[tpmn])   = mod;                        //模数, EPWM的周期为 ：MOD - CNTIN + 0x0001
    TPM_CnV_REG(TPMN[tpmn], ch) = cv;
    TPM_CNT_REG(TPMN[tpmn])   = 0;                          //计数器。只有低16位可用（写任何值到此寄存器，都会清0）
}

/*!
 *  @brief      设置TPM 的PWM 通道占空比
 *  @param      TPMn_e    模块号（TPM0、  TPM1、  TPM2）
 *  @param      TPM_CHn_e     通道号（CH0~CH7）
 *  @param      duty    占空比分子，占空比 = duty / TPMn_PRECISON
 *  @since      v5.0
 *  @note       同一个TPM，PWM频率是必须一样的，但占空比可不一样。共3个TPM，即可以输出3个不同频率PWM
 *  Sample usage:       tpm_pwm_duty(TPM0, TPM_CH6, 10);    //设置 TPM0_CH6占空比为 10/TPM0_PRECISON
 */
void tpm_pwm_duty(TPMn_e tpmn, TPM_CHn_e ch, uint32 duty)
{
    uint32 cv;
    uint32 mod = 0;

    ASSERT( ((tpmn == TPM0)&& (ch <= TPM_CH5)) || ( (tpmn == TPM1 || tpmn == TPM2 ) && (ch <= TPM_CH1))   );  //检查传递进来的通道是否正确

    switch(tpmn)
    {
    case TPM0:
        ASSERT(duty <= TPM0_PRECISON);     //用断言检测 占空比是否合理
        break;

    case TPM1:
        ASSERT(duty <= TPM1_PRECISON);     //用断言检测 占空比是否合理
        break;

    case TPM2:
        ASSERT(duty <= TPM2_PRECISON);     //用断言检测 占空比是否合理
        break;

    default:
        break;
    }


    //占空比 = (CnV-CNTIN)/(MOD-CNTIN+1)

    do
    {
        mod = TPM_MOD_REG(TPMN[tpmn]);        //读取 MOD 的值
    }
    while(mod == 0);      //读一次，可能是0 ，需要多读取几次。

    switch(tpmn)
    {
    case TPM0:
        cv = (duty * (mod - 0 + 1)) / TPM0_PRECISON;
        break;

    case TPM1:
        cv = (duty * (mod - 0 + 1)) / TPM1_PRECISON;
        break;

    case TPM2:
        cv = (duty * (mod - 0 + 1)) / TPM2_PRECISON;
        break;
    default:
        break;
    }

    // 配置TPM通道值
    TPM_CnV_REG(TPMN[tpmn], ch) = cv;

}

/*!
 *  @brief      设置TPM的频率
 *  @param      freq    频率（单位为Hz）
 *  @since      v5.0
 *  @note       修改PWM频率后，必须调用 TPM_PWM_Duty 重新配置占空比。同一个模块，PWM频率必须相同。
 *  Sample usage:       tpm_pwm_freq(TPM0,200);    //设置 TPM0 的 频率 为 200Hz
 */
void tpm_pwm_freq(TPMn_e tpmn, uint32 freq)             //设置TPM的频率
{
    uint32 clk_hz ;
    uint32 mod;
    uint8 ps;


    /*       计算频率设置        */
    // 以 CPWMS = 1 ，即双边捕捉脉冲为例
    clk_hz = (pll_clk_mhz * 1000 * 1000) >> 1 ; // pll频率 / 2

    mod = (clk_hz >> 16 ) / freq ;      // 临时用 mod 缓存一下
    ps = 0;
    while((mod >> ps) >= 1)             // 等 (mod >> ps) < 1 才退出 while 循环 ，即求 PS 的最小值
    {
        ps++;
    }

    ASSERT(ps <= 0x07);         // 断言， PS 最大为 0x07 ，超过此值，则 PWM频率设置过低，或 Bus 频率过高

    mod = (clk_hz >> ps) / freq;// 求 MOD 的值


    TPM_SC_REG(TPMN[tpmn])    = ( 0
                                  //| TPM_SC_CPWMS_MASK         //0：上升沿计数模式 ，1： 跳变沿计数模式选择 （注释了表示 0）
                                  | TPM_SC_PS(ps)             //分频因子，分频系数 = 2^PS
                                  | TPM_SC_CMOD(1)
                                  //| TPM_SC_CLKS(1)            //时钟选择， 0：没选择时钟，禁用； 1：bus 时钟； 2：MCGFFCLK； 3：EXTCLK（ 由SIM_SOPT4 选择输入管脚 TPM_CLKINx）
                                  //| TPM_SC_TOIE_MASK        //溢出中断使能（注释了表示 禁止溢出中断）
                                );
    TPM_MOD_REG(TPMN[tpmn])   = mod;                        //模数, EPWM的周期为 ：MOD - CNTIN + 0x0001
    TPM_CNT_REG(TPMN[tpmn])   = 0;                          //计数器。只有低16位可用（写任何值到此寄存器，都会加载 CNTIN 的值）
}

//////////////////////////////// 以上为输出PWM  //////////////////////////////////////////

/*!
 *  @brief      输入捕捉初始化函数
 *  @param      TPMn_e          模块号（TPM0、  TPM1、  TPM2）
 *  @param      TPM_CHn_e       通道号（CH0~CH7）
 *  @param      TPM_Input_cfg   输入捕捉触发条件配置
 *  @param      TPM_PS_e        分频因子
 *  @since      v5.2
 *  @note       同一个TPM，只能干一个活，用于输入捕捉后就不要再用于其他功能，例如PWM。
 *  Sample usage:       tpm_input_init(TPM0, TPM_CH0, TPM_Rising,TPM_PS_2);    //设置 TPM0_CH0上升沿触发输入捕捉，二分频
 */
void tpm_input_init(TPMn_e tpmn, TPM_CHn_e ch, TPM_Input_cfg cfg,TPM_PS_e ps)
{
    ASSERT( tpmn < TPM_MAX );  //检查传递进来的通道是否正确

    /******************* 开启时钟 和 复用IO口*******************/
    tpm_port_mux(tpmn,ch);

    /******************* 设置为输入捕捉功能 *******************/
    switch(cfg)
    {
        //输入捕捉模式下：DECAPEN = 0 ， DECAPEN = 0 ，CPWMS = 0， MSnB:MSnA = 0

        // ELSnB:ELSnA         1          10          11
        // 配置             上升沿      下降沿      跳变沿

    case TPM_Rising:    //上升沿触发
        TPM_CnSC_REG(TPMN[tpmn], ch) |=  ( TPM_CnSC_ELSA_MASK  | TPM_CnSC_CHIE_MASK );                   //置1
        TPM_CnSC_REG(TPMN[tpmn], ch) &= ~( TPM_CnSC_ELSB_MASK  | TPM_CnSC_MSB_MASK | TPM_CnSC_MSA_MASK); //清0
        break;

    case TPM_Falling:   //下降沿触发
        TPM_CnSC_REG(TPMN[tpmn], ch) |= (TPM_CnSC_ELSB_MASK  | TPM_CnSC_CHIE_MASK );                    //置1
        TPM_CnSC_REG(TPMN[tpmn], ch) &= ~( TPM_CnSC_ELSA_MASK | TPM_CnSC_MSB_MASK | TPM_CnSC_MSA_MASK); //清0
        break;

    case TPM_Rising_or_Falling: //上升沿、下降沿都触发
        TPM_CnSC_REG(TPMN[tpmn], ch) |=  ( TPM_CnSC_ELSB_MASK | TPM_CnSC_ELSA_MASK  | TPM_CnSC_CHIE_MASK ); //置1
        TPM_CnSC_REG(TPMN[tpmn], ch) &= ~( TPM_CnSC_MSB_MASK  | TPM_CnSC_MSA_MASK); //清0
        break;
    }

    /******************** 配置时钟和分频 ********************/
    SIM_SOPT2 &= ~ SIM_SOPT2_TPMSRC_MASK;
    SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1);        //     00 Clock disabled  01 MCGFLLCLK clock, or MCGPLLCLK/2  10 OSCERCLK clock   11 MCGIRCLK clock

    TPM_SC_REG(TPMN[tpmn]) = ( 0
                              //| TPM_SC_CPWMS_MASK     //输入捕捉模式，此值需要为0
                              | TPM_SC_CMOD(0x1)       //选择时钟模式
                              | TPM_SC_PS(ps)          //选择 分频系数
                             );

    TPM_CNT_REG(TPMN[tpmn])     = 0;
    //TPM_CnV_REG(TPMN[tpmn],ch)     = 0;

    TPM_STATUS_REG(TPMN[tpmn])  = ~0;               //清中断标志位
    TPM_STATUS_REG(TPMN[tpmn])  = ~0;

    //开启输入捕捉中断
    //enable_irq(TPM0_IRQn + tpmn);
}

/*!
 *  @brief      输入捕捉获取捕捉事件的时间
 *  @param      TPMn_e          模块号（TPM0、  TPM1、  TPM2）
 *  @param      TPM_CHn_e       通道号（CH0~CH7）
 *  @return     获取捕捉事件发生时的计数器值 (根据两次捕捉之间的差值可判断周期频率)
 *  @since      v5.2
 *  Sample usage:       uint16 data = tpm_input_get (TPM0, TPM_CH0) ;   // 获取TPM0_CH0输入捕捉事件触发时的计数值
 */
uint16   tpm_input_get (TPMn_e tpmn, TPM_CHn_e ch)
{
    ASSERT( tpmn < TPM_MAX );  //检查传递进来的通道是否正确

    return (uint16)TPM_CnV_REG(TPMN[tpmn],ch);    //保存
}

/*!
 *  @brief      输入捕捉，清空计数器的值
 *  @param      TPMn_e          模块号（TPM0、  TPM1、  TPM2）
 *  @since      v5.0
 */
void     tpm_input_clean (TPMn_e tpmn)
{
    ASSERT( tpmn < TPM_MAX);     //检查传递进来的通道是否正确

    TPM_CNT_REG(TPMN[tpmn]) = 0;               //清 计数器计数值
}



//////////////////////////////// 以下为脉冲计数  //////////////////////////////////////////
/*!
 *  @brief      脉冲计数初始化函数
 *  @param      TPMn_e          模块号（TPM0、  TPM1、  TPM2）
 *  @param      TPM_CLKIN_e     脉冲输入管脚
 *  @param      TPM_PS_e        分频因子
 *  @since      v5.2
 *  @note       同一个TPM，只能干一个活，用于脉冲计数后就不要再用于其他功能，例如PWM、输入捕捉。
 *  Sample usage:       tpm_pulse_init(TPM0,TPM_CLKIN1,TPM_PS_2);     //TPM0初始化为脉冲计数，脉冲输入管脚为 TPM_CLKIN1_PIN ，进行二分频计数。
 */
void tpm_pulse_init(TPMn_e tpmn,TPM_CLKIN_e clkin,TPM_PS_e ps)
{
    ASSERT(ps < TPM_PS_MAX);

    //配置 复用
    switch(tpmn)
    {
    case TPM0:
        SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK;       //使能TPM0时钟


        break;
    case TPM1:
        SIM_SCGC6 |= SIM_SCGC6_TPM1_MASK;       //使能TPM1时钟
        break;

    case TPM2:
        SIM_SCGC6 |= SIM_SCGC6_TPM2_MASK;       //使能TPM2时钟
        break;
    default:
        ASSERT(0);
    }

    if(clkin == TPM_CLKIN0)
    {
        SIM_SOPT4 &= ~( SIM_SOPT4_TPM0CLKSEL_MASK << tpmn );
        port_init(TPM_CLKIN0_PIN,ALT4);
    }
    else if(clkin == TPM_CLKIN1)
    {
        SIM_SOPT4 |= ( SIM_SOPT4_TPM0CLKSEL_MASK << tpmn );
        port_init(TPM_CLKIN1_PIN,ALT4);
    }
    else
    {
        ASSERT(0);              //断言，clkin 只能 0 和 1
    }

    /******************** 配置时钟和分频 ********************/
    SIM_SOPT2 &= ~ SIM_SOPT2_TPMSRC_MASK;
    SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1);        //     00 Clock disabled  01 MCGFLLCLK clock, or MCGPLLCLK/2  10 OSCERCLK clock   11 MCGIRCLK clock

    TPM_SC_REG(TPMN[tpmn])    = ( 0
                                  //| TPM_SC_CPWMS_MASK         //0：上升沿计数模式 ，1： 跳变沿计数模式选择 （注释了表示 0）
                                  | TPM_SC_PS(ps)             //分频因子，分频系数 = 2^PS
                                  | TPM_SC_CMOD(2)
                               );
    TPM_MOD_REG(TPMN[tpmn])   = 0xFFFF;
    TPM_CNT_REG(TPMN[tpmn])   = 0;                          //计数器。只有低16位可用（写任何值到此寄存器，都会清0）
}

/*!
 *  @brief      清空脉冲计数值
 *  @param      TPMn_e          模块号（TPM0、  TPM1、  TPM2）
 *  @since      v5.2
 */
void     tpm_pulse_clean (TPMn_e tpmn)
{
    TPM_CNT_REG(TPMN[tpmn])   = 0;                          //计数器。只有低16位可用（写任何值到此寄存器，都会清0）
}

/*!
 *  @brief      获取脉冲计数值
 *  @param      TPMn_e          模块号（TPM0、  TPM1、  TPM2）
 *  @since      v5.2
 *  Sample usage:     uint16 data = tpm_pulse_get(TPM0);     //获取TPM0的脉冲计数值。
 */
uint16     tpm_pulse_get (TPMn_e tpmn)
{
    return (uint16)TPM_CNT_REG(TPMN[tpmn]);                          //计数器。只有低16位可用（写任何值到此寄存器，都会清0）
}

