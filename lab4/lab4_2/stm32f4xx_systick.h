/*
 * stm32f4xx_systick.h
 *
 *  Created on: Feb 15, 2015
 *      Author: armdev
 */
#pragma once
#include "stdint.h"

/* Systick timer control registers */
volatile typedef struct {
	uint32_t CTRL; 					/* SYSTICK Control and status register - offset 0x00 */
	uint32_t LOAD; 					/* SYSTICK Reload Value Register  	   - offset 0x04 */
	uint32_t VAL; 					/* SYSTICK Current Value Register 	   - offset 0x08 */
} SYSTICK_TypeDef;


/* Systick addresses */
#define SYSTICK_BASE		(0xE000E010) /* Base address of SYSTICK register block */
#define SYSTICK 			((SYSTICK_TypeDef*)SYSTICK_BASE)
#define SYSTICK_ENABLE_MASK 	(0x00000007)
