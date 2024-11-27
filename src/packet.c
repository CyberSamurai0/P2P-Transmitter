#include "packet.h"
#include <stdio.h>
#include <string.h>

// Packet cache and rotator for reduced-packet context
Packet* PacketCache[PACKET_CACHE_SIZE] = {NULL};
uint8_t rotator = 0;

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

// Create a Packet from an array and cache it
Packet* createCachedPacket(uint8_t* data, int length) {
    Packet* p = arrayToPacket(data, length);
    if (!p) return NULL;
    p->identifier = rotator;  // Assign cyclic identifier
    PacketCache[rotator] = p; // Cache the packet
    rotator = (rotator + 1) % PACKET_CACHE_SIZE;
    return p;
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
uint8_t* packetToArray(Packet* p) {
    uint8_t* array = malloc(p->length);
    if (!array) {
        printf("Array malloc failed\n");
        return NULL;
    }
    Byte* current = p->firstByte;
    int i = 0;
    while (current) {
        array[i++] = current->value;
        current = current->next;
    }
    return array;
}

// Convert an array to a Packet
Packet* arrayToPacket(uint8_t* data, int length) {
    Packet* p = createPacket();
    if (!p) return NULL;
    for (int i = 0; i < length; i++) {
        Byte* b = toByte(data[i]);
        if (!b) {
            freePacket(p);
            return NULL;
        }
        if (p->lastByte) {
            p->lastByte->next = b;
            b->previous = p->lastByte;
        } else {
            p->firstByte = b;
        }
        p->lastByte = b;
        p->length++;
    }
    return p;
}

// Mock function to send a packet
void sendPacket(uint8_t* data, int length) {
    printf("Sending: ");
    for (int i = 0; i < length; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");
}

// Retransmit a cached packet
void retransmit(uint8_t id) {
    if (PacketCache[id]) {
        uint8_t* array = packetToArray(PacketCache[id]);
        if (array) {
            sendPacket(array, PacketCache[id]->length);
            free(array);
        }
    } else {
        printf("Error: No packet with ID %u in cache.\n", id);
    }
}