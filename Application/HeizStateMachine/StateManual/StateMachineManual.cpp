/*
 * ManualMachine.cpp
 *
 *  Created on: 07.07.2019
 *      Author: harald
 */

#include <HeizStateMachine/StateManual/StateMachineManual.h>

StateMachineManual::StateMachineManual() {
	_act_state = pause;
}

void StateMachineManual::cycle() {

}

void StateMachineManual::switch_on(void) {
	switch_state(active);
}

void StateMachineManual::switch_off(void) {
	switch_state(pause);
}
