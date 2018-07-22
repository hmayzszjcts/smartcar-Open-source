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
 * @brief      山外KL26 复用管脚配置
 * @author     山外科技
 * @version    v5.0
 * @date       2013-06-26
 */
#ifndef _PORT_CFG_H_
#define _PORT_CFG_H_

#include "MKL_port.h"

/**********************************  UART   ***************************************/

//      模块通道    端口          可选范围                                    建议
#define UART0_RX_PIN    PTA1      //PTA1、PTA15、PTB16、PTD6、PTE21
#define UART0_TX_PIN    PTA2       //PTA2、PTA14、PTB17、PTD7、PTE20

#define UART1_RX_PIN    PTC3        //PTA18、PTE1、PTC3                       PTA18不要用 （与晶振冲突）
#define UART1_TX_PIN    PTC4        //PTA19、PTE0、PTC4                       PTA19不要用 （与晶振冲突）

#define UART2_RX_PIN    PTD2        //PTD2、PTD4、PTE17、PTE23
#define UART2_TX_PIN    PTD3        //PTD3、PTD5、PTE16、PTE22



/**********************************  TPM    ***************************************/

#define TPM_CLKIN0_PIN  PTB16       //PTA18、PTB16、PTC12、PTE16、PTE29     PTA18不要用 （与晶振冲突）
#define TPM_CLKIN1_PIN  PTE17       //PTA19、PTB17、PTC13、PTE17、PTE30              PTA19不要用 （与晶振冲突）


//      模块通道    端口          可选范围                                    建议
#define TPM0_CH0_PIN    PTE24        //PTE24、PTA3、PTC1、PTD0                PTA3不要用（与SWD冲突）
#define TPM0_CH1_PIN    PTC2        //PTE25、PTA4、PTC2、PTD1
#define TPM0_CH2_PIN    PTC3        //PTE29、PTA5、PTC3、PTD2
#define TPM0_CH3_PIN    PTE30        //PTE30、PTA6、PTC4、PTD3
#define TPM0_CH4_PIN    PTC8       //PTE31、PTA7、PTC8、PTD4
#define TPM0_CH5_PIN    PTC9        //PTE26、PTA0、PTC9、PTD5                 PTA0不要用（与SWD冲突）


//      模块通道    端口          可选范围                                    建议
#define TPM1_CH0_PIN    PTA12       //PTA12、PTB0、PTE20
#define TPM1_CH1_PIN    PTA13       //PTA13、PTB1、PTE21

//      模块通道    端口          可选范围                                    建议
#define TPM2_CH0_PIN    PTA1       //PTA1、PTB2、PTB18、PTE22
#define TPM2_CH1_PIN    PTA2       //PTA2、PTB3、PTB19、PTE23



/**********************************  I2C   ***************************************/

//      模块通道    端口          可选范围                                    建议
#define I2C0_SCL_PIN    PTE18        // PTB0、PTB2、PTC8、PTE19、PTE24
#define I2C0_SDA_PIN    PTE19       // PTB1、PTB3、PTC9、PTE18、PTE25

#define I2C1_SCL_PIN    PTC10       // PTE1、PTA3、PTC1、PTC10                     PTA3不要用（与SWD冲突）
#define I2C1_SDA_PIN    PTC11       // PTE0、PTA4、PTC2、PTC11


/**********************************  SPI   ***************************************/
//PCS接口，不用的时候需要注释，就不会进行初始化对应的管脚
//      模块通道    端口          可选范围                  建议

#define SPI0_SCK_PIN    PTD1        // PTE17、PTA15、PTC5、PTD1
#define SPI0_SOUT_PIN   PTD2        // PTE18、PTE19、PTA16、PTA17、PTC6、PTC7、PTD2、PTD3
#define SPI0_SIN_PIN    PTD3        // PTE18、PTE19、PTA16、PTA17、PTC6、PTC7、PTD2、PTD3

#define SPI0_PCS0_PIN   PTD0        // PTE16、PTA14、PTC4、PTD0


#define SPI1_SOUT_PIN   PTE3        // PTB16、PTB17、PTD6、PTD7、PTE1、PTE3
#define SPI1_SCK_PIN    PTE2        // PTB9、PTB11、PTD5、PTE2
#define SPI1_SIN_PIN    PTE1        // PTB16、PTB17、PTD6、PTD7、PTE1、PTE3、PTE0
#define SPI1_PCS0_PIN   PTE4        // PTB8、PTB10、PTE4

/**********************************  DAC   ***************************************/
#define DAC0_OUT_PIN    PTE30       // PTE30


#endif  //_PORT_CFG_H_


