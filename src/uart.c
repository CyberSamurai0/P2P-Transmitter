#include <stdio.h>
#include "packet.h"
#include "uart.h"
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define TX_PIN 0
#define RX_PIN 1

// Function to send a single byte over GPIO
void sendByte(Byte *b, int delay_ms) {
    // Check if the byte value is valid
    if (b->value) {
        // Transmit each bit of the byte with a delay
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

// Function to send a packet of bytes
void sendPacket(Packet *p, int pin) {
    // Get the first byte in the packet
    Byte* b = p->firstByte;

    // Define the delay for transmitting each bit
    const int delay_ms = 50;

    // Transmit the START bit (low signal)
    gpio_put(TX_PIN, 0);
    sleep_ms(delay_ms); // Wait one interval for the start bit

    // Transmit each byte in the packet
    while (b) {
        sendByte(b, delay_ms); // Send the current byte
        b = b->next;           // Move to the next byte in the list
    }

    // Transmit the STOP bit (high signal)
    gpio_put(TX_PIN, 1);
    sleep_ms(delay_ms); // Wait one interval for the stop bit
}

// Function to send a message as binary
void sendMessage(char *m) {
    // Loop through each character in the message until the end
    while (*m) {
        // Print each bit of the character
        printf("%c", *m & 128 ? '1' : '0');
        printf("%c", *m & 64 ? '1' : '0');
        printf("%c", *m & 32 ? '1' : '0');
        printf("%c", *m & 16 ? '1' : '0');
        printf("%c", *m & 8 ? '1' : '0');
        printf("%c", *m & 4 ? '1' : '0');
        printf("%c", *m & 2 ? '1' : '0');
        printf("%c", *m & 1 ? '1' : '0');
        printf("\n"); // Add a newline for readability

        m++; // Move to the next character
    }
}

// UART configuration constants
#define UART_ID     uart0
#define BAUD_RATE   20 // With no multiplexing, this equates to 1b/50ms
#define DATA_BITS   8
#define STOP_BITS   1
#define PARITY      UART_PARITY_EVEN

/// @brief Converts an 8-bit integer to a Byte and appends it to the supplied Packet
/// @param packet The Packet structure to be modified
/// @param value The data value to be converted to a Byte
/// @deprecated This function should be implemented alongside the definition of the Packet structure. Use packetAppendByte() instead.
void addByteToPacket(Packet* packet, uint8_t value) {
    // Dynamically allocate memory for a new Byte struct
    Byte* newByte = malloc(sizeof(Byte));
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

/// @brief Reads from UART and reconstructs incoming data into a Packet structure.
/// @warning This function is blocking and will not exit until uart_is_readable(UART_ID) returns false. A parallel/async implementation is needed so that data can simultaneously be transferred.
/// @return Pointer to the reconstructed Packet
Packet* receiveMessage() {
    // Create an empty Packet to store our reconstructed data
    Packet* incomingPacket = createPacket(); // This can be NULL if we are out of memory

    // Continue reading while there is data available in the UART buffer
    while (incomingPacket && uart_is_enabled(UART_ID) && uart_is_readable(UART_ID)) {
        // Read a byte from the UART receiver
        uint8_t ch = uart_getc(UART_ID);

        // Mask the received byte to ensure it is only 8 bits
        uint8_t data = ch & 0xFF;

        // Add the valid byte to the received packet
        packetAppendByte(incomingPacket, toByte(data)); // TODO Test new function packetAppendByte to replace addByteToPacket
    }

    return incomingPacket;
}
