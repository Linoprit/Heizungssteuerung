/*
 * MainMachine.h
 *
 *  Created on: 07.07.2019
 *      Author: harald
 */

#ifndef HEIZSTATEMACHINE_MAINMACHINE_H_
#define HEIZSTATEMACHINE_MAINMACHINE_H_

#include <HeizStateMachine/StateManual/SchedulerManual.h>
#include <HeizStateMachine/StateSummer/SchedulerSummer.h>
#include <HeizStateMachine/StateWinter/SchedulerWinter.h>
#include <HeizStateMachine/StateMachineInterface.h>

#define NR_OF_INSTANCES 2

using namespace SummerMachine;

class MainMachine {
public:
	enum enm_states {init, summer, winter, manual};

	// Times in [ms]
	struct OpTimesType {
		uint32_t tactiveMax_winter;
		uint32_t tpauseMax_winter;
		uint32_t tactive_winter;
		uint32_t tpause_winter;
		StateMachineInterface::enm_states
			state;
		uint32_t tactive_summer;
		uint32_t tpause_summer;
		bool     isPumpActive;
	} __attribute__ ((packed)) ;

	struct OpStatesStruct {
	bool nightMode;
	MainMachine::enm_states MainMachineState;
	}__attribute__ ((packed));


	MainMachine(RtcSocket* rtcSocket);
	virtual ~MainMachine() {};

	void cycle(void);
	void set_state(enm_states state) { _state       = state; 	};
	void set_nightMode(bool isNight) { _isNightMode = isNight;  };
	bool isNightMode(void)         { return _isNightMode;     };
	MainMachine::enm_states get_state(void) { return _state;	};
	bool isPumpActive(uint8_t pumpID) { return _pumps[pumpID];  };

	OpTimesType get_opTimes(int8_t id);
	OpStatesStruct get_opStates(void);

	// interfacing to the underlying machines
	void switch_on_manual(int8_t pumpID) { _manScheduler->switch_on(pumpID);   };
	void switch_off_manual(int8_t pumpID){ _manScheduler->switch_off(pumpID);  };

	void set_tactiveMax_winter (int8_t pumpID, uint32_t time_ms) {
		_winterScheduler->set_tactiveMax(pumpID, time_ms);  };
	void set_tpauseMax_winter (int8_t pumpID, uint32_t time_ms) {
		_winterScheduler->set_tpauseMax(pumpID, time_ms);   };
	uint32_t get_tactiveMax_winter(int8_t pumpID) {
		return _winterScheduler->get_tactiveMax(pumpID); };
	uint32_t get_tpauseMax_winter(int8_t pumpID) {
		return _winterScheduler->get_tpauseMax(pumpID);  };
	uint32_t get_tactive_winter(int8_t pumpID) { return _winterScheduler->get_tactive(pumpID); };
	uint32_t get_tpause_winter(int8_t pumpID)  { return _winterScheduler->get_tpause(pumpID);  };
	StateMachineInterface::enm_states get_state_winter(int8_t pumpID) {
		return _winterScheduler->get_state(pumpID); };

	uint32_t get_tactive_summer(int8_t pumpID) { return _summerScheduler->get_tactive(pumpID); };
	uint32_t get_tpause_summer(int8_t pumpID)  { return _summerScheduler->get_tpause(pumpID);  };
	StateMachineInterface::enm_states get_state_summer(int8_t pumpID) {
		return _summerScheduler->get_state(pumpID); };


private:
	void st_init(void);
	void st_summer(void);
	void st_winter(void);
	void st_manual(void);

	bool			 _pumps[NR_OF_INSTANCES];
	bool             _isNightMode;
	enm_states       _state;
	SchedulerManual* _manScheduler;
	SchedulerSummer* _summerScheduler;
	SchedulerWinter* _winterScheduler;
};

#endif /* HEIZSTATEMACHINE_MAINMACHINE_H_ */
