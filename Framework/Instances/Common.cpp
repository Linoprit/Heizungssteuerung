/*
 * Initclasses.cpp
 *
 *  Created on: 06.05.2016
 *      Author: harald
 */

#include "Common.h"
#include "stm32f1xx_hal.h"
#include "main.h"
#include "../Framework/Instances/callbacks.h"
#include "../Framework/System/uart_printf.h"
#include "cmsis_os.h"



uart_socket*			Common::displaySocket	= NULL;
nextion*				Common::nexDevice		= NULL;
Heiz_display*			Common::heizDisplay		= NULL;
RtcSocket*				Common::rtcSocket		= NULL;
MainMachine*			Common::mainMachine		= NULL;
bool					Common::initIsDone 		= false;



// C interface
void common_init(void)	{ Common::init(); }

/**
 * Some of the constructors use a osDelay call. So we need a running os.
 * That's why we call this function from the Start* tasks.
 */
void Common::init()
{
	if (Common::initIsDone == true)
		return;

	initIsDone 		= true;

	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);

	rtcSocket		= new RtcSocket(get_rtc());
	mainMachine		= new MainMachine(rtcSocket);
	mainMachine->set_state(MainMachine::winter);

	displaySocket 	= new uart_socket(get_huart3(), get_displayTask() );

	//new uart_socket(get_huart1(), get_displayTask() );

	nexDevice     	= new nextion(displaySocket);
	heizDisplay    	= new Heiz_display();




}

bool Common::is_init_done(void)
{
	return initIsDone;
}

uint32_t Common::get_tick(void)
{
	return HAL_GetTick();
}



