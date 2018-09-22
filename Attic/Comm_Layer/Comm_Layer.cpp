/*
 * Comm_Layer.cpp
 *
 *  Created on: 11.01.2016
 *      Author: harald
 */

#include "Comm_Layer.h"
#include "libraries/simpleRingbuffer.h"
#include <stm32f1xx_hal_def.h>


Comm_Layer::Comm_Layer (Rx_Tx_interface *comm_socket_in)
{    
  this->comm_socket   = comm_socket_in;
  tx_queue            =
	  new simpleRingbuffer(TX_QUEUE_SIZE * sizeof(tx_message_type));
  rcv_header_ptr	= NULL;

  // TODO create a cycle and call it from scheduler
  SoftwareEvents::hookIn(this, SoftwareEvents::Tic_10ms);
}


void Comm_Layer::software_event_callback(
	SoftwareEvents::Event_Names_enum event_name)
{    
#ifdef QT_TARGET
  Q_UNUSED(event_name);
#else
  UNUSED(event_name);
#endif

  if (comm_socket->get_rx_ringbuffer()->HasData())
	inspect_received_data();

  if (tx_queue->HasData())
	send_from_queue();
}


void Comm_Layer::inspect_received_data(void)
{
  enum state_type
  {
	SYNC = 0, READ_HEADER = 1, COMP_CHECKSUM = 2
  };
  static enum state_type state = SYNC;
  uint16_t i;
  uint8_t __message_buff[MAX_MESSAGE_LEN + HEADER_LENGTH + CHK_SUM_LENGTH];
  uint8_t *message_buff = &__message_buff[0];
  uint8_t buff_1, buff_2;

  switch (state)
  {
	case SYNC:
	  rcv_header_ptr = (uint8_t *) &rcv_header; // reset data pointer

	  for (i = 0; i < comm_socket->get_rx_ringbuffer()->Count(); i++)
		{
		  buff_1 = comm_socket->get_rx_ringbuffer()->Read();
		  buff_2 = comm_socket->get_rx_ringbuffer()->Get();

		  if ((buff_1 == SYNC_CHAR_1) && (buff_2 == SYNC_CHAR_2))
			{
			  // empty read to skip SYNC_CHAR_2
			  comm_socket->get_rx_ringbuffer()->Read();
			  state = READ_HEADER;

			  break;
			}
		}
	  break;

	case READ_HEADER:
	  if (comm_socket->get_rx_ringbuffer()->Count() < HEADER_LENGTH)
		break;

	  // store message id and length
	  for (i = 0; i < HEADER_LENGTH; i++)
		{
		  *rcv_header_ptr = comm_socket->get_rx_ringbuffer()->Read();
		  rcv_header_ptr++;
		}
	  state = COMP_CHECKSUM;

	  break;

	case COMP_CHECKSUM:
	  // avoids getting stuck, if the remain of the message > serial_buffer
	  if (rcv_header.payload_len + CHK_SUM_LENGTH >
		  comm_socket->get_rx_ringbuffer()->buffer_size())
		{
		  state = SYNC;
		}

	  if (comm_socket->get_rx_ringbuffer()->Count() >=
		  (rcv_header.payload_len + CHK_SUM_LENGTH))
		{
		  // part 1: we must copy the header to message_buf, to get the right check sum later
		  rcv_header_ptr = (uint8_t *) &rcv_header;
		  for (i = 0; i < HEADER_LENGTH; i++)
			{
			  message_buff[i] = *rcv_header_ptr;
			  rcv_header_ptr++;
			}

		  // part 2: copy payload from ringbuffer
		  for (i = HEADER_LENGTH;
			  i < (HEADER_LENGTH + rcv_header.payload_len + CHK_SUM_LENGTH);
			  i++)
			{
			  message_buff[i] = comm_socket->get_rx_ringbuffer()->Read();
			}

		  if ((uint8_t) compare_chksum_ibutton(
			  (uint8_t*) &message_buff[0], (rcv_header_struct*) &rcv_header))
			Messages_Base::put_payload_to_struct(message_buff, &rcv_header);

		  state = SYNC;
		}
	  break;

	default:
	  break;
  }
}


void Comm_Layer::send_from_queue(void)
{
  if (!tx_queue->HasData())
	return;

  if (comm_socket->is_TX_pending() == true)
	return;

  tx_message_type tx_message;
  if (get_next_tx_message(&tx_message) == false)
	return;

  comm_socket->send_many_bytes(
	  (uint8_t*) tx_message.ptr_to_message, (uint8_t) tx_message.msg_length);
}


bool Comm_Layer::pack_n_queue_msg(rcv_header_struct *tx_header)
{
  uint8_t i;
  uint16_t  payload_len     = tx_header->payload_len;
  uint8_t*	crc_ptr         =
	  (uint8_t*) tx_header + HEADER_LENGTH + payload_len;


  *crc_ptr = Messages_Base::calc_checksum(tx_header);

  Comm_Layer::tx_message_type tx_message;
  uint8_t*  tx_ptr          = (uint8_t*) &tx_message;
  tx_message.ptr_to_message = (uint8_t*) tx_header - 2; // 2 for sync char
  tx_message.msg_length     =
	  (uint16_t) (2 + HEADER_LENGTH + tx_header->payload_len + 1);

  if(tx_queue->Count() >= (TX_QUEUE_SIZE * sizeof(tx_message_type)))
	return false;

  for(i=0; i<sizeof(tx_message_type); i++)
	{
	  tx_queue->Write(*tx_ptr);
	  tx_ptr++;
	}

  return true;
}


// The caller must make sure, that there is data in the ringbuffer!
bool Comm_Layer::get_next_tx_message(tx_message_type* tx_message_ptr)
{
  uint8_t i;
  uint8_t* tx_raw_ptr = (uint8_t*) tx_message_ptr;

  if(!tx_queue->HasData())
	{
	  return false;
	}

  for(i=0; i < sizeof(tx_message_type); i++)
	{
	  *tx_raw_ptr = tx_queue->Read();
	  tx_raw_ptr++;
	}

  return true;
}


uint8_t Comm_Layer::compare_chksum_ibutton(
	uint8_t* msg_buffer, rcv_header_struct *rcv_header_in)
{  
  uint8_t *msg_buf_crc =
	  msg_buffer + HEADER_LENGTH + rcv_header_in->payload_len;

  uint8_t crc = Messages_Base::calc_checksum(rcv_header_in, msg_buffer);

  //qDebug() << "crc: " << crc << " msgcrc: " << *msg_buf_crc;

  if (crc == *msg_buf_crc)
	return 1;

  return 0;
}

Rx_Tx_interface* Comm_Layer::get_rx_tx_interface(void)
{
  return comm_socket;
}


