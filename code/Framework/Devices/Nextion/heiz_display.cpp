/*
 * heiz_display.cpp
 *
 *  Created on: 06.06.2018
 *      Author: harald
 */


#include <Devices/Nextion/heiz_display.h>
#include "libraries/Arduino/itoa.h"
#include "Instances/Common.h"


// Declare objects [page id, component id, component name].
NexNumber n_pmp1_curr  = NexNumber(0, 13, "n_pmp1_curr");
NexNumber n_paus1_curr = NexNumber(0, 14, "n_paus1_curr");
NexNumber n_pmp2_curr  = NexNumber(0, 15, "n_pmp2_curr");
NexNumber n_paus2_curr = NexNumber(0, 16, "n_paus2_curr");
NexNumber t_daynight   = NexNumber(0, 10, "t_daynight");
NexNumber n_day        = NexNumber(0, 18, "n_day");
NexNumber n_month      = NexNumber(0, 19, "n_month");
NexNumber n_year       = NexNumber(0, 20, "n_year");
NexNumber n_hour       = NexNumber(0, 23, "n_hour");
NexNumber n_minute     = NexNumber(0, 24, "n_minute");
NexNumber n_second     = NexNumber(0, 25, "n_second");

// TODO define as second page
/*NexNumber  n_day       = NexNumber(1, 22, "n_day");
NexNumber  n_month     = NexNumber(1, 23, "n_month");
NexNumber  n_year      = NexNumber(1, 24, "n_year");
NexNumber  n_hour      = NexNumber(1, 27, "n_hour");
NexNumber  n_minute    = NexNumber(1, 28, "n_minute");
NexNumber  n_second    = NexNumber(1, 29, "n_second");*/
NexNumber  n_pmp1_max  = NexNumber(1, 5,  "n_pmp1_max");
NexNumber  n_paus1_max = NexNumber(1, 6,  "n_paus1_max");
NexNumber  n_pmp2_max  = NexNumber(1, 7,  "n_pmp2_max");
NexNumber  n_paus2_max = NexNumber(1, 8,  "n_paus2_max");

NexButton b1_Exit	   = NexButton(1, 9, "b1_Exit");

NexTouch* nex_listen_list[] =
	{
		&b1_Exit,
		NULL
	};


void b1_Exit_PopCallback(void *ptr)
{
  UNUSED(ptr);

  Error_messaging::write("\n\ncallback\n");


 /* char* buf;
  buf = (char*) malloc( 32 );

  simpleRingbuffer*	display_rbuf =
	  Common::nex_disp->get_socket()->get_rx_ringbuffer();

  char nbuf[10] = {0};
  utoa(display_rbuf->Count(), nbuf, 10);
  Error_messaging::write(nbuf, 10);


  for(uint8_t i=0; i < display_rbuf->Count(); i++)
	{
	  buf[i] = display_rbuf->Read();
	}

  Error_messaging::write(buf, 32);
*/

}

Heiz_display::Heiz_display ()
{
  nexInit();
  b1_Exit.attachPop(b1_Exit_PopCallback, &b1_Exit);

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



