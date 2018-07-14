/*
 * simpleRingbuffer.cpp
 *
 *  Created on: 28.10.2015
 *      Author: harald
 */

#include "simpleRingbuffer.h"


simpleRingbuffer::simpleRingbuffer (uint8_t ringBufferLength)
{
  buffer 		= new uint8_t[ringBufferLength];
  size 			= ringBufferLength;
  readIndex  	= 0;
  writeIndex 	= 0;
}

void simpleRingbuffer::Write(uint8_t c)
{
  uint8_t   newWriteIndex = writeIndex + 1;

  if(newWriteIndex >= size)
    newWriteIndex = 0;
  if(newWriteIndex != readIndex)
    { /* not yet full */
      buffer[writeIndex] = c;
      writeIndex = newWriteIndex;
    }
}

uint8_t simpleRingbuffer::HasData(void)
{
    return readIndex != writeIndex;
}

uint8_t simpleRingbuffer::Count(void)
{
  uint8_t   x = writeIndex - readIndex;

  if(writeIndex >= readIndex)
    return x;
  return x + size;
}

uint8_t simpleRingbuffer::Read(void)
{
  uint8_t c = buffer[readIndex];
  readIndex++;
  if(readIndex >= size)
    readIndex = 0;
  return c;
}

// read item and don't change the index
uint8_t simpleRingbuffer::Get(void)
{
  uint8_t c = buffer[readIndex];
  return c;
}

uint8_t simpleRingbuffer::buffer_size(void)
{
  return size;
}
