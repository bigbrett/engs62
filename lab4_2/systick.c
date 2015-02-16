/*
 * systick.c
 */

#include "systick.h"  /* GPIO definitions for the MCU */
#define WAIT_1_MS 16000 /* 1ms of clock cycles */

/* "TIMER" that is incremented by the systick handler, along with getters/setters */
static uint8_t systick_CTR = 0;
uint32_t get_CTR(void)
{
	return systick_CTR;
}
void set_CTR(uint32_t newtime)
{
	systick_CTR = newtime;
}
void inc_CTR(void)
{
	++systick_CTR;
}


/* Define callback functions */
static void (*rx_callback_fn)(void);



/* updates clock period of systick timer by writing new value into RVR */
void systick_reset(uint32_t period)
{
	SYSTICK->LOAD = period;
}

/* Sets systick interrupts */
void systick_init(void(*systick_callback_fn)(void))
{
	// configure proper callback function
	rx_callback_fn = systick_callback_fn;

	// Disable Systick timer
	SYSTICK->CTRL = 0x0;

	// disable interrupts temporarily
	__asm (" cpsid i \n ");

	/* Write new reload value to RVR */
	SYSTICK->LOAD = WAIT_1_MS*30; // 30ms interrupts

	/* Write 0 to CVR to clear */
	SYSTICK->VAL = 0x0;

	/* READ MODIFY WRITE to CSR to start SYSTICK timer */
	SYSTICK->CTRL |= SYSTICK_ENABLE_MASK;

	/* re-enable interrupts */
	__asm (" cpsie i \n");
}


/* Set systick timer to new value (in seconds) and re-initialize */
void systick_set(uint32_t newtime)
{
		// Disable Systick timer
		SYSTICK->CTRL = 0x0;

		// disable interrupts temporarily
		__asm (" cpsid i \n ");

		/* Write new reload value to RVR */
		SYSTICK->LOAD = WAIT_1_MS*1000*newtime; // 1 second * newtime

		/* Write 0 to CVR to clear */
		SYSTICK->VAL = 0x0;

		/* READ MODIFY WRITE to CSR to start SYSTICK timer */
		SYSTICK->CTRL |= SYSTICK_ENABLE_MASK;

		/* re-enable interrupts */
		__asm (" cpsie i \n");
}


/* Disbles SYSTICK timer */
void systick_kill(void)
{
	SYSTICK->CTRL = 0x0; // writes 0 to CSR
}



/*
 * Systick Interrupt Service Routine
 * increments timer each systick pulse
 */
void __attribute__ ((interrupt)) systick_handler(void)
{
	// Call the callback function
	if (rx_callback_fn)
		rx_callback_fn();
}

