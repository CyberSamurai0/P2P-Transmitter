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
        front->next->prev = NULL; // Unset next's .prev
        front->next = NULL; // Don't delete the entire list!
    } else q->first = NULL; // If no remaining elements, unset .first

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

}

// Frees all contained PacketQueueNodes, Packets, and itself
void freePacketQueue(PacketQueue* q) {

}