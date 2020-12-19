/*
 * Comm_socket.h
 *
 *  Created on: 29.03.2016
 *      Author: harald
 */

#ifndef DRIVERS_COMM_SOCKET_H_
#define DRIVERS_COMM_SOCKET_H_

#include <libraries/SimpleQueue.h>


class Rx_Tx_interface
{
public:
  Rx_Tx_interface() {};

  virtual ~Rx_Tx_interface() {};

  virtual bool send_byte(uint8_t byte) 						= 0;
  virtual bool send_many_bytes(
	  uint8_t *ptr_to_data, uint8_t len, uint16_t timeout) 	= 0;
  virtual bool is_TX_pending(void) 							= 0;
  virtual SimpleQueue<uint8_t>* get_rx_ringbuffer() 		= 0;



};

#endif /* DRIVERS_COMM_SOCKET_H_ */
