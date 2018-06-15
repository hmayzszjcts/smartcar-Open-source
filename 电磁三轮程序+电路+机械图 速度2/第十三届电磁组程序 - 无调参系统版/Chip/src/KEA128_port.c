/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		KEA128_port
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.23
 * @Target core		S9KEA128AMLK
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-11-6
 ********************************************************************************************************************/


#include "KEA128_port.h"



#define PTX(PTX_n)  (PTX_n>>5)  //获取模块号 PTX_n/32
#define PTn(PTX_n)  (PTX_n&0x1f)//获取引脚号 PTX_n%32

//-------------------------------------------------------------------------------------------------------------------
//  @brief      IO上拉
//  @param      ptx_n           选择端口
//  @return     void
//  @since      v2.0
//  Sample usage:               prot_pull(E0);			//E0 上拉电阻开启
//-------------------------------------------------------------------------------------------------------------------
void port_pull(PTX_n ptx_n)
{
    uint8 ptx,ptn;
    ptx = PTX(ptx_n);//记录模块号    //A,B......
    ptn = PTn(ptx_n);//记录引脚号    //0,1,2,3......
    
    switch(ptx)
    {
        case 0:
        {
            PORT->PUE0 |= (uint32)(1<<ptn);
        }break;
        
        case 1:
        {
            PORT->PUE1 |= (uint32)(1<<ptn);
        }break;
        
        case 2:
        {
            PORT->PUE2 |= (uint32)(1<<ptn);
        }break;
    }
}

