/*
 * CrcSocket.h
 *
 *  Created on: 09.11.2019
 *      Author: harald
 */

#ifndef CRCSOCKET_H_
#define CRCSOCKET_H_

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_crc.h"


class CrcSocket {
public:
	CrcSocket(CRC_HandleTypeDef *hcrc);
	virtual ~CrcSocket() {} ;

	uint32_t calc(uint32_t pBuffer[], uint32_t BufferLength);

	//Not really a crc8 calculation, calculates a crc32 and uses only the last byte (LSB).
	uint8_t  calcU8(uint32_t pBuffer[], uint32_t BufferLength);

private:
	CRC_HandleTypeDef *_hcrc;
};

#endif /* CRCSOCKET_H_ */
