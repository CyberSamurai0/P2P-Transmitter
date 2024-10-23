#include <stdint.h>
#include "bytes.h"

struct byte {
	uint8_t value;
	struct byte* next;
};

struct packet {
	int length;
	struct byte* firstByte;
};