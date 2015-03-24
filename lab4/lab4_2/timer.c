/*
 * timer.c - Routines for TIM4 timer on the STM3210C-EVAL board
 */
#include "timer.h"

/* PRIVATE: Enables Port B pin 6 for Alternate Function 2 (TIM4 ch1) for servo control */
void enable_PB6_AF2()
{
	/* Turn on GPIOB's clock */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

	/* Clear any existing GPIOB pin 6 mode settings */
	GPIOB->MODER &= GPIOx_MODER_PIN6_MASK;   //(0xffffcfff)

	// configure port B pin 6 for AF output
	GPIOB->MODER |= GPIOx_MODER_PIN6_AF; // configure Port B pin 6 for AF

	// Set AF mode to TIM4
	GPIOB->AFRL &= GPIOx_AFRL_AFRL6_MASK;
	GPIOB->AFRL |= GPIOx_AFRL_AFRL6_AF2; // map TIM4 to Pin 6 by setting AF as AF1

	// Enable tim4 peripheral clock
	RCC->APB1ENR |= RCC_APB1ENR_TIM4;
}


/* PUBLIC: Initializes timer to interact with servo motor via PWM on tim4 signal */
void tim4_init()
{
	// Enable port B pin 6, and configure for AF2
	enable_PB6_AF2();

	// configure CCMR1 OC1M field as "PWM mode 1" (bits 6:4 <= 110)
	TIM4->TIMx_CCMR1 |= TIMx_CCMR1_PWMMODE1EN;
	TIM4->TIMx_CCER |= 0x1;

	// Set PSC and ARR to achieve the desired fundemental frequency of 1kHz
	TIM4->TIMx_PSC = 159; // SYSCLOCK/(PSC+1) = CK_CNT; and SYSCLOCK = 16Mhz
	TIM4->TIMx_ARR = 2000; // 1/20kHz = 50 Hz

	// Set CCR1 to achieve desired 10% duty cycle
	TIM4->TIMx_CCR1 = 150;

	// Enable timer by writing 1 to CEN field in TIM4_CR1
	TIM4->TIMx_CR1 |= 0x1;

}

/* define callback function */
static void(*rx_callback_fn)(void);

void TIM2_init(void(*TIM2_rx_callback_fn)(void))
{
	// configure global rx callback function for tim 2 timer
	rx_callback_fn = TIM2_rx_callback_fn;

	// enable tim2 peripheral clock
	RCC->APB1ENR |= RCC_APB1ENR_TIM2;

	// configure CCMR1 OC1M and CCER so tim2 counts up
	TIM2->TIMx_CCMR1 |= TIMx_CCMR1_UPCOUNTEN_MASK;
	TIM2->TIMx_CCMR1 |= TIMx_CCMR1_UPCOUNTEN;

	// enable IRQ enable for overflow
	TIM2->TIMx_CR1 |= 0x4;

	// Enable TIM2_DIER_CC1IE interrupts
	TIM2->TIMx_DIER |= 0x1;

	// Set PSC and ARR to achieve the desired fundemental frequency of 0.1 Hz
	TIM2->TIMx_PSC = 15999; // 20ms period, counting every 0.01 ms
	TIM2->TIMx_ARR = 1000; // 10 second period

	// Enable timer by writing 1 to CEN field in TIM2_CR1
	TIM2->TIMx_CR1 |= 0x1;
}


/* Disables TIM2 timer */
void TIM2_disable(void)
{
	TIM2->TIMx_CR1 &= TIM2_CR1_DISABLE; // 0xFFFFFFFC
}

/* Set new period for TIM2 */
void TIM2_ARRupdate(uint32_t newARR)
{
	uint32_t period = newARR * 1000;

	// temporarily disable timer
	TIM2_disable();

	// set PSC and ARR to achive desired fundemental frequency
	TIM2->TIMx_PSC = 15999;
	TIM2->TIMx_ARR = period;
}


/* PUBLIC: updates TIM4 duty cycle to value passed as argument */
void tim4_updateDutyCycle(uint32_t newval)
{
	TIM4->TIMx_CCR1 = newval;
}


/* PUBLIC: Interrupt handler for TIM2 */
void __attribute__ ((interrupt)) TIM2_handler(void)
{
	USART2_putstr("TIM2_INTERRUPT");
	TIM2_disable();

	// call callback fn
	if (rx_callback_fn)
		rx_callback_fn();

}
