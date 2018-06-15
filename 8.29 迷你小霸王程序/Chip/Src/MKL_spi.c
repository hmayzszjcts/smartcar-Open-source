/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_spi.c
 * @brief      SPI驱动函数
 * @author     山外科技
 * @version    v5.0
 * @date       2013-07-16
 */

#include "common.h"
#include "MKL_port.h"
#include "MKL_gpio.h"
#include "MKL_spi.h"


SPI_MemMapPtr SPIN[2] = {SPI0_BASE_PTR, SPI1_BASE_PTR}; //定义两个指针数组保存 SPIx 的地址

static void spi_pcs_io(SPIn_e spin, SPI_PCSn_e pcs,uint8 data)        //IO控制 CS 电平高低
{
    switch(spin)
    {

    case SPI0:
        if(pcs ==SPI_PCS0)
        {
            PTXn_T(SPI0_PCS0_PIN,OUT) = data ;
        }
        break;

    case SPI1:
        if(pcs ==SPI_PCS0)
        {
            PTXn_T(SPI1_PCS0_PIN,OUT) = data ;
        }
        break;
    default:
        break;

    }

}

uint32 spi_set_baud(SPIn_e spin,uint32 baud)
{
    SPI_MemMapPtr spi_ptr = SPIN[spin];
    uint32 clk;
    uint8  spr;
    uint32 fit_baud,fit_sppr=0,fit_spr,min_diff =~0,diff;
    uint32 tmp,tmp_baud;

    if(spin == SPI0)
    {
        clk =  bus_clk_khz*1000;
    }
    else if(spin == SPI1)
    {
        clk =  core_clk_khz*1000;
    }

    //SPI 波特率 =  SPI 模块时钟 / （ (SPPR + 1) * (2<<(SPR )) ）
    for(spr = 0;spr<=8;spr++)
    {
        tmp = clk/((2<<spr)*baud );
        if(tmp>=8)continue;
        tmp_baud  = clk/((2<<spr)*tmp);
        diff = abs(tmp_baud - baud);
        if(min_diff > diff)
        {
            min_diff = diff;
            fit_spr = spr;
            fit_sppr = tmp -1;
            fit_baud = tmp_baud;
            if(diff == 0)
            {
                //刚好匹配
                goto SPI_CLK_EXIT;
            }
        }

        tmp++;

        tmp = clk/((2<<spr)*baud );
        if(tmp>=8)continue;
        tmp_baud  = clk/((2<<spr)*tmp);
        diff = abs(tmp_baud - baud);

        if(min_diff > diff)
        {
            min_diff = diff;
            fit_spr = spr;
            fit_sppr = tmp -1;
            fit_baud = tmp_baud;
            if(diff == 0)
            {
                //刚好匹配
                goto SPI_CLK_EXIT;
            }

        }
    }

SPI_CLK_EXIT:

    SPI_BR_REG(spi_ptr) = (0
                              | SPI_BR_SPR(fit_spr)
                              | SPI_BR_SPPR(fit_sppr)
                              );


    return fit_baud;

}

/*!
 *  @brief      SPI初始化，设置模式
 *  @param      SPIn_e          SPI模块(SPI0、SPI1)
 *  @param      SPIn_PCSn_e     片选管脚编号
 *  @param      SPI_CFG         SPI主从机模式选择
 *  @since      v5.0
 *  Sample usage:       spi_init(SPI0,SPIn_PCS0, MASTER);              //初始化SPI,选择CS0,主机模式
 */
uint32 spi_init(SPIn_e spin, SPI_PCSn_e pcs, SPI_CFG master,uint32 baud)
{
    SPI_MemMapPtr spi_ptr = SPIN[spin];



    //使能SPI模块时钟，配置SPI引脚功能
    if(spin == SPI0)
    {
        SIM_SCGC4 |= SIM_SCGC4_SPI0_MASK;

        //进行管脚复用
        port_init(SPI0_SCK_PIN , ALT2  );       //全部都是 ALT2

        if( (SPI0_SOUT_PIN == PTE19) || (SPI0_SOUT_PIN == PTA17) || (SPI0_SOUT_PIN == PTC7)  || (SPI0_SOUT_PIN == PTD3)  )
        {
            port_init(SPI0_SOUT_PIN, ALT5  );
        }
        else
        {
            port_init(SPI0_SOUT_PIN, ALT2  );
        }

        if( (SPI0_SIN_PIN == PTE18 ) || (SPI0_SIN_PIN == PTA16 ) || (SPI0_SIN_PIN == PTC6 ) || (SPI0_SIN_PIN == PTD2 ))
        {
            port_init(SPI0_SIN_PIN , ALT5 | PULLUP   );
        }
        else
        {
            port_init(SPI0_SIN_PIN , ALT2  );
        }

        if(pcs & SPI_PCS0)         //选择 IO 口 控制 CS
        {
            gpio_init(SPI0_PCS0_PIN,GPO,1);
            //port_init(SPI0_PCS0_PIN, ALT1  );
        }
    }
    else if(spin == SPI1)
    {
        SIM_SCGC4 |= SIM_SCGC4_SPI1_MASK;

        port_init(SPI1_SCK_PIN , ALT2  );

        if((SPI1_SOUT_PIN == PTB17) || (SPI1_SOUT_PIN == PTE3) || (SPI1_SOUT_PIN == PTD7) )
        {
            port_init(SPI1_SOUT_PIN, ALT5  );
        }
        else
        {
            port_init(SPI1_SOUT_PIN, ALT2  );
        }

        if((SPI1_SIN_PIN == PTE1) || (SPI1_SIN_PIN == PTB16) || (SPI1_SIN_PIN == PTD6))
        {
            port_init(SPI1_SIN_PIN , ALT5  );
        }
        else
        {
            port_init(SPI1_SIN_PIN , ALT2  );
        }

        if(pcs & SPI_PCS0)        //选择 IO 口 控制 CS
        {
            gpio_init(SPI1_PCS0_PIN,GPO,1);
            //port_init(SPI1_PCS0_PIN, ALT1  );
        }

    }
    else
    {
        //传递进来的 spi 模块有误，直接判断断言失败
        ASSERT(0);
    }

    //根据主从机模式设置工作模式。MCU提供最大主机频率是1/2主频，最大从机频率是1/4主频
    if(master == MASTER)
    {
        SPI_C1_REG(spi_ptr) = (0
                          | SPI_C1_SPE_MASK     //使能SPI
                          | SPI_C1_MSTR_MASK    //主机模式
                          //| SPI_C1_SSOE_MASK    //使能自动片选

                          );
        SPI_C2_REG(spi_ptr) =  (0
                                  //| SPI_C2_MODFEN_MASK //主机模式故障功能使能
                                   );
        return spi_set_baud(spin,baud);

    }
    else
    {
        SPI_C1_REG(spi_ptr) = (0
                          | SPI_C1_SPE_MASK     //使能SPI
                          //| SPI_C1_MSTR_MASK    //主机模式
                          //| SPI_C1_SSOE_MASK    //使能自动片选

                          );
        SPI_C2_REG(spi_ptr) =  (0
                                  //| SPI_C2_MODFEN_MASK //主机模式故障功能使能
                                   );
    }

    return 0;

}

/*!
 *  @brief      SPI发送接收函数
 *  @param      SPIn_e          SPI模块(SPI0、SPI1)
 *  @param      SPIn_PCSn_e     片选管脚编号
 *  @param      modata          发送的数据缓冲区地址(不需要接收则传 NULL)
 *  @param      midata          发送数据时接收到的数据的存储地址(不需要接收则传 NULL)
 *  @since      v5.0
 *  Sample usage:           spi_mosi(SPI0,SPIn_PCS0,buff,buff,2);    //发送buff的内容，并接收到buff里，长度为2字节
 */
void spi_mosi(SPIn_e spin, SPI_PCSn_e pcs, uint8 *modata, uint8 *midata, uint32 len)
{
    SPI_MemMapPtr spi_ptr = SPIN[spin];
    uint32 i = 0;

    while((!SPI_S_REG(spi_ptr))&SPI_S_SPTEF_MASK);//等待发送缓冲区空闲

    while((SPI_S_REG(spi_ptr)& SPI_S_SPRF_MASK))    //检测接收缓冲区为空
    {
        SPI_DL_REG(spi_ptr);                     //直接读取数据来清空接收缓冲区
    }

    spi_pcs_io(spin, pcs,0);


    /***************** 发送len个数据 *******************/                                                ;
    for(i = 0; i < (len ); i++)
    {
        while((!SPI_S_REG(spi_ptr))&SPI_S_SPTEF_MASK);  //等待发送缓冲区空闲
        SPI_DL_REG(spi_ptr)=modata[i];
        while(!(SPI_S_REG(spi_ptr)& SPI_S_SPRF_MASK));  //检测接收缓冲区非空
        if(midata != NULL)
        {
            midata[i] = SPI_DL_REG(spi_ptr);            //保存接收到的数据
        }
        else
        {
            SPI_DL_REG(spi_ptr);
        }
    }
    spi_pcs_io(spin, pcs,1);
}

/*!
 *  @brief      SPI发送接收函数
 *  @param      SPIn_e          SPI模块(SPI0、SPI1)
 *  @param      SPIn_PCSn_e     片选管脚编号
 *  @param      mocmd           发送的命令缓冲区地址(不需要接收则传 NULL)
 *  @param      micmd           发送命令时接收到的数据的存储地址(不需要接收则传 NULL)
 *  @param      modata          发送的数据缓冲区地址(不需要接收则传 NULL)
 *  @param      midata          发送数据时接收到的数据的存储地址(不需要接收则传 NULL)
 *  @since      v5.0
 *  Sample usage:           spi_mosi(SPI0,SPIn_PCS0,cmd,NULL,buff,buff,1,2);    //发送cmd/buff的内容，不接收cmd发送时的数据，接收buff发送时的数据到buff里，长度分别为1、2字节
 */
void spi_mosi_cmd(SPIn_e spin, SPI_PCSn_e pcs, uint8 *mocmd , uint8 *micmd , uint8 *modata , uint8 *midata, uint32 cmdlen , uint32 len)
{
    SPI_MemMapPtr spi_ptr = SPIN[spin];
    uint32 i = 0;

    while((!SPI_S_REG(spi_ptr))&SPI_S_SPTEF_MASK);//等待发送缓冲区空闲

    while((SPI_S_REG(spi_ptr)& SPI_S_SPRF_MASK))    //检测接收缓冲区为空
    {
        SPI_DL_REG(spi_ptr);                     //直接读取数据来清空接收缓冲区
    }

    spi_pcs_io(spin, pcs,0);

    /***************** 发送len个数据 *******************/                                                ;
    for(i = 0; i < cmdlen; i++)
    {
        while((!SPI_S_REG(spi_ptr))&SPI_S_SPTEF_MASK);  //等待发送缓冲区空闲
        SPI_DL_REG(spi_ptr)=mocmd[i];
        while(!(SPI_S_REG(spi_ptr)& SPI_S_SPRF_MASK));  //检测接收缓冲区非空
        if(micmd != NULL)
        {
            micmd[i] = SPI_DL_REG(spi_ptr);            //保存接收到的数据
        }
        else
        {
            SPI_DL_REG(spi_ptr);
        }
    }

    /***************** 发送len个数据 *******************/                                                ;
    for(i = 0; i < (len); i++)
    {
        while((!SPI_S_REG(spi_ptr))&SPI_S_SPTEF_MASK);  //等待发送缓冲区空闲
        SPI_DL_REG(spi_ptr)=modata[i];
        while(!(SPI_S_REG(spi_ptr)& SPI_S_SPRF_MASK));  //检测接收缓冲区非空
        if(midata != NULL)
        {
            midata[i] = SPI_DL_REG(spi_ptr);            //保存接收到的数据
        }
        else
        {
            SPI_DL_REG(spi_ptr);
        }
    }

    spi_pcs_io(spin, pcs,1);
}


