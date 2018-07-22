/*
 * Comm_layer_socket.cpp
 *
 *  Created on: 25.05.2016
 *      Author: harald
 */


#include <Sockets/uart_socket.h>
#include "Instances/callbacks.h"
#include <string.h>
#include <System/Error_messaging.h>

#include "Instances/Common.h"

/**
 * We need osTreadId to be passed as function pointer, because sockets
 * are initialized before the threads.
 */
uart_socket::uart_socket (UART_HandleTypeDef *huart,
						  osThreadId* task_to_notify )
{
  this->huart 	= huart;
  rx_ringbuffer = new simpleRingbuffer(simpleRingbuffer_len);

  uart_rx_buffer 	= new uint8_t[UART_RX_BUFF_LEN];
  memset(uart_rx_buffer, RX_PAD_CHAR, UART_RX_BUFF_LEN);

  uart_tx_buffer 	= new uint8_t[UART_TX_BUFF_LEN];
  memset(uart_tx_buffer, RX_PAD_CHAR, UART_TX_BUFF_LEN);

  __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE); // @suppress("Field cannot be resolved")

  // same as HAL_UART_RxCpltCallback
  uart_callback_add(huart, (ISR_callback*) this);
  HAL_UART_Receive_DMA(this->huart, uart_rx_buffer, UART_RX_BUFF_LEN);

  this->task_to_notify = task_to_notify;
}

bool uart_socket::send_byte(uint8_t byte)
{
  return send_many_bytes(&byte, 1);
}

bool uart_socket::send_many_bytes(
	uint8_t *ptr_to_data, uint8_t len, uint16_t timeout)
{
  uint16_t count = 0;
  HAL_UART_StateTypeDef uart_status = HAL_UART_GetState(huart);

  if (len > UART_TX_BUFF_LEN)
	return ERROR;


  while ( ( (uart_status != HAL_UART_STATE_READY) &&
	  (uart_status != HAL_UART_STATE_BUSY_RX) )  )
	{
	  if(count >= timeout)
		{
		  return ERROR;
		}

	  osDelay(1);
	  count++;
	  uart_status = HAL_UART_GetState(huart);
	}


  // copy dynamic content to static buffer
  for (uint8_t i=0; i < len; i++)
	{
	  uart_tx_buffer[i] = *ptr_to_data;
	  ptr_to_data++;
	}

  HAL_UART_Transmit_DMA(huart, uart_tx_buffer, len);
  return SUCCESS;
}

bool uart_socket::is_TX_pending(void)
{
  HAL_UART_StateTypeDef usart_status = HAL_UART_GetState(huart);

  return
	  ( (usart_status == HAL_UART_STATE_BUSY_TX) ||
		  (usart_status == HAL_UART_STATE_BUSY_TX_RX) );
}

simpleRingbuffer* uart_socket::get_rx_ringbuffer()
{
  return rx_ringbuffer;
}

/*
 * This function must be called either if uart-line is idle or
 * if dma-rx is complete (rx-buffer is full)
 */
void uart_socket::ISR_callback_fcn ()
{
  uint8_t i=0;
  uint16_t rxXferCount = 0;
  if(huart->hdmarx != NULL)
	{
	  DMA_HandleTypeDef *hdma = huart->hdmarx;

	  /* Determine how many items of data have been received */
	  rxXferCount = huart->RxXferSize - __HAL_DMA_GET_COUNTER(hdma); // @suppress("Field cannot be resolved")
	  HAL_DMA_Abort(huart->hdmarx);
	  huart->RxXferCount = 0;

	  // Check if a transmit process is ongoing or not
	  //FIXME has no effect!!
	  HAL_UART_StateTypeDef usart_status = HAL_UART_GetState(huart);
	  if(usart_status == HAL_UART_STATE_BUSY_TX_RX)
		{
		  //usart_status = HAL_UART_STATE_BUSY_TX;
		  huart->gState = HAL_UART_STATE_BUSY_TX;
		  huart->RxState = HAL_UART_STATE_READY;
		}
	  else
		{
		  //usart_status = HAL_UART_STATE_READY;
		  huart->gState = HAL_UART_STATE_READY;
		  huart->RxState = HAL_UART_STATE_READY;
		}
	}

  // copy uart-buffer to ringbuffer
  for (i = 0; i < rxXferCount; i++)
	{
	  /*if (uart_rx_buffer[i] == RX_PAD_CHAR)
		{
		  continue;
		}*/
	  rx_ringbuffer->Write(uart_rx_buffer[i]);
	}

  // we need rx-buffer filled with pad-chars
  memset(uart_rx_buffer, RX_PAD_CHAR, UART_RX_BUFF_LEN);

  HAL_UART_Receive_DMA(this->huart, uart_rx_buffer, UART_RX_BUFF_LEN);
  // notify the controlling task, that data has arrived
  osSignalSet (*task_to_notify, 0);
}


UART_HandleTypeDef*	uart_socket::get_handle(void)
{
  return huart;
}



