#ifndef _USART_FILE_H_
#define _USART_FILE_H_
#include "common.h"
#include "MK60_uart.h"

#define UART_PORT UART5
#define UART_BAUD 115200
extern void usart_init();
extern void uart5_handler(void);
#endif
