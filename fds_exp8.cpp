/*
Second year Computer Engineering class, set A of students like vanilla Ice-Cream and set B of students like butterscotch ice-cream. Write C++ program to store two sets using linked list. Compute and display
a) Set of students who like both vanilla and butterscotch
b) Set of students who like either vanilla or butterscotch or not both
c) Number of students who like neither vanilla not butterscotch.
*/

#include <iostream>
#include <unordered_set>

struct Node {
    int studentID;
    Node* next;
    
    Node(int id) : studentID(id), next(nullptr) {}
};

class LinkedList {
public:
    Node* head;

    LinkedList() : head(nullptr) {}

    // Insert a student ID at the end of the list
    void insert(int id) {
        Node* newNode = new Node(id);
        if (!head) {
            head = newNode;
            return;
        }
        Node* temp = head;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = newNode;
    }

    // Check if a student ID exists in the list
    bool contains(int id) {
        Node* temp = head;
        while (temp) {
            if (temp->studentID == id) return true;
            temp = temp->next;
        }
        return false;
    }

    // Display the list
    void display() {
        Node* temp = head;
        while (temp) {
            std::cout << temp->studentID << " ";
            temp = temp->next;
        }
        std::cout << std::endl;
    }

    // Count the elements in the list
    int count() {
        int count = 0;
        Node* temp = head;
        while (temp) {
            count++;
            temp = temp->next;
        }
        return count;
    }
};

// Set of students who like both vanilla and butterscotch (Intersection)
LinkedList intersection(LinkedList& setA, LinkedList& setB) {
    LinkedList result;
    Node* temp = setA.head;
    while (temp) {
        if (setB.contains(temp->studentID)) {
            result.insert(temp->studentID);
        }
        temp = temp->next;
    }
    return result;
}

// Set of students who like either vanilla or butterscotch but not both (Symmetric Difference)
LinkedList symmetricDifference(LinkedList& setA, LinkedList& setB) {
    LinkedList result;
    Node* temp = setA.head;
    while (temp) {
        if (!setB.contains(temp->studentID)) {
            result.insert(temp->studentID);
        }
        temp = temp->next;
    }
    temp = setB.head;
    while (temp) {
        if (!setA.contains(temp->studentID)) {
            result.insert(temp->studentID);
        }
        temp = temp->next;
    }
    return result;
}

// Number of students who like neither vanilla nor butterscotch
int countNeither(LinkedList& setA, LinkedList& setB, int totalStudents) {
    std::unordered_set<int> students;
    Node* temp = setA.head;
    while (temp) {
        students.insert(temp->studentID);
        temp = temp->next;
    }
    temp = setB.head;
    while (temp) {
        students.insert(temp->studentID);
        temp = temp->next;
    }
    return totalStudents - students.size();
}

int main() {
    LinkedList vanillaLovers;
    LinkedList butterscotchLovers;

    // Sample data
    vanillaLovers.insert(1);
    vanillaLovers.insert(2);
    vanillaLovers.insert(3);
    vanillaLovers.insert(4);

    butterscotchLovers.insert(3);
    butterscotchLovers.insert(4);
    butterscotchLovers.insert(5);
    butterscotchLovers.insert(6);

    int totalStudents = 10;  // Total students in the class

    // (a) Students who like both vanilla and butterscotch
    LinkedList both = intersection(vanillaLovers, butterscotchLovers);
    std::cout << "Students who like both vanilla and butterscotch: ";
    both.display();

    // (b) Students who like either vanilla or butterscotch but not both
    LinkedList eitherOrNotBoth = symmetricDifference(vanillaLovers, butterscotchLovers);
    std::cout << "Students who like either vanilla or butterscotch but not both: ";
    eitherOrNotBoth.display();

    // (c) Number of students who like neither vanilla nor butterscotch
    int neitherCount = countNeither(vanillaLovers, butterscotchLovers, totalStudents);
    std::cout << "Number of students who like neither vanilla nor butterscotch: " << neitherCount << std::endl;

    return 0;
}

/*
Students who like both vanilla and butterscotch: 3 4 
Students who like either vanilla or butterscotch but not both: 1 2 5 6 
Number of students who like neither vanilla nor butterscotch: 4
*/
