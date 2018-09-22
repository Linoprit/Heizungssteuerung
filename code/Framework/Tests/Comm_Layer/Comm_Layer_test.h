/*
 * Comm_Layer_test.h
 *
 *  Created on: 22.12.2016
 *      Author: harald
 */

#ifndef TESTS_COMM_LAYER_COMM_LAYER_TEST_H_
#define TESTS_COMM_LAYER_COMM_LAYER_TEST_H_

#include "Instances/Common.h"
#include "Devices/Char_LCD/Char_LCD.h"




class Comm_Layer_test: SoftwareEvents_base
{
public:
  Comm_Layer_test ();
  virtual ~Comm_Layer_test ();

  void loop();
  void software_event_callback(
  	SoftwareEvents::Event_Names_enum event_name);


private:
  Comm_Layer*		comm_layer	= NULL;
  Char_LCD* 		LCD 		= NULL;



};

#endif /* TESTS_COMM_LAYER_COMM_LAYER_TEST_H_ */
