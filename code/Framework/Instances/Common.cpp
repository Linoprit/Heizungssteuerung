/*
 * Initclasses.cpp
 *
 *  Created on: 06.05.2016
 *      Author: harald
 */

#include <System/Error_messaging.h>
#include "Common.h"
#include "stm32f1xx_hal.h"
#include "main.h"
#include "Instances/callbacks.h"
#include "cmsis_os.h"



SoftwareEvents*			Common::sw_events 		= new SoftwareEvents();
uart_socket*			Common::display_comm	= NULL;
uart_socket*			Common::errorMsg_comm	= NULL;
//errorMsgTask*	  		Common::errorMsg_tsk	= NULL;
nextion*				Common::nex_disp		= NULL;
Heiz_display*			Common::heiz_disp		= NULL;
Error_messaging*		Common::error_msg		= NULL;


// Workaround undefined reference error
//const uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
//const uint8_t APBPrescTable[8]  = {0, 0, 0, 0, 1, 2, 3, 4};

// C interface
void common_init(void)	{ Common::init(); }


void Common::init()
{
#ifdef TRACE
  errorMsg_comm = new uart_socket(get_huart1(), get_errorMsgTask());
  error_msg		= new Error_messaging(errorMsg_comm);
  //errorMsg_tsk  = new errorMsgTask();
#endif /* TRACE */


  display_comm = new uart_socket(get_huart3(), get_displayTask() );
  nex_disp     = new nextion(display_comm);
  heiz_disp    = new Heiz_display();

  // Workaround...
  //osSignalSet (*get_displayTask(), 0);


}


SoftwareEvents* Common::get_sw_events(void)
{
  return sw_events;
}

uint32_t Common::get_tick(void)
{
  return HAL_GetTick();
}



