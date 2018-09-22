/*
 * Comm_Layer.h
 *
 *  Created on: 11.01.2016
 *      Author: harald
 */

#ifndef SYSTEM_COMM_LAYER_H_
#define SYSTEM_COMM_LAYER_H_

#ifndef QT_TARGET
//#include <util/crc16.h>
#include "System/SoftwareEvents.h"
#else
#include "system/SoftwareEvents.h"
#endif
#include <stdint.h>
#include <Interfaces/RX_TX_interface.h>
#include "Instances/callbacks.h"
#include "Messages_Base.h"


#define INLINE __attribute__((always_inline))
#define	 PACKED	__attribute__ ((packed))

#define TX_QUEUE_SIZE 5


class Comm_Layer:
	SoftwareEvents_base
{

public:
	  struct tx_message_type
	  {
		uint8_t*  ptr_to_message;
		uint16_t  msg_length;
	  };

	  Comm_Layer (Rx_Tx_interface *comm_socket);

	  void inspect_received_data(void);
	  bool pack_n_queue_msg(rcv_header_struct *tx_header);
	  uint8_t compare_chksum_ibutton(
		  uint8_t* msg_buffer, rcv_header_struct* rcv_header);
	  Rx_Tx_interface* get_rx_tx_interface(void);

	  // cycle
	  void software_event_callback(
		  SoftwareEvents::Event_Names_enum event_name);

private:
	  uint8_t           *rcv_header_ptr;
	  simpleRingbuffer  *tx_queue;
	  Rx_Tx_interface   *comm_socket;
	  rcv_header_struct rcv_header;

	  bool get_next_tx_message(tx_message_type *tx_message_ptr);
	  void send_from_queue(void);

};

#endif /* SYSTEM_COMM_LAYER_H_ */
