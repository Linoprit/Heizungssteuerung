/*
 * heiz_display.cpp
 *
 *  Created on: 06.06.2018
 *      Author: harald
 */


#include <Devices/Nextion/heiz_display.h>
#include "libraries/Arduino/itoa.h"
#include "Instances/Common.h"

uint32_t Heiz_display::setup_vals[9];
uint32_t Heiz_display::bttn_states[3];



// Declare objects [page id, component id, component name]

// page 0, display values. pushed from control-task.
NexNumber n0_pmp1_curr  = NexNumber(0, 13, "n0_pmp1_curr");
NexNumber n0_paus1_curr = NexNumber(0, 14, "n0_paus1_curr");
NexNumber n0_pmp2_curr  = NexNumber(0, 15, "n0_pmp2_curr");
NexNumber n0_paus2_curr = NexNumber(0, 16, "n0_paus2_curr");
NexNumber t0_daynight   = NexNumber(0, 10, "t0_daynight");
NexNumber n0_day        = NexNumber(0, 18, "n0_day");
NexNumber n0_month      = NexNumber(0, 19, "n0_month");
NexNumber n0_year       = NexNumber(0, 20, "n0_year");
NexNumber n0_hour       = NexNumber(0, 23, "n0_hour");
NexNumber n0_minute     = NexNumber(0, 24, "n0_minute");
NexNumber n0_second     = NexNumber(0, 25, "n0_second");

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
NexDSButton bt0_pmp1	= NexDSButton(0, 28, "bt0_pmp1");
NexDSButton bt0_pmp2	= NexDSButton(0, 31, "bt0_pmp2");
NexDSButton b0_modus	= NexDSButton(0, 1,  "b0_modus");


NexTouch* nex_listen_list[] =
	{
		&b1_Exit,
		&bt0_pmp1,
		&bt0_pmp2,
		&b0_modus,
		NULL
	};

NexNumber* page1_setup_list[] =
	{
		&n1_day       ,
		&n1_month     ,
		&n1_year      ,
		&n1_hour      ,
		&n1_minute    ,
		&n1_pmp1_max  ,
		&n1_paus1_max ,
		&n1_pmp2_max  ,
		&n1_paus2_max ,
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

  String nmbr_str;
  for(i = 0; i < 9; i++)
	{
	  Error_messaging::num2str(&nmbr_str, Heiz_display::setup_vals[i]);
	  nmbr_str += ' ';
	}

  Error_messaging::num2str(&nmbr_str, chksum_ist);
  nmbr_str += ' ';
  Error_messaging::num2str(&nmbr_str, chksum_soll);

  if (chksum_ist != chksum_soll)
	nmbr_str += " ERROR";
  else
	nmbr_str += " ok";

  nmbr_str += '\n';
  Error_messaging::write(nmbr_str.c_str());

}

void bt0_pmp1_PopCallback(void *ptr)
{
  UNUSED(ptr);

  uint32_t* state =
	  &Heiz_display::bttn_states[Heiz_display::bttn_states_enum::pmp1];
  bt0_pmp1.getValue(state);

  String msg_str;
  msg_str += "bt0_pmp1_PopCallback: ";
  Error_messaging::num2str(&msg_str, *state);
  msg_str += '\n';
  Error_messaging::write(msg_str.c_str());
}

void bt0_pmp2_PopCallback(void *ptr)
{
  UNUSED(ptr);

  uint32_t* state =
	  &Heiz_display::bttn_states[Heiz_display::bttn_states_enum::pmp2];
  bt0_pmp2.getValue(state);

  String msg_str;
  msg_str += "bt0_pmp2_PopCallback: ";
  Error_messaging::num2str(&msg_str, *state);
  msg_str += '\n';
  Error_messaging::write(msg_str.c_str());
}

void b0_modus_PopCallback(void *ptr)
{
  UNUSED(ptr);

  uint32_t * state =
	  &Heiz_display::bttn_states[Heiz_display::bttn_states_enum::modus];
  va0_modus.getValue(state);

  if(*state == 0) // modus == "Winter"
	{
	  state =
	  	  &Heiz_display::bttn_states[Heiz_display::bttn_states_enum::pmp1];
	    bt0_pmp1.getValue(state);

	  state =
	  	  &Heiz_display::bttn_states[Heiz_display::bttn_states_enum::pmp2];
	  bt0_pmp2.getValue(state);
	}

  String msg_str;
  msg_str += "b0_modus_PopCallback: ";

  state =
  	  &Heiz_display::bttn_states[Heiz_display::bttn_states_enum::modus];
  if (*state == 0)
	{
	  msg_str += "Winter: ";
	  Error_messaging::num2str(
		  &msg_str,
		  Heiz_display::bttn_states[Heiz_display::bttn_states_enum::pmp1]);
	  msg_str += ' ';
	  Error_messaging::num2str(
		  &msg_str,
		  Heiz_display::bttn_states[Heiz_display::bttn_states_enum::pmp2]);
	  msg_str += '\n';
	}

  if (*state == 1)
	msg_str += "Sommer\n";

  if(*state == 2)
	  msg_str += "Manual\n";

  Error_messaging::write(msg_str.c_str());
}


Heiz_display::Heiz_display ()
{
  nexInit();
  b1_Exit.attachPop(b1_Exit_PopCallback, &b1_Exit);
  bt0_pmp1.attachPop(bt0_pmp1_PopCallback, &bt0_pmp1);
  bt0_pmp2.attachPop(bt0_pmp2_PopCallback, &bt0_pmp2);
  b0_modus.attachPop(b0_modus_PopCallback, &b0_modus);

}


void Heiz_display::loop(void)
{


  //n_second.setValue(25);


  nexLoop(nex_listen_list);

}

// Objektliste page 0
// [page id ,component id, component name]
// 0, 13, n_pmp1_curr
// 0, 14, n_paus1_curr
// 0, 15, n_pmp2_curr
// 0, 16, n_paus2_curr
// 0, 10, t_daynight
// 0, 18, n_day
// 0, 19, n_month
// 0, 20, n_year
// 0, 23, n_hour
// 0, 24, n_minute
// 0, 25, n_second
//
// Objektliste page 1
// 1, 22, n_day
// 1, 23, n_month
// 1, 24, n_year
// 1, 27, n_hour
// 1, 28, n_minute
// 1, 29, n_second
// 1, 5, n_pmp1_max
// 1, 6, n_paus1_max
// 1, 7, n_pmp2_max
// 1, 8, n_paus2_max

// Touch releas event exit-button page 1:
/*
get n_pmp1_max.id
get n_pmp1_max.val
get n_pmp2_max.id
get n_pmp2_max.val
get n_paus1_max.id
get n_paus1_max.val
get n_paus2_max.id
get n_paus2_max.val
get n_day.id
get n_day.val
get n_month.id
get n_month.val
get n_year.id
get n_year.val
get n_hour.id
get n_hour.val
get n_min.id
get n_min.val
page 0
 */



