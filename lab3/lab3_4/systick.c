/*
 * systick.c
 */

#include "stdint.h"     /* uint32_t, etc... */
#include "stm32f4xx.h"  /* Useful definitions for the MCU */
#include "LED.h"        /* C routines in LED.c */

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



