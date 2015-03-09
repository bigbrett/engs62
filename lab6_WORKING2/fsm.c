#include "fsm.h"
#define DEBUG 1

static uint32_t fsm_mutex;
static state_t state = STATE_RESET;
static int id = 20;
static uint32_t avg = 0;

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
			break;

		case STATE_ECHO_BYTES:
			TIM7_kill();

			LED_update( LED_ORANGE_OFF | LED_RED_OFF | LED_BLUE_ON | LED_GREEN_OFF );

			// Enter CMD mode
			USART3_putstr("$$$");
			break;

		case STATE_PING:
			LED_update( LED_ORANGE_ON | LED_RED_OFF | LED_BLUE_OFF | LED_GREEN_OFF );

			// Send Ping
			WIFI_send_ping();

			// Set timer to generate a single interrupt in 1 second //
			TIM7_1_sec();
			break;

		case STATE_PING_RECV:
			id = 20; // set to my id
			WIFI_recv_print(); // print last value
			fsm_set_state(STATE_PING); // send again
			break;

		case STATE_DMA:
			LED_update( LED_ORANGE_OFF | LED_RED_OFF | LED_BLUE_OFF | LED_GREEN_ON);
			ADC_start();
			break;

		case STATE_UPDATE:
			LED_update( LED_ORANGE_OFF | LED_RED_OFF | LED_BLUE_OFF | LED_GREEN_ON);
			id = 20; // set id to my id

			WIFI_send_update(ADC_getData()); // send potentiometer value over USART3
			TIM7_1_sec(); // Set timer to generate a single interrupt in 1 second //
			break;

		case STATE_UPDATE_RECV:
			id = 20; // set id to my id
			WIFI_recv_print(); // print last value
			WIFI_servoCtrl(id); // update servo motor position
			fsm_set_state(STATE_DMA);
			//fsm_set_state(STATE_UPDATE); // receive next update
			break;

		case STATE_GET_ID:
			TIM7_kill(); // kill timer
			LED_update( LED_ORANGE_OFF | LED_RED_ON | LED_BLUE_OFF | LED_GREEN_OFF);
			USART2_buffer_clr(); // clear buffer
			USART2_putstr("ENTER ID NUMBER > "); // prompt user for USART2 inupt
			break;

		case STATE_GOT_ID:
			LED_update( LED_ORANGE_OFF | LED_RED_ON | LED_BLUE_OFF | LED_GREEN_OFF);
			id = USART2_buffer_getInt(); // store input ID from buffer
			WIFI_send_update(ADC_getData()); // send potentiometer value over USART3
			TIM7_1_sec(); // set timer for 1 second
			break;

		case STATE_HAVE_ID:
			LED_update( LED_ORANGE_OFF | LED_RED_ON | LED_BLUE_OFF | LED_GREEN_OFF);
			WIFI_send_update(ADC_getData()); // send update (in order to get new data)
			TIM7_1_sec(); //fsm_set_state(STATE_ID_RECV); //
			break;

		case STATE_ID_RECV:
			LED_update( LED_ORANGE_OFF | LED_RED_ON | LED_BLUE_OFF | LED_GREEN_OFF);
			WIFI_recv_print(); // print received values
			WIFI_servoCtrl(id); // update servo motor position
			fsm_set_state(STATE_HAVE_ID); // get ready to send/receive update again
			break;
		}
	}
}
