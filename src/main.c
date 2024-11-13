/*
 * Point-to-Point Laser Diode Transmitter
 * https://github.com/CyberSamurai0/P2P-Transmitter
 */

#include "networking.h"
#include "uart.h"
#include "bytes.h"
#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"

#define TX_PIN 1
#define RX_PIN 2

int main() {
	stdio_init_all();

	// Initialise the Wi-Fi chip
    // if (cyw43_arch_init()) {
    //     printf("Wi-Fi init failed\n");
    //     return -1;
    // }

	sayHelloWorld();

//	char x[33];
//	getMessage(x);
//	Packet p = toPacket(x);
//	printPacket(&p);

	// while (true) {
    //     sayHelloWorld();
    //     sleep_ms(5000);
    // }

	int state = 0;
	while (state != 9) {
		sleep_ms(1000);
		printf("\n\n=====================[ CNIT 315 Point-to-Point Transmitter ]=====================\n\n");
		printf("1. Turn Laser On\n");
		printf("2. Turn Laser Off\n");
		printf("\n9. Exit\n");
		printf("\nSelection: ");
		scanf("%d", &state);
		switch (state) {
			case 1:
				printf("Console received ON");
				break;
			case 2:
				printf("Console received OFF");
				break;
		}
		sleep_ms(500);
	}

	return 0;
}