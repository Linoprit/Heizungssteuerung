/*
 * controlTask.cpp
 *
 *  Created on: 06.07.2018
 *      Author: harald
 */

#include <Tasks/displayTask.h>
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "main.h"
#include <Instances/Common.h>

#include <TestWString.h>


#define DO_PRINT_DSPLTSK


// C interface
void StartDisplayTsk(void const * argument)
{
	UNUSED(argument);

	Common::init();



	/*uint8_t buff[9] ;
	//HAL_StatusTypeDef result;
	for (uint8_t i=0; i < 9; i++)
		buff[i] = '\0';*/





	Heiz_display* hdsp = Common::get_heizDisplay();

	for(;;)
	{
		// wait for task-notification
		osSignalWait(0x1, 1000);//osWaitForever);

		hdsp->loop();





		//HAL_GPIO_TogglePin(LED_03_GPIO_Port, LED_03_Pin);
		//HAL_GPIO_TogglePin(LED_04_GPIO_Port, LED_04_Pin);

#ifdef DO_PRINT_DSPLTSK

		/*SimpleQueue<uint8_t>* rb = Common::get_displaySocket()->get_rx_ringbuffer();

		if (!rb->isEmpty()) {
			for (uint8_t i=0; i < 8; i++) {
				if (rb->isEmpty())
					break;
				buff[i] = rb->dequeue();
			}
		}

		tx_printf("received: %s, %i, %i\n", buff,
				Common::get_displaySocket()->get_rx_ringbuffer()->size(),
				get_huart3()->ErrorCode);

		for (uint8_t i=0; i<9; i++)
			buff[i] = '\0';*/

		//tx_printf("Dspl\n");

#endif


		HAL_GPIO_TogglePin(LED_02_GPIO_Port, LED_02_Pin);
	}
}

