/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_ftm.c
 * @brief      FTM定时器函数库
 * @author     山外科技
 * @version    v5.2
 * @date       2014-10-31
 */


/*
 * 包含头文件
 */
#include "common.h"
#include  "MK60_FTM.h"

/*
 * 定义数组
 */
FTM_MemMapPtr FTMN[FTM_MAX] = {FTM0_BASE_PTR, FTM1_BASE_PTR, FTM2_BASE_PTR
#ifdef MK60F15
, FTM3_BASE_PTR         //FX 才有FTM3
#endif
}; //定义三个指针数组保存 FTMn_e 的地址

static uint32 ftm_mod[FTM_MAX];

/*!
 *  @brief      初始化FTM 的端口
 *  @param      FTMn_e    模块号（FTM0、  FTM1、  FTM2）
 *  @param      FTM_CHn_e     通道号（CH0~CH7）
 *  @since      v5.0
 */
static void FTM_port_mux(FTMn_e ftmn, FTM_CHn_e ch)
{
    /******************* 开启时钟 和 复用IO口*******************/
    switch(ftmn)
    {
    case FTM0:
        SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK;       //使能FTM0时钟
        switch(ch)
        {
        case FTM_CH0:
            if(FTM0_CH0_PIN == PTC1)
            {
                port_init(FTM0_CH0_PIN, ALT4);
            }
            else if(FTM0_CH0_PIN == PTA3)
            {
                port_init(FTM0_CH0_PIN, ALT3);
            }
            else
            {
                ASSERT(0);                      //设置管脚有误？
            }
            break;

        case FTM_CH1:
            if(FTM0_CH1_PIN == PTC2)
            {
                port_init(FTM0_CH1_PIN, ALT4);
            }
            else if(FTM0_CH1_PIN == PTA4)
            {
                port_init(FTM0_CH1_PIN, ALT3);
            }
            else
            {
                ASSERT(0);                      //设置管脚有误？
            }
            break;

        case FTM_CH2:
            if(FTM0_CH2_PIN == PTC3)
            {
                port_init(FTM0_CH2_PIN, ALT4);
            }
            else if(FTM0_CH2_PIN == PTA5)
            {
                port_init(FTM0_CH2_PIN, ALT3);
            }
            else
            {
                ASSERT(0);                      //设置管脚有误？
            }
            break;

        case FTM_CH3:
            if(FTM0_CH3_PIN == PTC4)
            {
                port_init(FTM0_CH3_PIN, ALT4);
            }
            else if(FTM0_CH3_PIN == PTA6)
            {
                port_init(FTM0_CH3_PIN, ALT3);
            }
            else
            {
                ASSERT(0);                      //设置管脚有误？
            }
            break;

        case FTM_CH4:
            if(FTM0_CH4_PIN == PTD4)
            {
                port_init(FTM0_CH4_PIN, ALT4);
            }
            else if(FTM0_CH4_PIN == PTA7)
            {
                port_init(FTM0_CH4_PIN, ALT3);
            }
            else
            {
                ASSERT(0);                      //设置管脚有误？
            }
            break;

        case FTM_CH5:
            if(FTM0_CH5_PIN == PTD5)
            {
                port_init(FTM0_CH5_PIN, ALT4);
            }
            else if(FTM0_CH5_PIN == PTA0)
            {
                port_init(FTM0_CH5_PIN, ALT3);
            }
            else
            {
                ASSERT(0);                      //设置管脚有误？
            }
            break;

        case FTM_CH6:
            if(FTM0_CH6_PIN == PTD6)
            {
                port_init(FTM0_CH6_PIN, ALT4);
            }
            else if(FTM0_CH6_PIN == PTA1)
            {
                port_init(FTM0_CH6_PIN, ALT3);
            }
            else
            {
                ASSERT(0);                      //设置管脚有误？
            }
            break;

        case FTM_CH7:
            if(FTM0_CH7_PIN == PTD7)
            {
                port_init(FTM0_CH7_PIN, ALT4);
            }
            else if(FTM0_CH7_PIN == PTA2)
            {
                port_init(FTM0_CH7_PIN, ALT3);
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

    case FTM1:
        SIM_SCGC6 |= SIM_SCGC6_FTM1_MASK;       //使能FTM1时钟
        switch(ch)
        {
        case FTM_CH0:
            if((FTM1_CH0_PIN == PTA8) || (FTM1_CH0_PIN == PTA12) || (FTM1_CH0_PIN == PTB0) )
            {
                port_init(FTM1_CH0_PIN, ALT3);
            }
            else
            {
                ASSERT(0);                      //设置管脚有误？
            }
            break;


        case FTM_CH1:
            if((FTM1_CH1_PIN == PTA9) || (FTM1_CH1_PIN == PTA13) || (FTM1_CH1_PIN == PTB1) )
            {
                port_init(FTM1_CH1_PIN, ALT3);
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

    case FTM2:
        SIM_SCGC3 |= SIM_SCGC3_FTM2_MASK;                           //使能FTM2时钟
        switch(ch)
        {
        case FTM_CH0:
            if((FTM2_CH0_PIN == PTA10) || (FTM2_CH0_PIN == PTB18) )
            {
                port_init(FTM2_CH0_PIN, ALT3);
            }
            else
            {
                ASSERT(0);                      //设置管脚有误？
            }
            break;

        case FTM_CH1:
            if((FTM2_CH1_PIN == PTA11) || (FTM2_CH1_PIN == PTB19))
            {
                port_init(FTM2_CH1_PIN, ALT3);
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
#ifdef MK60F15
    case FTM3:
        SIM_SCGC3 |= SIM_SCGC3_FTM3_MASK;       //使能FTM3时钟
        switch(ch)
        {
        case FTM_CH0:
            if(FTM3_CH0_PIN == PTD0)
            {
                port_init(FTM3_CH0_PIN, ALT4);
            }
            else if(FTM3_CH0_PIN == PTE5)
            {
                port_init(FTM3_CH0_PIN, ALT6);
            }
            else
            {
                ASSERT(0);                      //设置管脚有误？
            }
            break;

        case FTM_CH1:
            if(FTM3_CH1_PIN == PTD1)
            {
                port_init(FTM3_CH1_PIN, ALT4);
            }
            else if(FTM3_CH1_PIN == PTE6)
            {
                port_init(FTM3_CH1_PIN, ALT6);
            }
            else
            {
                ASSERT(0);                      //设置管脚有误？
            }
            break;

        case FTM_CH2:
            if(FTM3_CH2_PIN == PTD2)
            {
                port_init(FTM3_CH2_PIN, ALT4);
            }
            else if(FTM3_CH2_PIN == PTE7)
            {
                port_init(FTM3_CH2_PIN, ALT6);
            }
            else
            {
                ASSERT(0);                      //设置管脚有误？
            }
            break;

        case FTM_CH3:
            if(FTM3_CH3_PIN == PTD3)
            {
                port_init(FTM3_CH3_PIN, ALT4);
            }
            else if(FTM3_CH3_PIN == PTE8)
            {
                port_init(FTM3_CH3_PIN, ALT6);
            }
            else
            {
                ASSERT(0);                      //设置管脚有误？
            }
            break;

        case FTM_CH4:
            if(FTM3_CH4_PIN == PTC8)
            {
                port_init(FTM3_CH4_PIN, ALT3);
            }
            else if(FTM3_CH4_PIN == PTE9)
            {
                port_init(FTM3_CH4_PIN, ALT6);
            }
            else
            {
                ASSERT(0);                      //设置管脚有误？
            }
            break;

        case FTM_CH5:
            if(FTM3_CH5_PIN == PTC9)
            {
                port_init(FTM3_CH5_PIN, ALT3);
            }
            else if(FTM3_CH5_PIN == PTE10)
            {
                port_init(FTM3_CH5_PIN, ALT6);
            }
            else
            {
                ASSERT(0);                      //设置管脚有误？
            }
            break;

        case FTM_CH6:
            if(FTM3_CH6_PIN == PTC10)
            {
                port_init(FTM3_CH6_PIN, ALT3);
            }
            else if(FTM3_CH6_PIN == PTE11)
            {
                port_init(FTM3_CH6_PIN, ALT6);
            }
            else
            {
                ASSERT(0);                      //设置管脚有误？
            }
            break;

        case FTM_CH7:
            if(FTM3_CH7_PIN == PTC11)
            {
                port_init(FTM3_CH7_PIN, ALT3);
            }
            else if(FTM3_CH7_PIN == PTE12)
            {
                port_init(FTM3_CH7_PIN, ALT6);
            }
            else
            {
                ASSERT(0);                      //设置管脚有误？
            }
        }
        break;

#endif
    default:
        break;
    }
}

/*!
 *  @brief      初始化FTM 的PWM 功能
 *  @param      FTMn_e    模块号（FTM0、  FTM1、  FTM2）
 *  @param      FTM_CHn_e     通道号（CH0~CH7）
 *  @param      freq    频率（单位为Hz）
 *  @param      duty    占空比分子，占空比 = duty / FTMn_PRECISON
 *  @since      v5.0
 *  @note       同一个FTM，PWM频率是必须一样的，但占空比可不一样。共3个FTM，即可以输出3个不同频率PWM
 *  Sample usage:       ftm_pwm_init(FTM0, FTM_CH6,200, 10);    //初始化 FTM0_CH6 为 频率 200Hz 的PWM，占空比为 10/FTM0_PRECISON
 */
void ftm_pwm_init(FTMn_e ftmn, FTM_CHn_e ch, uint32 freq, uint32 duty)
{
    uint32 clk_hz ;
    uint16 mod;
    uint8  ps;
    uint16 cv;

    ASSERT( (ftmn == FTM0) || ( (ftmn == FTM1 || ftmn == FTM2 ) && (ch <= FTM_CH1))
#ifdef MK60F15
            || (ftmn == FTM3)       //FX  有FTM3
#endif
           );  //检查传递进来的通道是否正确

    /******************* 开启时钟 和 复用IO口*******************/
    FTM_port_mux(ftmn,ch);

    /*       计算频率设置        */
    //  若 CPWMS = 1 ，即双边捕捉脉冲，则 PMW频率 =  bus频率 /2 /(2^预分频因子)/模数
    //  若 CPWMS = 0 ，即单边捕捉脉冲，则 PMW频率 =  bus频率    /(2^预分频因子)/模数
    //  EPWM的周期 ：MOD - CNTIN + 0x0001   (CNTIN 设为0)
    //  脉冲宽度：CnV - CNTIN

    //  模数 MOD < 0x10000
    //  预分频因子 PS  < 0x07
    //  预分频因子 PS 越小时，模数 mod 就越大，计数就越精准，PWM输出更为准确
    //  MOD  = clk_hz/(freq*(1 << PS)) < 0x10000  ==>  clk_hz/(freq*0x10000) < (1<< PS)  ==>  (clk_hz/(freq*0x10000) >> PS) < 1
    //  即 (((clk_hz/0x10000 )/ freq ) >> PS ) < 1

    // 以 CPWMS = 0 ，即单边捕捉脉冲为例
    clk_hz = (bus_clk_khz * 1000) ;     // bus频率

    mod = (clk_hz >> 16 ) / freq ;      // 临时用 mod 缓存一下
    ps = 0;
    while((mod >> ps) >= 1)             // 等 (mod >> ps) < 1 才退出 while 循环 ，即求 PS 的最小值
    {
        ps++;
    }

    ASSERT(ps <= 0x07);                 // 断言， PS 最大为 0x07 ，超过此值，则 PWM频率设置过低，或 Bus 频率过高

    mod = (clk_hz >> ps) / freq;        // 求 MOD 的值

    ftm_mod[ftmn]=mod;

    switch(ftmn)                        // 初值 CNTIN 设为0 ，脉冲宽度：CnV - CNTIN ，即 CnV 就是 脉冲宽度了。
    {
        // EPWM的周期 ： MOD - CNTIN + 0x0001 == MOD - 0 + 1
        // 则 CnV = (MOD - 0 + 1) * 占空比 = (MOD - 0 + 1) * duty/ FTM_PRECISON
    case FTM0:
        cv = (duty * (mod - 0 + 1)) / FTM0_PRECISON;
        break;

    case FTM1:
        cv = (duty * (mod - 0 + 1)) / FTM1_PRECISON;
        break;

    case FTM2:
        cv = (duty * (mod - 0 + 1)) / FTM2_PRECISON;
        break;
#ifdef MK60F15
    case FTM3:
        cv = (duty * (mod - 0 + 1)) / FTM3_PRECISON;
        break;
#endif

    default:
        break;
    }

    /******************** 选择输出模式为 边沿对齐PWM *******************/
    //通道状态控制，根据模式来选择 边沿或电平
    FTM_CnSC_REG(FTMN[ftmn], ch) &= ~FTM_CnSC_ELSA_MASK;
    FTM_CnSC_REG(FTMN[ftmn], ch)  = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;
    // MSnB:MSnA = 1x       边沿对齐PWM
    // ELSnB:ELSnA = 10     先高后低
    // ELSnB:ELSnA = 11     先低后高

    /******************** 配置时钟和分频 ********************/
    FTM_SC_REG(FTMN[ftmn])    = ( 0
                                  //| FTM_SC_CPWMS_MASK         //0：上升沿计数模式 ，1： 跳变沿计数模式选择 （注释了表示 0）
                                  | FTM_SC_PS(ps)             //分频因子，分频系数 = 2^PS
                                  | FTM_SC_CLKS(1)            //时钟选择， 0：没选择时钟，禁用； 1：bus 时钟； 2：MCGFFCLK； 3：EXTCLK（ 由SIM_SOPT4 选择输入管脚 FTM_CLKINx）
                                  //| FTM_SC_TOIE_MASK        //溢出中断使能（注释了表示 禁止溢出中断）
                                );
    FTM_MOD_REG(FTMN[ftmn])   = mod;                        //模数, EPWM的周期为 ：MOD - CNTIN + 0x0001
    FTM_CNTIN_REG(FTMN[ftmn]) = 0;                          //计数器初始化值。设置脉冲宽度：(CnV - CNTIN).
    FTM_CnV_REG(FTMN[ftmn], ch) = cv;
    FTM_CNT_REG(FTMN[ftmn])   = 0;                          //计数器。只有低16位可用（写任何值到此寄存器，都会加载 CNTIN 的值）
}

/*!
 *  @brief      设置FTM 的PWM 通道占空比
 *  @param      FTMn_e    模块号（FTM0、  FTM1、  FTM2）
 *  @param      FTM_CHn_e     通道号（CH0~CH7）
 *  @param      duty    占空比分子，占空比 = duty / FTMn_PRECISON
 *  @since      v5.0
 *  @note       同一个FTM，PWM频率是必须一样的，但占空比可不一样。共3个FTM，即可以输出3个不同频率PWM
 *  Sample usage:       ftm_pwm_duty(FTM0, FTM_CH6, 10);    //设置 FTM0_CH6占空比为 10/FTM0_PRECISON
 */
void ftm_pwm_duty(FTMn_e ftmn, FTM_CHn_e ch, uint32 duty)
{
    uint32 cv;
    uint32 mod = 0;

    ASSERT( (ftmn == FTM0) || ( (ftmn == FTM1 || ftmn == FTM2 ) && (ch <= FTM_CH1))
#ifdef MK60F15
            || (ftmn == FTM3)       //FX  有FTM3
#endif
           );  //检查传递进来的通道是否正确

    switch(ftmn)
    {
        //如果此处断言失败，说明占空比超过 100% ，务必自行检查是否出现这情况。
    case FTM0:
        ASSERT(duty <= FTM0_PRECISON);     //用断言检测 占空比是否合理
        break;

    case FTM1:
        ASSERT(duty <= FTM1_PRECISON);     //用断言检测 占空比是否合理
        break;

    case FTM2:
        ASSERT(duty <= FTM2_PRECISON);     //用断言检测 占空比是否合理
        break;

#ifdef MK60F15
    case FTM3:
        ASSERT(duty <= FTM3_PRECISON);     //用断言检测 占空比是否合理
        break;
#endif

    default:
        break;
    }


    //占空比 = (CnV-CNTIN)/(MOD-CNTIN+1
    mod=ftm_mod[ftmn];


    switch(ftmn)
    {
    case FTM0:
        cv = (duty * (mod - 0 + 1)) / FTM0_PRECISON;
        break;

    case FTM1:
        cv = (duty * (mod - 0 + 1)) / FTM1_PRECISON;
        break;

    case FTM2:
        cv = (duty * (mod - 0 + 1)) / FTM2_PRECISON;
        break;

#ifdef MK60F15
    case FTM3:
        cv = (duty * (mod - 0 + 1)) / FTM3_PRECISON;
        break;
#endif

    default:
        break;
    }

    // 配置FTM通道值
    FTM_CnV_REG(FTMN[ftmn], ch) = cv;

}

/*!
 *  @brief      设置FTM的频率
 *  @param      freq    频率（单位为Hz）
 *  @since      v5.0
 *  @note       修改PWM频率后，必须调用 ftm_pwm_duty 重新配置占空比。同一个模块，PWM频率必须相同。
 *  Sample usage:       ftm_pwm_freq(FTM0,200);    //设置 FTM0 的 频率 为 200Hz
 */
void ftm_pwm_freq(FTMn_e ftmn, uint32 freq)             //设置FTM的频率
{
    uint32 clk_hz = (bus_clk_khz * 1000) >> 1;        //bus频率/2
    uint32 mod;
    uint8 ps;

    /*       计算频率设置        */
    // 以 CPWMS = 0 ，即单边捕捉脉冲为例
    clk_hz = (bus_clk_khz * 1000) ;     // bus频率

    mod = (clk_hz >> 16 ) / freq ;      // 临时用 mod 缓存一下
    ps = 0;
    while((mod >> ps) >= 1)             // 等 (mod >> ps) < 1 才退出 while 循环 ，即求 PS 的最小值
    {
        ps++;
    }

    ASSERT(ps <= 0x07);                 // 断言， PS 最大为 0x07 ，超过此值，则 PWM频率设置过低，或 Bus 频率过高

    mod = (clk_hz >> ps) / freq;        // 求 MOD 的值

    ftm_mod[ftmn]=mod;

    /******************** 配置时钟和分频 ********************/
    FTM_SC_REG(FTMN[ftmn])    = ( 0
                                  //| FTM_SC_CPWMS_MASK         //0：上升沿计数模式 ，1： 跳变沿计数模式选择 （注释了表示 0）
                                  | FTM_SC_PS(ps)             //分频因子，分频系数 = 2^PS
                                  | FTM_SC_CLKS(1)            //时钟选择， 0：没选择时钟，禁用； 1：bus 时钟； 2：MCGFFCLK； 3：EXTCLK（ 由SIM_SOPT4 选择输入管脚 FTM_CLKINx）
                                  //| FTM_SC_TOIE_MASK        //溢出中断使能（注释了表示 禁止溢出中断）
                                );
    FTM_CNTIN_REG(FTMN[ftmn]) = 0;      //计数器初始化值。设置脉冲宽度：(CnV - CNTIN).
    FTM_MOD_REG(FTMN[ftmn])   = mod;    //模数, EPWM的周期为 ：MOD - CNTIN + 0x0001
    FTM_CNT_REG(FTMN[ftmn])   = 0;      //计数器。只有低16位可用（写任何值到此寄存器，都会加载 CNTIN 的值）
}

//////////////////////////////// 以上为输出PWM  //////////////////////////////////////////

//////////////////////////////// 以下为输入捕捉 //////////////////////////////////////////

/*!
 *  @brief      输入捕捉初始化函数
 *  @param      FTMn_e          模块号（FTM0、  FTM1、  FTM2）
 *  @param      FTM_CHn_e       通道号（CH0~CH7）
 *  @param      FTM_Input_cfg   输入捕捉触发条件配置
 *  @param      FTM_PS_e        分频因子
 *  @since      v5.0
 *  @note       同一个FTM，只能干一个活，用于输入捕捉后就不要再用于其他功能，例如PWM。
 *  Sample usage:       ftm_input_init(FTM0, FTM_CH0, FTM_Rising,FTM_PS_2);    //设置 FTM0_CH0上升沿触发输入捕捉，二分频
 */
void ftm_input_init(FTMn_e ftmn, FTM_CHn_e ch, FTM_Input_cfg cfg,FTM_PS_e ps)
{
    ASSERT( (ftmn == FTM0) || ( (ftmn == FTM1 || ftmn == FTM2 ) && (ch <= FTM_CH1))
#ifdef MK60F15
            || (ftmn == FTM3)       //FX  有FTM3
#endif
           );  //检查传递进来的通道是否正确

    /******************* 开启时钟 和 复用IO口*******************/
    FTM_port_mux(ftmn,ch);

    /******************* 设置为输入捕捉功能 *******************/
    switch(cfg)
    {
        //输入捕捉模式下：DECAPEN = 0 ， DECAPEN = 0 ，CPWMS = 0， MSnB:MSnA = 0

        // ELSnB:ELSnA         1          10          11
        // 配置             上升沿      下降沿      跳变沿

    case FTM_Rising:    //上升沿触发
        FTM_CnSC_REG(FTMN[ftmn], ch) |=  ( FTM_CnSC_ELSA_MASK  | FTM_CnSC_CHIE_MASK );                   //置1
        FTM_CnSC_REG(FTMN[ftmn], ch) &= ~( FTM_CnSC_ELSB_MASK  | FTM_CnSC_MSB_MASK | FTM_CnSC_MSA_MASK); //清0
        break;

    case FTM_Falling:   //下降沿触发
        FTM_CnSC_REG(FTMN[ftmn], ch) |= (FTM_CnSC_ELSB_MASK  | FTM_CnSC_CHIE_MASK );                    //置1
        FTM_CnSC_REG(FTMN[ftmn], ch) &= ~( FTM_CnSC_ELSA_MASK | FTM_CnSC_MSB_MASK | FTM_CnSC_MSA_MASK); //清0
        break;

    case FTM_Rising_or_Falling: //上升沿、下降沿都触发
        FTM_CnSC_REG(FTMN[ftmn], ch) |=  ( FTM_CnSC_ELSB_MASK | FTM_CnSC_ELSA_MASK  | FTM_CnSC_CHIE_MASK ); //置1
        FTM_CnSC_REG(FTMN[ftmn], ch) &= ~( FTM_CnSC_MSB_MASK  | FTM_CnSC_MSA_MASK); //清0
        break;
    }

    FTM_SC_REG(FTMN[ftmn]) = ( 0
                              | FTM_SC_CLKS(0x1)       //选择 bus 时钟
                              | FTM_SC_PS(ps)          //选择 分频系数
                             );
    FTM_MODE_REG(FTMN[ftmn])    |= FTM_MODE_WPDIS_MASK; //禁止写保护
    FTM_COMBINE_REG(FTMN[ftmn]) = 0;
    FTM_MODE_REG(FTMN[ftmn])    &= ~FTM_MODE_FTMEN_MASK;    //使能FTM
    FTM_CNTIN_REG(FTMN[ftmn])   = 0;

    FTM_STATUS_REG(FTMN[ftmn])  = 0x00;               //清中断标志位

    //开启输入捕捉中断
    //enable_irq(FTM0_IRQn + ftmn);
}

/*!
 *  @brief      输入捕捉初始化函数
 *  @param      FTMn_e          模块号（FTM0、  FTM1、  FTM2）
 *  @param      FTM_CHn_e       通道号（CH0~CH7）
 *  @param      FTM_Input_cfg   输入捕捉触发条件配置
 *  @return     获取捕捉事件发生时的计数器值 (根据两次捕捉之间的差值可判断周期频率)
 *  @since      v5.0
 *  Sample usage:       uint16 data = ftm_input_get (FTM0, FTM_CH0) ;   // 获取FTM0_CH0输入捕捉事件触发时的计数值
 */
uint16   ftm_input_get (FTMn_e ftmn, FTM_CHn_e ch)
{
        ASSERT( (ftmn == FTM0) || ( (ftmn == FTM1 || ftmn == FTM2 ) && (ch <= FTM_CH1))
#ifdef MK60F15
                || (ftmn == FTM3)       //FX  有FTM3
#endif
               );  //检查传递进来的通道是否正确

    return (uint16)FTM_CnV_REG(FTMN[ftmn],ch);    //保存
}

void     ftm_input_clean (FTMn_e ftmn)
{
    ASSERT( ftmn < FTM_MAX);     //检查传递进来的通道是否正确

    FTM_CNT_REG(FTMN[ftmn]) = 0;               //清 计数器计数值
}


/*!
 *  @brief      FTM测试中断服务函数
 *  @since      v5.0
 *  @warning    此函数需要用户根据自己需求完成，这里仅仅是提供一个模版
 *  Sample usage:       set_vector_handler(FTM0_IRQn , FTM1_Input_test_handler);    //把 FTM1_Input_test_handler 函数添加到中断向量表，不需要我们手动调用
 */
void FTM1_Input_test_handler(void)
{
    uint8 s = FTM1_STATUS;             //读取捕捉和比较状态  All CHnF bits can be checked using only one read of STATUS.
    uint8 CHn;

    FTM1_STATUS = 0x00;             //清中断标志位

    CHn = 0;
    if( s & (1 << CHn) )
    {
        //FTM_IRQ_DIS(FTM1, CHn);     //禁止输入捕捉中断
        /*     用户任务       */

        /*********************/
        //FTM_IRQ_EN(FTM1, CHn); //开启输入捕捉中断

    }

    /* 这里添加 n=1 的模版，根据模版来添加 */
    CHn = 1;
    if( s & (1 << CHn) )
    {
        //FTM_IRQ_EN(FTM1, CHn); //开启输入捕捉中断
        /*     用户任务       */


        /*********************/
        //不建议在这里开启输入捕捉中断
        //FTM_IRQ_EN(FTM1, CHn); //开启输入捕捉中断
    }
}

//////////////////////////////// 以上为输入捕捉  //////////////////////////////////////////

//////////////////////////////// 以下为正交解码 //////////////////////////////////////////

/*!
 *  @brief      初始化FTM 的正交解码 功能
 *  @param      FTMn_e    模块号（ FTM1、  FTM2）
 *  @since      v5.0
 *  Sample usage:       ftm_quad_init(FTM1);    //初始化 FTM1 为正交解码模式
 */
void ftm_quad_init(FTMn_e ftmn)
{
    ASSERT( (ftmn == FTM1) || (ftmn == FTM2 ) ); //检查传递进来的通道是否正确

    /******************* 开启时钟 和 复用IO口*******************/
    switch(ftmn)
    {

    case FTM1:
        SIM_SCGC6 |= SIM_SCGC6_FTM1_MASK;       //使能FTM1时钟
        if((FTM1_QDPHA_PIN == PTA8) || (FTM1_QDPHA_PIN == PTB0) )                 //管脚复用
        {
            port_init(FTM1_QDPHA_PIN, ALT6);
        }
        else if(FTM1_QDPHA_PIN == PTA12)
        {
            port_init(FTM1_QDPHA_PIN, ALT7);
        }
        else
        {
            ASSERT(0);                          //断言，配置的管脚不符合要求
        }

        if((FTM1_QDPHB_PIN == PTA9) ||(FTM1_QDPHB_PIN == PTB1))
        {
            port_init(FTM1_QDPHB_PIN, ALT6);
        }
        else if(FTM1_QDPHB_PIN == PTA13)
        {
            port_init(FTM1_QDPHB_PIN, ALT7);
        }
        else
        {
            ASSERT(0);                          //断言，配置的管脚不符合要求
        }
        break;

    case FTM2:
        SIM_SCGC3 |= SIM_SCGC3_FTM2_MASK;                           //使能FTM2时钟
        if((FTM2_QDPHA_PIN == PTA10) || (FTM2_QDPHA_PIN == PTB18))                  //管脚复用
        {
            port_init(FTM2_QDPHA_PIN, ALT6);
        }
        else
        {
            ASSERT(0);                          //断言，配置的管脚不符合要求
        }

        if((FTM2_QDPHB_PIN == PTA11) || (FTM2_QDPHB_PIN == PTB19) )                 //管脚复用
        {
            port_init(FTM2_QDPHB_PIN, ALT6);
        }
        else
        {
            ASSERT(0);                          //断言，配置的管脚不符合要求
        }
        break;
    default:
        ASSERT(0);                              //断言，配置的模块有误
        break;
    }

    FTM_MODE_REG(FTMN[ftmn])  |=    (0
                                     | FTM_MODE_WPDIS_MASK  //写保护禁止
                                     //| FTM_MODE_FTMEN_MASK   //使能 FTM
                                    );
    FTM_QDCTRL_REG(FTMN[ftmn]) |=   (0
                                    | FTM_QDCTRL_QUADMODE_MASK
                                     );
    FTM_CNTIN_REG(FTMN[ftmn])   = 0;
    FTM_MOD_REG(FTMN[ftmn])     = FTM_MOD_MOD_MASK;
    FTM_QDCTRL_REG(FTMN[ftmn]) |=   (0
                                    | FTM_QDCTRL_QUADEN_MASK
                                     );
    FTM_MODE_REG(FTMN[ftmn])  |= FTM_QDCTRL_QUADEN_MASK;
    FTM_CNT_REG(FTMN[ftmn])     = 0;                    //计数器。只有低16位可用（写任何值到此寄存器，都会加载 CNTIN 的值）
}

/*!
 *  @brief      获取FTM 正交解码 的脉冲数
 *  @param      FTMn_e    模块号（ FTM1、  FTM2）
 *  @since      v5.0
 *  Sample usage:       int16 count = ftm_quad_get(FTM1);    //获取  FTM1 交解码 的脉冲数
 */
int16 ftm_quad_get(FTMn_e ftmn)
{
    int16 val;
    ASSERT( (ftmn == FTM1) || (ftmn == FTM2 ) );        //检查传递进来的通道是否正确
    val = FTM_CNT_REG(FTMN[ftmn]);

    return val;
}

/*!
 *  @brief      清 FTM 正交解码 的脉冲数
 *  @param      FTMn_e    模块号（ FTM1、  FTM2）
 *  @since      v5.0
 *  Sample usage:       ftm_quad_clean(FTM1);    //复位 FTM1 正交解码 的脉冲数
 */
void ftm_quad_clean(FTMn_e ftmn)
{
    ASSERT( (ftmn == FTM1) || (ftmn == FTM2 ) ); //检查传递进来的通道是否正确

    FTM_CNT_REG(FTMN[ftmn])     = 0;             //计数器。只有低16位可用（写任何值到此寄存器，都会加载 CNTIN 的值）
}



