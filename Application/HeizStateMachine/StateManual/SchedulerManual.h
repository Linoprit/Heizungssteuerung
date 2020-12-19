/*
 * ManualMachineScheduler.h
 *
 *  Created on: 07.07.2019
 *      Author: harald
 */

#ifndef HEIZSTATEMACHINE_MANUALMACHINE_MANUALMACHINESCHEDULER_H_
#define HEIZSTATEMACHINE_MANUALMACHINE_MANUALMACHINESCHEDULER_H_

#include <HeizStateMachine/StateManual/StateMachineManual.h>
#include <HeizStateMachine/StateMachineInterface.h>
#include <HeizStateMachine/StateMachineList.h>


class SchedulerManual {
public:
	SchedulerManual(uint8_t threadCount): _machineList(threadCount) { };
	virtual ~SchedulerManual() { };

	void switch_on(int8_t threadNo) {
		StateMachineManual* manualMachineBasis = _machineList.get(threadNo);
		manualMachineBasis->switch_on();
	};

	void switch_off(int8_t threadNo) {
		StateMachineManual* manualMachineBasis = _machineList.get(threadNo);
		manualMachineBasis->switch_off();
	};

	StateMachineInterface::enm_states get_state(int8_t threadNo) {
		StateMachineManual* manualMachineBasis = _machineList.get(threadNo);
		return manualMachineBasis->get_state();
	}

	uint8_t  get_threadListlen(void) { return _machineList.len(); };

private:
	StateMachineList::MachineList<StateMachineManual> _machineList;

};

#endif /* HEIZSTATEMACHINE_MANUALMACHINE_MANUALMACHINESCHEDULER_H_ */
