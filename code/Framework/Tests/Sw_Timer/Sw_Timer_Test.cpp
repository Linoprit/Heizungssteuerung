/*
 * Sw_Timer_Test.cpp
 *
 *  Created on: 15.05.2016
 *      Author: harald
 */

#include <Tests/Sw_Timer/Sw_Timer_Test.h>
#include "stm32f3xx_hal.h"
#include "Instances/Common.h"
#include "Second_timer.h"


Sw_Timer_Test::Sw_Timer_Test ()
{
  timer = NULL;
}

Sw_Timer_Test::~Sw_Timer_Test ()
{
}

void Sw_Timer_Test::Do_Timer_Test()
{
  SoftwareEvents* 	sw_events 	= Common::get_sw_events();


  Second_timer 		*second_timer = new Second_timer();
  timer = new SoftwareTimer(1, this, SoftwareEvents::Tic_100ms);


  while(1)
	{
	  // we must loop the sw-events here!
	  sw_events->loop();

	  // Some callback must call SoftwareEvents_checkTicks(tick);
	  // We have put all this into callbacks.c, located in cube
	  // directory.

	}
}


void Sw_Timer_Test::timer_hit_callback(void)
{
  HAL_GPIO_TogglePin(LD9_GPIO_Port, LD9_Pin);
}
