/*
 * dspCallbackTask.h
 *
 *  Created on: 06.07.2018
 *      Author: harald
 */

#ifndef SYSTEM_dspCallbackTask_H_
#define SYSTEM_dspCallbackTask_H_

/**
 * Used as link between C-call from main to Start...Task().
 * Does not need to be instantiated, if no use of C++ calls to other
 * classes is done.
 */

// C interface
#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

EXTERNC void StartErrorMsgTsk(void const * argument);

#undef EXTERNC

#ifdef __cplusplus

// put cpp includes here!!


class errorMsgTask
{
public:
  errorMsgTask ();
  virtual ~errorMsgTask () {};

  static void StartErrorMsgTsk(void const * argument);
};

#endif // C interface
#endif /* SYSTEM_dspCallbackTask_H_ */
