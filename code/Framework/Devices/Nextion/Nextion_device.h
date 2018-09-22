/*
 * Error_messaging.h
 *
 *  Created on: 19.03.2017
 *      Author: harald
 */

#ifndef SYSTEM_nextion_H_
#define SYSTEM_nextion_H_

#include "stm32f1xx_hal.h"
#include <Sockets/uart_socket.h>
#include "Devices/Nextion/ITEADLIB_Nextion/Nextion.h"

/**
 * Stellt die Verbindung zum UART-Socket her und stellt
 * Service-Funktionen zur Verfügung.
 * Wird von NexHardware-Funktionen aufgerufen.
 */
class nextion
{
public:
  nextion(uart_socket* socket);
  virtual ~nextion () { };

  size_t  write(const char* buf, size_t nbyte);
  size_t  readBytes(char* buffer, size_t nbyte);
  uint8_t read(void);
  bool 	  wait_for_data(uint8_t expected_amount);
  uint8_t available(void);

  inline void setTimeout(uint32_t timeout)
  { this->timeout = timeout;  };

  //void set_nex_listen_list(NexTouch *nex_listen_list[]);

private:
  uart_socket*	socket;
  uint16_t 		timeout = 100;

};



#endif /* SYSTEM_nextion_H_ */
