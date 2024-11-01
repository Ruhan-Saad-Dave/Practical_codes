/*
A palindrome is a string of character that's the same forward and backward. Typically, punctuation, capitalization, and spaces are ignored. For example, "Poor Dan is in a droop" is a palindrome, as can be seen by examining the characters "poor danisina droop" and observing that they are the same forward and backward. One way to check for a palindrome is to reserve the characters in the string and then compare with them the original-in a palindrome, the sequence will be identical. Write C++ program with functions:
a) To print original string followed by reversed string using stack.
b) To check wether given string is palindrome or not.
*/

#include <iostream>
#include <stack>
#include <cctype>   // for isalpha, tolower

// Function to preprocess the string: removes non-alphabetic characters and converts to lowercase
std::string preprocess(const std::string& str) {
    std::string result;
    for (char ch : str) {
        if (std::isalpha(ch)) {            // Check if character is alphabetic
            result += std::tolower(ch);     // Convert to lowercase
        }
    }
    return result;
}

// Function to reverse a string using a stack
std::string reverseUsingStack(const std::string& str) {
    std::stack<char> stack;
    for (char ch : str) {
        stack.push(ch);
    }

    std::string reversedStr;
    while (!stack.empty()) {
        reversedStr += stack.top();
        stack.pop();
    }
    return reversedStr;
}

// Function to check if a string is a palindrome
bool isPalindrome(const std::string& str) {
    std::string cleanedStr = preprocess(str);         // Remove non-alphabetic characters and convert to lowercase
    std::string reversedStr = reverseUsingStack(cleanedStr); // Reverse the cleaned string
    return cleanedStr == reversedStr;                 // Check if original and reversed strings are the same
}

int main() {
    std::string input;
    std::cout << "Enter a string: ";
    std::getline(std::cin, input);

    // Preprocess and display original and reversed strings
    std::string cleanedStr = preprocess(input);
    std::string reversedStr = reverseUsingStack(cleanedStr);

    std::cout << "Original cleaned string: " << cleanedStr << std::endl;
    std::cout << "Reversed string: " << reversedStr << std::endl;

    // Check if the cleaned input is a palindrome
    if (isPalindrome(input)) {
        std::cout << "The given string is a palindrome." << std::endl;
    } else {
        std::cout << "The given string is not a palindrome." << std::endl;
    }

    return 0;
}

/*
Enter a string: Poor Dan is in a droop
Original cleaned string: poordanisinadroop
Reversed string: poordanisinadroop
The given string is a palindrome.
*/
