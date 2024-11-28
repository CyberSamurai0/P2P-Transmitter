#include "packet.h"
#include <stdio.h>
#include <string.h>

const int DEBUG_MODE = 0;

// Create a Byte
Byte* toByte(char c) {
    Byte* b = malloc(sizeof(Byte));
    if (!b) {
        printf("Byte malloc failed\n");
        return NULL;
    }
    b->value = (uint8_t)c;
    b->next = NULL;
    b->previous = NULL;
    return b;
}

// Create an empty Packet
Packet* createPacket() {
    Packet* p = malloc(sizeof(Packet));
    if (!p) {
        printf("Packet malloc failed\n");
        return NULL;
    }
    p->length = 0;
    p->firstByte = NULL;
    p->lastByte = NULL;
    return p;
}

// Create a packet with no Bytes that will
Packet* createRetransmitPacket(uint8_t identifier) {
    Packet* p = createPacket();
    p->retransmit = 1;
    p->identifier = identifier;
}

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

// Create a Packet from an array and cache it
/// @deprecated
// Packet* createCachedPacket(uint8_t* data, int length) {
//     Packet* p = arrayToPacket(data, length);
//     if (!p) return NULL;
//     p->identifier = rotator;  // Assign cyclic identifier
//     PacketCache[rotator] = p; // Cache the packet
//     rotator = (rotator + 1) % PACKET_CACHE_SIZE;
//     return p;
// }

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

void printByte(Byte* b) {
	printf("\nByte[%ld] (%p) {\n", sizeof(*b), b);
    printf("\tValue: %c\n", b->value);
    printf("\tPrev: %p\n", b->previous);
    printf("\tNext: %p\n", b->next);
	printf("}\n");
}

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

// Free a Packet and its bytes
void freePacket(Packet* p) {
    Byte* current = p->firstByte;
    while (current) {
        Byte* next = current->next;
        free(current);
        current = next;
    }
    free(p);
}

// Convert a Packet to an array
// uint8_t* packetToArray(Packet* p) {
//     uint8_t* array = malloc(p->length);
//     if (!array) {
//         printf("Array malloc failed\n");
//         return NULL;
//     }
//     Byte* current = p->firstByte;
//     int i = 0;
//     while (current) {
//         array[i++] = current->value;
//         current = current->next;
//     }
//     return array;
// }

// Convert an array to a Packet
// Packet* arrayToPacket(uint8_t* data, int length) {
//     Packet* p = createPacket();
//     if (!p) return NULL;
//     for (int i = 0; i < length; i++) {
//         Byte* b = toByte(data[i]);
//         if (!b) {
//             freePacket(p);
//             return NULL;
//         }
//         if (p->lastByte) {
//             p->lastByte->next = b;
//             b->previous = p->lastByte;
//         } else {
//             p->firstByte = b;
//         }
//         p->lastByte = b;
//         p->length++;
//     }
//     return p;
// }

// Mock function to send a packet
// void sendPacket(uint8_t* data, int length) {
//     printf("Sending: ");
//     for (int i = 0; i < length; i++) {
//         printf("%02X ", data[i]);
//     }
//     printf("\n");
// }

// Retransmit a cached packet
// void retransmit(uint8_t id) {
//     if (PacketCache[id]) {
//         uint8_t* array = packetToArray(PacketCache[id]);
//         if (array) {
//             sendPacket(array, PacketCache[id]->length);
//             free(array);
//         }
//     } else {
//         printf("Error: No packet with ID %u in cache.\n", id);
//     }
// }