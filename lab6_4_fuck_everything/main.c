#include "fsm.h"
#include "USART2.h"
#include "USART3.h"
#include "userbutton.h"
#include "WiFi.h"
#include "timer.h"
#include "ADC.h"
#include "DMA.h"

int i =0;

/* Callback function for USART2 interrupts */
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

/* Callback function for USART3 interrupts */
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
		// current packet ready to be buffered
		else if (current_state == STATE_PING)
			WIFI_recv_ping(byte);
		// current packet ready to be buffered
		else if (current_state == STATE_UPDATE || current_state == STATE_GOT_ID
				|| current_state == STATE_HAVE_ID || current_state == STATE_ID_RECV) // if in UPDATE state, receive update packets
			WIFI_recv_update(byte);

		fsm_unlock();
	}
}

/* Callback function for Userbutton interrupts */
void userbutton_callback_fn(void)
{
	if( fsm_lock() == FSM_LOCK_ACQUIRED )
	{
		// get current state
		state_t current_state = fsm_get_state();

		// if in reset, go to PING
		if (current_state == STATE_RESET) {
			fsm_set_state(STATE_PING);
		}
		// if in CMD mode, go to ping
		else if (current_state == STATE_ECHO_BYTES) {
			USART3_putstr("exit\r");
			fsm_set_state(STATE_PING);
		}
		// if pinging, go to update
		else if ( current_state == STATE_PING  || current_state == STATE_PING_RECV)
			fsm_set_state(STATE_UPDATE); // fsm_set_state(STATE_ECHO_BYTES);
		// if updating, go to ID_update
		else if (current_state == STATE_UPDATE || current_state == STATE_UPDATE_RECV)
			fsm_set_state(STATE_GET_ID);
		// if ID_updating go back to CMD mode
		else if (current_state == STATE_GET_ID || current_state == STATE_GOT_ID
				|| current_state == STATE_ID_RECV || current_state == STATE_HAVE_ID)
			fsm_set_state(STATE_ECHO_BYTES);
		else {
			USART2_putstr("ERROR: BAD STATE IN USERBUTTON \n\r");

		}
		fsm_unlock();
	}
}

/* Callback function for ADC interrupts */
void ADC_callback_fn(uint16_t* ADCbuffer, uint32_t buffer_size)
{
	//	if( fsm_lock() == FSM_LOCK_ACQUIRED )
	//	{
//	int i=0;
//	for (i=0; i<buffer_size; i++) {
//		USART2_putstr(int2str(i));
//		USART2_putstr(": ");
//		USART2_putstr(int2str(ADCbuffer[i]));
//		USART2_putstr("\n\r");
//	}
	ADC_printBuffer();
	ADC_init(ADC_callback_fn);
	//ADC_start();
	//	}
}


/* Callback function for TIM2 interrupts */
void TIM7_callback_fn(void)
{
	LED_toggle(LED_RED_ON);

	if( fsm_lock() == FSM_LOCK_ACQUIRED )
	{
		// get current state
		state_t current_state = fsm_get_state();

		// timer will always signify transition to respective mode's receive state
		if (current_state == STATE_PING)
			fsm_set_state(STATE_PING_RECV);
		else if (current_state == STATE_UPDATE)
			fsm_set_state(STATE_UPDATE_RECV);
		else if (current_state == STATE_GOT_ID || current_state == STATE_HAVE_ID)
			fsm_set_state(STATE_ID_RECV);
		else // else error
			USART2_putstr("ERROR: BAD STATE IN TIM7 CALLBACK \n\r");
		fsm_unlock();
	}
}


void main(void)
{
	/* Initialize modules */
	USART2_init(USART2_callback_fn);
	//	USART3_init(USART3_callback_fn);
	//	LED_init();
	//	userbutton_init(userbutton_callback_fn);
	//	fsm_init();
	//	TIM7_init(TIM7_callback_fn);
	ADC_init(ADC_callback_fn);
	ADC_start();
	//	tim4_init();

	/* Enable interrupts - do this after initializing the system */
	__asm ("  cpsie i \n" );

	/* Wait here forever - everything is now interrupt driven */
	while(1)
	{
		;;;
	}
}
