/*
 * Point-to-Point Laser Diode Transmitter
 * https://github.com/CyberSamurai0/P2P-Transmitter
 */

#include "networking.h"
#include "uart.h"

int main() {
    sayHelloWorld();
	char x[33];
	getMessage(x);
	sendMessage(x);
}