/*
 * WiFi.c
 * WiFi module support for STM32f4xx board
 *
 *  Created on: Feb 21, 2015
 *      Author: armdev
 */
#include "WiFi.h"

static Ping_t message;
static Ping_t response;

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
		pbyte++;
	}
	USART2_putstr("Bytes sent");
}

void WIFI_recv_ping(uint8_t byte)
{

}
