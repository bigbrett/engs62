/*
 * DMA.h
 *
 *  Created on: Mar 5, 2015
 *      Author: Brett Nicholas
 */
#pragma once
#include "stm32f4xx_dma.h"
#include "stm32f4xx_adc.h"
typedef unsigned short uint16_t;

/* Enables DMA2 S0 to interface with ADC1 */
void DMA2_S0_init(uint16_t* p_buffer);
