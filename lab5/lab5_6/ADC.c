/*
 * ADC.c
 * Analog to Digital Converter support for STM32F4xx discovery board
 *
 *  Created on: Jan 31, 2015
 *      Author: Brett Nicholas
 */

#include "ADC.h"
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


uint32_t ADC_getData(void)
{
	/* Begin conversion by setting SWSTRT (bit 30) to 1*/
	ADC->CR2 |= 0x40000000; // bit 30 <= 1
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
	char* curr = "Latest  >> \0";
	char* newline = "\n\r\0";
	USART2_putstr(newline);
	USART2_putstr("***** ADC SAMPLE HISTORY *****");
	/* print history */
	int i;
	for (i=0; i<4; i++) {
		if (i==0) {
			USART2_putstr(newline);
			USART2_putstr(curr);
			printHex(history[i]);
			USART2_putstr(hist);
		}
		else {
			USART2_putchar('\t');
			USART2_putchar(i+48);
			USART2_putchar(')');
			USART2_putchar(' ');
			printHex(history[i]);
		}
	}
}


void ADC_avgHist(void)
{
	uint32_t avg = 0;
	int i;
	for (i=0; i<4; i++) {
		avg += history[i];
	}
	avg = avg/4; // take average

	USART2_putstr("\n\rAverage >> ");
	printHex(avg);
}


void ADC_clrHist(void)
{
	int i;
	for (i=0; i<4; i++) {
		history[i] = 0;
	}
	USART2_putstr("\n\r** History Cleared **\n\r");
}

/* handles ADC_button_press */
void ADC_buttonScan(void)
{
	uint32_t data = ADC_getData(); // cget data from ADC
	ADC_updateHist(data);   // add value to history
	USART2_putstr("\n\r***** BUTTON PRESS REGISTERED *****\n\r");
	USART2_putstr("VALUE >> ");
	printHex(data);
}


uint32_t ADC_scanEcho(void)
{
	uint32_t data = ADC_getData(); // Get value from ADC
	ADC_updateHist(data);   // add value to history
	USART2_putstr("VALUE >> ");
	printHex(data);
	return data;
}





