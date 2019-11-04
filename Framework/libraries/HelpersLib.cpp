/*
 * HelpersLib.cpp
 *
 *  Created on: 26.10.2015
 *      Author: harald
 */

#include "HelpersLib.h"
#include <stdlib.h>
#include "Arduino/itoa.h"


HelpersLib::HelpersLib ()
{
}

uint8_t HelpersLib::findFirstSetBitFromLeft(uint8_t inByte)
{
  if (inByte == 0)
	return 0xff;

  uint8_t offset = 0;

  if ((inByte & 0b11110000) > 0)
	{
	  offset += 4;
	  inByte = (inByte >> 4);
	}

  if ((inByte & 0b00001100) > 0)
	{
	  offset += 2;
	  inByte = (inByte >> 2);
	}

  if ((inByte & 0b00000010) > 0)
	{
	  offset += 1;
	}

  return offset;
}

uint8_t HelpersLib::findFirstSetBitFromRight(uint8_t inByte)
{
  if (inByte == 0)
	return 0xff;

  uint8_t offset = 0;

  if ((inByte & 0b00001111) == 0)
	{
	  offset += 4;
	  inByte = (inByte >> 4);
	}

  if ((inByte & 0b00000011) == 0)
	{
	  offset += 2;
	  inByte = (inByte >> 2);
	}

  if ((inByte & 0b00000001) == 0)
	{
	  offset += 1;
	}

  return offset;
}

void HelpersLib::value2char(
	char* result_buff, uint8_t result_buff_len,
	uint8_t exp, long int value)
{
  // long range: -2,147,483,648 to 2,147,483,647
  char tmp_buff[MAX_BUFFER_LEN];
  uint8_t tmp_buff_len = 0, min_buff_len, i, j;
  uint8_t zero_buff_len;

  if (value != 0)
	ltoa(value, tmp_buff, DEC_RADIX);
  else // avoid problems with zero values
	{
	  if (exp > 0)
		zero_buff_len = result_buff_len - 1;
	  else
		zero_buff_len = result_buff_len;

	  for (i = 0; i < zero_buff_len; i++)
		tmp_buff[i] = '0';

	  tmp_buff[zero_buff_len] = '\0';
	}

  // count result len
  for (i = 0; i < MAX_BUFFER_LEN; i++)
	if (tmp_buff[i] != '\0')
	  tmp_buff_len++;
	else
	  break;

  // check necessary result_buff_len
  min_buff_len = tmp_buff_len;

  if (exp > 0)
	min_buff_len++;

  // tmp_buff fits into result buff?
  if ((min_buff_len > result_buff_len) || (exp >= result_buff_len))
	{
	  for (i = 0; i < result_buff_len; i++)
		result_buff[i] = '#';
	  return;
	}

  // pad zeros
  for (i = 0; i < result_buff_len; i++)
	result_buff[i] = '0';

  // copy tail into result
  if (exp >= min_buff_len)
	{
	  for (i = 0; i < tmp_buff_len; i++)
		result_buff[result_buff_len - tmp_buff_len + i] = tmp_buff[i];

	  // place dot
	  result_buff[result_buff_len - exp - 1] = '.';
	}
  else
	{
	  for (i = 0; i < exp; i++)
		result_buff[result_buff_len - exp + i] =
			tmp_buff[tmp_buff_len - exp + i];

	  // copy leading part
	  i = tmp_buff_len - exp;

	  if (exp > 0)
		{
		  j = result_buff_len - exp - 1;
		  // place dot
		  result_buff[j] = '.';
		}
	  else
		j = result_buff_len - exp;

	  while (i > 0)
		{
		  i--;
		  j--;

		  if (tmp_buff[i] == '-')
			{
			  result_buff[0] = '-';
			  break;
			}
		  else
			result_buff[j] = tmp_buff[i];
		}
	}

  return;
}

void HelpersLib::num2str(String* msg_str, uint32_t number)
{
  char buf[10] = {0};
  utoa(number, buf, 10);
  *msg_str += buf;
}

/* following is based on usart.c functions by Lonley Wolf.
 * See Github: https://github.com/LonelyWolf/stm32/tree/master/nrf24l01 */

#include <stdarg.h>
#define HEX_CHARS      "0123456789ABCDEF"

void HelpersLib::hex2str(String* msg_str, uint8_t* buf)
{
	uint8_t ch = *buf;
	*msg_str += (HEX_CHARS[(ch >> 4)   % 0x10]);
	*msg_str += (HEX_CHARS[(ch & 0x0f) % 0x10]);
}

// Actually some formats don't work. Will fix it sometime...
//
// Transmit formatted string
// input:
//   msg_str - pointer to where we print in.
//   fmt - format string
//   ... - optional arguments
// return: number of transmitted characters
// note:
//   supported sub-set of standard format:
//     type:
//       i, d - signed integer
//       u    - unsinged integer
//       b    - binary
//       o    - octal
//       x    - hexadecimal in lowercase
//       X    - hexadecimal in uppercase
//       c    - character
//       s    - string
//     flag:
//       0    - add zero padding
//       -    - left justify field
//       .X   - add a decimal point (on 'X' place must be a single digit
//              determining number of digits after the decimal point)
// examples:
//   ("%d", 1234)     --> "1234"
//   ("%-5d", 123)    --> "123  "
//   ("%5d", 123)     --> "  123"
//   ("%05d", 123)    --> "00123"
//   ("%2s", "USART") --> "US"
//   ("%.3u", 123456) --> "123.456"
//   ("%-8.2u", 1234) --> "12.34   "
void HelpersLib::sprintf(String* msg_str, const char *fmt, ...)
{
	va_list ap;        // arguments pointer
	uint8_t flag;      // field flag
	uint8_t width;     // specified field width
	uint8_t prec;      // specified precision
	uint8_t len = 0;   // actual field length
	uint8_t radix;     // value conversion radix
	uint32_t num;      // value to convert
	char str[33];      // maximal width of 32-bit binary + string terminating symbol
	char *pstr, *tstr; // pointers for various string manipulations
	char chr;          // to hold a single character from format string

	// Initialize an arguments pointer
	va_start(ap, fmt);

	// Process format string
	while (1) {
		// Get character from the format string
		chr = *fmt++;

		// String termination?
		if (!chr) { break; }

		// Non escape character
		if (chr != '%') {
			*msg_str += chr;
			continue;
		}

		// Next after an escape character
		chr = *fmt++;

		// Flag?
		flag = 0x00;
		if (chr == '0') {
			// Flag '0' --> padding
			flag = 0x01;
			chr = *fmt++;
		} else if (chr == '-') {
			// Flag '-' --> left justified
			flag = 0x02;
			chr = *fmt++;
		}

		// Width?
		width = 0;
		while ((chr > '0' - 1) && (chr < '9' + 1)) {
			width *= 10;
			width += chr - '0';
			chr = *fmt++;
		}

		// Precision?
		prec = 0;
		if (chr == '.') {
			chr = *fmt++;
			if ((chr > '0' - 1) && (chr < '9' + 1)) {
				// Flag '.' --> number with decimal point
				prec = chr - '0';
				if (prec) { flag |= 0x08; }
				chr = *fmt++;
			}
		}

		// Ensure a string is not terminated yet
		if (!chr) { break; }

		// A type is...
		// (chr | 0x20) --> convert character to lower case
		switch (chr | 0x20) {
			case 's':
				// String

				// Calculate string length
				len = 0;
				tstr = pstr = va_arg(ap, char*);
				while (*pstr++) { len++; }

				// Transmit leading spaces if string length is less than field width
				if (!(flag & 0x02) && (len < width)) {
					while (len++ < width) {
						*msg_str += (' ');
					}
				}

				// Transmit string
				pstr = tstr;
				if (width) len = width;
				while (*pstr) {
					*msg_str += (*pstr++);
					if (!(--len)) { break; }
				}

				// Transmit trailing spaces in case of left justified field
				if (flag & 0x02) {
					while (len--) {
						*msg_str += (' ');
					}
				}

				continue;
			case 'c':
				// Character
				*msg_str += ((char)va_arg(ap, int));
				continue;
			case 'b':
				// Binary
				num = (uint32_t)va_arg(ap, uint32_t);
				radix = 2;
				break;
			case 'o':
				// Octal
				num = (uint32_t)va_arg(ap, uint32_t);
				radix = 8;
				break;
			case 'i':
			case 'd':
				// Signed decimal
				num = (int32_t)va_arg(ap, int32_t);
				if (num & 0x80000000) {
					num = 0 - num;
					flag |= 0x04;
				}
				radix = 10;
				break;
			case 'u':
				// Unsigned decimal
				num = (uint32_t)va_arg(ap, uint32_t);
				radix = 10;
				break;
			case 'x':
				// Hexadecimal
				num = (uint32_t)va_arg(ap, uint32_t);
				radix = 16;
				break;
			default:
				// Unknown: just pass
				*msg_str += (chr);
				continue;
		}

		// Convert value to characters
		pstr = str;
		*pstr++ = '\0';
		len = 0;
		if (num) {
			// Value is not a zero
			do {
				*pstr = (num % radix);
				if (*pstr > 9) {
					*pstr += (chr == 'x') ? 0x27 : 0x07;
				}
				*pstr++ += '0';
				len++;
				if (flag & 0x08) {
					if (len == prec) {
						*pstr++ = '.';
						len++;
					}
				}
			} while (num /= radix);

			// In case of format with decimal point the leading zeroes must be added if
			// current length of the number is less than a specified precision
			if (flag & 0x08) {
				if (len == prec + 1) {
					*pstr++ = '0';
					len++;
				} else if (len <= prec) {
					while (len < prec) {
						*pstr++ = '0';
						len++;
					}
					*pstr++ = '.';
					*pstr++ = '0';
					len += 2;
				}
			}

			// Add negative symbol
			if (flag & 0x04) {
				*pstr++ = '-';
				len++;
			}
		} else {
			// Little optimization for a zero value
			if (flag & 0x08) {
				while (len++ < prec) { *pstr++ = '0'; }
				*pstr++ = '.';
				len++;
			} else len++;
			*pstr++ = '0';
		}

		// Add leading zeroes or spaces
		prec += (flag & 0x08) ? (width - prec) : width;
		if (!(flag & 0x02) && (len < prec)) {
			while (len++ < prec) {
				*pstr++ = (flag & 0x01) ? '0' : ' ';
			}
		}

		// Transmit value as text
		while (*--pstr) {
			*msg_str += (*pstr);
		}

		// Transmit trailing spaces
		if (flag & 0x02) {
			while (len++ < width) {
				*msg_str += (' ');
			}
		}
	}

	// Cleanup for an arguments pointer
	va_end(ap);
}
