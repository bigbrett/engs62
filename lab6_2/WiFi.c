/*
 * WiFi.c
 * WiFi module support for STM32f4xx board
 *
 *  Created on: Feb 21, 2015
 *      Author: armdev
 */
#include "WiFi.h"

static Ping_t response_ping; /* holds server ping response received over USART3 */
static Update_resp_t response_update; /* holds server update response received over USART3 */
static uint8_t *pstruct; /* pointer to packet place in structure (used to add value in values[3]) */
static int ctr = 0; /* counter for number of packets transmitted */


void WIFI_send_ping()
{
	Ping_t msg; // Ping mesage
	Ping_t *pmsg = &msg; // pointer to ping message
	uint8_t *pbyte;

	// initialize message contents
	pmsg->type = PING;
	pmsg->id = MYID;

	// pointer to a byte of the m5essage (achieved by casting to uint8_t)
	pbyte = (uint8_t *)pmsg;

	USART2_putstr("\n\n\n\rSending bytes...\n\r");
	// loop through message and send it byte by byte
	int i=0;
	for (i=0; i<sizeof(Ping_t); i++) {
		USART3_putchar(*pbyte);
		pbyte++;
	}
	USART2_putstr("Bytes sent\n\r");

	// re-initialize response structures
	//	response_update.type = 0;
	//	response_update.id = 0;
	//	response_ping.type = 0;
	//	response_update.id = 0;
	//  ctr = 0; // re-initialize counter}
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

	if (ctr == 0) {
		response_ping.type = 0;
		response_ping.id = 0;
	}

	// update either type or id field of response buffer
	if (ctr < 4) {
		response_ping.type = response_ping.type + (byte * multiplier);
	}
	else if (ctr < 8) {
		response_ping.id = response_ping.id + (byte * multiplier);
	}
	ctr++; // increment packet count
}


/* Prints last received value from USART3 */
void WIFI_recv_print()
{
	if (response_ping.type == PING && fsm_getID() == 20) {
		USART2_putstr("RECEIVED\n\r\tType: Ping\n\r\tID: ");
		USART2_putstr(int2str(response_ping.id));
		//printHex(response_ping.id);
	}
	else if (response_ping.type == UPDATE && fsm_getID() == 20) {
		USART2_putstr("RECEIVED\n\r\tType: UPDATE\n\r\tID: "); USART2_putstr(int2str(response_update.id));
		USART2_putstr("\n\r\tAVERAGE: "); USART2_putstr(int2str(response_update.average));
		USART2_putstr("\n\r\tVALUES: "); print_array(response_update.values, 30);
	} else {
		USART2_putstr("\n\n\rRECEIVED\n\r\tType: UPDATE\n\r\tID: "); USART2_putstr(int2str(fsm_getID()));
		USART2_putstr("\n\r\tVALUE: "); USART2_putstr(int2str(response_update.values[fsm_getID()]));
	}
}


/* Sends an UPDATE structure holding passed value over USART3 to server */
void WIFI_send_update(uint32_t value)
{
	Update_req_t msg; // UPDATE mesage
	Update_req_t *pmsg = &msg; // pointer to ping message
	uint8_t *pbyte;

	// initialize message contents
	pmsg->type = UPDATE;
	pmsg->id = MYID;
	pmsg->value = value;

	// pointer to a byte of the message (achieved by casting to uint8_t)
	pbyte = (uint8_t *)pmsg;

	// loop through message and send it byte by byte
	USART2_putstr("\n\n\n\rSending bytes...\n\r");
	int i=0;
	for (i=0; i<sizeof(Update_req_t); i++) {
		USART3_putchar(*pbyte);
		pbyte++;
	}
	USART2_putstr("Bytes sent\n\r");

	// re-initialize response structures
	response_update.type = 0;
	response_update.id = 0;
	response_update.average = 0;
	response_ping.type = 0;
	response_update.id = 0;
	ctr = 0; // re-initialize counter

}


/* Receives update from USART3 */
void WIFI_recv_update(uint8_t byte)
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

	// if first packet, set pointer to address of structure
	if (ctr == 0) {
		response_update.type = 0;
		response_update.id = 0;
		response_update.average = 0;
		response_ping.type = 0;
		response_update.id = 0;
		pstruct = &response_update;
	}

	// update either type or id field of response buffer
	if (ctr < 4) {
		response_ping.type = response_ping.type + (byte * multiplier);
		response_update.type = response_update.type + (byte * multiplier);
	}
	else if (ctr < 8) {
		response_update.id = response_update.id + (byte * multiplier);
		response_ping.id = response_ping.id + (byte * multiplier);
	}
	else if (ctr < 12)
		response_update.average = response_update.average + (byte * multiplier);
	else if (ctr < 131)
		*pstruct = byte; // byte gets inserted at current position in values[30]

	ctr++; // increment packet count
	pstruct++; // increment pointer
}

/* updates servo motor based on passed ID */
void WIFI_servoCtrl(uint32_t target_id)
{
	uint32_t idval = response_update.values[target_id];
	servo_update(idval);
}

