/*
 * SaveValues.h
 *
 *  Created on: 04.11.2019
 *      Author: harald
 */

#ifndef VALUESTORAGE_H_
#define VALUESTORAGE_H_

#include <stdint.h>
#include <Sockets/CrcSocket.h>
#include <Sockets/RtcSocket.h>



class ValueStorage {
public:

	// Times in [min]
	// sizeof must be dividable by 4 (sizeof uint32)
	struct RtcBackupType {
		uint8_t tactiveMax_winter_1;
		uint8_t tpauseMax_winter_1;
		uint8_t tactiveMax_winter_2;
		uint8_t tpauseMax_winter_2;
		uint8_t MainMachineState;
		uint8_t day;
		uint8_t month;
		uint8_t year;
		uint8_t free_1;
		uint8_t free_2;
		uint8_t free_3;
		uint8_t chksum;
	}__attribute__ ((packed));
#define RTC_BACKUP_DATA_SIZE 12 // in bytes

	ValueStorage(CrcSocket *crcSocket, RtcSocket* rtc);
	virtual ~ValueStorage() { };

	void storeDate(uint8_t day, uint8_t month, uint8_t year);
	void storeBlock(ValueStorage::RtcBackupType rtcBackup);
	const RtcBackupType getBlock(void);
	void  getBlock(RtcBackupType* rtcRestore);

private:
	CrcSocket*		_crcSocket;
	RtcSocket* 		_rtcSocket;
	uint8_t calcChecksum(RtcBackupType rtcBackup);
	void fillInDefaultVals(RtcBackupType* rtcBackup);

};

#endif /* VALUESTORAGE_H_ */
