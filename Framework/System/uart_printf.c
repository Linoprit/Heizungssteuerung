/*
 * uart_messaging.c
 *
 *  Created on: 16.12.2018
 *      Author: harald
 */

#include "uart_printf.h"
#include "cmsis_os.h"
#include <stdio.h>
#include <stdarg.h>

extern UART_HandleTypeDef* get_huart1(void);
#define UART_PORT get_huart1()

#define TX_BUFF_LEN 128
uint8_t txBuff[TX_BUFF_LEN];
uint16_t tx_act_pos = 0;


#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

/*
 to enable float formatted output, see:
 https://stackoverflow.com/questions/28334435/stm32-printf-float-variable.
 - add -u _printf_float to your LDFLAGS.
 or
 - printf("Result is: %d.%d", i/10, i%10);
 */

int tx_printf(const char *format, ...)
{
	const uint16_t timeout = 0x00FF;
	volatile uint16_t count = 0;
	HAL_UART_StateTypeDef uart_status = HAL_UART_GetState(UART_PORT);

	while ( ( (uart_status != HAL_UART_STATE_READY)  &&
			(uart_status != HAL_UART_STATE_BUSY_RX) )  )
	{
		if(count >= timeout)
		{
			return ERROR;
		}
		osDelay(1);
		count++;
		uart_status = HAL_UART_GetState(UART_PORT);
	}

	tx_buff_clear();

	va_list arg;
	va_start (arg, format);
	//done = vfprintf (stdout, format, arg);
	tx_act_pos = vsprintf ((char*) &txBuff[0], format, arg);
	va_end (arg);

	HAL_UART_Transmit_DMA(UART_PORT, &txBuff[0], tx_act_pos);
	return SUCCESS;
}

void tx_buff_clear(void)
{
	for(uint16_t i=0; i < TX_BUFF_LEN; i++)
	{
		txBuff[i] = '\0';
	}
	tx_act_pos = 0;
}
