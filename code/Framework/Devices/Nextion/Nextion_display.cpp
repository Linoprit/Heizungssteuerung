/*
 * Error_messaging.cpp
 *
 *  Created on: 19.03.2017
 *      Author: harald
 */

#include <Devices/Nextion/Nextion_display.h>
#include <string.h>
#include "Instances/Common.h"
#include "libraries/Arduino/itoa.h"


nextion::nextion(UART_HandleTypeDef *huart)
{
  display_comm = new uart_socket( huart );

}

uart_socket* nextion::get_socket(void)
{
  return display_comm;
}

size_t nextion::write(const char* buf, size_t nbyte)
{
  while (display_comm->is_TX_pending())
	{
	}

  while (!display_comm->send_many_bytes((uint8_t*) buf, (uint8_t) nbyte))
	{	}

  return nbyte;
}

size_t nextion::readBytes(char* buffer, size_t nbyte)
{
  uint8_t count = 0;
  simpleRingbuffer* rx_buffer = display_comm->get_rx_ringbuffer();

  if (!rx_buffer->HasData())
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

void nextion::processRx(void)
{


}

void nextion::setTimeout(uint32_t timeout)
{
  UNUSED(timeout);
  // for compatibility with Nextion-lib
}

uint8_t nextion::available(void)
{
  return display_comm->get_rx_ringbuffer()->Count();
}

uint8_t nextion::read(void)
{
  return display_comm->get_rx_ringbuffer()->Read();
}








