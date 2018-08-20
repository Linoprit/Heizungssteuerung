/*
 * rtc_device.cpp
 *
 *  Created on: 22.07.2018
 *      Author: harald
 */

#include <Devices/Realtime_clock/rtc_device.h>
#include <Instances/Common.h>
#include <System/Error_messaging.h>
#include "stm32f1xx_hal_rtc.h"
#include <../Application/heiz_display.h>
#include "stm32f1xx_hal_rtc_ex.h"
#include "stm32f1xx_hal_rcc.h"



Rtc_device::Rtc_device (RTC_HandleTypeDef*  rtc_handle)
{
  this->rtc_handle = rtc_handle;

  RTC_TimeTypeDef sTime;
  sTime.Hours = 4;
  sTime.Minutes = 5;
  sTime.Seconds = 30;
  HAL_RTC_SetTime(rtc_handle,&sTime, RTC_FORMAT_BIN);

  RTC_DateTypeDef sDate;
  sDate.Date = 2;
  sDate.Month = 8;
  sDate.Year = 18;
  HAL_RTC_SetDate(rtc_handle, &sDate, RTC_FORMAT_BIN);

}

void	Rtc_device::set_time_date(uint32_t* setup_vals)
{
  RTC_TimeTypeDef sTime;
  sTime.Hours   = setup_vals[Heiz_display::hour];
  sTime.Minutes = setup_vals[Heiz_display::minute];
  HAL_RTC_SetTime(rtc_handle, &sTime, RTC_FORMAT_BIN);

  RTC_DateTypeDef sDate;
  sDate.Date  = setup_vals[Heiz_display::day];
  sDate.Month = setup_vals[Heiz_display::month];
  sDate.Year  = setup_vals[Heiz_display::year];
  HAL_RTC_SetDate(rtc_handle, &sDate, RTC_FORMAT_BIN);
}

void Rtc_device::get_time_date(uint32_t* setup_vals)
{
  RTC_TimeTypeDef sTime;
  RTC_DateTypeDef sDate;

  HAL_RTC_GetTime(rtc_handle, &sTime, RTC_FORMAT_BIN);
  HAL_RTC_GetDate(rtc_handle, &sDate, RTC_FORMAT_BIN);

  setup_vals[Heiz_display::hour]   = sTime.Hours   ;
  setup_vals[Heiz_display::minute] = sTime.Minutes ;
  setup_vals[Heiz_display::second] = sTime.Seconds ;
  setup_vals[Heiz_display::day]    = sDate.Date    ;
  setup_vals[Heiz_display::month]  = sDate.Month   ;
  setup_vals[Heiz_display::year]   = sDate.Year    ;

}

void Rtc_device::get_time_date(RTC_TimeTypeDef *sTime, RTC_DateTypeDef *sDate)
{
  HAL_RTC_GetTime(rtc_handle, sTime, RTC_FORMAT_BIN);
  HAL_RTC_GetDate(rtc_handle, sDate, RTC_FORMAT_BIN);
}

uint32_t Rtc_device::calc_next_pumpservice(void)
{
#define SERVICE_WEEKDAY 3
#define SERVICE_HOUR	12
#define SERVICE_MINUTE	00

  uint32_t service_minutes =
	  (SERVICE_WEEKDAY * 24 * 60) + (SERVICE_HOUR * 60) + SERVICE_MINUTE;

  uint32_t current_minutes = get_time_minutes();

  if (current_minutes < service_minutes)
	return service_minutes - current_minutes;

  return WHOLE_WEEK_MINUTES - current_minutes + service_minutes;
}

uint32_t Rtc_device::get_time_minutes(void)
{
  RTC_TimeTypeDef sTime;
  RTC_DateTypeDef sDate;
  HAL_RTC_GetTime(rtc_handle, &sTime, RTC_FORMAT_BIN);
  HAL_RTC_GetDate(rtc_handle, &sDate, RTC_FORMAT_BIN);

  return (sDate.WeekDay * 24 * 60) + (sTime.Hours * 60) + sTime.Minutes;
}


void Rtc_device::save_backup_value(uint32_t Data, uint32_t BackupRegister)
{
  // BackupRegister must be in range of 1 to 10 !!!
  HAL_RTCEx_BKUPWrite(rtc_handle, BackupRegister, Data);
}

uint32_t Rtc_device::read_backup_value(uint32_t BackupRegister)
{
  return HAL_RTCEx_BKUPRead(rtc_handle, BackupRegister);
}

