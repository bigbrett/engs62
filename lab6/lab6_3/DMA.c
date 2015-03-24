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
void DMA2_init(void)
{
	/* Enable DMA 2 */

	/* Enable DMA2_stream interrupts */
	DMA_S0_init();
	/* Enable DMA2 Peripheral to memory mode DMA->SxCR EN=1*/

}

/*
 * Configures DMA stream 0, and enables interrupts
 */
void DMA2_S0_init(void)
{
	// TODO page 231 of reference manual
	//DMA->DMA_SxCR CHSEL[2:0]
}
