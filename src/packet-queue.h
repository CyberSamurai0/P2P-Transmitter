#ifndef P2P_TRANSMITTER_BYTES_H
#define P2P_TRANSMITTER_BYTES_H

#include <stdint.h>
#include "bytes.h"

typedef struct PacketQueueNode {
	struct packet* value;
	struct PacketQueueNode* next;
	struct PacketQueueNode* prev;
} PacketQueueNode;

typedef struct PacketQueue {
	int length;
	PacketQueueNode* first;
	PacketQueueNode* last;
} PacketQueue;

#endif //P2P_TRANSMITTER_BYTES_H
