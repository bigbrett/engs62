/*
 * stringops.h
 *
 *  Created on: Mar 2, 2015
 *      Author: armdev
 */
#pragma once
#include "stdint.h"

void print_array(uint32_t arr[], uint32_t size);
int strlen(uint8_t *str);
int arrlen(uint32_t arr[]);
uint8_t* int2str(uint32_t num);
