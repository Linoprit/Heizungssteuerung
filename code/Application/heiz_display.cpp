/*
 * heiz_display.cpp
 *
 *  Created on: 06.06.2018
 *      Author: harald
 */


#include "heiz_display.h"
#include "libraries/Arduino/itoa.h"
#include "Instances/Common.h"
#include <../Application/State_Machine.h>
#include <libraries/HelpersLib.h>



uint32_t Heiz_display::setup_vals[SETUP_VALS_LEN];
uint32_t Heiz_display::bttn_states[BTTN_STATES_LEN];
char     Heiz_display::date_time_str[DATE_TIME_LEN];
char     Heiz_display::tag_nacht[TAGT_NACHT_LEN];
uint8_t  Heiz_display::curr_page = 0xFF; // 0xFF = init



// Declare objects [page id, component id, component name]

// page 0, display values. pushed from control-task.
NexNumber n0_pmp1_curr  = NexNumber(0, 13, "n0_pmp1_curr");
NexNumber n0_paus1_curr = NexNumber(0, 14, "n0_paus1_curr");
NexNumber n0_pmp2_curr  = NexNumber(0, 15, "n0_pmp2_curr");
NexNumber n0_paus2_curr = NexNumber(0, 16, "n0_paus2_curr");
NexText   t0_daynight   = NexText  (0, 10, "t0_daynight");
NexNumber n0_pmp1_max	= NexNumber(0,  3, "n0_pmp1_max");
NexNumber n0_paus1_max	= NexNumber(0,  7, "n0_paus1_max");
NexNumber n0_pmp2_max	= NexNumber(0,  8, "n0_pmp2_max");
NexNumber n0_paus2_max  = NexNumber(0,  9, "n0_paus2_max");
NexNumber n0_paus2_sm   = NexNumber(0, 24, "n0_paus2_sm");
NexNumber n0_second     = NexNumber(0, 23, "n0_second");
NexText   t0_date_time  = NexText  (0, 33, "t0_date_time");


// page 1, setup values. we need to get them, if Exit-button was released.
NexNumber  n1_day       = NexNumber(1, 22, "page1.n1_day");
NexNumber  n1_month     = NexNumber(1, 23, "page1.n1_month");
NexNumber  n1_year      = NexNumber(1, 24, "page1.n1_year");
NexNumber  n1_hour      = NexNumber(1, 27, "page1.n1_hour");
NexNumber  n1_minute    = NexNumber(1, 28, "page1.n1_minute");
NexNumber  n1_pmp1_max  = NexNumber(1, 5,  "page1.n1_pmp1_max");
NexNumber  n1_paus1_max = NexNumber(1, 6,  "page1.n1_paus1_max");
NexNumber  n1_pmp2_max  = NexNumber(1, 7,  "page1.n1_pmp2_max");
NexNumber  n1_paus2_max = NexNumber(1, 8,  "page1.n1_paus2_max");

NexVariable va1_checksum = NexVariable(1, 42, "page1.va1_checksum");
NexVariable va0_modus	= NexVariable(0, 32, "va0_modus");

NexButton b1_Exit	   	= NexButton(1, 9, 	 "b1_Exit");
NexButton b0_setup      = NexButton(0, 11, 	 "b0_setup");
NexDSButton bt0_pmp1	= NexDSButton(0, 18, "bt0_pmp1");
NexDSButton bt0_pmp2	= NexDSButton(0, 21, "bt0_pmp2");
NexDSButton b0_modus	= NexDSButton(0, 1,  "b0_modus");


NexTouch* nex_listen_list[] =
	{
		&b1_Exit,
		&b0_setup,
		&bt0_pmp1,
		&bt0_pmp2,
		&b0_modus,
		NULL
	};

NexNumber* page1_setup_list[] =
	{
		&n1_pmp1_max  ,
		&n1_paus1_max ,
		&n1_pmp2_max  ,
		&n1_paus2_max ,
		&n1_day       ,
		&n1_month     ,
		&n1_year      ,
		&n1_hour      ,
		&n1_minute    ,
		NULL
	};

void b1_Exit_PopCallback(void *ptr)
{
  UNUSED(ptr);

  uint32_t chksum_ist=0, chksum_soll=0;
  NexNumber *e = NULL;
  uint8_t i;

  for(i = 0; (e = page1_setup_list[i]) != NULL; i++)
	{
	  e->getValue(&Heiz_display::setup_vals[i]);
	  chksum_soll += Heiz_display::setup_vals[i];
	}

  va1_checksum.getValue(&chksum_ist);

  if (chksum_ist == chksum_soll)
	{
	  Common::rtc->set_time_date(&Heiz_display::setup_vals[0]);

	  // save to backup
	  for (uint32_t i=0; i < Heiz_display::paus2_max+1; i++)
		Common::rtc->save_backup_value(Common::heiz_disp->setup_vals[i], i+1);
	}

  sendCommand("page 0");
  Heiz_display::curr_page = 0;

  if (Common::machine->get_modus() == State_Machine::mod_sommer)
	Common::heiz_disp->modus_sommer_active();

  Common::machine->next_state();

}

void bt0_setup_PopCallback(void *ptr)
{
  UNUSED(ptr);

  sendCommand("page 1");
  Heiz_display::curr_page = 1;
}

void bt0_pmp1_PopCallback(void *ptr)
{
  UNUSED(ptr);

  uint32_t* state =
	  &Heiz_display::bttn_states[Heiz_display::bttn_state_vals_enum::st_pmp1];
  bt0_pmp1.getValue(state);
}

void bt0_pmp2_PopCallback(void *ptr)
{
  UNUSED(ptr);

  uint32_t* state =
	  &Heiz_display::bttn_states[Heiz_display::bttn_state_vals_enum::st_pmp2];
  bt0_pmp2.getValue(state);
}

void b0_modus_PopCallback(void *ptr)
{
  UNUSED(ptr);

  Common::machine->inc_modus();
  Common::heiz_disp->upd_mode_bttn_text();
  Common::machine->loop();
  Common::heiz_disp->upd_page_0();
}


Heiz_display::Heiz_display ()
{
  b1_Exit.attachPop (b1_Exit_PopCallback, 	&b1_Exit);
  b0_setup.attachPop(bt0_setup_PopCallback, &b0_setup);
  bt0_pmp1.attachPop(bt0_pmp1_PopCallback, 	&bt0_pmp1);
  bt0_pmp2.attachPop(bt0_pmp2_PopCallback, 	&bt0_pmp2);
  b0_modus.attachPop(b0_modus_PopCallback, 	&b0_modus);

  // RTC-Backup holen
  for (uint32_t i=0; i < paus2_max+1; i++)
	setup_vals[i] = Common::rtc->read_backup_value(i+1);

  setup_vals[pmp1_summer_max]  = PMP1_SOMMER_MAX	;
  setup_vals[paus1_summer_max] = PAUS1_SOMMER_MAX	;
  setup_vals[pmp2_summer_max]  = PMP2_SOMMER_MAX	;
  setup_vals[paus2_summer_max] = WHOLE_WEEK_MINUTES ;
}


void Heiz_display::loop(void)
{
  static bool 		init_setup_vals = false;
  static uint32_t	old_second	= 0;

  nexLoop(nex_listen_list);

  uint32_t time_date[SETUP_VALS_LEN];
  Common::rtc->get_time_date(time_date);

  // initialize
  if (Heiz_display::curr_page == 0xFF)
	{
	  nexInit();
	  sendCommand("page 0");
	  Heiz_display::curr_page = 0;
	  upd_page_0();
	}


  if (Heiz_display::curr_page == 0)
	{
	  init_setup_vals = false;

	  if (old_second != time_date[Heiz_display::second])
		{
		  old_second = time_date[Heiz_display::second];
		  n0_second.setValue(time_date[Heiz_display::second]);
		}

	  if (time_date[Heiz_display::second] == 0 )
		{
		  upd_page_0();
		}
	}

  if ( (Heiz_display::curr_page == 1) && (init_setup_vals == false) )
  	{
	  init_setup_vals = true;

	  Common::rtc->get_time_date(&setup_vals[0]);
	  n1_day.setValue(setup_vals[Heiz_display::day]);
	  n1_month.setValue(setup_vals[Heiz_display::month]);
	  n1_year.setValue(setup_vals[Heiz_display::year]);
	  n1_hour.setValue(setup_vals[Heiz_display::hour]);
	  n1_minute.setValue(setup_vals[Heiz_display::minute]);
  	}

}

void Heiz_display::upd_page_0(void)
{
  upd_mode_bttn_text();
  upd_betrieb_text();
  upd_time_date_str();
  upd_op_times();
}

void Heiz_display::upd_op_times(void)
{
  uint32_t op_times[OP_TIMES_LEN];
  Common::machine->get_op_times(&op_times[0]);

  n0_pmp1_curr. setValue(op_times[State_Machine::op_pmp1]);
  n0_paus1_curr.setValue(op_times[State_Machine::op_paus1]);
  n0_pmp2_curr. setValue(op_times[State_Machine::op_pmp2]);

  if (Common::machine->get_modus() == State_Machine::mod_sommer)
	{
	  n0_paus2_sm.setValue(op_times[State_Machine::op_paus2]);
	  n0_paus2_curr.setValue(0);
	}
  else
	{
	  n0_paus2_sm.setValue(0);
	  n0_paus2_curr.setValue(op_times[State_Machine::op_paus2]);
	}

}

void Heiz_display::inactivate_pump_bttns(void)
{
  bt0_pmp1.setValue(0);
  bt0_pmp2.setValue(0);
  bttn_states[st_pmp1] = 0;
  bttn_states[st_pmp2] = 0;
}


void Heiz_display::upd_mode_bttn_text(void)
{
  if (Common::machine->get_modus() == State_Machine::mod_winter)
	b0_modus.setText("Winter");
  if (Common::machine->get_modus() == State_Machine::mod_sommer)
	b0_modus.setText("Sommer");
  if (Common::machine->get_modus() == State_Machine::mod_manual)
	b0_modus.setText("Manual");
}

void Heiz_display::upd_betrieb_text(void)
{
  if (Common::machine->get_betrieb() == State_Machine::bet_tag)
	t0_daynight.setText("Tag");
  if (Common::machine->get_betrieb() == State_Machine::bet_nacht)
	t0_daynight.setText("Nacht");
}

void  Heiz_display::upd_time_date_str(void)
{
  RTC_TimeTypeDef sTime;
  RTC_DateTypeDef sDate;
  Common::rtc->get_time_date(&sTime, &sDate);

  String time_str = time_date_to_str(sTime, sDate);
  t0_date_time.setText(time_str.c_str());

  n0_second.setValue(sTime.Seconds);
}

void Heiz_display::modus_sommer_active(void)
{
  n0_pmp1_max.setValue(setup_vals[pmp1_summer_max]);
  n0_paus1_max.setValue(setup_vals[paus1_summer_max]);
  n0_pmp2_max.setValue(setup_vals[pmp2_summer_max]);
  //n0_paus2_max.setValue(WHOLE_WEEK_MINUTES); // whole week
  n0_paus2_max.setValue(0);
}

void Heiz_display::modus_winter_active(void)
{
  n0_pmp1_max.setValue(setup_vals[pmp1_max]);
  n0_paus1_max.setValue(setup_vals[paus1_max]);
  n0_pmp2_max.setValue(setup_vals[pmp2_max]);
  n0_paus2_max.setValue(setup_vals[paus2_max]);
}

String	Heiz_display::time_date_to_str(
	RTC_TimeTypeDef sTime, RTC_DateTypeDef sDate)
{
  String time_str;

  if(sDate.WeekDay == 1)
	time_str += "Mo ";
  if(sDate.WeekDay == 2)
	time_str += "Di ";
  if(sDate.WeekDay == 3)
	time_str += "Mi ";
  if(sDate.WeekDay == 4)
	time_str += "Do ";
  if(sDate.WeekDay == 5)
	time_str += "Fr ";
  if(sDate.WeekDay == 6)
	time_str += "Sa ";
  if(sDate.WeekDay == 0)
	time_str += "So ";

  if (sDate.Date< 10)
  	time_str += "0";
  HelpersLib::num2str(&time_str, (uint32_t) sDate.Date);
  if (sDate.Month< 10)
	time_str += ".0";
  else
	time_str += ".";
  HelpersLib::num2str(&time_str, (uint32_t) sDate.Month);
  if (sDate.Year< 10)
	time_str += ".200";
  else
	time_str += ".20";
  HelpersLib::num2str(&time_str, (uint32_t) sDate.Year);
  time_str += "  ";

  if (sTime.Hours < 10)
  	time_str += "0";
  HelpersLib::num2str(&time_str, (uint32_t) sTime.Hours);
  if (sTime.Minutes < 10)
	time_str += ":0";
  else
	time_str += ":";
  HelpersLib::num2str(&time_str, (uint32_t) sTime.Minutes);
  time_str += ":";

  return time_str;
}


/*



	  op_times[op_pmp1] = 12;
	  String msg_str = "optimes: ";
	  HelpersLib::num2str(&msg_str, op_times[op_pmp1]);
	  msg_str += " setupval: ";
	  HelpersLib::num2str(&msg_str, Common::heiz_disp->get_setup_vals()[Heiz_display::pmp1_max]);
	  msg_str += "\n";
	  Error_messaging::write(msg_str.c_str());




  // TODO remove
  String msg_str = "bt0_pmp2_state: ";
  HelpersLib::num2str(&msg_str, *state );
  msg_str += "\n";
  Error_messaging::write(msg_str.c_str());




  // TODO remove
  String msg_str = "bt0_pmp1_state: ";
  HelpersLib::num2str(&msg_str, *state );
  msg_str += "\n";
  Error_messaging::write(msg_str.c_str());



 */


