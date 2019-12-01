/*
 * Initclasses.h
 *
 *  Created on: 06.05.2016
 *      Author: harald
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <stdbool.h>
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"


// C interface
#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

EXTERNC void common_init(void);
EXTERNC bool common_initIsDone(void);

// bodies are in main.c
EXTERNC UART_HandleTypeDef* get_huart1(void);
EXTERNC UART_HandleTypeDef* get_huart3(void);
EXTERNC RTC_HandleTypeDef*  get_rtc(void)   ;
EXTERNC CRC_HandleTypeDef*  get_crc(void)	;
EXTERNC osThreadId* 		get_controlTask(void) ;
EXTERNC osThreadId* 		get_displayTask(void) ;
EXTERNC osThreadId* 		get_errorMsgTask(void);
EXTERNC osMessageQId* 		get_uartQueue(void)	;
EXTERNC uint8_t* 			get_uartBuffer(void);
#undef EXTERNC

#ifdef __cplusplus

// put cpp includes here!!
#include <Devices/Nextion/Nextion_device.h>
#include <heiz_display.h>
#include <HeizStateMachine/MainMachine.h>
#include <Sockets/RtcSocket.h>
#include <Sockets/CrcSocket.h>
#include <ValueStorage.h>



class Common
{
public:

  Common() {};
  virtual ~Common() {};
  static bool isInitDone(void);

  static void init(void);
  static uint32_t get_tick(void);

  static inline void delay(uint32_t delay) { HAL_Delay(delay); }


  static inline uart_socket*	get_displaySocket(void) { return displaySocket; };
  static inline RtcSocket*		get_rtcSocket(void)		{ return rtcSocket;		};
  static inline CrcSocket*		get_crcSocket(void)		{ return crcSocket;		};

  static inline nextion*		get_nexDisplay(void)	{ return nexDevice;		};

  static inline MainMachine* 	get_mainMachine(void) 	{ return mainMachine; 	};
  static inline Heiz_display* 	get_heizDisplay(void) 	{ return heizDisplay;   };
  static inline ValueStorage*   get_valueStorage(void)	{ return valueStorage;	};

private:
  static bool _initIsDone;

  // sockets
  static uart_socket	*displaySocket;
  static RtcSocket	 	*rtcSocket;
  static CrcSocket		*crcSocket;

  // public Devices
  static nextion	  	*nexDevice;

  // application Classes
  static Heiz_display 	*heizDisplay;
  static MainMachine	*mainMachine;
  static ValueStorage	*valueStorage;

};

#endif // C interface
#endif /* COMMON_H_ */
