/*
 * ADC.h
 *
 *  Created on: Jan 31, 2015
 *      Author: Brett Nicholas
 */
#pragma once


/* Initializes ADC */
void ADC_init(void);


/* echo ADC value to screen */
void ADC_scan(void);


/* Takes a 32-bit value and prints on console as hexadecimal number */
void print_hex(uint32_t val);

/* prints last 4 ADC scans */
void ADC_printHist();

/* updates history based on present value */
void ADC_updateHist(uint32_t newval);

/* Gets data from ADC */
uint32_t ADC_getData(void);

/* Prints average value of ADC history */
void ADC_avgHist();


/* Clears ADC history */
void clrHist();
