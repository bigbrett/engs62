/*
 * timer.c - Routines for TIM4 timer on the STM3210C-EVAL board
 */
#include "timer.h"

/* define callback function */
static void(*rx_callback_fn)(void);

void TIM7_init(void(*TIM7_callback_fn)(void))
{
	// configure global rx callback function for timer
	rx_callback_fn = TIM7_callback_fn;

	// enable tim6 peripheral clock
	RCC->APB1ENR |= RCC_APB1ENR_TIM7;

	// Set PSC and ARR to achieve the desired period
	TIM7->TIMx_ARR = 159999;

	// Enable TIM6_DIER_CC1IE interrupts
	TIM7->TIMx_DIER |= 0x1;

	// one pulse mode and interrupt on overflow
	NVIC_INTERRUPT_TIM7_ENABLE();
	TIM7->TIMx_CR1 |= 0x8C;
}

void TIM7_1_sec(void)
{
	TIM7->TIMx_PSC = 800; // set prescaler (old = 65536)
	TIM7->TIMx_EGR |= 0x1;
	TIM7->TIMx_CR1 |= 0x1;
}

void TIM7_kill(void)
{
	TIM7->TIMx_CR1 &= ~0x1;
}


/* PUBLIC: Interrupt handler for TIM2 */
void __attribute__ ((interrupt)) TIM7_handler(void)
{
	// call callback fn
	if (rx_callback_fn)
		TIM7->TIMx_SR &= ~0x1; // clear interrupts
		rx_callback_fn();
}



