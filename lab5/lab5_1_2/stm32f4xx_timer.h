/*
 * stm32f4xx_timer.h
 *
 *  Created on: Feb 15, 2015
 *      Author: armdev
 */
#pragma once
#include "stdint.h"

/* General Purpose Timer: TIM2-TIM5 registers */
volatile typedef struct {
	uint32_t TIMx_CR1; 				/* TIMx Control Register 1					-offset 0x00 */
	uint32_t TIMx_CR2; 				/* TIMx Control Register 2					-offset 0x04 */
	uint32_t TIMx_SMCR;				/* TIMx Slave Mode Control Register 		-offset 0x08 */
	uint32_t TIMx_DIER;				/* TIMx DMA/Intterupt Enable Register 		-offset 0x0C */
	uint32_t TIMx_SR;				/* TIMx Status Register 					-offset 0x10 */
	uint32_t TIMx_EGR;				/* TIMx Event Generation Register	 		-offset 0x14 */
	uint32_t TIMx_CCMR1;			/* TIMx Capture/Compare mode register 1		-offset 0x18 */
	uint32_t TIMx_CCMR2;			/* TIMx Capture/Compare mode register 2		-offset 0x1C */
	uint32_t TIMx_CCER;				/* TIMx Capture/Compare Enable Register 	-offset 0x20 */
	uint32_t TIMx_CNT; 				/* TIMx Counter								-offset 0x24 */
	uint32_t TIMx_PSC;				/* TIMx Prescaler							-offset 0x28 */
	uint32_t TIMx_ARR;				/* TIMx Auto-Reload Register				-offset 0x2C */
	uint32_t RESERVED1;				/* NOTHING									-offset 0x30 */
	uint32_t TIMx_CCR1;				/* TIMx Capture/Compare Register 1			-offset 0x34 */
	uint32_t TIMx_CCR2;				/* TIMx Capture/Compare Register 2			-offset 0x38 */
	uint32_t TIMx_CCR3;				/* TIMx Capture/Compare Register 1			-offset 0x3C */
	uint32_t TIMx_CCR4;				/* TIMx Capture/Compare Register 4			-offset 0x40 */
	uint32_t RESERVED2;				/* NOTHING									-offset 0x44 */
	uint32_t TIMx_DCR;				/* TIMx DMA Control Register				-offset 0x48 */
	uint32_t TIMx_DMAR;				/* TIMx DMA Address for Full Transfer 		-offset 0x4C */
	uint32_t TIMx_OR;				/* TIM2/5 Option Register					-offset 0x50 */
} TIM2to5_TypeDef;

/* TIM 2-4 Definitions */
#define TIM2_BASE				(0x40000000) // Base address of TIM3
#define TIM2					((TIM2to5_TypeDef*)TIM2_BASE)
#define TIM2_CR1_DISABLE 		(0xFFFFFFFc) //F F F F F F F F 1110


#define TIMx_CCMR1_UPCOUNTEN_MASK (0xFFFFFF8F) // upcount enable mask
#define TIMx_CCMR1_UPCOUNTEN	(0x10)

#define TIM4_BASE				(0x40000800) // Base Address of TIM4
#define TIM4 					((TIM2to5_TypeDef*)TIM4_BASE)
#define TIMx_CCMR1_PWMMODE1EN	(0x00000060) // Bits 6:4 <= 0110


