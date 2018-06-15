/*********************************************************************************************************************
 * 
 * @file       		common.h
 *  		
 * @core			S9KEA128
 * @date       		2018
 ********************************************************************************************************************/
 
 
#ifndef _common_h
#define _common_h


//数据类型声明
typedef unsigned char       uint8;   //  8 bits 
typedef unsigned short int  uint16;  // 16 bits 
typedef unsigned long int   uint32;  // 32 bits 
typedef unsigned long long  uint64;  // 64 bits 

typedef char                int8;    //  8 bits 
typedef short int           int16;   // 16 bits 
typedef long  int           int32;   // 32 bits 
typedef long  long          int64;   // 64 bits 

typedef volatile int8       vint8;   //  8 bits 
typedef volatile int16      vint16;  // 16 bits 
typedef volatile int32      vint32;  // 32 bits 
typedef volatile int64      vint64;  // 64 bits 

typedef volatile uint8      vuint8;  //  8 bits 
typedef volatile uint16     vuint16; // 16 bits 
typedef volatile uint32     vuint32; // 32 bits 
typedef volatile uint64     vuint64; // 64 bits 




typedef enum //枚举端口状态
{
    GPI = 0, //定义管脚输入方向      
    GPO = 1, //定义管脚输出方向
}GPIO_MOD;


typedef enum // 枚举管脚编号 
{
    //  A端口     //0~31
    A0,  A1,  A2,  A3,  A4,  A5,  A6,  A7,

    //  B端口     //32~63
    B0,  B1,  B2,  B3,  B4,  B5,  B6,  B7,

    //  C端口     //64~95
    C0,  C1,  C2,  C3,  C4,  C5,  C6,  C7,

    //  D端口     //96~127
    D0,  D1,  D2,  D3,  D4,  D5,  D6,  D7,

    //  E端口     //128~159
    E0,  E1,  E2,  E3,  E4,  E5,  E6,  E7,
    
    //  F端口     //128~159
    F0,  F1,  F2,  F3,  F4,  F5,  F6,  F7,
    
    //  G端口     //128~159
    G0,  G1,  G2,  G3,  G4,  G5,  G6,  G7,
    
    //  H端口     //128~159
    H0,  H1,  H2,  H3,  H4,  H5,  H6,  H7,
    
    //  I端口     //128~159
    I0,  I1,  I2,  I3,  I4,  I5,  I6
} PTX_n;

#define PTX(PTX_n)  (PTX_n>>5)  //获取模块号 PTX_n/32
#define PTn(PTX_n)  (PTX_n&0x1f)//获取引脚号 PTX_n%32

extern uint32 ics_clk_khz;
extern uint32 ftm_clk_khz;
extern uint32 core_clk_khz;
extern uint32 bus_clk_khz;
void get_clk(void);


// Compiler Related Definitions 
#ifdef __CC_ARM                         // ARM Compiler 
    #define ALIGN(n)                    __attribute__((aligned(n)))
#elif defined (__IAR_SYSTEMS_ICC__)     // for IAR Compiler 
    #define PRAGMA(x)                   _Pragma(#x)
    #define ALIGN(n)                    PRAGMA(data_alignment=n)
#elif defined (__GNUC__)                // GNU GCC Compiler 
    #define ALIGN(n)                    __attribute__((aligned(n)))
#endif // Compiler Related Definitions 



#include "includefile.h"




#endif
