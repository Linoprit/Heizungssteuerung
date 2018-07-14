/*
 * Initclasses.h
 *
 *  Created on: 06.05.2016
 *      Author: harald
 */

#ifndef COMMON_H_
#define COMMON_H_

#include "stm32f1xx_hal.h"


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

#undef EXTERNC

#ifdef __cplusplus

// put cpp includes here!!
#include <System/SoftwareEvents.h>
#include <Devices/Nextion/Nextion_display.h>
#include <Devices/Nextion/heiz_display.h>


class Common
{
public:

  Common() {};
  virtual ~Common() {};

  static void init(void);
  static SoftwareEvents*    get_sw_events(void);

  static uint32_t get_tick(void);

  static inline void delay(uint32_t delay)
  { HAL_Delay(delay); }

  // public Devices
  static nextion	  *nex_disp;
  static Heiz_display *heiz_disp;

private:
 /* static void init_comm_layer(void);
  static Comm_Layer *comm_layer;*/


  static SoftwareEvents    *sw_events;
};

#endif // C interface
#endif /* COMMON_H_ */
