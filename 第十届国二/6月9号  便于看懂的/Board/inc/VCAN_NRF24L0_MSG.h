/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       VCAN_NRF24L0_MSG.h
 * @brief      无线调试 消息机制 函数
 * @author     山外科技
 * @version    v5.0
 * @date       2014-01-04
 */


#ifndef _VCAN_NRF24L0_MSG_H_
#define _VCAN_NRF24L0_MSG_H_

#include "vcan_ui_var.h"
#include "VCAN_NRF24L0.h"



#define COM_LEN     2   //com命令占用的字节



//控制命令COM执行函数返回结果
//COM命令，需要执行对应的函数
//MSG消息，发送方主动通知，不需要执行任何函数
typedef enum
{
    NRF_RESULT_FALSE = 0,                   //假、失败、事件没发生
    NRF_RESULT_TRUE = 1,                    //真、成功、事件发生了
    NRF_RESULT_QUIT,                    //退出

    NRF_RESULT_NULL,                    //不进行任何处理

    //接收数据
    NRF_RESULT_RX_NO,                   //没接收到数据
    NRF_RESULT_RX_VALID,                //接收有效数据
    NRF_RESULT_RX_NOVALID,              //接收到数据，但无效


    /*** EVENT 事件 ***/
    //  NRF_RESULT_EVENT_FALSE,             //没事件发生，或传输事件数据有误
    //  NRF_RESULT_EVENT_FINIST,            //事件传输完毕
    //  NRF_RESULT_EVENT_CONTINUE,          //事件继续传输（多个事件发送了，需要继续传输）

} nrf_result_e;

//控制命令消息类型
typedef enum
{
    ////////////////// COM命令 /////////////////////
    //需要校验接收的命令是否正确
    //发送的数据： XXX_COM ，~XXX_COM    后面继续发送需要发送的信息，由对应的执行函数进行处理

    //图像传输
    COM_IMG,

    //线性CCD
    COM_CCD,

    //变量传输控制
    COM_VAR,

    COM_RETRAN,//复位传输，丢弃之前接收到的数据

    /*  需要添加功能，请放入此位置  */


    COM_MAX     ,       //最大控制命令数目                                                  ********************************

} com_e;

extern void             nrf_msg_init();                                //初始化消息处理(并没有初始化 nrf 模块)
extern nrf_result_e     nrf_msg_tx(com_e   com, uint8 *sendbuf);       //发送数据，发送长度由com决定
extern nrf_result_e     nrf_msg_rx(com_e  *com, uint8 *rebuf);         //接收数据,并进行处理








#endif  //_VCAN_NRF24L0_MSG_H_