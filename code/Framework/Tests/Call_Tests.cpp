/*
 * Call_Tests.cpp
 *
 *  Created on: 17.12.2016
 *      Author: harald
 */


#include "Call_Tests.h"
#ifdef DO_TESTS

//#include <Tests/UART_Socket/UART_socket_test.h>
//#include "Sw_Timer/Sw_Timer_Test.h"
#include "Comm_Layer/Comm_Layer_test.h"
#include "Rotary_Encoder/Rotary_Encoder_Tests.h"
#include "Char_LCD/Char_LCD_test.h"
#include "Debug_Comm/Debug_Comm_test.h"
#include "EEPROM_93C86/eeprom_93c86_test.h"


Call_Tests::Call_Tests ()
{
}

Call_Tests::~Call_Tests() {}

void Call_Tests::cycle()
{
  // Software-Timer and Events Tests
  //Sw_Timer_Test *sw_timer_test 	= new Sw_Timer_Test();
  //sw_timer_test->Do_Timer_Test();

  //UART_socket_test* uart_socket_test = new UART_socket_test();
  //uart_socket_test->loop();

  // Char LCD
  //Char_LCD_test* LCD_test = new Char_LCD_test();
  //LCD_test->loop();

  //Rotary_Encoder_Tests *encoder_test = new Rotary_Encoder_Tests();
  //encoder_test->loop();

  // Comm_Layer tests
  // Comm_Layer_test* comm_layer_test = new Comm_Layer_test();
  // comm_layer_test->loop();

  // Debug communication tests
  //Debug_Comm_test* debug_comm_test = new Debug_Comm_test();
  //debug_comm_test->loop();

  // Eeprom socket tests
  eeprom_93c86_test* eeprom_test = new eeprom_93c86_test();
  eeprom_test->loop();

  while(1) { }

}
#endif /* DO_TESTS */
