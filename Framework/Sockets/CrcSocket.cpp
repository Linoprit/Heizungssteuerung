/*
 * CrcSocket.cpp
 *
 *  Created on: 09.11.2019
 *      Author: harald
 */

#include <Sockets/CrcSocket.h>
#include <main.h>


CrcSocket::CrcSocket(CRC_HandleTypeDef *hcrc) {
	_hcrc = hcrc;
	HAL_CRC_DeInit(_hcrc);
}

uint32_t CrcSocket::calc(uint32_t pBuffer[], uint32_t BufferLength) {

	HAL_GPIO_WritePin(LED_03_GPIO_Port, LED_03_Pin, GPIO_PIN_SET);

	HAL_CRC_Init(_hcrc);

	HAL_GPIO_WritePin(LED_03_GPIO_Port, LED_03_Pin, GPIO_PIN_RESET);

	uint32_t result = HAL_CRC_Calculate(_hcrc, pBuffer, BufferLength);
	HAL_CRC_DeInit(_hcrc);





	return result;
}

/*
 * Not really a crc8 calculation, calculates a crc32 and uses only the last byte (LSB).
 */
uint8_t CrcSocket::calcU8(uint32_t pBuffer[], uint32_t BufferLength) {
	uint32_t result32 = calc(pBuffer, BufferLength);
	result32 = result32 & 0x000F;
	return static_cast<uint8_t>(result32);
}
