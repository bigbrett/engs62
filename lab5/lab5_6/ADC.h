/*
 * ADC.h
 *
 *  Created on: Jan 31, 2015
 *      Author: Brett Nicholas
 */
#pragma once
#include "stdint.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_adc.h"
#include "USART2.h"

/* Initializes ADC */
void ADC_init(void);


/* echo ADC value to screen */
void ADC_buttonScan(void);

/* Takes a 32-bit value and prints on console as hexadecimal number */
void printHex(uint32_t val);

/* prints last 4 ADC scans */
void ADC_printHist();

/* updates history based on present value */
void ADC_updateHist(uint32_t newval);

/* Gets data from ADC */
uint32_t ADC_getData(void);

/* Gets data from ADC, adds to history, and echoes over USART */
uint32_t ADC_scanEcho(void);

/* Prints average value of ADC history */
void ADC_avgHist();


/* Clears ADC history */
void clrHist();
