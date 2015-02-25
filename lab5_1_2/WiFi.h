/*
 * WiFi.h
 * WiFi module support for STM32f4xx board
 *
 *  Created on: Feb 21, 2015
 *      Author: Brett Nicholas
 */
#pragma once
#include "stdint.h"

#define PING 1
#define MYID 20
#define UPDATE 2

typedef struct
{
	int type;
	int id;
} Ping_t;


typedef struct
{
	int type;
	int id;
	int average;
	int values[30];
} Update_resp_t;

void WIFI_send_ping();
void WIFI_recv_ping(uint8_t byte);
