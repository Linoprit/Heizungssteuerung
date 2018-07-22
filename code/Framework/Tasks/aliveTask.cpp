/*
 * aliveTask.cpp
 *
 *  Created on: 21.07.2018
 *      Author: harald
 */

#include <Tasks/aliveTask.h>
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include <Instances/Common.h>
#include <Tasks/controlTask.h>


// C interface
void StartAliveTsk(void const * argument)
{
  alive_Task::StartAliveTsk(argument);
}




alive_Task::alive_Task ()
{
  // TODO Auto-generated constructor stub

}

alive_Task::~alive_Task ()
{
  // TODO Auto-generated destructor stub
}

void alive_Task::StartAliveTsk(void const * argument)
{
  UNUSED(argument);

  for(;;)
	{

	  HAL_GPIO_TogglePin(LED_03_GPIO_Port, LED_03_Pin);
	  osDelay(200);


	}
}
