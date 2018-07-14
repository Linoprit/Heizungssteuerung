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


SoftwareEvents*			Common::sw_events 		= new SoftwareEvents();
nextion*				Common::nex_disp		= NULL;
Heiz_display*			Common::heiz_disp		= NULL;


// Workaround undefined reference error
//const uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
//const uint8_t APBPrescTable[8]  = {0, 0, 0, 0, 1, 2, 3, 4};

// C interface
void common_init(void)	{ Common::init(); }


void Common::init()
{
#ifdef TRACE
  Error_messaging::init_debug_comm(get_huart1());
#endif /* TRACE */

  nex_disp  = new nextion(get_huart3());
  heiz_disp = new Heiz_display();





}


SoftwareEvents* Common::get_sw_events(void)
{
  return sw_events;
}

uint32_t Common::get_tick(void)
{
  return HAL_GetTick();
}



