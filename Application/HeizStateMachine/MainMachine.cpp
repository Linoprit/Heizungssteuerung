/*
 * MainMachine.cpp
 *
 *  Created on: 07.07.2019
 *      Author: harald
 */

#include <HeizStateMachine/MainMachine.h>

MainMachine::MainMachine(RtcSocket* rtcSocket) {
	_isNightMode      = false;
	_state            = init;
	_manScheduler     = new SchedulerManual(NR_OF_INSTANCES);
	_summerScheduler  = new SchedulerSummer(rtcSocket, NR_OF_INSTANCES);
	_winterScheduler  = new SchedulerWinter(NR_OF_INSTANCES);

	for (uint8_t i=0; i < NR_OF_INSTANCES; i++) {
		_pumps[i] = false;
	}
}

void MainMachine::cycle(void){

	if (get_state() == init)
		st_init();
	else if (get_state() == summer)
		st_summer();
	else if (get_state() == winter)
		st_winter();
	else if (get_state() == manual)
		st_manual();
}

void MainMachine::st_init(void){

}

void MainMachine::st_summer(void) {
	_summerScheduler->cycle();

	for (uint8_t i=0; i < NR_OF_INSTANCES; i++) {
		// Night Mode is not respected
		if ((_summerScheduler->get_state(i) == StateMachineInterface::active) )
			_pumps[i] = true;
		else
			_pumps[i] = false;
	}
}

void MainMachine::st_winter(void) {

	if (_isNightMode == false) {
		_winterScheduler->cycle();
	}

	for (uint8_t i=0; i < NR_OF_INSTANCES; i++) {
		if ((_winterScheduler->get_state(i) == StateMachineInterface::active) &&
				(_isNightMode == false) )
			_pumps[i] = true;
		else
			_pumps[i] = false;
	}
}

void MainMachine::st_manual(void) {
	for (uint8_t i=0; i < NR_OF_INSTANCES; i++) {
		if (_manScheduler->get_state(i) == StateMachineInterface::active)
			_pumps[i] = true;
		else
			_pumps[i] = false;
	}
}

MainMachine::OpTimesType MainMachine::get_opTimes(int8_t id) {
	OpTimesType opTimes;

	opTimes.tactiveMax_winter = get_tactiveMax_winter(id);
	opTimes.tpauseMax_winter  = get_tpauseMax_winter(id);
	opTimes.tactive_winter    = get_tactive_winter(id);
	opTimes.tpause_winter     = get_tpause_winter(id);
	opTimes.tactive_summer    = get_tactive_summer(id);
	opTimes.tpause_summer     = get_tpause_summer(id);
	opTimes.pumpActive        = isPumpActive(id);

	return opTimes;
}

MainMachine::OpStatesStruct MainMachine::get_opStates(void){
	OpStatesStruct opStates;

	opStates.nightMode 			= isNightMode();
	opStates.MainMachineState 	= get_state();

	return opStates;
}


