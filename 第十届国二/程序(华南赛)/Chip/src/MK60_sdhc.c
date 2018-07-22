/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 *     本驱动由brtos代码提取并修改而成:http://code.google.com/p/brtos/downloads/detail?name=BRTOS%201.66%20Kinetis%20-%20SD%20card%20-%20BMP%2024%20bit.rar&can=2&q=
 *
 * @file       MK60_sdhc.c
 * @brief      SD模块函数
 * @author     山外科技 & gustavo(brtos作者)
 * @version    v5.0
 * @date       2013-09-19
 */


#include "common.h"
#include "MK60_sdhc.h"

SDCARD_t        SDHC_card;


//SD卡命令
const uint32 ESDHC_COMMAND_XFERTYP[] =
{
    /* CMD0 */
    SDHC_XFERTYP_CMDINX(ESDHC_CMD0) | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_NO),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD1) | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_NO),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD2) | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_136),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD3) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD4) | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_NO),
    /* CMD5 */
    SDHC_XFERTYP_CMDINX(ESDHC_CMD5) | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD6) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD7) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48BUSY),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD8) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD9) | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_136),
    /* CMD10 */
    SDHC_XFERTYP_CMDINX(ESDHC_CMD10) | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_136),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD11) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD12) | SDHC_XFERTYP_CMDTYP(ESDHC_XFERTYP_CMDTYP_ABORT) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48BUSY),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD13) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    ~0,
    /* CMD15 */
    SDHC_XFERTYP_CMDINX(ESDHC_CMD15) | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_NO),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD16) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD17) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD18) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48)| SDHC_XFERTYP_AC12EN_MASK,
    ~0,
    /* CMD20 */
    SDHC_XFERTYP_CMDINX(ESDHC_CMD20) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    ~0,
    SDHC_XFERTYP_CMDINX(ESDHC_ACMD22) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_ACMD23) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD24) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    /* CMD25 */
    SDHC_XFERTYP_CMDINX(ESDHC_CMD25) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48) | SDHC_XFERTYP_AC12EN_MASK,
    SDHC_XFERTYP_CMDINX(ESDHC_CMD26) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD27) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD28) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48BUSY),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD29) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48BUSY),
    /* CMD30 */
    SDHC_XFERTYP_CMDINX(ESDHC_CMD30) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    ~0,
    SDHC_XFERTYP_CMDINX(ESDHC_CMD32) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD33) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD34) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    /* CMD35 */
    SDHC_XFERTYP_CMDINX(ESDHC_CMD35) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD36) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD37) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD38) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48BUSY),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD39) | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    /* CMD40 */
    SDHC_XFERTYP_CMDINX(ESDHC_CMD40) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_ACMD41) | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD42) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48BUSY),
    ~0,
    ~0,
    /* CMD45 */
    ~0,
    ~0,
    ~0,
    ~0,
    ~0,
    /* CMD50 */
    ~0,
    SDHC_XFERTYP_CMDINX(ESDHC_ACMD51) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD52) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD53) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    ~0,
    /* CMD55 */
    SDHC_XFERTYP_CMDINX(ESDHC_CMD55) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD56) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48BUSY),
    ~0,
    ~0,
    ~0,
    /* CMD60 */
    SDHC_XFERTYP_CMDINX(ESDHC_CMD60) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48BUSY),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD61) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48BUSY),
    ~0,
    ~0
};

#define SDHC_CMD_MAX    ARR_SIZE( ESDHC_COMMAND_XFERTYP )



/*!
 *  @brief      SDHC初始化(仅初始化模块，不初始化SD卡)
 *  @since      v5.0
 */
void SDHC_init()
{

    SDHC_card.CARD = ESDHC_CARD_NONE;

#if defined( MK60DZ10)
    /* 使能 SDHC 模块时钟 */
    SIM_SCGC3 |= SIM_SCGC3_SDHC_MASK;

#elif defined( MK60F15)
    /* 使能 SDHC 模块时钟 */
    SIM_SCGC3 |= SIM_SCGC3_ESDHC_MASK;
#endif

    /* 复位 ESDHC */
    SDHC_SYSCTL = SDHC_SYSCTL_RSTA_MASK | SDHC_SYSCTL_SDCLKFS(0x80);
    while (SDHC_SYSCTL & SDHC_SYSCTL_RSTA_MASK) {};

    /* 初始化值 */
    SDHC_VENDOR     = 0;
    SDHC_BLKATTR    = SDHC_BLKATTR_BLKCNT(1) | SDHC_BLKATTR_BLKSIZE(512);
    SDHC_PROCTL     = SDHC_PROCTL_EMODE(ESDHC_PROCTL_EMODE_INVARIANT) | SDHC_PROCTL_D3CD_MASK;
    SDHC_WML        = SDHC_WML_RDWML(1) | SDHC_WML_WRWML(1);

    /* 配置 ESDHC 波特率 */
    SDHC_set_baudrate ( SDHC_INIT_BANDRATE );

    /* Poll inhibit bits */
    while (SDHC_PRSSTAT & (SDHC_PRSSTAT_CIHB_MASK | SDHC_PRSSTAT_CDIHB_MASK)) {};

    /* 初始化管脚复用 */
    port_init(PTE0, ALT4 | HDS | PULLUP ); /* ESDHC.D1  */
    port_init(PTE1, ALT4 | HDS | PULLUP ); /* ESDHC.D0  */
    port_init(PTE2, ALT4 | HDS          ); /* ESDHC.CLK */
    port_init(PTE3, ALT4 | HDS | PULLUP ); /* ESDHC.CMD */
    port_init(PTE4, ALT4 | HDS | PULLUP ); /* ESDHC.D3  */
    port_init(PTE5, ALT4 | HDS | PULLUP ); /* ESDHC.D2  */

    /* 使能请求 */
    SDHC_IRQSTAT = 0xFFFF;
    SDHC_IRQSTATEN =      SDHC_IRQSTATEN_DEBESEN_MASK | SDHC_IRQSTATEN_DCESEN_MASK | SDHC_IRQSTATEN_DTOESEN_MASK
                          | SDHC_IRQSTATEN_CIESEN_MASK | SDHC_IRQSTATEN_CEBESEN_MASK | SDHC_IRQSTATEN_CCESEN_MASK | SDHC_IRQSTATEN_CTOESEN_MASK
                          | SDHC_IRQSTATEN_BRRSEN_MASK | SDHC_IRQSTATEN_BWRSEN_MASK | SDHC_IRQSTATEN_CRMSEN_MASK
                          | SDHC_IRQSTATEN_TCSEN_MASK | SDHC_IRQSTATEN_CCSEN_MASK;

    /* 发送 80 个 最初的时钟周期 到卡上，卡上电期间是需要用到 */
    SDHC_SYSCTL |= SDHC_SYSCTL_INITA_MASK;
    while (SDHC_SYSCTL & SDHC_SYSCTL_INITA_MASK) {};        //等待 80个SD周期发送完成

    /* 检测 卡是否插入 */
    if (SDHC_PRSSTAT & SDHC_PRSSTAT_CINS_MASK)              // CINS 字段 由 0变1表示插入卡，由1变0表示拔出卡
    {
        SDHC_card.CARD = ESDHC_CARD_UNKNOWN;          //插入了卡，类型未知
    }
    SDHC_IRQSTAT |= SDHC_IRQSTAT_CRM_MASK;                  //写1清 CRM 标志位。0表示插入卡，1表示移除卡。写1清0后，卡还是没有插入，则恢复值为1
}




/*!
 *  @brief      SDHC波特率配置
 *  @param      baudrate    波特率(Hz)
 *  @since      v5.0
 */
void SDHC_set_baudrate(uint32 baudrate)
{
    uint32 pres, div, min, minpres = 0x80, mindiv = 0x0F;
    int32  val;
    uint32 clock = SDHC_CLOCK;

    /* 计算最佳配置 */
    //SDCLK 时钟频率 = 基时钟/(预分频器*除数)
    //SD 时钟频率的最大值是 50MHz
    //预分频 = 2,4,8,16,32,64,128,256  ,且 SDCLKFS = 预分频 >> 1;
    //除数 = DVS + 1 ,DVS 的取值范围是 0~ 0xF
    min = (uint32) - 1;                         //先把 min 配置为最大值
    for (pres = 2; pres <= 256; pres <<= 1)     //pres 即 预分频器 prescaler
    {
        for (div = 1; div <= 16; div++)         //div 即 除数Divisor
        {
            val = pres * div * baudrate - clock;
            if (val >= 0)                       //波特率 大于或等于 设定的目标值
            {
                if (min > val)                  //选择最接近 目标值 的
                {
                    min = val;
                    minpres = pres;
                    mindiv = div;
                }
            }
        }
    }

    /* 禁止 ESDHC 时钟 */
    SDHC_SYSCTL &= (~ SDHC_SYSCTL_SDCLKEN_MASK);

    /* 改变分频系数 */
    SDHC_SYSCTL = (   (SDHC_SYSCTL & (~ (SDHC_SYSCTL_DTOCV_MASK | SDHC_SYSCTL_SDCLKFS_MASK | SDHC_SYSCTL_DVS_MASK)) ) //先 清 SDHC_SYSCTL 的 DTOCV 、SDCLKFS 、DVS 字段
                      | SDHC_SYSCTL_DTOCV(0x0E)               //数据超时计数器值 = SDCLK x (DTOCV + 213)  ,DTOCV 的范围是 0 - 0x0E
                      | SDHC_SYSCTL_SDCLKFS(minpres >> 1)     //SDCLK 频率选择 = 基时钟 / (1 << SDCLKFS )
                      | SDHC_SYSCTL_DVS(mindiv - 1)

                  );

    /* 等待 SD 时钟稳定  */
    while (0 == (SDHC_PRSSTAT & SDHC_PRSSTAT_SDSTB_MASK));


    /* 使能 ESDHC 时钟 */
    SDHC_SYSCTL |= SDHC_SYSCTL_SDCLKEN_MASK;

    SDHC_IRQSTAT |= SDHC_IRQSTAT_DTOE_MASK;     //清 数据超时错误 标志
}


/*!
 *  @brief      SDHC发送cmd命令
 *  @param      command     命令
 *  @return     发送命令结果，    0表示成功，1表示错误，-1 表示 超时
 *  @since      v5.0
 */
uint32 SDHC_cmd (pESDHC_CMD_t command) /* [IN/OUT] Command specification */
{
    uint32  xfertyp;
    uint32  blkattr;

    //ASSERT(SDHC_CMD_MAX > command->COMMAND  );      //断言，命令不能超过 SDHC_CMD_MAX

    /* 检查命令 */
    xfertyp = ESDHC_COMMAND_XFERTYP[command->COMMAND ];

    ASSERT (~0 != xfertyp);                             //断言，合法的CMD,xfertyp 不能为 ~0

    /* 准备检测卡插拔状态 */
    SDHC_IRQSTAT |= SDHC_IRQSTAT_CRM_MASK;

    /* 等待CMD 线空闲 */
    while (SDHC_PRSSTAT & SDHC_PRSSTAT_CIHB_MASK) {};

    /* 设置命令 */
    SDHC_CMDARG = command->ARGUMENT;
    xfertyp &= (~ SDHC_XFERTYP_CMDTYP_MASK);
    xfertyp |= SDHC_XFERTYP_CMDTYP(command->TYPE);
    if (ESDHC_TYPE_RESUME == command->TYPE)     //恢复命令 必须 DPSEL 位 置位
    {
        xfertyp |= SDHC_XFERTYP_DPSEL_MASK;
    }

    if (0 != command->BLOCKS)
    {
        if ((xfertyp & SDHC_XFERTYP_RSPTYP_MASK) != SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48BUSY))
        {
            xfertyp |= SDHC_XFERTYP_DPSEL_MASK;
        }
        if (command->READ)       //写数据
        {
           xfertyp |= SDHC_XFERTYP_DTDSEL_MASK;
        }
        if (command->BLOCKS > 1)
        {
            xfertyp |= SDHC_XFERTYP_MSBSEL_MASK;
        }
        if ((uint32) - 1 != command->BLOCKS)
        {
            blkattr  = SDHC_BLKATTR_BLKCNT(command->BLOCKS) | SDHC_BLKATTR_BLKSIZE(SDCARD_BLOCK_SIZE);
            xfertyp |= SDHC_XFERTYP_BCEN_MASK;
        }
        else
        {
            blkattr = SDHC_BLKATTR_BLKCNT(0xFFFF)| SDHC_BLKATTR_BLKSIZE(SDCARD_BLOCK_SIZE) ;
        }
    }
    else
    {
        blkattr = 0;
    }

    SDHC_BLKATTR = blkattr;


    /* 发出命令 */
    SDHC_DSADDR = 0;
    SDHC_XFERTYP = xfertyp;

    /* 等待回应 */
    SDHC_STATUS_WAIT (SDHC_IRQSTAT_CIE_MASK | SDHC_IRQSTAT_CEBE_MASK | SDHC_IRQSTAT_CCE_MASK | SDHC_IRQSTAT_CC_MASK);
    if (SDHC_STATUS_GET(SDHC_IRQSTAT_CIE_MASK | SDHC_IRQSTAT_CEBE_MASK | SDHC_IRQSTAT_CCE_MASK | SDHC_IRQSTAT_CC_MASK) != SDHC_IRQSTAT_CC_MASK)
    {
        SDHC_IRQSTAT |= SDHC_IRQSTAT_CTOE_MASK | SDHC_IRQSTAT_CIE_MASK | SDHC_IRQSTAT_CEBE_MASK | SDHC_IRQSTAT_CCE_MASK | SDHC_IRQSTAT_CC_MASK;
        return ESDHC_CMD_TIMEOUT;
    }

    /* 检测卡拔插状态 */
    if (SDHC_IRQSTAT & SDHC_IRQSTAT_CRM_MASK)
    {
        SDHC_IRQSTAT |= SDHC_IRQSTAT_CTOE_MASK | SDHC_IRQSTAT_CC_MASK;
        return ESDHC_CMD_ERR;
    }

    /* 如果有，则获取相应 */
    if (SDHC_IRQSTAT & SDHC_IRQSTAT_CTOE_MASK)
    {
        SDHC_IRQSTAT |= SDHC_IRQSTAT_CTOE_MASK | SDHC_IRQSTAT_CC_MASK;
        return ESDHC_CMD_TIMEOUT;
    }

    //如果 发送有相应 命令
    if ((xfertyp & SDHC_XFERTYP_RSPTYP_MASK) != SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_NO))
    {
        command->RESPONSE[0] = SDHC_CMDRSP(0);

        //如果接收到长相应
        if ((xfertyp & SDHC_XFERTYP_RSPTYP_MASK) == SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_136))
        {
            command->RESPONSE[1] = SDHC_CMDRSP(1);
            command->RESPONSE[2] = SDHC_CMDRSP(2);
            command->RESPONSE[3] = SDHC_CMDRSP(3);
        }
    }

    //完成命令
    SDHC_IRQSTAT |= SDHC_IRQSTAT_CC_MASK;

    return 0;
}



/*!
 *  @brief      SDHC 控制命令
 *  @param      ESDHC_IOCTL_CMD_e       命令
 *  @return     ESDHC_IOCTL_ERR_e       操作结果
 *  @since      v5.0
 */
ESDHC_IOCTL_ERR_e SDHC_ioctl(ESDHC_IOCTL_CMD_e cmd, void *param_ptr )
{
    ESDHC_CMD_t             command;
    uint8                   mem, io, mmc, ceata, mp, hc;
    int32                   val;
    ESDHC_IOCTL_ERR_e                  result ;
    uint32                  *param32_ptr = param_ptr;

    result = ESDHC_IOCTL_OK;

    switch (cmd)
    {
    case ESDHC_IOCTL_INIT:

        SDHC_init();        //初始化 SD卡模块

        mem = FALSE;
        io  = FALSE;
        mmc = FALSE;
        ceata = FALSE;
        hc = FALSE;
        mp = FALSE;

        /* CMD0 - Go to idle - reset card */
        command.COMMAND = ESDHC_CMD0;
        command.TYPE = ESDHC_TYPE_NORMAL;
        command.ARGUMENT = 0;
        command.READ = FALSE;
        command.BLOCKS = 0;
        if (SDHC_cmd (&command))
        {
            result = ESDHC_INIT_ERR;
            break;
        }

        DELAY_MS(1100);

        /* CMD8 - Send interface condition - check HC support */
        command.COMMAND = ESDHC_CMD8;
        command.TYPE = ESDHC_TYPE_NORMAL;
        command.ARGUMENT = 0x000001AA;
        command.READ = FALSE;
        command.BLOCKS = 0;
        val = SDHC_cmd (&command);

        if (val == 0)
        {
            // SDHC Card
            if (command.RESPONSE[0] != command.ARGUMENT)
            {
                result = ESDHC_INIT_ERR;
                break;
            }
            hc = TRUE;
        }

        mp = TRUE;

        if (mp)
        {
            /* CMD55 - Application specific command - check MMC */
            command.COMMAND = ESDHC_CMD55;
            command.TYPE = ESDHC_TYPE_NORMAL;
            command.ARGUMENT = 0;
            command.READ = FALSE;
            command.BLOCKS = 0;
            val = SDHC_cmd (&command);
            if (val > 0)
            {
                result = ESDHC_INIT_ERR;
                break;
            }
            if (val < 0)
            {
                /* MMC or CE-ATA */
                io = FALSE;
                mem = FALSE;
                hc = FALSE;

                /* CMD1 - Send operating conditions - check HC */
                command.COMMAND = ESDHC_CMD1;
                command.TYPE = ESDHC_TYPE_NORMAL;
                command.ARGUMENT = 0x40300000;
                command.READ = FALSE;
                command.BLOCKS = 0;
                if (SDHC_cmd (&command))
                {
                    result = ESDHC_INIT_ERR;
                    break;
                }
                if (0x20000000 == (command.RESPONSE[0] & 0x60000000))
                {
                    hc = TRUE;
                }
                mmc = TRUE;

                /* CMD39 - Fast IO - check CE-ATA signature CE */
                command.COMMAND = ESDHC_CMD39;
                command.TYPE = ESDHC_TYPE_NORMAL;
                command.ARGUMENT = 0x0C00;
                command.READ = FALSE;
                command.BLOCKS = 0;
                if (SDHC_cmd (&command))
                {
                    result = ESDHC_INIT_ERR;
                    break;
                }
                if (0xCE == (command.RESPONSE[0] >> 8) & 0xFF)
                {
                    /* CMD39 - Fast IO - check CE-ATA signature AA */
                    command.COMMAND = ESDHC_CMD39;
                    command.TYPE = ESDHC_TYPE_NORMAL;
                    command.ARGUMENT = 0x0D00;
                    command.READ = FALSE;
                    command.BLOCKS = 0;
                    if (SDHC_cmd (&command))
                    {
                        result = ESDHC_INIT_ERR;
                        break;
                    }
                    if (0xAA == (command.RESPONSE[0] >> 8) & 0xFF)
                    {
                        mmc = FALSE;
                        ceata = TRUE;
                    }
                }
            }
            else
            {
                /* SD */
                /* ACMD41 - Send Operating Conditions */
                command.COMMAND = ESDHC_ACMD41;
                command.TYPE = ESDHC_TYPE_NORMAL;
                command.ARGUMENT = 0;
                command.READ = FALSE;
                command.BLOCKS = 0;
                if (SDHC_cmd (&command))
                {
                    result = ESDHC_INIT_ERR;
                    break;
                }
                if (command.RESPONSE[0] & 0x300000)
                {
                    val = 0;
                    do
                    {
                        DELAY_MS(10);
                        val++;

                        /* CMD55 + ACMD41 - Send OCR */
                        //MMC卡对CMD55+ACMD41命令不作响应，故在初始化过程中用这一命令可以区别SD卡跟MMC卡
                        command.COMMAND = ESDHC_CMD55;
                        command.TYPE = ESDHC_TYPE_NORMAL;
                        command.ARGUMENT = 0;
                        command.READ = FALSE;
                        command.BLOCKS = 0;
                        if (SDHC_cmd (&command))
                        {
                            result = ESDHC_INIT_ERR;
                            break;
                        }

                        command.COMMAND = ESDHC_ACMD41;
                        command.TYPE = ESDHC_TYPE_NORMAL;
                        if (hc)
                        {
                            command.ARGUMENT = 0x40300000;
                        }
                        else
                        {
                            command.ARGUMENT = 0x00300000;
                        }
                        command.READ = FALSE;
                        command.BLOCKS = 0;
                        if (SDHC_cmd (&command))
                        {
                            result = ESDHC_INIT_ERR;
                            break;
                        }
                    }
                    while ((0 == (command.RESPONSE[0] & 0x80000000)) && (val < SEND_OCR_TIME));
                    if (ESDHC_IOCTL_OK != result)
                    {
                        break;
                    }
                    if (val >= SEND_OCR_TIME)
                    {
                        hc = FALSE;
                    }
                    else
                    {
                        mem = TRUE;
                        if (hc)
                        {
                            hc = FALSE;
                            if (command.RESPONSE[0] & 0x40000000)
                            {
                                hc = TRUE;
                            }
                        }
                    }
                }
            }
        }


        if (mmc)
        {
            SDHC_card.CARD = ESDHC_CARD_MMC;
        }
        if (ceata)
        {
            SDHC_card.CARD = ESDHC_CARD_CEATA;
        }
        if (io)
        {
            SDHC_card.CARD = ESDHC_CARD_SDIO;
        }
        if (mem)
        {
            SDHC_card.CARD = ESDHC_CARD_SD;
            if (hc)
            {
                SDHC_card.CARD = ESDHC_CARD_SDHC;
            }
        }
        if (io && mem)
        {
            SDHC_card.CARD = ESDHC_CARD_SDCOMBO;
            if (hc)
            {
                SDHC_card.CARD = ESDHC_CARD_SDHCCOMBO;
            }
        }

        /* 设置 ESDHC 默认波特率 */
        SDHC_set_baudrate (SDHC_BANDRATE);

#if defined( MK60DZ10)
        /* 使能 SDHC 模块时钟 */
        SIM_SCGC3 |= SIM_SCGC3_SDHC_MASK;

#elif defined( MK60F15)
        /* 使能 SDHC 模块时钟 */
        SIM_SCGC3 |= SIM_SCGC3_ESDHC_MASK;
#endif
        break;
    case ESDHC_IOCTL_SEND_CMD:
        val = SDHC_cmd ((pESDHC_CMD_t)param32_ptr);
        if (val > 0)
        {
            result = ESDHC_CMD_ERR;
        }
        if (val < 0)
        {
            result = ESDHC_CMD_TIMEOUT;
        }
        break;
    case ESDHC_IOCTL_GET_BUS_WIDTH:
        if (NULL == param32_ptr)
        {
            result = ESDHC_PARAM_INVALID;
        }
        else
        {
            /* 获取 ESDHC 总线宽度 */
            val = (SDHC_PROCTL & SDHC_PROCTL_DTW_MASK) >> SDHC_PROCTL_DTW_SHIFT;
            if (ESDHC_PROCTL_DTW_1BIT == val)
            {
                *param32_ptr = ESDHC_BUS_WIDTH_1BIT;
            }
            else if (ESDHC_PROCTL_DTW_4BIT == val)
            {
                *param32_ptr = ESDHC_BUS_WIDTH_4BIT;
            }
            else if (ESDHC_PROCTL_DTW_8BIT == val)
            {
                *param32_ptr = ESDHC_BUS_WIDTH_8BIT;
            }
            else
            {
                result = ESDHC_BUS_WIDTH_INVALID;
            }
        }
        break;
    case ESDHC_IOCTL_SET_BUS_WIDTH:
        if (NULL == param32_ptr)
        {
            result = ESDHC_PARAM_INVALID;
        }
        else
        {
            /* 设置 ESDHC 总线宽度 */
            if (! SDHC_is_running())
            {
                if (ESDHC_BUS_WIDTH_1BIT == *param32_ptr)
                {
                    SDHC_PROCTL &= (~ SDHC_PROCTL_DTW_MASK);
                    SDHC_PROCTL |= SDHC_PROCTL_DTW(ESDHC_PROCTL_DTW_1BIT);
                }
                else if (ESDHC_BUS_WIDTH_4BIT == *param32_ptr)
                {
                    SDHC_PROCTL &= (~ SDHC_PROCTL_DTW_MASK);
                    SDHC_PROCTL |= SDHC_PROCTL_DTW(ESDHC_PROCTL_DTW_4BIT);
                }
                else if (ESDHC_BUS_WIDTH_8BIT == *param32_ptr)
                {
                    SDHC_PROCTL &= (~ SDHC_PROCTL_DTW_MASK);
                    SDHC_PROCTL |= SDHC_PROCTL_DTW(ESDHC_PROCTL_DTW_8BIT);
                }
                else
                {
                    result = ESDHC_BUS_WIDTH_INVALID;
                }
            }
            else
            {
                result = ESDHC_BUSY;
            }
        }
        break;
    case ESDHC_IOCTL_GET_CARD:
        if (NULL == param32_ptr)
        {
            result = ESDHC_PARAM_INVALID;
        }
        else
        {
            /* 80 clocks to update levels */
            SDHC_SYSCTL |= SDHC_SYSCTL_INITA_MASK;
            while (SDHC_SYSCTL & SDHC_SYSCTL_INITA_MASK)
                { };

            /* Update and return actual card status */
            if (SDHC_IRQSTAT & SDHC_IRQSTAT_CRM_MASK)
            {
                SDHC_IRQSTAT |= SDHC_IRQSTAT_CRM_MASK;
                SDHC_card.CARD = ESDHC_CARD_NONE;
            }
            if (SDHC_PRSSTAT & SDHC_PRSSTAT_CINS_MASK)
            {
                if (ESDHC_CARD_NONE == SDHC_card.CARD)
                {
                    SDHC_card.CARD = ESDHC_CARD_UNKNOWN;
                }
            }
            else
            {
                SDHC_card.CARD = ESDHC_CARD_NONE;
            }
            *param32_ptr = SDHC_card.CARD;
        }
        break;
    default:
        result = ESDHC_CMD_INVALID;
        break;
    }
    return result;
}
