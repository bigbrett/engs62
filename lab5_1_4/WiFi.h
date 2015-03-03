/*
 * WiFi.h
 * WiFi module support for STM32f4xx board
 *
 *  Created on: Feb 21, 2015
 *      Author: Brett Nicholas
 */
#pragma once
#include "stdint.h"
#include "stringops.h"

#define PING 1
#define UPDATE 2
#define MYID 20

/*
 * Ping request and response
 */
typedef struct
{
	int type;
	int id;
} Ping_t;


/*
 * Update response
 */
typedef struct
{
	int type;
	int id;
	int average;
	int values[30];
} Update_resp_t;


/*
 * Update request
 */
typedef struct
{
	int type;
	int id;
	int value;
} Update_req_t;

/* PING FUNCTIONS */
void WIFI_send_ping(); /* sends ping over USART3 */
void WIFI_recv_ping(uint8_t byte); /* receivs ping packets from USART3 */
int WIFI_recv_print(); /* prints last received packet */

/* SERVO FUNCTIONS */
void WIFI_send_update(uint32_t value);
void WIFI_recv_update(uint8_t byte);

