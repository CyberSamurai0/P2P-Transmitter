#include <stdio.h>
#include <stdlib.h>
#include "packet-queue.h"

// Initializes a Queue and returns the pointer
PacketQueue* createQueue() {
    PacketQueue* q = malloc(sizeof(PacketQueue));
    q->length = 0;
    q->first = NULL;
    q->last = NULL;
    return q;
}

// Initializes a QueueNode containing the given Packet and returns the pointer
PacketQueueNode* createQueueNode(Packet* p) {
    PacketQueueNode* n = malloc(sizeof(PacketQueueNode));
    n->value = p;
    n->prev = NULL;
    n->next = NULL;
    return n;
}

// Removes the first item from the queue and returns it
PacketQueueNode* popQueue(PacketQueue* q) {
    PacketQueueNode* front = q->first;
    if (!front) return NULL; // No elements

    if (front->next) {
        q->first = front->next; // Update .first
        if (q->first) q->first->prev = NULL; // Unset next's .prev
        // q->last is already the last item when we pop from the front, no need to update
        front->next = NULL; // Don't delete the entire list!
    } else {
        q->first = NULL; // If no remaining elements, unset .first
        q->last = NULL; // Also unset last
    }

    q->length--; // Decrement length
    return front; // Return pointer to removed item
}

// Adds an item to the end of the queue
void pushQueue(PacketQueue* q, PacketQueueNode* n) {
    n->prev = q->last; // Set new item prev to be old last
    if (q->first == NULL) {
        q->first = n; // No items in queue, set to first
    } else q->last->next = n; // Update old last to have new item as next
    q->last = n; // Set to last
    q->length++; // Increment length
}

// Frees the associated Packet and itself
void freePacketQueueNode(PacketQueueNode* n) {
    freePacket(n->value); // Free the packet
    n->next = NULL;
    n->prev = NULL;
    free(n); // Free the node
}

// Frees all contained PacketQueueNodes, Packets, and itself
void freePacketQueue(PacketQueue* q) {
    while (q->first) {
        freePacketQueueNode(popQueue(q)); // Pop each item from the queue and free it
    }
    free(q); // Free the queue itself
}

void printPacketQueue(PacketQueue* q) {
    PacketQueueNode* n = q->first;
    if (!n) printf("Packet Queue is empty\n");
    while (n) {
        printPacket(n->value);
        n = n->next;
    }
}