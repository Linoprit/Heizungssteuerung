/*
 * simpleRingbuffer.h
 *
 *  Created on: 28.10.2015
 *      Author: harald
 *
 *  Simple Implementation for uint8_t contents.
 */

#ifndef LIBRARIES_SIMPLERINGBUFFER_H_
#define LIBRARIES_SIMPLERINGBUFFER_H_

#include <stdlib.h>
#include <stdint.h>

class simpleRingbuffer
{
public:
  simpleRingbuffer (uint8_t length);

  void Write(uint8_t c);
  uint8_t HasData(void);
  uint8_t Count(void);
  uint8_t Read(void);
  uint8_t buffer_size(void);

  // read item and don't change the index
  uint8_t Get(void);

private:
  uint8_t *buffer;
  uint8_t size;
  uint8_t readIndex;
  uint8_t writeIndex;

};

#endif /* LIBRARIES_SIMPLERINGBUFFER_H_ */
