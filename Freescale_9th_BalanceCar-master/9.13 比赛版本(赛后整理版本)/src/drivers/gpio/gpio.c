/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：gpio.c
 * 描述         ：gpio驱动函数
 *
 * 实验平台     ：野火kinetis开发板
 * 库版本       ：
 * 嵌入系统     ：
 *
 * 作者         ：野火嵌入式开发工作室
 * 淘宝店       ：http://firestm32.taobao.com
 * 技术支持论坛 ：http://www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
**********************************************************************************/

#include "common.h"
#include "gpio.h"


volatile struct GPIO_MemMap *GPIOx[5] = {PTA_BASE_PTR, PTB_BASE_PTR, PTC_BASE_PTR, PTD_BASE_PTR, PTE_BASE_PTR}; //定义五个指针数组保存 GPIOx 的地址
volatile struct PORT_MemMap *PORTX[5] = {PORTA_BASE_PTR, PORTB_BASE_PTR, PORTC_BASE_PTR, PORTD_BASE_PTR, PORTE_BASE_PTR};

/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：gpio_init
*  功能说明：初始化gpio
*  参数说明：PORTx       端口号（PORTA,PORTB,PORTC,PORTD,PORTE）
*            n           端口引脚
*            IO          引脚方向,0=输入,1=输出
*            data        输出初始状态,0=低电平,1=高电平 （对输入无效）
*  函数返回：无
*  修改时间：2012-1-15   已测试
*  备    注：
*************************************************************************/
void gpio_init (PORTx portx, u8 n, GPIO_CFG cfg, u8 data)
{
    ASSERT( (n < 32u)  && (data < 2u) );           //使用断言检查输入、电平 是否为1bit

    //选择功能脚 PORTx_PCRx ，每个端口都有个寄存器 PORTx_PCRx
    PORT_PCR_REG(PORTX[portx], n) = (0 | PORT_PCR_MUX(1) | cfg);

    //端口方向控制输入还是输出
    if( ( (cfg & 0x01) == GPI) || (cfg == GPI_UP) ||     (cfg == GPI_UP_PF)      )
        //   最低位为0则输入   ||   输入上拉模式  ||   输入上拉，带无源滤波器
    {
        GPIO_PDDR_REG(GPIOx[portx]) &= ~(1 << n);  //设置端口方向为输入
    }
    else
    {
        GPIO_PDDR_REG(GPIOx[portx]) |= (1 << n);    //设置端口方向为输出
        if(data == 1)//output
        {
            GPIO_SET(portx, n, 1);                  //对端口输出控制，输出为1
        }
        else
        {
            GPIO_SET(portx, n, 0);                  //对端口输出控制，输出为0
        }
    }
}

/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：gpio_set
*  功能说明：设置引脚状态
*  参数说明：PORTx       端口号（PORTA,PORTB,PORTC,PORTD,PORTE）
*            n           端口引脚
*            data        输出初始状态,0=低电平,1=高电平
*  函数返回：无
*  修改时间：2012-1-16   已测试
*  备    注：
*************************************************************************/
void gpio_set (PORTx portx, u8 n, u8 data)
{
    ASSERT( (n < 32u)  && (data < 2u) );           //使用断言检查输入、电平 是否为1bit

    if(data == 1)               //输出
        GPIO_SET(portx, n, 1);  //GPIO_PDOR_REG(PORTx) |= (1<<n);
    else
        GPIO_SET(portx, n, 0);  //GPIO_PDOR_REG(PORTx) &= ~(1<<n);
}

/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：gpio_turn
*  功能说明：反转引脚状态
*  参数说明：PORTx       端口号（PORTA,PORTB,PORTC,PORTD,PORTE）
*            n           端口引脚
*  函数返回：无
*  修改时间：2012-1-15   已测试
*  备    注：
*************************************************************************/
void gpio_turn (PORTx portx, u8 n)
{
    ASSERT( n < 32u );           //使用断言检查引脚号
    GPIO_TURN(portx, n);
}

/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：gpio_get
*  功能说明：读取引脚输入状态
*  参数说明：PORTx       端口号（PORTA,PORTB,PORTC,PORTD,PORTE）
*            n           端口引脚
*  函数返回：无
*  修改时间：2012-1-15   已测试
*  备    注：
*************************************************************************/
u8 gpio_get(PORTx portx, u8 n)          //读取引脚状态
{
    ASSERT( n < 32u );                  //使用断言检查引脚号
    return GPIO_Get(portx, n);
}
