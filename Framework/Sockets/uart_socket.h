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
#include <libraries/SimpleQueue.h>
#include <Instances/callbacks.h>
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_uart.h"
#include "cmsis_os.h"


#define UART_TX_BUFF_LEN	50
#define UART_RX_BUFF_LEN 	150
#define RX_PAD_CHAR			 4   // 4 = EOT
#define RX_QUEUE_LEN		54


class uart_socket:
	Rx_Tx_interface,
	ISR_callback
{
public:
	  uart_socket (UART_HandleTypeDef *huart,
				   osThreadId* task_to_notify );

	  virtual ~uart_socket () {};

	  bool 	send_byte(uint8_t byte);
	  bool 	send_many_bytes(
		  uint8_t *ptr_to_data, uint8_t len, uint16_t timeout=10);
	  bool 	is_TX_pending(void);

	  SimpleQueue<uint8_t>*	get_rx_ringbuffer(void);

	  UART_HandleTypeDef*	get_handle(void);


	  // callback usart receive complete / idle line
	  void ISR_callback_fcn (void);

	  uint8_t 				*uart_rx_buffer;


private:
	  uint8_t 				*uart_tx_buffer;
	  UART_HandleTypeDef 	*huart;
	  osThreadId 			*task_to_notify;
	  SimpleQueue<uint8_t>  *rx_ringbuffer;
};

#endif /* MACHINE_COMM_LAYER_COMM_LAYER_SOCKET_H_ */
