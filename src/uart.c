#include <stdio.h>
#include "bytes.h"
#include "uart.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define TX_PIN 0
#define RX_PIN 1

// void printByte(Byte *b) {
// 	if (b->value) {
// 		printf("%c", b->value & 128 ? '1' : '0');
// 		printf("%c", b->value & 64 ? '1' : '0');
// 		printf("%c", b->value & 32 ? '1' : '0');
// 		printf("%c", b->value & 16 ? '1' : '0');
// 		printf("%c", b->value & 8 ? '1' : '0');
// 		printf("%c", b->value & 4 ? '1' : '0');
// 		printf("%c", b->value & 2 ? '1' : '0');
// 		printf("%c", b->value & 1 ? '1' : '0');
// 		printf("\n");
// 	}
// }

void sendByte(Byte *b, int delay_ms) {
	if (b->value) {
		b->value & 128 ? gpio_put(TX_PIN, 1) : gpio_put(TX_PIN, 0);
		sleep_ms(delay_ms);

		b->value & 64 ? gpio_put(TX_PIN, 1) : gpio_put(TX_PIN, 0);
		sleep_ms(delay_ms);

		b->value & 32 ? gpio_put(TX_PIN, 1) : gpio_put(TX_PIN, 0);
		sleep_ms(delay_ms);

		b->value & 16 ? gpio_put(TX_PIN, 1) : gpio_put(TX_PIN, 0);
		sleep_ms(delay_ms);

		b->value & 8 ? gpio_put(TX_PIN, 1) : gpio_put(TX_PIN, 0);
		sleep_ms(delay_ms);

		b->value & 4 ? gpio_put(TX_PIN, 1) : gpio_put(TX_PIN, 0);
		sleep_ms(delay_ms);

		b->value & 2 ? gpio_put(TX_PIN, 1) : gpio_put(TX_PIN, 0);
		sleep_ms(delay_ms);

		b->value & 1 ? gpio_put(TX_PIN, 1) : gpio_put(TX_PIN, 0);
		sleep_ms(delay_ms);
	}
}

void sendPacket(Packet *p, int pin) {
	Byte* b = p->firstByte;

	const int delay_ms = 50;

	gpio_put(TX_PIN, 0); // Set to low (START BIT)
	sleep_ms(delay_ms); // Wait at least one interval

	while (b) { // Check if b->next is undefined
		sendByte(b, delay_ms);
		b = b->next; // Increment over the linked list
	}

	// TODO implement parity?

	gpio_put(TX_PIN, 1); // Set to high (STOP BIT)
	sleep_ms(delay_ms); // Wait at least one interval
}

void sendMessage(char *m) {
	// While the current character is defined and not '\0'
	while (*m) {
		// Binary AND comparison between char and each bit
		// 10110100 & 10000000 = True
		// 10110100 & 01000000 = False

		printf("%c", *m & 128 ? '1' : '0');
		printf("%c", *m & 64 ? '1' : '0');
		printf("%c", *m & 32 ? '1' : '0');
		printf("%c", *m & 16 ? '1' : '0');
		printf("%c", *m & 8 ? '1' : '0');
		printf("%c", *m & 4 ? '1' : '0');
		printf("%c", *m & 2 ? '1' : '0');
		printf("%c", *m & 1 ? '1' : '0');
		printf("\n");

		m++; // Move to next character
	}
}
