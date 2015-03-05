/*
 * timer.h
 *
 *  Created on: Feb 5, 2015
 *      Author: Brett Nicholas
 */
#pragma once
#include "stdint.h"
#include "stm32f4xx_timer.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "cortexm4_nvic-template.h"

///* Initialization steps for TIM4 timer peripheral */
//void tim4_init(void);
//
///* Updates TIM4 duty cycle to value passed as argument */
//void tim4_updateDutyCycle(uint32_t newval);
//
///* Initialization steps for TIM2 timer peripheral */
//void TIM2_init(void(*tim2_rx_callback)(void));
//
///* Interrupt handler for TIM2 */
//void TIM2_handler(void);
//
///* Initializes basic timer 6 */
//void TIM6_init(void(*tim6_rx_callback)(void));
//void TIM6_1_sec();

/* Interrupt handler for TIM6 */
//void TIM6_handler(void);

/* Initializes basic timer 6 */
void TIM7_init(void(*tim6_rx_callback)(void));
void TIM7_1_sec();
void TIM7_kill(void);

/* Interrupt handler for TIM6 */
void TIM7_handler(void);

