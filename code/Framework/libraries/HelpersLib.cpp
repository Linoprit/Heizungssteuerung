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
