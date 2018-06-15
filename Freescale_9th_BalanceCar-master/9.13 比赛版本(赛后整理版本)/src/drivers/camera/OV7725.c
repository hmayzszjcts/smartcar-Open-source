#include "define.h"
#include "common.h"
#include "gpio.h"
#include "port.h"
#include "dma.h"
#include "SCCB.h"
#include "OV7725.h"
#include "OV7725_REG.h"


unsigned char Image_fire[CAMERA_H+1][CAMERA_DMA_NUM]={{0}};
volatile u8 img_flag = IMG_FINISH;		//图像状态



void Ov7725_exti_Init()
{
    //FIFO端口初始化  PB8~15
    gpio_init(PORTB,16, GPI, HIGH);
    gpio_init(PORTB,17, GPI, HIGH);
    gpio_init(PORTB,18, GPI, HIGH);
    gpio_init(PORTB,19, GPI, HIGH);
    gpio_init(PORTB,20, GPI, HIGH);
    gpio_init(PORTB,21, GPI, HIGH);
    gpio_init(PORTB,22, GPI, HIGH);
    gpio_init(PORTB,23, GPI, HIGH);
    //DMA通道0初始化，PTB8上升沿触发DMA传输，源地址为PTD_BYTE0_IN，目的地址为：BUFF ，每次传输1Byte，传输CAMERA_SIZE次后停止传输
    DMA_PORTx2BUFF_Init(CAMERA_DMA_CH, (void *)&PTB_BYTE2_IN, (void *)(Image_fire), PTE27, DMA_BYTE1, CAMERA_SIZE , DMA_falling);//KEEPON
    port_init(PTE27,DMA_RISING | PULLUP );    //PCLK
    DMA_IRQ_EN(DMA_CH0);
    port_init(PTD14, IRQ_RISING| PULLUP | PF);    //场中断，下拉，下降沿触发中断，带滤波
    disable_irq(90); 			          //关闭PTA的中断
}

void ov7725_get_img()
{
    img_flag = IMG_START;		//开始采集图像
    PORTD_ISFR=~0;			//写1清中断标志位(必须的，不然回导致一开中断就马上触发中断)
    enable_irq(90); 			//允许PTA的中断
    while(img_flag != IMG_FINISH)       //等待图像采集完毕
    {
        if(img_flag == IMG_FAIL)        //假如图像采集错误，则重新开始采集
        {
            img_flag = IMG_START;	//开始采集图像
            PORTD_ISFR=~0;		//写1清中断标志位(必须的，不然回导致一开中断就马上触发中断)
            enable_irq(90); 		//允许PTA的中断
        }
    }
}


/*OV7725初始化配置表*/
Register_Info ov7727_reg[] =
{
    //寄存器，寄存器值次
    {COM4         ,0xC1},
    {CLKRC        ,0x00}, 
    {COM2         ,0x03}, 
    {COM3         ,0xD0}, 
    {COM7         ,0x40},  
    {HSTART       ,0x3F},
    {HSIZE        ,0x50}, 
    {VSTRT        ,0x03},
    {VSIZE        ,0x78},
    {HREF         ,0x00}, 
    {SCAL0        ,0x0A},
    {AWB_Ctrl0    ,0xE0},
    {DSPAuto      ,0xff},
    {DSP_Ctrl2    ,0x0C}, 
    {DSP_Ctrl3    ,0x00}, 
    {DSP_Ctrl4    ,0x00},
    
#if (CAMERA_W == 80)
{HOutSize     ,0x14},
#elif (CAMERA_W == 160)  
{HOutSize     ,0x28},
#elif (CAMERA_W == 240)  
{HOutSize     ,0x3c},
#elif (CAMERA_W == 320)  
{HOutSize     ,0x50}, 
#else

#endif

#if (CAMERA_H == 60 )
{VOutSize     ,0x1E},
#elif (CAMERA_H == 120 )
{VOutSize     ,0x3c},
#elif (CAMERA_H == 180 )
{VOutSize     ,0x5a},
#elif (CAMERA_H == 240 )
{VOutSize     ,0x78},
#else

#endif 

{EXHCH        ,0x00},
{GAM1         ,0x0c},
{GAM2         ,0x16},
{GAM3         ,0x2a},
{GAM4         ,0x4e},
{GAM5         ,0x61},
{GAM6         ,0x6f},
{GAM7         ,0x7b},
{GAM8         ,0x86},
{GAM9         ,0x8e},
{GAM10        ,0x97},
{GAM11        ,0xa4},
{GAM12        ,0xaf},
{GAM13        ,0xc5},
{GAM14        ,0xd7},
{GAM15        ,0xe8},
{SLOP         ,0x20},
{LC_RADI      ,0x00},
{LC_COEF      ,0x13},
{LC_XC        ,0x08},
{LC_COEFB     ,0x14},
{LC_COEFR     ,0x17},
{LC_CTR       ,0x05},
{BDBase       ,0x99},
{BDMStep      ,0x03},
{SDE          ,0x04},
{BRIGHT       ,0x00},                              
{CNST         ,80},//150:楼上楼下都能跑（第一个车稳）70
{SIGN         ,0x06},
{UVADJ0       ,0x11},
{UVADJ1       ,0x02},
};

u8 cfgnum = sizeof(ov7727_reg)/sizeof(ov7727_reg[0]);   /*结构体数组成员数目*/



/************************************************
* 函数名：Ov7725_Init
* 描述  ：Sensor初始化
* 输入  ：无
* 输出  ：返回1成功，返回0失败
* 注意  ：无
************************************************/
u8 Ov7725_Init()
{
    while(Ov7725_reg_Init() == 0);
    Ov7725_exti_Init();
    return 0;
}

/************************************************
* 函数名：Ov7725_reg_Init
* 描述  ：Sensor 寄存器 初始化
* 输入  ：无
* 输出  ：返回1成功，返回0失败
* 注意  ：无
************************************************/
u8 Ov7725_reg_Init(void)
{
    u16 i = 0;
    u8 Sensor_IDCode = 0;
    SCCB_GPIO_init();
    OV7725_Delay_ms(50);
    while( 0 == SCCB_WriteByte ( 0x12, 0x80 ) ) /*复位sensor */
    {
        i++;
        if(i == 20)
        {
            printf("警告:SCCB写数据错误");
            //OV7725_Delay_ms(50);
            return 0 ;
        }
    }
    OV7725_Delay_ms(50);
    if( 0 == SCCB_ReadByte( &Sensor_IDCode, 1, 0x0b ) )	 /* 读取sensor ID号*/
    {
        printf("警告:读取ID失败");
        return 0;
    }
    printf("Get ID success，SENSOR ID is 0x%x", Sensor_IDCode);
    printf("Config Register Number is %d ", cfgnum);
    if(Sensor_IDCode == OV7725_ID)
    {
        for( i = 0 ; i < cfgnum ; i++ )
        {
            if( 0 == SCCB_WriteByte(ov7727_reg[i].Address, ov7727_reg[i].Value) )
            {
                printf("警告:写寄存器0x%x失败", ov7727_reg[i].Address);
                return 0;
            }
        }
    }
    else
    {
        return 0;
    }
    printf("OV7725 Register Config Success!");
    return 1;
}


