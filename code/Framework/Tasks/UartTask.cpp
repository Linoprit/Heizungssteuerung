/*
 * UartTask.cpp
 *
 *  Created on: 14.07.2018
 *      Author: harald
 */

#include <Tasks/UartTask.h>
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"


// C interface
void StartUartTsk(void const * argument)
{
  UartTask::StartUartTsk(argument);
}



UartTask::UartTask ()
{

}


void UartTask::StartUartTsk(void const * argument)
{
  UNUSED(argument);

   for(;;)
 	{
 	  osDelay(400);

 	  HAL_GPIO_TogglePin(LED_04_GPIO_Port, LED_04_Pin);

 	 //Error_messaging::write("Message from task\n", 18);
 	}
}
