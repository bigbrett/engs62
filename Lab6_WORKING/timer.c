/*
 * timer.c - Routines for TIM4 timer on the STM3210C-EVAL board
 */
#include "timer.h"

/* define callback function */
static void(*rx_callback_fn)(void);

/*******************************************************************************
 ******************************* TIM 7 *****************************************/
/* Interrupt handler for TIM7 */
void __attribute__ ((interrupt)) TIM7_handler(void)
{
	// call callback fn
	if (rx_callback_fn)
		TIM7->TIMx_SR &= ~0x1; // clear interrupts
		rx_callback_fn();
}

/*
 * Initializer for TIM7
 */
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

void TIM7_2_sec(void)
{
	TIM7->TIMx_PSC = 1600; // set prescaler (old = 65536)
	TIM7->TIMx_EGR |= 0x1;
	TIM7->TIMx_CR1 |= 0x1;
}



void TIM7_kill(void)
{
	TIM7->TIMx_CR1 &= ~0x1;
}


/*******************************************************************************
 ******************************* TIM 2 *****************************************/
/* PUBLIC: Initializes timer to interact with servo motor via PWM on tim4 signal */
void tim2_init()
{
	// enable tim2 peripheral clock
	RCC->APB1ENR |= RCC_APB1ENR_TIM2;

	// enable TIM2 capture and compare channel 3
	TIM2->TIMx_CCER |= 0x100; // set bit 8

	// configure CCMR2 OC3M field as "PWM mode 1" (bits 6:4 <= 110)
	TIM2->TIMx_CCMR2 |= 0x60;

//	// set TIMx->DIER CC3DE (capture/compare DMA reaquest enable: bit 11)
//	TIM2->TIMx_DIER |= 0x800; // 0000 0000 0000
//
//	// set TIMx->EGR TF (trigger generation: bit 6) and CC3G (capture/compare 3 generation: bit 3)
//	TIM2->TIMx_EGR |= 0x48;

	// Set PSC and ARR to achieve the desired fundemental frequency of 1kHz
	TIM2->TIMx_PSC = 159; // SYSCLOCK/(PSC+1) = CK_CNT; and SYSCLOCK = 16Mhz
	TIM2->TIMx_ARR = 10000; // 1/20kHz = 50 Hz

	// Set CCR1 to achieve desired 10% duty cycle
	TIM2->TIMx_CCR3 = 5000;
}

/* Start timer 2*/
void tim2_start()
{
	TIM2->TIMx_CR1 |= 0x1;
}

/* Kill timer 2*/
void tim2_kill()
{
	TIM2->TIMx_CR1 &= ~0x1;
}
