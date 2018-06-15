/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MKL_uart.c
 * @brief      uart串口函数
 * @author     山外科技
 * @version    v5.0
 * @date       2013-06-26
 */


#include "common.h"
#include "MKL_uart.h"

UART_MemMapPtr UARTN[UART_MAX] = {(UART_MemMapPtr)UART0_BASE_PTR, UART1_BASE_PTR, UART2_BASE_PTR}; //定义三个指针数组保存 UARTN 的地址
                                                                                                   //注意，uart0 的 结构体 跟 其他的是 不一样的

/*!
 *  @brief      初始化串口，设置波特率
 *  @param      UARTn_e       模块号（UART0~UART5）
 *  @param      baud        波特率，如9600、19200、56000、115200等
 *  @since      v5.0
 *  @note       UART所用的管脚在 vcan_drivers_cfg.h 里进行配置，
                printf所用的管脚和波特率在 k60_vcan.h 里进行配置
 *  Sample usage:       uart_init (UART3, 9600);        //初始化串口3，波特率为9600
 */
void uart_init (UARTn_e uratn, uint32 baud)
{
    register uint16 sbr, osr;
    uint8 temp;
    uint32 sysclk,tmp_baud;     //时钟
    uint16 tmp_diff=~0,tmp_sbr,diff_abs_baud;

    /* 配置 UART功能的 复用管脚 */
    switch(uratn)
    {
    case UART0:
        SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;      //使能 UART0 时钟

        if(UART0_RX_PIN == PTA1)
        {
            port_init( UART0_RX_PIN, ALT2);
        }
        else if((UART0_RX_PIN == PTA15)  || (UART0_RX_PIN == PTB16) || (UART0_RX_PIN == PTD6) )
        {
            port_init( UART0_RX_PIN, ALT3);
        }
        else
        {
            ASSERT(0);                           //上诉条件都不满足，直接断言失败了，设置管脚有误？
        }

        if(UART0_TX_PIN == PTA2)
        {
            port_init( UART0_TX_PIN, ALT2);
        }
        else if((UART0_TX_PIN == PTA14)  || (UART0_TX_PIN == PTB17) || (UART0_TX_PIN == PTD7) )
        {
            port_init( UART0_TX_PIN, ALT3);
        }
        else
        {
            ASSERT(0);                           //上诉条件都不满足，直接断言失败了，设置管脚有误？
        }

        break;

    case UART1:
        SIM_SCGC4 |= SIM_SCGC4_UART1_MASK;

        if((UART1_RX_PIN == PTA18) || (UART1_RX_PIN == PTC3) || (UART1_RX_PIN == PTE1) )
        {
            port_init( UART1_RX_PIN, ALT3);
        }
        else
        {
            ASSERT(0);                           //上诉条件都不满足，直接断言失败了，设置管脚有误？
        }

        if((UART1_TX_PIN == PTA19)  || (UART1_TX_PIN == PTC4) || (UART1_TX_PIN == PTE0) )
        {
            port_init( UART1_TX_PIN, ALT3);
        }
        else
        {
            ASSERT(0);                           //上诉条件都不满足，直接断言失败了，设置管脚有误？
        }

        break;

    case UART2:
        SIM_SCGC4 |= SIM_SCGC4_UART2_MASK;

        if((UART2_RX_PIN == PTD2) || (UART2_RX_PIN == PTD4) || (UART2_RX_PIN == PTE17))
        {
            port_init( UART2_RX_PIN, ALT3);
        }
        else if(UART2_RX_PIN == PTE23)
        {
            port_init( UART2_RX_PIN, ALT4);
        }
        else
        {
            ASSERT(0);                           //上诉条件都不满足，直接断言失败了，设置管脚有误？
        }

        if((UART2_TX_PIN == PTD3) || (UART2_TX_PIN == PTD5) || (UART2_TX_PIN == PTE16))
        {
            port_init( UART2_TX_PIN, ALT3);
        }
        else if(UART2_TX_PIN == PTE22)
        {
            port_init( UART2_TX_PIN, ALT4);
        }
        break;


    default:
        break;
    }

    if( uratn == UART0)
    {
        //选择 FLL 或者 PLL/2 (山外代码使用 PLL，因此为 PLL 的 1/2 )
        SIM_SOPT2 &= ~SIM_SOPT2_UART0SRC_MASK;
        SIM_SOPT2 |= (0
                      | SIM_SOPT2_UART0SRC(1)       //
                      | SIM_SOPT2_PLLFLLSEL_MASK
                     );

        // 设置的时候，应该禁止发送接受
        UART0_C2 &= ~(UART0_C2_TE_MASK | UART0_C2_RE_MASK);
        UART0_C1 = 0;       //默认工作模式
        sysclk = pll_clk_mhz * 1000* 1000/2;

        //UART 波特率 = UART 模块时钟/((OSR+1) × SBR)
        //4 <= OSR <= 32
        //SBR = (1 ,8191)
        for(temp = 4;temp<=32;temp++)
        {
            tmp_sbr = sysclk/(baud*(temp+1));
            if(tmp_sbr > 0x1FFF)
            {
                continue;
            }
            tmp_baud = sysclk/(tmp_sbr*(temp+1));
            diff_abs_baud =  ABS((int32)(tmp_baud - baud));
            if(diff_abs_baud == 0)
            {
                sbr = tmp_sbr;
                osr = temp;
                break;
            }
            if(tmp_diff > diff_abs_baud )
            {
                tmp_diff = diff_abs_baud;
                sbr = tmp_sbr;
                osr = temp;
            }

            //由于不一定是整除，因此还需要 加1 试试
            tmp_sbr++;
            if(tmp_sbr > 0x1FFF)
            {
                continue;
            }
            tmp_baud = sysclk/(tmp_sbr*(temp+1));
            diff_abs_baud =  ABS((int32)(tmp_baud - baud));
            if(diff_abs_baud == 0)
            {
                sbr = tmp_sbr;
                osr = temp;
                break;
            }
            if(tmp_diff > diff_abs_baud )
            {

                sbr = tmp_sbr;
                osr = temp;
            }
        }

        //写 SBR
        temp = UART0_BDH & (~UART0_BDH_SBR_MASK);           //缓存 清空 SBR 的 UARTx_BDH的值
        UART0_BDH = temp |  UART0_BDH_SBR(sbr >> 8);        //先写入SBR高位
        UART0_BDL = UART0_BDL_SBR(sbr);                     //再写入SBR低位

        //写 OSR
        temp = UART0_C4 & (~UART0_C4_OSR_MASK) ;           //缓存 清空 BRFA 的 UARTx_C4 的值
        UART0_C4 = temp |  UART0_C4_OSR(osr);             //写入BRFA

        // 设置完毕后，应该使能发送接受
        UART0_C2 |= (UART0_C2_TE_MASK | UART0_C2_RE_MASK);
    }
    else
    {
        //设置的时候，应该禁止发送接受
        UART_C2_REG(UARTN[uratn]) &= ~(0
                                   | UART_C2_TE_MASK
                                   | UART_C2_RE_MASK
                                  );

        //配置成8位无校验模式
        //设置 UART 数据格式、校验方式和停止位位数。通过设置 UART 模块控制寄存器 C1 实现；
        UART_C1_REG(UARTN[uratn]) = (0
                                      //| UART_C2_M_MASK                    //9 位或 8 位模式选择 : 0 为 8位 ，1 为 9位（注释了表示0，即8位） （如果是9位，位8在UARTx_C3里）
                                      //| UART_C2_PE_MASK                   //奇偶校验使能（注释了表示禁用）
                                      //| UART_C2_PT_MASK                   //校验位类型 : 0 为 偶校验 ，1 为 奇校验
                                     );

        sysclk = bus_clk_khz * 1000;                                    //bus时钟

        //UART 波特率 = UART 模块时钟 / (16 × (SBR[12:0] ))
        //不考虑 BRFA 的情况下， SBR = UART 模块时钟 / (16 * UART 波特率)
        sbr = (uint16)(sysclk / (baud * 16));
        if(sbr > 0x1FFF)sbr = 0x1FFF;                                       //SBR 是 13bit，最大为 0x1FFF

        //写 SBR
        temp = UART_BDH_REG(UARTN[uratn]) & (~UART_BDH_SBR_MASK);           //缓存 清空 SBR 的 UARTx_BDH的值
        UART_BDH_REG(UARTN[uratn]) = temp |  UART_BDH_SBR(sbr >> 8);        //先写入SBR高位
        UART_BDL_REG(UARTN[uratn]) = UART_BDL_SBR(sbr);                     //再写入SBR低位

        /* 允许发送和接收 */
        UART_C2_REG(UARTN[uratn]) |= (0
                                      | UART_C2_TE_MASK                     //发送使能
                                      | UART_C2_RE_MASK                     //接收使能
                                      //| UART_C2_TIE_MASK                  //发送中断或DMA传输请求使能（注释了表示禁用）
                                      //| UART_C2_TCIE_MASK                 //发送完成中断使能（注释了表示禁用）
                                      //| UART_C2_RIE_MASK                  //接收满中断或DMA传输请求使能（注释了表示禁用）
                                     );
    }

    //设置是否允许接收和发送中断。通过设置 UART 模块的 C2 寄存器的
    //RIE 和 TIE 位实现。如果使能中断，必须首先实现中断服务程序；
}

/*!
 *  @brief      等待接受1个字节
 *  @param      UARTn_e       模块号（UART0~UART5）
 *  @param      ch          接收地址
 *  @since      v5.0
 *  @note       如果需要查询接收状态，可用 uart_query ，
                如果需要查询接收数据，可用 uart_querychar
 *  Sample usage:       char ch = uart_getchar (UART3);   //等待接受1个字节，保存到 ch里
 */
void uart_getchar (UARTn_e uratn, char *ch)
{
    if(uratn == UART0)
    {
        /* Wait until character has been received */
        while (!(UART0_S1 & UART0_S1_RDRF_MASK));

        /* Return the 8-bit data from the receiver */
        *ch = UART0_D;
    }
    else
    {
        /* Wait until character has been received */
        while (!(UART_S1_REG(UARTN[uratn]) & UART_S1_RDRF_MASK));

        /* Return the 8-bit data from the receiver */
        *ch = UART_D_REG(UARTN[uratn]);
    }
}


/*!
 *  @brief      查询接收1个字符
 *  @param      UARTn_e       模块号（UART0~UART5）
 *  @param      ch          接收地址
 *  @return     1为接收成功，0为接收失败
 *  @since      v5.0
 *  @note       如果需要等待接收，可用 uart_getchar
 *  Sample usage:       char ch ;
                        if( uart_querychar (UART3,&ch) == 1)     //查询接收1个字符，保存到 ch里
                        {
                            printf("成功接收到一个字节");
                        }
 */
char uart_querychar (UARTn_e uratn, char *ch)
{
    if(uratn == UART0)
    {
        if(UART0_S1 & UART0_S1_RDRF_MASK)
        {
            *ch = UART0_D;
            return 1;
        }
    }
    else
    {
        if(UART_S1_REG(UARTN[uratn]) & UART_S1_RDRF_MASK)
        {
            *ch = UART_D_REG(UARTN[uratn]);
            return 1;
        }
    }

    return 0;                                   //返回0表示接收失败
}


/*!
 *  @brief      查询接收字符串
 *  @param      UARTn_e       模块号（UART0~UART5）
 *  @param      str         接收地址
 *  @param      max_len     最大接收长度
 *  @return     接收到的字节数目
 *  @since      v5.0
 *  Sample usage:       char str[100];
                        uint32 num;
                        num = uart_querystr (UART3,&str,100);
                        if( num != 0 )
                        {
                            printf("成功接收到%d个字节:%s",num,str);
                        }
 */
uint32 uart_querystr (UARTn_e uratn, char *str, uint32 max_len)
{
    uint32 i = 0,j;

    for(j=0;j<10000;j++)                 // 10000 的作用是延时，可自行根据情况修改
    {
        while(uart_querychar(uratn, str + i)  )
        {
            j=0;
            if( *(str + i) == NULL )    //接收到字符串结束符
            {
                return i;
            }

            i++;
            if(i >= max_len)            //超过设定的最大值，退出
            {
                *(str + i) = 0;     //确保字符串结尾是0x00
                return i;
            }
        }
    }

    *(str + i) = 0;                     //确保字符串结尾是0x00
    return i;
}


/*!
 *  @brief      查询接收buff
 *  @param      UARTn_e       模块号（UART0~UART5）
 *  @param      str         接收地址
 *  @param      max_len     最大接收长度
 *  @return     接收到的字节数目
 *  @since      v5.0
 *  Sample usage:       char buff[100];
                        uint32 num;
                        num = uart_querybuff (UART3,&buff,100);
                        if( num != 0 )
                        {
                            printf("成功接收到%d个字节:%s",num,buff);
                        }
 */
uint32 uart_querybuff (UARTn_e uratn, char *buff, uint32 max_len)
{
    uint32 i = 0,j;
    for(j=0;j<10000;j++)                 // 10000 的作用是延时，可自行根据情况修改
    {
        while(uart_querychar(uratn, buff + i)  )
        {
            i++;
            if(i >= max_len)            //超过设定的最大值，退出
            {
                return i;
            }
        }
    }

    return i;
}


/*!
 *  @brief      串口发送一个字节
 *  @param      UARTn_e       模块号（UART0~UART5）
 *  @param      ch          需要发送的字节
 *  @since      v5.0
 *  @note       printf需要用到此函数
 *  @see        fputc
 *  Sample usage:       uart_putchar (UART3, 'A');  //发送字节'A'
 */
void uart_putchar (UARTn_e uratn, char ch)
{
    if(uratn == UART0)
    {
        //等待发送缓冲区空
        while(!(UART0_S1 & UART0_S1_TDRE_MASK));

        //发送数据
        UART0_D = (uint8)ch;
    }
    else
    {
        //等待发送缓冲区空
        while(!(UART_S1_REG(UARTN[uratn]) & UART_S1_TDRE_MASK));

        //发送数据
        UART_D_REG(UARTN[uratn]) = (uint8)ch;
    }
}

/*!
 *  @brief      查询是否接受到一个字节
 *  @param      UARTn_e       模块号（UART0~UART5）
 *  @return     接收到数据返回1，没接收到数据返回0
 *  @since      v5.0
 *  Sample usage:       char ch;
                        if(uart_query (UART3) == 1)     //查询是否接收到数据
                        {
                            ch = uart_getchar (UART3);  //等待接收一个数据，保存到 ch里
                        }
 */
char uart_query (UARTn_e uratn)
{

    if(uratn == UART0)
    {
        if(UART0_S1 & UART0_S1_RDRF_MASK)
        {
            return 1;
        }
    }
    else
    {
        if(UART_S1_REG(UARTN[uratn]) & UART_S1_RDRF_MASK)
        {
            return 1;
        }
    }

    return 0;                                   //返回0表示接收失败
}

/*!
 *  @brief      发送指定len个字节长度数组 （包括 NULL 也会发送）
 *  @param      UARTn_e       模块号（UART0~UART5）
 *  @param      buff        数组地址
 *  @param      len         发送数组的长度
 *  @since      v5.0
 *  Sample usage:       uart_putbuff (UART3,"1234567", 3); //实际发送了3个字节'1','2','3'
 */
void uart_putbuff (UARTn_e uratn, uint8 *buff, uint32 len)
{
    while(len--)
    {
        uart_putchar(uratn, *buff);
        buff++;
    }
}


/*!
 *  @brief      发送字符串(遇 NULL 停止发送)
 *  @param      UARTn_e       模块号（UART0~UART5）
 *  @param      str         字符串地址
 *  @since      v5.0
 *  Sample usage:       uart_putstr (UART3,"1234567"); //实际发送了7个字节
 */
void uart_putstr (UARTn_e uratn, const uint8 *str)
{
    while(*str)
    {
        uart_putchar(uratn, *str++);
    }
}

/*!
 *  @brief      开串口接收中断
 *  @param      UARTn_e       模块号（UART0~UART5）
 *  @since      v5.0
 *  Sample usage:       uart_rx_irq_en(UART3);         //开串口3接收中断
 */
void uart_rx_irq_en(UARTn_e uratn)
{
    UART_C2_REG(UARTN[uratn]) |= UART_C2_RIE_MASK;                          //使能UART接收中断
    enable_irq((IRQn_t)((IRQn_t)(uratn) + UART0_IRQn));                  //使能IRQ中断
}

/*!
 *  @brief      开串口发送中断
 *  @param      UARTn_e       模块号（UART0~UART5）
 *  @since      v5.0
 *  Sample usage:       uart_tx_irq_en(UART3);         //开串口3发送中断
 */
void uart_tx_irq_en(UARTn_e uratn)
{
    UART_C2_REG(UARTN[uratn]) |= UART_C2_TIE_MASK;                          //使能UART发送中断
    enable_irq((IRQn_t)((IRQn_t)(uratn) + UART0_IRQn));                  //使能IRQ中断
}

/*!
 *  @brief      开串口发送完成中断
 *  @param      UARTn_e       模块号（UART0~UART5）
 *  @since      v5.0
 *  Sample usage:       uart_tx_irq_en(UART3);         //开串口3发送中断
 */
void uart_txc_irq_en(UARTn_e uratn)
{
    UART_C2_REG(UARTN[uratn]) |= UART_C2_TCIE_MASK;                         //使能UART发送中断
    enable_irq((IRQn_t)((IRQn_t)(uratn) + UART0_IRQn));                  //使能IRQ中断
}

/*!
 *  @brief      关串口接收中断
 *  @param      UARTn_e       模块号（UART0~UART5）
 *  @since      v5.0
 *  Sample usage:       uart_rx_irq_dis(UART3);         //关串口3接收中断
 */
void uart_rx_irq_dis(UARTn_e uratn)
{
    UART_C2_REG(UARTN[uratn]) &= ~UART_C2_RIE_MASK;                         //禁止UART接收中断

    //如果发送中断还没有关，则不关闭IRQ
    if(!(UART_C2_REG(UARTN[uratn]) & (UART_C2_TIE_MASK | UART_C2_TCIE_MASK)) )
    {
        disable_irq((IRQn_t)((IRQn_t)(uratn) + UART0_IRQn));             //关IRQ中断
    }
}

/*!
 *  @brief      关串口发送中断
 *  @param      UARTn_e       模块号（UART0~UART5）
 *  @since      v5.0
 *  Sample usage:       uart_tx_irq_dis(UART3);         //关串口3发送中断
 */
void uart_txc_irq_dis(UARTn_e uratn)
{
    UART_C2_REG(UARTN[uratn]) &= ~UART_C2_TCIE_MASK;                        //禁止UART发送完成中断

    //如果接收中断还没有关，则不关闭IRQ
    if(!(UART_C2_REG(UARTN[uratn]) & UART_C2_RIE_MASK) )
    {
        disable_irq((IRQn_t)((IRQn_t)(uratn) + UART0_IRQn));             //关IRQ中断
    }
}

/*!
 *  @brief      关串口发送中断
 *  @param      UARTn_e       模块号（UART0~UART5）
 *  @since      v5.0
 *  Sample usage:       uart_tx_irq_dis(UART3);         //关串口3发送中断
 */
void uart_tx_irq_dis(UARTn_e uratn)
{
    UART_C2_REG(UARTN[uratn]) &= ~UART_C2_TIE_MASK;                         //禁止UART发送中断

    //如果接收中断还没有关，则不关闭IRQ
    if(!(UART_C2_REG(UARTN[uratn]) & UART_C2_RIE_MASK) )
    {
        disable_irq((IRQn_t)((IRQn_t)(uratn) + UART0_IRQn));             //关IRQ中断
    }
}

/*!
 *  @brief      UART1测试中断服务函数
 *  @since      v5.0
 *  @warning    此函数需要用户根据自己需求完成，这里仅仅是提供一个模版
 *  Sample usage:       set_vector_handler(UART3_RX_TX_VECTORn , uart3_test_handler);    //把 uart3_handler 函数添加到中断向量表，不需要我们手动调用
 */
void uart1_test_handler(void)
{
    UARTn_e uratn = UART1;

    if(UART_S1_REG(UARTN[uratn]) & UART_S1_RDRF_MASK)   //接收数据寄存器满
    {
        //用户需要处理接收数据

    }

    if(UART_S1_REG(UARTN[uratn]) & UART_S1_TDRE_MASK )  //发送数据寄存器空
    {
        //用户需要处理发送数据

    }
}