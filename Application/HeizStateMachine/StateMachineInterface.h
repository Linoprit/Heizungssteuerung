/*
 * StateMachineInterface.h
 *
 *  Created on: 07.07.2019
 *      Author: harald
 */

#ifndef HEIZSTATEMACHINE_STATEMACHINEINTERFACE_H_
#define HEIZSTATEMACHINE_STATEMACHINEINTERFACE_H_

#include <stdint.h>
#include <libraries/SimpleQueue.h>

#define MIN2MS(x) (x*60*1000)


class StateMachineInterface	{
public:
	enum enm_states {init, active, pause, ready};

	virtual ~StateMachineInterface(void) { };

	virtual void	cycle(void) 		= 0;
	enm_states 		get_state(void)    { return _act_state;	};
	void 	   		set_go(void)	   { _go = true; 		};
	void 	   		reset_go(void)	   { _go = false; 		};
	int8_t 			get_taskID(void)   { return _taskID; 	};

	virtual void	initialize(
			SimpleQueue<int8_t>* threadQueue_in, int8_t taskID)   = 0;

protected:
	enm_states _act_state;
	bool       _entry;
	bool       _go;
	uint8_t    _taskID;
	SimpleQueue<int8_t>* threadQueue;

	void  switch_state(enm_states state){
		_entry = true;
		_act_state = state;

		cycle();
	};

	bool  check_entry(void)	{
		if(_entry == true)	{
			_entry = false;
			return true;
		}
		return false;
	};
};


#endif /* HEIZSTATEMACHINE_STATEMACHINEINTERFACE_H_ */
