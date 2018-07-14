/*
 * controlTask.h
 *
 *  Created on: 06.07.2018
 *      Author: harald
 */

#ifndef TASKS_DISPLAYTASK_H_
#define TASKS_DISPLAYTASK_H_


// C interface
#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

EXTERNC void StartDisplayTsk(void const * argument);

#undef EXTERNC

#ifdef __cplusplus

// put cpp includes here!!


class displayTask
{
public:
  displayTask ();
  virtual ~displayTask () {};

  static void StartDisplayTsk(void const * argument);
};

#endif // C interface
#endif /* TASKS_DISPLAYTASK_H_ */
