#ifndef P2P_TRANSMITTER_UART_H
#define P2P_TRANSMITTER_UART_H

#include "packet.h"

void sendMessage(char* m);
void sendPacket(Packet* p, int pin);

#endif //P2P_TRANSMITTER_UART_H