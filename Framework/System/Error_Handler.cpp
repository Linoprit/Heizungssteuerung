/*
 * Error_Handler.cpp
 *
 *  Created on: 07.10.2019
 *      Author: harald
 */

#include <System/Error_Handler.h>
#include <System/uart_printf.h>
#include <stdint.h>
#include <stdio.h>
#include <main.h>


void error_handler(const char* file, uint32_t line)
{
	HAL_GPIO_TogglePin(LED_04_GPIO_Port, LED_04_Pin);

	tx_printf("Exception in %s, line: %ld\n", file, line);

	while (1) { }; // infinite loop
}

