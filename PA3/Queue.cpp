#include "Queue.h"
#include <iostream>

// Constructor to initialize an empty queue
Queue::Queue() {
    // TODO: Your code here
    front = -1;
    rear = -1;
}

// Adds a province to the end of the queue
void Queue::enqueue(int province) {
    // TODO: Your code here
    // Implement circular structure 
    // Add the province  
    if (isEmpty()) {
        front++;
        rear++;
        data[front] = province;
    } else if (front == rear) {
        rear = (rear + 1) % MAX_QUEUE_SIZE;
        data[rear] = province;
    } else {
        if ((rear + 1) % MAX_QUEUE_SIZE != front) {
            rear = (rear + 1) % MAX_QUEUE_SIZE;
            data[rear] = province;
        }
    }
}

// Removes and returns the front province from the queue
int Queue::dequeue() {
    // TODO: Your code here
    if (isEmpty()) {
        return -1;
    } else if (rear == front) {
        int item = data[front];
        rear = -1;
        front = -1;
        return item;
    } else {
        int item = data[front];
        front = (front + 1) % MAX_QUEUE_SIZE;
        return item;
    }
}

// Returns the front province without removing it
int Queue::peek() const {
    // TODO: Your code here
    if (isEmpty()) {
        return -1;
    }
    return data[front];
}

// Checks if the queue is empty
bool Queue::isEmpty() const {
    // TODO: Your code here
    return rear == -1;
}

// Add a priority neighboring province in a way that will be dequeued and explored before other non-priority neighbors
void Queue::enqueuePriority(int province) {
    // TODO: Your code here
    if (isEmpty()) {
        front++;
        rear++;
        data[front] = province;
    } else {
        if (front - 1 < 0) {
            front = MAX_QUEUE_SIZE - 1;
            data[front] = province;
        } else {
            front--;
            data[front] = province;
        }

    }
}

