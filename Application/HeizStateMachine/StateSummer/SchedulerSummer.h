/*
 * SummerMachineScheduler.h
 *
 *  Created on: 13.07.2019
 *      Author: harald
 */

#ifndef HEIZSTATEMACHINE_SUMMERMACHINE_SUMMERMACHINESCHEDULER_H_
#define HEIZSTATEMACHINE_SUMMERMACHINE_SUMMERMACHINESCHEDULER_H_

#include <Sockets/RtcSocket.h>
#include "../StateWinter/StateMachineWinter.h"
#include <HeizStateMachine/StateMachineInterface.h>
#include <HeizStateMachine/StateMachineList.h>


namespace SummerMachine {


class SchedulerSummer {
public:
	SchedulerSummer(RtcSocket* rtcSocket, uint8_t threadCount);

	virtual ~SchedulerSummer() {};
	void cycle(void);
	int8_t get_last_active_thread(void) { return _lastActiveThread; };
	int8_t get_active_thread(void);
	uint32_t get_tactiveMax (int8_t threadNo);
	uint32_t get_tpauseMax  (int8_t threadNo);
	uint32_t get_tactive (int8_t threadNo);
	uint32_t get_tpause  (int8_t threadNo);

	StateMachineInterface::enm_states get_state(int8_t threadNo) {
		StateMachineWinter* machineBasis = _machineList.get(threadNo);
		return machineBasis->get_state();
	}

	uint8_t  get_threadListlen(void) { return _machineList.len(); };

private:
	void set_triggerService(bool state);

	// we use the winterMachine
	StateMachineList::MachineList<StateMachineWinter> _machineList;
	RtcSocket* _rtcSocket;
	int8_t _lastActiveThread;
	bool   _triggerService;
	bool   _old_triggerService;

};

} /* namespace SummerMachine */

#endif /* HEIZSTATEMACHINE_SUMMERMACHINE_SUMMERMACHINESCHEDULER_H_ */
