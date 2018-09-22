/*
 * Messages_Base.cpp
 *
 *  Created on: 03.02.2016
 *      Author: harald
 */

#include "Messages_Base.h"


command_type 		Messages_Base::command_rx;


#ifdef  COMMLAYER_TEST_ENABLE
msg_test_01_type 	Messages_Base::msg_test_01_tx;
test_01_type 		Messages_Base::test_01_rx;
msg_test_02_type 	Messages_Base::msg_test_02_tx;
test_02_type 		Messages_Base::test_02_rx;
msg_test_03_type 	Messages_Base::msg_test_03_tx;
test_03_type 		Messages_Base::test_03_rx;
#endif

Messages_Base::Messages_Base ()
{ }

void Messages_Base::init()
{
  // prepare tx message headers
#ifdef  COMMLAYER_TEST_ENABLE
  INIT_MSG(msg_test_01_tx, COMMLAYER_TEST_CLASS, TEST_MSG_01_ID);
  INIT_MSG(msg_test_02_tx, COMMLAYER_TEST_CLASS, TEST_MSG_02_ID);
  INIT_MSG(msg_test_03_tx, COMMLAYER_TEST_CLASS, TEST_MSG_03_ID);
#endif







}

void Messages_Base::put_payload_to_struct(
	uint8_t *msg_buf, rcv_header_struct *rcv_header)
{
  uint8_t *pointer = 0;

  if (rcv_header->msg_class == POWER_SUPPLY_CLASS)
	{
	  switch (rcv_header->msg_id)
	  {
		case COMMAND_MSG_ID: // must use define!
		  pointer = (uint8_t *) &Messages_Base::command_rx;
		  break;

		  // ...more POWER_SUPPLY_CLASS messages

		default:
		  pointer = 0;
		  break;
	  }
	}

#ifdef  COMMLAYER_TEST_ENABLE
  if (rcv_header->msg_class == COMMLAYER_TEST_CLASS)
	{
	  switch (rcv_header->msg_id)
	  {
		case TEST_MSG_01_ID:
		  pointer = (uint8_t *) &Messages_Base::test_01_rx;
		  break;
		case TEST_MSG_02_ID:
		  pointer = (uint8_t *) &Messages_Base::test_02_rx;
		  break;
		case TEST_MSG_03_ID:
		  pointer = (uint8_t *) &Messages_Base::test_03_rx;
		  break;


		default:
		  break;
	  }
	}
#endif

  if (pointer != 0)
	{
	  for (unsigned int i = 0; i < rcv_header->payload_len; i++)
		{
		  *pointer = msg_buf[i + HEADER_LENGTH]; // payload is behind header
		  pointer++;
		}
	}
}


uint8_t Messages_Base::calc_checksum(rcv_header_struct *rcv_header)
{
  uint8_t* msg_buffer = (uint8_t*) rcv_header;
  return calc_checksum(rcv_header, msg_buffer);
}


uint8_t Messages_Base::calc_checksum(
	rcv_header_struct *rcv_header,
	uint8_t* msg_buffer)
{
  // use avr algorithm
  uint8_t i, crc = 0;

  // we must exclude the chksum byte at the end!
  for (i = 0; i < HEADER_LENGTH + rcv_header->payload_len; i++)
	{
	  crc = _crc_ibutton_update(crc, *msg_buffer);
	  msg_buffer++;
	}
  return crc;
}


//#ifdef QT_TARGET
// TODO use hardware crc
uint8_t _crc_ibutton_update(uint8_t crc, uint8_t data)
{
  uint8_t i;

  crc = crc ^ data;
  for (i = 0; i < 8; i++)
	{
	  if (crc & 0x01)
		crc = (crc >> 1) ^ 0x8C;
	  else
		crc >>= 1;
	}

  return crc;
}
//#endif
