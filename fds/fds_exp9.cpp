/*
A palindrome is a string of character that's the same forward and backward. Typically, punctuation, capitalization, and spaces are ignored. For example, "Poor Dan is in a droop" is a palindrome, as can be seen by examining the characters "poor danisina droop" and observing that they are the same forward and backward. One way to check for a palindrome is to reserve the characters in the string and then compare with them the original-in a palindrome, the sequence will be identical. Write C++ program with functions:
a) To print original string followed by reversed string using stack.
b) To check wether given string is palindrome or not.
*/

#include <iostream>
using namespace std;

// Stack class definition
class Stack {
private:
    char* arr;
    int top;
    int capacity;

public:
    // Constructor
    Stack(int size) {
        arr = new char[size];
        top = -1;
        capacity = size;
    }

    // Destructor
    ~Stack() {
        delete[] arr;
    }

    // Push an element onto the stack
    void push(char element) {
        if (top < capacity - 1) {
            arr[++top] = element;
        } else {
            cout << "Stack Overflow" << endl;
        }
    }

    // Pop an element from the stack
    char pop() {
        if (top >= 0) {
            return arr[top--];
        } else {
            cout << "Stack Underflow" << endl;
            return '\0'; // Return null character on underflow
        }
    }

    // Check if the stack is empty
    bool isEmpty() {
        return top == -1;
    }
};

// Function to check if a string is a palindrome
bool isPalindrome(const string& str) {
    int length = str.length();
    Stack stack(length);

    // Push all characters onto the stack
    for (int i = 0; i < length; i++) {
        stack.push(str[i]);
    }

    // Compare characters popped from the stack with the original string
    for (int i = 0; i < length; i++) {
        if (str[i] != stack.pop()) {
            return false;
        }
    }

    return true;
}

int main() {
    string input;

    // Input the string
    cout << "Enter a string: ";
    cin >> input;

    // Check if the string is a palindrome
    if (isPalindrome(input)) {
        cout << "The string is a palindrome." << endl;
    } else {
        cout << "The string is not a palindrome." << endl;
    }

    return 0;
}

//Enter a string: 1234554321
//The string is a palindrome.

/*
Enter a string: Poor Dan is in a droop
Original cleaned string: poordanisinadroop
Reversed string: poordanisinadroop
The given string is a palindrome.
*/
