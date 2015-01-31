/*
 * main.c
 */
#include "stdint.h"     /* uint32_t, etc... */
#include "stm32f4xx.h"  /* Useful definitions for the MCU */
#include "LED.h"        /* C routines in LED.c */
#include "USART2.h"     /* assembly routines in USART.S */
#include "systick.h"

/* Interrupt handler for USART2 */
void __attribute__ ((interrupt)) USART2_handler(void)
{
	uint8_t word = USART2_recv();
	USART2_send(word);
	USART2_clr();
}


/* Takes a 32-bit value and prints on console as hexadecimal number */
void print_hex(uint32_t val)
{
	uint32_t nibbles[8]; // array holding each nibble
	uint32_t mask = 0xF; // 1-nibble mask
	int i;

	// foreach nibble, get LSNibble, add offset, then LSR 4 bits to so next nibble is LSN
	for (i=0; i<8; ++i, val>>=4) {
		nibbles[i]= val & mask; // get nibble i

		// add ASCII offsets
		if (nibbles[i]< 10) {
			nibbles[i] += 48;
		}
		else {
			nibbles[i] += 55;
		}
	}

	// Print array in reverse order
	for (i=7; i>=0; i--) {
		USART2_send(nibbles[i]);
	}
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

	print_hex(234234234);

	/* Wait here forever */
	while(1);

	/* We'll never reach this line */
	return 0;
}
