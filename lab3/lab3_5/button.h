/*
 * button.h - routines for initializing and controlling LEDs on the STM3210C-EVAL board
 */
#pragma once

/* initialize button GPIOA */
void button_init(void);

/* Busywait for button press */
void button_scan(void);

/* What to do when button press happens */
void button_handler(void);
