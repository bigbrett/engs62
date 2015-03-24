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
		else // otherwise, time to receive a ping
			WIFI_recv_ping(byte);

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
		else if (current_state == STATE_RECV || current_state == STATE_PING)
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

		if (current_state == STATE_PING)
			fsm_set_state(STATE_RECV);
		else if (current_state == STATE_RECV)
			fsm_set_state(STATE_PING);
		else
			USART2_putstr("ERROR: BAD STATE IN TIM7 CALLBACK \n\r");

		fsm_unlock();
	}
}


void main(void)
{
	/* Set up the USART2 9600-8N1 and to call USART2_callback_fn when new data arrives */
	USART2_init(USART2_callback_fn);
	LED_init();
	USART3_init(USART3_callback_fn);


	/* Configure user pushbutton and call pushbutton_callback_fn when button press-released */
	userbutton_init(userbutton_callback_fn);

	/* initialize the finite state machine */
	fsm_init();

	/* Initialize basic timer */
	TIM7_init(TIM7_callback_fn);
//	TIM7_1_sec();

	/* Enable interrupts - do this after initializing the system */
	__asm ("  cpsie i \n" );

	/* Wait here forever - everything is now interrupt driven */
	while(1)
	{
		;;;
	}
}
