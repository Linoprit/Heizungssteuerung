/*
 * Debug_Comm_test.h
 *
 *  Created on: 08.03.2017
 *      Author: harald
 */

#ifndef TESTS_DEBUG_COMM_DEBUG_COMM_TEST_H_
#define TESTS_DEBUG_COMM_DEBUG_COMM_TEST_H_


#include "Instances/Common.h"
#include "System/Error_messaging.h"


class Debug_Comm_test
{
public:
  Debug_Comm_test ();
  virtual ~Debug_Comm_test ();

  void loop();


private:
  Comm_Layer_socket*	debug_comm = NULL;
  SoftwareEvents* 		sw_events  = NULL;

};

#endif /* TESTS_DEBUG_COMM_DEBUG_COMM_TEST_H_ */
