/*
 * Second_timer.cpp
 *
 *  Created on: 18.12.2016
 *      Author: harald
 */

#include <Tests/Sw_Timer/Second_timer.h>
#include "stm32f3xx_hal.h"

Second_timer::Second_timer ()
{
  timer_2 = new SoftwareTimer(2, this, SoftwareEvents::Tic_100ms);

}

Second_timer::~Second_timer ()
{
  // TODO Auto-generated destructor stub
}

void Second_timer::timer_hit_callback(void)
{
  HAL_GPIO_TogglePin(LD9_GPIO_Port, LD8_Pin);
}
