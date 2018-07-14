/*
 * Value_PT1_filtered.cpp
 *
 *  Created on: 28.12.2017
 *      Author: harald
 */

#include "libraries/HelpersLib.h"
#include <math.h>
#include <System/Value_PT1_filtered.h>


Value_PT1_filtered::Value_PT1_filtered (float k_in)
{
  value = 0;
  last_value = 0;
  k = k_in;
}


int32_t Value_PT1_filtered::getValue_int(uint8_t decimal_places)
{
  float factor = pow(10.0f, (float) decimal_places);
  return (int32_t) (get_Value_float() * factor);
}

float Value_PT1_filtered::get_Value_float(void)
{
  return value;
}

void Value_PT1_filtered::toString(char* buffer, uint8_t len, uint8_t decimal_places)
{
  HelpersLib::value2char(
	  (char*) &buffer[0], len, decimal_places, getValue_int(decimal_places));
}

void Value_PT1_filtered::cycle(float new_value)
{
  value = last_value - ( k * (last_value - new_value) );
  last_value = value;
}
