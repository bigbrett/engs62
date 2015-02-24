/*
 * USART3.h
 *
 *  Created on: Feb 15, 2015
 *      Author: armdev
 */
#pragma once

#include "stdint.h"
#include <USART2.h>
#include <cortexm4_nvic-template.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_usart.h>

/*
 * Initialize the USART to 9600 baud, 8 data bits, no parity, one stop bit.
 * Received data are passed back using the USART2_rx_callback() function passed in to init.
 */
void USART3_init(void(*USART2_rx_callback)(uint8_t byte));

/* Send a single character out USART2 */
void USART3_putchar(uint8_t byte);

/* Send a null-terminated string out USART2 */
void USART3_putstr(uint8_t *buffer);





