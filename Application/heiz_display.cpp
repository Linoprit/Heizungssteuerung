/*
 * heiz_display.cpp
 *
 *  Created on: 06.06.2018
 *      Author: harald
 */


#include "heiz_display.h"
#include "../Framework/libraries/Arduino/itoa.h"
#include "../Framework/Instances/Common.h"
#include "../Framework/libraries/HelpersLib.h"
#include <stm32f1xx_hal_uart.h>
#include <HeizStateMachine/MainMachine.h>

#include <main.h>


//uint32_t Heiz_display::setup_vals[SETUP_VALS_LEN];
uint32_t Heiz_display::bttn_states[BTTN_STATES_LEN];
char     Heiz_display::date_time_str[DATE_TIME_LEN];
char     Heiz_display::tag_nacht[TAGT_NACHT_LEN];
uint8_t  Heiz_display::curr_page = 0xFF; // 0xFF = init



// Declare objects [page id, component id, component name]

// page 0, display values. pushed from control-task.
NexNumber n0_pmp1_curr  = NexNumber(0, 13, "n0_pmp1_curr");
NexNumber n0_paus1_curr = NexNumber(0, 14, "n0_paus1_curr");
NexNumber n0_pmp2_curr  = NexNumber(0, 15, "n0_pmp2_curr");
NexNumber n0_paus2_curr = NexNumber(0, 16, "n0_paus2_curr");
NexText   t0_daynight   = NexText  (0, 10, "t0_daynight");
NexNumber n0_pmp1_max	= NexNumber(0,  3, "n0_pmp1_max");
NexNumber n0_paus1_max	= NexNumber(0,  7, "n0_paus1_max");
NexNumber n0_pmp2_max	= NexNumber(0,  8, "n0_pmp2_max");
NexNumber n0_paus2_max  = NexNumber(0,  9, "n0_paus2_max");
//NexNumber n0_paus2_sm   = NexNumber(0, 24, "n0_paus2_sm");
NexNumber n0_second     = NexNumber(0, 23, "n0_second");
NexText   t0_date_time  = NexText  (0, 33, "t0_date_time");

NexProgressBar j0_pmp1  = NexProgressBar(0, 2, "j0_pmp1");
NexProgressBar j0_paus1  = NexProgressBar(0, 4, "j0_paus1");
NexProgressBar j0_pmp2  = NexProgressBar(0, 5, "j0_pmp2");
NexProgressBar j0_paus2  = NexProgressBar(0, 6, "j0_paus2");

NexNumber  n1_pmp1_max  = NexNumber(1, 5,  "page1.n1_pmp1_max");
NexNumber  n1_paus1_max = NexNumber(1, 6,  "page1.n1_paus1_max");
NexNumber  n1_pmp2_max  = NexNumber(1, 7,  "page1.n1_pmp2_max");
NexNumber  n1_paus2_max = NexNumber(1, 8,  "page1.n1_paus2_max");

NexVariable va0_modus	= NexVariable(0, 32, "va0_modus");

NexNumber  n2_day       = NexNumber(2, 2, "page2.n2_day");
NexNumber  n2_month     = NexNumber(2, 3, "page2.n2_month");
NexNumber  n2_year      = NexNumber(2, 4, "page2.n2_year");
NexNumber  n2_hour      = NexNumber(2, 7, "page2.n2_hour");
NexNumber  n2_minute    = NexNumber(2, 8, "page2.n2_minute");

NexButton b0_setup      = NexButton(0, 11, 	 "b0_setup");
NexButton b1_Exit	   	= NexButton(1, 9, 	 "b1_Exit");
NexButton b0_timedate   = NexButton(0, 24, 	 "b0_timedate");
NexButton b2_Exit  		= NexButton(2, 1, 	 "b2_Exit");
NexDSButton bt0_pmp1	= NexDSButton(0, 18, "bt0_pmp1");
NexDSButton bt0_pmp2	= NexDSButton(0, 21, "bt0_pmp2");
NexDSButton b0_modus	= NexDSButton(0, 1,  "b0_modus");


NexTouch* nex_listen_list[] =
{
		&b1_Exit,
		&b0_setup,
		&b0_timedate,
		&b2_Exit,
		&bt0_pmp1,
		&bt0_pmp2,
		&b0_modus,
		NULL
};


void b1_Exit_PopCallback(void *ptr)
{
	UNUSED(ptr);

	uint32_t tmpVal = 0;
	MainMachine* mMach = Common::get_mainMachine();
	ValueStorage* vs = Common::get_valueStorage();
	ValueStorage::RtcBackupType rtcStorage = vs->getBlock();

	n1_pmp1_max.getValue(&tmpVal);
	rtcStorage.tactiveMax_winter_1 = static_cast<uint8_t>(tmpVal);
	mMach->set_tactiveMax_winter(0, tmpVal * 6000);

	n1_paus1_max.getValue(&tmpVal);
	rtcStorage.tpauseMax_winter_1  = static_cast<uint8_t>(tmpVal);
	mMach->set_tpauseMax_winter(0,  tmpVal * 60000);

	n1_pmp2_max.getValue(&tmpVal);
	rtcStorage.tactiveMax_winter_2 = static_cast<uint8_t>(tmpVal);
	mMach->set_tactiveMax_winter(1, tmpVal * 60000);

	n1_paus2_max.getValue(&tmpVal);
	rtcStorage.tpauseMax_winter_2  = static_cast<uint8_t>(tmpVal);
	mMach->set_tpauseMax_winter(1,  tmpVal * 60000);

	vs->storeBlock(rtcStorage);

	sendCommand("page 0");
	Heiz_display::curr_page = 0;
}

void bt0_setup_PopCallback(void *ptr)
{
	UNUSED(ptr);

	sendCommand("page 1");
	Heiz_display::curr_page = 1;
}

void b0_timedate_PopCallback(void *ptr)
{
	UNUSED(ptr);
	sendCommand("page 2");
	Heiz_display::curr_page = 2;
}

void b2_Exit_PopCallback(void *ptr)
{
	UNUSED(ptr);

	uint32_t tmpVal = 0;

	ValueStorage* vs = Common::get_valueStorage();
	ValueStorage::RtcBackupType rtcStorage = vs->getBlock();

	n2_day.getValue(&tmpVal);
	rtcStorage.day = static_cast<uint8_t>(tmpVal);

	n2_month.getValue(&tmpVal);
	rtcStorage.month = static_cast<uint8_t>(tmpVal);

	n2_year.getValue(&tmpVal);
	rtcStorage.year = static_cast<uint8_t>(tmpVal);

	vs->storeBlock(rtcStorage);
	Common::get_rtcSocket()->set_date(
			rtcStorage.day, rtcStorage.month, rtcStorage.year);

	n2_hour.getValue(&tmpVal);
	uint8_t hour = static_cast<uint8_t>(tmpVal);

	n2_minute.getValue(&tmpVal);
	uint8_t min = static_cast<uint8_t>(tmpVal);

	Common::get_rtcSocket()->set_time(hour, min, 0u);

	sendCommand("page 0");
	Heiz_display::curr_page = 0;
}

void bt0_pmp1_PopCallback(void *ptr)
{
	UNUSED(ptr);
	MainMachine* mm = Common::get_mainMachine();

	uint32_t* state =
			&Heiz_display::bttn_states[Heiz_display::bttn_state_vals_enum::st_pmp1];

	bt0_pmp1.getValue(state);

	if (*state == 1)
		mm->switch_on_manual(0);
	else
		mm->switch_off_manual(0);
}

void bt0_pmp2_PopCallback(void *ptr)
{
	UNUSED(ptr);
	MainMachine* mm = Common::get_mainMachine();

	uint32_t* state =
			&Heiz_display::bttn_states[Heiz_display::bttn_state_vals_enum::st_pmp2];

	bt0_pmp2.getValue(state);

	if (*state == 1)
		mm->switch_on_manual(1);
	else
		mm->switch_off_manual(1);

}

void b0_modus_PopCallback(void *ptr)
{
	UNUSED(ptr);
	MainMachine* mm  = Common::get_mainMachine();
	ValueStorage* vs = Common::get_valueStorage();
	ValueStorage::RtcBackupType rtcStorage = vs->getBlock();

	MainMachine::enm_states currState = mm->get_state();

	if (currState == MainMachine::winter) {
		mm->set_state(MainMachine::summer);
		rtcStorage.MainMachineState = MainMachine::summer;
	}
	else if (currState == MainMachine::summer) {
		mm->set_state(MainMachine::manual);
		rtcStorage.MainMachineState = MainMachine::manual;
	}
	else if (currState == MainMachine::manual) {
		mm->set_state(MainMachine::winter);
		rtcStorage.MainMachineState = MainMachine::winter;
		Common::get_heizDisplay()->inactivate_pump_bttns();
	}

	Common::get_heizDisplay()->upd_page_0();
	vs->storeBlock(rtcStorage);
}


Heiz_display::Heiz_display ()
{
	b1_Exit.attachPop (b1_Exit_PopCallback, 	&b1_Exit);
	b0_setup.attachPop(bt0_setup_PopCallback, 	&b0_setup);
	bt0_pmp1.attachPop(bt0_pmp1_PopCallback, 	&bt0_pmp1);
	bt0_pmp2.attachPop(bt0_pmp2_PopCallback, 	&bt0_pmp2);
	b0_modus.attachPop(b0_modus_PopCallback, 	&b0_modus);
	b0_timedate.attachPop(b0_timedate_PopCallback, &b0_timedate);
	b2_Exit.attachPop(b2_Exit_PopCallback, 		&b1_Exit);
}

void Heiz_display::loop(void)
{
	static bool init_disp_vals = false;

	nexLoop(nex_listen_list);

	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;
	Common::get_rtcSocket()->get_time_date(&sTime, &sDate);

	ValueStorage* vs = Common::get_valueStorage();
	ValueStorage::RtcBackupType rtcStorage = vs->getBlock();

	// initialize
	if (Heiz_display::curr_page == 0xFF) {
		nexInit();
		Heiz_display::curr_page = 0;
		upd_page_0();

		n0_pmp1_max.setValue( rtcStorage.tactiveMax_winter_1);
		n0_paus1_max.setValue(rtcStorage.tpauseMax_winter_1 );
		n0_pmp2_max.setValue( rtcStorage.tactiveMax_winter_2);
		n0_paus2_max.setValue(rtcStorage.tpauseMax_winter_2 );
	}

	if (Heiz_display::curr_page == 0) {
		init_disp_vals = false;

		upd_page_0();
		n0_second.setValue(sTime.Seconds);

		// store changed date at 0:00h
		if ( (sTime.Minutes == 0) && (sTime.Hours == 0) && (sTime.Seconds == 0) ) {
			Common::get_valueStorage()->storeDate(
					sDate.WeekDay, sDate.Month, sDate.Year);
		}
	}

	if ( (Heiz_display::curr_page == 2) && (init_disp_vals == false) ) {
		init_disp_vals = true;


		n2_day.setValue(sDate.WeekDay);
		n2_month.setValue(sDate.Month);
		n2_year.setValue(sDate.Year);
		n2_hour.setValue(sTime.Hours);
		n2_minute.setValue(sTime.Minutes);
	}
}

void Heiz_display::upd_page_0(void)
{
	upd_mode_bttn_text();
	upd_dayNight_text();
	upd_time_date_str();
	upd_op_times();
}

void Heiz_display::upd_op_times(void)
{
	MainMachine* mm = Common::get_mainMachine();

	MainMachine::OpTimesType opTimes = mm->get_opTimes(0);
	n0_pmp1_curr. setValue((opTimes.tactive_winter + 59900) / 60000 );
	n0_paus1_curr.setValue((opTimes.tpause_winter  + 59900) / 60000 );

	if (opTimes.state == StateMachineInterface::active) {
		j0_pmp1.Set_font_color_pco(5862u);
		j0_paus1.Set_font_color_pco(31u);
	}
	else if (opTimes.state == StateMachineInterface::pause) {
		j0_pmp1.Set_font_color_pco(31u);
		j0_paus1.Set_font_color_pco(5862u);
	}
	else if ( (opTimes.state == StateMachineInterface::ready)
			|| (opTimes.state == StateMachineInterface::init) ) {
		j0_pmp1.Set_font_color_pco(31u);
		j0_paus1.Set_font_color_pco(31u);
	}

	opTimes = mm->get_opTimes(1);
	n0_pmp2_curr. setValue((opTimes.tactive_winter + 59900) / 60000 );
	n0_paus2_curr.setValue((opTimes.tpause_winter  + 59900) / 60000 );

	if (opTimes.state == StateMachineInterface::active) {
		j0_pmp2.Set_font_color_pco(5862u);
		j0_paus2.Set_font_color_pco(31u);
	}
	else if (opTimes.state == StateMachineInterface::pause) {
		j0_pmp2.Set_font_color_pco(31u);
		j0_paus2.Set_font_color_pco(5862u);
	}
	else if ( (opTimes.state == StateMachineInterface::ready)
			|| (opTimes.state == StateMachineInterface::init) ) {
		j0_pmp2.Set_font_color_pco(31u);
		j0_paus2.Set_font_color_pco(31u);
	}
}

void Heiz_display::inactivate_pump_bttns(void)
{
	bt0_pmp1.setValue(0);
	bt0_pmp2.setValue(0);
	bttn_states[st_pmp1] = 0;
	bttn_states[st_pmp2] = 0;
}

void Heiz_display::upd_mode_bttn_text(void)
{
	MainMachine* mm = Common::get_mainMachine();

	if (mm->get_state() == MainMachine::winter)
		b0_modus.setText("Winter");
	if (mm->get_state() == MainMachine::summer)
		b0_modus.setText("Sommer");
	if (mm->get_state() == MainMachine::manual)
		b0_modus.setText("Manual");
}

void Heiz_display::upd_dayNight_text(void)
{
	MainMachine* mm = Common::get_mainMachine();

	if (mm->isNightMode() == true )
		t0_daynight.setText("Nacht");
	if (mm->isNightMode() == false)
		t0_daynight.setText("Tag");
}

void  Heiz_display::upd_time_date_str(void)
{
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;
	Common::get_rtcSocket()->get_time_date(&sTime, &sDate);

	String time_str = time_date_to_str(sTime, sDate);
	t0_date_time.setText(time_str.c_str());
}

String	Heiz_display::time_date_to_str(
		RTC_TimeTypeDef sTime, RTC_DateTypeDef sDate)
{
	String time_str;

	if(sDate.WeekDay == 1)
		time_str += "Mo ";
	if(sDate.WeekDay == 2)
		time_str += "Di ";
	if(sDate.WeekDay == 3)
		time_str += "Mi ";
	if(sDate.WeekDay == 4)
		time_str += "Do ";
	if(sDate.WeekDay == 5)
		time_str += "Fr ";
	if(sDate.WeekDay == 6)
		time_str += "Sa ";
	if(sDate.WeekDay == 0)
		time_str += "So ";

	if (sDate.Date < 10)
		time_str += "0";
	HelpersLib::num2str(&time_str, (uint32_t) sDate.Date);
	if (sDate.Month< 10)
		time_str += ".0";
	else
		time_str += ".";
	HelpersLib::num2str(&time_str, (uint32_t) sDate.Month);
	if (sDate.Year< 10)
		time_str += ".200";
	else
		time_str += ".20";
	HelpersLib::num2str(&time_str, (uint32_t) sDate.Year);
	time_str += "  ";

	if (sTime.Hours < 10)
		time_str += "0";
	HelpersLib::num2str(&time_str, (uint32_t) sTime.Hours);
	if (sTime.Minutes < 10)
		time_str += ":0";
	else
		time_str += ":";
	HelpersLib::num2str(&time_str, (uint32_t) sTime.Minutes);
	time_str += ":";

	return time_str;
}

bool Heiz_display::get_bttn_pmp1(void)
{
	if( Common::get_heizDisplay()->get_bttn_states()[Heiz_display::st_pmp1] > 0)
		return true;

	return false;
}

bool Heiz_display::get_bttn_pmp2(void)
{
	if( Common::get_heizDisplay()->get_bttn_states()[Heiz_display::st_pmp2] > 0)
		return true;

	return false;
}

