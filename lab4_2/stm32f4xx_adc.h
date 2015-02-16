/*
 * stm32f4xx_adc.h
 *
 *  Created on: Feb 15, 2015
 *      Author: armdev
 */
#pragma once
#include "stdint.h"

/* Analog to Digital Converter */
volatile typedef struct {
	uint32_t SR; 					/* ADC Status Register 				   - offset 0x00  */
	uint32_t CR1;					/* ADC Control Register #1 			   - offset 0x04  */
	uint32_t CR2; 					/* ADC Control Register #2 			   - offset 0x08  */
	uint32_t SMPR1;					/* ADC Sample Time Register #1 		   - offset 0x10  */
	uint32_t SMPR2;					/* ADC Sample Time Register #2 		   - offset 0x14  */
	uint32_t JOFR1;					/* ADC Sample Time Register #3 		   - offset 0x18  */
	uint32_t JOFR2;					/* ADC Sample Time Register #3 		   - offset 0x18  */
	uint32_t JOFR3;					/* ADC Sample Time Register #3 		   - offset 0x18  */
	uint32_t JOFR4;					/* ADC Sample Time Register #3 		   - offset 0x18  */
	uint32_t HTR;
	uint32_t LTR;
	uint32_t SQR1; 					/* ADC Regular Sequence Register #1    - offset 0x2C  */
	uint32_t SQR2; 					/* ADC Regular Sequence Register #2    - offset 0x30  */
	uint32_t SQR3; 					/* ADC Regular Sequence Register #3    - offset 0x34  */
	uint32_t JSQR;
	uint32_t JDR1;
	uint32_t JDR2;
	uint32_t JDR3;
	uint32_t JDR4;
	uint32_t DR; 					/* ADC Regular Data Register		   - offset 0x4C  */
	uint32_t CSR; 					/* ADC Common Status Register	-base 0x300 -offset 0x00  */
	uint32_t CCR; 					/* ADC Common Control Register	-base 0x300 - offset 0x04  */
	uint32_t CDR;
} ADC_TypeDef;


/* ADC Definitions */
#define ADC_BASE 				(0x40012000) // Base Address of ADC peripheral
#define ADC						((ADC_TypeDef*)ADC_BASE)
