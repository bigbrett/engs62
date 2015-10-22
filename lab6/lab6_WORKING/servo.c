/*
 * servo.c - Servo Motor support for stm32f4xx board
 *
 *  Created on: Feb 7, 2015
 *  For use with ENGS 62 Lab 4
 *      Author: Brett Nicholas
 */
#include "servo.h"

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


/* updates TIM4 duty cycle to value passed as argument */
void servo_update(uint32_t newval)
{
	TIM4->TIMx_CCR1 = newval/41 + 100;
}
