/*
* @file         MKL_Scope.h
  * @brief      山外虚拟示波器例程
  * @author     xiao_fang
  * @version    v1.0
  * @date       2017-3-23
*/
#ifndef __MKL_SCOPE_H__
#define __MKL_SCOPE_H__

#define CMD_WARE     3           //定义虚拟示波器的特征码

void vcan_scope(uint8 *wareaddr, uint32 waresize);

#endif