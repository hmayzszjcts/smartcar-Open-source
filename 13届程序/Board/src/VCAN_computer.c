/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2015,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       VCAN_computer.c
 * @brief      山外多功能调试助手上位机驱动代码
 * @author     山外科技
 * @version    v5.2.2
 * @date       2015-03-24
 */


/*
 * 包含头文件
 */
#include "common.h"
#include "MK60_uart.h"
#include "VCAN_computer.h"



/*!
 *  @brief      山外多功能调试助手上位机，摄像头显示函数
 *  @param      imgaddr    图像起始地址
 *  @param      imgsize    图像占用空间的大小
 *  @since      v5.0
*  Sample usage:
             具体用法参考这帖子:
            【山外摄像头】鹰眼上位机例程和微焦效果 - 智能车资料区
             http://vcan123.com/forum.php?mod=viewthread&tid=6242&ctid=27
 */
void vcan_sendimg(void *imgaddr, uint32_t imgsize)
{
#define CMD_IMG     1
    uint8_t cmdf[2] = {CMD_IMG, ~CMD_IMG};    //山外上位机 使用的命令
    uint8_t cmdr[2] = {~CMD_IMG, CMD_IMG};    //山外上位机 使用的命令

    uart_putbuff(VCAN_PORT, cmdf, sizeof(cmdf));    //先发送命令

    uart_putbuff(VCAN_PORT, (uint8_t *)imgaddr, imgsize); //再发送图像

    uart_putbuff(VCAN_PORT, cmdr, sizeof(cmdr));    //先发送命令
}


/*!
 *  @brief      山外多功能调试助手上位机，线性CCD显示函数
 *  @param      ccdaddr    CCD图像起始地址
 *  @param      ccdsize    CCD图像占用空间的大小
 *  @since      v5.0
*  Sample usage:
             具体用法参考这帖子:
            【山外线性CCD】上位机显示例程和超广角采集效果 - 智能车资料区
             http://vcan123.com/forum.php?mod=viewthread&tid=6340&ctid=27
 */
void vcan_sendccd(void *ccdaddr, uint32_t ccdsize)
{
#define CMD_CCD     2
    uint8_t cmdf[2] = {CMD_CCD, ~CMD_CCD};    //开头命令
    uint8_t cmdr[2] = {~CMD_CCD, CMD_CCD};    //结尾命令

    uart_putbuff(VCAN_PORT, cmdf, sizeof(cmdf));    //先发送命令

    uart_putbuff(VCAN_PORT, (uint8_t *)ccdaddr, ccdsize); //再发送图像

    uart_putbuff(VCAN_PORT, cmdr, sizeof(cmdr));    //再发送命令
}


/*!
 *  @brief      山外多功能调试助手上位机，虚拟示波器显示函数
 *  @param      wareaddr    波形数组起始地址
 *  @param      waresize    波形数组占用空间的大小
 *  @since      v5.0
*  Sample usage:
             具体用法参考这帖子:
            【山外资料】陀螺仪和加速度 上位机显示例程 - 智能车资料区
             http://vcan123.com/forum.php?mod=viewthread&tid=6253&ctid=27
 */
void vcan_sendware(void *wareaddr, uint32_t waresize)
{
#define CMD_WARE     3
    uint8_t cmdf[2] = {CMD_WARE, ~CMD_WARE};    //串口调试 使用的前命令
    uint8_t cmdr[2] = {~CMD_WARE, CMD_WARE};    //串口调试 使用的后命令

    uart_putbuff(VCAN_PORT, cmdf, sizeof(cmdf));    //先发送前命令
    uart_putbuff(VCAN_PORT, (uint8_t *)wareaddr, waresize);    //发送数据
    uart_putbuff(VCAN_PORT, cmdr, sizeof(cmdr));    //发送后命令

}