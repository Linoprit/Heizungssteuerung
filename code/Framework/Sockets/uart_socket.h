/*

 * Comm_layer_socket.h
 *
 *  Created on: 25.05.2016
 *      Author: harald
 */

#ifndef MACHINE_COMM_LAYER_COMM_LAYER_SOCKET_H_
#define MACHINE_COMM_LAYER_COMM_LAYER_SOCKET_H_

#include <stdint.h>
#include <System/RX_TX_interface.h>
#include "libraries/simpleRingbuffer.h"
#include "Instances/callbacks.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_uart.h"


#define UART_TX_BUFF_LEN	50
#define UART_RX_BUFF_LEN 	50
#define RX_PAD_CHAR			 4   // 4 = EOT



class uart_socket:
	Rx_Tx_interface,
	ISR_callback //,
	//SoftwareTimer_base
{
public:
	  uart_socket (UART_HandleTypeDef *huart);
	  virtual
	  ~uart_socket ();

	  bool 					send_byte(uint8_t byte);
	  bool 					send_many_bytes(uint8_t *ptr_to_data, uint8_t len);
	  bool 					is_TX_pending(void);
	  simpleRingbuffer* 	get_rx_ringbuffer(void);
	  UART_HandleTypeDef*	get_handle(void);


	  // callback usart receive complete / idle line
	  void ISR_callback_fcn (void);

	  // timer, flush rx-buffer if expired
	  void timer_hit_callback(void);

	  uint8_t 				*uart_rx_buffer;


private:
	  //SoftwareTimer 		*timer;

	  uint8_t 				*uart_tx_buffer;
	  UART_HandleTypeDef 	*huart;
	  simpleRingbuffer 		*rx_ringbuffer;


	  // we use Timer event Tic_1ms!!
	  // set TimerDuration to 200ms for testing 0with console input
	  const uint8_t TimerDuration        = 1;   // 5 = 5ms
	  const uint8_t simpleRingbuffer_len = 54;

};

#endif /* MACHINE_COMM_LAYER_COMM_LAYER_SOCKET_H_ */
