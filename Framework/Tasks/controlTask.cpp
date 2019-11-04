/*
 * controlTask.cpp
 *
 *  Created on: 06.07.2018
 *      Author: harald
 */

#include <Tasks/controlTask.h>
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "main.h"
#include <Instances/Common.h>
#include <System/uart_printf.h>
#include <HeizStateMachine/MainMachine.h>


//#define DO_PRINT_CTRLTSK

// C interface
void StartControlTsk(void const * argument)
{
	UNUSED(argument);

	Common::init();

	for(;;)
	{
		MainMachine* mMach = Common::get_mainMachine();

		if (mMach != NULL) {
			mMach->cycle();

			if (HAL_GPIO_ReadPin(DayNight_GPIO_Port, DayNight_Pin) == GPIO_PIN_SET)
				mMach->set_nightMode(true);
			else
				mMach->set_nightMode(false);

			if (mMach->isPumpActive(0))
				HAL_GPIO_WritePin(Pump_1_GPIO_Port, Pump_1_Pin, GPIO_PIN_SET);
			else
				HAL_GPIO_WritePin(Pump_1_GPIO_Port, Pump_1_Pin, GPIO_PIN_RESET);

			if (mMach->isPumpActive(1))
				HAL_GPIO_WritePin(Pump_2_GPIO_Port, Pump_2_Pin, GPIO_PIN_SET);
			else
				HAL_GPIO_WritePin(Pump_2_GPIO_Port, Pump_2_Pin, GPIO_PIN_RESET);




		}

#ifdef DO_PRINT_CTRLTSK
		if (mMach != NULL)
		{
			tx_printf("MainMachine state: %i, night: %i\n",
					mMach->get_state(), mMach->get_nightMode());

			tx_printf("pmp \t on \t state \t t_actv \t t_paus \t tactMax \t tpausMax \n");
			for (uint8_t i=0; i < 2; i++) {
				tx_printf("%i \t %i \t %i \t %6i \t %6i \t %6i \t %6i\n",
						i,
						mMach->isPumpActive(i),
						mMach->get_state_winter(i),
						mMach->get_tactive_winter(i),
						mMach->get_tpause_winter(i),
						mMach->get_tactiveMax_winter(i),
						mMach->get_tpauseMax_winter(i)
				);
			}
		}
		else
			tx_printf("mMach = NULL\n");
#endif


		// osSignalSet (*get_displayTask(), 0x1);
		//static uint32_t counter = 0;
		//osMessagePut(*get_uartQueue(), (uint32_t)counter++, 1000);



		HAL_GPIO_TogglePin(LED_01_GPIO_Port, LED_01_Pin);
		osDelay(1000);


	}
}

