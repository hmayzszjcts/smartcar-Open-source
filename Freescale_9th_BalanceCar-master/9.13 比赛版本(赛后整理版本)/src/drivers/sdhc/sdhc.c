#include "sdhc.h"

SDCARD_STRUCT 		SDHC_Card;
SDCARD_INIT_STRUCT  SDHC_Init;
ESDHC_INFO_STRUCT	SDHC_Info;
ESDHC_DEVICE_STRUCT SDHC_Device;

const ESDHC_INIT_STRUCT K60_SDHC0_init =
{
    0,                          /* ESDHC device number */
    25000000,                   /* ESDHC baudrate      */
    //200000000            /* ESDHC clock source  */
};


//SD卡命令实际使用到的命令有 32条，其中基本命令 25条，用户应用命令 7条
const unsigned long ESDHC_COMMAND_XFERTYP[] =
{
    /* CMD0 */
    SDHC_XFERTYP_CMDINX(ESDHC_CMD0) | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_NO),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD1) | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_NO),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD2) | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_136),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD3) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD4) | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_NO),
    /* CMD5 */
    SDHC_XFERTYP_CMDINX(ESDHC_CMD5) | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD6) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD7) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48BUSY),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD8) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD9) | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_136),
    /* CMD10 */
    SDHC_XFERTYP_CMDINX(ESDHC_CMD10) | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_136),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD11) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD12) | SDHC_XFERTYP_CMDTYP(ESDHC_XFERTYP_CMDTYP_ABORT) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48BUSY),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD13) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    0,
    /* CMD15 */
    SDHC_XFERTYP_CMDINX(ESDHC_CMD15) | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_NO),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD16) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD17) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD18) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    0,
    /* CMD20 */
    SDHC_XFERTYP_CMDINX(ESDHC_CMD20) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    0,
    SDHC_XFERTYP_CMDINX(ESDHC_ACMD22) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_ACMD23) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD24) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    /* CMD25 */
    SDHC_XFERTYP_CMDINX(ESDHC_CMD25) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD26) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD27) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD28) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48BUSY),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD29) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48BUSY),
    /* CMD30 */
    SDHC_XFERTYP_CMDINX(ESDHC_CMD30) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    0,
    SDHC_XFERTYP_CMDINX(ESDHC_CMD32) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD33) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD34) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    /* CMD35 */
    SDHC_XFERTYP_CMDINX(ESDHC_CMD35) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD36) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD37) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD38) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48BUSY),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD39) | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    /* CMD40 */
    SDHC_XFERTYP_CMDINX(ESDHC_CMD40) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_ACMD41) | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD42) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48BUSY),
    0,
    0,
    /* CMD45 */
    0,
    0,
    0,
    0,
    0,
    /* CMD50 */
    0,
    SDHC_XFERTYP_CMDINX(ESDHC_ACMD51) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD52) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD53) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    0,
    /* CMD55 */
    SDHC_XFERTYP_CMDINX(ESDHC_CMD55) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD56) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48BUSY),
    0,
    0,
    0,
    /* CMD60 */
    SDHC_XFERTYP_CMDINX(ESDHC_CMD60) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48BUSY),
    SDHC_XFERTYP_CMDINX(ESDHC_CMD61) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48BUSY),
    0,
    0
};


void SDHC_set_baudrate
(
    /* [IN] Module input clock in Hz */
    uint32         clock,

    /* [IN] Desired baudrate in Hz */
    uint32         baudrate
)
{
    uint32 pres, div, min, minpres = 0x80, mindiv = 0x0F;
    int32  val;

    /* Find closest setting */
    min = (uint32) - 1;
    for (pres = 2; pres <= 256; pres <<= 1)
    {
        for (div = 1; div <= 16; div++)
        {
            val = pres * div * baudrate - clock;
            if (val >= 0)
            {
                if (min > val)
                {
                    min = val;
                    minpres = pres;
                    mindiv = div;
                }
            }
        }
    }

    /* Disable ESDHC clocks */
    SDHC_SYSCTL &= (~ SDHC_SYSCTL_SDCLKEN_MASK);

    /* Change dividers */
    div = SDHC_SYSCTL & (~ (SDHC_SYSCTL_DTOCV_MASK | SDHC_SYSCTL_SDCLKFS_MASK | SDHC_SYSCTL_DVS_MASK));
    SDHC_SYSCTL = div | (SDHC_SYSCTL_DTOCV(0x0E) | SDHC_SYSCTL_SDCLKFS(minpres >> 1) | SDHC_SYSCTL_DVS(mindiv - 1));

    /* Wait for stable clock */
    while (0 == (SDHC_PRSSTAT & SDHC_PRSSTAT_SDSTB_MASK));


    /* Enable ESDHC clocks */
    SDHC_SYSCTL |= SDHC_SYSCTL_SDCLKEN_MASK;
    SDHC_IRQSTAT |= SDHC_IRQSTAT_DTOE_MASK;
}


uint32 SDHC_init
(
    /* [IN/OUT] Device runtime information */
    ESDHC_INFO_STRUCT_PTR  esdhc_info_ptr,

    /* [IN] Device initialization data */
    ESDHC_INIT_STRUCT_CPTR esdhc_init_ptr
)
{

    esdhc_info_ptr->CARD = ESDHC_CARD_NONE;

    /* Reset ESDHC */
    SDHC_SYSCTL = SDHC_SYSCTL_RSTA_MASK | SDHC_SYSCTL_SDCLKFS(0x80);
    while (SDHC_SYSCTL & SDHC_SYSCTL_RSTA_MASK) {};

    /* Initial values */
    SDHC_VENDOR = 0;
    SDHC_BLKATTR = SDHC_BLKATTR_BLKCNT(1) | SDHC_BLKATTR_BLKSIZE(512);
    SDHC_PROCTL = SDHC_PROCTL_EMODE(ESDHC_PROCTL_EMODE_INVARIANT) | SDHC_PROCTL_D3CD_MASK;
    SDHC_WML = SDHC_WML_RDWML(1) | SDHC_WML_WRWML(1);

    /* Set the ESDHC initial baud rate divider and start */
    //SDHC_set_baudrate (esdhc_init_ptr->CLOCK_SPEED,380000);
    SDHC_set_baudrate (core_clk_khz * 1000, 380000);

    /* Poll inhibit bits */
    while (SDHC_PRSSTAT & (SDHC_PRSSTAT_CIHB_MASK | SDHC_PRSSTAT_CDIHB_MASK)) {};

    /* 初始化管脚复用 */
    PORTE_PCR(0) = 0xFFFF & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    /* ESDHC.D1  */
    PORTE_PCR(1) = 0xFFFF & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    /* ESDHC.D0  */
    PORTE_PCR(2) = 0xFFFF & (PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK);                                          /* ESDHC.CLK */
    PORTE_PCR(3) = 0xFFFF & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    /* ESDHC.CMD */
    PORTE_PCR(4) = 0xFFFF & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    /* ESDHC.D3  */
    PORTE_PCR(5) = 0xFFFF & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    /* ESDHC.D2  */

    /* Enable clock gate to SDHC module */
    SIM_SCGC3 |= SIM_SCGC3_SDHC_MASK;

    /* Enable requests */
    SDHC_IRQSTAT = 0xFFFF;
    SDHC_IRQSTATEN = 	  SDHC_IRQSTATEN_DEBESEN_MASK | SDHC_IRQSTATEN_DCESEN_MASK | SDHC_IRQSTATEN_DTOESEN_MASK
                          | SDHC_IRQSTATEN_CIESEN_MASK | SDHC_IRQSTATEN_CEBESEN_MASK | SDHC_IRQSTATEN_CCESEN_MASK | SDHC_IRQSTATEN_CTOESEN_MASK
                          | SDHC_IRQSTATEN_BRRSEN_MASK | SDHC_IRQSTATEN_BWRSEN_MASK | SDHC_IRQSTATEN_CRMSEN_MASK
                          | SDHC_IRQSTATEN_TCSEN_MASK | SDHC_IRQSTATEN_CCSEN_MASK;

    /* 80 initial clocks */
    SDHC_SYSCTL |= SDHC_SYSCTL_INITA_MASK;
    while (SDHC_SYSCTL & SDHC_SYSCTL_INITA_MASK) {};

    /* Check card */
    if (SDHC_PRSSTAT & SDHC_PRSSTAT_CINS_MASK)
    {
        esdhc_info_ptr->CARD = ESDHC_CARD_UNKNOWN;
    }
    SDHC_IRQSTAT |= SDHC_IRQSTAT_CRM_MASK;

    return ESDHC_OK;
}

/*FUNCTION****************************************************************
*
* Function Name    : _esdhc_is_running
* Returned Value   : TRUE if running, FALSE otherwise
* Comments         :
*    Checks whether eSDHC module is currently in use.
*
*END*********************************************************************/
uint8 SDHC_is_running(void)
{
    return (0 != (SDHC_PRSSTAT & (SDHC_PRSSTAT_RTA_MASK | SDHC_PRSSTAT_WTA_MASK | SDHC_PRSSTAT_DLA_MASK | SDHC_PRSSTAT_CDIHB_MASK | SDHC_PRSSTAT_CIHB_MASK)));
}

/*FUNCTION****************************************************************
*
* Function Name    : SDHC_status_wait
* Returned Value   : bits set for given mask
* Comments         :
*    Waits for ESDHC interrupt status register bits according to given mask.
*
*END*********************************************************************/
uint32 SDHC_status_wait(uint32	mask)        /* [IN] Mask of IRQSTAT bits to wait for */
{
    uint32	result;
    do
    {
        result = SDHC_IRQSTAT & mask;
    }
    while (0 == result);
    return result;
}

/*FUNCTION****************************************************************
*
* Function Name    : SDHC_send_command
* Returned Value   : 0 on success, 1 on error, -1 on timeout
* Comments         :
*    One ESDHC command transaction.
*
*END*********************************************************************/
uint32 SDHC_send_command (ESDHC_COMMAND_STRUCT_PTR command) /* [IN/OUT] Command specification */
{
    uint32	xfertyp;

    /* Check command */
    xfertyp = ESDHC_COMMAND_XFERTYP[command->COMMAND & 0x3F];
    if ((0 == xfertyp) && (0 != command->COMMAND))
    {
        return 1;
    }

    /* Card removal check preparation */
    SDHC_IRQSTAT |= SDHC_IRQSTAT_CRM_MASK;

    /* Wait for cmd line idle */
    while (SDHC_PRSSTAT & SDHC_PRSSTAT_CIHB_MASK) {};

    /* Setup command */
    SDHC_CMDARG = command->ARGUMENT;
    xfertyp &= (~ SDHC_XFERTYP_CMDTYP_MASK);
    xfertyp |= SDHC_XFERTYP_CMDTYP(command->TYPE);
    if (ESDHC_TYPE_RESUME == command->TYPE)
    {
        xfertyp |= SDHC_XFERTYP_DPSEL_MASK;
    }
    if (ESDHC_TYPE_SWITCH_BUSY == command->TYPE)
    {
        if ((xfertyp & SDHC_XFERTYP_RSPTYP_MASK) == SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48))
        {
            xfertyp &= (~ SDHC_XFERTYP_RSPTYP_MASK);
            xfertyp |= SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48BUSY);
        }
        else
        {
            xfertyp &= (~ SDHC_XFERTYP_RSPTYP_MASK);
            xfertyp |= SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48);
        }
    }
    SDHC_BLKATTR &= (~ SDHC_BLKATTR_BLKCNT_MASK);
    if (0 != command->BLOCKS)
    {
        if ((xfertyp & SDHC_XFERTYP_RSPTYP_MASK) != SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48BUSY))
        {
            xfertyp |= SDHC_XFERTYP_DPSEL_MASK;
        }
        if (command->READ)
        {
            xfertyp |= SDHC_XFERTYP_DTDSEL_MASK;
        }
        if (command->BLOCKS > 1)
        {
            xfertyp |= SDHC_XFERTYP_MSBSEL_MASK;
        }
        if ((uint32) - 1 != command->BLOCKS)
        {
            SDHC_BLKATTR |= SDHC_BLKATTR_BLKCNT(command->BLOCKS);
            xfertyp |= SDHC_XFERTYP_BCEN_MASK;
        }
    }

    /* Issue command */
    SDHC_DSADDR = 0;
    SDHC_XFERTYP = xfertyp;

    /* Wait for response */
    if (SDHC_status_wait (SDHC_IRQSTAT_CIE_MASK | SDHC_IRQSTAT_CEBE_MASK | SDHC_IRQSTAT_CCE_MASK | SDHC_IRQSTAT_CC_MASK) != SDHC_IRQSTAT_CC_MASK)
    {
        SDHC_IRQSTAT |= SDHC_IRQSTAT_CTOE_MASK | SDHC_IRQSTAT_CIE_MASK | SDHC_IRQSTAT_CEBE_MASK | SDHC_IRQSTAT_CCE_MASK | SDHC_IRQSTAT_CC_MASK;
        return 1;
    }

    /* Check card removal */
    if (SDHC_IRQSTAT & SDHC_IRQSTAT_CRM_MASK)
    {
        SDHC_IRQSTAT |= SDHC_IRQSTAT_CTOE_MASK | SDHC_IRQSTAT_CC_MASK;
        return 1;
    }

    /* Get response, if available */
    if (SDHC_IRQSTAT & SDHC_IRQSTAT_CTOE_MASK)
    {
        SDHC_IRQSTAT |= SDHC_IRQSTAT_CTOE_MASK | SDHC_IRQSTAT_CC_MASK;
        return -1;
    }
    if ((xfertyp & SDHC_XFERTYP_RSPTYP_MASK) != SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_NO))
    {
        command->RESPONSE[0] = SDHC_CMDRSP(0);
        if ((xfertyp & SDHC_XFERTYP_RSPTYP_MASK) == SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_136))
        {
            command->RESPONSE[1] = SDHC_CMDRSP(1);
            command->RESPONSE[2] = SDHC_CMDRSP(2);
            command->RESPONSE[3] = SDHC_CMDRSP(3);
        }
    }

    SDHC_IRQSTAT |= SDHC_IRQSTAT_CC_MASK;

    return 0;
}


/*FUNCTION****************************************************************
*
* Function Name    : _esdhc_ioctl
* Returned Value   : MQX error code
* Comments         :
*    This function performs miscellaneous services for the ESDHC I/O device.
*
*END*********************************************************************/
int32 SDHC_ioctl
(
    /* [IN] The command to perform */
    uint32              cmd,

    /* [IN/OUT] Parameters for the command */
    void                *param_ptr
)
{
    ESDHC_INFO_STRUCT_PTR   esdhc_info_ptr;
    ESDHC_DEVICE_STRUCT_PTR esdhc_device_ptr;
    ESDHC_INIT_STRUCT_CPTR  esdhc_init_ptr;
    ESDHC_COMMAND_STRUCT    command;
    uint8                   mem, io, mmc, ceata, mp, hc;
    int32                  val;
    uint32                  result = ESDHC_OK;
    uint32             	    *param32_ptr = param_ptr;

    /* Check parameters */
    esdhc_info_ptr = (ESDHC_INFO_STRUCT_PTR)&SDHC_Info;

    if (NULL == esdhc_info_ptr)
    {
        return IO_DEVICE_DOES_NOT_EXIST;
    }

    esdhc_device_ptr = &SDHC_Device;
    if (NULL == esdhc_device_ptr)
    {
        return IO_ERROR_DEVICE_INVALID;
    }

    esdhc_init_ptr = esdhc_device_ptr->INIT;
    if (NULL == esdhc_init_ptr)
    {
        return IO_ERROR_DEVICE_INVALID;
    }

    switch (cmd)
    {
    case IO_IOCTL_ESDHC_INIT:

        result = SDHC_init (esdhc_info_ptr, &K60_SDHC0_init);
        if (ESDHC_OK != result)
        {
            break;
        }

        mem = FALSE;
        io = FALSE;
        mmc = FALSE;
        ceata = FALSE;
        hc = FALSE;
        mp = FALSE;

        /* CMD0 - Go to idle - reset card */
        command.COMMAND = ESDHC_CMD0;
        command.TYPE = ESDHC_TYPE_NORMAL;
        command.ARGUMENT = 0;
        command.READ = FALSE;
        command.BLOCKS = 0;
        if (SDHC_send_command (&command))
        {
            result = ESDHC_ERROR_INIT_FAILED;
            break;
        }

        DELAY_MS(1100);

        /* CMD8 - Send interface condition - check HC support */
        command.COMMAND = ESDHC_CMD8;
        command.TYPE = ESDHC_TYPE_NORMAL;
        command.ARGUMENT = 0x000001AA;
        command.READ = FALSE;
        command.BLOCKS = 0;
        val = SDHC_send_command (&command);

        if (val == 0)
        {
            // SDHC Card
            if (command.RESPONSE[0] != command.ARGUMENT)
            {
                result = ESDHC_ERROR_INIT_FAILED;
                break;
            }
            hc = TRUE;
        }

        mp = TRUE;

        if (mp)
        {
            /* CMD55 - Application specific command - check MMC */
            command.COMMAND = ESDHC_CMD55;
            command.TYPE = ESDHC_TYPE_NORMAL;
            command.ARGUMENT = 0;
            command.READ = FALSE;
            command.BLOCKS = 0;
            val = SDHC_send_command (&command);
            if (val > 0)
            {
                result = ESDHC_ERROR_INIT_FAILED;
                break;
            }
            if (val < 0)
            {
                /* MMC or CE-ATA */
                io = FALSE;
                mem = FALSE;
                hc = FALSE;

                /* CMD1 - Send operating conditions - check HC */
                command.COMMAND = ESDHC_CMD1;
                command.TYPE = ESDHC_TYPE_NORMAL;
                command.ARGUMENT = 0x40300000;
                command.READ = FALSE;
                command.BLOCKS = 0;
                if (SDHC_send_command (&command))
                {
                    result = ESDHC_ERROR_INIT_FAILED;
                    break;
                }
                if (0x20000000 == (command.RESPONSE[0] & 0x60000000))
                {
                    hc = TRUE;
                }
                mmc = TRUE;

                /* CMD39 - Fast IO - check CE-ATA signature CE */
                command.COMMAND = ESDHC_CMD39;
                command.TYPE = ESDHC_TYPE_NORMAL;
                command.ARGUMENT = 0x0C00;
                command.READ = FALSE;
                command.BLOCKS = 0;
                if (SDHC_send_command (&command))
                {
                    result = ESDHC_ERROR_INIT_FAILED;
                    break;
                }
                if (0xCE == (command.RESPONSE[0] >> 8) & 0xFF)
                {
                    /* CMD39 - Fast IO - check CE-ATA signature AA */
                    command.COMMAND = ESDHC_CMD39;
                    command.TYPE = ESDHC_TYPE_NORMAL;
                    command.ARGUMENT = 0x0D00;
                    command.READ = FALSE;
                    command.BLOCKS = 0;
                    if (SDHC_send_command (&command))
                    {
                        result = ESDHC_ERROR_INIT_FAILED;
                        break;
                    }
                    if (0xAA == (command.RESPONSE[0] >> 8) & 0xFF)
                    {
                        mmc = FALSE;
                        ceata = TRUE;
                    }
                }
            }
            else
            {
                /* SD */
                /* ACMD41 - Send Operating Conditions */
                command.COMMAND = ESDHC_ACMD41;
                command.TYPE = ESDHC_TYPE_NORMAL;
                command.ARGUMENT = 0;
                command.READ = FALSE;
                command.BLOCKS = 0;
                if (SDHC_send_command (&command))
                {
                    result = ESDHC_ERROR_INIT_FAILED;
                    break;
                }
                if (command.RESPONSE[0] & 0x300000)
                {
                    val = 0;
                    do
                    {
                        DELAY_MS(BSP_ALARM_RESOLUTION);
                        val++;

                        /* CMD55 + ACMD41 - Send OCR */
                        command.COMMAND = ESDHC_CMD55;
                        command.TYPE = ESDHC_TYPE_NORMAL;
                        command.ARGUMENT = 0;
                        command.READ = FALSE;
                        command.BLOCKS = 0;
                        if (SDHC_send_command (&command))
                        {
                            result = ESDHC_ERROR_INIT_FAILED;
                            break;
                        }

                        command.COMMAND = ESDHC_ACMD41;
                        command.TYPE = ESDHC_TYPE_NORMAL;
                        if (hc)
                        {
                            command.ARGUMENT = 0x40300000;
                        }
                        else
                        {
                            command.ARGUMENT = 0x00300000;
                        }
                        command.READ = FALSE;
                        command.BLOCKS = 0;
                        if (SDHC_send_command (&command))
                        {
                            result = ESDHC_ERROR_INIT_FAILED;
                            break;
                        }
                    }
                    while ((0 == (command.RESPONSE[0] & 0x80000000)) && (val < BSP_ALARM_FREQUENCY));
                    if (ESDHC_OK != result)
                    {
                        break;
                    }
                    if (val >= BSP_ALARM_FREQUENCY)
                    {
                        hc = FALSE;
                    }
                    else
                    {
                        mem = TRUE;
                        if (hc)
                        {
                            hc = FALSE;
                            if (command.RESPONSE[0] & 0x40000000)
                            {
                                hc = TRUE;
                            }
                        }
                    }
                }
            }
        }


        if (mmc)
        {
            esdhc_info_ptr->CARD = ESDHC_CARD_MMC;
        }
        if (ceata)
        {
            esdhc_info_ptr->CARD = ESDHC_CARD_CEATA;
        }
        if (io)
        {
            esdhc_info_ptr->CARD = ESDHC_CARD_SDIO;
        }
        if (mem)
        {
            esdhc_info_ptr->CARD = ESDHC_CARD_SD;
            if (hc)
            {
                esdhc_info_ptr->CARD = ESDHC_CARD_SDHC;
            }
        }
        if (io && mem)
        {
            esdhc_info_ptr->CARD = ESDHC_CARD_SDCOMBO;
            if (hc)
            {
                esdhc_info_ptr->CARD = ESDHC_CARD_SDHCCOMBO;
            }
        }

        /* De-Init GPIO */
        PORTE_PCR(0) = 0 & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    /* ESDHC.D1  */
        PORTE_PCR(1) = 0 & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    /* ESDHC.D0  */
        PORTE_PCR(2) = 0 & (PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK);                                          /* ESDHC.CLK */
        PORTE_PCR(3) = 0 & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    /* ESDHC.CMD */
        PORTE_PCR(4) = 0 & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    /* ESDHC.D3  */
        PORTE_PCR(5) = 0 & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    /* ESDHC.D2  */

        /* Set the ESDHC default baud rate */
        //SDHC_set_baudrate (esdhc_init_ptr->CLOCK_SPEED, esdhc_init_ptr->BAUD_RATE);
        SDHC_set_baudrate (core_clk_khz * 1000, esdhc_init_ptr->BAUD_RATE);

        /* Init GPIO again */
        PORTE_PCR(0) = 0xFFFF & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    /* ESDHC.D1  */
        PORTE_PCR(1) = 0xFFFF & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    /* ESDHC.D0  */
        PORTE_PCR(2) = 0xFFFF & (PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK);                                          /* ESDHC.CLK */
        PORTE_PCR(3) = 0xFFFF & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    /* ESDHC.CMD */
        PORTE_PCR(4) = 0xFFFF & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    /* ESDHC.D3  */
        PORTE_PCR(5) = 0xFFFF & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    /* ESDHC.D2  */

        /* Enable clock gate to SDHC module */
        SIM_SCGC3 |= SIM_SCGC3_SDHC_MASK;

        break;
    case IO_IOCTL_ESDHC_SEND_COMMAND:
        val = SDHC_send_command ((ESDHC_COMMAND_STRUCT_PTR)param32_ptr);
        if (val > 0)
        {
            result = ESDHC_ERROR_COMMAND_FAILED;
        }
        if (val < 0)
        {
            result = ESDHC_ERROR_COMMAND_TIMEOUT;
        }
        break;
    case IO_IOCTL_ESDHC_GET_BAUDRATE:
        if (NULL == param32_ptr)
        {
            result = BRTOS_INVALID_PARAMETER;
        }
        else
        {
            /* Get actual baudrate */
            val = ((SDHC_SYSCTL & SDHC_SYSCTL_SDCLKFS_MASK) >> SDHC_SYSCTL_SDCLKFS_SHIFT) << 1;
            val *= ((SDHC_SYSCTL & SDHC_SYSCTL_DVS_MASK) >> SDHC_SYSCTL_DVS_SHIFT) + 1;
            //*param32_ptr = (uint32)((esdhc_init_ptr->CLOCK_SPEED) / val);
            *param32_ptr = (uint32)((core_clk_khz * 1000) / val);
        }
        break;
    case IO_IOCTL_ESDHC_SET_BAUDRATE:
        if (NULL == param32_ptr)
        {
            result = BRTOS_INVALID_PARAMETER;
        }
        else if (0 == (*param32_ptr))
        {
            result = BRTOS_INVALID_PARAMETER;
        }
        else
        {
            if (! SDHC_is_running())
            {
                /* De-Init GPIO */
                PORTE_PCR(0) = 0 & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    /* ESDHC.D1  */
                PORTE_PCR(1) = 0 & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    /* ESDHC.D0  */
                PORTE_PCR(2) = 0 & (PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK);                                          /* ESDHC.CLK */
                PORTE_PCR(3) = 0 & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    /* ESDHC.CMD */
                PORTE_PCR(4) = 0 & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    /* ESDHC.D3  */
                PORTE_PCR(5) = 0 & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    /* ESDHC.D2  */

                /* Set closest baudrate */
                //SDHC_set_baudrate (esdhc_init_ptr->CLOCK_SPEED, *param32_ptr);
                SDHC_set_baudrate (core_clk_khz * 1000, *param32_ptr);

                /* Init GPIO again */
                PORTE_PCR(0) = 0xFFFF & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    /* ESDHC.D1  */
                PORTE_PCR(1) = 0xFFFF & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    /* ESDHC.D0  */
                PORTE_PCR(2) = 0xFFFF & (PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK);                                          /* ESDHC.CLK */
                PORTE_PCR(3) = 0xFFFF & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    /* ESDHC.CMD */
                PORTE_PCR(4) = 0xFFFF & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    /* ESDHC.D3  */
                PORTE_PCR(5) = 0xFFFF & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    /* ESDHC.D2  */

                /* Enable clock gate to SDHC module */
                SIM_SCGC3 |= SIM_SCGC3_SDHC_MASK;
            }
            else
            {
                result = IO_ERROR_DEVICE_BUSY;
            }
        }
        break;
    case IO_IOCTL_ESDHC_GET_BLOCK_SIZE:
        if (NULL == param32_ptr)
        {
            result = BRTOS_INVALID_PARAMETER;
        }
        else
        {
            /* Get actual ESDHC block size */
            *param32_ptr = (SDHC_BLKATTR & SDHC_BLKATTR_BLKSIZE_MASK) >> SDHC_BLKATTR_BLKSIZE_SHIFT;
        }
        break;
    case IO_IOCTL_ESDHC_SET_BLOCK_SIZE:
        if (NULL == param32_ptr)
        {
            result = BRTOS_INVALID_PARAMETER;
        }
        else
        {
            /* Set actual ESDHC block size */
            if (! SDHC_is_running())
            {
                if (*param32_ptr > 0x0FFF)
                {
                    result = BRTOS_INVALID_PARAMETER;
                }
                else
                {
                    SDHC_BLKATTR &= (~ SDHC_BLKATTR_BLKSIZE_MASK);
                    SDHC_BLKATTR |= SDHC_BLKATTR_BLKSIZE(*param32_ptr);
                }
            }
            else
            {
                result = IO_ERROR_DEVICE_BUSY;
            }
        }
        break;
    case IO_IOCTL_ESDHC_GET_BUS_WIDTH:
        if (NULL == param32_ptr)
        {
            result = BRTOS_INVALID_PARAMETER;
        }
        else
        {
            /* Get actual ESDHC bus width */
            val = (SDHC_PROCTL & SDHC_PROCTL_DTW_MASK) >> SDHC_PROCTL_DTW_SHIFT;
            if (ESDHC_PROCTL_DTW_1BIT == val)
            {
                *param32_ptr = ESDHC_BUS_WIDTH_1BIT;
            }
            else if (ESDHC_PROCTL_DTW_4BIT == val)
            {
                *param32_ptr = ESDHC_BUS_WIDTH_4BIT;
            }
            else if (ESDHC_PROCTL_DTW_8BIT == val)
            {
                *param32_ptr = ESDHC_BUS_WIDTH_8BIT;
            }
            else
            {
                result = ESDHC_ERROR_INVALID_BUS_WIDTH;
            }
        }
        break;
    case IO_IOCTL_ESDHC_SET_BUS_WIDTH:
        if (NULL == param32_ptr)
        {
            result = BRTOS_INVALID_PARAMETER;
        }
        else
        {
            /* Set actual ESDHC bus width */
            if (! SDHC_is_running())
            {
                if (ESDHC_BUS_WIDTH_1BIT == *param32_ptr)
                {
                    SDHC_PROCTL &= (~ SDHC_PROCTL_DTW_MASK);
                    SDHC_PROCTL |= SDHC_PROCTL_DTW(ESDHC_PROCTL_DTW_1BIT);
                }
                else if (ESDHC_BUS_WIDTH_4BIT == *param32_ptr)
                {
                    SDHC_PROCTL &= (~ SDHC_PROCTL_DTW_MASK);
                    SDHC_PROCTL |= SDHC_PROCTL_DTW(ESDHC_PROCTL_DTW_4BIT);
                }
                else if (ESDHC_BUS_WIDTH_8BIT == *param32_ptr)
                {
                    SDHC_PROCTL &= (~ SDHC_PROCTL_DTW_MASK);
                    SDHC_PROCTL |= SDHC_PROCTL_DTW(ESDHC_PROCTL_DTW_8BIT);
                }
                else
                {
                    result = ESDHC_ERROR_INVALID_BUS_WIDTH;
                }
            }
            else
            {
                result = IO_ERROR_DEVICE_BUSY;
            }
        }
        break;
    case IO_IOCTL_ESDHC_GET_CARD:
        if (NULL == param32_ptr)
        {
            result = BRTOS_INVALID_PARAMETER;
        }
        else
        {
            /* 80 clocks to update levels */
            SDHC_SYSCTL |= SDHC_SYSCTL_INITA_MASK;
            while (SDHC_SYSCTL & SDHC_SYSCTL_INITA_MASK)
                { };

            /* Update and return actual card status */
            if (SDHC_IRQSTAT & SDHC_IRQSTAT_CRM_MASK)
            {
                SDHC_IRQSTAT |= SDHC_IRQSTAT_CRM_MASK;
                esdhc_info_ptr->CARD = ESDHC_CARD_NONE;
            }
            if (SDHC_PRSSTAT & SDHC_PRSSTAT_CINS_MASK)
            {
                if (ESDHC_CARD_NONE == esdhc_info_ptr->CARD)
                {
                    esdhc_info_ptr->CARD = ESDHC_CARD_UNKNOWN;
                }
            }
            else
            {
                esdhc_info_ptr->CARD = ESDHC_CARD_NONE;
            }
            *param32_ptr = esdhc_info_ptr->CARD;
        }
        break;
    case IO_IOCTL_DEVICE_IDENTIFY:
        /* Get ESDHC device parameters */
        param32_ptr[IO_IOCTL_ID_PHY_ELEMENT]  = IO_DEV_TYPE_PHYS_ESDHC;
        param32_ptr[IO_IOCTL_ID_LOG_ELEMENT]  = IO_DEV_TYPE_LOGICAL_MFS;
        param32_ptr[IO_IOCTL_ID_ATTR_ELEMENT] = IO_ESDHC_ATTRIBS;
        /*
        if (esdhc_fd_ptr->FLAGS & IO_O_RDONLY)
        {
            param32_ptr[IO_IOCTL_ID_ATTR_ELEMENT] &= (~ IO_DEV_ATTR_WRITE);
        }
        */
        break;
    case IO_IOCTL_FLUSH_OUTPUT:
        /* Wait for transfer complete */
        SDHC_status_wait (SDHC_IRQSTAT_TC_MASK);
        if (SDHC_IRQSTAT & (SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK))
        {
            SDHC_IRQSTAT |= SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK;
            result = ESDHC_ERROR_DATA_TRANSFER;
        }
        SDHC_IRQSTAT |= SDHC_IRQSTAT_TC_MASK | SDHC_IRQSTAT_BRR_MASK | SDHC_IRQSTAT_BWR_MASK;
        break;
    default:
        result = IO_ERROR_INVALID_IOCTL_CMD;
        break;
    }
    return result;
}
