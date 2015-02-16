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
