/*
 * WiFi.c
 * WiFi module support for STM32f4xx board
 *
 *  Created on: Feb 21, 2015
 *      Author: armdev
 */
#include "WiFi.h"
#define DEBUG 1

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

	USART2_putstr("Sending bytes...\n\r");
	// loop through message and send it byte by byte
	int i=0;
	for (i=0; i<sizeof(Ping_t); i++) {
		USART3_putchar(*pbyte);
	//	printHex(*pbyte);
		pbyte++;
	}
	USART2_putstr("Bytes sent\n\n\n\n\n\r");
}


/* Receives ping from USART3 */
void WIFI_recv_ping(uint8_t byte)
{
	if (ctr == 0) {
		response.type = 0;
		response.id = 0;
	}

	uint32_t *ptr;

	if (ctr < 4)
		ptr = &response.type;
	else
		ptr = &response.id;

	*ptr = *ptr + ((uint32_t)byte << 7);

	if (ctr >= 7) {
		ctr = 0;
		USART2_putstr("RECEIVED: \r\n");
		printHex(response.type);
		printHex(response.id);
	}
	ctr++;
}

void WIFI_recv_print()
{
	if (response.type == PING) {
		USART2_putstr("RECEIVED\nType: Ping\nID: ");
	}
}


