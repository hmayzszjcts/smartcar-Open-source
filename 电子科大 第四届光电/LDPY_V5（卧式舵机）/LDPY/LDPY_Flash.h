
#ifndef __LDPY_FLASH_H__
#define __LDPY_FLASH_H__

#include "MK60DZ10.h"
#include "myStruct.h"

/* 错误代码定义 */
#define FLASH_OK                    0x00
#define FLASH_FACCERR               0x01
#define FLASH_FPVIOL                0x02
#define FLASH_MGSTAT0               0x04
#define FLASH_RDCOLERR              0x08
#define FLASH_NOT_ERASED            0x10
#define FLASH_CONTENTERR            0x11

/* flash命令 */
#define FLASH_CMD_PGM4              0x06      //编程长字命令
#define FLASH_CMD_ERSSCR            0x09      //擦除Flash扇区命令

void LDPY_Flash_Init(void);
uint8 LDPY_Flash_SectorErase(uint32 FlashPtr);
uint8 LDPY_Flash_ByteProgram(uint32 FlashStartAdd, uint32 *DataSrcPtr, uint32 NumberOfBytes);

#endif

/*------------------------------end of LDPY_Flash.h------------------------------*/
