/*
 * Comm_Layer_socket_test.h
 *
 *  Created on: 15.06.2016
 *      Author: harald
 */

#ifndef COMM_LAYER_SOCKET_TEST_H_
#define COMM_LAYER_SOCKET_TEST_H_

#include "Devices/Comm_Layer/Comm_Layer_socket.h"


class UART_socket_test
{
public:
  UART_socket_test();

  virtual
  ~UART_socket_test() {};

  void loop (void);

  void clearTextbuffer(char *textbuffer, uint8_t len);
  void get_uart_state_str(UART_HandleTypeDef *uart,
 								char *textbuffer);
  //void get_hal_state_str(HAL_StatusTypeDef state,
  //						   char *textbuffer);



private:
  Comm_Layer_socket* socket;

};

#endif /* COMM_LAYER_SOCKET_TEST_H_ */
