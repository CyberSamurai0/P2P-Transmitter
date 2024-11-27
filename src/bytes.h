#ifndef P2P_TRANSMITTER_BYTES_H
#define P2P_TRANSMITTER_BYTES_H

#include <stdint.h>

/**
 * @file
 * @brief Declares Byte, Packet, and associated methods
 */

/// @brief Double Linked List Node wrapper for an 8-bit integer
typedef struct byte {
	/// @brief 8-bit data portion of the Byte structure
	uint8_t value;
	
	/// @brief The next Byte in a Packet sequence, or NULL if this is the last in the sequence
	struct byte* next;

	/// @brief The previous Byte in a Packet sequence, or NULL if this is the first in the sequence
	struct byte* previous;
} Byte;

/// @brief Double Linked List structure with tracked length
typedef struct packet {
	/// @brief Number of contained Bytes within the Packet. This value is not reliable and must be updated in any custom Packet implementations.
	int length;

	/// @brief The first Byte contained within the Linked List, used for forward traversal
	Byte* firstByte;

	/// @brief The last Byte contained within the Linked List, used for reverse traversal
	Byte* lastByte;
} Packet;

// Byte Creation

/// @brief Converts a character to a Byte struct (linked list node)
/// @param c 8-bit char to be used as the Byte's value 
/// @return Pointer to the resulting Byte, NULL if there is no available memory
Byte* toByte(char c);

// Packet Creation

/// @brief Creates a reserved, empty Packet struct instance
/// @return Pointer to the resulting Packet, NULL if there is no available memory
/// @relates packet
Packet* createPacket();

/// @brief Converts a char* string to a Packet containing one Byte per char
/// @param s The char* string to be converted to a Byte sequence
/// @return Pointer to the resulting Packet, NULL if there is no available memory
Packet* toPacket(char* s);

/// @brief Creates a Packet structure containing the provided Byte
/// @param b Pointer to the Byte to be wrapped
/// @return Pointer to the resulting Packet
Packet* byteToPacket(Byte* b);

// Packet Modification

/// @brief Inserts a Byte at the end of a Packet. This function ignores linked Bytes, so they should be handled appropriately.
/// @param p Pointer to the Packet to be modified
/// @param b Pointer to the Byte to be appended. The addresses stored within .previous and .next will be overwritten.
void packetAppendByte(Packet* p, Byte* b);

// Print Functions

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

#endif //P2P_TRANSMITTER_BYTES_H
