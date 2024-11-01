#include <stdio.h>
#include "bytes.h"
#include "uart.h"

void sendByte(Byte *b) {
	if (b->value) {
		printf("%c", b->value & 128 ? '1' : '0');
		printf("%c", b->value & 64 ? '1' : '0');
		printf("%c", b->value & 32 ? '1' : '0');
		printf("%c", b->value & 16 ? '1' : '0');
		printf("%c", b->value & 8 ? '1' : '0');
		printf("%c", b->value & 4 ? '1' : '0');
		printf("%c", b->value & 2 ? '1' : '0');
		printf("%c", b->value & 1 ? '1' : '0');
		printf("\n");
	}
}

void sendPacket(Packet *p) {
	Byte* b = p->firstByte;
	while (b) { // Check if b->next is undefined
		sendByte(b);
		b = b->next; // Increment over the linked list
	}
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
