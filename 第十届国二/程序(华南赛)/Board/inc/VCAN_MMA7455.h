/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       VCAN_MMA7455.h
 * @brief      KEY驱动头文件
 * @author     山外科技
 * @version    v5.0
 * @date       2013-07-10
 */

#ifndef __VCAN_MMA7455_H__
#define __VCAN_MMA7455_H__


#define     MMA7455_DEVICE          I2C0        //定义MMA7455 所用的接口 为 I2C0

#define     MMA7455_ADRESS          (0x1D)      /*MMA7455_Device Address*/

/* MMA7455 Register Address ------------------------------------------------------------*/
#define     MMA7455_XOUTL   0x00    //00  10 bits output value X LSB (Read only)
#define     MMA7455_XOUTH   0x01    //01  10 bits output value X MSB (Read only)
#define     MMA7455_YOUTL   0x02    //02  10 bits output value Y LSB (Read only)
#define     MMA7455_YOUTH   0x03    //03  10 bits output value Y MSB (Read only)
#define     MMA7455_ZOUTL   0x04    //04  10 bits output value Z LSB (Read only)
#define     MMA7455_ZOUTH   0x05    //05  10 bits output value Z MSB (Read only)
#define     MMA7455_XOUT8   0x06    //06   8 bits Output value X (Read only)
#define     MMA7455_YOUT8   0x07    //07   8 bits Output value Y (Read only)
#define     MMA7455_ZOUT8   0x08    //08   8 bits Output value Z (Read only)
#define     MMA7455_STATUS  0x09    //09  Status registers (Read only)
#define     MMA7455_DETSRC  0x0A    //10  Detection source registers (Read only)
#define     MMA7455_TOUT    0x0B    //11  Temperature output value (Optional)

#define     MMA7455_I2CAD   0x0D    //13  I2C device address (Bit[6:0]: Read only, Bit[7]: Read/Write)
#define     MMA7455_USRINF  0x0E    //14  User information  (Optional, Read only)
#define     MMA7455_WHOAMI  0x0F    //15  Who am I value (Optional, Read only)
#define     MMA7455_XOFFL   0x10    //16  Offset drift X value (LSB) (Read/Write)
#define     MMA7455_XOFFH   0x11    //17  Offset drift X value (MSB) (Read/Write)
#define     MMA7455_YOFFL   0x12    //18  Offset drift Y value (LSB) (Read/Write)
#define     MMA7455_YOFFH   0x13    //19  Offset drift Y value (MSB) (Read/Write)
#define     MMA7455_ZOFFL   0x14    //20  Offset drift Z value (LSB) (Read/Write)
#define     MMA7455_ZOFFH   0x15    //21  Offset drift Z value (MSB) (Read/Write)
#define     MMA7455_MCTL    0x16    //22  Mode control (Read/Write)
#define     MMA7455_INTRST  0x17    //23  Interrupt latch reset (Read/Write)
#define     MMA7455_CTL1    0x18    //24  Control 1 (Read/Write)
#define     MMA7455_CTL2    0x19    //25  Control 2 (Read/Write)
#define     MMA7455_LDTH    0x1A    //26  Level detection threshold limit value (Read/Write)
#define     MMA7455_PDTH    0x1B    //27  Pulse detection threshold limit value (Read/Write)
#define     MMA7455_PW      0x1C    //28  Pulse duration value (Read/Write)
#define     MMA7455_LT      0x1D    //29  Latency time value  (Read/Write)
#define     MMA7455_TW      0x1E    //30  Time window for second pulse value(Read/Write)

//函数声明
extern void  mma7455_init(void);                        //初始化MMA7455
extern void  mma7455_write_reg(uint8 reg, uint8 Data);  //写MMA7455寄存器
extern uint8 mma7455_read_reg(uint8 reg);               //读MMA7455寄存器


#endif  //__VCAN_MMA7455_H__