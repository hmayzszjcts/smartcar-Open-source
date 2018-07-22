/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       VCAN_TSL1401.c
 * @brief      线阵CCD函数库
 * @author     山外科技
 * @version    v5.0
 * @date       2014-01-04
 */

#include "common.h"
#include "math.h"
#include <stdarg.h>
#include "MK60_port.h"
#include "MK60_gpio.h"
#include "MK60_adc.h"
#include "MK60_pit.h"
#include "VCAN_TSL1401.h"


static volatile tsl1401_status_e  tsl1401_flag = tsl1401_noint;
static volatile uint32            tsl1401_time;


static void tsl1401_restet();
static void tsl1401_delay(volatile uint32 time );
static void tsl1401_gather(void);

//配置CCD管脚
//                          CCD1            CCD2        CCD3

#if 1             //宏条件编译选择不同的信号类型
ADCn_Ch_e   tsl1401_ch[] = {ADC0_DP1,     ADC0_DP0,    ADC1_DP1};       //CCD所用的 ADC通道(原始信号)
#else
ADCn_Ch_e   tsl1401_ch[] = {ADC0_DM1,     ADC0_DM0,    ADC1_DM1};       //CCD所用的 ADC通道(放大信号)
#endif
PTXn_e      tsl1401_si[] = {PTE6 ,        PTE8 ,       PTE10};          //CCD所用的 SI管脚
PTXn_e      tsl1401_clk[] = {PTE7 ,        PTE9 ,       PTE12};         //CCD所用的 SI管脚
PTXn_e      tsl1401_led[] = {PTA8 ,        PTA9 ,       PTD15};         //CCD 补光 所用的 LED_EN 管脚

//CCD 采集回来的存储空间指针数组
uint8 *tsl1401_addr[TSL1401_MAX] = {0};                                 //CCD 采集图像的地址数组

//全部CCD的SI管脚输出数据
void tsl1401_si_out(uint8 data)
{
    uint8   i = TSL1401_MAX;

    ASSERT(data < 2);

    while(i--)
    {
        gpio_set(tsl1401_si[i], data);
    }
}

//全部CCD的CLK管脚输出数据
void tsl1401_clk_out(uint8 data)
{
    uint8   i = TSL1401_MAX;

    ASSERT(data < 2);

    while(i--)
    {
        gpio_set(tsl1401_clk[i], data);
    }
}

//tsl1401,线性CCD初始化，time为曝光时间，单位与 TSL1401_INT_TIME 的设置一致，初始化后必须调用tsl1401_set_addrs 来设置存储地址（或者先设置存储图像地址）
void tsl1401_init(uint32 time)
{
    uint8 i = TSL1401_MAX;

    while(i)
    {
        i--;
        adc_init(tsl1401_ch[i]); //初始化 CCD 所用的 ADC通道

        //初始化 CCD 的控制管脚 CLK 和 SI
        gpio_init (tsl1401_clk[i], GPO, 0);
        gpio_init (tsl1401_si[i] , GPO, 0);
    }

    tsl1401_time = time;
    TSL1401_INT_TIME(tsl1401_time);         //  设置中断时间

    tsl1401_restet();                       //  丢弃第一帧的数据（乱的）
}

//设置采集图像的地址
//num 为 TSL1401_MAX 时，设置全部地址，否则设置指定的地址
void tsl1401_set_addrs(TSL1401_e num, uint8 *addr, ... )
{
    ASSERT(num <= TSL1401_MAX);

    va_list ap;                                 //创建栈指针ap
    uint8_t *value;
    uint8_t n = 0;

    if(num < TSL1401_MAX)
    {
        tsl1401_addr[num] = addr;
    }
    else if(num == TSL1401_MAX)
    {
        va_start(ap, addr);                         //获取可变参数列表的第一个参数的地址
        value = addr;
        while(num--)
        {
            tsl1401_addr[n++] = value;
            value = va_arg(ap, uint8_t * );             //获取可变参数的当前参数，返回指定类型并将指针指向下一参数
        }

        va_end(ap);                                 //清空va_list可变参数列表
    }
}


//num 为 TSL1401_MAX 时，开启全部LED，否则开启对应的 LED
void tsl1401_led_en(TSL1401_e num)
{
    ASSERT(num <= TSL1401_MAX);

    if(num < TSL1401_MAX)
    {
        gpio_init(tsl1401_led[num], GPO, 0);
    }
    else if(num == TSL1401_MAX)
    {

        while(num--)
        {
            gpio_init(tsl1401_led[num], GPO, 0);
        }
    }
}

//num 为 TSL1401_MAX 时，开启全部LED，否则开启对应的 LED
void tsl1401_led_dis(TSL1401_e num)
{
    ASSERT(num <= TSL1401_MAX);

    if(num < TSL1401_MAX)
    {
        gpio_init(tsl1401_led[num], GPI, 1);
    }
    else if(num == TSL1401_MAX)
    {

        while(num--)
        {
            gpio_init(tsl1401_led[num], GPI, 1);
        }
    }
}

//tsl1401,线性CCD采集图像
void tsl1401_get_img(void)
{
    tsl1401_flag = tsl1401_start;           //启动采集
    while(tsl1401_flag != tsl1401_finish);  //等待采集完成
}

//获取 tsl1401,线性CCD 曝光时间
uint32 tsl1401_get_time(void)
{
    return tsl1401_time;
}

//设置 tsl1401,线性CCD 曝光时间
void tsl1401_set_time(uint32 time)
{
    tsl1401_time = time;
    TSL1401_INT_TIME(tsl1401_time);         //  设置中断时间
}

//放入定时中断里，以便于定时复位曝光时间
void tsl1401_time_isr()
{
    //只判断是否开始采集和是否采集完成
    if(tsl1401_flag == tsl1401_start)
    {
        tsl1401_gather();
        tsl1401_flag = tsl1401_finish;
    }
    else
    {
        tsl1401_restet();                   // 复位
    }
}

void tsl1401_delay( uint32 time )
{
    volatile uint32 i = time;

    while(i--);
}


//采集图像
void tsl1401_gather(void)
{
#define TSL1401_TIME   10   //延时时间

    uint8_t n = TSL1401_SIZE, k = 0;
    uint8_t i;

    tsl1401_clk_out(0);             //CLK = 0
    tsl1401_delay(TSL1401_TIME);
    tsl1401_si_out(1);              //SI  = 1
    tsl1401_delay(TSL1401_TIME);
    tsl1401_clk_out(1);             //CLK = 1
    tsl1401_delay(TSL1401_TIME);

    tsl1401_si_out(0);              //SI  = 0
    tsl1401_delay(TSL1401_TIME);

    while(n--)
    {

        tsl1401_clk_out(0);        //CLK = 0

        i = TSL1401_MAX;
        while(i--)
        {
            (tsl1401_addr[i])[k] = (uint8_t)adc_once(tsl1401_ch[i], ADC_8bit);
            //*img++ =  adc_once(ADC1_AD8, ADC_8bit);
        }

        tsl1401_clk_out(1);        //CLK = 1
        tsl1401_delay(TSL1401_TIME);
        k++;
    }
#undef TSL1401_TIME
}

//用于调整曝光时间
void tsl1401_restet()
{
#define TSL1401_RETIME   1

    uint8 n = TSL1401_SIZE;

    tsl1401_clk_out(0);             //CLK = 0
    tsl1401_delay(TSL1401_RETIME);
    tsl1401_si_out(1);              //SI  = 1
    tsl1401_delay(TSL1401_RETIME);
    tsl1401_clk_out(1);             //CLK = 1
    tsl1401_delay(TSL1401_RETIME);

    tsl1401_si_out(0);              //SI  = 0
    tsl1401_delay(TSL1401_RETIME);

    while(n--)
    {
        tsl1401_clk_out(0);             //CLK = 0
        tsl1401_delay(TSL1401_RETIME);
        tsl1401_clk_out(1);             //CLK = 1
        tsl1401_delay(TSL1401_RETIME);
    }
#undef TSL1401_RETIME
}


