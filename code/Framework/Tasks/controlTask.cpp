/*
 * controlTask.cpp
 *
 *  Created on: 06.07.2018
 *      Author: harald
 */

#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include <Instances/Common.h>
#include <Tasks/controlTask.h>
#include <Devices/Realtime_clock/rtc_device.h>


// C interface
void StartControlTsk(void const * argument)
{
  controlTask::StartControlTsk(argument);
}

controlTask::controlTask ()
{


}

void controlTask::StartControlTsk(void const * argument)
{
  UNUSED(argument);

  for(;;)
	{

	  Common::machine->loop();

	  HAL_GPIO_TogglePin(LED_01_GPIO_Port, LED_01_Pin);
	  osDelay(1000);


	}
}