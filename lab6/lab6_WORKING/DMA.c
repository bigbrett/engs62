/*
 * DMA.c
 *
 * DMA support for use with Analog to Digital Converter on STM32F4xx discovery board
 *
 *  Created on: March 5, 2015
 *      Author: Brett Nicholas
 */

/*
 * Init function for DMA2
 */
#include "DMA.h"


/*
 * Configures DMA stream 0 for ADC data, and enables interrupts. See pg 231 of reference manual
 */
void DMA2_S0_init(volatile uint16_t* buffer)
{
	// Enable DMA2 clock
	RCC->AHB1ENR |= 0x00400000;

	// disable stream by clearing EN bit, then wait for it to be read as 0 confirming no ongoing stream operation
	DMA->S0CR &= ~0x00000001;

	/* Set peripheral port register address
	 * Data will be read from this address after peripheral event */
	DMA->S0PAR = &(ADC->DR); //address of ADC data reg = 0x40026400)

	/* Set memory address
	 * Data will be written to this space after the peripheral event */
	DMA->S0M0AR = (uint32_t)buffer;// address of memory space

	/* Configure total number of data items to be transferred. Value decremented after each peripheral event */
	DMA->S0NDTR = 10;

	/* Select channel 0 */
	DMA->S0CR &= ~0x0E000000;

	/* Set memory and peripheral data size as 16-bit half-word. */
	DMA->S0CR |= 0x2800; // bits [14:13] = [12:11] = 01

	/* Set memory increment value to size of word */
	DMA->S0CR |= 0x400;

	/* Enable transfer complete interrupt (TCIE bit 4) */
	DMA->S0CR |= 0x10; // 0x8;

	/* set LISR TCIF0 bits to 1 to clear TCIE before re-enabling */
	DMA->LISR |= 0x20;

	/* Enable DMA2 Stream 0  */
	DMA->S0CR |= 0x1;
}
