/*
 * stringops.c
 *
 *  Created on: Mar 2, 2015
 *      Author: armdev
 */
#include "stringops.h"
#define MAX 4294967294

void print_array(uint32_t *arr)
{
	uint8_t *str_ptr;
	uint32_t val;

	int i = 0;
	USART2_putstr("[");
	for (i=0; i < arrlen(arr); i++) {
		printHex(arr[i]);
		USART2_putstr(" , ");
	}
	USART2_putstr("]\n\r");
}


/* returns the number of bytes (chars) in a string */
int strlen(uint8_t *str)
{
	int ctr = 0;
	while (str != '\0') {
		ctr++;
		str++;
		if (ctr >= MAX) {
			return 0;
		}
	}
	return ctr;
}

int arrlen(uint32_t arr[])
{
	return sizeof(arr)/sizeof(arr[0]);
}


uint8_t* int2str(uint32_t num)
{
	uint8_t* ptr;

	// deal with edge cases
	if (num == 0) {
		ptr = "0\0";
		return ptr;
	}

	// send pointer to the end of string;
	uint32_t temp = num;
	while (temp > 0) {
		temp = temp / 10;
		ptr++;
	}

	// Null terminate the string then loop through and populate with ASCII values
	*ptr = '\0';
	while (num > 0) {
		ptr--;
		*ptr = (num % 10) + 48; // ascii values
		num = num/10;
	}

	return ptr; // return pointer to new string
}
