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
  curr_modus   	= mod_winter;
  old_modus    	= mod_manual;
  curr_state	= st_pmp1;
  old_state		= st_paus2;
  target_time   = 0;
  curr_time 	= &op_times[op_pmp1];

  for (uint8_t i = 0; i < OP_TIMES_LEN; i++)
	 op_times[i] = 0;
}


void State_Machine::loop(void)
{
  if (curr_modus == mod_winter)
	{
	  if (old_modus != curr_modus) // init
		{
		  old_modus 	= curr_modus;
		  curr_state	= st_paus2;
		  old_state		= st_pmp2;

		  pump_1_off();
		  pump_2_off();
		  Common::heiz_disp->inactivate_pump_bttns();

		  next_state();
		}

	  if(get_betrieb() == bet_tag)
		state_loop();
	  else
		state_stop();
	}

  else if (curr_modus == mod_sommer)
	{
	  if (old_modus != curr_modus) // init
		{
		  old_modus = curr_modus;
		  Common::heiz_disp->modus_sommer_active();

		  curr_state	= st_pmp2;
		  old_state		= st_paus1;

		  pump_1_off();
		  pump_2_off();

		  next_state();
		}

	  state_loop();
	}
  else if (curr_modus == mod_manual)
	{
	  if (old_modus != curr_modus) // init
		{
		  old_modus = curr_modus;
		  Common::heiz_disp->modus_winter_active();

		  next_state();

		  for (uint32_t i=0; i < op_paus2; i++)
			op_times[i] = 0;


		}

	  if( Common::heiz_disp->get_bttn_states()[Heiz_display::st_pmp1] > 0)
		pump_1_on();
	  else
		pump_1_off();

	  if( Common::heiz_disp->get_bttn_states()[Heiz_display::st_pmp2] > 0)
		pump_2_on();
	  else
		pump_2_off();
	}
}

void State_Machine::next_state(void)
{
  *curr_time = 0;

  if(curr_state == st_pmp1)
	{
	  curr_state = st_paus1;
	  if (curr_modus == mod_winter)
		op_times[op_paus1] =
			Common::heiz_disp->get_setup_vals()[Heiz_display::paus1_max];
	  else if (curr_modus == mod_sommer)
		op_times[op_paus1] =
			Common::heiz_disp->get_setup_vals()[Heiz_display::paus1_summer_max];
	  else
		op_times[op_paus1] = 0;

	  curr_time  = &op_times[op_paus1];

	  pump_1_off();
	  pump_2_off();
	}
  else if(curr_state == st_paus1)
	{
	  curr_state = st_pmp2;
	  if (curr_modus == mod_winter)
		op_times[op_pmp2] =
			Common::heiz_disp->get_setup_vals()[Heiz_display::pmp2_max];
	  else if (curr_modus == mod_sommer)
		op_times[op_pmp2] =
			Common::heiz_disp->get_setup_vals()[Heiz_display::pmp2_summer_max];
	  else
		op_times[op_pmp2] = 0;

	  curr_time  = &op_times[op_pmp2];

	  pump_1_off();
	  pump_2_on();
	}
  else if (curr_state == st_pmp2)
	{
	  curr_state = st_paus2;
	  if (curr_modus == mod_winter)
		op_times[op_paus2] =
			Common::heiz_disp->get_setup_vals()[Heiz_display::paus2_max];
	  else if (curr_modus == mod_sommer)
		op_times[op_paus2] = Common::rtc->calc_next_pumpservice();
	  else
		op_times[op_paus2] = 0;

	  curr_time  = &op_times[op_paus2];

	  pump_1_off();
	  pump_2_off();
	}
  else if(curr_state == st_paus2)
	{
	  curr_state = st_pmp1;
	  if (curr_modus == mod_winter)
		op_times[op_pmp1] =
			Common::heiz_disp->get_setup_vals()[Heiz_display::pmp1_max];
	  else
		op_times[op_pmp1] =
			Common::heiz_disp->get_setup_vals()[Heiz_display::pmp1_summer_max];

	  curr_time  = &op_times[op_pmp1];

	  pump_1_on();
	  pump_2_off();
	}


//TODO remove
  String msg_str = "curr_state: ";
  HelpersLib::num2str(&msg_str, curr_state);
  msg_str += " curr_time: ";
  HelpersLib::num2str(&msg_str, *curr_time);
  msg_str += "\n";
  Error_messaging::write(msg_str.c_str());


}

void State_Machine::state_loop(void)
{
  if (old_state != curr_state) // init
	{
	  old_state = curr_state;
	  inc_target_time();
	}



  // TODO remove
  String msg_str = "get_time: ";
  HelpersLib::num2str(&msg_str, Common::rtc->get_time_minutes());
  msg_str += " ";
  HelpersLib::num2str(&msg_str, target_time);
  msg_str += " ";



  if (Common::rtc->get_time_minutes() != target_time)
	return;

  // cycle
  *curr_time -= 1;



  // TODO remove
  msg_str += " curr_time: ";
  HelpersLib::num2str(&msg_str, *curr_time);
  msg_str += "\n";
  Error_messaging::write(msg_str.c_str());




  if (*curr_time == 0)
	next_state();
  else
	 inc_target_time();
}

void State_Machine::inc_target_time(void)
{
  if (Common::rtc->get_time_minutes() == WHOLE_WEEK_MINUTES - 1)
	target_time = 0;
  else
	target_time = Common::rtc->get_time_minutes() + 1;
}

void State_Machine::state_stop(void)
{
  uint32_t i;

  pump_1_off();
  pump_2_off();

  for(i=0; i < OP_TIMES_LEN; i++)
	op_times[i] = 0;
}

State_Machine::modus_enum State_Machine::get_modus(void)
{
  return curr_modus;
}

void State_Machine::inc_modus(void)
{
  if(curr_modus == mod_manual)
	curr_modus = mod_winter;
  else if (curr_modus == mod_winter)
	curr_modus = mod_sommer;
  else if (curr_modus == mod_sommer)
	curr_modus = mod_manual;
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
	values[i] = this->op_times[i];
}




