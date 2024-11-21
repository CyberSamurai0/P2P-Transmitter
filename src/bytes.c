#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "bytes.h"

const int DEBUG_MODE = 0;

Byte* toByte(char c) {
	Byte* b = malloc(sizeof(Byte));
	if (DEBUG_MODE) printf("Allocated Byte [%p]\n", b);
	b->value = (uint8_t)c;
	b->next = NULL;
	b->previous = NULL;
	return b;
}

Packet* toPacket(char* s) {
	Packet* p = malloc(sizeof(Packet));
	if (DEBUG_MODE) printf("Allocated Packet [%p]\n", p);
	if (p == NULL) {
		printf("\nPacket malloc failed\n");
	}

	p->length = 0;
	p->firstByte = NULL;
	p->lastByte = NULL;

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

	p->length = 1;
	p->firstByte = b;
	p->lastByte = b;

	return p;
}

void printByte(Byte* b) {
	printf("\nByte[%ld] (%p)\n", sizeof(*b), b);
    printf("\tValue: %c\n", b->value);
    printf("\tPrev: %p\n", b->previous);
    printf("\tNext: %p\n", b->next);
}

void freePacket(Packet *p) {
	Byte* nextByte = p->firstByte;
	while (nextByte) {
		Byte* thisByte = nextByte;
		nextByte = nextByte->next;
		free(thisByte);
	}
	free(p);
}

void printPacket(Packet *p) {
	printf("Packet[%d]\n", p->length);
	Byte* b = p->firstByte;
	while (b) {
		printf("%c", b->value & 128 ? '1' : '0');
		printf("%c", b->value & 64 ? '1' : '0');
		printf("%c", b->value & 32 ? '1' : '0');
		printf("%c", b->value & 16 ? '1' : '0');
		printf("%c", b->value & 8 ? '1' : '0');
		printf("%c", b->value & 4 ? '1' : '0');
		printf("%c", b->value & 2 ? '1' : '0');
		printf("%c", b->value & 1 ? '1' : '0');

		printf(" %c\n", (char) b->value);
		b = b->next;
	}
}