#ifndef P2P_TRANSMITTER_PACKET_H
#define P2P_TRANSMITTER_PACKET_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * @file
 */

// Byte struct representing a single byte in the linked list
typedef struct Byte {
    uint8_t value;
    struct Byte* next;
    struct Byte* previous;
} Byte;

// Packet struct representing a sequence of bytes
typedef struct Packet {
	/// @brief Number of contained Bytes within the Packet. This value is not reliable and must be updated in any custom Packet implementations.
    int length;

    /// @brief 4-bit Packet header
    /// @details Most significant bit is a Boolean used to request that the peer retransmit the specified Packet Identifier. The three remaining bits are the Packet Identifier.
    uint8_t header;

	/// @brief The first Byte contained within the Linked List, used for forward traversal
    Byte* firstByte;
	/// @brief The last Byte contained within the Linked List, used for reverse traversal
    Byte* lastByte;

    /// @brief Boolean used for even parity implementation
    bool parity;
} Packet;

/// @brief Converts a character to a Byte struct (linked list node)
/// @param c 8-bit char to be used as the Byte's value 
/// @return Pointer to the resulting Byte, NULL if there is no available memory
Byte* toByte(char c);

/// @brief Creates a reserved, empty Packet struct instance
/// @return Pointer to the resulting Packet, NULL if there is no available memory
Packet* createPacket();

/// @brief Creates a Packet requesting that the peer retransmit the cached Packet with the specified identifier
/// @return Pointer to the resulting Packet, NULL if there is no available memory
Packet* createRetransmitPacket(uint8_t identifier);

/// @brief Converts a char* string to a Packet containing one Byte per char
/// @param s The char* string to be converted to a Byte sequence
/// @return Pointer to the resulting Packet, NULL if there is no available memory
Packet* toPacket(char* s);

/// @brief Creates a Packet structure containing the provided Byte
/// @param b Pointer to the Byte to be wrapped
/// @return Pointer to the resulting Packet
Packet* byteToPacket(Byte* b);

/// @brief Extracts the Packet Identifier from the header field
/// @param p Pointer to the Packet to be examined
/// @return The value of the Packet Identifier (ID) bits
uint8_t getPacketID(Packet* p);

/// @brief Checks the header field to determine if the specified Packet ID should be retransmitted
/// @param p Pointer to the Packet to be examined
/// @return The value of the Retransmission (RT) bit
bool getPacketRT(Packet* p);

/// @brief Sets the Packet Identifier within the header field
/// @param p Pointer to the Packet to be modified
/// @param id The 3-bit Packet ID value
void setPacketID(Packet* p, uint8_t id);

/// @brief Creates a Packet to be stored within the PacketCache
/// @return Pointer to the resulting Packet, NULL if there is no available memory
/// @deprecated The Packet Cache is being migrated to main.c
// Packet* createCachedPacket(uint8_t* data, int length);

/// @brief Inserts a Byte at the end of a Packet. This function ignores linked Bytes, so they should be handled appropriately.
/// @param p Pointer to the Packet to be modified
/// @param b Pointer to the Byte to be appended. The addresses stored within .previous and .next will be overwritten.
void packetAppendByte(Packet* p, Byte* b);

// Conversion functions
// uint8_t* packetToArray(Packet* p);
// Packet* arrayToPacket(uint8_t* data, int length);

/// @brief Sending and retransmitting
/// @deprecated Packet transmission should be implemented in uart.c
// void sendPacket(uint8_t* data, int length);
/// @brief Sending and retransmitting
/// @deprecated Packet transmission should be implemented in uart.c
// void retransmit(uint8_t id);

/// @brief Print the detailed contents of a Byte struct instance
/// @param b Pointer to the Byte to be printed
void printByte(Byte* b);

/// @brief Display the bytes contained within the packet and their ASCII representation
/// @param p Pointer to the Packet to be printed 
void printPacket(Packet* p);

// Memory Handling
/// @brief Free the memory reserved for the packet and all contained bytes
/// @param p Pointer to the Packet to be freed
void freePacket(Packet* p);

#endif // P2P_TRANSMITTER_PACKET_H
