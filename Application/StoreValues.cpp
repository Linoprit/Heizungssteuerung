/*
 * SaveValues.cpp
 *
 *  Created on: 04.11.2019
 *      Author: harald
 */

#include <StoreValues.h>

StoreValues::StoreValues() {

}

void StoreValues::storeBlock(void) {
}

void StoreValues::restoreBlock(void) {
}

const StoreValues::RtcBackupType StoreValues::getBlockConst(void) {
}

StoreValues::RtcBackupType* StoreValues::getBlock(void) {
}
/*

 		RtcSocket* rtc = Common::get_rtcSocket();
		RtcBackupType rtcBackup;
		rtcBackup.tactiveMax_winter_1 = 11;
		rtcBackup.tpauseMax_winter_1  = 22;
		rtcBackup.tactiveMax_winter_2 = 33;
		rtcBackup.tpauseMax_winter_2  = 44;
		rtcBackup.day                 = 55;
		rtcBackup.month               = 66;
		rtcBackup.year                = 77;
		rtcBackup.MainMachineState    = 88;

		rtc->save_backup_struct(reinterpret_cast<uint16_t*>(&rtcBackup),
				sizeof(RtcBackupType) );

		RtcBackupType rtcRestore;
		rtc->read_backup_struct(reinterpret_cast<uint16_t*>(&rtcRestore),
				sizeof(RtcBackupType) );

			tx_printf("%i %i %i %i ",
				rtcRestore.tactiveMax_winter_1 ,
				rtcRestore.tpauseMax_winter_1  ,
				rtcRestore.tactiveMax_winter_2 ,
				rtcRestore.tpauseMax_winter_2  );

		tx_printf("%i %i %i %i ",
				rtcRestore.day                 ,
				rtcRestore.month               ,
				rtcRestore.year                ,
				rtcRestore.MainMachineState    );



 */
