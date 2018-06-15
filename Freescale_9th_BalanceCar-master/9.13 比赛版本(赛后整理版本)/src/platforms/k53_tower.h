/*
 * File:        k60_tower.h
 * Purpose:     Definitions for the Kinetis K60 tower card
 *
 * Notes:
 */

#ifndef __K53_TOWER_H__
#define __K53_TOWER_H__

#include "mcg.h"

/********************************************************************/

/* Global defines to use for all boards */
#define DEBUG_PRINT



/* Defines specific to the K53 tower board */


/* Define for the CPU on the K53board */
#define CPU_MK53N512CMD100

/*
 * System Bus Clock Info
 */
#define K53_CLK             1
#define REF_CLK             XTAL8   /* value isn't used, but we still need something defined */
#define CORE_CLK_MHZ        PLL96      /* 96MHz is only freq tested for a clock input*/

/*
 * Serial Port Info
 */

/*
 * Select the serial port that is being used below. Only one of the
 * options should be uncommented at any time.
 */
//#define SERIAL_CARD     // use this option for serial port on TWR-SER
#define OSJTAG         // use this option for serial port over the OS-JTAG circuit

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


#endif /* __K53_TOWER_H__ */
