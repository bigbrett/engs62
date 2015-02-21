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

	// Send three '$' in rapid succession to module over USART 3 to enter command mode
	USART3_putstr("$$$");
}

void Wifi_config() {
	// set join mode to auto-connect immediately upon wakeup to stored network
		USART3_putstr("set wlan join 1\n");

	// set wireless authentication to WPA-2 PSK
		USART3_pustr("set wlan auth 4\n");

	// Set wireless SSID to "ENGS62"
		USART3_putstr("set wlan ssid ENGS62\n");

	// Set wireless passphrase to "Engs62connect"
		USART3_putstr("set wlan phrase Engs62connect\n");

	// Set IP configuration to use DHCP from the access point
		USART3_putstr("set ip dhcp 1\n");

	// Set IP remote host to 129.170.66.33
		USART3_putstr("set ip host 129.170.66.33\n");

	// Set IP remote port number to 8880
		USART3_putstr("set ip remote 8880\n");

	// set IP protocol to UDP
		USART3_putstr("set ip protocol 0\n");

	// save configuration to the WiFi module's non-volatile memory
		USART3_putstr("save\n");
}


