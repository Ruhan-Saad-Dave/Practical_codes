/*
Create a class that includees the data items (which should be template) and the priority (which should be int). The inorder list list should contain these objects, with operator <= overloaded so that the items with highest priority appear at the beginning of the list (which will make it relatively easy to retrive the highest item.)
*/

#include <iostream>
#include <list>

template <typename T>
class PriorityItem {
public:
    T data;       // The data item, templated for any data type
    int priority; // Priority of the item

    // Constructor to initialize the data and priority
    PriorityItem(T data, int priority) : data(data), priority(priority) {}

    // Overload the <= operator for comparison based on priority
    bool operator<=(const PriorityItem<T>& other) const {
        return this->priority >= other.priority;
    }
};

// Function to insert items into the list in order based on priority
template <typename T>
void insertInOrder(std::list<PriorityItem<T>>& itemList, const PriorityItem<T>& item) {
    // Find the correct position to insert the new item
    auto it = itemList.begin();
    while (it != itemList.end() && *it <= item) {
        ++it;
    }
    // Insert item at the correct position
    itemList.insert(it, item);
}

// Function to display items in the list
template <typename T>
void displayList(const std::list<PriorityItem<T>>& itemList) {
    for (const auto& item : itemList) {
        std::cout << "Data: " << item.data << ", Priority: " << item.priority << std::endl;
    }
}

int main() {
    // Create an inorder list to store PriorityItem objects
    std::list<PriorityItem<std::string>> itemList;

    // Insert some items with different priorities
    insertInOrder(itemList, PriorityItem<std::string>("Item A", 2));
    insertInOrder(itemList, PriorityItem<std::string>("Item B", 5));
    insertInOrder(itemList, PriorityItem<std::string>("Item C", 1));
    insertInOrder(itemList, PriorityItem<std::string>("Item D", 4));

    // Display the list (highest priority items should appear first)
    std::cout << "Inorder List by Priority:" << std::endl;
    displayList(itemList);

    return 0;
}

/*
Inorder List by Priority:
Data: Item B, Priority: 5
Data: Item D, Priority: 4
Data: Item A, Priority: 2
Data: Item C, Priority: 1
*/
