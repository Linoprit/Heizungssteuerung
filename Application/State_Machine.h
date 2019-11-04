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

/*
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
	op_pmp1, op_paus1, op_pmp2, op_paus2
  };

  State_Machine ();
  virtual ~State_Machine () {};

  void check_pumps(void);
  void check_tables(void);
  void check_act_time(void);

  void loop(void);
  void state_pmp1(void);
  void state_paus1(void);
  void state_pmp2(void);
  void state_paus2(void);

  modus_enum   get_modus(void);
  void		   inc_modus(void);
  betrieb_enum get_betrieb(void);
  state_enum   get_state(void);
  void 		   inc_target_time(void);
  void         get_op_times(uint32_t* values)	;
  void		   next_state(void);

  void pump_1_on(void);
  void pump_1_off(void);
  void pump_2_on(void);
  void pump_2_off(void);


private:
  modus_enum   	act_modus;
  modus_enum 	old_modus;
  state_enum 	act_state;

  uint32_t		*act_time;
  uint32_t 		remaining_times[OP_TIMES_LEN];
  uint32_t		*time_set; // point to setup_vals, summer or winter
  uint32_t		target_time;
  bool			is_entry;


};
*/

#endif /* STATE_MACHINE_H_ */
