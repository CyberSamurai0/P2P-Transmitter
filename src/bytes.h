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

Byte* toByte(char c);
Packet* toPacket(char* s);
void printByte(Byte* b);
void freePacket(Packet* p);
void printPacket(Packet* p);

#endif //P2P_TRANSMITTER_BYTES_H
