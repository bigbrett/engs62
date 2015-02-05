/*
 * timer.c - Routines for TIM4 timer on the STM3210C-EVAL board
 */

#include "stm32f4xx.h"

void timer_init(void) {

	/* Turn on GPIOD's clock */
	RCC->AHB1ENR |= RCC->AHB1ENR | RCC_AHB1ENR_GPIODEN;

	// configure port D pin 12 for appropriate AF output
	GPIOD->MODER |= 0xc;//GPIOx_MODER_PIN1_GPANALOG;

	// Enable tim4 peripheral clock

	// configure CCMR12 OCM1 field as "PWM mode 1"

	// Set PSC and ARR to achieve the desired fundemental frequency ( )

	// Set CCMR1 to achieve desired duty cycle

	// Enable timer by writing 1 to CEN field in TIM4_CR1





}
