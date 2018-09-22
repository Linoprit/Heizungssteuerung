/*
 * scheduler.cpp
 *
 *  Created on: 09.12.2016
 *      Author: harald
 */

#include "scheduler.h"
#include "stm32f1xx_hal.h"
#include "SoftwareEvents.h"
#include "../Devices/Comm_Layer/Comm_Layer.h"
#include "../Devices/Comm_Layer/Messages_Base.h"
#include "../Instances/Common.h"


#ifdef DO_TESTS
#include "../Tests/Call_Tests.h"
#endif /* DO_TESTS */

// TODO remove later?
#include "System/Error_messaging.h"
#include "libraries/Arduino/itoa.h"
#include "libraries/Arduino/WString.h"
#include "stm32f1xx_hal.h"


//#include "Tests/Call_Tests.h"


uint32_t scheduler::old_tick = 0;



// C interface
void scheduler_cycle(void) 	{ scheduler::cycle(); }
void scheduler_init(void)	{ scheduler::init();  }

scheduler::scheduler (void) { }
scheduler::~scheduler (void) { }


void scheduler::init(void)
{
  Common::init();
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);

#ifdef DO_TESTS
  Call_Tests *tests = new Call_Tests();
  tests->cycle();
#endif /* DO_TESTS */
}


void scheduler::cycle(void)
{
  uint32_t tick = HAL_GetTick();

  Common::get_sw_events()->loop(); // work off events


  // 1ms cycle
  if (tick != old_tick)
	{

	 /* UART_HandleTypeDef 	*usart = Common::nex_disp->get_socket()->get_handle();
	  HAL_UART_StateTypeDef uart_status = HAL_UART_GetState(usart);

	  if (uart_status == HAL_UART_STATE_RESET)
		Error_messaging::write("HAL_UART_STATE_RESET\n");
	  else if (uart_status == HAL_UART_STATE_READY)
		Error_messaging::write("HAL_UART_STATE_READY\n");
	  else if (uart_status == HAL_UART_STATE_BUSY)
		Error_messaging::write("HAL_UART_STATE_BUSY\n");
	  else if (uart_status == HAL_UART_STATE_BUSY_TX)
		Error_messaging::write("HAL_UART_STATE_BUSY_TX\n");
	  //else if (uart_status == HAL_UART_STATE_BUSY_RX)
		//Error_messaging::write("HAL_UART_STATE_BUSY_RX\n");
	  else if (uart_status == HAL_UART_STATE_BUSY_TX_RX)
		Error_messaging::write("HAL_UART_STATE_BUSY_TX_RX\n");
	  else if (uart_status == HAL_UART_STATE_TIMEOUT)
		Error_messaging::write("HAL_UART_STATE_TIMEOUT\n");
	  else if (uart_status == HAL_UART_STATE_ERROR)
		Error_messaging::write("HAL_UART_STATE_ERROR\n");*/

	}

  // 5ms cycle
  if ( ((tick % 5) == 0) && (tick != old_tick) )
	{

	}

  // 50ms cycle
  if ( ((tick % 50) == 0) && (tick != old_tick) )
	{



	  /*
	  // LED ON
	  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

	  // LED OFF
	  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
	   */

	}

  if ( ((tick % 500) == 0) && (tick != old_tick) )
	{
	  //trace_write ("Message from STM32\n", 19);
	  //HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
	  Common::heiz_disp->loop();


	  /*  simpleRingbuffer* rx_buffer =
		  Common::nex_disp->get_socket()->get_rx_ringbuffer();

	 uint8_t rb_count = rx_buffer->Count();
	  char buff[5] = {0};
	  itoa(rb_count, buff, 10 );
	  String msg;
	  msg += "received: ";
	  msg += buff;
	  msg += "\n";
	  Error_messaging::write(msg.c_str());

	  char rec_buff[40] = {0};
	  for(uint8_t i=0; i<rb_count; i++)
		{
		  rec_buff[i] = rx_buffer->Read();
		}
	  String msg2;
	  msg2 += rec_buff;
	  msg2 += "\n";
	  Error_messaging::write(msg2.c_str());*/

	  /*char buff[20] = {0};
	  uint8_t count;
	  count = Common::nex_disp->readBytes(buff, 20);*/

	  /* char nbuff[5] = {0};
	  itoa(count, nbuff, 10 );

	  String msg;
	  msg += nbuff;
	  msg += " received: ";
	  msg += buff;
	  msg += "\n";*/
	  //Error_messaging::write(buff);



	}


  old_tick = tick;
}








