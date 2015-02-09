/*
 * timer.h
 *
 *  Created on: Feb 5, 2015
 *      Author: Brett Nicholas
 */
#pragma once
#include "stdint.h"

/* Initialization steps for TIM4 timer peripheral */
void timer_init(void);

/* Updates TIM4 duty cycle to value passed as argument */
void timer_updateDutyCycle(uint32_t newval);
