#ifndef P2P_TRANSMITTER_BYTES_H
#define P2P_TRANSMITTER_BYTES_H

#include <stdint.h>

typedef struct byte {
	uint8_t value;
	struct byte* next;
	struct byte* previous;
} Byte;

typedef struct packet {
	int length;
	Byte* firstByte;
	Byte* lastByte;
} Packet;

// Byte Creation

Byte* toByte(char c);

// Packet Creation

Packet* createPacket();
Packet* toPacket(char* s);
Packet* byteToPacket(Byte* b);

// Print Functions

void printByte(Byte* b);
void printPacket(Packet* p);

// Memory Handling

void freePacket(Packet* p);

#endif //P2P_TRANSMITTER_BYTES_H
