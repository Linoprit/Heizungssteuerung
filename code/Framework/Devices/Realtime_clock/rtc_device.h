/*
 * rtc_device.h
 *
 *  Created on: 22.07.2018
 *      Author: harald
 */

#ifndef DEVICES_REALTIME_CLOCK_RTC_DEVICE_H_
#define DEVICES_REALTIME_CLOCK_RTC_DEVICE_H_

#include "stm32f1xx_hal.h"
#include "libraries/Arduino/WString.h"

#define WHOLE_WEEK_MINUTES 10080

class Rtc_device
{
public:

  Rtc_device (RTC_HandleTypeDef*  rtc_handle);
  virtual ~Rtc_device () { };

  void	 set_time_date(uint32_t* setup_vals);
  void   get_time_date(uint32_t* setup_vals);
  void   get_time_date(RTC_TimeTypeDef *sTime, RTC_DateTypeDef *sDate);
  uint32_t get_time_minutes(void);
  uint32_t calc_next_pumpservice(void);

  void 		save_backup_value(uint32_t Data, uint32_t BackupRegister);
  uint32_t 	read_backup_value(uint32_t BackupRegister);


private:
  RTC_HandleTypeDef*  rtc_handle;


};

#endif /* DEVICES_REALTIME_CLOCK_RTC_DEVICE_H_ */
