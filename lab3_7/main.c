/*
 * main.c
 */
#include "stdint.h"     /* uint32_t, etc... */
#include "stm32f4xx.h"  /* Useful definitions for the MCU */
#include "LED.h"        /* C routines in LED.c */
#include "USART2.h"     /* assembly routines in USART.S */
#include "systick.h"	/* systick timer support */
#include "ADC.h" 		/* ADC operations */
#include "button.h" 	/* button routines in button.S */
#include "mutex.h" 		/* mutex support */

/* Interrupt handler for USART2 */
void __attribute__ ((interrupt)) USART2_handler(void)
{
	int status; // mutex lock status

	uint8_t word = USART2_recv();
	if (word == 'h') {
		ADC_printHist();
	}
	else if (word == 'a') {
		ADC_avgHist();
	}
	else if (word == 'c') {
		status = lock_mutex(); // check mutex before clearing

		if (status == 0) { // if mutex is unlocked
			ADC_clrHist(); // clear history
			unlock_mutex();  // unlock mutex
		}

	} else {
		USART2_send(word);
	}
	USART2_clr();
}


int main()
{
	/*
	 * Demonstrate use of LED.h/LED.c - modifies hardware registers using C
	 */
	LED_init();
	LED_update(LED_BLUE_ON);
	LED_update(LED_BLUE_OFF);
	LED_update(LED_RED_ON | LED_BLUE_ON | LED_ORANGE_ON | LED_GREEN_ON );

	/*
	 * Demonstrate use of in-line assembly - enable interrupts
	 */
	__asm ("  cpsie i \n" )button_init();


}


