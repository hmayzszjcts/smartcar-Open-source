/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：AT24C02.h
 * 描述         ：AT24C02芯片操作函数定义
 *
 * 实验平台     ：野火kinetis开发板
 * 库版本       ：
 * 嵌入系统     ：
 *
 * 作者         ：野火嵌入式开发工作室
 * 淘宝店       ：http://firestm32.taobao.com
 * 技术支持论坛 ：http://www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
**********************************************************************************/



#ifndef _AT24C02_H_
#define _AT24C02_H_

#define AT24C02_I2C_ADDRESS                         0x50
#define AT24C02_PageSize                            8			/* AT24C02每页有8个字节 */


#define AT24C02_init()                              I2C_init(I2C0)

#define AT24C02_WriteByte(Addr,Data)                I2C_WriteAddr(I2C0,AT24C02_I2C_ADDRESS,Addr,Data)   //读取地址里的内容
#define AT24C02_ReadByte(Addr)                      I2C_ReadAddr(I2C0,AT24C02_I2C_ADDRESS,Addr)          //从地址读取内容

#endif  //_AT24C02_H_