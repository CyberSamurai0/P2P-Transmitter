/**
 * @file
 * @brief Program entrypoint and synchronization controller
 * Point-to-Point Laser Diode Transmitter
 * https://github.com/CyberSamurai0/P2P-Transmitter
 */

#include "packet.h"
#include "packet-queue.h"
#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <sys/select.h>

/// @brief If the Pi Pico module used does not have an onboard LED, set this to 0.
/// @todo Adjust configuration based on your own hardware!
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

#define BAUD_RATE 10

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


/*====================================
	SET RECEIVER TRACKING VARS
====================================*/



/// @brief Contains a pointer to the packet we are currently transmitting
Packet* TX_Packet = NULL;
#define TX_PacketLength 12
bool TX_PacketBits[TX_PacketLength] = {};

/// @brief Contains the index of the bit within the packet that we are currently transmitting
uint8_t TX_BitIndex = 0;

/// @brief Stores the alarm_id used to delay sending the next bit without sleeping
alarm_id_t TX_Alarm = 0;

int64_t TX_FinishedSendingBit(alarm_id_t id, __unused void *user_data);

/// @brief Callback function to trigger transmission of the next bit in the sequence
/// @param id The alarm_id_t that expired, triggering this function
/// @param user_data Allow passing additional values without use of globals
int64_t TX_FinishedSendingBit(alarm_id_t id, __unused void *user_data) {
	TX_BitIndex++; // Increment current bit of packet

	if (TX_BitIndex >= TX_PacketLength) {
		// cachePacket(TX_Packet); // Lets not do retransmission for now :)
		// Reset the alarm_id so that the main loop knows to move on to the next packet
		TX_Alarm = 0;
		// printf("\tDone!\n");
	} else {
		gpio_put(TX_PIN, TX_PacketBits[TX_BitIndex]);
		if (USE_ONBOARD_LED) setOnboardLED(TX_PacketBits[TX_BitIndex]);
		// printf("%d", TX_PacketBits[TX_BitIndex]);
		TX_Alarm = add_alarm_in_ms(1000 / BAUD_RATE, TX_FinishedSendingBit, NULL, true);
	}

	return 0;
}

bool RX_PacketBits[TX_PacketLength-1] = {};
uint8_t RX_BitIndex = 0;
alarm_id_t RX_Alarm = 0;
bool RX_Started = 0;

int64_t RX_ReadNextBit(alarm_id_t id, __unused void *user_data);

void RX_PinStateChanged(uint gpio, uint32_t events) {
	if (GPIO_IRQ_EDGE_RISE & events) {
		if (!RX_Started) {
			RX_Started = 1;
			RX_Alarm = add_alarm_in_ms(1000 / BAUD_RATE, RX_ReadNextBit, NULL, true);
			RX_BitIndex = 0;
			RX_PacketBits[0] = 0; // Start Bit
			printf("\nIncoming: 0");
		}
		gpio_acknowledge_irq(gpio, GPIO_IRQ_EDGE_RISE);
	}
}

int64_t RX_ReadNextBit(alarm_id_t id, __unused void *user_data) {
	RX_BitIndex++; // Increment current bit of reassembled packet

	// If we have reached the last packet, cache it and clear the timer
	/// @warning we are maxing out at 8 bits here!
	if (RX_BitIndex >= TX_PacketLength-1) {
		// cachePacket(TX_Packet); // Lets not do retransmission for now :)
		// Reset the alarm_id so that the main loop knows we're done reassembling the packet
		char c = RX_PacketBits[1] << 7 | RX_PacketBits[2] << 6 | RX_PacketBits[3] << 5 | RX_PacketBits[4] << 4 | RX_PacketBits[5] << 3 | RX_PacketBits[6] << 2 | RX_PacketBits[7] << 1 | RX_PacketBits[8];
		printf("\t: %c", c);
		RX_Alarm = 0;
		RX_Started = 0;
		// printf("\tDone!\n");
		// TODO we could just print the packet here and move on with our lives
	} else {
		bool RX_Bit = gpio_get(RX_PIN) ? 0 : 1; // Invert!
		printf("%d", RX_Bit);
		RX_PacketBits[RX_BitIndex] = RX_Bit;
		RX_Alarm = add_alarm_in_ms(1000 / BAUD_RATE, RX_ReadNextBit, NULL, true);
	}

	return 0;
}




/*====================================


			MAIN FUNCTION


====================================*/

/// @brief Entrypoint function for the program
/// @return Status code
int main() {
	/*====================================
			INITIALIZE BACKGROUND
			 LIBRARY  PROCESSING
	====================================*/
	stdio_init_all(); // Initialize STDIO

	// Initialize Transmission Pin
	gpio_init(TX_PIN);
	gpio_set_dir(TX_PIN, GPIO_OUT);
	gpio_put(TX_PIN, 1); // Default to High
	
	// Initialize Reception Pin
	gpio_init(RX_PIN);
	gpio_set_dir(RX_PIN, GPIO_IN);
	gpio_set_irq_enabled_with_callback(RX_PIN, GPIO_IRQ_EDGE_RISE, true, RX_PinStateChanged);

	// Initialize uart0 instance
	// uart_init(uart0, BAUD_RATE);
	// uart_set_format(uart0, 8, 1, UART_PARITY_EVEN);
	// // Enable UART Read Interrupts so that we can call a handler anytime a Byte is reassembled
	// uart_set_irqs_enabled(uart0, true, false);

	// If Pico W model, initialize the Wi-Fi chip and onboard LED
	if (USE_ONBOARD_LED) {
		if (cyw43_arch_init()) { // Returns 0 on success
			printf("Wi-Fi and Onboard LED init failed\n");
		} else cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
	}

	/*====================================
			SET TRACKING VARIABLES
	====================================*/

	/// @brief State of the onboard LED, used for status indication
	int LED_ON = 0;

	/// @brief Store created Packets until they can be transmitted in sequence
	PacketQueue* outboundQueue = createQueue();

	/*====DEBUG STUFF====*/
	// Packet* TEST1 = byteToPacket(toByte('A'));
	// Packet* TEST2 = byteToPacket(toByte('B'));
	// Packet* TEST3 = byteToPacket(toByte('C'));


	/*====================================
				MAIN CLOCK LOOP
	====================================*/
	// while (true) {
	// 	// Add things to our outboundQueue so that we are always transmitting
	// 	// if (outboundQueue->length == 0) {
	// 	// 	pushQueue(outboundQueue, createQueueNode(TEST1));
	// 	// 	pushQueue(outboundQueue, createQueueNode(TEST2));
	// 	// 	pushQueue(outboundQueue, createQueueNode(TEST3));
	// 	// }

	// 	// RECEIVER PROCESSING


	// 	// TRANSMITTER PROCESSING

	// 	if (TX_Alarm <= 0 && outboundQueue->length != 0) {
	// 		PacketQueueNode* n = popQueue(outboundQueue);
	// 		TX_Packet = n->value;

	// 		n->value = NULL; // Don't free the packet yet
	// 		freePacketQueueNode(n); // Free the queue node since we no longer need it

	// 		TX_BitIndex = 0;

	// 		TX_PacketBits[0] = 0; // Start Bit
	// 		TX_PacketBits[1] = TX_Packet->firstByte->value & 128 ? 1 : 0;
	// 		TX_PacketBits[2] = TX_Packet->firstByte->value & 64 ? 1 : 0;
	// 		TX_PacketBits[3] = TX_Packet->firstByte->value & 32 ? 1 : 0;
	// 		TX_PacketBits[4] = TX_Packet->firstByte->value & 16 ? 1 : 0;
	// 		TX_PacketBits[5] = TX_Packet->firstByte->value & 8 ? 1 : 0;
	// 		TX_PacketBits[6] = TX_Packet->firstByte->value & 4 ? 1 : 0;
	// 		TX_PacketBits[7] = TX_Packet->firstByte->value & 2 ? 1 : 0;
	// 		TX_PacketBits[8] = TX_Packet->firstByte->value & 1 ? 1 : 0;
	// 		TX_PacketBits[9] = TX_Packet->parity; /// @warning This is always zero because we are never computing parity for created packets
	// 		TX_PacketBits[10] = 0; // Stop Bit
	// 		TX_PacketBits[11] = 1; // Reset to always-on status and delay for at least one bit

	// 		// printf("\n%c %d\n", TX_Packet->firstByte->value, TX_Packet->firstByte->value);
	// 		// printf("Transmit: %d%d%d%d%d%d%d%d%d%d%d\n", TX_PacketBits[0], TX_PacketBits[1], TX_PacketBits[2], TX_PacketBits[3], TX_PacketBits[4], TX_PacketBits[5], TX_PacketBits[6], TX_PacketBits[7], TX_PacketBits[8], TX_PacketBits[9], TX_PacketBits[10]);
		
	// 		gpio_put(TX_PIN, TX_PacketBits[0]); // Send the Start Bit (we know it will always be zero but for consistency's sake)
	// 		// printf("%d", TX_PacketBits[0]);

	// 		TX_Alarm = add_alarm_in_ms(1000 / BAUD_RATE, TX_FinishedSendingBit, NULL, true);
	// 	}

	// 	// STDIO PROCESSING
		

	// 	tight_loop_contents();
	// }

	/*===================================
			EVERYTHING BELOW IS OLD
	===================================*/

	// Packet* m = toPacket("The quick brown fox jumps over the lazy dog.");

	while (true) {
		// if (USE_ONBOARD_LED) {
		// 	LED_ON = !LED_ON;
		// 	setOnboardLED(LED_ON); // Toggle LED on each input
		// }

		printf("\nSend Message: ");
		char x[33];
		scanf("%32s", &x);
		printf("%s", x);
		int i=0;
		while (x[i]) {
			printf("\nSending: ");
			printf("%c", x[i] & 128 ? '1' : '0');
			printf("%c", x[i] & 64 ? '1' : '0');
			printf("%c", x[i] & 32 ? '1' : '0');
			printf("%c", x[i] & 16 ? '1' : '0');
			printf("%c", x[i] & 8 ? '1' : '0');
			printf("%c", x[i] & 4 ? '1' : '0');
			printf("%c", x[i] & 2 ? '1' : '0');
			printf("%c", x[i] & 1 ? '1' : '0');
			printf(" : %c\n", x[i]);

			gpio_put(TX_PIN, 0); // start
			sleep_ms(1000 / BAUD_RATE);
			gpio_put(TX_PIN, x[i] & 128 ? 1 : 0);
			sleep_ms(1000 / BAUD_RATE);
			gpio_put(TX_PIN, x[i] & 64 ? 1 : 0);
			sleep_ms(1000 / BAUD_RATE);
			gpio_put(TX_PIN, x[i] & 32 ? 1 : 0);
			sleep_ms(1000 / BAUD_RATE);
			gpio_put(TX_PIN, x[i] & 16 ? 1 : 0);
			sleep_ms(1000 / BAUD_RATE);
			gpio_put(TX_PIN, x[i] & 8 ? 1 : 0);
			sleep_ms(1000 / BAUD_RATE);
			gpio_put(TX_PIN, x[i] & 4 ? 1 : 0);
			sleep_ms(1000 / BAUD_RATE);
			gpio_put(TX_PIN, x[i] & 2 ? 1 : 0);
			sleep_ms(1000 / BAUD_RATE);
			gpio_put(TX_PIN, x[i] & 1 ? 1 : 0);




			sleep_ms(1000 / BAUD_RATE);
			gpio_put(TX_PIN, 0); // parity
			sleep_ms(1000 / BAUD_RATE);
			gpio_put(TX_PIN, 0); // stop
			sleep_ms(1000 / BAUD_RATE);
			gpio_put(TX_PIN, 1); // set final
			sleep_ms(1000 / BAUD_RATE);

			i++;
		}
		
		// char x = 0; // Reset on every iteration
		// while (x != '\n' && x != '\r' && x != 13) {
			
		// 	scanf("%c", &x); // Scan char by char
		// 	printf("%c", x); // Echo the provided character

		// 	if (x == '\n' || x == '\r') break; // REMOVE THIS LATER, WE WANT TO IMMEDIATELY SEND ALL DATA ANYWAY
		// 	Packet* m2 = byteToPacket(toByte(x)); // Create a packet for each character (like SSH)
		// 	m2->header = 0b0000;
		// 	cachePacket(m2);
		// 	sendPacket(m2, TX_PIN);
		// 	PacketQueueNode* n = createQueueNode(m2);
		// 	pushQueue(outboundQueue, n);
		// 	// printPacket(m2);
		// }

		//printf("\nQUEUE CONTENTS:\n");
		// TODO manually collect string input to be added to queue

		//printPacketQueue(outboundQueue);
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