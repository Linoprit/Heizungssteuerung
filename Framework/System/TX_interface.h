/*
 * Comm_socket_TX_only.h
 *
 *  Created on: 29.03.2016
 *      Author: harald
 */

#ifndef DRIVERS_COMM_SOCKET_TX_only_H_
#define DRIVERS_COMM_SOCKET_TX_only_H_

#include "libraries/simpleRingbuffer.h"


class TX_interface
{
public:
  TX_interface() {};

  virtual ~TX_interface() {};

  virtual bool send_byte(uint8_t byte) 								= 0;
  virtual bool send_many_bytes(uint8_t *ptr_to_data, uint8_t len) 	= 0;
  virtual bool is_TX_pending(void) 									= 0;
};

#endif /* DRIVERS_COMM_SOCKET_TX_only_H_ */
