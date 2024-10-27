/*
 * Point-to-Point Laser Diode Transmitter
 * https://github.com/CyberSamurai0/P2P-Transmitter
 */

#include "networking.h"
#include "uart.h"
#include "bytes.h"

int main() {
	sayHelloWorld();
	char x[33];
	getMessage(x);
	Packet p = toPacket(x);
	printPacket(&p);

	return 0;
}