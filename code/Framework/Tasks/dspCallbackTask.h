/*
 * dspCallbackTask.h
 *
 *  Created on: 06.07.2018
 *      Author: harald
 */

#ifndef SYSTEM_dspCallbackTask_H_
#define SYSTEM_dspCallbackTask_H_


// C interface
#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

EXTERNC void StartDspCallbackTsk(void const * argument);

#undef EXTERNC

#ifdef __cplusplus

// put cpp includes here!!


class dspCallbackTask
{
public:
  dspCallbackTask ();
  virtual ~dspCallbackTask () {};

  static void StartDspCallbackTsk(void const * argument);
};

#endif // C interface
#endif /* SYSTEM_dspCallbackTask_H_ */
