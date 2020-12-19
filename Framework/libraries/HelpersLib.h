/*
 * HelpersLib.h
 *
 *  Created on: 26.10.2015
 *      Author: harald
 */

#ifndef LIBRARIES_HELPERSLIB_H_
#define LIBRARIES_HELPERSLIB_H_

#include <stdint.h>
#include "Arduino/WString.h"


// value2char: char* len, that holds long type result
#define MAX_BUFFER_LEN 12

// value2char: Base
#define BIN_RADIX 2
#define DEC_RADIX 10
#define HEX_RADIX 16




class HelpersLib
{
public:

  HelpersLib ();

  static uint8_t findFirstSetBitFromRight(uint8_t inByte);
  static uint8_t findFirstSetBitFromLeft(uint8_t inByte);
  static void value2char(
      char* result_buff, uint8_t result_buff_len,
      uint8_t exp, long int value);
  static void num2str(String *msg_str, uint32_t number);
  static void hex2str(String* msg_str, uint8_t *buf);
  static void sprintf(String* msg_str, const char *fmt, ...);


};

#endif /* LIBRARIES_HELPERSLIB_H_ */
