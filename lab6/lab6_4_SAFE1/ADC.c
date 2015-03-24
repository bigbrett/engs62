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
#define ADCBUFFSIZE 10
static int history[4];
static int initialized = 0;

// TODO: No longer volatile
static volatile uint16_t ADCbuffer[10];

/* module-global pointer to the callback function for rx'd bytes populated in init function. */
static void(*rx_callback_fn)(volatile uint16_t* ADCbuffer, uint32_t buffer_size);

/* module-global pointer to the callback function for DMA TCIE signals */
//void(*ADC_callback_fn)(uint16_t* buffer, uint32_t buffer_size);


/*
 * Interrupt handler for ADC: passes converted value to callback function
 */
void __attribute__ ((interrupt)) ADC_handler(void)
{
	// get newly converted data
	uint16_t adc_data = ADC->DR;

	// pass converted data to callback function
//	if (rx_callback_fn)
//		rx_callback_fn(adc_data);
}

/*
 * Interrupt handler for ADC DMA configuration
 */
void __attribute__ ((interrupt)) DMA2_stream0_handler(void)
{
	// Clear TCIF by setting CTCIF0 bit
	DMA->LIFCR |= 0x20;

	// Disable TIM2
	tim2_kill();

	// Call ADC callback function
	if (rx_callback_fn)
		rx_callback_fn(ADCbuffer, 10);

	//tim2_init(); tim2_start();
}


/*
 * Initializes ADC, but does NOT begin conversion
 */
void ADC_init(void(*ADC_callback_fn)(volatile uint16_t* ADCbuffer, uint32_t buffer_size))
{
	// declare callback function
	rx_callback_fn = ADC_callback_fn;

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

	/* configure ADC to start conversions using TIM2_CH3 as an external trigger */
	ADC->CR2 |= 0x1;// chose ADC 1
	ADC->CR2 |= 0x14000000; // enable TIM2 to trigger

	/* clear DMA flag before DMA_init */
	ADC->CR2 &= ~0x00000100;

	/* disable TIM2 before DMA_init */
	tim2_kill();

	 //TODO: Set DDS (bit 9) to 1 (DMA requests are issued as long as data are converted and DMA=1)
	ADC->CR2 |= 0x200;

	/* Configure DMA to control ADC data */
	DMA2_S0_init(ADCbuffer);
	NVIC_INTERRUPT_DMA2_ENABLE();

	/* Set DMA flag to enable ADC DMA control */
	ADC->CR2 |= 0x00000100;

	/* configure TIM2 counter to trigger ADC conversion */
	tim2_init();

	/* initialize history array */
	int i;
	for (i=0; i<4; i++) {
		history[i] = 0;
	}


	initialized = 1;
}


/*
 * Enables ADC interrupts
 */
void ADC_IE(void)
{
	NVIC_INTERRUPT_ADC_ENABLE();
	ADC->CR1 |= ADCx_CR1_EOCIE; // set bit 5 to 1
	ADC->CR2 |= 0x1; // Enable ADC
}


/*
 * Enable ADC, busywait for conversion to complete, then return value
 */
uint32_t ADC_getData(void)
{
	/* Begin conversion by setting SWSTRT (bit 30) to 1*/
	ADC->CR2 |= 0x1; // Enable ADC
	ADC->CR2 |= 0x40000000; // bit 30 <= 1
	while (ADC->SR & 0x2 == 0); // busywait for EOC flag
	return ADC->DR; // return Data (implicit EOC flag reset)
}


/*
 * Enables TIM2 control of ADC
 */
void ADC_start()
{
	//	ADC->CR2 |= 0x1; // Enable ADC
	//	ADC->CR2 |= 0x40000000; // Start conversion
	ADC->CR2 |= 0x1; // Enable ADC
	tim2_start();
}


/*
 * Returns pointer to 10-byte ADC buffer
 */
volatile uint16_t* ADC_getBuffer(void)
{
	return ADCbuffer;
}


/*
 * Clears 10-byte ADC buffer
 */
//void ADC_clearBuffer(void)
//{
//	int i=0;
//	for (i=0; i<ADCBUFFSIZE; i++)
//		ADCbuffer[i] = 0;
//}

void ADC_printBuffer(void)
{
	int i=0;
	for (i=0; i<ADCBUFFSIZE; i++) {
		USART2_putstr((uint8_t*)int2str(i));
		USART2_putstr(": ");
		USART2_putstr((uint8_t*)int2str(ADCbuffer[i]));
		USART2_putstr("\n\r");
	}
}
