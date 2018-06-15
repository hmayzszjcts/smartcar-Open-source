
#include "common.h"
#include  "sdhc.h"
#include  "diskio.h"


static volatile DSTATUS   Stat             = STA_NOINIT;	/* Disk status */
static volatile uint32	  Timer            = 0;           	/* Read/Write timer */


DSTATUS disk_initialize (unsigned char drv)
{
    uint32              		param, c_size, c_size_mult, read_bl_len;
    ESDHC_COMMAND_STRUCT 		command;
    ESDHC_DEVICE_STRUCT_PTR     esdhc_device_ptr = &SDHC_Device;
    SDCARD_STRUCT_PTR    		sdcard_ptr = (SDCARD_STRUCT_PTR)&SDHC_Card;
    sdcard_ptr->INIT = 			&SDHC_Init;

    if (drv) return STA_NOINIT;			/* Supports only single drive */
    if (Stat & STA_NODISK) return Stat;	/* No card in the socket */
    if ((Stat & 0x03) == 0)	return 0;

    /* Check parameters */
    if ((NULL == sdcard_ptr) || (NULL == sdcard_ptr->INIT))
    {
        return FALSE;
    }

    esdhc_device_ptr->INIT = &K60_SDHC0_init;
    // Indicates one SDHC open
    esdhc_device_ptr->COUNT = 1;

    sdcard_ptr->SD_TIMEOUT = 0;
    sdcard_ptr->NUM_BLOCKS = 0;
    sdcard_ptr->ADDRESS = 0;
    sdcard_ptr->SDHC = FALSE;
    sdcard_ptr->VERSION2 = FALSE;

    /* Enable clock gate to SDHC module */
    SIM_SCGC3 |= SIM_SCGC3_SDHC_MASK;

    /* Initialize and detect card */
    if (ESDHC_OK != SDHC_ioctl (IO_IOCTL_ESDHC_INIT, NULL))
    {
        return FALSE;
    }

    /* SDHC check */
    param = 0;
    if (ESDHC_OK != SDHC_ioctl (IO_IOCTL_ESDHC_GET_CARD, &param))
    {
        return FALSE;
    }
    if ((ESDHC_CARD_SD == param) || (ESDHC_CARD_SDHC == param) || (ESDHC_CARD_SDCOMBO == param) || (ESDHC_CARD_SDHCCOMBO == param))
    {
        if ((ESDHC_CARD_SDHC == param) || (ESDHC_CARD_SDHCCOMBO == param))
        {
            sdcard_ptr->SDHC = TRUE;
        }
    }
    else
    {
        return FALSE;
    }

    /* Card identify */
    command.COMMAND = ESDHC_CMD2;
    command.TYPE = ESDHC_TYPE_NORMAL;
    command.ARGUMENT = 0;
    command.READ = FALSE;
    command.BLOCKS = 0;
    if (ESDHC_OK != SDHC_ioctl (IO_IOCTL_ESDHC_SEND_COMMAND, &command))
    {
        return FALSE;
    }

    /* Get card address */
    command.COMMAND = ESDHC_CMD3;
    command.TYPE = ESDHC_TYPE_NORMAL;
    command.ARGUMENT = 0;
    command.READ = FALSE;
    command.BLOCKS = 0;
    if (ESDHC_OK != SDHC_ioctl (IO_IOCTL_ESDHC_SEND_COMMAND, &command))
    {
        return FALSE;
    }
    sdcard_ptr->ADDRESS = command.RESPONSE[0] & 0xFFFF0000;

    /* Get card parameters */
    command.COMMAND = ESDHC_CMD9;
    command.TYPE = ESDHC_TYPE_NORMAL;
    command.ARGUMENT = sdcard_ptr->ADDRESS;
    command.READ = FALSE;
    command.BLOCKS = 0;
    if (ESDHC_OK != SDHC_ioctl (IO_IOCTL_ESDHC_SEND_COMMAND, &command))
    {
        return FALSE;
    }
    if (0 == (command.RESPONSE[3] & 0x00C00000))
    {
        read_bl_len = (command.RESPONSE[2] >> 8) & 0x0F;
        c_size = command.RESPONSE[2] & 0x03;
        c_size = (c_size << 10) | (command.RESPONSE[1] >> 22);
        c_size_mult = (command.RESPONSE[1] >> 7) & 0x07;
        sdcard_ptr->NUM_BLOCKS = (c_size + 1) * (1 << (c_size_mult + 2)) * (1 << (read_bl_len - 9));
    }
    else
    {
        sdcard_ptr->VERSION2 = TRUE;
        c_size = (command.RESPONSE[1] >> 8) & 0x003FFFFF;
        sdcard_ptr->NUM_BLOCKS = (c_size + 1) << 10;
    }

    /* Select card */
    command.COMMAND = ESDHC_CMD7;
    command.TYPE = ESDHC_TYPE_NORMAL;
    command.ARGUMENT = sdcard_ptr->ADDRESS;
    command.READ = FALSE;
    command.BLOCKS = 0;
    if (ESDHC_OK != SDHC_ioctl (IO_IOCTL_ESDHC_SEND_COMMAND, &command))
    {
        return FALSE;
    }

    /* Set block size */
    command.COMMAND = ESDHC_CMD16;
    command.TYPE = ESDHC_TYPE_NORMAL;
    command.ARGUMENT = IO_SDCARD_BLOCK_SIZE;
    command.READ = FALSE;
    command.BLOCKS = 0;
    if (ESDHC_OK != SDHC_ioctl (IO_IOCTL_ESDHC_SEND_COMMAND, &command))
    {
        return FALSE;
    }

    if (ESDHC_BUS_WIDTH_4BIT == sdcard_ptr->INIT->SIGNALS)
    {
        /* Application specific command */
        command.COMMAND = ESDHC_CMD55;
        command.TYPE = ESDHC_TYPE_NORMAL;
        command.ARGUMENT = sdcard_ptr->ADDRESS;
        command.READ = FALSE;
        command.BLOCKS = 0;
        if (ESDHC_OK != SDHC_ioctl (IO_IOCTL_ESDHC_SEND_COMMAND, &command))
        {
            return FALSE;
        }

        /* Set bus width == 4 */
        command.COMMAND = ESDHC_ACMD6;
        command.TYPE = ESDHC_TYPE_NORMAL;
        command.ARGUMENT = 2;
        command.READ = FALSE;
        command.BLOCKS = 0;
        if (ESDHC_OK != SDHC_ioctl (IO_IOCTL_ESDHC_SEND_COMMAND, &command))
        {
            return FALSE;
        }

        param = ESDHC_BUS_WIDTH_4BIT;
        if (ESDHC_OK != SDHC_ioctl (IO_IOCTL_ESDHC_SET_BUS_WIDTH, &param))
        {
            return FALSE;
        }
    }

    Stat &= ~STA_NOINIT;		/* Clear STA_NOINIT */

    return (Stat & 0x03);
}



/*-----------------------------------------------------------------------*/
/* Receive a data packet from MMC                                        */
/*-----------------------------------------------------------------------*/

static int rcvr_datablock (
    uint8   *buff,			/* Data buffer to store received data */
    uint32  btr				/* Byte count (must be multiple of 4) */
)
{
    uint32	bytes, i, j;
    uint32	*ptr = (uint32 *)buff;
    ESDHC_INFO_STRUCT_PTR   esdhc_info_ptr;

    /* Check parameters */
    esdhc_info_ptr = (ESDHC_INFO_STRUCT_PTR)&SDHC_Info;

    /* Check parameters */
    if (NULL == esdhc_info_ptr)
    {
        return 0;
    }

    /* Workaround for random bit polling failures - not suitable for IO cards */
    if ((esdhc_info_ptr->CARD == ESDHC_CARD_SD) || (esdhc_info_ptr->CARD == ESDHC_CARD_SDHC) || (esdhc_info_ptr->CARD == ESDHC_CARD_MMC) || (esdhc_info_ptr->CARD == ESDHC_CARD_CEATA))
    {
        while (SDHC_PRSSTAT & SDHC_PRSSTAT_DLA_MASK) {};
    }

    /* Read data in 4 byte counts */
    bytes = btr;
    while (bytes)
    {
        i = bytes > 512 ? 512 : bytes;
        for (j = (i + 3) >> 2; j != 0; j--)
        {
            if (SDHC_IRQSTAT & (SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK))
            {
                SDHC_IRQSTAT |= SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK | SDHC_IRQSTAT_BRR_MASK;
                return 0;
            }

            while (0 == (SDHC_PRSSTAT & SDHC_PRSSTAT_BREN_MASK)) {};

#if BRTOS_ENDIAN == BRTOS_LITTLE_ENDIAN
            *ptr++ = SDHC_DATPORT;
#else
            *ptr++ = _psp_swap4byte (SDHC_DATPORT);
#endif
        }
        bytes -= i;
    }

    return 1;						/* Return with success */
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (  //读磁盘扇区
    uint8  drv,			/* Physical drive nmuber (0) */
    uint8  *buff,		/* Pointer to the data buffer to store read data */
    uint32 sector,		/* Start sector number (LBA) */
    uint8  count			/* Sector count (1..255) */
)
{
    ESDHC_COMMAND_STRUCT command;
    SDCARD_STRUCT_PTR    sdcard_ptr = (SDCARD_STRUCT_PTR)&SDHC_Card;

    if (drv || !count) return RES_PARERR;
    if (Stat & STA_NOINIT) return RES_NOTRDY;

    /* Check parameters */
    if ((NULL == buff))
    {
        return RES_PARERR;//参数无效
    }

    if (!sdcard_ptr->SDHC)
    {
        sector *= 512;	/* Convert to byte address if needed */
    }

    if (count == 1)	/* Single block read */
    {
        command.COMMAND = ESDHC_CMD17;
        command.TYPE = ESDHC_TYPE_NORMAL;
        command.ARGUMENT = sector;
        command.READ = TRUE;
        command.BLOCKS = count;

        if (ESDHC_OK == SDHC_ioctl (IO_IOCTL_ESDHC_SEND_COMMAND, &command))
        {
            if (rcvr_datablock(buff, 512))
            {
                count = 0;
            }
        }
    }
    else
    {
        /* Multiple block read */
        // N鉶 sei se ?17 ou 18 no ESDHC
        command.COMMAND = ESDHC_CMD18;
        //command.COMMAND = ESDHC_CMD17;
        command.TYPE = ESDHC_TYPE_NORMAL;
        command.ARGUMENT = sector;
        command.READ = TRUE;
        command.BLOCKS = count;

        if (ESDHC_OK == SDHC_ioctl (IO_IOCTL_ESDHC_SEND_COMMAND, &command))
        {
            if (rcvr_datablock(buff, 512 * count))
            {
                count = 0;
            }
        }
    }

    return count ? RES_ERROR : RES_OK;
}

/*-----------------------------------------------------------------------*/
/* Send a data packet to MMC                                             */
/*-----------------------------------------------------------------------*/

static int xmit_datablock (
    const uint8 *buff,			/* 512 byte data block to be transmitted */
    uint32 btr				/* Byte count (must be multiple of 4) */
)
{
    uint32  bytes, i;
    uint32	*ptr = (uint32 *)buff;

    /* Write data in 4 byte counts */
    bytes = btr;
    while (bytes)
    {
        i = bytes > 512 ? 512 : bytes;
        bytes -= i;
        for (i = (i + 3) >> 2; i != 0; i--)
        {
            if (SDHC_IRQSTAT & (SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK))
            {
                SDHC_IRQSTAT |= SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK | SDHC_IRQSTAT_BWR_MASK;
                return IO_ERROR;
            }
            while (0 == (SDHC_PRSSTAT & SDHC_PRSSTAT_BWEN_MASK)) {};

#if PSP_ENDIAN == BRTOS_LITTLE_ENDIAN
            SDHC_DATPORT = *ptr++;
#else
            SDHC_DATPORT = _psp_swap4byte (*ptr++);
#endif

        }
    }


    return 1;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
    uint8  drv,			/* Physical drive nmuber (0) */
    const uint8  *buff,		/* Pointer to the data buffer to store read data */
    uint32 sector,		/* Start sector number (LBA) */
    uint8  count		/* Sector count (1..255) */
)
{
    ESDHC_COMMAND_STRUCT command;
    SDCARD_STRUCT_PTR    sdcard_ptr = (SDCARD_STRUCT_PTR)&SDHC_Card;

    if (drv || !count) return RES_PARERR;
    if (Stat & STA_NOINIT) return RES_NOTRDY;
    if (Stat & STA_PROTECT) return RES_WRPRT;

    /* Check parameters */
    if ((NULL == buff))
    {
        return RES_PARERR;		// 参数无效
    }

    if (!sdcard_ptr->SDHC)
    {
        sector *= 512;	/* Convert to byte address if needed */
    }

    if (count == 1)	/* Single block write */
    {
        command.COMMAND = ESDHC_CMD24;
        command.TYPE = ESDHC_TYPE_NORMAL;
        command.ARGUMENT = sector;
        command.READ = FALSE;
        command.BLOCKS = count;

        if (ESDHC_OK == SDHC_ioctl (IO_IOCTL_ESDHC_SEND_COMMAND, &command))
        {
            if (xmit_datablock(buff, 512))
            {
                count = 0;
            }
        }
    }
    else
    {
        //if (CardType & CT_SDC) send_cmd(ACMD23, count);
        //if (send_cmd(CMD25, sector) == 0) {	/* WRITE_MULTIPLE_BLOCK */
        command.COMMAND = ESDHC_CMD25;
        command.TYPE = ESDHC_TYPE_NORMAL;
        command.ARGUMENT = sector;
        command.READ = FALSE;
        command.BLOCKS = count;

        if (ESDHC_OK == SDHC_ioctl (IO_IOCTL_ESDHC_SEND_COMMAND, &command))
        {
            if (xmit_datablock(buff, 512 * count))
            {
                count = 0;
            }
        }
    }

    /* Wait for card ready / transaction state */
    do
    {
        command.COMMAND = ESDHC_CMD13;
        command.TYPE = ESDHC_TYPE_NORMAL;
        command.ARGUMENT = sdcard_ptr->ADDRESS;
        command.READ = FALSE;
        command.BLOCKS = 0;
        if (ESDHC_OK != SDHC_ioctl (IO_IOCTL_ESDHC_SEND_COMMAND, &command))
        {
            return RES_ERROR;
        }

        /* Card status error check */
        if (command.RESPONSE[0] & 0xFFD98008)
        {
            return RES_ERROR;
        }
    }
    while (0x000000900 != (command.RESPONSE[0] & 0x00001F00));

    return count ? RES_ERROR : RES_OK;
}


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
    uint8 drv,		/* Physical drive nmuber (0) */
    uint8 ctrl,		/* Control code */
    void  *buff		/* Buffer to send/receive control data */
)
{
    DRESULT 			 res;
    ESDHC_COMMAND_STRUCT command;
    SDCARD_STRUCT_PTR	 sdcard_ptr = (SDCARD_STRUCT_PTR)&SDHC_Card;

    if (drv) return RES_PARERR;

    res = RES_ERROR;

    if (Stat & STA_NOINIT) return RES_NOTRDY;

    switch (ctrl)
    {
    case CTRL_SYNC :		/* Make sure that no pending write process. Do not remove this or written sector might not left updated. */
        res = RES_OK;
        break;

    case GET_SECTOR_COUNT :	/* Get number of sectors on the disk (DWORD) */
        *(unsigned long *)buff = sdcard_ptr->NUM_BLOCKS;
        res = RES_OK;
        break;

    case GET_SECTOR_SIZE :	/* Get R/W sector size (WORD) */
        *(unsigned short *)buff = 512;
        res = RES_OK;
        break;

    case GET_BLOCK_SIZE :			/* Get erase block size in unit of sector (DWORD) */
        // Implementar
        command.COMMAND = ESDHC_CMD9;
        command.TYPE = ESDHC_TYPE_NORMAL;
        command.ARGUMENT = sdcard_ptr->ADDRESS;
        command.READ = FALSE;
        command.BLOCKS = 0;
        if (ESDHC_OK != SDHC_ioctl (IO_IOCTL_ESDHC_SEND_COMMAND, &command))
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
            //*(DWORD*)buff = (((command.RESPONSE[2] >> 18) & 0x7F) << (((command.RESPONSE[3] >> 8) & 0x03) - 1));
        }
        res = RES_OK;
        break;

    default:
        res = RES_PARERR;
    }

    return res;
}




/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
    uint8 drv		/* Physical drive nmuber (0) */
)
{
    if (drv) return STA_NOINIT;		/* Supports only single drive */
    return Stat;
}



DWORD  get_fattime (void)
{
    return   0;
}


