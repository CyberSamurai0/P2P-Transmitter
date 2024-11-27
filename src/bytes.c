#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "bytes.h"

const int DEBUG_MODE = 0;

/// @brief Converts a character to a Byte struct (linked list node)
/// @param c 8-bit char to be used as the Byte's value 
/// @return Pointer to the resulting Byte, NULL if there is no available memory
/// @addtogroup Byte
Byte* toByte(char c) {
	Byte* b = malloc(sizeof(Byte));

	if (b == NULL) {
		printf("\nByte malloc failed\n");
		return NULL; // TODO this requires some handling
	}

	if (DEBUG_MODE) printf("Allocated Byte [%p]\n", b);
	b->value = (uint8_t)c;
	b->next = NULL;
	b->previous = NULL;
	return b;
}

/// @brief Creates a reserved, empty Packet struct instance
/// @return Pointer to the resulting Packet, NULL if there is no available memory
/// @addtogroup Packet
Packet* createPacket() {
	Packet* p = malloc(sizeof(Packet));

	if (p == NULL) {
		printf("\nPacket malloc failed\n");
		return NULL; // TODO this requires some handling
	}

	if (DEBUG_MODE) printf("Allocated Packet [%p]\n", p);

	// Ensure that our pointers are zeroed out
	p->firstByte = NULL;
	p->lastByte = NULL;
	p->length = 0; // Set length to 0

	return p;
}

/// @brief Converts a char* string to a Packet containing one Byte per char
/// @param s The char* string to be converted to a Byte sequence
/// @return Pointer to the resulting Packet, NULL if there is no available memory
Packet* toPacket(char* s) {
	Packet* p = createPacket();

	Byte* firstByte = NULL; // The structure is not reliably empty memory
	Byte* previousByte = NULL; // Track the last byte so we can double-link
	while (*s) {
		Byte* b = toByte(*s);
		// printf("Byte Created (%p) with value '%c'\n", b, b->value);
		if (!(p->firstByte)) {
			p->firstByte = b;
			// printf("FirstByte set to %p\n", p->firstByte);
		}
		if (previousByte) { // If there is a previous byte, set its next byte to be this one!
			previousByte->next = b;
			b->previous = previousByte;
		}
		previousByte = b; // Set this byte to be the new previous byte
		p->lastByte = b; // Update "last byte" of packet
		p->length += 1; // Increase length
		s++;
	}
	// printf("THE FIRST BYTE IS CURRENTLY: %p\n", p->firstByte);
	return p;
}

/// @brief Creates a Packet structure containing the provided Byte
/// @param b Pointer to the Byte to be wrapped
/// @return Pointer to the resulting Packet
Packet* byteToPacket(Byte* b) {
	Packet* p = malloc(sizeof(Packet));
	if (DEBUG_MODE) printf("Allocated Packet [%p]\n", p);
	if (p == NULL) {
		printf("\nPacket malloc failed\n");
	}

	p->length = 1; // TODO this is not always true, the provided Byte* could have .next set!!!
	p->firstByte = b;
	p->lastByte = b;

	return p;
}

/// @brief Inserts a Byte at the end of a Packet. This function ignores linked Bytes, so they should be handled appropriately.
/// @param p Pointer to the Packet to be modified
/// @param b Pointer to the Byte to be appended. The addresses stored within .previous and .next will be overwritten.
void packetAppendByte(Packet* p, Byte* b) {
	// Clear existing pointers within the Byte, and pray that the caller handled them appropriately
	b->next = NULL;
	b->previous = NULL;

	// Assuming that both Packet pointers are set appropriately...
	if (p->lastByte) { // If the Packet is not empty,
		p->lastByte->next = b; // Add to the end of the Byte sequence
		b->previous = p->lastByte; // Track the previous Byte before we lose the reference
		p->lastByte = b; // Set as .lastByte
	} else { // If the packet is empty,
		p->firstByte = b; // Set as .firstByte
		p->lastByte = b; // Set as .lastByte
	}
	p->length++; // Increment length!
}

/// @brief Print the detailed contents of a Byte struct instance
/// @param b Pointer to the Byte to be printed
void printByte(Byte* b) {
	printf("\nByte[%ld] (%p) {\n", sizeof(*b), b);
    printf("\tValue: %c\n", b->value);
    printf("\tPrev: %p\n", b->previous);
    printf("\tNext: %p\n", b->next);
	printf("}\n");
}

/// @brief Display the bytes contained within the packet and their ASCII representation
/// @param p Pointer to the Packet to be printed 
void printPacket(Packet* p) {
	printf("Packet[%d]\n", p->length);
	Byte* b = p->firstByte;
	while (b) {
		// Convert to binary byte
		printf("%c", b->value & 128 ? '1' : '0');
		printf("%c", b->value & 64 ? '1' : '0');
		printf("%c", b->value & 32 ? '1' : '0');
		printf("%c", b->value & 16 ? '1' : '0');
		printf("%c", b->value & 8 ? '1' : '0');
		printf("%c", b->value & 4 ? '1' : '0');
		printf("%c", b->value & 2 ? '1' : '0');
		printf("%c", b->value & 1 ? '1' : '0');

		// Display character value
		printf(" %c\n", (char) b->value);

		// Repeat for next
		b = b->next;
	}
}

/// @brief Free the memory reserved for the packet and all contained bytes
/// @param p Pointer to the Packet to be freed
void freePacket(Packet* p) {
	Byte* nextByte = p->firstByte;
	while (nextByte) {
		Byte* thisByte = nextByte;
		nextByte = nextByte->next;
		free(thisByte);
	}
	free(p);
}