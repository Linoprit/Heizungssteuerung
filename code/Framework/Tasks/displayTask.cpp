/*
 * controlTask.cpp
 *
 *  Created on: 06.07.2018
 *      Author: harald
 */

#include <Tasks/displayTask.h>
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include <Instances/Common.h>


// C interface
void StartDisplayTsk(void const * argument)
{
  displayTask::StartDisplayTsk(argument);
}

displayTask::displayTask ()
{


}

// TODO müsste keine cpp-Klasse mehr sein
void displayTask::StartDisplayTsk(void const * argument)
{
  UNUSED(argument);

  for(;;)
	{
	  // wait for task-notification
	  osSignalWait(0, osWaitForever);

	  if (Common::heiz_disp != NULL)
		Common::heiz_disp->loop();

	  HAL_GPIO_TogglePin(LED_02_GPIO_Port, LED_02_Pin);
	}
}
