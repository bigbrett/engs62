#include "fsm.h"
#include "USART2.h"
#include "USART3.h"
#include "userbutton.h"
#include "WiFi.h"
#include "timer.h"


void USART2_callback_fn(uint8_t byte)
{
	//USART3_putchar(byte);
	if( fsm_lock() == FSM_LOCK_ACQUIRED )
	{
		// get current state
		state_t current_state = fsm_get_state();

		// only echo to wifi when in CMD mode
		if (current_state == STATE_ECHO_BYTES || current_state == STATE_RESET)
			USART3_putchar(byte);
		else if (current_state == STATE_GET_ID) {
			USART2_buffer_add(byte); // add to buffer
			USART2_putchar(byte); // echo to screen
			// if buffer is full, got valid ID
			if (USART2_buffer_length() >= 2)
				fsm_set_state(STATE_GOT_ID);
		}




		fsm_unlock();
	}
}

void USART3_callback_fn(uint8_t byte)
{
	//USART2_putchar(byte);
	if( fsm_lock() == FSM_LOCK_ACQUIRED )
	{
		// get current state
		state_t current_state = fsm_get_state();

		// if in CMD mode or reset, continue echoing over USART2
		if (current_state == STATE_ECHO_BYTES || current_state == STATE_RESET)
			USART2_putchar(byte); // echo to console
		else if (current_state == STATE_PING) // if in ping state, receive ping packets
			WIFI_recv_ping(byte);
		else if (current_state == STATE_UPDATE) // if in UPDATE state, receive update packets
			WIFI_recv_update(byte);

		fsm_unlock();
	}
}



void userbutton_callback_fn(void)
{
	if( fsm_lock() == FSM_LOCK_ACQUIRED )
	{
		// get current state
		state_t current_state = fsm_get_state();

		// toggle state between ping and echo bytes
		if (current_state == STATE_RESET) {
			fsm_set_state(STATE_PING);
		}
		else if (current_state == STATE_ECHO_BYTES) {
			USART3_putstr("exit\r");
			fsm_set_state(STATE_PING);
		}
		else if ( current_state == STATE_PING  || current_state == STATE_PING_RECV)
			fsm_set_state(STATE_UPDATE); // fsm_set_state(STATE_ECHO_BYTES);
		else if (current_state == STATE_UPDATE || current_state == STATE_UPDATE_RECV)
			fsm_set_state(STATE_GET_ID);
		else if (current_state == STATE_GET_ID || current_state == STATE_GOT_ID)
			fsm_set_state(STATE_ECHO_BYTES);
		else {
			USART2_putstr("ERROR: BAD STATE IN USERBUTTON \n\r");

		}
		fsm_unlock();
	}
}


/* Callback function for TIM2 interrupts */
void TIM7_callback_fn(void)
{
	LED_toggle(LED_RED_ON);

	if( fsm_lock() == FSM_LOCK_ACQUIRED )
	{
		// get current state
		state_t current_state = fsm_get_state();

		// if in ping or update states, time to receive
		if (current_state == STATE_PING)
			fsm_set_state(STATE_PING_RECV);
		else if (current_state == STATE_UPDATE)
			fsm_set_state(STATE_UPDATE_RECV);
		else // else error
			USART2_putstr("ERROR: BAD STATE IN TIM7 CALLBACK \n\r");
		fsm_unlock();
	}
}


void main(void)
{
	/* Initialize modules */
	USART2_init(USART2_callback_fn);
	USART3_init(USART3_callback_fn);
	LED_init();
	userbutton_init(userbutton_callback_fn);
	fsm_init();
	TIM7_init(TIM7_callback_fn);
	ADC_init();
	tim4_init();

	/* Enable interrupts - do this after initializing the system */
	__asm ("  cpsie i \n" );

	/* Wait here forever - everything is now interrupt driven */
	while(1)
	{
		;;;
	}
}
