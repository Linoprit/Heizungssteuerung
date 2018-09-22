/*
 * Debug_Comm_test.cpp
 *
 *  Created on: 08.03.2017
 *      Author: harald
 */

#include <Tests/Debug_Comm/Debug_Comm_test.h>
#include "../gnuarmeclipse/system/include/diag/Trace.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>



Debug_Comm_test::Debug_Comm_test ()
{
  //debug_comm = Error_messaging::get_debug_comm();
  sw_events  = Common::get_sw_events();
}


Debug_Comm_test::~Debug_Comm_test () { }

void Debug_Comm_test::loop()
{
  static  uint32_t 	old_counter = 0;
  uint32_t 	act_counter         = 0;

  int 		k = 34;
  int 		result;
  char 		s[30];

  while (1)
	{
	  sw_events->loop();

	  act_counter = Common::get_tick();
	  if (act_counter - old_counter  > 100)
		{
		  sprintf(s, (const char*) "Use of sprintf! %d \n", k);
		  result = trace_write((const char*) &s[0], 21);


		  // use of trace_write
		  result = trace_write ( (const char*) "trace_write test\n", 17);
		  result = trace_write((const char*) "Blah second line\n", 17);

		  // use macro
		  WRITE_TRACE (
				  (const char*) "Use WRITE_TRACE macro in your function.\n", 40);

		  // use of Trace.c -> trace_printf
		  result = trace_printf((const char*) "Hi from MCU %d, %d\n\n", k, result);
		  // scrambles the buffer...
		  //trace_printf((const char*) "Blah second line %d\n", k);


		  k++;
		  //HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);


		  old_counter = act_counter;
		}
	}
}



