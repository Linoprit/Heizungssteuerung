/**
 * @file NexConfig.h
 *
 * Options for user can be found here. 
 *
 * @author  Wu Pengfei (email:<pengfei.wu@itead.cc>)
 * @date    2015/8/13
 * @copyright 
 * Copyright (C) 2014-2015 ITEAD Intelligent Systems Co., Ltd. \n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */
#ifndef __NEXCONFIG_H__
#define __NEXCONFIG_H__

//#include "../Framework/System/Error_messaging.h"
#include "string.h"
#include "strings.h"
#include "../Framework/System/uart_printf.h"

/**
 * @addtogroup Configuration 
 * @{ 
 */

/** 
 * Define DEBUG_SERIAL_ENABLE to enable debug serial. 
 * Comment it to disable debug serial. 
 */
//#define DEBUG_SERIAL_ENABLE
// BE CARFULL, CALLING dbSerialPrint WITH a = '0' CAUSES TX_PRINT TO CRASH



/**
 * Define nexSerial for communicate with Nextion touch panel. 
 */
#define nexSerial Common::get_nexDisplay()
#define millis	  Common::get_tick


#ifdef DEBUG_SERIAL_ENABLE

#define dbSerialPrint(a)    tx_printf("%s", a)
#define dbSerialPrintln(a)  tx_printf("%s\n", a)
#define dbSerialBegin(a)
#else
#define dbSerialPrint(a)    do{}while(0)
#define dbSerialPrintln(a)  do{}while(0)
#define dbSerialBegin(a)    do{}while(0)
#endif

/**
 * @}
 */

#endif /* #ifndef __NEXCONFIG_H__ */
