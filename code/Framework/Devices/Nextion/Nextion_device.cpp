/*
 * Error_messaging.cpp
 *
 *  Created on: 19.03.2017
 *      Author: harald
 */

#include <Devices/Nextion/Nextion_device.h>
#include <string.h>
#include "cmsis_os.h"

#include "Instances/Common.h"
#include "libraries/Arduino/itoa.h"


nextion::nextion(uart_socket* socket)
{
  this->socket = socket;
}

size_t nextion::write(const char* buf, size_t nbyte)
{
  bool result =
	  socket->send_many_bytes((uint8_t*) buf, (uint8_t) nbyte, timeout);

  if (result == SUCCESS)
	return nbyte;

  return 0;
}

bool nextion::data_arrived(uint8_t expected_amount)
{
  uint8_t count = socket->get_rx_ringbuffer()->Count();
  uint32_t timeout_tick = Common::get_tick() + timeout;

  while (expected_amount > count)
	{
	  osSignalWait(0, timeout_tick - Common::get_tick());
	  count = socket->get_rx_ringbuffer()->Count();
	  if (Common::get_tick() >= timeout_tick)
		break;
	}

  return socket->get_rx_ringbuffer()->HasData();
}


size_t nextion::readBytes(char* buffer, size_t nbyte)
{
  uint8_t count = 0;
  simpleRingbuffer* rx_buffer = socket->get_rx_ringbuffer();

  if (!data_arrived(nbyte))
	return 0;

  while(rx_buffer->HasData())
	{
	  if (count >= nbyte)
		break;

	  buffer[count] = rx_buffer->Read();
	  count++;
	}

  return count;
}


/**
 * Use only in conjunction with nextion::available(void)!!
 */
uint8_t nextion::read(void)
{
  return socket->get_rx_ringbuffer()->Read();
}


uint8_t nextion::available(void)
{
  // if (!data_arrived(1))
  //	return 0;

  if(!socket->get_rx_ringbuffer()->HasData())
	return 0;

  return socket->get_rx_ringbuffer()->Count();
}






