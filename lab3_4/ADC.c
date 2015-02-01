/*
 * ADC.c
 * Analog to Digital Converter support for STM32F4xx discovery board
 *
 *  Created on: Jan 31, 2015
 *      Author: Brett Nicholas
 */
#pragma once

#include "stdint.h"
#include "stm32f4xx.h"
#define ADC_SQR1_MASK 				0xff0fffff


void ADC_init(void)
{
	uint32_t temp;


	/* Turn on GPIOA's clock */
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	/* Turn on ADC clock (bit 8 <= 1) */
		RCC->APB2ENR |= 0x00000100;

	/* Configure GPIO PA1 as Analog */
		GPIOA->MODER |= GPIOx_MODER_PIN1_GPANALOG;

	/* Turn on ADC1 by setting ADON (bit 0) to 1 in ADC_CR2 */
		ADC->CR2 |= 0x00000001;

	/* Set ADC regular sequence register length (ADC_SQR1) to len=1 (0000) */
		ADC->SQR1 &= ADC_SQR1_MASK;

	/* Configure Channel 1 as first conversion in regular sequence */
		ADC->SQR3 |= 0x00000001; // nibble 1 <= 1
		// TODO: check if this is correct

	/* SET SAMPLE TIME/ RESOLUTION */
		//TODO

    /* Configure for continuous conversion mode by setting SWSTRT (bit 30) to 1*/
		ADC->CR2 |= 0x40000000; // bit 30 <= 1
}
