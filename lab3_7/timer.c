/*
 * timer.c - Routines for TIM4 timer on the STM3210C-EVAL board
 */

#include "stm32f4xx.h"

void timer_init(void) {

	/* Turn on GPIOB's clock */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

	// configure port B pin 6 for appropriate AF output
	GPIOB->MODER |= GPIOx_MODER_PIN6_AF; // configure Port B pin 6 for AF
	GPIOB->AFRL |= GPIOx_AFRL_AFRL6_AF1; // map TIM4 to Pin 6 by setting AF as AF1

	// Enable tim4 peripheral clock
	RCC->APB1ENR |= RCC_APB1ENR_TIM4;

	// configure CCMR1 OC1M field as "PWM mode 1" (bits 6:4 <= 110)
	TIM4->TIMx_CCMR1 |= TIMx_CCMR1_PWMMODE1EN;

	// Set PSC and ARR to achieve the desired fundemental frequency of 1kHz
	TIM4->TIMx_PSC = 159;
    TIM4->TIMx_ARR = 2000;

	// Set CCR1 to achieve desired 10% duty cycle
    TIM4->TIMx_CCR1 = 100;

	// Enable timer by writing 1 to CEN field in TIM4_CR1
    TIM4->TIMx_CR1 |= 0x1;
}
