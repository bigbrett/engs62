/*
 * WiFi.c
 * WiFi module support for STM32f4xx board
 *
 *  Created on: Feb 21, 2015
 *      Author: armdev
 */
#include "WiFi.h"


/* Initializer for wifi module to connect to ENGS62 LAN */
void WiFi_init() {
	// initialize USART 2,3 peripherals to communicate with WiFi module
	USART2_init();
	USART3_init();

	// configure WiFi module network settings
	Wifi_config();
}

/* private helper function to compartmentalize WiFi configuration for portability
 * NOTE: in CMD mode, all commands must terminate with a carriage return '\r' */
void Wifi_config()
{
	// Send three '$' in rapid succession to module over USART 3 to enter command mode
	USART3_putstr("$$$");

	// set join mode to auto-connect immediately upon wakeup to stored network
	USART3_putstr("set wlan join 1\r");

	// set wireless authentication to WPA-2 PSK
	USART3_pustr("set wlan auth 4\r");

	// Set wireless SSID to "ENGS62"
	USART3_putstr("set wlan ssid ENGS62\r");

	// Set wireless passphrase to "Engs62connect"
	USART3_putstr("set wlan phrase Engs62connect\r");

	// Set IP configuration to use DHCP from the access point
	USART3_putstr("set ip dhcp 1\r");

	// Set IP remote host to 129.170.66.33
	USART3_putstr("set ip host 129.170.66.33\r");

	// Set IP remote port number to 8880
	USART3_putstr("set ip remote 8880\r");

	// set IP protocol to UDP
	USART3_putstr("set ip protocol 0\r");

	// save configuration to the WiFi module's non-volatile memory
	USART3_putstr("save\r");

	// echo network status to screen
	USART3_putstr("show net\r");
	USART3_putstr("get wlan\n");
	USART3_putstr("get ip\r");

	// exit command mode
	USART3_putstr("exit\r");
}


