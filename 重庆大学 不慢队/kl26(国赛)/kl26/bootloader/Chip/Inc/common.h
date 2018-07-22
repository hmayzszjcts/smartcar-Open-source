/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       common.h
 * @brief      山外K60 平台常用类型声明和宏定义
 * @author     山外科技
 * @version    v5.0
 * @date       2013-06-26
 */

#ifndef _COMMON_H_
#define _COMMON_H_

/*
 * 数据类型声明
 */
typedef unsigned char       uint8;  /*  8 bits */
typedef unsigned short int  uint16; /* 16 bits */
typedef unsigned long int   uint32; /* 32 bits */
typedef unsigned long long  uint64; /* 64 bits */

typedef char                int8;   /*  8 bits */
typedef short int           int16;  /* 16 bits */
typedef long  int           int32;  /* 32 bits */
typedef long  long          int64;  /* 64 bits */

typedef volatile int8       vint8;  /*  8 bits */
typedef volatile int16      vint16; /* 16 bits */
typedef volatile int32      vint32; /* 32 bits */
typedef volatile int64      vint64; /* 64 bits */

typedef volatile uint8      vuint8;  /*  8 bits */
typedef volatile uint16     vuint16; /* 16 bits */
typedef volatile uint32     vuint32; /* 32 bits */
typedef volatile uint64     vuint64; /* 64 bits */

/*
 * 定义带位域的联合体类型
 */
typedef union
{
    uint32  DW;
    uint16  W[2];
    uint8   B[4];
    struct
    {
        uint32 b0: 1;
        uint32 b1: 1;
        uint32 b2: 1;
        uint32 b3: 1;
        uint32 b4: 1;
        uint32 b5: 1;
        uint32 b6: 1;
        uint32 b7: 1;
        uint32 b8: 1;
        uint32 b9: 1;
        uint32 b10: 1;
        uint32 b11: 1;
        uint32 b12: 1;
        uint32 b13: 1;
        uint32 b14: 1;
        uint32 b15: 1;
        uint32 b16: 1;
        uint32 b17: 1;
        uint32 b18: 1;
        uint32 b19: 1;
        uint32 b20: 1;
        uint32 b21: 1;
        uint32 b22: 1;
        uint32 b23: 1;
        uint32 b24: 1;
        uint32 b25: 1;
        uint32 b26: 1;
        uint32 b27: 1;
        uint32 b28: 1;
        uint32 b29: 1;
        uint32 b30: 1;
        uint32 b31: 1;
    };
} Dtype;    //sizeof(Dtype) 为 4

/*
 * 定义坐标结构体
 */
typedef struct
{
    uint16 x;
    uint16 y;
} Site_t;

/*
 * 定义矩形大小结构体
 */
typedef struct
{
    uint16 W;       //宽
    uint16 H;       //高
} Size_t;

#define TRUE    1
#define FALSE   0


/*
 * 中断号类型声明
 */
typedef enum
{
   NMI_IRQn                      = -14,                 /**< Non-maskable interrupt */
   Hard_Fault_IRQn               = -13,                 /**< Hard fault exception */
   Reserved4_IRQn                = -12,                 /**< Reserved interrupt 4 */
   Reserved5_IRQn                = -11,                 /**< Reserved interrupt 5 */
   Reserved6_IRQn                = -10,                 /**< Reserved interrupt 6 */
   Reserved7_IRQn                = -9,                  /**< Reserved interrupt 7 */
   Reserved8_IRQn                = -8,                  /**< Reserved interrupt 8 */
   Reserved9_IRQn                = -7,                  /**< Reserved interrupt 9 */
   Reserved10_IRQn               = -6,                  /**< Reserved interrupt 10 */
   SVCall_IRQn                   = -5,                  /**< A supervisor call exception */
   Reserved12_IRQn               = -4,                  /**< Reserved interrupt 12 */
   Reserved13_IRQn               = -3,                  /**< Reserved interrupt 13 */
   PendableSrvReq_IRQn           = -2,                  /**< PendSV exception - request for system level service */
   SysTick_IRQn                  = -1,                  /**< SysTick interrupt */

   DMA0_IRQn                     =  0,                  /**< DMA channel 0 transfer complete/error interrupt */
   DMA1_IRQn                     =  1,                  /**< DMA channel 1 transfer complete/error interrupt */
   DMA2_IRQn                     =  2,                  /**< DMA channel 2 transfer complete/error interrupt */
   DMA3_IRQn                     =  3,                  /**< DMA channel 3 transfer complete/error interrupt */
   Reserved20_IRQn               =  4,                  /**< Reserved interrupt 20 */
   FTFA_IRQn                     =  5,                  /**< FTFA command complete/read collision interrupt */
   LVD_LVW_IRQn                  =  6,                  /**< Low Voltage Detect, Low Voltage Warning */
   LLW_IRQn                      =  7,                  /**< Low Leakage Wakeup */
   I2C0_IRQn                     =  8,                  /**< I2C0 interrupt */
   I2C1_IRQn                     =  9,                  /**< I2C0 interrupt 25 */
   SPI0_IRQn                     = 10,                  /**< SPI0 interrupt */
   SPI1_IRQn                     = 11,                  /**< SPI1 interrupt */
   UART0_IRQn                    = 12,                  /**< UART0 status/error interrupt */
   UART1_IRQn                    = 13,                  /**< UART1 status/error interrupt */
   UART2_IRQn                    = 14,                  /**< UART2 status/error interrupt */
   ADC0_IRQn                     = 15,                  /**< ADC0 interrupt */
   CMP0_IRQn                     = 16,                  /**< CMP0 interrupt */
   TPM0_IRQn                     = 17,                  /**< TPM0 fault, overflow and channels interrupt */
   TPM1_IRQn                     = 18,                  /**< TPM1 fault, overflow and channels interrupt */
   TPM2_IRQn                     = 19,                  /**< TPM2 fault, overflow and channels interrupt */
   RTC_IRQn                      = 20,                  /**< RTC interrupt */
   RTC_Seconds_IRQn              = 21,                  /**< RTC seconds interrupt */
   PIT_IRQn                      = 22,                  /**< PIT timer interrupt */
   I2S0_IRQn                     = 23,                  /**< I2S0 transmit interrupt */
   USB0_IRQn                     = 24,                  /**< USB0 interrupt */
   DAC0_IRQn                     = 25,                  /**< DAC0 interrupt */
   TSI0_IRQn                     = 26,                  /**< TSI0 interrupt */
   MCG_IRQn                      = 27,                  /**< MCG interrupt */
   LPTMR_IRQn                    = 28,                  /**< LPTimer interrupt */
   Reserved45_IRQn               = 29,                  /**< Reserved interrupt 45 */
   PORTA_IRQn                    = 30,                  /**< Port A interrupt */
   PORTC_PORTD_IRQn              = 31                   /**< Port C and port D interrupt */
} IRQn_t, IRQn_Type;


/*
 * 中断向量表编号声明
 */
typedef enum
{
   Initial_Stack_Pointer_VECTORn    = 0,                /**< Initial stack pointer */
   Initial_Program_Counter_VECTORn  = 1,                /**< Initial program counter */
   NMI_VECTORn                      = 2,                /**< Non-maskable interrupt */
   Hard_Fault_VECTORn               = 3,                /**< Hard fault exception */
   Reserved4_VECTORn                = 4,                /**< Reserved interrupt 4 */
   Reserved5_VECTORn                = 5,                /**< Reserved interrupt 5 */
   Reserved6_VECTORn                = 6,                /**< Reserved interrupt 6 */
   Reserved7_VECTORn                = 7,                /**< Reserved interrupt 7 */
   Reserved8_VECTORn                = 8,                /**< Reserved interrupt 8 */
   Reserved9_VECTORn                = 9,                /**< Reserved interrupt 9 */
   Reserved10_VECTORn               = 10,               /**< Reserved interrupt 10 */
   SVCall_VECTORn                   = 11,               /**< A supervisor call exception */
   Reserved12_VECTORn               = 12,               /**< Reserved interrupt 12 */
   Reserved13_VECTORn               = 13,               /**< Reserved interrupt 13 */
   PendableSrvReq_VECTORn           = 14,               /**< PendSV exception - request for system level service */
   SysTick_VECTORn                  = 15,               /**< SysTick interrupt */
   DMA0_VECTORn                     = 16,               /**< DMA channel 0 transfer complete/error interrupt */
   DMA1_VECTORn                     = 17,               /**< DMA channel 1 transfer complete/error interrupt */
   DMA2_VECTORn                     = 18,               /**< DMA channel 2 transfer complete/error interrupt */
   DMA3_VECTORn                     = 19,               /**< DMA channel 3 transfer complete/error interrupt */
   Reserved20_VECTORn               = 20,               /**< Reserved interrupt 20 */
   FTFA_VECTORn                     = 21,               /**< FTFA command complete/read collision interrupt */
   LVD_LVW_VECTORn                  = 22,               /**< Low Voltage Detect, Low Voltage Warning */
   LLW_VECTORn                      = 23,               /**< Low Leakage Wakeup */
   I2C0_VECTORn                     = 24,               /**< I2C0 interrupt */
   I2C1_VECTORn                     = 25,               /**< I2C0 interrupt 25 */
   SPI0_VECTORn                     = 26,               /**< SPI0 interrupt */
   SPI1_VECTORn                     = 27,               /**< SPI1 interrupt */
   UART0_VECTORn                    = 28,               /**< UART0 status/error interrupt */
   UART1_VECTORn                    = 29,               /**< UART1 status/error interrupt */
   UART2_VECTORn                    = 30,               /**< UART2 status/error interrupt */
   ADC0_VECTORn                     = 31,               /**< ADC0 interrupt */
   CMP0_VECTORn                     = 32,               /**< CMP0 interrupt */
   TPM0_VECTORn                     = 33,               /**< TPM0 fault, overflow and channels interrupt */
   TPM1_VECTORn                     = 34,               /**< TPM1 fault, overflow and channels interrupt */
   TPM2_VECTORn                     = 35,               /**< TPM2 fault, overflow and channels interrupt */
   RTC_VECTORn                      = 36,               /**< RTC interrupt */
   RTC_Seconds_VECTORn              = 37,               /**< RTC seconds interrupt */
   PIT_VECTORn                      = 38,               /**< PIT timer interrupt */
   I2S0_VECTORn                     = 39,               /**< I2S0 transmit interrupt */
   USB0_VECTORn                     = 40,               /**< USB0 interrupt */
   DAC0_VECTORn                     = 41,               /**< DAC0 interrupt */
   TSI0_VECTORn                     = 42,               /**< TSI0 interrupt */
   MCG_VECTORn                      = 43,               /**< MCG interrupt */
   LPTMR_VECTORn                    = 44,                 /**< LPTimer interrupt */
   Reserved45_VECTORn               = 45,               /**< Reserved interrupt 45 */
   PORTA_VECTORn                    = 46,               /**< Port A interrupt */
   PORTC_PORTD_VECTORn              = 47                /**< Port C and port D interrupt */
} VECTORn_t;

/*
 * 定义运行到RAM的函数
 */
#if defined(__ICCARM__)
//IAR 用 __ramfunc 来声明
#define     __RAMFUNC __ramfunc
#else
#define     __RAMFUN
#endif


/*
 * 包含Cortex-M内核的通用头文件
 */
#include    <stdio.h>                       //printf
#include    <string.h>                      //memcpy
#include    <stdlib.h>                      //malloc
#include "misc.h"

#define   __NVIC_PRIO_BITS    2       /* interrupt priority shift*/
#include "arm_math.h"

/*
 * 包含山外KL26平台的配置头文件
 */

#include "MKL_conf.h"

/*
 * 包含平台头文件
 */
#if defined(MKL26Z4)
#include "MKL26Z4.h"
#endif




/*
 * 包含常用头文件
 */
#include    "system_MKL.h"                  //系统配置
#include    "PORT_cfg.h"               //管脚复用配置
#include    "MKL_mcg.h"                    //K60 时钟模块

#if (defined(IAR))
#include "intrinsics.h"
#endif




/**
 *  @brief 变量的位清0和置1
 */
#define BIT_CLEAN(var,n)        (var) &= ~(1<<(n))   //变量var 的n位（即第n+1位）清0
#define BIT_SET(var,n)          (var) |=  (1<<(n))   //变量var 的n位（即第n+1位）置1
#define BIT_GET(var,n)          (((var)>>(n))&0x01)  //读取变量var 的n位（即第n+1位）

/**
 *  @brief 仿二进制赋值
 */
#define  HEX__(n)   0x##n##UL
#define  B8__(x)   ( (x & 0x0000000FUL) ? 1:0 )\
    +( (x & 0x000000F0UL) ? 2:0 )\
    +( (x & 0x00000F00UL) ? 4:0 )\
    +( (x & 0x0000F000UL) ? 8:0 )\
    +( (x & 0x000F0000UL) ? 16:0 )\
    +( (x & 0x00F00000UL) ? 32:0 )\
    +( (x & 0x0F000000UL) ? 64:0 )\
    +( (x & 0xF0000000UL) ? 128:0 )
#define  B8(x)                                     ((unsigned char)B8__(HEX__(x)))
#define  B16(x_msb,x_lsb)                (((unsigned int)B8(x_msb)<<8) + B8(x_lsb))
#define  B32(x_msb,x_2,x_3,x_lsb)   (((unsigned long)B8(x_msb)<<24) + ((unsigned long)B8(x_2)<<16) + ((unsigned long)B8(x_3)<<8) + B8(x_lsb))
/* Sample usage:
B8(01010101) = 85
B16(10101010,01010101) = 43605
B32(10000000,11111111,10101010,01010101) = 2164238933
*/

/*
 * 打印宏定义展开结果
 */
#define _MKSTR(str)  #str
#define MKSTR(str)  _MKSTR(str)                       //把宏定义展开结果转换为字符串

#define DEFINE_PRINTF(def)  printf(MKSTR(def)"\n")    //串口打印宏定义展开结果


/*
 * 求最大值和最小值
 */
#define MAX( x, y ) ( ((x) > (y)) ? (x) : (y) )
#define MIN( x, y ) ( ((x) < (y)) ? (x) : (y) )


/*
 * 返回数组元素的个数
 */
#define ARR_SIZE( a ) ( sizeof( (a) ) / sizeof( ((a)[0]) ) )

/*
 * 宏定义实现返回绝对值（x里不能有自加自减的语句，否则变量出错）
 */
#define ABS(x) (((x) > 0) ? (x) : (-(x)))

/*
 * 获取结构体某成员偏移
 */
#define OFFSET(type, member)    (uint32)(&(((type *)0)->member))

/*
 * 确保x的范围为 min~max
 */
#define RANGE(x,max,min)        ((uint8)((x)<(min) ? (min) : ( (x)>(max) ? (max):(x) )))

/*
 * 交换32位数据的4字节顺序
 */
#define SWAP32(data)    (uint32)((((uint32)(data) & (0xFFu<<0 ))<<24)      \
                          | (((uint32)(data) & (0xFFu<<8 ))<<8)            \
                          | (((uint32)(data) & (0xFFu<<16))>>8)            \
                          | (((uint32)(data) & (0xFFu<<24))>>24)           \
                        )

/*
 * 交换16位数据的2字节顺序
 */
#define SWAP16(data)    (uint32)((((uint16)(data) & (0xFF<<0 ))<<8)      \
                          | (((uint32)(data) & (0xFF<<8))>>8)            \
                        )

/*
 * 交换 x, y 的值
 */
#define SWAP(x,y)           do{x^=y;y^=x;x^=y;}while(0)


/********************************************************************/

#endif /* _COMMON_H_ */
