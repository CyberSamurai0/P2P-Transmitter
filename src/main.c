/*
 * Point-to-Point Laser Diode Transmitter
 * https://github.com/CyberSamurai0/P2P-Transmitter
 */

#include "networking.h"
#include "uart.h"
#include "bytes.h"
#include "packet-queue.h"
#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"


// If the Pi Pico module used does not have an onboard LED, set this to 0.
#define USE_ONBOARD_LED 1

#if USE_ONBOARD_LED == 1
	#include "pico/cyw43_arch.h" // Wireless Chip and Onboard LED
	
	/// @brief Sets the Raspberry Pi Pico W onboard LED to be on or off
	/// @param state Boolean representing whether the light should be on or off
	void setOnboardLED(bool state) {
		// Check that the onboard LED exists (Pico W model)
		if (cyw43_arch_gpio_put) {
			cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN || 0, state);
		}
	}
#endif

#define TX_PIN 0
#define RX_PIN 1

int main() {
	stdio_init_all(); // Initialize STDIO

	// Initialize Transmission Pin
	gpio_init(TX_PIN);
	gpio_set_dir(TX_PIN, GPIO_OUT);
	gpio_put(TX_PIN, 1); // Default to High
	
	// Initialize Reception Pin
	gpio_init(RX_PIN);
	gpio_set_dir(RX_PIN, GPIO_IN);
	
	if (USE_ONBOARD_LED) {
		// Initialize the Wi-Fi chip and onboard LED
		if (cyw43_arch_init()) { // Returns 0 on success
			printf("Wi-Fi and Onboard LED init failed\n");
		} else cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
	}

	int LED_ON = 0;

	Packet* m = toPacket("The quick brown fox jumps over the lazy dog.");

	PacketQueue* queue = createQueue();


	// while (!stdio_usb_connected) {
	// 	sleep_ms(50);
	// }

	while (true) {
		if (USE_ONBOARD_LED) {
			LED_ON = !LED_ON;
			setOnboardLED(LED_ON); // Toggle LED on each input
		}

		printf("\nTesting Scan: ");
		char x = 0; // Reset on every iteration
		while (x != '\n' && x != '\r') {
			scanf("%c", &x); // Scan char by char
			printf("%c", x); // Print char
			Packet* m2 = byteToPacket(toByte(x)); // Create a packet for each character (like SSH)
			PacketQueueNode* n = createQueueNode(m2);
			pushQueue(queue, n);
			// printPacket(m2);
			freePacket(m2);
		}
		// TODO manually collect string input to be added to queue

		printf("\r\nYEEHAW\n");
		// PRINT THE QUEUE HERE!!!
	}

	while (true) {	
		// sleep_ms(1000);

		// ON ? gpio_put(TX_PIN, 1) : gpio_put(TX_PIN, 0);
		// ON = !ON;
		
		printf("\n\nHello!\n");

		sendPacket(m, TX_PIN);
		
		// printf("Packet[%ld] (%p)\n", sizeof(*m), m);
		// printf("\tLength: %d\n", m->length);
		// printf("\tFirst Byte[%ld] (%p) - Value: '%c'\n", sizeof(*(m->firstByte)), m->firstByte, m->firstByte->value);
		// printf("\tLast Byte[%ld] (%p) - Value: '%c'\n", sizeof(*(m->lastByte)), m->lastByte, m->lastByte->value);

		// printf("Let's try printByte(m->firstByte).\n");
		// printByte(m->firstByte);

		// printf("Let's try printPacket(m).\n");
		// printPacket(m);


	}

	freePacket(m);

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