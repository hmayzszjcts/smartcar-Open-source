/*
 * File:        k40_tower.h
 * Purpose:     Definitions for the Kinetis K40 tower card
 *
 * Notes:
 */

#ifndef __K40_TOWER_H__
#define __K40_TOWER_H__

#include "mcg.h"

/********************************************************************/

/* Global defines to use for all boards */
#define DEBUG_PRINT


/* Defines specific to the K40 tower board */


/* Define for the CPU on the K60 board */
#define CPU_MK40N512VMD100

/*
 * System Bus Clock Info
 */
#define REF_CLK             XTAL8
#define CORE_CLK_MHZ        PLL96      /* system/core frequency in MHz */


/*
 * Serial Port Info
 */

/*
 * Select the serial port that is being used below. Only one of the
 * options should be uncommented at any time.
 */
//#define SERIAL_CARD     // use this option for serial port over TWR-SER
#define OSJTAG        // use this option for serial port over the OS-JTAG circuit

#if (defined(SERIAL_CARD))
#define TERM_PORT           UART3_BASE_PTR
#define TERMINAL_BAUD       115200
#undef  HW_FLOW_CONTROL
#elif (defined(OSJTAG))
#define TERM_PORT           UART0_BASE_PTR
#define TERMINAL_BAUD       115200
#undef  HW_FLOW_CONTROL
#else
#error "No valid serial port defined"
#endif


#endif /* __K40_TOWER_H__ */
