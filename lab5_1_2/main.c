#include "fsm.h"
#include "USART2.h"
#include "USART3.h"
#include "userbutton.h"
#include "WiFi.h"


void USART2_callback_fn(uint8_t byte)
{
	// get current state
	state_t current_state = fsm_get_state();

	// only echo to wifi when in CMD mode
	if (current_state == STATE_ECHO_BYTES || current_state == STATE_RESET)
		USART3_putchar(byte);
}

void USART3_callback_fn(uint8_t byte)
{
	// get current state
	state_t current_state = fsm_get_state();

	// if in CMD mode or reset, continue echoing over USART2
	if (current_state == STATE_ECHO_BYTES || current_state == STATE_RESET)
		USART2_putchar(byte); // echo to wifi module
	else // otherwise, time to receive a ping
		WIFI_recv_ping(byte);
}



void userbutton_callback_fn(void)
{
	if( fsm_lock() == FSM_LOCK_ACQUIRED )
	{
		// get current state
		state_t current_state = fsm_get_state();

		// toggle state between ping and echo bytes
		if (current_state != STATE_PING) {
			USART3_putstr("exit\r");
			fsm_set_state(STATE_PING);
		}
		else
			fsm_set_state(STATE_ECHO_BYTES);

		fsm_unlock();
	}
}


/* Callback function for TIM2 interrupts */
void TIM2_callback_fn(void)
{
	// TODO write TIM2_rx_callback function
}


void main(void)
{
	/* Set up the USART2 9600-8N1 and to call USART2_callback_fn when new data arrives */
	USART2_init(USART2_callback_fn);
	//USART3_init(USART3_callback_fn);

	/* Configure user pushbutton and call pushbutton_callback_fn when button press-released */
	userbutton_init(userbutton_callback_fn);

	/* initialize the finite state machine */
	fsm_init();
	WIFI_send_ping();



	/* Enable interrupts - do this after initializing the system */
	__asm ("  cpsie i \n" );

	//	WIFI_recv_ping(0x01);
	//	WIFI_recv_ping(0x00);
	//	WIFI_recv_ping(0x00);
	//	WIFI_recv_ping(0x00);
	//	WIFI_recv_ping(0x01);
	//	WIFI_recv_ping(0x00);
	//	WIFI_recv_ping(0x00);
	//	WIFI_recv_ping(0x00);

	/* Wait here forever - everything is now interrupt driven */
	while(1)
	{
		;;;
	}
}
