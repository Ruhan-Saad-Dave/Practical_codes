/*
write cpp code to implement a priority queue using an inorder list to stor the items in the queue. create a class that includes the dataitems (which should be template) and the priority (which should be int). The order list should contain these objects with the operator <= overloaded so that items with highest priority appear at the beginning of list (which will make it relatively easy to retrievee the highest item) 
*/

#include <iostream>
#include <list>
using namespace std;

// Template class for an item with priority
template <typename T>
class PriorityItem {
public:
    T data;           // Template data item
    int priority;     // Priority of the item

    // Constructor
    PriorityItem(T data, int priority) : data(data), priority(priority) {}

    // Overload <= operator for comparison based on priority
    bool operator<=(const PriorityItem& other) const {
        return this->priority >= other.priority; // Higher priority comes first
    }
};

// Function to print the priority list
template <typename T>
void printList(const list<PriorityItem<T>>& itemList) {
    for (const auto& item : itemList) {
        cout << "Data: " << item.data << ", Priority: " << item.priority << endl;
    }
}

int main() {
    list<PriorityItem<string>> priorityList; // List of PriorityItem objects

    // Add items to the list
    priorityList.push_back(PriorityItem<string>("Task A", 3));
    priorityList.push_back(PriorityItem<string>("Task B", 5));
    priorityList.push_back(PriorityItem<string>("Task C", 1));
    priorityList.push_back(PriorityItem<string>("Task D", 4));

    // Sort the list based on priority
    priorityList.sort([](const PriorityItem<string>& a, const PriorityItem<string>& b) {
        return a <= b; // Use the overloaded <= operator
    });

    // Print the sorted list
    cout << "Priority List (Highest Priority First):" << endl;
    printList(priorityList);

    return 0;
}


/*
Inorder List by Priority:
Data: Item B, Priority: 5
Data: Item D, Priority: 4
Data: Item A, Priority: 2
Data: Item C, Priority: 1
*/
