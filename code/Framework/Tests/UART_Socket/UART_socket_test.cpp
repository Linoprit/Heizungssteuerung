/*
 * Comm_Layer_socket_test.cpp
 *
 *  Created on: 15.06.2016
 *      Author: harald
 */

#include "Instances/Common.h"
#include <Tests/UART_Socket/UART_socket_test.h>
#include "System/SoftwareEvents.h"
#include "stm32f3xx_hal.h"
#include "libraries/itoa.h"
#include "main.h"

/**
 *
 * 1.  Configure SoftwareEvents, PCD8544 and Keypad in Common
 * 2.  add following to Call_Tests::cycle():
 *      Comm_Layer_socket_test* comm_layer_socket_test =
 *	  		new Comm_Layer_socket_test();
 *	    comm_layer_socket_test->loop(); // never returns
 * 3.  Be sure, that no other instance of Comm_Layer_socket exists.
 * 	    (Have a look into Common.h and comment out init_comm_layer() )
 * 4.  In Comm_Layer_socket-constructor, comment out the timer initialization
 * 	    and replace it by following line:
 * 	  	 timer = new SoftwareTimer(20, this, SoftwareEvents::Tic_100ms);
 * 5.  Compile and flash.
 * 6.  Test: Open Minicom and connect to uart1 (use some FTDI serial
 * 	    to usb converter
 * 7.  Test: Type two or three chars in minicom, some seconds later,
 * 	    you should see them displayed, together with many diamond chars
 * 8.  Test: fast type ten chars into minicom, you should see them
 *      instantly displayed. Type really fast, so that the timer
 *      interrupt could not fire. You can use the '*' key on the
 *      keypad to clear the line.
 * 9.  On the keypad, press the '#' key to send a string to minicom.
 * 10. Press '0' to send a single char.
 *
 */

UART_socket_test::UART_socket_test ()
{
  socket = new Comm_Layer_socket( get_UART_1() );
}


void UART_socket_test::loop (void)
{
/*  SoftwareEvents* 	  sw_events 	= Common::get_sw_events();
  PCD8544_graphics*	  LCD		 	= Common::get_LCD_grfx();
  Keypad* 			  keypad 		= Common::get_keypad();



  char				last_key;
  Keypad::button_state_enum key_state 		= Keypad::bttn_released;
  Keypad::button_state_enum old_key_state 	= Keypad::bttn_released;

  const uint8_t ringbuffer_content_len 	= 17;
  char 			*ringbuffer_content 	=
	  (char*) malloc(ringbuffer_content_len * sizeof (char));
  uint8_t ringbuffer_content_counter	= 0;
  clearTextbuffer(ringbuffer_content, ringbuffer_content_len);

  const uint8_t textbuffer_len 	= 17;
  char 			*textbuffer 	=
	  (char*) malloc(textbuffer_len * sizeof (char));
  clearTextbuffer(textbuffer, textbuffer_len);

#define MESSAGE "This is a message from comm_layer_socket. \0"
  const char* transmit_message = MESSAGE;
  uint8_t	transmit_message_len = sizeof(MESSAGE);


  while (1)
 	{
 	  sw_events->loop();

 	  last_key 		= keypad->getLastChar();
 	  key_state 	= keypad->getActualKeyState();

 	  if( (old_key_state != key_state) &&
 		  (key_state == Keypad::bttn_pressed) )
 		{
 		  if (last_key == '*')
 			{
 			  clearTextbuffer(ringbuffer_content, ringbuffer_content_len);
 			  ringbuffer_content_counter = 0;
 			}

 		  if (last_key == '#')
 			{
 			  socket->send_many_bytes(
 				  (uint8_t*) transmit_message, transmit_message_len);
 			}

 		  if (last_key == '0')
 			{
 			  socket->send_byte('U');
 			}
 		}

 	  old_key_state = key_state;


 	  simpleRingbuffer* rx_ringbuffer = socket->get_rx_ringbuffer();

 	  // put ringbuffer content to LCD
 	  //clearTextbuffer(ringbuffer_content, ringbuffer_content_len);
 	  uint8_t count = rx_ringbuffer->Count();
 	  for (uint8_t i = 0; i < count; i++)
 		{
 		  ringbuffer_content[ringbuffer_content_counter] = rx_ringbuffer->Read();

 		  ringbuffer_content_counter++;
 		  if (ringbuffer_content_counter >= ringbuffer_content_len - 1)
 			ringbuffer_content_counter = 0;
 		}
 	  LCD->write_string(0, LCD->conv_line_2_y_pix(1), ringbuffer_content);

 	  // put uart-state to LCD
 	  clearTextbuffer(textbuffer, textbuffer_len);
 	  get_uart_state_str(socket->get_handle(), textbuffer);
 	  LCD->write_string(0, LCD->conv_line_2_y_pix(3), textbuffer);


 	  LCD->display();

 	}*/
}


void UART_socket_test::get_uart_state_str(
	UART_HandleTypeDef *uart, char *textbuffer)
{
  HAL_UART_StateTypeDef state = HAL_UART_GetState(uart);

 /* switch (state)
  {
	case HAL_UART_STATE_RESET:
	  strncpy(&textbuffer[0], (const char*) "RESET", 5);
	  break;

	case HAL_UART_STATE_READY:
	  strncpy(&textbuffer[0], (const char*) "READY", 5);
	  break;

	case HAL_UART_STATE_BUSY:
	  strncpy(&textbuffer[0], (const char*) "BUSY", 4);
	  break;

	case HAL_UART_STATE_BUSY_TX:
	  strncpy(&textbuffer[0], (const char*) "BUSY_TX", 7);
	  break;

	case HAL_UART_STATE_BUSY_RX:
	  strncpy(&textbuffer[0], (const char*) "BUSY_RX", 7);
	  break;

	case HAL_UART_STATE_BUSY_TX_RX:
	  strncpy(&textbuffer[0], (const char*) "BUSY_TX_RX", 10);
	  break;

	case HAL_UART_STATE_TIMEOUT:
	  strncpy(&textbuffer[0], (const char*) "TIMEOUT", 7);
	  break;

	case HAL_UART_STATE_ERROR:
	  strncpy(&textbuffer[0], (const char*) "ERROR", 5);
	  break;
  }*/
}

void UART_socket_test::clearTextbuffer(char *textbuffer, uint8_t len)
{
  for (uint8_t i=0; i < len; i++)
	textbuffer[i] = ' ';
  textbuffer[len-1] = '\0';
}
