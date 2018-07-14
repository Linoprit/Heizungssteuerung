/*
 * controlTask.cpp
 *
 *  Created on: 06.07.2018
 *      Author: harald
 */

#include <Tasks/displayTask.h>
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"

// C interface
void StartDisplayTsk(void const * argument)
{
  displayTask::StartDisplayTsk(argument);
}

displayTask::displayTask ()
{


}

void displayTask::StartDisplayTsk(void const * argument)
{
  UNUSED(argument);

  for(;;)
	{
	  osDelay(200);
	  HAL_GPIO_TogglePin(LED_02_GPIO_Port, LED_02_Pin);
	}
}
