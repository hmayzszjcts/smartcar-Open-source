#include "common.h"
#include "w25x16.h"
#include "spi.h"

/* Private typedef -----------------------------------------------------------*/
//#define SPI_FLASH_PageSize                    4096
#define SPI_FLASH_PageSize                      256
#define SPI_FLASH_PerWritePageSize              256

/* Private define ------------------------------------------------------------*/
#define W25X_WriteEnable		        0x06
#define W25X_WriteDisable		        0x04
#define W25X_ReadStatusReg		        0x05
#define W25X_WriteStatusReg		        0x01
#define W25X_ReadData			        0x03
#define W25X_FastReadData		        0x0B
#define W25X_FastReadDual		        0x3B
#define W25X_PageProgram		        0x02
#define W25X_BlockErase			        0xD8
#define W25X_SectorErase		        0x20
#define W25X_ChipErase			        0xC7
#define W25X_PowerDown			        0xB9
#define W25X_ReleasePowerDown	                0xAB
#define W25X_DeviceID			        0xAB
#define W25X_ManufactDeviceID   	        0x90
#define W25X_JedecDeviceID		        0x9F

#define WIP_Flag                  0x01  /* Write In Progress (WIP) flag */

#define Dummy_Byte                0xFF

#define w25x16_SPIn SPI0

void w25x16_init()
{
    spi_init(w25x16_SPIn, MASTER);
}

u32 w25x16_ReadDeviceID()       // FlashID is 0xEF3015,  Manufacturer Device ID is 0x14
{
    u8  RDID[] = {W25X_DeviceID, Dummy_Byte, Dummy_Byte, Dummy_Byte, Dummy_Byte}; // Dummy_Byte 这些是无意义的数据，是为了传输发送数据的时候，能接收到数据
    u32 Temp = 0;

    spi_send(w25x16_SPIn, RDID, 4);



    spi_re(w25x16_SPIn, RDID);
    Temp = RDID[0];

    return Temp;
}

u32 w25x16_ReadFlashID(void)
{
    u8  RDID[] = {W25X_JedecDeviceID, Dummy_Byte, Dummy_Byte, Dummy_Byte}; // Dummy_Byte 这些是无意义的数据，是为了传输发送数据的时候，能接收到数据
    u32 Temp = 0;

    spi_send(w25x16_SPIn, RDID, 4);
    Temp = spi_re(w25x16_SPIn, RDID);
    if(Temp == 3)
        Temp = (RDID[0] << 16) | (RDID[1] << 8) | RDID[2];


    return Temp;
}