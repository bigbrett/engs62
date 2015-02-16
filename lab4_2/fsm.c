#include "fsm.h"
#define DEBUG0 1
#define DEBUG1 1

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
uint8_t get_HOLD(void) {
	return HOLD;
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
void set_HOLD(uint8_t val) {
	HOLD = val;
}
void echo_signals(void) {
	if (DEBUG1) {
		USART2_putstr("Signal Values:");
		if (DEBUG0)
			USART2_putstr("\n\r\tCTR: "); print_hex(get_CTR());
		USART2_putstr("\n\r\tARRIVING: "); print_hex(get_ARRIVING());
		USART2_putstr("\n\r\tCLEAR: "); print_hex(get_CLEAR());
		USART2_putstr("\n\r\tHOLD: "); print_hex(get_HOLD());
		USART2_putstr("\n\r\tCLOSED: "); print_hex(get_CLOSED());
		USART2_putstr("\n\r\tPED: "); print_hex(get_PED());
		USART2_putstr("\n\r");
	}
}

/************** CONTROL FUNCTIONS **************/
/* FSM method to close gate and update relevant control signals */
void close_gate(void) {
	servo_gateClose(); // move servo motor to 0
	CLOSED = 1;
	USART2_putstr("GATE CLOSED\r\n");
}
void open_gate(void) {
	servo_gateOpen(); // move servo motor to 90
	CLOSED = 0;
	USART2_putstr("GATE OPENED\r\n");
}
/* FSM method to stop traffic and update necessary signals*/
void stop_traffic(void) {
	// TODO: eventually integrate a yellow light?
	LED_update( LED_ORANGE_OFF | LED_RED_ON | LED_BLUE_OFF | LED_GREEN_OFF );
	USART2_putstr("TRAFFIC STOPPED\r\n");
}
void start_traffic(void) {
	systick_set(5); // start traffic timer for 5 seconds
	LED_update( LED_ORANGE_OFF | LED_RED_OFF | LED_BLUE_OFF | LED_GREEN_ON);
	USART2_putstr("TRAFFIC FLOW RESUMED\r\n");
}
/************************************************/


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
			set_CTR(0);

			/* Display usage information */
			USART2_putstr("STATE: RESET\r\n");
			USART2_putstr("Press 'a' for ARRIVING signal -- train coming\r\n");
			USART2_putstr("Press 'm' or 'h' for HOLD signal -- maintenance\r\n");
			USART2_putstr("Press 'c' for CLEAR signal -- resume traffic\r\n");
			USART2_putstr("Press 'user button' to signal pedestrian crossing \r\n");
			fsm_set_state(STATE_TRAFFIC_INIT);
			break;

			/* No trains, peds, or maintenance
			 * DO: initialize 3 minute traffic timer, set lights to green */
		case STATE_TRAFFIC_INIT:
			set_CTR(0); //reset systick timer
			open_gate();
			start_traffic();

			// clear all control signals, as they will trip upon an interrupt
			set_CLOSED(0);
			set_CLEAR(0);

			USART2_putstr("STATE: TRAFFIC_INIT\r\n");
			echo_signals();
			break;

		case STATE_TRAFFIC_TIMING:
			set_CLOSED(0);

			USART2_putstr("STATE: TRAFFIC_TIMING\r\n");
			echo_signals();
			break;


			/* No trains, peds, or maintenance
			 * DO: traffic can flow continuously for a minimum of 3 mins */
		case STATE_TRAFFIC_FLOWING:
			//LED_update( LED_ORANGE_OFF | LED_RED_OFF | LED_BLUE_OFF | LED_GREEN_ON);
			start_traffic();
			set_CLOSED(0);

			USART2_putstr("STATE: TRAFFIC_FLOWING\r\n");
			echo_signals();
			break;

			/* ARRIVING signal from substation indicates Train is coming.
			 * DO: stop traffic flow, close gate, send CLOSED signal to substation */
		case STATE_TRAIN_INIT:
			close_gate();
			stop_traffic();
			set_CLOSED(1);

			USART2_putstr("STATE: TRAIN_INIT\r\n");
			echo_signals();

			break;

			/* Wait for train to pass
			 * DO: keep everything closed until CLEAR signal from substation */
		case STATE_TRAIN_WAIT:
			stop_traffic();
			close_gate();

			USART2_putstr("STATE: TRAIN_WAIT\r\n");
			echo_signals();
			/* Turn on the red LED only */
			break;


			/* Pedestrian pushes button
			 * DO: Wait until traffic timer > 3min, stop traffic, let peds cross for 20 seconds, start traffic */
		case STATE_PED_INIT:
			open_gate();
			stop_traffic();
			LED_update( LED_ORANGE_ON | LED_RED_OFF | LED_BLUE_OFF | LED_GREEN_OFF);
			systick_set(1); // start countdown for 20 seconds

			USART2_putstr("STATE: PED_INIT\r\n");
			echo_signals();
			break;

		case STATE_PED_WAIT:
			USART2_putstr("STATE: PED_WAIT\r\n");
			echo_signals();
			break;

			/* MAINTENANCE signal from substation indicates maintenance
			 * DO: issue HOLD message, stop traffic, close gates, flash blue light, wait until CLEAR signal */
		case STATE_MAINTENANCE_INIT:
			close_gate();
			stop_traffic();
			set_CLOSED(1);
			set_CTR(0); // initialize systick counter

			USART2_putstr("STATE: MAINTENANCE_INIT\r\n");
			echo_signals();
			systick_set(1); // set systick for 1 second timer: handler does the blinking

			break;

		case STATE_MAINTENANCE:
			close_gate();
			// if CTR==0, turn ON LED
			if (get_CTR() == 0) {
				LED_update(LED_BLUE_ON | LED_RED_ON);
				inc_CTR();
			}
			// IF CTR==1, turn OFF LED
			else if (get_CTR() == 1) {
				LED_update(LED_BLUE_OFF | LED_RED_ON);
				inc_CTR();
			}
			// if CTR >= 2, reset CTR and turn ON blue LED
			else if (get_CTR() == 2){
				LED_update(LED_BLUE_ON | LED_RED_ON);
				inc_CTR();
			}
			else {
				LED_update(LED_BLUE_OFF | LED_RED_ON);
				set_CTR(0);
			}

			USART2_putstr("STATE: MAINTENANCE\r\n");
			echo_signals();

			break;
		}


	}
}

