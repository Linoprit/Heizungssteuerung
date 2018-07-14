/*
 * Value_PT1_filtered.h
 *
 *  Created on: 28.12.2017
 *      Author: harald
 */

#ifndef SYSTEM_VALUE_PT1_FILTERED_H_
#define SYSTEM_VALUE_PT1_FILTERED_H_

#include <stdint.h>

class Value_PT1_filtered
{

public:
  Value_PT1_filtered (float k_in);
  virtual ~Value_PT1_filtered () { };

  int32_t  getValue_int(uint8_t decimal_places);

  float    get_Value_float(void);

  void 	   toString(char* buffer, uint8_t len, uint8_t decimal_places);

  void 	   cycle(float new_value);

protected:
  float value; // Y(n)
  float last_value; // Y(n-1)
  float k;

};

#endif /* SYSTEM_VALUE_PT1_FILTERED_H_ */
