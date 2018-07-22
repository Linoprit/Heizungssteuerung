/*
 * SoftwareEvents.cpp
 *
 *  Created on: 28.10.2015
 *      Author: harald
 */

#include <System/Error_messaging.h>
#include <System/SoftwareEvents.h>
#include "stm32f1xx_hal.h"
#include "../gnuarmeclipse/system/include/diag/Trace.h"


simpleRingbuffer swe_ringbuffer(EVENTS_BUFFER_LEN);
SoftwareEvents::EventSet SoftwareEvents::EventTable[EVENTS_TABLE_LEN];


// C interface
void SoftwareEvents_checkTicks(uint32_t sysTick)
{
  SoftwareEvents::checkTicks(sysTick);
}


SoftwareEvents::SoftwareEvents ()
{
  uint8_t i;

  for (i=0; i < EVENTS_TABLE_LEN; i++)
	EventTable[i].class_pointer = NULL;
}


void SoftwareEvents::loop (void)
{
  uint8_t event;

  while (swe_ringbuffer.HasData())
	{
	  event = swe_ringbuffer.Read();
	  execute_callbacks((Event_Names_enum) event);
	}
}


void SoftwareEvents::execute_callbacks(Event_Names_enum event_name)
{
  uint8_t i;

  // TODO does event "all" work?
  for (i=0; i < EVENTS_TABLE_LEN; i++)
	{
	  if ((EventTable[i].class_pointer != NULL) &&
		  (EventTable[i].listening_event_name == event_name) )
		{
		  EventTable[i].class_pointer->
		  software_event_callback(event_name);
		}
	}
}


void SoftwareEvents::sendEvent(Event_Names_enum event_name)
{
  if (event_name > All)
	swe_ringbuffer.Write(event_name);
}


void SoftwareEvents::checkTicks(uint32_t sysTick)
{
  SoftwareEvents::sendEvent(SoftwareEvents::Tic_1ms);

  if (sysTick % 10 == 0)
	{
      SoftwareEvents::sendEvent(SoftwareEvents::Tic_10ms);
	}

  if (sysTick % 100 == 0)
	{
      SoftwareEvents::sendEvent(SoftwareEvents::Tic_100ms);
	}
}


uint8_t SoftwareEvents::hookIn (
	SoftwareEvents_base *class_pointer,
	Event_Names_enum listening_event_name)
{
  uint8_t i;

  for (i=0; i < EVENTS_TABLE_LEN; i++)
	{
	  if (EventTable[i].class_pointer == NULL)
		{
		  EventTable[i].class_pointer        = class_pointer;
		  EventTable[i].listening_event_name = listening_event_name;
		  return i;
		}
	}

  // Use macro, if Trace.c doesn't work
  //WRITE_TRACE ((const char*) "SoftwareEvents::hookIn: Table is full!\n", 39);
  trace_printf((const char*) "SoftwareEvents::hookIn: Table is full!\n");

  return 0xFF; // errorcode
}


void SoftwareEvents::hookOff (
	uint8_t ID,
	SoftwareEvents_base *class_pointer)
{
  if (EventTable[ID].class_pointer == class_pointer)
	EventTable[ID].class_pointer = NULL;
}


void SoftwareEvents::hookOff (
	SoftwareEvents_base *class_pointer,
	Event_Names_enum event_name)
{
  uint8_t i;

  for (i=0; i < EVENTS_TABLE_LEN; i++)
	{
	  if ((EventTable[i].class_pointer == class_pointer) &&
		  (EventTable[i].listening_event_name  == event_name) )
		EventTable[i].class_pointer = NULL;
	}
}












