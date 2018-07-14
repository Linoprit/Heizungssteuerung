/*
 * heiz_display.h
 *
 *  Created on: 06.06.2018
 *      Author: harald
 */

#ifndef DEVICES_NEXTION_HEIZ_DISPLAY_H_
#define DEVICES_NEXTION_HEIZ_DISPLAY_H_

#include "Devices/Nextion/ITEADLIB_Arduino_Nextion/Nextion.h"


class Heiz_display
{
public:
  Heiz_display ();
  virtual ~Heiz_display () { };


  void loop(void);

private:


};

#endif /* DEVICES_NEXTION_HEIZ_DISPLAY_H_ */
