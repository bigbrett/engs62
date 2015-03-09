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
#include "cortexm4_nvic-template.h"
#include "USART2.h"
#include "timer.h"
#include "DMA.h"
#define ADC_SQR1_MASK 				0xff0fffff
#define ADC_SAMPLE_RATE				0x8	// 15
#define SIZE 10

static volatile uint16_t ADCbuffer[SIZE];
static uint32_t results[3];

/* Initializes ADC */
//void ADC_init(void(*rx_callback_fn)(uint16_t arg));
void ADC_init(void(*ADC_callback_fn)(volatile uint16_t* ADCbuffer, uint32_t buffer_size));

/* echo ADC value to screen */
void ADC_buttonScan(void);

/* Takes a 32-bit value and prints on console as hexadecimal number */
void printHex(uint32_t val);

/* prints last 4 ADC scans */
void ADC_printHist(void);

/* updates history based on present value */
void ADC_updateHist(uint32_t newval);

/* Gets data from ADC */
uint32_t ADC_getData(void);

/* Enables ADC */
void ADC_start(void);

/* Gets data from ADC, adds to history, and echoes over USART */
uint32_t ADC_scanEcho(void);

/* Prints average value of ADC history */
uint32_t* ADC_calcVals(void);
uint32_t ADC_printBuffer(void);

/* Clears ADC history */
void clrHist(void);
