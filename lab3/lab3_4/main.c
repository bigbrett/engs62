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


/* Interrupt handler for USART2 */
void __attribute__ ((interrupt)) USART2_handler(void)
{
	uint8_t word = USART2_recv();
	USART2_send(word);
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
	__asm ("  cpsie i \n" );


	/* Initialize the USART for 9600, 8N1, send '!' - calls into USART2.S */
	/* NOTE: must set USART2 interrupt config register to enable TX/RX interrupts */
	USART2_init();
	USART2_send('!');

	print_hex(0x00001111);
	USART2_send('!');

	button_init();
	ADC_init();
	button_scan();

	/* Wait here forever */
	while(1);

	/* We'll never reach this line */
	return 0;
}
