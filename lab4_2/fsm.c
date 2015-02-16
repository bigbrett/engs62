#include <fsm.h>

static uint32_t fsm_mutex;
static state_t state = STATE_RESET;

/* Declare control signals and get/set methods */
uint8_t ARRIVING=0, CLOSED=0, HOLD=0, CLEAR=0, PED=0;
uint8_t get_PED(void) {
	return PED;
}
uint8_t get_ARRIVING(void) {
	return ARRIVING;
}
uint8_t get_CLOSED(void) {
	return CLOSED;
}
uint8_t get_CLEAR(void) {
	return CLEAR;
}
void set_PED(uint8_t val) {
	PED = val;
}
void set_ARRIVING(uint8_t val) {
	ARRIVING = val;
}
void set_CLOSED(uint8_t val) {
	CLOSED = val;
}
void set_CLEAR(uint8_t val) {
	CLEAR = val;
}


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
			USART2_putstr("FSM Rest\r\n");
			USART2_putstr("Press 'a' for ARRIVING signal -- train coming\r\n");
			USART2_putstr("Press 'm' or 'h' for HOLD signal -- maintenance\n");
			USART2_putstr("Press 'c' for CLEAR signal -- resume traffic\r\n");
			USART2_putstr("Press 'user button' to signal pedestrian crossing r\n\n");
			break;

		/* No trains, peds, or maintenance
		 * DO: initialize 3 minute traffic timer, set lights to green */
		case STATE_TRAFFIC_INIT:
			// clear all control signals, as they will trip upon an interrupt
			set_HOLD(0);
			set_ARRIVING(0);
			set_CLEAR(0);
			set_PED(0);

			// clear timer count
			set_TIMER(0);

			// start traffic timer for 5 seconds
			systick_set(5); // start traffic timer for 5 seconds

			// green light
			LED_update( LED_ORANGE_OFF | LED_RED_OFF | LED_BLUE_OFF | LED_GREEN_ON);
			break;

		case STATE_TRAFFIC_TIMING:


			break;


		/* No trains, peds, or maintenance
		 * DO: traffic can flow continuously for a minimum of 3 mins */
		case STATE_TRAFFIC_FLOWING:
			LED_UPDATE(LED_GREEN_ON); // ensure light still on
			break;

		/* ARRIVING signal from substation indicates Train is coming.
		 * DO: stop traffic flow, close gate, send CLOSED signal to substation */
		case STATE_TRAIN_INIT:
			close_gate();
			stop_traffic();
			break;

		/* Wait for train to pass
		 * DO: keep everything closed until CLEAR signal from substation */
		case STATE_TRAIN_WAIT:
			/* Turn on the red LED only */
			stop_traffic();
			break;

		/* Pedestrian pushes button
		 * DO: Wait until traffic timer > 3min, stop traffic, let peds cross for 20 seconds, start traffic */
		case STATE_PED_INIT:
			stop_traffic();
			close_gate();
			LED_UPDATE(LED_ORANGE_ON);

			systick_set(1); // start countdown for 20 seconds
			break;

		case STATE_PED_WAIT:

			break;

		/* MAINTENANCE signal from substation indicates maintenance
		 * DO: issue HOLD message, stop traffic, close gates, flash blue light, wait until CLEAR signal */
		case STATE_MAINTENANCE:
				systick_set(1); // set systick for 1 second timer: handler does the blinking
				LED_update(LED_BLUE_ON);
				break;
			}


		}
	}


/************** CONTROL FUNCTIONS **************/
/* FSM method to close gate and update relevant control signals */
void close_gate(void) {
	servo_gateClose(); // move servo motor to 0
	CLOSED = 1;
	USART2_putstr("GATE CLOSED");
}

void open_gate(void) {
	servo_gateOpen(); // move servo motor to 90
	CLOSED = 0;
	USART2_putstr("GATE OPENED");
}

/* FSM method to stop traffic and update necessary signals*/
void stop_Traffic(void) {
	// TODO: eventually integrate a yellow light?
	LED_update( LED_ORANGE_OFF | LED_RED_ON | LED_BLUE_OFF | LED_GREEN_OFF );
	USART2_putstr("TRAFFIC STOPPED");
}

void start_Traffic(void) {
	systick_set(5); // start traffic timer for 5 seconds
	LED_update( LED_ORANGE_OFF | LED_RED_OFF | LED_BLUE_OFF | LED_GREEN_ON);
	USART2_putstr("TRAFFIC FLOW RESUMED");
}

}
