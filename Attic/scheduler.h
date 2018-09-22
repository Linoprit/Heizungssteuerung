/*
 * scheduler.h
 *
 *  Created on: 09.12.2016
 *      Author: harald
 */

#ifndef SYSTEM_SCHEDULER_H_
#define SYSTEM_SCHEDULER_H_


#include <stdint.h>

// C interface
#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

EXTERNC void scheduler_cycle(void);
EXTERNC void scheduler_init(void);

#undef EXTERNC

#ifdef __cplusplus

// put cpp includes here!!
// #include blah....


class scheduler
{
public:
  scheduler (void);
  virtual ~scheduler (void);

  static void cycle (void);
  static void init (void);


private:
  static uint32_t old_tick;

};

#endif // C interface
#endif /* SYSTEM_SCHEDULER_H_ */
