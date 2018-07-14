/*
 * Error_messaging.h
 *
 *  Created on: 19.03.2017
 *      Author: harald
 */

#ifndef SYSTEM_nextion_H_
#define SYSTEM_nextion_H_

#include "stm32f1xx_hal.h"
#include "Sockets/uart_socket.h"
#include "Devices/Nextion/ITEADLIB_Arduino_Nextion/Nextion.h"


class nextion
{
public:
  nextion(UART_HandleTypeDef *huart);
  virtual ~nextion () { };

  uart_socket* get_socket(void);
  size_t  write(const char* buf, size_t nbyte);
  size_t  readBytes(char* buffer, size_t nbyte);
  void 	  processRx(void);
  void 	  setTimeout(uint32_t timeout);
  uint8_t available(void);
  uint8_t read(void);
  void    set_nex_listen_list(NexTouch *nex_listen_list[]);

private:
  uart_socket*	display_comm;

};





#endif /* SYSTEM_nextion_H_ */
