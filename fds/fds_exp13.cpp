/*
Pizza parlor accepting maximum M orders. Orders are served in first come first served basis. Oder once placed cannot be cancelled. Write a C++ program to simulate the system using circular queue using array.
*/

#include <iostream>
#define MAX 5  // Maximum number of orders the pizza parlor can handle

class CircularQueue {
private:
    int orders[MAX];
    int front;
    int rear;

public:
    // Constructor to initialize the circular queue
    CircularQueue() : front(-1), rear(-1) {}

    // Check if the queue is full
    bool isFull() {
        return ((rear + 1) % MAX == front);
    }

    // Check if the queue is empty
    bool isEmpty() {
        return (front == -1);
    }

    // Function to add a new order to the queue (enqueue)
    void placeOrder(int orderID) {
        if (isFull()) {
            std::cout << "Order queue is full. No more orders can be accepted.\n";
            return;
        }

        // If the queue is empty, initialize front and rear to the first position
        if (isEmpty()) {
            front = rear = 0;
        } else {
            // Move rear to the next position in a circular manner
            rear = (rear + 1) % MAX;
        }

        orders[rear] = orderID;
        std::cout << "Order " << orderID << " placed successfully.\n";
    }

    // Function to serve an order (dequeue)
    void serveOrder() {
        if (isEmpty()) {
            std::cout << "No orders to serve.\n";
            return;
        }

        std::cout << "Order " << orders[front] << " served.\n";

        // If the queue has only one order, reset front and rear to indicate an empty queue
        if (front == rear) {
            front = rear = -1;
        } else {
            // Move front to the next position in a circular manner
            front = (front + 1) % MAX;
        }
    }

    // Function to display all pending orders in the queue
    void displayOrders() {
        if (isEmpty()) {
            std::cout << "No pending orders.\n";
            return;
        }

        std::cout << "Pending orders: ";
        int i = front;
        while (true) {
            std::cout << orders[i] << " ";
            if (i == rear) break;
            i = (i + 1) % MAX;
        }
        std::cout << "\n";
    }
};

// Main function to test the pizza parlor ordering system
int main() {
    CircularQueue pizzaParlor;
    int choice, orderID;

    do {
        std::cout << "\nPizza Parlor Ordering System\n";
        std::cout << "1. Place an Order\n";
        std::cout << "2. Serve an Order\n";
        std::cout << "3. Display Pending Orders\n";
        std::cout << "4. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                std::cout << "Enter Order ID to place: ";
                std::cin >> orderID;
                pizzaParlor.placeOrder(orderID);
                break;

            case 2:
                pizzaParlor.serveOrder();
                break;

            case 3:
                pizzaParlor.displayOrders();
                break;

            case 4:
                std::cout << "Exiting the system.\n";
                break;

            default:
                std::cout << "Invalid choice. Please try again.\n";
                break;
        }
    } while (choice != 4);

    return 0;
}

/*
Pizza Parlor Ordering System
1. Place an Order
2. Serve an Order
3. Display Pending Orders
4. Exit
Enter your choice: 1
Enter Order ID to place: 101
Order 101 placed successfully.

Enter your choice: 1
Enter Order ID to place: 102
Order 102 placed successfully.

Enter your choice: 3
Pending orders: 101 102 

Enter your choice: 2
Order 101 served.

Enter your choice: 3
Pending orders: 102 

Enter your choice: 4
Exiting the system.
*/
