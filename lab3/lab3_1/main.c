/*
 * main.c
 */

#include "stdint.h"     /* uint32_t, etc... */
#include "stm32f4xx.h"  /* Useful definitions for the MCU */
#include "LED.h"        /* C routines in LED.c */
#include "USART2.h"     /* assembly routines in USART.S */

/*
 * The systick Interrupt Service Routine
 */
void __attribute__ ((interrupt)) systick_handler(void)
{
	uint32_t status = GPIOD->IDR;
	status &= 0x00001000; // get value of GREEN LED bit

	// Toggle LED status
	if (status > 0) {
		LED_update(LED_GREEN_OFF);
	}
	else {
		LED_update(LED_GREEN_ON);
	}
}



/* Sets systick interrupts */
void systick_init(void)
{
	/* Disable Systick timer */
	SYSTICK->CTRL = 0x0;

	/* Write new reload value to RVR */
	SYSTICK->LOAD = 160000000; // one second of clock cycles

	/* Write 0 to CVR to clear */
	SYSTICK->VAL = 0x0;

	/* READ MODIFY WRITE to CSR to start SYSTICK timer */
	SYSTICK->CTRL |= SYSTICK_ENABLE_MASK;
}


/* Disbles SYSTICK timer */
void systick_kill(void)
{
	SYSTICK->CTRL = 0x0; // writes 0 to CSR
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

	systick_init();


	/* Wait here forever */
	while(1);

	/* We'll never reach this line */
	return 0;
}
