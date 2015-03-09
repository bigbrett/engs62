/*
 * stm32f4xx_dma.h
 * DMA registers and type definitions
 *
 */
#pragma once
#include "stdint.h"
typedef unsigned int uint32_t;


/* DMA registers */
volatile typedef struct
{
	uint32_t LISR;				/* DMA Low Interrupt Status Register 			-offset 0x00 */
	uint32_t HISR;				/* DMA High Interrupt Status Register 			-offset 0x04 */
	uint32_t LIFCR;				/* DMA Low Interrupt Flag Clear Register		-offset 0x08 */
	uint32_t HIFCR;				/* DMA Hight Interupt Flag Clear Register 		-offset 0x0C */
	uint32_t S0CR;				/* DMA Stream 0 Configuration Register			-offset 0x10 */
	uint32_t S0NDTR;			/* DMA Stream 0 Number of Data Register 		-offset 0x14 */
	uint32_t S0PAR;				/* DMA Stream 0 Peripheral Address Register 	-offset 0x18 */
	uint32_t S0M0AR;			/* DMA Stream 0 Memory 0 Address Register		-offset 0x1C */
	uint32_t S0M1AR;			/* DMA Stream 0 Memory 1 Address Register 		-offset 0x20 */
	uint32_t S0FCR;				/* DMA Stream 0 FIFO Control Register 			-offset 0x24 */
} DMA_TypeDef;



/* ADC Definitions */
#define DMA2_BASE 				(0x40026400) // Base Address of DMA2
#define DMA						((DMA_TypeDef*)DMA2_BASE)


