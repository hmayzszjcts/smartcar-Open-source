/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2013        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "sdcard.h"			//SDÍ·ÎÄ¼þ




/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber (0..) */
)
{
  return 0;
}
/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber (0..) */
)
{
  return 0;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	       /* Sector address (LBA) */
	BYTE count		/* Number of sectors to read (1..128) */
)
{
	DRESULT res;
	int result;
        int i=0;
         
	switch (pdrv) {
	case 0 :
		// translate the arguments here
              for(i=0;i<count;i++)
		result = SD_ReadDisk(buff+512*i,sector+i);
		// translate the reslut code here
		if(result == 0x00) res = RES_OK;	 
    	else res = RES_ERROR;

		return res;
	}
	return RES_PARERR;
}



#if 1
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	BYTE count			/* Number of sectors to write (1..128) */
)
{
  int result;
        int i=0;
  if(pdrv || (count == 0))
    return RES_PARERR;
     for(i=0;i<count;i++)
          result = SD_WriteDisk(buff+512*i,sector+i);
  return result;
}
#endif

DRESULT disk_ioctl (
    uint8 drv,      /* Physical drive nmuber (0) */
    uint8 ctrl,     /* Control code */
    void  *buff     /* Buffer to send/receive control data */
)
{
    DRESULT              res;
    ESDHC_CMD command;
    //pSDCARD_t  sdcard_ptr = (pSDCARD_t)&SDHC_card;

    if (drv) return RES_PARERR;

    res = RES_ERROR;

    if (Stat & STA_NOINIT) return RES_NOTRDY;

    switch (ctrl)
    {
    case CTRL_SYNC :        /* Make sure that no pending write process. Do not remove this or written sector might not left updated. */
        res = RES_OK;
        break;

    case GET_SECTOR_COUNT : /* Get number of sectors on the disk (uint32) */
        *(unsigned long *)buff = SDHC_card.NUM_BLOCKS;
        res = RES_OK;
        break;

    case GET_SECTOR_SIZE :  /* Get R/W sector size (WORD) */
        *(unsigned short *)buff = SDCARD_BLOCK_SIZE;
        res = RES_OK;
        break;

    case GET_BLOCK_SIZE :           /* Get erase block size in unit of sector (uint32) */
        // Implementar
        command.COMMAND = ESDHC_CMD9;
        command.TYPE = ESDHC_TYPE_NORMAL;
        command.ARGUMENT = SDHC_card.ADDRESS;
        command.READ = FALSE;
        command.BLOCKS = 0;
        if (ESDHC_IOCTL_OK != SDHC_ioctl (ESDHC_IOCTL_SEND_CMD, &command))
        {
            return RES_ERROR;
        }
        if (0 == (command.RESPONSE[3] & 0x00C00000))
        {
            //SD V1
            *(unsigned long *)buff = ((((command.RESPONSE[2] >> 18) & 0x7F) + 1) << (((command.RESPONSE[3] >> 8) & 0x03) - 1));
        }
        else
        {
            //SD V2
            // Implementar
            //*(uint32*)buff = (((command.RESPONSE[2] >> 18) & 0x7F) << (((command.RESPONSE[3] >> 8) & 0x03) - 1));
        }
        res = RES_OK;
        break;

    default:
        res = RES_PARERR;
    }

    return res;
}


DWORD get_fattime (void)
{				 
	return 0;
}













