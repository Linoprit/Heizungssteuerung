/*
 * ThreadBasis.h
 *
 *  Created on: 13.05.2019
 *      Author: harald
 */

#ifndef THREADBASIS_H_
#define THREADBASIS_H_

#include <stdint.h>
#include <libraries/SimpleQueue.h>
#include "../StateMachineInterface.h"


#define ONE_TICK 1


class StateMachineWinter: public StateMachineInterface {
public:

	StateMachineWinter(void);
	StateMachineWinter(SimpleQueue<int8_t>* threadQueue_in, int8_t taksID);
	virtual ~StateMachineWinter() { };

	void 	   cycle(void);

	void set_tpauseMax(uint32_t value)  {
		_t_pauseMax  = value;
		_t_pause     = value;
	}; // ms
	void set_tactiveMax(uint32_t value) {
		_t_activeMax = value;
		_t_active    = value;
	}; // ms
	uint32_t get_tpauseMax(void)  { return _t_pauseMax; }; // ms
	uint32_t get_tactiveMax(void) { return _t_activeMax; }; // ms
	uint32_t get_tactive (void) { return _t_active; };
	uint32_t get_tpause  (void) { return _t_pause;  };

	void initialize(SimpleQueue<int8_t>* threadQueue_in, int8_t taksID);

private:
	uint32_t _my_time;   // [ms]
	uint32_t _t_active;  // [ms]
	uint32_t _t_pause;   // [ms]
	uint32_t _t_activeMax; // [ms]
	uint32_t _t_pauseMax;  // [ms]

	void initialize(void); // called by constructor

	void st_active(void);
	void st_pause(void);
	void st_ready(void);
	void st_init(void);

	void tr_active_pause(void);
	void tr_active_ready(void);
	void tr_pause_ready(void);
	void tr_ready_active(void);

	uint32_t time_diff(void);
};

#endif /* THREADBASIS_H_ */
