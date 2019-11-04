/*
 * Error_messaging.cpp
 *
 *  Created on: 19.03.2017
 *      Author: harald
 */

#include "../Framework/Devices/Nextion/Nextion_device.h"
#include <string.h>
#include "cmsis_os.h"

#include "../Framework/Instances/Common.h"
#include "../Framework/libraries/Arduino/itoa.h"


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

bool nextion::wait_for_data(uint8_t expected_amount)
{
  uint16_t count = socket->get_rx_ringbuffer()->size();
  uint32_t timeout_tick = Common::get_tick() + timeout;

  while (expected_amount > count)
	{
	  osSignalWait(0x1, timeout_tick - Common::get_tick());
	  count = socket->get_rx_ringbuffer()->size();
	  if (Common::get_tick() >= timeout_tick)
		break;
	}

  return !socket->get_rx_ringbuffer()->isEmpty();
}


size_t nextion::readBytes(char* buffer, size_t nbyte)
{
  uint8_t count = 0;
  SimpleQueue<uint8_t>* rx_buffer = socket->get_rx_ringbuffer();

  if (!wait_for_data(nbyte))
	return 0;

  while(!rx_buffer->isEmpty())
	{
	  if (count >= nbyte)
		break;

	  buffer[count] = rx_buffer->dequeue();
	  count++;
	}

  return count;
}


/**
 * Use only in conjunction with nextion::available(void)!!
 */
uint8_t nextion::read(void)
{
  return socket->get_rx_ringbuffer()->dequeue();
}


uint8_t nextion::available(void)
{
  if(socket->get_rx_ringbuffer()->isEmpty())
	return 0;

  return socket->get_rx_ringbuffer()->size();
}






