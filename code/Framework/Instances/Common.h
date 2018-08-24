/*
 * Initclasses.h
 *
 *  Created on: 06.05.2016
 *      Author: harald
 */

#ifndef COMMON_H_
#define COMMON_H_

#include "stm32f1xx_hal.h"
#include "cmsis_os.h"


// C interface
#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

EXTERNC void common_init(void);
EXTERNC UART_HandleTypeDef* get_huart1(void);
EXTERNC UART_HandleTypeDef* get_huart3(void);
EXTERNC RTC_HandleTypeDef*  get_rtc(void)   ;
EXTERNC osThreadId* 		get_controlTask(void) ;
EXTERNC osThreadId* 		get_displayTask(void) ;
EXTERNC osThreadId* 		get_errorMsgTask(void);


#undef EXTERNC

#ifdef __cplusplus

// put cpp includes here!!
#include <Devices/Nextion/Nextion_device.h>
#include "../Application/heiz_display.h"
#include <Devices/Realtime_clock/rtc_device.h>
#include <../Application/State_Machine.h>
#include <Tasks/errorMsgTask.h>



class Common
{
public:

  Common() {};
  virtual ~Common() {};

  static void init(void);
  static uint32_t get_tick(void);

  static inline void delay(uint32_t delay)
  { HAL_Delay(delay); }

  // sockets
  static uart_socket	*display_comm;
  static uart_socket	*errorMsg_comm;

  // public Devices
  static nextion	  	*nex_disp;
  static Rtc_device 	*rtc;

  // application Classes
  static Heiz_display 	*heiz_disp;
  static State_Machine	*machine;

  // system Class
  static Error_messaging *error_msg;

private:

};

#endif // C interface
#endif /* COMMON_H_ */
