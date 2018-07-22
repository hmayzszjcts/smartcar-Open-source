/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       port_cfg.h
 * @brief      山外K60 复用管脚配置
 * @author     山外科技
 * @version    v5.1
 * @date       2013-06-26
 */
#ifndef _PORT_CFG_H_
#define _PORT_CFG_H_

#include "MK60_port.h"

/**********************************  UART   ***************************************/

//      模块通道    端口          可选范围                          建议
#define UART0_RX_PIN    PTA15//PTD6        //PTA1、PTA15、PTB16、PTD6          PTA1不要用（与Jtag冲突）
#define UART0_TX_PIN   PTA14//PTD7        //PTA2、PTA14、PTB17、PTD7          PTA2不要用（与Jtag冲突）

#define UART1_RX_PIN    PTC3        //PTC3、PTE1
#define UART1_TX_PIN    PTC4        //PTC4、PTE0

#define UART2_RX_PIN    PTD2        //PTD2
#define UART2_TX_PIN    PTD3        //PTD3

#define UART3_RX_PIN    PTC16       //PTB10、PTC16、PTE5
#define UART3_TX_PIN    PTC17       //PTB11、PTC17、PTE4

#define UART4_RX_PIN    PTC14       //PTC14、PTE25
#define UART4_TX_PIN    PTC15       //PTC15、PTE24

#define UART5_RX_PIN    PTE9        //PTD8、PTE9
#define UART5_TX_PIN    PTE8        //PTD9、PTE8

/**********************************  FTM    ***************************************/

//      模块通道    端口          可选范围              建议
#define FTM0_CH0_PIN    PTC1        //PTC1、PTA3            PTA3不要用（与Jtag、SWD冲突）
#define FTM0_CH1_PIN    PTA4        //PTC2、PTA4
#define FTM0_CH2_PIN    PTA5        //PTC3、PTA5
#define FTM0_CH3_PIN    PTA6        //PTC4、PTA6
#define FTM0_CH4_PIN    PTA7        //PTD4、PTA7
#define FTM0_CH5_PIN    PTD5        //PTD5、PTA0            PTA0不要用（与Jtag、SWD冲突）
#define FTM0_CH6_PIN    PTD6        //PTD6、PTA1            PTA1不要用（与Jtag冲突）
#define FTM0_CH7_PIN    PTD7        //PTD7、PTA2            PTA2不要用（与Jtag冲突）


//      模块通道    端口          可选范围              建议
#define FTM1_CH0_PIN    PTA12       //PTA8、PTA12、PTB0
#define FTM1_CH1_PIN    PTA13       //PTA9、PTA13、PTB1

//      模块通道    端口          可选范围              建议
#define FTM2_CH0_PIN    PTA10       //PTA10、PTB18
#define FTM2_CH1_PIN    PTA11       //PTA11、PTB19


#ifdef  MK60F15                 //ALT6       ALT3        ALT4        FX 才有FTM3模块

#define FTM3_CH0_PIN    PTE5        //PTE5                   PTD0
#define FTM3_CH1_PIN    PTE6        //PTE6                   PTD1
#define FTM3_CH2_PIN    PTE7        //PTE7                   PTD2
#define FTM3_CH3_PIN    PTE8        //PTE8                   PTD3
#define FTM3_CH4_PIN    PTE9        //PTE9       PTC8
#define FTM3_CH5_PIN    PTE10       //PTE10      PTC9
#define FTM3_CH6_PIN    PTE11       //PTE11      PTC10
#define FTM3_CH7_PIN    PTE12       //PTE12      PTC11

#endif

//正交解码模块通道  端口          可选范围              建议
#define FTM1_QDPHA_PIN  PTA12       //PTA8、PTA12、PTB0
#define FTM1_QDPHB_PIN  PTA13       //PTA9、PTA13、PTB1

#define FTM2_QDPHA_PIN  PTA10       //PTA10、PTB18
#define FTM2_QDPHB_PIN  PTA11       //PTA11、PTB19


/**********************************  I2C   ***************************************/

//      模块通道    端口          可选范围              建议
#define I2C0_SCL_PIN    PTD8        // PTB0、PTB2、PTD8
#define I2C0_SDA_PIN    PTD9        // PTB1、PTB3、PTD9

#define I2C1_SCL_PIN    PTC10       // PTE1、PTC10
#define I2C1_SDA_PIN    PTC11       // PTE0、PTC11


/**********************************  SPI   ***************************************/
//PCS接口，不用的时候需要注释，就不会进行初始化对应的管脚
//      模块通道    端口          可选范围                  建议

#define SPI0_SCK_PIN    PTA15       // PTA15、PTC5、PTD1        全部都是 ALT2
#define SPI0_SOUT_PIN   PTA16       // PTA16、PTC6、PTD2        全部都是 ALT2
#define SPI0_SIN_PIN    PTA17       // PTA17、PTC7、PTD3        全部都是 ALT2

#define SPI0_PCS0_PIN   PTA14       // PTA14、PTC4、PTD0、      全部都是 ALT2
#define SPI0_PCS1_PIN   PTC3        // PTC3、PTD4               全部都是 ALT2
#define SPI0_PCS2_PIN   PTC2        // PTC2、PTD5               全部都是 ALT2
#define SPI0_PCS3_PIN   PTC1        // PTC1、PTD6               全部都是 ALT2
#define SPI0_PCS4_PIN   PTC0        // PTC0、                   全部都是 ALT2
#define SPI0_PCS5_PIN   PTB23       // PTB23                    ALT3


#define SPI1_SCK_PIN    PTB11       // PTE2、PTB11、            全部都是 ALT2
#define SPI1_SOUT_PIN   PTB16       // PTE1、PTB16、            全部都是 ALT2
#define SPI1_SIN_PIN    PTB17       // PTE3、PTB17、            全部都是 ALT2

#define SPI1_PCS0_PIN   PTB10       // PTE4、PTB10、            全部都是 ALT2
#define SPI1_PCS1_PIN   PTE0        // PTE0、PTB9、             全部都是 ALT2
#define SPI1_PCS2_PIN   PTE5        // PTE5、                   全部都是 ALT2
#define SPI1_PCS3_PIN   PTE6        // PTE6、                   全部都是 ALT2


#define SPI2_SCK_PIN    PTB21       // PTB21、PTD12             全部都是 ALT2
#define SPI2_SOUT_PIN   PTB22       // PTB22、PTD13             全部都是 ALT2
#define SPI2_SIN_PIN    PTB23       // PTB23、PTD14             全部都是 ALT2
#define SPI2_PCS0_PIN   PTB20       // PTB20、PTD11             全部都是 ALT2
#define SPI2_PCS1_PIN   PTD15       // PTD15                    全部都是 ALT2


/**********************************  CAN   ***************************************/
#define CAN0_TX_PIN     PTA12       //PTA12、PTB18              全部都是 ALT2
#define CAN0_RX_PIN     PTA13       //PTA13、PTB19              全部都是 ALT2

#define CAN1_TX_PIN     PTE24       //PTE24、PTC17              全部都是 ALT2
#define CAN1_RX_PIN     PTE25       //PTE25、PTC16              全部都是 ALT2


#endif  //_PORT_CFG_H_


