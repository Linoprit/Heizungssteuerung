/*
 * Sw_Timer_Test.h
 *
 *  Created on: 15.05.2016
 *      Author: harald
 */

#ifndef MACHINE_TEST_SW_TIMER_TEST_H_
#define MACHINE_TEST_SW_TIMER_TEST_H_

#include "System/SoftwareTimer.h"
#include "System/SoftwareEvents.h"


class Sw_Timer_Test: SoftwareTimer_base
{
public:
  Sw_Timer_Test ();
  virtual ~Sw_Timer_Test ();

  void Do_Timer_Test();
  void timer_hit_callback(void);

private:
  SoftwareTimer* timer;

};

#endif /* MACHINE_TEST_SW_TIMER_TEST_H_ */
