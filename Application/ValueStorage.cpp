/*
 * SaveValues.cpp
 *
 *  Created on: 04.11.2019
 *      Author: harald
 */

#include <ValueStorage.h>
#include <HeizStateMachine/MainMachine.h>

ValueStorage::ValueStorage(CrcSocket *crcSocket, RtcSocket* rtcSocket) {
	_crcSocket  	= crcSocket;
	_rtcSocket		= rtcSocket;

	RtcBackupType rtcRestore = this->getBlock();
	uint8_t crc = calcChecksum(rtcRestore);
	if (crc != rtcRestore.chksum) {
		fillInDefaultVals(&rtcRestore);
		storeBlock(rtcRestore);
	}

	// restore date
	_rtcSocket->set_date(rtcRestore.day, rtcRestore.month, rtcRestore.year);
}

void ValueStorage::storeDate(uint8_t day, uint8_t month, uint8_t year) {
	RtcBackupType rtcBackup = getBlock();
	rtcBackup.day		= day;
	rtcBackup.month		= month;
	rtcBackup.year		= year;

	storeBlock(rtcBackup);
	_rtcSocket->set_date(rtcBackup.day, rtcBackup.month, rtcBackup.year);
}


void ValueStorage::storeBlock(ValueStorage::RtcBackupType rtcBackup) {
	uint8_t crc = calcChecksum(rtcBackup);
	rtcBackup.chksum = crc;

	_rtcSocket->save_backup_struct(reinterpret_cast<uint16_t*>(&rtcBackup),
			RTC_BACKUP_DATA_SIZE );

	_rtcSocket->set_date(rtcBackup.day, rtcBackup.month, rtcBackup.year);
}

void ValueStorage::getBlock(RtcBackupType* rtcRestore) {
	_rtcSocket->read_backup_struct(reinterpret_cast<uint16_t*>(rtcRestore),
			RTC_BACKUP_DATA_SIZE /2 );
}

const ValueStorage::RtcBackupType ValueStorage::getBlock(void) {
	RtcBackupType rtcRestore;
	_rtcSocket->read_backup_struct(reinterpret_cast<uint16_t*>(&rtcRestore),
			RTC_BACKUP_DATA_SIZE / 2 );

	return rtcRestore;
}

uint8_t ValueStorage::calcChecksum(RtcBackupType rtcBackup) {
	rtcBackup.chksum = 0;
	uint32_t* buffer = reinterpret_cast<uint32_t*>( &rtcBackup );
	return 	_crcSocket->calcU8(buffer, RTC_BACKUP_DATA_SIZE / 4);
}

void ValueStorage::fillInDefaultVals(RtcBackupType* rtcBackup) {
	rtcBackup->tactiveMax_winter_1 = 5u;
	rtcBackup->tpauseMax_winter_1  = 15u;
	rtcBackup->tactiveMax_winter_2 = 5u;
	rtcBackup->tpauseMax_winter_2  = 20u;
	rtcBackup->day                 = 26u;
	rtcBackup->month               = 4u;
	rtcBackup->year                = 20u;
	rtcBackup->MainMachineState    = MainMachine::winter;
}


/*
 	ValueStorage* vs = Common::get_valueStorage();

	ValueStorage::RtcBackupType rtcStorage = vs->getBlock();
	tx_printf("startup:\n");
	tx_printf("%i %i %i %i ",
			rtcStorage.tactiveMax_winter_1 ,
			rtcStorage.tpauseMax_winter_1  ,
			rtcStorage.tactiveMax_winter_2 ,
			rtcStorage.tpauseMax_winter_2  );

	tx_printf("%i %i %i %i \n",
			rtcStorage.day                 ,
			rtcStorage.month               ,
			rtcStorage.year                ,
			rtcStorage.MainMachineState    );

		rtcStorage = vs->getBlock();

		rtcStorage.tactiveMax_winter_1 = rtcStorage.tactiveMax_winter_1 + 1;
		rtcStorage.tpauseMax_winter_1  = rtcStorage.tpauseMax_winter_1  + 1;
		rtcStorage.tactiveMax_winter_2 = rtcStorage.tactiveMax_winter_2 + 1;
		rtcStorage.tpauseMax_winter_2  = rtcStorage.tpauseMax_winter_2  + 1;
		rtcStorage.day                 = rtcStorage.day                 + 1;
		rtcStorage.month               = rtcStorage.month               + 1;
		rtcStorage.year                = rtcStorage.year                + 1;
		rtcStorage.MainMachineState    = rtcStorage.MainMachineState    + 1;

		vs->storeBlock(rtcStorage);



		tx_printf("cycle:\n");
		tx_printf("%i %i %i %i ",
				rtcStorage.tactiveMax_winter_1 ,
				rtcStorage.tpauseMax_winter_1  ,
				rtcStorage.tactiveMax_winter_2 ,
				rtcStorage.tpauseMax_winter_2  );

		tx_printf("%i %i %i %i \n",
				rtcStorage.day                 ,
				rtcStorage.month               ,
				rtcStorage.year                ,
				rtcStorage.MainMachineState    );

 */
