/*
 * Error_messaging.h
 *
 *  Created on: 19.03.2017
 *      Author: harald
 */

#ifndef TASKS_ERROR_MESSAGING_H_
#define TASKS_ERROR_MESSAGING_H_

#include "stm32f1xx_hal.h"
#include "Sockets/uart_socket.h"
#include "string.h"



#ifdef TRACE

#define INPUT_BUFF_LEN 20


class Error_messaging
{
public:
  Error_messaging (uart_socket* socket);
  virtual ~Error_messaging () {};

  uart_socket*   get_debug_comm(void);
  static size_t  write(const char* buf, size_t nbyte);
  static size_t  write(const char* buf);
  static void	 input_loop(void);

  void 	         print_hal_status(HAL_StatusTypeDef status);




private:
  static uart_socket*	socket;
  const static uint8_t 	timeout = 10;

  static char	 input_buff[INPUT_BUFF_LEN];
  static uint8_t cursor;

};

// C-interface
#ifdef __cplusplus
extern "C" {
#endif

//caddr_t _sbrk(int incr);

ssize_t trace_write (const char* buf __attribute__((unused)),
	     size_t nbyte __attribute__((unused)));

#ifdef __cplusplus
 }
#endif

#define WRITE_TRACE(msg, len)	Error_messaging::write(msg, len)
#else
#define WRITE_TRACE(msg, len)
#define WRITE_TRACE(msg)
#endif /* TRACE */



#endif /* TASKS_ERROR_MESSAGING_H_ */
