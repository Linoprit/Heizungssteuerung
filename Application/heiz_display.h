/*
 * heiz_display.h
 *
 *  Created on: 06.06.2018
 *      Author: harald
 */

#ifndef DEVICES_NEXTION_HEIZ_DISPLAY_H_
#define DEVICES_NEXTION_HEIZ_DISPLAY_H_

#include "stm32f1xx_hal.h"
#include "../Framework/Devices/Nextion/ITEADLIB_Nextion/Nextion.h"
#include "../Framework/libraries/Arduino/WString.h"
//#include "../Application/State_Machine.h"
#include <HeizStateMachine/MainMachine.h>


/**
 * Verbindet einen Task mit der NexDisplay Funktionalität.
 * Enthält alle Callbacks, welche das Display ruft.
 */
class Heiz_display
{
public:
#define SETUP_VALS_LEN   14
#define BTTN_STATES_LEN   2
#define DATE_TIME_LEN    23
#define TAGT_NACHT_LEN    5

#define PMP1_SOMMER_MAX	  5
#define PAUS1_SOMMER_MAX  1
#define PMP2_SOMMER_MAX	  5



  enum setup_vals_enum {
	pmp1_max=0  ,	paus1_max ,				// inital aus RTC-Backup holen
	pmp2_max  	,	paus2_max ,
	day       	,	month     ,	year    ,	// nur relevant aus Setup-Exit heraus
	hour      	,	minute    , second	,
	pmp1_summer_max  ,	paus1_summer_max , // feste Werte, paus2 wird berechnet
	pmp2_summer_max  ,	paus2_summer_max
  };

  enum bttn_state_vals_enum {
	st_pmp1, st_pmp2
  };

  Heiz_display ();
  virtual ~Heiz_display () { };

  void loop(void);

  inline uint32_t*  get_setup_vals(void) 	{ return &setup_vals[0]; 	}
  inline uint32_t*  get_bttn_states(void)	{ return &bttn_states[0];	}
  inline uint32_t*  get_winter_times(void) 	{ return &setup_vals[0]; 	}
  inline uint32_t*  get_summer_times(void) 	{ return &setup_vals[pmp1_summer_max]; 	}
  bool get_bttn_pmp1(void);
  bool get_bttn_pmp2(void);

  void inactivate_pump_bttns(void);

  void 	upd_mode_bttn_text(void);
  void 	upd_dayNight_text(void);
  void  upd_time_date_str(void);
  void  upd_op_times(void);
  void 	upd_page_0(void);

  void modus_sommer_active(void);
  void modus_winter_active(void);

  String time_date_to_str(RTC_TimeTypeDef sTime, RTC_DateTypeDef sDate);

  // display interface. must be public
  static uint32_t setup_vals[SETUP_VALS_LEN]; // corresponding to page1_setup_list
  static uint32_t bttn_states[BTTN_STATES_LEN];

  static char date_time_str[DATE_TIME_LEN];
  static char tag_nacht[TAGT_NACHT_LEN];

  static uint8_t curr_page;
  //static State_Machine::betrieb_enum old_betrieb;
  //static  MainMachine::enm_states old_betrieb;

private:




};

#endif /* DEVICES_NEXTION_HEIZ_DISPLAY_H_ */
