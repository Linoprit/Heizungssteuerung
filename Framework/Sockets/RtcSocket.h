/*
 * RtcSocket.h
 *
 *  Created on: 05.10.2019
 *      Author: harald
 */

#ifndef SOCKETS_RTCSOCKET_H_
#define SOCKETS_RTCSOCKET_H_

#include "stm32f1xx_hal.h"
#include "../Framework/libraries/Arduino/WString.h"

#define WHOLE_WEEK_MINUTES 10080
#define BACKUP_REG_YEAR       10
#define BACKUP_REG_MONTH       9
#define BACKUP_REG_DAY         8

// TODO handle other CPUs
#define BACKUP_REGISTER_LEN  10 // F103C8T6: 10 x uint16_t
/*
how to test:
 RTC_HandleTypeDef*  rtc_handle = get_rtc();
 for (uint8_t i=0; i < 30; i++) {
 	uint16_t wData = 22;
 	uint16_t rData = 0;

 	HAL_RTCEx_BKUPWrite(rtc_handle, i+1, wData);
 	rData = HAL_RTCEx_BKUPRead(rtc_handle, i+1);

 	if (rData != wData)
 		tx_printf("diff at: %i / w: %i / r: %i\n", i, wData, rData);
 }
*/

// must be defined outside the class
bool operator ==(const RTC_TimeTypeDef right, const RTC_TimeTypeDef left);

class RtcSocket {
public:
	RtcSocket(RTC_HandleTypeDef*  rtc_handle);
	virtual ~RtcSocket() { };

	void set_time_date(uint32_t* setup_vals);
	void set_date(uint32_t* setup_vals);
	void set_time(uint32_t* setup_vals);
	void restore_date(uint32_t* setup_vals);

	void get_time_date(uint32_t* setup_vals);
	void get_time_date(RTC_TimeTypeDef *sTime, RTC_DateTypeDef *sDate);
	RTC_DateTypeDef   get_date(void);
	RTC_TimeTypeDef   get_time(void);

	uint32_t get_time_minutes(void); // since sunday, 0:00;

	RTC_DateTypeDef operator = (const RTC_DateTypeDef right);
	RTC_TimeTypeDef operator = (const RTC_TimeTypeDef right);

	void 		save_backup_value(uint32_t Data, uint8_t BackupRegister);
	uint32_t 	read_backup_value(uint8_t BackupRegister);
	void 		save_backup_struct(uint16_t* const Data, uint8_t lenInBytes);
	void 		read_backup_struct(uint16_t* Data, uint8_t lenInBytes);


private:
	RTC_HandleTypeDef*  rtc_handle;

};

#endif /* SOCKETS_RTCSOCKET_H_ */
