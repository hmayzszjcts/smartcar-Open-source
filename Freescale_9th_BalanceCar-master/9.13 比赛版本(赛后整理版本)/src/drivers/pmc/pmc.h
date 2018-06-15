/*
 * File:		pmc.h
 * Purpose:     Provides low power mode entry routines
 *
 * Notes:
 */

#ifndef __PMC_H__
#define __PMC_H__

/********************************************************************/

// function prototypes
void enter_wait(void);
void enter_stop(void);
void enter_vlpr(char lpwui_value);
void exit_vlpr(void);
void enter_vlps(char lpwui_value);
void enter_lls(void);
void enter_vlls3(void);
void enter_vlls2(void);
void enter_vlls1(void);
void enter_stop_mode(char mode);

void enable_lpwui(void);
void disable_lpwui(void);

/********************************************************************/
#endif /* __PMC_H__ */
