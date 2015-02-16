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


static int history[4];


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

	/* initialize history array */
		int i;
		for (i=0; i<4; i++) {
			history[i] = 0;
		}
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


uint32_t ADC_getData(void)
{
	while (ADC->SR & 0x2 ==0); // busywait for EOC flag
	return ADC->DR; // return Data (implicit EOC flag reset)
}


/* Adds newest value to history array, and removes oldest */
void ADC_updateHist(uint32_t newval)
{
	history[3] = history[2];
	history[2] = history[1];
	history[1] = history[0];
	history[0] = newval;
}


/* */
void ADC_printHist(void)
{
	char* hist = "History:\n\r\0";
	char* curr = "Current >> \0";
	char* newline = "\n\r\0";

	/* print history */
	int i;
	for (i=0; i<4; i++) {
		if (i==0) {
			TXword(newline);
			TXword(curr);
			print_hex(history[i]);
			TXword(hist);
		}
		else {
			USART2_send('\t');
			USART2_send(i+48);
			USART2_send(')');
			USART2_send(' ');
			print_hex(history[i]);
		}
	}
}


/* handles ADC_button_press */
void ADC_scan(void)
{
    /* Begin conversion by setting SWSTRT (bit 30) to 1*/
	ADC->CR2 |= 0x40000000; // bit 30 <= 1

	uint32_t data = ADC_getData(); // current sample from ADC
	ADC_updateHist(data);   // add value to history
	ADC_printHist(); 		// display history

//    while (ADC->SR & 0x2 == 0); // busywait for EOC flag
//   	data = ADC->DR; // get ADC data (implicit EOC flag reset)
}







