/*
 * dspCallbackTask.cpp
 *
 *  Created on: 06.07.2018
 *      Author: harald
 */

#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include <Devices/Nextion/Nextion_display.h>
#include <Instances/Common.h>
#include <Tasks/dspCallbackTask.h>
#include <System/Error_messaging.h>


// C interface
void StartDspCallbackTsk(void const * argument)
{
  dspCallbackTask::StartDspCallbackTsk(argument);
}

dspCallbackTask::dspCallbackTask ()
{


}

void dspCallbackTask::StartDspCallbackTsk(void const * argument)
{
  UNUSED(argument);

  for(;;)
	{
	  //osDelay(400);
	  osSignalWait (0, osWaitForever);
	  Common::nex_disp->processRx();
	  HAL_GPIO_TogglePin(LED_03_GPIO_Port, LED_03_Pin);

	 //Error_messaging::write("Message from task\n", 18);
	}
}
