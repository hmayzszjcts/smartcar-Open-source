/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_dma.c
 * @brief      DMA函数库
 * @author     山外科技
 * @version    v5.0
 * @date       2013-09-01
 */


#include "common.h"
#include "MKL_port.h"
#include "MKL_gpio.h"
#include "MKL_dma.h"


static void dma_gpio_input_init(void *SADDR,uint8 BYTEs)
{
    uint8 n, tmp;
    uint8 ptxn;

    //SADDR 实际上就是 GPIO的 输入寄存器 PDIR 的地址
    //GPIOA、GPIOB、GPIOC、GPIOD、GPIOE 的地址 分别是 0x400FF000u 、0x400FF040u 、0x400FF080u、 0x400FF0C0u、0x400FF100u
    //sizeof(GPIO_MemMap) = 0x18
    //每个GPIO地址 &0x1C0 后，得到 0x000 , 0x040 , 0x080 ,0x0C0 ,0x100
    //再 /0x40 后得到 0 、 1 、 2、 3、4 ，刚好就是 PTA、PTB、PTC 、PTD 、PTE
    //再 *32 就等于 PTA0、PTB0、PTC0 、PTD0 、PTE0
    uint8 ptx0 = ((((uint32)SADDR) & 0x1C0) / 0x40 ) * 32;

    //每个GPIO 对应的寄存器地址， &0x 3F 后得到的值都是相同的。
    //PTA_B0_IN 即 GPIOA 的 输入寄存器 PDIR 的 地址
    // (SADDR & 0x3f - PTA_B0_IN & 0x3f) 等效于 (SADDR - PTA_B0_IN) & 0x3f
    //假设需要采集的位 为 0~7、8~15、16~23、24~31 ，则 上面式子对应的值 为 0、1、2、3
    //刚好是  0~7、8~15、16~23、24~31 位的地址偏移，再 * 8 就变成 0、8、16、24

    n = (uint8)(((uint32)SADDR - ((uint32)(&PTA_B0_IN))) & 0x3f) * 8;       //最小的引脚号

    ptxn = ptx0 + n;
    tmp = ptxn + (BYTEs * 8 ) - 1;                                          //最大的引脚号
    while(ptxn <= tmp)
    {
        //这里加入 GPIO 初始化为输入
        gpio_init((PTXn_e )ptxn, GPI, 0);               //设置为输入
        port_init((PTXn_e )ptxn , ALT1 | PULLDOWN );    //输入源默认配置为下拉，默认读取到的是0
        ptxn ++;
    }
}

/*!
 *  @brief      DMA初始化，读取端口数据到内存
 *  @param      DMA_CHn         通道号（DMA_CH0 ~ DMA_CH15）
 *  @param      SADDR           源地址( (void * )&PTx_Bn_IN 或 (void * )&PTx_Wn_IN   )
 *  @param      DADDR           目的地址
 *  @param      PTxn            触发端口
 *  @param      DMA_BYTEn       每次DMA传输字节数
 *  @param      count           一个主循环传输字节数
 *  @param      cfg             DMA传输配置，从DMA_cfg里选择
 *  @since      v5.0
 *  @note       DMA PTXn触发源默认上升沿触发传输，若需修改，则初始化后调用 port_init 配置DMA 触发方式
                初始化后，需要调用 DMA_EN 来实现
 *  Sample usage:   uint8 buff[10];
                    dma_portx2buff_init(DMA_CH0, PTB_B0_IN, buff, PTA7, DMA_BYTE1, 10, DADDR_RECOVER);
                    //DMA初始化，源地址：PTB_B0_IN，目的地址：buff,PTA7触发(默认上升沿)，每次传输1字节，共传输 10次 ，传输结束后恢复地址

                    port_init(PTA7,ALT1 | DMA_FALLING);             //默认触发源是上升沿，此处改为 下降沿触发

                    DMA_EN(DMA_CH0);                //需要使能 DMA 后才能传输数据
 */
void dma_portx2buff_init(DMA_CHn CHn, void *SADDR, void *DADDR, PTXn_e ptxn, DMA_BYTEn byten, uint32 count)
{
    uint8 BYTEs = (byten == DMA_BYTE1 ? 1 : (byten == DMA_BYTE2 ? 2 : (byten == DMA_BYTE4 ? 4 : 0 ) ) ); //计算传输字节数    uint8 ptx0;

    //断言，检测传递进来参数是否正确
    ASSERT(                                             //用断言检测 源地址和每次传输字节数是否正确
        (   (byten == DMA_BYTE1)                    //传输一个字节
            && ( (SADDR >= &PTA_B0_IN) && (SADDR <= ( &PTE_B3_IN )))
        )

        || (   (byten == DMA_BYTE2)                   //传输两个字节(注意，不能跨端口)
               && ( (SADDR >= &PTA_B0_IN)
                    && (SADDR <= ( &PTE_W1_IN ))
                    && (((uint32)SADDR & 0x03) != 0x03) )         //保证不跨端口
           )

        || (   (byten == DMA_BYTE4)                   //传输四个字节
               && ((SADDR >= &PTA_B0_IN) && (SADDR <= ( &PTE_B0_IN )))
               && (((uint32)SADDR & 0x03) == 0x00)           //保证不跨端口
           )
    );

    ASSERT(count < 0x8000); //断言，最大只支持0x7FFF

    //DMA 寄存器 配置

    /* 开启时钟 */
    SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;                        //打开DMA模块时钟

#if (defined(MK60DZ10)|| defined(MKL26Z4))
    SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;                     //打开DMA多路复用器时钟
#elif defined(MK60F15)
    SIM_SCGC6 |= SIM_SCGC6_DMAMUX0_MASK;                    //打开DMA多路复用器时钟
#endif

    DMAMUX0_CHCFG(CHn)=0x00;
    DMA_DSR_BCR(CHn)|=DMA_DSR_BCR_DONE_MASK;

    /* 配置 DMA 通道 的 传输控制块 TCD ( Transfer Control Descriptor ) */
    DMA_SAR(CHn) =    (uint32)SADDR;                      // 设置  源地址
    DMA_DAR(CHn) =    (uint32)DADDR;                      // 设置目的地址
    DMA_DCR(CHn) =    (0
                       | DMA_DCR_SSIZE(byten)
                       | DMA_DCR_DSIZE(byten)
                       //| DMA_DCR_SINC_MASK                  //传输后源地址增加(根据位宽)
                       | DMA_DCR_DINC_MASK                  //传输后目的地址增加(根据位宽)
                       | DMA_DCR_CS_MASK                  // 0为不停得传输，直到BCR为0；1为一次请求传输一次
                       //| DMA_DCR_START_MASK               //软件触发传输
                       | DMA_DCR_ERQ_MASK                   //硬件触发传输（与上面START二选一）
                       | DMA_DCR_D_REQ_MASK                 //传输完成后硬件自动清ERQ

                       );
    DMA_DSR_BCR(CHn) =   (0
                     | DMA_DSR_BCR_BCR(count)              //传输数目
                     );


    /* 配置 DMA 触发源 */
#if defined(MK60DZ10)
    DMAMUX_CHCFG_REG(DMAMUX_BASE_PTR, CHn) = (0
#elif (defined(MK60F15) || defined(MKL26Z4))
    DMAMUX_CHCFG_REG(DMAMUX0_BASE_PTR, CHn) = (0
#endif
            | DMAMUX_CHCFG_ENBL_MASK                        /* Enable routing of DMA request */
            //| DMAMUX_CHCFG_TRIG_MASK                        /* Trigger Mode: Periodic   PIT周期触发传输模式   通道1对应PIT1，必须使能PIT1，且配置相应的PIT定时触发 */
            | DMAMUX_CHCFG_SOURCE( PTX(ptxn) + DMA_PORTA) /* 通道触发传输源:     */
                                             );

    //配置触发源（默认是 上升沿触发）
    port_init(ptxn, ALT1 | DMA_RISING);

    /*  配置输入源   */
    dma_gpio_input_init(SADDR,BYTEs);

    DMA_DIS(CHn);                                    //使能通道CHn 硬件请求
    //DMA_IRQ_CLEAN(CHn);

    /* 开启中断 */
    //DMA_EN(CHn);                                    //使能通道CHn 硬件请求
    //DMA_IRQ_EN(CHn);                                //允许DMA通道传输
}
