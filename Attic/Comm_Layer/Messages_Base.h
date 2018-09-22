/*
 * Messages_Base.h
 *
 *  Created on: 03.02.2016
 *      Author: harald
 */

#ifndef SYSTEM_COMM_LAYER_MESSAGES_BASE_H_
#define SYSTEM_COMM_LAYER_MESSAGES_BASE_H_

#include <stdint.h>

#ifdef QT_TARGET
#include <QDebug>
#define COMMLAYER_TEST_ENABLE
#else
#define COMMLAYER_TEST_ENABLE
//#include <util/crc16.h>
//#include "../../ApplicationModules/PPM_InOut/PPM_bitbanging.h"
#endif


#define	 PACKED	__attribute__ ((packed))

#define SYNC_CHAR_1          0 // DEC
#define SYNC_CHAR_2        255 // DEC
#define CHK_SUM_LENGTH       1
#define MAX_MESSAGE_LEN     30 // Maximum bytelength of received message


#ifdef QT_TARGET
uint8_t _crc_ibutton_update(uint8_t crc, uint8_t data);
#else
// TODO use hardware crc
uint8_t _crc_ibutton_update(uint8_t crc, uint8_t data);
#endif


//------------ < do not change > ------------
// header part of serial messages
typedef struct PACKED
{
  uint8_t  msg_class      : 8;
  uint8_t  msg_id         : 8;
  uint16_t payload_len    : 16;
} rcv_header_struct;
const uint8_t HEADER_LENGTH = sizeof(rcv_header_struct);

#define MSG_STRUCT(payload_type)						\
	typedef struct PACKED								\
	{													\
	uint8_t               sync_char[2];					\
	rcv_header_struct 	  header;						\
	payload_type 	      payload;						\
	uint8_t               checksum;						\
	} msg_##payload_type;								\
	const uint8_t MSG_##payload_type##_LEN = sizeof(msg_##payload_type)

#define INIT_MSG(msg_instance, CLASS_ID, MSG_ID) 		      	\
	msg_instance.sync_char[0]       = SYNC_CHAR_1; 	      		\
	msg_instance.sync_char[1]       = SYNC_CHAR_2; 	      		\
	msg_instance.header.msg_class   = CLASS_ID;   		  		\
	msg_instance.header.msg_id      = MSG_ID;       		  	\
	msg_instance.header.payload_len =							\
	  sizeof(msg_instance) - 2 - HEADER_LENGTH - 1;
//------------ </ do not change > ------------

#define POWER_SUPPLY_CLASS		5

// message class command_type
enum command_enum { write2eeprom, send_stored_params };
#define COMMAND_MSG_ID		1
typedef struct PACKED
{
  command_enum     command;
} command_type;
//MSG_STRUCT(command_type); only needed for tx

// ...more POWER_SUPPLY_CLASS messages


// test message class
#ifdef  COMMLAYER_TEST_ENABLE
#define COMMLAYER_TEST_CLASS    254

#define TEST_MSG_01_ID        	1
typedef struct PACKED
{
  uint32_t    counter;
  uint16_t    value_2;
  int16_t     value_4;
  int8_t      value_3;
} test_01_type;
MSG_STRUCT(test_01_type);

#define TEST_MSG_02_ID        	2
typedef struct PACKED
{
  uint32_t    counter;
  uint16_t    value_2;
  int16_t     value_4;
  int8_t      value_3;
} test_02_type;
MSG_STRUCT(test_02_type);

#define TEST_MSG_03_ID        	3
typedef struct PACKED
{
  uint32_t    counter;
  uint16_t    value_2;
  int16_t     value_4;
  int8_t      value_3;
} test_03_type;
MSG_STRUCT(test_03_type);

#endif





class Messages_Base
{
public:
  static command_type		command_rx;



  Messages_Base ();

  // we use init() instead the constructor in static classes
  static void init();

  static void 	 put_payload_to_struct(
	  uint8_t *msg_buf, rcv_header_struct *rcv_header);

  static uint8_t calc_checksum(
	  rcv_header_struct *rcv_header, uint8_t *msg_buffer);

  static uint8_t calc_checksum(
	  rcv_header_struct *rcv_header);


#ifdef  COMMLAYER_TEST_ENABLE
  static msg_test_01_type 	msg_test_01_tx;
  static test_01_type 		test_01_rx;

  static msg_test_02_type 	msg_test_02_tx;
  static test_02_type 		test_02_rx;

  static msg_test_03_type 	msg_test_03_tx;
  static test_03_type 		test_03_rx;
#endif
};

#endif /* SYSTEM_COMM_LAYER_MESSAGES_BASE_H_ */


// just to remember...
/*typedef struct PACKED
{
  uint8_t               sync_char[2];
  rcv_header_struct 	header;
  ppm_command_type 	payload;
  uint8_t               checksum;
} msg_ppm_command_type;
const uint8_t MSG_PPM_COMMAND_LEN = sizeof(msg_ppm_command_type);
const uint8_t PPM_COMMAND_LEN = sizeof(ppm_command_type);*/

