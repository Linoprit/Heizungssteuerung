/*
 * UartTask.h
 *
 *  Created on: 14.07.2018
 *      Author: harald
 */

#ifndef TASKS_UARTTASK_H_
#define TASKS_UARTTASK_H_

// C interface
#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

EXTERNC void StartUartTsk(void const * argument);

#undef EXTERNC

#ifdef __cplusplus

// put cpp includes here!!


class UartTask
{
public:
  UartTask ();
  virtual ~UartTask () {};

  static void StartUartTsk(void const * argument);

};


#endif // C interface
#endif /* TASKS_UARTTASK_H_ */
