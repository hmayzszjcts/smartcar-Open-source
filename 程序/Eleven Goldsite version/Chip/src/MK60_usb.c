/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_usb.c
 * @brief      USB 代码库(目前 仅 有 虚拟 串口 )
 * @author     山外科技
 * @version    v5.0
 * @date       2013-09-26
 * @note       本例程 移植 飞思卡尔公司 提供的 K60 参考代码
 */


#include "common.h"
#include "usb.h"
#include "MK60_usb.h"


/*!
 *  @brief      USB模块 初始化
 *  @since      v5.0
 */
void usb_init(void)
{
    /* 软件配置 */
    Setup_Pkt = (tUSB_Setup *)BufferPointer[bEP0OUT_ODD];
    gu8USB_State = uPOWER;                          //标记状态为上电状态
    /* MPU 配置 */
    MPU_CESR = 0;                                   // MPU is disable. All accesses from all bus masters are allowed

    /* SIM 配置 */
#ifdef USB_CLOCK_CLKIN
    FLAG_SET(SIM_SCGC5_PORTE_SHIFT, SIM_SCGC5);
    PORTE_PCR26 = (0 | PORT_PCR_MUX(7));            // Enabling PTE26 as CLK input

#endif


#if defined(MK60DZ10)
#ifdef USB_CLOCK_PLL
    FLAG_SET(SIM_SOPT2_PLLFLLSEL_SHIFT, SIM_SOPT2); // Select PLL output
#endif


#ifndef USB_CLOCK_CLKIN
    FLAG_SET(SIM_SOPT2_USBSRC_SHIFT, SIM_SOPT2);    // PLL/FLL selected as CLK source
#endif

    SIM_CLKDIV2 = ((SIM_CLKDIV2 & ~( SIM_CLKDIV2_USBDIV_MASK | SIM_CLKDIV2_USBFRAC_MASK ))
                   | SIM_CLKDIV2_USBDIV(USB_CLK_DIV)                    //USB 分频因子
                   | (USB_CLK_FRAC << SIM_CLKDIV2_USBFRAC_SHIFT)        //USB 倍频因子
                   //USB clk = PLL × [ (USBFRAC+1) / (USBDIV+1) ]
                  );
    SIM_SCGC4 |= SIM_SCGC4_USBOTG_MASK;             //USB Clock Gating
                                                    //开启USB模块的时钟源
#elif defined(MK60F15)
#ifdef USB_CLOCK_PLL
    SIM_SOPT2 |=(0
                | SIM_SOPT2_PLLFLLSEL(1)       /** PLL0 reference */   
                | SIM_SOPT2_USBFSRC(0)         /** MCGPLLCLK as CLKC source */
                | SIM_SOPT2_USBF_CLKSEL_MASK   /** USB fractional divider like USB reference clock */  
                );

#endif


#ifndef USB_CLOCK_CLKIN
    FLAG_SET(SIM_SOPT2_USBHSRC_SHIFT, SIM_SOPT2);    // PLL/FLL selected as CLK source
#endif

    SIM_CLKDIV2 = ((SIM_CLKDIV2 & ~( SIM_CLKDIV2_USBFSDIV_MASK | SIM_CLKDIV2_USBFSFRAC_MASK ))
                   | SIM_CLKDIV2_USBFSDIV(USB_CLK_DIV)                  //USB 分频因子
                   | (USB_CLK_FRAC << SIM_CLKDIV2_USBFSFRAC_SHIFT)      //USB 倍频因子
                   //USB clk = PLL × [ (USBFRAC+1) / (USBDIV+1) ]
                  );

    //SIM_SCGC6 |= SIM_SCGC6_USB2OTG_MASK;
    SIM_SCGC4 |= SIM_SCGC4_USBFS_MASK;             //USB Clock Gating
                                                    //开启USB模块的时钟源
#endif


    /* NVIC模块配置 */
    set_vector_handler(USB0_VECTORn, USB_ISR);
    enable_irq(USB0_IRQn);                          //使能NVIC中的USB OTG中断

    /* USB模块配置 */
    USB0_USBTRC0 |= USB_USBTRC0_USBRESET_MASK;      //复位USB模块
    while(FLAG_CHK(USB_USBTRC0_USBRESET_SHIFT, USB0_USBTRC0));
    USB0_BDTPAGE1 = (uint8)((uint32)tBDTtable >> 8); //配置当前缓冲描述符表BDT
    USB0_BDTPAGE2 = (uint8)((uint32)tBDTtable >> 16);
    USB0_BDTPAGE3 = (uint8)((uint32)tBDTtable >> 24);

    // 清 USB 复位标记
    FLAG_SET(USB_ISTAT_USBRST_MASK, USB0_ISTAT);

    // 使能 USB 复位中断
    FLAG_SET(USB_INTEN_USBRSTEN_SHIFT, USB0_INTEN);

    // Enable weak pull downs
    USB0_USBCTRL = 0x40;

    USB0_USBTRC0 |= 0x40;

    USB0_CTL |= 0x01;

    // 上拉使能
    FLAG_SET(USB_CONTROL_DPPULLUPNONOTG_SHIFT, USB0_CONTROL);
}


/*!
 *  @brief      USB 虚拟串口 初始化
 *  @since      v5.0
 */
void usb_com_init(void)
{
    usb_init(); //初始化USB模块

    CDC_Init(); //初始化USB CDC模式
}


/*!
 *  @brief      USB 等待枚举
 *  @since      v5.0
 */
void usb_enum_wait(void)
{
    while(gu8USB_State != uENUMERATED);//等待USB设备被枚举
}

/*!
 *  @brief      USB 虚拟串口 接收
 *  @param      rx_buf          数据接收缓冲区
 *  @return     接收到的长度
 *  @since      v5.0
 */
uint8 usb_com_rx(uint8_t *rx_buf)
{
    uint8 len;
    uint8 temp = 0;
    uint8 *pu8EPBuffer;

    if(FLAG_CHK(EP_OUT, gu8USB_Flags)) // 如果有数据到来
    {
        len = USB_EP_OUT_SizeCheck(EP_OUT);
        temp = len;
        pu8EPBuffer = gu8EP3_OUT_ODD_Buffer;

        while(len--)
            *rx_buf++ = *pu8EPBuffer++;

        usbEP_Reset(EP_OUT);
        usbSIE_CONTROL(EP_OUT);
        FLAG_CLR(EP_OUT, gu8USB_Flags);
    }
    return temp;
}


/*!
 *  @brief      USB 虚拟串口 发送
 *  @param      tx_buf          数据发送缓冲区
 *  @param      需要发送的数据长度
 *  @since      v5.0
 */
void usb_com_tx(uint8 *tx_buf, uint8 len)
{
    EP_IN_Transfer(EP_IN, tx_buf, len);
}




