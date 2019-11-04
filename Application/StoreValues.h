/*
 * SaveValues.h
 *
 *  Created on: 04.11.2019
 *      Author: harald
 */

#ifndef STOREVALUES_H_
#define STOREVALUES_H_

class StoreValues {
public:

	// Times in [min]
	struct RtcBackupType {
		uint8_t tactiveMax_winter_1;
		uint8_t tpauseMax_winter_1;
		uint8_t tactiveMax_winter_2;
		uint8_t tpauseMax_winter_2;
		uint8_t day;
		uint8_t month;
		uint8_t year;
		uint8_t MainMachineState;
		uint8_t chksum;
	}__attribute__ ((packed));


	StoreValues();
	virtual ~StoreValues() { };

	void storeBlock(void);
	void restoreBlock(void);
	const RtcBackupType getBlockConst(void);
	RtcBackupType* getBlock(void);

private:
	RtcBackupType block;

	uint8_t calcCheckSum(void);
};

#endif /* STOREVALUES_H_ */
