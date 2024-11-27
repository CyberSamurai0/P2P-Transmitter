#ifndef P2P_TRANSMITTER_BYTES_H
#define P2P_TRANSMITTER_BYTES_H

#include <stdint.h>

/// @brief Double Linked List Node wrapper for an 8-bit integer
/// @addtogroup Packet
typedef struct byte {
	/// @brief 8-bit data portion of the Byte structure
	uint8_t value;
	
	/// @brief The next Byte in a Packet sequence, or NULL if this is the last in the sequence
	struct byte* next;

	/// @brief The previous Byte in a Packet sequence, or NULL if this is the first in the sequence
	struct byte* previous;
} Byte;

/// @brief Double Linked List structure with tracked length
/// @addtogroup Packet
typedef struct packet {
	/// @brief Number of contained Bytes within the Packet. This value is not reliable and must be updated in any custom Packet implementations.
	int length;

	/// @brief The first Byte contained within the Linked List, used for forward traversal
	Byte* firstByte;

	/// @brief The last Byte contained within the Linked List, used for reverse traversal
	Byte* lastByte;
} Packet;

// Byte Creation

Byte* toByte(char c);

// Packet Creation

Packet* createPacket();
Packet* toPacket(char* s);
Packet* byteToPacket(Byte* b);

// Packet Modification

void packetAppendByte(Packet* p, Byte* b);

// Print Functions

void printByte(Byte* b);
void printPacket(Packet* p);

// Memory Handling

void freePacket(Packet* p);

#endif //P2P_TRANSMITTER_BYTES_H
