/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：flash.h
 * 描述         ：flash驱动函数
 *
 * 实验平台     ：野火kinetis开发板
 * 库版本       ：
 * 嵌入系统     ：
 *
 * 作者         ：
 * 淘宝店       ：http://firestm32.taobao.com
 * 技术支持论坛 ：http://www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
 * 备注			：从苏州大学例程修改而成
**********************************************************************************/


#ifndef _FLASH_H_
#define _FLASH_H_

//k60N512包含512K的程序Flash
//512K的程序Flash分为256个扇区，每个扇区2K大小
//K60X256包含256K的程序Flash
//512K的程序Flash分为128个扇区，每个扇区2K大小
//    sector（2K）为擦除最小单位
//    长字（32b）为写的最小单位

#include "common.h"


__RAMFUN	void 	flash_init();																				//擦除指定flash扇区

__RAMFUN 	uint8 	flash_erase_sector	(uint16 sectorNo);														//擦除指定flash扇区
__RAMFUN 	uint8 	flash_write		(uint16 sectorNo,uint16 offset,uint32 data);							//写入flash操作
__RAMFUN 	uint8 	flash_write_buf		(uint16 sectorNo,uint16 offset,uint16 cnt,uint8 buf[]);			//从缓存区写入flash操作

#define		flash_read(sectorNo,offset,type)	(*(type *)((uint32)(((sectorNo)<<11) + (offset))))			//读取扇区
#endif //_FLASH_H_ 
