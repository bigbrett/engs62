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

	// Set PSC and ARR to achieve the desired fundemental frequency of 0.1 Hz
	TIM7->TIMx_ARR = 0x13FF; // 10 second period

	// Enable TIM6_DIER_CC1IE interrupts
	TIM7->TIMx_DIER |= 0x1;

	// one pulse mode and interrupt on overflow
	NVIC_INTERRUPT_TIM7_ENABLE();
	TIM7->TIMx_CR1 |= 0x8C;
}

void TIM7_1_sec(void)
{
	TIM7->TIMx_PSC = 0xC34;
	TIM7->TIMx_EGR |= 0x1; //
	TIM7->TIMx_CR1 |= 0x1;
}

void TIM7_kill(void)
{
	TIM7->TIMx_CR1 &= ~0x00000001;
}


/* PUBLIC: Interrupt handler for TIM2 */
void __attribute__ ((interrupt)) TIM7_handler(void)
{
	// call callback fn
	if (rx_callback_fn)
		rx_callback_fn();
}



