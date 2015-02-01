/*
 * LED.c - routines for initializing and manipulating LEDs on the STM3210C-EVAL board
 */

#include "LED.h"
#include "stm32f4xx.h"

void LED_init(void)
{
	uint32_t temp;

	/* Turn on GPIOD's clock */
	RCC->AHB1ENR |= RCC->AHB1ENR | RCC_AHB1ENR_GPIODEN;

	/*
	 * GPIOD setup assumes GPIOD config registers contain the
	 * power-on/reset defaults, but masking using C is
	 * demonstrated for GPIOD->MODER
	 */

	/* Clear any existing GPIOD pin 15:12 mode settings */
	GPIOD->MODER &= GPIOx_MODER_PIN12_MASK &
					GPIOx_MODER_PIN13_MASK &
					GPIOx_MODER_PIN14_MASK &
					GPIOx_MODER_PIN15_MASK;

	/* Configure GPIOD pins 15:12 as output, push-pull  */
	GPIOD->MODER |= GPIOx_MODER_PIN12_GPOUT |
					GPIOx_MODER_PIN13_GPOUT |
					GPIOx_MODER_PIN14_GPOUT |
					GPIOx_MODER_PIN15_GPOUT;
}

void LED_update(uint32_t new_state)
{
	GPIOD->BSRR = new_state;
}

