/*
 * @brief      二值化摄像头驱动代码
 */

#include "common.h"
#include "MK60_gpio.h"
#include "MK60_port.h"
#include "MK60_dma.h"
#include "camera.h"
#include "include.h"

uint8 imgbuff[CAMERA_SIZE];                       //定义存储接收图像的数组
uint8 imgDrawbuff[CAMERA_SIZE],imgprocessed;                       //定义存储接收图像的数组
uint8 img[CAMERA_W*CAMERA_H];                           //


//内部函数声明
static uint8 camera_reg_init(void);
static void camera_port_init();


/*OV7725初始化配置表*/
reg_s camera_reg[] =
{
    //寄存器，寄存器值次
    {OV7725_COM4         , 0xC1},
    {OV7725_CLKRC        , 0x00},
    {OV7725_COM2         , 0x03},
    {OV7725_COM3         , 0xD0},
    {OV7725_COM7         , 0x40},
    {OV7725_HSTART       , 0x3F},
    {OV7725_HSIZE        , 0x50},
    {OV7725_VSTRT        , 0x03},
    {OV7725_VSIZE        , 0x78},
    {OV7725_HREF         , 0x00},
    {OV7725_SCAL0        , 0x0A},
    {OV7725_AWB_Ctrl0    , 0xE0},
    {OV7725_DSPAuto      , 0xff},
    {OV7725_DSP_Ctrl2    , 0x0C},
    {OV7725_DSP_Ctrl3    , 0x00},
    {OV7725_DSP_Ctrl4    , 0x00},

#if (CAMERA_W == 80)
    {OV7725_HOutSize     , 0x14},
#elif (CAMERA_W == 160)
    {OV7725_HOutSize     , 0x28},
#elif (CAMERA_W == 240)
    {OV7725_HOutSize     , 0x3c},
#elif (CAMERA_W == 320)
    {OV7725_HOutSize     , 0x50},
#else

#endif

#if (CAMERA_H == 60 )
    {OV7725_VOutSize     , 0x1E},
#elif (CAMERA_H == 120 )
    {OV7725_VOutSize     , 0x3c},
#elif (CAMERA_H == 180 )
    {OV7725_VOutSize     , 0x5a},
#elif (CAMERA_H == 240 )
    {OV7725_VOutSize     , 0x78},
#else

#endif

    {OV7725_EXHCH        , 0x00},
    {OV7725_GAM1         , 0x0c},
    {OV7725_GAM2         , 0x16},
    {OV7725_GAM3         , 0x2a},
    {OV7725_GAM4         , 0x4e},
    {OV7725_GAM5         , 0x61},
    {OV7725_GAM6         , 0x6f},
    {OV7725_GAM7         , 0x7b},
    {OV7725_GAM8         , 0x86},
    {OV7725_GAM9         , 0x8e},
    {OV7725_GAM10        , 0x97},
    {OV7725_GAM11        , 0xa4},
    {OV7725_GAM12        , 0xaf},
    {OV7725_GAM13        , 0xc5},
    {OV7725_GAM14        , 0xd7},
    {OV7725_GAM15        , 0xe8},
    {OV7725_SLOP         , 0x20},
    {OV7725_LC_RADI      , 0x00},
    {OV7725_LC_COEF      , 0x13},
    {OV7725_LC_XC        , 0x08},
    {OV7725_LC_COEFB     , 0x14},
    {OV7725_LC_COEFR     , 0x17},
    {OV7725_LC_CTR       , 0x05},
    {OV7725_BDBase       , 0x99},
    {OV7725_BDMStep      , 0x03},
    {OV7725_SDE          , 0x04},
    {OV7725_BRIGHT       , 0x00},
    {OV7725_CNST         , 0xFF},
    {OV7725_SIGN         , 0x06},
    {OV7725_UVADJ0       , 0x11},
    {OV7725_UVADJ1       , 0x02},

};

/*!
 *  @brief      摄像头初始化
 */
uint8 camera_init(void)
{
    while(camera_reg_init() == 0);
    camera_port_init();
     
    return 0;
}

/*!
 *  @brief      ov7725管脚初始化（内部调用）
 */

void camera_port_init()
{
    //DMA通道0初始化，PTC12触发源(默认上升沿)，源地址为PTD_B0_IN，目的地址为：IMG_BUFF，每次传输1Byte
    dma_portx2buff_init(DMA_CH0, (void *)&PTD_B0_IN, imgbuff, PTC12, DMA_BYTE1, CAMERA_DMA_NUM, DADDR_KEEPON);

    DMA_DIS(DMA_CH0);
    disable_irq(PORTC_IRQn);                        //关闭PTC的中断
    DMA_IRQ_CLEAN(DMA_CH0);                   //清除通道传输中断标志位
    DMA_IRQ_EN(DMA_CH0);

    port_init(PTC12, ALT1 | DMA_RISING | PULLUP );         //PCLK
    port_init(PTC13, ALT1 | IRQ_RISING | PULLUP | PF);     //场中断，上拉，上降沿触发中断，带滤波
}



/*!
 *  @brief      PORTC中断服务函数
 *  @since      v5.0
 */
void PORTC_IRQHandler()
{
    uint8  n = 0;    //引脚号
    uint32 flag = PORTC_ISFR;
    PORTC_ISFR  = ~0;                                   //清中断标志位
    n = 13;                                             //场中断
    if(flag & (1 << n))                                 //PTC13触发中断
    {
      DMA_DADDR(DMA_CH0) = (uint32)imgbuff;    //恢复地址
      DMA_EN(DMA_CH0);                  //使能通道CHn 硬件请求     
    }

}

/*!
 *  @brief      DMA0中断服务函数
 *  @since      v5.0
 */
void DMA0_IRQHandler()
{
  int i;
  DMA_IRQ_CLEAN(DMA_CH0);           //清除通道传输中断标志位  DMA传输完成，说明采集也完成了
  if(imgprocessed==1)
  {
    imgprocessed=0;
    for(i=0;i<CAMERA_SIZE;i++)
    {
     *(imgDrawbuff+i)=*(imgbuff+i);
    }
  }

}


uint8 camera_cfgnum = ARR_SIZE( camera_reg ) ; /*结构体数组成员数目*/


/*
 *  @return     初始化结果（0表示失败，1表示成功）
 */
uint8 camera_reg_init(void)
{
    uint16 i = 0;
    uint8 Sensor_IDCode = 0;
    SCCB_GPIO_init();

    if( 0 == SCCB_WriteByte ( OV7725_COM7, 0x80 ) ) //复位传感器
    {
        DEBUG_PRINTF("\n警告:SCCB写数据错误");
        return 0 ;
    }


    if( 0 == SCCB_ReadByte( &Sensor_IDCode, 1, OV7725_VER ) )   //读取传感器ID
    {
        DEBUG_PRINTF("\n警告:读取ID失败");
        return 0;
    }
    DEBUG_PRINTF("\nGet ID success，SENSOR ID is 0x%x", Sensor_IDCode);
    DEBUG_PRINTF("\nConfig Register Number is %d ", camera_cfgnum);
    if(Sensor_IDCode == OV7725_ID)
    {
        for( i = 0 ; i < camera_cfgnum ; i++ )
        {
            if( 0 == SCCB_WriteByte(camera_reg[i].addr, camera_reg[i].val) )
            {
                DEBUG_PRINTF("\n警告:写寄存器0x%x失败", camera_reg[i].addr);
                return 0;
            }
        }
    }
    else
    {
        return 0;
    }
    DEBUG_PRINTF("\nOV7725 Register Config Success!");
    return 1;
}

/*!
 *  @brief      二值化图像解压
 */
void img_extract(uint8 *dst, uint8 *src, uint32 srclen)
{
    uint8 colour[2] = {1, 0};//01
    uint8 tmpsrc;
    while(srclen --)
    {
        tmpsrc = *src++;
        *dst++ = colour[ (tmpsrc >> 7 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 6 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 5 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 4 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 3 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 2 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 1 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 0 ) & 0x01 ];
    }
}

