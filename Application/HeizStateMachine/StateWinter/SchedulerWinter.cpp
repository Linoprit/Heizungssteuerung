/*
 * HeizScheduler.cpp
 *
 *  Created on: 29.05.2019
 *      Author: harald
 */

#include "SchedulerWinter.h"
#include <HeizStateMachine/StateMachineList.h>

SchedulerWinter::SchedulerWinter(uint8_t threadCount) :
_machineList(threadCount)
{
	_lastActiveThread = -1;
}

void SchedulerWinter::cycle(void) {
	// cycle all tasks
	bool oneTaskIsActive = false;
	for (uint8_t i=0; i < _machineList.len(); i++) {
		StateMachineWinter* machine = _machineList.get(i);
		machine->cycle();
		if(machine->get_state() == StateMachineWinter::active)
			oneTaskIsActive = true;
	}

	if((!_machineList.get_queue()->isEmpty()) && (!oneTaskIsActive)) {
		// select next active
		int8_t ID = _machineList.get_queue()->front();
		if(ID == _lastActiveThread) { // requeue
			ID = _machineList.get_queue()->dequeue();
			_machineList.get_queue()->enqueue(ID);

			//printf("requeuing thread %i\n", ID);
		}

		ID = _machineList.get_queue()->dequeue();
		StateMachineWinter* machine = _machineList.get(ID);
		machine->set_go();
		_lastActiveThread = ID;
		machine->cycle();
	}
}

int8_t SchedulerWinter::get_active_thread(void)
{
	int8_t result = -1;

	for (uint8_t i=0; i < _machineList.len(); i++) {
		StateMachineWinter* machine = _machineList.get(i);

		if(machine->get_state() == StateMachineWinter::active)
		{
			result = machine->get_taskID();
		}
		//printf("thread %i=%i ", thread->get_taskID(), thread->get_state());
	}
	//printf("\n");
	return result;
}

void SchedulerWinter::set_tactiveMax (int8_t threadNo, uint32_t tactive){
	StateMachineWinter* thread = _machineList.get(threadNo);
	thread->set_tactiveMax(tactive);
}

void SchedulerWinter::set_tpauseMax  (int8_t threadNo, uint32_t tpause){
	StateMachineWinter* thread = _machineList.get(threadNo);
	thread->set_tpauseMax(tpause);
}

uint32_t SchedulerWinter::get_tactiveMax (int8_t threadNo) {
	StateMachineWinter* thread = _machineList.get(threadNo);
	return thread->get_tactiveMax();
}

uint32_t SchedulerWinter::get_tpauseMax  (int8_t threadNo){
	StateMachineWinter* thread = _machineList.get(threadNo);
	return thread->get_tpauseMax();
}


uint32_t SchedulerWinter::get_tactive (int8_t threadNo){
	StateMachineWinter* thread = _machineList.get(threadNo);
	return thread->get_tactive();
}

uint32_t SchedulerWinter::get_tpause  (int8_t threadNo){
	StateMachineWinter* thread = _machineList.get(threadNo);
	return thread->get_tpause();
}




