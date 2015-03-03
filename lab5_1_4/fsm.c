#include <fsm.h>
#define DEBUG 1

static uint32_t fsm_mutex;

static state_t state = STATE_RESET;

void fsm_init(void)
{
	mutex_init(&fsm_mutex);
	while( fsm_lock() != FSM_LOCK_ACQUIRED );
	fsm_set_state(STATE_RESET);
	fsm_unlock();
}

uint8_t fsm_lock(void)
{
	uint8_t retval = FSM_LOCK_FAILED;

	if( mutex_lock(&fsm_mutex) == MUTEX_LOCKED )
		retval = FSM_LOCK_ACQUIRED;

	return retval;
}

void fsm_unlock(void)
{
	mutex_unlock(&fsm_mutex);
}

state_t fsm_get_state(void)
{
	return state;
}

void fsm_set_state(state_t new_state)
{
	/* only do this if the FSM has been locked! */
	if( fsm_mutex == MUTEX_LOCKED )
	{
		state = new_state;

		switch( state )
		{
		case STATE_RESET:
		default:
			/* Initialize the LEDs */
			LED_init();

			/* Display usage information */
			USART2_putstr("Press the user button to toggle between CMD mode and PING mode\n\r");

			/* Turn off all of the LEDs */
			LED_update( LED_ORANGE_ON | LED_RED_ON | LED_BLUE_ON | LED_GREEN_ON );
			//TIM7_kill();
			break;

		case STATE_BUTTON_PRESS:
			break;

		case STATE_DEBOUNCE:
			break;

		case STATE_DEBOUNCE_WAIT:
			break;

		case STATE_ECHO_BYTES:
			TIM7_kill();

			// Turn on the blue LED only
			LED_update( LED_ORANGE_OFF | LED_RED_OFF | LED_BLUE_ON | LED_GREEN_OFF );

			// Enter CMD mode
			if (!DEBUG) USART3_putstr("$$$");
			break;

		case STATE_PING:
			// Turn on the orange LED
			LED_update( LED_ORANGE_ON | LED_RED_OFF | LED_BLUE_OFF | LED_GREEN_OFF );

			// Send Ping
			WIFI_send_ping();

			// Set timer to generate a single interrupt in 1 second //
			TIM7_1_sec();
			break;

		case STATE_PING_RECV:
			// print last value and remember last state
			WIFI_recv_print();
			fsm_set_state(STATE_PING);
			break;

		case STATE_UPDATE:
			// Turn on the green LED
			LED_update( LED_ORANGE_OFF | LED_RED_OFF | LED_BLUE_OFF | LED_GREEN_ON);

			// send potentiometer value over USART3
			WIFI_send_update(ADC_getData());

			// Set timer to generate a single interrupt in 1 second //
			TIM7_1_sec();
			break;

		case STATE_UPDATE_RECV:
			WIFI_recv_print();
			fsm_set_state(STATE_UPDATE);

			break;
		}
	}
}
