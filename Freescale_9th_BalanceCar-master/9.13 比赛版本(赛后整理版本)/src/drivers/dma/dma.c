#include "common.h"
#include "gpio.h"
#include "PIT.h"
#include "dma.h"

//u8 counttempaddr;
#define COUNTSADDR   0x4004000C  //(&counttempaddr)
#define COUNTDADDR   0x4004000C  //(&counttempaddr)

u32 count_init[16];         //用来保存16个通道的初始化计数值



/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：DMA_PORTx2BUFF_Init
*  功能说明：DMA初始化，读取端口数据到内存
*  参数说明：DMA_CHn              通道号（DMA_CH0 ~ DMA_CH15）
*            SADDR                源地址( (void * )&PTx_BYTEn_IN 或 (void * )&PTx_WORDn_IN   )
*            DADDR                目的地址
*            PTxn                 触发端口
*            DMA_BYTEn            每次DMA传输字节数
*            count                一个主循环传输字节数
*            DMA_PORTx2BUFF_cfg   DMA传输配置
*  函数返回：无
*  修改时间：2012-2-20
*  备    注：
*************************************************************************/
void DMA_PORTx2BUFF_Init(DMA_CHn CHn, void *SADDR, void *DADDR, PTxn ptxn, DMA_BYTEn byten, u32 count, DMA_PORTx2BUFF_cfg cfg)
{
    u8 n, i, tmp;

    ASSERT(                                             //用断言检测 源地址和每次传输字节数是否正确
        (   (byten == DMA_BYTE1)                    //传输一个字节
            && ( (SADDR >= &PTA_BYTE0_IN) && (SADDR <= ( &PTE_BYTE3_IN )))
        )

        || (   (byten == DMA_BYTE2)                   //传输两个字节(注意，不能跨端口)
               && ( (SADDR >= &PTA_BYTE0_IN)
                    && (SADDR <= ( &PTE_WORD1_IN ))
                    && (((u32)SADDR & 0x03) != 0x03) )         //保证不跨端口
           )

        || (   (byten == DMA_BYTE4)                   //传输四个字节
               && ((SADDR >= &PTA_BYTE0_IN) && (SADDR <= ( &PTE_BYTE0_IN )))
               && (((u32)SADDR & 0x03) == 0x00)           //保证不跨端口
           )
    );

    u8 BYTEs = (byten == DMA_BYTE1 ? 1 : (byten == DMA_BYTE2 ? 2 : (byten == DMA_BYTE4 ? 4 : 16 ) ) ); //计算传输字节数

    /* 开启时钟 */
    SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;                        //打开DMA模块时钟
    SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;                     //打开DMA多路复用器时钟

    /* 配置 DMA 通道 的 传输控制块 TCD ( Transfer Control Descriptor ) */
    DMA_SADDR(CHn) =    (u32)SADDR;                         // 设置  源地址
    DMA_DADDR(CHn) =    (u32)DADDR;                         // 设置目的地址
    DMA_SOFF(CHn)  =    0x00u;                              // 设置源地址偏移 = 0x0, 即不变
    DMA_DOFF(CHn)  =    BYTEs;                              // 每次传输后，目的地址加 BYTEs

    DMA_ATTR(CHn)  =    (0
                         | DMA_ATTR_SMOD(0x0)                // 源地址模数禁止  Source address modulo feature is disabled
                         | DMA_ATTR_SSIZE(byten)             // 源数据位宽 ：DMA_BYTEn  。    SSIZE = 0 -> 8-bit ，SSIZE = 1 -> 16-bit ，SSIZE = 2 -> 32-bit ，SSIZE = 4 -> 16-byte
                         | DMA_ATTR_DMOD(0x0)                // 目标地址模数禁止
                         | DMA_ATTR_DSIZE(byten)             // 目标数据位宽 ：DMA_BYTEn  。  设置参考  SSIZE
                        );

    DMA_CITER_ELINKNO(CHn)  = DMA_CITER_ELINKNO_CITER(count); //当前主循环次数
    DMA_BITER_ELINKNO(CHn)  = DMA_BITER_ELINKYES_BITER(count);//起始主循环次数


    DMA_CR &= ~DMA_CR_EMLM_MASK;                            // CR[EMLM] = 0

    //当CR[EMLM] = 0 时:
    DMA_NBYTES_MLNO(CHn) =   DMA_NBYTES_MLNO_NBYTES(BYTEs); // 通道每次传输字节数，这里设置为BYTEs个字节。注：值为0表示传输4GB */


    /* 配置 DMA 传输结束后的操作 */
    DMA_SLAST(CHn)      =   0;                              //调整  源地址的附加值,主循环结束后恢复  源地址
    DMA_DLAST_SGA(CHn)  =   (u32)( (cfg & 0x20) == 0 ? (-count)  : 0 ); //调整目的地址的附加值,主循环结束后恢复目的地址或者保持地址
    DMA_CSR(CHn)        =   (0
                             | DMA_CSR_DREQ_MASK            //主循环结束后停止硬件请求
                             | DMA_CSR_INTMAJOR_MASK        //主循环结束后产生中断
                            );

    /* 配置 DMA 触发源 */
    DMAMUX_CHCFG_REG(DMAMUX_BASE_PTR, CHn) = (0
            | DMAMUX_CHCFG_ENBL_MASK                        /* Enable routing of DMA request */
            //| DMAMUX_CHCFG_TRIG_MASK                        /* Trigger Mode: Periodic   PIT周期触发传输模式   通道1对应PIT1，必须使能PIT1，且配置相应的PIT定时触发 */
            | DMAMUX_CHCFG_SOURCE((ptxn >> 5) + DMA_Port_A) /* 通道触发传输源:     */
                                             );

    SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK << (ptxn>>5));                                                               //开启PORTx端口
    GPIO_PDDR_REG(GPIOx[(ptxn>>5)]) &= ~(1 << (ptxn & 0x1f));                                                       //设置端口方向为输入
    PORT_PCR_REG(PORTX[(ptxn>>5)], (ptxn & 0x1F)) = ( 0
            | PORT_PCR_MUX(1)               // 复用GPIO
            | PORT_PCR_IRQC(cfg & 0x03 )    // 确定触发模式
            | ((cfg & 0xc0 ) >> 6)          // 开启上拉或下拉电阻，或者没有
                                                    );
    GPIO_PDDR_REG(GPIOx[(ptxn>>5)]) &= ~(1 << (ptxn && 0x1F));                                                      //输入模式

    /*  配置输入源   */
    SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK << ((((u32)SADDR) & 0x1ff)>>6));             //开启PORTx端口
    switch(byten)
    {
    case DMA_BYTE1:
        *((u8 *)((u32)SADDR + 4)) = 0;   //设置为输入方向。为什么加4？PDIR地址加4后，就变成对应的PDDR地址
        break;
    case DMA_BYTE2:
        *((u16 *)((u32)SADDR + 4)) = 0;
        break;
    case DMA_BYTE4:
        *((u32 *)((u32)SADDR + 4)) = 0;
        break;
    default:
        assert_failed(__FILE__, __LINE__);
        break;
    }

    /*  输入源管脚选择功能脚  */
    n = (u8)(((u32)SADDR - ((u32)(&PTA_BYTE0_IN))) & 0x3f);         //最小的引脚号
    tmp = n + (BYTEs << 3);                                         //最大的引脚号
    for(i = n; i < tmp; i++)
    {
        PORT_PCR_REG(PORTX[   ((((u32)SADDR)&0x1ff)>>6)    ], i) = (0
                | PORT_PCR_MUX(1)
                | GPI_DOWN             //输入源应该下拉，默认读取到的是0
                                                                   );
    }

    /* 开启中断 */
    DMA_EN(CHn);                                    //使能通道CHn 硬件请求
    DMA_IRQ_EN(CHn);                                //允许DMA通道传输
}





/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：DMA_count_Init
*  功能说明：DMA累加计数初始化
*  参数说明：DMA_CHn              通道号（DMA_CH0 ~ DMA_CH15）
*            PTxn                 触发端口
*            count                累加计数中断值
*            DMA_Count_cfg        DMA传输配置
*  函数返回：无
*  修改时间：2012-1-20
*  备    注：
*************************************************************************/
void DMA_count_Init(DMA_CHn CHn, PTxn ptxn, u32 count, DMA_Count_cfg cfg)
{
    u8 byten = DMA_BYTE1;
    u8 BYTEs = (byten == DMA_BYTE1 ? 1 : (byten == DMA_BYTE2 ? 2 : (byten == DMA_BYTE4 ? 4 : 16 ) ) ); //计算传输字节数
    if(count > 0x7FFF )count = 0x7FFF;
    count_init[CHn] = count;

    /* 开启时钟 */
    SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;                        //打开DMA模块时钟
    SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;                     //打开DMA多路复用器时钟

    /* 配置 DMA 通道 的 传输控制块 TCD ( Transfer Control Descriptor ) */
    DMA_SADDR(CHn) =    (u32)COUNTSADDR;                    // 设置  源地址
    DMA_DADDR(CHn) =    (u32)COUNTDADDR;                    // 设置目的地址
    DMA_SOFF(CHn)  =    0;                                  // 设置源地址不变
    DMA_DOFF(CHn)  =    0;                                  // 每次传输后，目的地址不变

    DMA_ATTR(CHn)  =    (0
                         | DMA_ATTR_SMOD(0x0)                // 源地址模数禁止  Source address modulo feature is disabled
                         | DMA_ATTR_SSIZE(byten)             // 源数据位宽 ：DMA_BYTEn  。    SSIZE = 0 -> 8-bit ，SSIZE = 1 -> 16-bit ，SSIZE = 2 -> 32-bit ，SSIZE = 4 -> 16-byte
                         | DMA_ATTR_DMOD(0x0)                // 目标地址模数禁止
                         | DMA_ATTR_DSIZE(byten)             // 目标数据位宽 ：DMA_BYTEn  。  设置参考  SSIZE
                        );

    DMA_CITER_ELINKNO(CHn)  = DMA_CITER_ELINKNO_CITER(count); //当前主循环次数
    DMA_BITER_ELINKNO(CHn)  = DMA_BITER_ELINKYES_BITER(count);//起始主循环次数

    DMA_CR &= ~DMA_CR_EMLM_MASK;                            // CR[EMLM] = 0

    DMA_NBYTES_MLNO(CHn) =   DMA_NBYTES_MLNO_NBYTES(BYTEs); // 通道每次传输字节数，这里设置为BYTEs个字节。注：值为0表示传输4GB */

    /* 配置 DMA 传输结束后的操作 */
    DMA_SLAST(CHn)      =   -count;                              //调整  源地址的附加值,主循环结束后恢复  源地址
    DMA_DLAST_SGA(CHn)  =   0;                                  //调整目的地址的附加值,主循环结束后恢复目的地址或者保持地址
    DMA_CSR(CHn)        =   (0
                             | DMA_CSR_DREQ_MASK            //主循环结束后停止硬件请求
                             | DMA_CSR_INTMAJOR_MASK        //主循环结束后产生中断
                            );

    /* 配置 DMA 触发源 */
    DMAMUX_CHCFG_REG(DMAMUX_BASE_PTR, CHn) = (0
            | DMAMUX_CHCFG_ENBL_MASK                        /* Enable routing of DMA request */
            | DMAMUX_CHCFG_SOURCE((ptxn >> 5) + DMA_Port_A) /* 通道触发传输源:     */
                                             );

    SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK << (ptxn>>5));                                                               //开启PORTx端口
    GPIO_PDDR_REG(GPIOx[(ptxn>>5)]) &= ~(1 << (ptxn & 0x1f));                                                       //设置端口方向为输入
    PORT_PCR_REG(PORTX[(ptxn>>5)], (ptxn & 0x1F)) = ( 0
            | PORT_PCR_MUX(1)               // 复用GPIO
            | PORT_PCR_IRQC(cfg & 0x03 )    // 确定触发模式
            | ((cfg & 0xc0 ) >> 6)          // 开启上拉或下拉电阻，或者没有
                                                    );
    GPIO_PDDR_REG(GPIOx[(ptxn>>5)]) &= ~(1 << (ptxn && 0x1F));                                                      //输入模式

    /* 开启中断 */
    DMA_EN(CHn);                                    //使能通道CHn 硬件请求
    DMA_IRQ_EN(CHn);                                //允许DMA通道传输
}

/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：DMA_count_get
*  功能说明：返回累加计数值
*  参数说明：DMA_CHn              通道号（DMA_CH0 ~ DMA_CH15）
*  函数返回：累加计数值
*  修改时间：2012-3-320
*  备    注：
*************************************************************************/
u32 DMA_count_get(DMA_CHn CHn)
{
    u32 temp =  count_init[CHn] - DMA_CITER_ELINKNO(CHn)  ;
    return temp;
}

void DMA_count_reset(DMA_CHn CHn)
{
    DMA_CITER_ELINKNO(CHn) = count_init[CHn] ;
}


