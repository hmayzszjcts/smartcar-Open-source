/*
* @file         MKL_Scope.c
  * @brief      山外虚拟示波器例程
  * @author     xiao_fang
  * @version    v1.0
  * @date       2017-3-23
*/

#include "common.h"
#include "MKL_Scope.h"
#include "MKL_uart.h"

/*
 *  @brief      虚拟示波器发送数据
    wareaddr    要发送的数组的地址
    waresize    要发送得数组的长度
    example :   vcan_scope((uint8_t *)var, sizeof(var)); 
 *  @since      v1.0
*/
void vcan_scope(uint8 *wareaddr, uint32 waresize)
{
    uint8 cmdf[2] = {CMD_WARE, ~CMD_WARE};    //串口调试 使用的前命令
    uint8 cmdr[2] = {~CMD_WARE, CMD_WARE};    //串口调试 使用的后命令

    uart_putbuff(VCAN_PORT, cmdf, sizeof(cmdf));    //先发送前命令
    uart_putbuff(VCAN_PORT, wareaddr, waresize);    //发送数据
    uart_putbuff(VCAN_PORT, cmdr, sizeof(cmdr));    //发送后命令

}