#pragma once
#include "stm32f4xx_systick.h"
#include "stm32f4xx_gpio.h"
#include "LED.h"        /* C routines in LED.c */
#include "stdint.h"
/*
 * The systick Interrupt Service Routine
 */
void __attribute__ ((interrupt)) systick_handler(void);

/* Sets systick interrupts */
void systick_init(void);

/* Sets systick to new value and resets */
void systick_reset(uint32_t period);

/* Disbles SYSTICK timer */
void systick_kill(void);

