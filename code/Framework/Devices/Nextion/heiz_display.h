/*
 * heiz_display.h
 *
 *  Created on: 06.06.2018
 *      Author: harald
 */

#ifndef DEVICES_NEXTION_HEIZ_DISPLAY_H_
#define DEVICES_NEXTION_HEIZ_DISPLAY_H_

#include "Devices/Nextion/ITEADLIB_Arduino_Nextion/Nextion.h"

/**
 * Verbindet einen Task mit der NexDisplay Funktionalität.
 * Enthält alle Callbacks, welche das Display ruft.
 */
class Heiz_display
{
public:
  enum setup_vals_enum {
	day       ,
	month     ,
	year      ,
	hour      ,
	minute    ,
	pmp1_max  ,
	paus1_max ,
	pmp2_max  ,
	paus2_max
  };

  enum bttn_states_enum {
	pmp1, pmp2, modus
  };

  Heiz_display ();
  virtual ~Heiz_display () { };

  void loop(void);

  // display interface
  static uint32_t setup_vals[9]; // corresponding to page1_setup_list
  static uint32_t bttn_states[3];


private:


};

#endif /* DEVICES_NEXTION_HEIZ_DISPLAY_H_ */
