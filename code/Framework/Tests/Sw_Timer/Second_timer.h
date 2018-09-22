/*
 * Second_timer.h
 *
 *  Created on: 18.12.2016
 *      Author: harald
 */

#ifndef TESTS_SW_TIMER_SECOND_TIMER_H_
#define TESTS_SW_TIMER_SECOND_TIMER_H_


#include "System/SoftwareTimer.h"


class Second_timer: SoftwareTimer_base
{
public:
  Second_timer ();
  virtual
  ~Second_timer ();

  void timer_hit_callback(void);

private:
  SoftwareTimer* timer_2;
};

#endif /* TESTS_SW_TIMER_SECOND_TIMER_H_ */
