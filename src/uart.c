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

#define UART_ID uart0
#define BAUD_RATE 20
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE

// Function to add a byte to the packet
void addByteToPacket(Packet* packet, uint8_t value) {
    // Dynamically allocate memory for a new Byte struct
    Byte* newByte = (Byte*)malloc(sizeof(Byte));
    // If memory allocation fails, exit the function
    if (!newByte) return;

    // Assign the value to the new Byte's value field
    newByte->value = value;
    // Set the next pointer of the new Byte to NULL (no next node yet)
    newByte->next = NULL;
    // Set the previous pointer to point to the current last Byte in the packet
    newByte->previous = packet->lastByte;

    // Check if the packet already has a lastByte (packet is not empty)
    if (packet->lastByte) {
        // Update the current lastByte's next pointer to the new Byte
        packet->lastByte->next = newByte;
    } else {
        // If the packet is empty, set the firstByte to the new Byte
        packet->firstByte = newByte;
    }

    // Update the packet's lastByte to the new Byte
    packet->lastByte = newByte;
    // Increment the length of the packet
    packet->length++;
}

// Function to receive and reconstruct the message
void receiveMessage() {
    // Continue reading while there is data available in the UART buffer
    while (uart_is_readable(UART_ID)) {
        // Read a byte from the UART receiver
        uint8_t ch = uart_getc(UART_ID);

        // Mask the received byte to ensure it is only 8 bits
        uint8_t data = ch & 0xFF;

        // Add the valid byte to the received packet
        addByteToPacket(&receivedPacket, data);
    }
}


}
