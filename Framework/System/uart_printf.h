/*
 * uart_messaging.h
 *
 *  Created on: 16.12.2018
 *      Author: harald
 */

#ifndef UART_PRINTF_H_
#define UART_PRINTF_H_

#include "stm32f1xx_hal.h"

// C interface
#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

EXTERNC int tx_printf(const char *format, ...);
EXTERNC void tx_buff_clear(void);

#undef EXTERNC
#ifdef __cplusplus
// put cpp includes here!!

#endif // C interface
#endif /* UART_PRINTF_H_ */
