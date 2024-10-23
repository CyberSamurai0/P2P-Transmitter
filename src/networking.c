// This file should be the entrypoint for physical layer encapsulation/decapsulation

#include <stdio.h>
#include "networking.h"

void sayHelloWorld() {
    printf("Hello World\n");
}

void getMessage(char readToString[33]) {
	printf("Please enter a message to transmit: ");
	scanf("%32s", readToString);
	printf("\n");
}