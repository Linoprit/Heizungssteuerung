/*
 * aliveTask.h
 *
 *  Created on: 21.07.2018
 *      Author: harald
 */

#ifndef TASKS_ALIVETASK_H_
#define TASKS_ALIVETASK_H_


// C interface
#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

EXTERNC void StartAliveTsk(void const * argument);

#undef EXTERNC

#ifdef __cplusplus

// put cpp includes here!!


class alive_Task
{
public:
  alive_Task ();
  virtual
  ~alive_Task ();

  static void StartAliveTsk(void const * argument);
};

#endif // C interface
#endif /* TASKS_ALIVETASK_H_ */
