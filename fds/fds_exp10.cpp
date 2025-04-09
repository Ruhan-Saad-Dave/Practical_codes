/*
Implement C++ program for expression conversion as infix to postfix and its evaluation using stack based on given conditions:
1) Operands and operators, both must be single character
2) Input Postfix expression must be in a desired format
3) Only '+', '-', '*' and '/' operators are expected.
*/

#include <iostream>
#include <cctype>
using namespace std;

class Stack {
private:
    int top;
    char arr[100]; // Stack size limit

public:
    Stack() { top = -1; }

    void push(char c) {
        if (top >= 99) {
            cout << "Stack Overflow" << endl;
            return;
        }
        arr[++top] = c;
    }

    char pop() {
        if (top < 0) {
            cout << "Stack Underflow" << endl;
            return -1;
        }
        return arr[top--];
    }

    char peek() {
        if (top < 0) return -1;
        return arr[top];
    }

    bool isEmpty() {
        return top == -1;
    }
};

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

string infixToPostfix(string infix) {
    Stack stack;
    string postfix = "";

    for (char c : infix) {
        if (isalnum(c)) {
            // Operand directly to output
            postfix += c;
        } else if (c == '(') {
            // Push '(' to stack
            stack.push(c);
        } else if (c == ')') {
            // Pop and add to output until '(' is found
            while (!stack.isEmpty() && stack.peek() != '(') {
                postfix += stack.pop();
            }
            stack.pop(); // Remove '('
        } else if (isOperator(c)) {
            // Operator: pop from stack to output if precedence is higher or equal
            while (!stack.isEmpty() && precedence(stack.peek()) >= precedence(c)) {
                postfix += stack.pop();
            }
            stack.push(c);
        }
    }

    // Pop remaining operators from the stack
    while (!stack.isEmpty()) {
        postfix += stack.pop();
    }

    return postfix;
}

int evaluatePostfix(string postfix) {
    Stack stack;

    for (char c : postfix) {
        if (isdigit(c)) {
            // Push operand to stack
            stack.push(c - '0');
        } else if (isOperator(c)) {
            // Pop two operands
            int b = stack.pop();
            int a = stack.pop();

            switch (c) {
            case '+': stack.push(a + b); break;
            case '-': stack.push(a - b); break;
            case '*': stack.push(a * b); break;
            case '/': stack.push(a / b); break;
            }
        }
    }

    // Result will be the last element in the stack
    return stack.pop();
}

int main() {
    string infix;
    cout << "Enter an infix expression: ";
    cin >> infix;

    string postfix = infixToPostfix(infix);
    cout << "Postfix expression: " << postfix << endl;

    int result = evaluatePostfix(postfix);
    cout << "Evaluation result: " << result << endl;

    return 0;
}

/*
Enter an infix expression: (2+3)*(4-5)/2
Postfix expression: 23+45-*2/
Evaluation result: -2
*/
