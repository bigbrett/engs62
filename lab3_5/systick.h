#pragma once

/*
 * The systick Interrupt Service Routine
 */
void __attribute__ ((interrupt)) systick_handler(void);

/* Sets systick interrupts */
void systick_init(void);


/* Disbles SYSTICK timer */
void systick_kill(void);

