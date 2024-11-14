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
#include "hardware/gpio.h"

#define TX_PIN 0
#define RX_PIN 1

int main() {
	stdio_init_all();

	gpio_init(TX_PIN);
	gpio_set_dir(TX_PIN, GPIO_OUT);
	gpio_put(TX_PIN, 1);
	
	// gpio_init(RX_PIN);
	
	// Initialise the Wi-Fi chip
    // if (cyw43_arch_init()) {
    //     printf("Wi-Fi init failed\n");
    //     return -1;
    // }

	sayHelloWorld();

	// Packet* m = toPacket("The quick brown fox jumps over the lazy dog.");

	while (true) {
		sleep_ms(1000);
		printf("Hello!");
		// printPacket(m);
	}

	// while (true) {
	// 	sleep_ms(1000);
	// 	sendPacket(m);
	// }

	// int state = 0;
	// while (state != 9) {
	// 	sleep_ms(1000);
	// 	printf("\n\n=====================[ CNIT 315 Point-to-Point Transmitter ]=====================\n\n");
	// 	printf("1. Turn Laser On\n");
	// 	printf("2. Turn Laser Off\n");
	// 	printf("\n9. Exit\n");
	// 	printf("\nSelection: ");
	// 	scanf("%d", &state);
	// 	switch (state) {
	// 		case 1:
	// 			printf("Console received ON");
	// 			break;
	// 		case 2:
	// 			printf("Console received OFF");
	// 			break;
	// 	}
	// 	sleep_ms(500);
	// }

	return 0;
}