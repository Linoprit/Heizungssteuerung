/*
 * Error_messaging.cpp
 *
 *  Created on: 19.03.2017
 *      Author: harald
 */

#include <errno.h>
#include <System/Error_messaging.h>
#include "Instances/Common.h"
#include <libraries/HelpersLib.h>


#ifdef TRACE

uart_socket*	Error_messaging::socket		= NULL;
char			Error_messaging::input_buff[INPUT_BUFF_LEN];
uint8_t 		Error_messaging::cursor		= 0;



Error_messaging::Error_messaging(uart_socket* socket)
{
  this->socket = socket;
}

uart_socket* Error_messaging::get_debug_comm(void)
{
  return socket;
}

size_t Error_messaging::write(const char* buf, size_t nbyte)
{
  if (socket == NULL)
	return 0;

  bool result =
	  socket->send_many_bytes((uint8_t*) buf, (uint8_t) nbyte, timeout);

  if (result == SUCCESS)
	return nbyte;

  return 0;
}

size_t Error_messaging::write(const char* buf)
{
  return write(buf, (size_t) strlen(buf));
}

ssize_t trace_write (const char* buf __attribute__((unused)),
	     size_t nbyte __attribute__((unused)))
{
  return Error_messaging::write(buf, nbyte);
}


void Error_messaging::print_hal_status(HAL_StatusTypeDef status)
{
  switch (status)
  {
	case HAL_OK:
	  write( (const char*) "HAL_OK", 6);
	  break;
	case HAL_ERROR:
	  write( (const char*) "HAL_ERROR", 9);
	  break;
	case HAL_BUSY:
	  write( (const char*) "HAL_BUSY", 8);
	  break;
	case HAL_TIMEOUT:
	  write( (const char*) "HAL_TIMEOUT", 11);
	  break;


  }
}

void Error_messaging::input_loop(void)
{
  char byte;
  uint8_t i;

  while(socket->get_rx_ringbuffer()->Count() > 0)
	{
	  byte = socket->get_rx_ringbuffer()->Read();

	  if((uint8_t) byte == 13)
		{

		  // TODO process input
		  Error_messaging::write(input_buff, cursor);
		  Error_messaging::write("\n");

		  cursor = 0;
		  for (i=0; i<INPUT_BUFF_LEN; i++)
			input_buff[i] = ' ';
		}
	  if ((uint8_t) byte == 8) // backspace
		{
		  if (cursor != 0)
			  cursor --;
		  continue;
		}

	  input_buff[cursor] = byte;

	  cursor++;
	  if (cursor >= INPUT_BUFF_LEN)
		cursor = INPUT_BUFF_LEN;
	}



}

// workaround for sprintf TODO remove
/*caddr_t _sbrk(int incr)
{
  extern char _ebss; // Defined by the linker
  static char *heap_end;
  char *prev_heap_end;

  if (heap_end == 0)
  {
    heap_end = &_ebss;
  }

  prev_heap_end = heap_end;
  char * stack = (char*) __get_MSP();

  if (heap_end + incr > stack)
  {
    errno = ENOMEM;
    return (caddr_t) -1;
  }

  heap_end += incr;
  return (caddr_t) prev_heap_end;
}*/

#endif  /* TRACE */
