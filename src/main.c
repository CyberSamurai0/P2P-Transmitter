/*
 * Point-to-Point Laser Diode Transmitter
 * https://github.com/CyberSamurai0/P2P-Transmitter
 */

#include "networking.h"
#include "uart.h"
#include "bytes.h"
#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"

int main() {
	stdio_init_all();

	sayHelloWorld();

//	int state = 0;
//	while (state != -1) {
//		scanf("%d", &state);
//		if (state == 1) {
//			pullUpDnControl(8, PUD_UP);
//		}
//	}

//	char x[33];
//	getMessage(x);
//	Packet p = toPacket(x);
//	printPacket(&p);

	while (true) {
        sayHelloWorld();
        sleep_ms(5000);
    }

	return 0;
}