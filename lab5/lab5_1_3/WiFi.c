/*
 * WiFi.c
 * WiFi module support for STM32f4xx board
 *
 *  Created on: Feb 21, 2015
 *      Author: armdev
 */
#include "WiFi.h"

/* holds server ping response received over USART3 */
static Ping_t response;

static int ctr = 0;


void WIFI_send_ping()
{
	Ping_t msg; // Ping mesage
	Ping_t *pmsg = &msg; // pointer to ping message
	uint8_t *pbyte;

	// initialize message contents
	pmsg->type = PING;
	pmsg->id = MYID;

	// pointer to a byte of the message (achieved by casting to uint8_t)
	pbyte = (uint8_t *)pmsg;

	USART2_putstr("\n\n\n\rSending bytes...\n\r");
	// loop through message and send it byte by byte
	int i=0;
	for (i=0; i<sizeof(Ping_t); i++) {
		USART3_putchar(*pbyte);
		pbyte++;
	}
	USART2_putstr("Bytes sent\n\r");
}


/* Receives ping from USART3 */
void WIFI_recv_ping(uint8_t byte)
{
	// assign correct multiplier value depending on number of packets received
	int multiplier = 0;
	if (ctr % 4 == 3) { // 0th or 4th packet
		multiplier = 16777216;
	}
	else if (ctr % 4 == 2) { // 1st or 5th packet
		multiplier = 65536;
	}
	else if (ctr % 4 == 1) { // 2nd or 6th packet
		multiplier = 256;
	}
	else if (ctr % 4 == 0) { // 3rd or 7th packet
		multiplier = 1;
	}

	// update either type or id field of response buffer
	if (ctr < 4) {
		response.type = response.type + (byte * multiplier);
	}
	else if (ctr < 8) {
		response.id = response.id + (byte * multiplier);
	}
	ctr++; // increment packet count
}


/* Prints last received value from USART3 */
void WIFI_recv_print()
{
	if (response.type == PING) {
		USART2_putstr("RECEIVED\nType: Ping\n\r\tID: ");
		printHex(response.id);
	}
}


