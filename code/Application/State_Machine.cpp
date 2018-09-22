/*
 * State_Machine.cpp
 *
 *  Created on: 07.08.2018
 *      Author: harald
 */

#include "State_Machine.h"
#include "stm32f1xx_hal.h"
#include "Instances/Common.h"
#include "libraries/HelpersLib.h"



State_Machine::State_Machine ()
{
  act_modus   	= mod_winter;
  old_modus    	= mod_manual;
  act_state		= st_paus2;
  target_time   = 0;
  act_time 		= &remaining_times[op_pmp1];
  *act_time		= 1;
  time_set 		= Common::heiz_disp->get_winter_times();
  is_entry		= true;

  for (uint8_t i = 0; i < OP_TIMES_LEN; i++)
	 remaining_times[i] = 0;
}

void State_Machine::check_pumps(void)
{
  bool bttn_pmp1 = Common::heiz_disp->get_bttn_pmp1();
  bool bttn_pmp2 = Common::heiz_disp->get_bttn_pmp2();

  if (
	  ( (get_betrieb() == bet_tag) && (get_modus() == mod_winter) && (get_state() == st_pmp1) )
	  ||
	  ( (get_modus() == mod_sommer) && (get_state() == st_pmp1 )  )
	  ||
	  ( (get_modus() == mod_manual) && (bttn_pmp1 == true) )
  )
	{
	  pump_1_on();
	}

  if (
	  ( (get_betrieb() == bet_nacht) && (get_modus() == mod_winter) )
	  ||
	  ( (get_modus() != mod_manual) && ( (get_state() == st_paus1) || (get_state() == st_paus2)) )
	  ||
	  ( (get_modus() == mod_manual) && (bttn_pmp1 == false) )
  )
	{
	  pump_1_off();
	}

  if (
	  ( (get_betrieb() == bet_tag) && (get_modus() == mod_winter) && (get_state() == st_pmp2) )
	  ||
	  ( (get_modus() == mod_sommer) && (get_state() == st_pmp2 )  )
	  ||
	  ( (get_modus() == mod_manual) && (bttn_pmp2 == true) )
  )
	{
	  pump_2_on();
	}

  if (
	  ( (get_betrieb() == bet_nacht) && (get_modus() == mod_winter) )
	  ||
	  ( (get_modus() != mod_manual) && ( (get_state() == st_paus1) || (get_state() == st_paus2)) )
	  ||
	  ( (get_modus() == mod_manual) && (bttn_pmp2 == false) )
  )
	{
	  pump_2_off();
	}
}

void State_Machine::check_tables(void)
{
  if (act_modus != old_modus)
	{
	  old_modus = act_modus;

	  if (act_modus == mod_sommer)
		{
		  Common::heiz_disp->inactivate_pump_bttns();
		  Common::heiz_disp->modus_sommer_active();
		  time_set 		= Common::heiz_disp->get_summer_times();
		  time_set[op_paus2] = Common::rtc->calc_next_pumpservice();
		}

	  if (act_modus == mod_winter)
		{
		  Common::heiz_disp->inactivate_pump_bttns();
		  Common::heiz_disp->modus_winter_active();
		  time_set 		= Common::heiz_disp->get_winter_times();
		}

	  if ( act_modus == mod_manual )
		{
		  Common::heiz_disp->modus_winter_active();
		  time_set 		= Common::heiz_disp->get_winter_times();
		  next_state();
		}
	}
}

void State_Machine::state_pmp1(void)
{
  // entry
  if (is_entry == true)
	{
	  is_entry = false;

	  remaining_times[op_paus2] = 0;
	  remaining_times[op_pmp1] = *(time_set + Heiz_display::pmp1_max);
	  act_time  = &remaining_times[op_pmp1];
	  inc_target_time();
	}

  check_act_time();
}

void State_Machine::state_pmp2(void)
{
  // entry
  if (is_entry == true)
	{
	  is_entry = false;

	  remaining_times[op_paus1] = 0;
	  remaining_times[op_pmp2] = *(time_set + Heiz_display::pmp2_max);
	  act_time  = &remaining_times[op_pmp2];
	  inc_target_time();
	}

  check_act_time();
}

void State_Machine::state_paus1(void)
{
  // entry
  if (is_entry == true)
	{
	  is_entry = false;

	  remaining_times[op_pmp1] = 0;
	  remaining_times[op_paus1] = *(time_set + Heiz_display::paus1_max);
	  act_time  = &remaining_times[op_paus1];
	  inc_target_time();
	}

  check_act_time();
}

void State_Machine::state_paus2(void)
{
  // entry
  if (is_entry == true)
	{
	  is_entry = false;

	  remaining_times[op_pmp2] = 0;

	  if (act_modus != mod_sommer)
		remaining_times[op_paus2] = *(time_set + Heiz_display::paus2_max);
	  else
		remaining_times[op_paus2] = Common::rtc->calc_next_pumpservice();

	  act_time  = &remaining_times[op_paus2];
	  inc_target_time();
	}

  check_act_time();
}

void State_Machine::check_act_time(void)
{
  if (Common::rtc->get_time_minutes() != target_time)
 	return;

   // cycle
   if (*act_time > 0)
 	*act_time -= 1;

   inc_target_time();
}

void State_Machine:: next_state(void)
{
  is_entry = true;

  if (act_state == st_pmp1)
	  act_state = st_paus1;
  else if (act_state == st_paus1)
	  act_state = st_pmp2;
  else if (act_state == st_pmp2)
	  act_state = st_paus2;
  else if (act_state == st_paus2)
	  act_state = st_pmp1;
}

void State_Machine::loop(void)
{
  RTC_TimeTypeDef sTime;
  RTC_DateTypeDef sDate;
  Common::rtc->get_time_date(&sTime, &sDate);

  bool its_wednesday =
	  (sDate.WeekDay == RTC_WEEKDAY_WEDNESDAY) && (sTime.Hours == 12) && (sTime.Minutes == 0);




  // TODO remove
  /*String msg_str = "state: ";
  HelpersLib::num2str(&msg_str, act_state);
  msg_str += " is_entry: ";
  HelpersLib::num2str(&msg_str, is_entry);
  msg_str += " modus: ";
  HelpersLib::num2str(&msg_str, get_modus());
  msg_str += " betrieb: ";
  HelpersLib::num2str(&msg_str, get_betrieb());
  msg_str += " r: ";
  for(int i=0; i<op_paus2+1; i++)
	{
	  HelpersLib::num2str(&msg_str, remaining_times[i]);
	  msg_str += " ";
	}
  msg_str += " mxs: ";
  for(int i=0; i<op_paus2+1; i++)
	{
	  HelpersLib::num2str(&msg_str, time_set[i]);
	  msg_str += " ";
	}
  msg_str += "\n";
  Error_messaging::write(msg_str.c_str());*/






  check_tables();

  // transitions
  if ( (*act_time == 0) && (act_state != st_paus2) )
	{
	  next_state();
	}
  else if ( (act_state == st_paus2) &&
	  (
		  ( (*act_time == 0) && (get_modus() == mod_winter) )
		  ||
		  ( (its_wednesday) && (get_modus() == mod_sommer) )
	  ) )
	{
	  next_state();
	}

  // states
  if (act_state == st_pmp1)
	state_pmp1();

  if (act_state == st_paus1)
	state_paus1();

  if (act_state == st_pmp2)
	state_pmp2();

  if (act_state == st_paus2)
	state_paus2();

  check_pumps();
}


void State_Machine::inc_target_time(void)
{
  if (Common::rtc->get_time_minutes() == WHOLE_WEEK_MINUTES - 1)
	target_time = 0;
  else
	target_time = Common::rtc->get_time_minutes() + 1;
}

State_Machine::modus_enum State_Machine::get_modus(void)
{
  return act_modus;
}

void State_Machine::inc_modus(void)
{
  if(act_modus == mod_manual)
	act_modus = mod_winter;
  else if (act_modus == mod_winter)
	act_modus = mod_sommer;
  else if (act_modus == mod_sommer)
	act_modus = mod_manual;
}

void State_Machine::pump_1_on(void)
{
  HAL_GPIO_WritePin(Pump_1_GPIO_Port, Pump_1_Pin, GPIO_PIN_SET);
}

void State_Machine::pump_1_off(void)
{
  HAL_GPIO_WritePin(Pump_1_GPIO_Port, Pump_1_Pin, GPIO_PIN_RESET);
}

void State_Machine::pump_2_on(void)
{
  HAL_GPIO_WritePin(Pump_2_GPIO_Port, Pump_2_Pin, GPIO_PIN_SET);
}

void State_Machine::pump_2_off(void)
{
  HAL_GPIO_WritePin(Pump_2_GPIO_Port, Pump_2_Pin, GPIO_PIN_RESET);
}

State_Machine::state_enum State_Machine::get_state(void)
{
  return act_state;
}

State_Machine::betrieb_enum State_Machine::get_betrieb(void)
{
  if (HAL_GPIO_ReadPin(DayNight_GPIO_Port, DayNight_Pin) == GPIO_PIN_RESET)
	return bet_tag;
  else
	return bet_nacht;
}

void State_Machine::get_op_times(uint32_t* values)
{
  uint32_t i;
  for (i=0; i < OP_TIMES_LEN; i++)
	values[i] = this->remaining_times[i];
}




