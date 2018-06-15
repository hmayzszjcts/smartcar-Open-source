/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：main.c
 * 描述         ：I2C通信实验
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
#include "include.h"


/*************************************************************************
*                             野火嵌入式开发工作室
*                               I2C通信实验
*
*  实验说明：野火 I2C通信实验，往 AT24C02 写入数据，再读出来，通过串口来显示。
*            野火串口默认为： UART1，TX接PTC4，RX接PTC3
*            k60_fire.h里定义了 printf 函数的输出设置：
*                #define FIRE_PORT           UART1
*                #define FIRE_BAUD           19200
*
*  实验操作：打开串口助手 ，设置波特率为 19200 。
*            串口线（经过MAX3232电平转换）：TX接PTC4，RX接PTC3 。
*            运行程序后，在串口助手里看到。
*
*  实验效果：串口助手里显示数据：
*
*      		野火kinetis核心板测试程序
*       内核频率：200MHz	总线频率 ：66MHz
*       flex频率：66MHz 	flash频率：28MHz
*
*       Software Reset
*
*       K60-144pin      Silicon rev 1.2
*       Flash parameter version 0.0.7.0
*       Flash version ID 3.2.7.0
*       512 kBytes of P-flash	P-flash only
*       128 kBytes of RAM
*
*       AT24C02 I2C 实验
*
*        		——野火kinetis开发板
*
*       接收到的数据为：0
*
*       接收到的数据为：1
*
*       ......
*
*
*  实验目的：测试 I2C 通信功能
*
*  修改时间：2012-2-29     已测试
*
*  备    注：
*
*************************************************************************/

#if 1           //这里选择 0 或 1 来切换编译两个不同的main函数，两个都是一样的功能
// 第一个 用I2C里的函数
// 第二个 用AT24C02里的函数
// 两种都是相同的

void main()
{
#define ADDR    0x00
    u8 i = 0;
    u8 Data;
    uart_init(UART1, 19600);                        //初始化串口

    I2C_init(I2C0);                                 //初始化I2C0

    printf("AT24C02 I2C 实验\n\n");
    printf("\t\t——野火kinetis开发板\n\n");

    while(1)
    {
        for(i = 0; i < 255; i++)
        {
            I2C_WriteAddr(I2C0, AT24C02_I2C_ADDRESS, ADDR, i);          //I2C向AT24C02_I2C_ADDRESS芯片写入数据 i 到地址为ADDR的寄存器
            Data    =   I2C_ReadAddr(I2C0, AT24C02_I2C_ADDRESS, ADDR);  //I2C向AT24C02_I2C_ADDRESS芯片读取寄存器地址为ADDR的数据

            printf("接收到的数据为：%d\n\n", Data);                     //发送到串口显示出来

            time_delay_ms(1000);                                        //延时1s
        }
    }
#undef  ADDR
}

#else           //两个是一样的，前者是后者宏定义的展开

void main()
{
#define ADDR    0x00
    u8 i = 0;
    u8 Data;
    uart_init(UART1, 19600);                    //初始化串口

    AT24C02_init();                             //初始化AT24C02，启动I2C总线

    printf("AT24C02 I2C 实验\n\n");
    printf("\t\t——野火kinetis开发板\n\n");
    while(1)
    {
        for(i = 0; i < 255; i++)
        {
            AT24C02_WriteByte(ADDR, i);         //向地址ADDR写入数据i

            Data = AT24C02_ReadByte(ADDR);      //读取地址ADDR的数据

            printf("接收到的数据为：%d\n\n", Data); //发送到串口显示出来

            time_delay_ms(1000);                //延时1s
        }
    }
#undef  ADDR
}

#endif
