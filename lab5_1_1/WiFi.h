/*
 * WiFi.h
 * WiFi module support for STM32f4xx board
 *
 *  Created on: Feb 21, 2015
 *      Author: Brett Nicholas
 */
#pragma once
#include "USART2.h"
#include "USART3.h"
#include "stdint.h"
#include "mutex.h"

/* Method to initialize WiFi module to communicate over USART 3 peripheral */
void WiFi_init();

