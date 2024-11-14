#include <stdint.h>
#include <stdio.h>
#include "bytes.h"

Byte* toByte(char c) {
	Byte* b = malloc(sizeof(Byte));
	b->value = (uint8_t)c;
	return b;
}

Packet* toPacket(char* s) {
	Packet* p = malloc(sizeof(Packet));
	
	if (p == NULL) {
		printf("\nPacket malloc failed\n");
	}

	p->length = 0;

	Byte* previousByte; // Track the last byte so we can double-link
	while (*s) {
		Byte* b = toByte(*s);
		if (!p->firstByte) p->firstByte = b;
		if (previousByte) { // If there is a previous byte, set its next byte to be this one!
			previousByte->next = &b;
			b->previous = previousByte;
		}
		previousByte = &b; // Set this byte to be the new previous byte
		p->lastByte = &b; // Update "last byte" of packet
		p->length += 1; // Increase length
		s++;
	}
	return p;
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