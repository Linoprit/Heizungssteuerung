/*
 * StateMachineList.h
 *
 *  Created on: 13.07.2019
 *      Author: harald
 */

#ifndef HEIZSTATEMACHINE_STATEMACHINELIST_H_
#define HEIZSTATEMACHINE_STATEMACHINELIST_H_

#include "StateMachineInterface.h"
#include <libraries/SimpleQueue.h>
#include <cstdlib>
#include <exception>
#include <System/Error_Handler.h>



namespace StateMachineList {

class MachineListOutOfBoundsExeption: public std::exception {
public:
	virtual const char* what() const throw() {
		return "Pos out of ThreadList bounds.";
	}
};



template <class T> class MachineList {
public:
	MachineList(uint8_t length) {
		_length 	 = length;
		_threadQueue = new SimpleQueue<int8_t>(_length);
		_threads  	 = new T[_length];

		for(uint8_t i=0; i < _length; i++) {
			_threads[i].initialize(_threadQueue, i);
		}
	};

	virtual ~MachineList() {
		delete[] _threads;
		delete _threadQueue;
	};

	uint8_t len(void) { return _length; };

	T*  get (uint8_t pos){
		if((pos < _length) && (pos >= 0))
			return &_threads[pos];
		else
		{
			error_handler(__FILE__, __LINE__ );
			return 0;
		}
	};

	SimpleQueue<int8_t>* get_queue(void) { return _threadQueue; };

private:
	SimpleQueue<int8_t>* _threadQueue;
	uint8_t 			 _length;
	T*					 _threads;

};

} /* namespace StateMachineList */

#endif /* HEIZSTATEMACHINE_STATEMACHINELIST_H_ */
