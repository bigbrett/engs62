#include "fsm.h"
#define DEBUG 1

static uint32_t fsm_mutex;
static state_t state = STATE_RESET;
static int id = 20;


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

int fsm_getID()
{
	return id;
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

		case STATE_ECHO_BYTES:
			TIM7_kill();

			// Turn on the blue LED only
			LED_update( LED_ORANGE_OFF | LED_RED_OFF | LED_BLUE_ON | LED_GREEN_OFF );

			// Enter CMD mode
			USART3_putstr("$$$");
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
			id = 20; // set to my id
			// print last value and remember last state
			WIFI_recv_print();
			fsm_set_state(STATE_PING);
			break;

		case STATE_UPDATE:
			// Turn on the green LED
			LED_update( LED_ORANGE_OFF | LED_RED_OFF | LED_BLUE_OFF | LED_GREEN_ON);

			// set id to my id
			id = 20;

			// send potentiometer value over USART3
			WIFI_send_update(ADC_getData());

			// Set timer to generate a single interrupt in 1 second //
			TIM7_1_sec();
			break;

		case STATE_UPDATE_RECV:
			WIFI_recv_print();
			WIFI_servoCtrl(id); // update servo motor position
			fsm_set_state(STATE_UPDATE);
			break;

		case STATE_GET_ID:
			TIM7_kill();
			// Turn on the red LED
			LED_update( LED_ORANGE_OFF | LED_RED_ON | LED_BLUE_OFF | LED_GREEN_OFF);
			USART2_buffer_clr();
			USART2_putstr("ENTER ID NUMBER > ");
			break;

		case STATE_GOT_ID:
			// Turn on the red LED
			LED_update( LED_ORANGE_OFF | LED_RED_ON | LED_BLUE_OFF | LED_GREEN_OFF);
			id = USART2_buffer_getInt();
			// send potentiometer value over USART3
			//WIFI_send_update(ADC_getData());
			TIM7_1_sec(); //fsm_set_state(STATE_ID_RECV); // TODO issues?
			break;

		case STATE_HAVE_ID:
			// Turn on the red LED
			LED_update( LED_ORANGE_OFF | LED_RED_ON | LED_BLUE_OFF | LED_GREEN_OFF);
			TIM7_1_sec(); //fsm_set_state(STATE_ID_RECV); // TODO issues?
			break;

		case STATE_ID_RECV:
			//TIM7_kill();
			// Turn on the red LED
			LED_update( LED_ORANGE_OFF | LED_RED_ON | LED_BLUE_OFF | LED_GREEN_OFF);
			WIFI_recv_print();
			WIFI_servoCtrl(id); // update servo motor position
			fsm_set_state(STATE_HAVE_ID);
			break;

		}
	}
}
