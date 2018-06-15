/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		KEA128引脚配置
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.23
 * @Target core		S9KEA128AMLK
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-11-6
 ********************************************************************************************************************/

#ifndef _KEA128_port_cfg_h
#define _KEA128_port_cfg_h


//-----------------------------------  IRQ  -----------------------------------
//      模块通道        端口      可选范围                   建议
#define IRQ_PIN         I5        //A5 I0 I1 I2 I3 I4 I5 I6  A5不要用（与复位按键冲突）   

//-----------------------------------  UART  -----------------------------------

//      模块通道        端口      可选范围                建议
//在切换引脚时，务必成对切换。编号相邻得两个引脚为一对
#define UART0_RX_PIN    A2        //A2 B0							A2 A3作为输出是开漏的需要上啦
#define UART0_TX_PIN    A3        //A3 B1
                                       
#define UART1_RX_PIN    F2        //C6 F2
#define UART1_TX_PIN    F3        //C7 F3
                                       
#define UART2_RX_PIN    I0        //D6 I0
#define UART2_TX_PIN    I1        //D7 I1

//-----------------------------------  FTM  -----------------------------------

//      模块通道        端口      可选范围                建议
#define FTM0_CH0_PIN    B2        //B2 A0		 	A0 A1  B2  B3均为ADC如使用FTM必须占用
#define FTM0_CH1_PIN    B3        //B3 A1

//      模块通道        端口      可选范围                建议
#define FTM1_CH0_PIN    H2        //H2 C4				  C4不要用（与SWD_CLK冲突）
#define FTM1_CH1_PIN    C5        //E7 C5

//      模块通道        端口      可选范围                建议
#define FTM2_CH0_PIN    H0        //C0  H0 F0              
#define FTM2_CH1_PIN    H1        //C1  H1 F1  
#define FTM2_CH2_PIN    G4        //C2  G4 D0  
#define FTM2_CH3_PIN    G5        //C3  G5 D1
#define FTM2_CH4_PIN    G6        //B4  G6  
#define FTM2_CH5_PIN    G7        //B5  G7              


//外部计数模块通道      端口      可选范围                建议
#define FTM0_COUNT_PIN  E0        //E7 A5  E0             A5不要用（与复位按键冲突）                
                                           
#define FTM1_COUNT_PIN  E7        //E0 A5  E7             A5不要用（与复位按键冲突）
                                           
#define FTM2_COUNT_PIN  A5        //E0 E7  A5             A5不要用（与复位按键冲突）


//-----------------------------------  I2C  -----------------------------------
//在切换引脚时，务必成对切换。编号相邻得两个引脚为一对
//      模块通道        端口      可选范围                建议
#define I2C0_SCL_PIN    B7        //A3 B7									A3 A2串口使用了
#define I2C0_SDA_PIN    B6        //A2 B6 
                                       
#define I2C1_SCL_PIN    H4        //E1 H4
#define I2C1_SDA_PIN    H3        //E0 H3


//-----------------------------------  SPI  -----------------------------------
//      模块通道        端口      可选范围              建议
//在切换引脚时，务必成对切换。编号相邻得四个引脚为一对
#define SPI0_SCK_PIN    E0       //E0 B2								B2 B3不要用 上面FTM用了
#define SPI0_SOUT_PIN   E1       //E1 B3
#define SPI0_SIN_PIN    E2       //E2 B4                        
#define SPI0_PCS0_PIN   E3       //E3 B5
                                      
                                      
#define SPI1_SCK_PIN    D0       //G4 D0
#define SPI1_SOUT_PIN   D1       //G5 D1
#define SPI1_SIN_PIN    D2       //G6 D2
#define SPI1_PCS0_PIN   D3       //G7 D3


//-----------------------------------  CAN  -----------------------------------
//在切换引脚时，务必成对切换。编号相邻得两个引脚为一对   H2与E7为一对
#define CAN0_TX_PIN     C7       //E7 C7
#define CAN0_RX_PIN     C6       //H2 C6



#endif
