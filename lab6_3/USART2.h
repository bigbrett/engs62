#pragma once

#include "stdint.h"
#include "cortexm4_nvic-template.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"
/*
 * Initialize the USART to 9600 baud, 8 data bits, no parity, one stop bit.
 * Received data are passed back using the USART2_rx_callback() function passed in to init.
 */
void USART2_init(void(*USART2_rx_callback)(uint8_t byte));

/* Send a single character out USART2 */
void USART2_putchar(uint8_t byte);

/* Send a null-terminated string out USART2 */
void USART2_putstr(uint8_t *buffer);


void printHex(uint32_t val);

uint32_t USART2_buffer_length();
uint32_t* USART2_buffer_getInt();
uint8_t* USART2_buffer_getStr();
void USART2_buffer_add(uint8_t byte);
void USART2_buffer_clr();
