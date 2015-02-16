/*
 * ADC.c
 * Analog to Digital Converter support for STM32F4xx discovery board
 *
 *  Created on: Jan 31, 2015
 *      Author: Brett Nicholas
 */


#include "stdint.h"
#include "stm32f4xx.h"
#include "USART2.h"
#define ADC_SQR1_MASK 				0xff0fffff
#define ADC_SAMPLE_RATE				0x8	// 15

void ADC_init(void)
{
	uint32_t temp;


	/* Turn on GPIOA's clock */
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	/* Configure GPIO PA1 as Analog */
		GPIOA->MODER |= 0xc;//GPIOx_MODER_PIN1_GPANALOG;

    /* Turn on ADC clock (bit 8 <= 1) */
		RCC->APB2ENR |= 0x100;


	/* Set ADC regular sequence register length (ADC_SQR1) to len=1 (0000) */
		ADC->SQR1 &= ADC_SQR1_MASK;

	/* Configure Channel 1 as first conversion in regular sequence */
		ADC->SQR3 = 1; // nibble 1 <= 1

	/* set sample time/ resolution for channel 1 */
		ADC->SMPR2 |= ADC_SAMPLE_RATE; // 0x8

	/* Turn on ADC1 by setting ADON (bit 0) to 1 in ADC_CR2 */
		ADC->CR2 |= 0x1;



}


/* Takes a 32-bit value and prints on console as hexadecimal number */
void print_hex(uint32_t val)
{
	uint32_t nibbles[8]; // array holding each nibble
	uint32_t mask = 0xF; // 1-nibble mask
	int i;

	// foreach nibble, get LSNibble, add offset, then LSR 4 bits to so next nibble is LSN
	for (i=0; i<8; ++i, val>>=4) {
		nibbles[i]= val & mask; // get nibble i

		// add ASCII offsets
		if (nibbles[i]< 10) {
			nibbles[i] += 48;
		}
		else {
			nibbles[i] += 55;
		}
	}

	// Print array in reverse order
	for (i=7; i>=0; i--) {
		USART2_send(nibbles[i]);
	}
	USART2_send('\n');
	USART2_send('\r');
}


/* echo ADC data when ready over USART */
void ADC_scan(void)
{
    /* Configure for continuous conversion mode by setting SWSTRT (bit 30) to 1*/
	ADC->CR2 |= 0x40000000; // bit 30 <= 1

	uint32_t data=0; // holds 32 bit ADC value
	uint32_t bar = ADC->SR;
	uint32_t foo = ADC->SR & 0x2;
    while (ADC->SR & 0x2 == 0); // busywait for EOC flag

   	data = ADC->DR; // get ADC data (implicit EOC flag reset)
	bar = ADC->SR;
   	print_hex(data); // send ADC_data over USART
}

