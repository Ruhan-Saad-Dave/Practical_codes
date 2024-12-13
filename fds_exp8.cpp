/*
Second year Computer Engineering class, set A of students like vanilla Ice-Cream and set B of students like butterscotch ice-cream. Write C++ program to store two sets using linked list. Compute and display
a) Set of students who like both vanilla and butterscotch
b) Set of students who like either vanilla or butterscotch or not both
c) Number of students who like neither vanilla not butterscotch.
*/

#include <iostream>
using namespace std;

class LinkedList {
private:
    class Node {
    public:
        int data;
        Node* next;
        Node(int val) : data(val), next(nullptr) {}
    };

    Node* head;

public:
    LinkedList() : head(nullptr) {}

    // Insert a new element into the list
    void insert(int value) {
        Node* newNode = new Node(value);
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

    // Check if an element exists in the list
    bool exists(int value) const {
        Node* temp = head;
        while (temp) {
            if (temp->data == value) {
                return true;
            }
            temp = temp->next;
        }
        return false;
    }

    // Compute the intersection of two lists
    LinkedList intersection(const LinkedList& other) const {
        LinkedList result;
        Node* temp = head;
        while (temp) {
            if (other.exists(temp->data)) {
                result.insert(temp->data);
            }
            temp = temp->next;
        }
        return result;
    }

    // Compute the symmetric difference of two lists
    LinkedList symmetricDifference(const LinkedList& other) const {
        LinkedList result;
        Node* temp = head;
        while (temp) {
            if (!other.exists(temp->data)) {
                result.insert(temp->data);
            }
            temp = temp->next;
        }
        temp = other.head;
        while (temp) {
            if (!exists(temp->data)) {
                result.insert(temp->data);
            }
            temp = temp->next;
        }
        return result;
    }

    // Count elements not in two other lists
    int countNotIn(const LinkedList& listA, const LinkedList& listB) const {
        int count = 0;
        Node* temp = head;
        while (temp) {
            if (!listA.exists(temp->data) && !listB.exists(temp->data)) {
                count++;
            }
            temp = temp->next;
        }
        return count;
    }

    // Display the elements of the list
    void display() const {
        Node* temp = head;
        while (temp) {
            cout << temp->data << " ";
            temp = temp->next;
        }
        cout << endl;
    }

    ~LinkedList() {
        Node* temp;
        while (head) {
            temp = head;
            head = head->next;
            delete temp;
        }
    }
};

int main() {
    LinkedList setA; // Students who like vanilla
    LinkedList setB; // Students who like butterscotch
    LinkedList allStudents; // All students in the class

    int n, m, totalStudents, value;

    // Input total number of students
    cout << "Enter total number of students in the class: ";
    cin >> totalStudents;

    cout << "Enter the roll numbers of all students: ";
    for (int i = 0; i < totalStudents; ++i) {
        cin >> value;
        allStudents.insert(value);
    }

    // Input set A (vanilla lovers)
    cout << "Enter the number of students who like vanilla: ";
    cin >> n;
    cout << "Enter their roll numbers: ";
    for (int i = 0; i < n; ++i) {
        cin >> value;
        setA.insert(value);
    }

    // Input set B (butterscotch lovers)
    cout << "Enter the number of students who like butterscotch: ";
    cin >> m;
    cout << "Enter their roll numbers: ";
    for (int i = 0; i < m; ++i) {
        cin >> value;
        setB.insert(value);
    }

    // Compute and display intersection
    LinkedList both = setA.intersection(setB);
    cout << "Students who like both vanilla and butterscotch: ";
    both.display();

    // Compute and display symmetric difference
    LinkedList eitherOr = setA.symmetricDifference(setB);
    cout << "Students who like either vanilla or butterscotch but not both: ";
    eitherOr.display();

    // Compute and display count of students who like neither
    int neitherCount = allStudents.countNotIn(setA, setB);
    cout << "Number of students who like neither vanilla nor butterscotch: " << neitherCount << endl;

    return 0;
}

/*
Students who like both vanilla and butterscotch: 3 4 
Students who like either vanilla or butterscotch but not both: 1 2 5 6 
Number of students who like neither vanilla nor butterscotch: 4
*/
