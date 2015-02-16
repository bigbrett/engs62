#include "stdint.h"
#include "fsm.h"
#include "USART2.h"
#include "userbutton.h"
#include "timer.h"
#include "LED.h"


/* Callback function for USART 2 interrupts
   NOTE : (USART functions as substation) */
void USART2_callback_fn(uint8_t input)
{
	if (fsm_lock() == FSM_LOCK_ACQUIRED )
	{

		switch( fsm_get_state() ) {

		case STATE_RESET:

		case STATE_TRAFFIC_TIMING:
			if(input == 'a') {
				set_ARRIVING(1); // set arriving control signal
				fsm_set_state(STATE_TRAIN_INIT);
			}
			else if (input == 'm' || input == 'h') {
				set_HOLD(1); // set hold control signal
				fsm_set_state(STATE_MAINTENANCE);
			}
			break;

		case STATE_TRAFFIC_FLOWING:
			if(input == 'a') {
				set_ARRIVING(1); // set arriving control signal
				fsm_set_state(STATE_TRAIN_INIT);
			}
			else if (input == 'm' || input == 'h') {
				set_HOLD(1); // set hold control signal
				fsm_set_state(STATE_MAINTENANCE);
			}
			break;

		case STATE_MAINTENANCE:
			if (input == 'c') {
				set_CLEAR(1); // clear maintenance
				fsm_set_state(STATE_TRAIN_INIT);
			}
			break;

		default:
			break;

		}
	}
}
/* Callback function for UserButton Interrupts */
void userbutton_callback_fn(void)
{
	if (fsm_lock() == FSM_LOCK_ACQUIRED )
	{
		set_PED(1); // crosswalk signal goes high
		switch( fsm_get_state() ) {

		// only valid state where pedestrians could be allowed to cross
		case STATE_TRAFFIC_FLOWING:
			fsm_set_state(STATE_PED_INIT);
			break;

		default:
			break;
		}
	}
}


/* Callback function for Systick interrupts: state transitions based entirely on control signals */
void systick_callback_fn(void)
{
	if (fsm_lock() == FSM_LOCK_ACQUIRED )
	{
		switch( fsm_get_state() ) {

		case STATE_RESET:
			fsm_set_state(STATE_TRAFFIC_INIT);
			break;

			/* traffic timer done, go to PED state */
		case STATE_TRAFFIC_FLOWING:
			if (get_ARRIVING() == 1) // if train coming, deal with it
				fsm_set_state(STATE_TRAIN_INIT);
			else if (get_HOLD() == 1) // if maintenance, deal with it
				fsm_set_state(STATE_MAINTENANCE);
			else if (get_PED() == 1) // if pedestrian, go to ped state
				fsm_set_state(STATE_PED_INIT);
			break;

			/* walk timer done, deal with edge cases or allow traffic to flow again*/
		case STATE_PED_INIT:
			if (get_ARRIVING() == 1) // if train coming, deal with it
				fsm_set_state(STATE_TRAIN_INIT);
			else if (get_HOLD() == 1) // if maintenance, deal with it
				fsm_set_state(STATE_MAINTENANCE);
			else
				fsm_set_state(STATE_TRAFFIC_INIT);
			break;

		case STATE_MAINTENANCE:
			if (get_CLEAR() == 1)
				fsm_set_state(STATE_TRAFFIC_INIT);
			/* TODO: DEAL WITH TIMER FOR MAINENANCE FLASHING
			 * might need more states*/
			break;


		}
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

	/* Initialize TIM4 timer */
	tim4_init();

	/* Initialize SYSTICK counter */
	systick_init();

	/* Configure user pushbutton and call pushbutton_callback_fn when button press-released */
	userbutton_init(userbutton_callback_fn);

	/* initialize the finite state machine */
	fsm_init();

	/* Enable interrupts - do this after initializing the system */
	__asm ("  cpsie i \n" );

	/* Wait here forever - everything is now interrupt driven */
	while(1)
	{
		;;;
	}
}
