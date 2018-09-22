/*
 * State_Machine.h
 *
 *  Created on: 07.08.2018
 *      Author: harald
 */

#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

#include "stdint.h"


#define OP_TIMES_LEN     4


class State_Machine
{
public:

  enum modus_enum {
	mod_winter=0, mod_sommer=1, mod_manual=2
  };

  enum betrieb_enum {
	bet_tag, bet_nacht
  };

  enum state_enum {
	st_pmp1, st_paus1, st_pmp2, st_paus2
  };

  enum op_times_enum {
	op_pmp1, op_pmp2, op_paus1, op_paus2
  };

  State_Machine ();
  virtual ~State_Machine () {};

  void loop(void);
  modus_enum   get_modus(void);
  void		   inc_modus(void);
  betrieb_enum get_betrieb(void);
  void 		   inc_target_time(void);
  void         get_op_times(uint32_t* values)	;

  void next_state(void);
  void state_loop(void);
  void state_stop(void);

  void pump_1_on(void);
  void pump_1_off(void);
  void pump_2_on(void);
  void pump_2_off(void);


private:
  modus_enum   	curr_modus;
  modus_enum 	old_modus;
  state_enum 	curr_state;
  state_enum	old_state;

  uint32_t		*curr_time;
  uint32_t 		op_times[OP_TIMES_LEN];
  uint32_t		target_time;


};

#endif /* STATE_MACHINE_H_ */
