/*
 * callbacks.h
 *
 *  Created on: 17.05.2016
 *      Author: harald
 */

#ifndef CALLBACKS_H_
#define CALLBACKS_H_

#include <stdlib.h>
#include "stm32f1xx_hal.h"


class ISR_callback {
public:
  virtual void ISR_callback_fcn () = 0;
  virtual ~ISR_callback(void) { };
};


void uart_callback_add(UART_HandleTypeDef *huart, ISR_callback* callback);


#endif /* CALLBACKS_H_ */
