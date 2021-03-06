#pragma once
#include "stm32f4xx_usart.h"
#include <cortexm4_nvic.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include "stdint.h"
/*
 * Initialize the USART to 9600 baud, 8 data bits, no parity, one stop bit.
 * Received data are passed back using the USART2_rx_callback() function passed in to init.
 */
void USART2_init(void(*USART2_rx_callback)(uint8_t byte));

/* Send a single character out USART2 */
void USART2_putchar(uint8_t byte);

/* Send a null-terminated string out USART2 */
void USART2_putstr(uint8_t *buffer);

/* Interrupt handler */
void USART2_handler(void);

/* prints hex value over usart2 */
void print_hex(uint32_t val);
