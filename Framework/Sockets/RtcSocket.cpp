/*
 * RtcSocket.cpp
 *
 *  Created on: 05.10.2019
 *      Author: harald
 */

#include "RtcSocket.h"
#include "../Framework/Instances/Common.h"
#include "../Application/heiz_display.h"



// is not a class member
bool operator ==(const RTC_TimeTypeDef left, const RTC_TimeTypeDef right) {
	if( (right.Seconds == left.Seconds)
			&& (right.Minutes == left.Minutes)
			&& (right.Hours   == left.Hours) 	)
	{
		return true;
	}

	return false;
}

RtcSocket::RtcSocket(RTC_HandleTypeDef*  rtc_handle) {
	this->rtc_handle = rtc_handle;

	RTC_DateTypeDef sDate;
	sDate.Date = 20;
	sDate.Month = 8;
	sDate.Year = 18;
	HAL_RTC_SetDate(rtc_handle, &sDate, RTC_FORMAT_BIN);
}

void	RtcSocket::set_time_date(uint32_t* setup_vals)
{
	set_time(setup_vals);
	set_date(setup_vals);
}

void	RtcSocket::set_time(uint32_t* setup_vals)
{
	RTC_TimeTypeDef sTime;
	sTime.Hours   = setup_vals[Heiz_display::hour];
	sTime.Minutes = setup_vals[Heiz_display::minute];
	HAL_RTC_SetTime(rtc_handle, &sTime, RTC_FORMAT_BIN);
}

void	RtcSocket::set_date(uint32_t* setup_vals)
{
	RTC_DateTypeDef sDate;
	sDate.Date  = setup_vals[Heiz_display::day];
	sDate.Month = setup_vals[Heiz_display::month];
	sDate.Year  = setup_vals[Heiz_display::year];
	HAL_RTC_SetDate(rtc_handle, &sDate, RTC_FORMAT_BIN);

	save_backup_value( sDate.Year,  BACKUP_REG_YEAR);
	save_backup_value( sDate.Month, BACKUP_REG_MONTH);
	save_backup_value( sDate.Date,  BACKUP_REG_DAY);
}

void	RtcSocket::restore_date(uint32_t* setup_vals)
{
	setup_vals[Heiz_display::day]   = read_backup_value(BACKUP_REG_DAY );
	setup_vals[Heiz_display::month] = read_backup_value(BACKUP_REG_MONTH);
	setup_vals[Heiz_display::year]  = read_backup_value(BACKUP_REG_YEAR);

	RTC_DateTypeDef sDate;
	sDate.Date =  read_backup_value(BACKUP_REG_DAY );
	sDate.Month = read_backup_value(BACKUP_REG_MONTH);
	sDate.Year  = read_backup_value(BACKUP_REG_YEAR);
	HAL_RTC_SetDate(rtc_handle, &sDate, RTC_FORMAT_BIN);
}

void RtcSocket::get_time_date(uint32_t* setup_vals)
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

void RtcSocket::get_time_date(RTC_TimeTypeDef *sTime, RTC_DateTypeDef *sDate)
{
	HAL_RTC_GetTime(rtc_handle, sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(rtc_handle, sDate, RTC_FORMAT_BIN);
}

RTC_DateTypeDef RtcSocket::get_date(void) {
	RTC_DateTypeDef sDate;
	HAL_RTC_GetDate(rtc_handle, &sDate, RTC_FORMAT_BIN);
	return sDate;
}

RTC_TimeTypeDef RtcSocket::get_time(void) {
	RTC_TimeTypeDef sTime;
	HAL_RTC_GetTime(rtc_handle, &sTime, RTC_FORMAT_BIN);
	return sTime;
}

uint32_t RtcSocket::get_time_minutes(void) {
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;
	HAL_RTC_GetTime(rtc_handle, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(rtc_handle, &sDate, RTC_FORMAT_BIN);

	return (sDate.WeekDay * 24 * 60) + (sTime.Hours * 60) + sTime.Minutes;
}

void RtcSocket::save_backup_value(uint32_t Data, uint8_t BackupRegister)
{
	if (BackupRegister > BACKUP_REGISTER_LEN )
		BackupRegister = BACKUP_REGISTER_LEN ;

	HAL_RTCEx_BKUPWrite(rtc_handle, BackupRegister, Data);
}

uint32_t RtcSocket::read_backup_value(uint8_t BackupRegister)
{
	if (BackupRegister > BACKUP_REGISTER_LEN )
		BackupRegister = BACKUP_REGISTER_LEN ;

	return HAL_RTCEx_BKUPRead(rtc_handle, BackupRegister);
}

void RtcSocket::save_backup_struct(uint16_t* const Data, uint8_t lenInBytes) {
	if (lenInBytes > (BACKUP_REGISTER_LEN * 2) )
		lenInBytes = (BACKUP_REGISTER_LEN * 2);

	for (uint8_t i=0; i < lenInBytes; i++) {
		HAL_RTCEx_BKUPWrite(rtc_handle, i+1, Data[i]);
	}
}

void RtcSocket::read_backup_struct(uint16_t* Data, uint8_t lenInBytes) {
	if (lenInBytes > (BACKUP_REGISTER_LEN * 2))
		lenInBytes = (BACKUP_REGISTER_LEN * 2);

	for (uint8_t i=0; i < lenInBytes; i++) {
		Data[i] = HAL_RTCEx_BKUPRead(rtc_handle, i+1);
	}
}

RTC_DateTypeDef RtcSocket::operator =(const	RTC_DateTypeDef right) {
	RTC_DateTypeDef result;
	result.Date    = right.Date;
	result.Month   = right.Month;
	result.Year    = right.Year;
	result.WeekDay = right.WeekDay;

	return result;
}

RTC_TimeTypeDef RtcSocket::operator =(const RTC_TimeTypeDef right) {
	RTC_TimeTypeDef result;
	result.Seconds = right.Seconds;
	result.Minutes = right.Minutes;
	result.Hours   = right.Hours;

	return result;
}


