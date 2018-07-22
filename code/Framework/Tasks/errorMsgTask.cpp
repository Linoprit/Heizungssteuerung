/*
 * dspCallbackTask.cpp
 *
 *  Created on: 06.07.2018
 *      Author: harald
 */

#include <Devices/Nextion/Nextion_device.h>
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include <Instances/Common.h>
#include <System/Error_messaging.h>
#include <Tasks/errorMsgTask.h>


// C interface
void StartErrorMsgTsk(void const * argument)
{
  //errorMsgTask::StartErrorMsgTsk(argument);

  UNUSED(argument);

  for(;;)
	{
	  //osDelay(400);
	  osSignalWait (0, osWaitForever);

	  Error_messaging::write("errorTask\n");

	  HAL_GPIO_TogglePin(LED_04_GPIO_Port, LED_04_Pin);

	 //Error_messaging::write("Message from task\n", 18);
	}

}

errorMsgTask::errorMsgTask ()
{


}

// TOOD doesn't need to be a class
void errorMsgTask::StartErrorMsgTsk(void const * argument)
{
  UNUSED(argument);
}
