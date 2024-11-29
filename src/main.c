/**
 * @file
 * @brief Program entrypoint and synchronization controller
 * Point-to-Point Laser Diode Transmitter
 * https://github.com/CyberSamurai0/P2P-Transmitter
 */

#include "networking.h"
#include "uart.h"
#include "packet.h"
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

#define PACKET_CACHE_SIZE 8

/// @brief Caches the most recently transmitted packets to support retransmission
Packet* PacketCache[PACKET_CACHE_SIZE] = {NULL};
/// @brief The current usable index of \ref PacketCache. This should reset to 0 if it exceeds the length of \ref PacketCache.
int PacketCacheIndex = 0;

/// @brief Add a Packet to PacketCache and set its identifier to its index within the cache
/// @warning Packets should not be cached until they have been dequeued (transmitted), otherwise we may free them too early
/// @param p Pointer to the Packet to be cached
void cachePacket(Packet* p) {
	// Free the cached Packet that has the same identifier
	if (PacketCache[PacketCacheIndex]) freePacket(PacketCache[PacketCacheIndex]);

    // Set the cache item at the provided index to the Packet* p
    PacketCache[PacketCacheIndex] = p;

	// Set the Packet identifier to the assigned cache index
	setPacketID(p, PacketCacheIndex);

    PacketCacheIndex++; // Increment the index
	PacketCacheIndex %= PACKET_CACHE_SIZE; // Reset to zero if we run out of bits
}

/// @brief Entrypoint function for the program
/// @return Status code
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

	/// @brief Store created Packets until they can be transmitted in sequence
	PacketQueue* outboundQueue = createQueue();


	// Packet* m = toPacket("The quick brown fox jumps over the lazy dog.");

	while (true) {
		// if (USE_ONBOARD_LED) {
		// 	LED_ON = !LED_ON;
		// 	setOnboardLED(LED_ON); // Toggle LED on each input
		// }

		printf("\nTesting Scan: ");
		char x = 0; // Reset on every iteration
		while (x != '\n' && x != '\r' && x != 13) {
			scanf("%c", &x); // Scan char by char
			printf("%c", x); // Echo the provided character
			if (x == '\n' || x == '\r') break; // REMOVE THIS LATER, WE WANT TO IMMEDIATELY SEND ALL DATA ANYWAY
			Packet* m2 = byteToPacket(toByte(x)); // Create a packet for each character (like SSH)
			m2->header = 0b0000;
			cachePacket(m2);
			sendPacket(m2, TX_PIN);
			PacketQueueNode* n = createQueueNode(m2);
			pushQueue(outboundQueue, n);
			// printPacket(m2);
		}

		printf("\nQUEUE CONTENTS:\n");
		// TODO manually collect string input to be added to queue

		printPacketQueue(outboundQueue);
		// PacketQueueNode* n = popQueue(outboundQueue);
		// sendPacket(n->value, TX_PIN);


		// PRINT THE QUEUE HERE!!!
	}

	while (true) {	
		// sleep_ms(1000);

		// ON ? gpio_put(TX_PIN, 1) : gpio_put(TX_PIN, 0);
		// ON = !ON;
		
		printf("\n\nHello!\n");

		// sendPacket(m, TX_PIN);
		
		// printf("Packet[%ld] (%p)\n", sizeof(*m), m);
		// printf("\tLength: %d\n", m->length);
		// printf("\tFirst Byte[%ld] (%p) - Value: '%c'\n", sizeof(*(m->firstByte)), m->firstByte, m->firstByte->value);
		// printf("\tLast Byte[%ld] (%p) - Value: '%c'\n", sizeof(*(m->lastByte)), m->lastByte, m->lastByte->value);

		// printf("Let's try printByte(m->firstByte).\n");
		// printByte(m->firstByte);

		// printf("Let's try printPacket(m).\n");
		// printPacket(m);


	}

	// freePacket(m);

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