/*
 * ManualMachine.h
 *
 *  Created on: 07.07.2019
 *      Author: harald
 */

#ifndef HEIZSTATEMACHINE_MANUALMACHINE_MANUALMACHINEBASIS_H_
#define HEIZSTATEMACHINE_MANUALMACHINE_MANUALMACHINEBASIS_H_

#include "../StateMachineInterface.h"
#include <libraries/SimpleQueue.h>


class StateMachineManual: public StateMachineInterface {
public:
	StateMachineManual();
	virtual ~StateMachineManual() {};

	void	cycle(void);
	void    initialize(SimpleQueue<int8_t>* threadQueue_in, int8_t taskID) { };

	void switch_on(void);
	void switch_off(void);
};

#endif /* HEIZSTATEMACHINE_MANUALMACHINE_MANUALMACHINEBASIS_H_ */
