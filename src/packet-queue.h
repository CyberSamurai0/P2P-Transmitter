#ifndef P2P_TRANSMITTER_PACKET_QUEUE_H
#define P2P_TRANSMITTER_PACKET_QUEUE_H

#include <stdint.h>
#include "bytes.h"

typedef struct PacketQueueNode {
	Packet* value;
	struct PacketQueueNode* next;
	struct PacketQueueNode* prev;
} PacketQueueNode;

typedef struct PacketQueue {
	int length;
	PacketQueueNode* first;
	PacketQueueNode* last;
} PacketQueue;

// Initializes a Queue and returns the pointer
PacketQueue* createQueue();

// Initializes a QueueNode containing the given Packet and returns the pointer
PacketQueueNode* createQueueNode(Packet* p);

// Removes the first item from the queue and returns it
PacketQueueNode* popQueue(PacketQueue* q);

// Adds an item to the end of the queue
void pushQueue(PacketQueue* q, PacketQueueNode* n);

// Frees the associated Packet and itself
void freePacketQueueNode(PacketQueueNode* n);

// Frees all contained PacketQueueNodes, Packets, and itself
void freePacketQueue(PacketQueue* q);

#endif //P2P_TRANSMITTER_PACKET_QUEUE_H
