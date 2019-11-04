/*
 * controlTask.h
 *
 *  Created on: 06.07.2018
 *      Author: harald
 */

#ifndef TASKS_CONTROLTASK_H_
#define TASKS_CONTROLTASK_H_


// C interface
#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

EXTERNC void StartControlTsk(void const * argument);

#undef EXTERNC

#ifdef __cplusplus

// put cpp includes here!!




#endif // C interface
#endif /* TASKS_CONTROLTASK_H_ */
