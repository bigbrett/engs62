#include "fsm.h"
#include "USART2.h"
#include "userbutton.h"

void USART2_callback_fn(uint8_t byte)
{
	USART3_putchar(byte); // forward to wifi module

}

void USART3_callback_fn(uint8_t byte)
{
	USART2_putchar(byte); // echo to computer

}



void userbutton_callback_fn(void)
{
	if( fsm_lock() == FSM_LOCK_ACQUIRED )
	{
		fsm_set_state(STATE_RESET);
		fsm_unlock();
	}
}

void main(void)
{
	/* Set up the USART2 9600-8N1 and to call USART2_callback_fn when new data arrives */
	USART2_init(USART2_callback_fn);

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
