#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>
#include <stdlib.h>

/**
 * @file
 */

// Byte struct representing a single byte in the linked list
typedef struct Byte {
    uint8_t value;
    struct byte* next;
    struct byte* previous;
} Byte;

/// @defgroup PACKET_FLAGS
/// @{
/// @brief Indicates that the Packet with the provided identifier should be retransmitted from the peer's cache
#define PACKET_FLAGS_RETRANSMIT 0x01
/// @}

// Packet struct representing a sequence of bytes
typedef struct Packet {
	/// @brief Number of contained Bytes within the Packet. This value is not reliable and must be updated in any custom Packet implementations.
    int length;             // Number of bytes in the packet

    /// @brief Transmission flags
    /// 
    uint8_t flags;
    uint8_t identifier;     // 4-bit packet identifier for reduced-packet context

	/// @brief The first Byte contained within the Linked List, used for forward traversal
    Byte* firstByte;
	/// @brief The last Byte contained within the Linked List, used for reverse traversal
    Byte* lastByte;

    /// @brief Boolean used for even parity implementation
    uint8_t parity;
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
