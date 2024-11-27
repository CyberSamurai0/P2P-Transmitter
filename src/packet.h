#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>
#include <stdlib.h>

// Byte struct representing a single byte in the linked list
typedef struct byte {
    uint8_t value;
    struct byte* next;
    struct byte* previous;
} Byte;

// Packet struct representing a sequence of bytes
typedef struct packet {
    int length;              // Number of bytes in the packet
    Byte* firstByte;         // Pointer to the first byte
    Byte* lastByte;          // Pointer to the last byte
    uint8_t identifier;      // 4-bit packet identifier for reduced-packet context
} Packet;

// Packet cache for retransmission
#define PACKET_CACHE_SIZE 16
extern Packet* PacketCache[PACKET_CACHE_SIZE];
extern uint8_t rotator;

// Byte functions
Byte* toByte(char c);

// Packet functions
Packet* createPacket();
Packet* createCachedPacket(uint8_t* data, int length);
void freePacket(Packet* p);

// Conversion functions
uint8_t* packetToArray(Packet* p);
Packet* arrayToPacket(uint8_t* data, int length);

// Sending and retransmitting
void sendPacket(uint8_t* data, int length);
void retransmit(uint8_t id);

#endif // PACKET_H
