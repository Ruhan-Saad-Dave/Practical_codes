/*
A double-ended queue (deque) is a linear list in which additions and deletions may be made at either end. Obtain a data representation mapping a deque into a one-dimensional array. Write C++ program to simulate deque with functions to add and delete elements from either end of the deque.
*/

#include <iostream>
#define MAX 10 // Maximum size of the deque

class Deque {
private:
    int arr[MAX];
    int front;
    int rear;

public:
    // Constructor to initialize the deque
    Deque() : front(-1), rear(0) {}

    // Check if the deque is full
    bool isFull() {
        return ((front == 0 && rear == MAX - 1) || front == rear + 1);
    }

    // Check if the deque is empty
    bool isEmpty() {
        return (front == -1);
    }

    // Insert element at the front
    void insertFront(int key) {
        if (isFull()) {
            std::cout << "Overflow: Deque is full\n";
            return;
        }

        // If deque is initially empty
        if (front == -1) {
            front = 0;
            rear = 0;
        }
        // Front is at the first position, wrap around to end
        else if (front == 0) {
            front = MAX - 1;
        } else {
            front = front - 1;
        }

        arr[front] = key;
    }

    // Insert element at the rear
    void insertRear(int key) {
        if (isFull()) {
            std::cout << "Overflow: Deque is full\n";
            return;
        }

        // If deque is initially empty
        if (front == -1) {
            front = 0;
            rear = 0;
        }
        // Rear is at the last position, wrap around to front
        else if (rear == MAX - 1) {
            rear = 0;
        } else {
            rear = rear + 1;
        }

        arr[rear] = key;
    }

    // Delete element from the front
    void deleteFront() {
        if (isEmpty()) {
            std::cout << "Underflow: Deque is empty\n";
            return;
        }

        // Single element left, deque becomes empty
        if (front == rear) {
            front = -1;
            rear = -1;
        }
        // Wrap around front pointer
        else if (front == MAX - 1) {
            front = 0;
        } else {
            front = front + 1;
        }
    }

    // Delete element from the rear
    void deleteRear() {
        if (isEmpty()) {
            std::cout << "Underflow: Deque is empty\n";
            return;
        }

        // Single element left, deque becomes empty
        if (front == rear) {
            front = -1;
            rear = -1;
        }
        // Wrap around rear pointer
        else if (rear == 0) {
            rear = MAX - 1;
        } else {
            rear = rear - 1;
        }
    }

    // Get the front element
    int getFront() {
        if (isEmpty()) {
            std::cout << "Deque is empty\n";
            return -1;
        }
        return arr[front];
    }

    // Get the rear element
    int getRear() {
        if (isEmpty()) {
            std::cout << "Deque is empty\n";
            return -1;
        }
        return arr[rear];
    }
};

// Main function to test the deque
int main() {
    Deque dq;

    dq.insertRear(5);
    dq.insertRear(10);
    std::cout << "Rear element: " << dq.getRear() << std::endl;

    dq.deleteRear();
    std::cout << "After deleting rear, new rear element: " << dq.getRear() << std::endl;

    dq.insertFront(15);
    std::cout << "Front element: " << dq.getFront() << std::endl;

    dq.deleteFront();
    std::cout << "After deleting front, new front element: " << dq.getFront() << std::endl;

    dq.insertFront(25);
    dq.insertRear(35);
    std::cout << "Front element: " << dq.getFront() << std::endl;
    std::cout << "Rear element: " << dq.getRear() << std::endl;

    return 0;
}

/*
Rear element: 10
After deleting rear, new rear element: 5
Front element: 15
After deleting front, new front element: 5
Front element: 25
Rear element: 35
*/
