/*
 * SummerMachineScheduler.cpp
 *
 *  Created on: 13.07.2019
 *      Author: harald
 */

#include <HeizStateMachine/StateSummer/SchedulerSummer.h>


namespace SummerMachine {

#define T_ACTIVE    5 // minutes
#define START_DAY   RTC_WEEKDAY_WEDNESDAY
#define START_HOURS 1
#define START_MIN   0

const RTC_TimeTypeDef startTime = { START_HOURS,  START_MIN, 0};


SchedulerSummer::SchedulerSummer(
		RtcSocket* rtcSocket, uint8_t threadCount): _machineList(threadCount) {
	_rtcSocket          = rtcSocket;
	_lastActiveThread   = -1;
	_triggerService     = false;
	_old_triggerService = false;

	// set the standard durations
	for (uint8_t i=0; i < _machineList.len(); i++) {
		StateMachineWinter* machine = _machineList.get(i);
		machine->set_tactiveMax(MIN2MS(T_ACTIVE));
		// make sure no new queueing happens within a service cycle
		machine->set_tpauseMax(	MIN2MS(T_ACTIVE) * threadCount * 2 );
	}
};

void SchedulerSummer::cycle(void) {

	if( (_rtcSocket->get_time() == startTime)
			&& (_rtcSocket->get_date().WeekDay == START_DAY) ) {
		set_triggerService(true);
	}

	if (_triggerService) {
		bool oneTaskIsActive = false;
		for (uint8_t i=0; i < _machineList.len(); i++) {
			StateMachineWinter* machine = _machineList.get(i);
			machine->cycle();
			if(machine->get_state() == StateMachineWinter::active)
				oneTaskIsActive = true;
		}

		// we have tasks in the queue and no task is active
		if((!_machineList.get_queue()->isEmpty()) && (!oneTaskIsActive)) {
			// select next active
			int8_t ID = _machineList.get_queue()->front();
			if(ID == _lastActiveThread) { // requeue
				ID = _machineList.get_queue()->dequeue();
				_machineList.get_queue()->enqueue(ID);
			}

			ID = _machineList.get_queue()->dequeue();
			StateMachineWinter* machine = _machineList.get(ID);
			machine->set_go();
			_lastActiveThread = ID;
			machine->cycle();
		}

		// check again, if there is an active task
		for (uint8_t i=0; i < _machineList.len(); i++) {
			StateMachineWinter machine = *_machineList.get(i);

			if(machine.get_state() == StateMachineWinter::active)
				oneTaskIsActive = true;
		}

		// no waiting task and no active task, we are done with the service cycle
		if ((_machineList.get_queue()->isEmpty()) && (!oneTaskIsActive)) {
			set_triggerService(false);

			// we need one more cycle to leave the machines in a ordered state
			for (uint8_t i=0; i < _machineList.len(); i++) {
				StateMachineWinter* machine = _machineList.get(i);
				machine->cycle();
			}
		}
	}
}

void SchedulerSummer::set_triggerService(bool state) {
	_old_triggerService = _triggerService;
	_triggerService     = state;
}

int8_t SchedulerSummer::get_active_thread(void) {
	int8_t result = -1;

	for (uint8_t i=0; i < _machineList.len(); i++) {
		StateMachineWinter* thread = _machineList.get(i);

		if(thread->get_state() == StateMachineWinter::active)
		{
			result = thread->get_taskID();
		}
		//printf("thread %i=%i ", thread->get_taskID(), thread->get_state());
	}
	//printf("\n");
	return result;
}

uint32_t SchedulerSummer::get_tactiveMax (int8_t threadNo) {
	StateMachineWinter* thread = _machineList.get(threadNo);
	return thread->get_tactiveMax();
}

uint32_t SchedulerSummer::get_tpauseMax  (int8_t threadNo){
	StateMachineWinter* thread = _machineList.get(threadNo);
	return thread->get_tpauseMax();
}


uint32_t SchedulerSummer::get_tactive (int8_t threadNo){
	StateMachineWinter* thread = _machineList.get(threadNo);
	return thread->get_tactive();
}

uint32_t SchedulerSummer::get_tpause  (int8_t threadNo){
	StateMachineWinter* thread = _machineList.get(threadNo);
	return thread->get_tpause();
}


} /* namespace SummerMachine */
