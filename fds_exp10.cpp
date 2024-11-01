/*
Implement C++ program for expression conversion as infix to postfix and its evaluation using stack based on given conditions:
1) Operands and operators, both must be single character
2) Input Postfix expression must be in a desired format
3) Only '+', '-', '*' and '/' operators are expected.
*/

#include <iostream>
#include <stack>
#include <string>
#include <cctype> // for isdigit

// Function to get the precedence of operators
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Function to convert infix expression to postfix expression
std::string infixToPostfix(const std::string& infix) {
    std::stack<char> operators;
    std::string postfix;

    for (char ch : infix) {
        if (std::isdigit(ch) || std::isalpha(ch)) { // Operand
            postfix += ch;
        } else if (ch == '(') {
            operators.push(ch);
        } else if (ch == ')') {
            // Pop operators until '('
            while (!operators.empty() && operators.top() != '(') {
                postfix += operators.top();
                operators.pop();
            }
            operators.pop(); // Remove '('
        } else { // Operator
            while (!operators.empty() && precedence(operators.top()) >= precedence(ch)) {
                postfix += operators.top();
                operators.pop();
            }
            operators.push(ch);
        }
    }

    // Pop remaining operators
    while (!operators.empty()) {
        postfix += operators.top();
        operators.pop();
    }

    return postfix;
}

// Function to evaluate postfix expression
int evaluatePostfix(const std::string& postfix) {
    std::stack<int> values;

    for (char ch : postfix) {
        if (std::isdigit(ch)) { // Operand (assuming single-digit values)
            values.push(ch - '0'); // Convert char to int
        } else { // Operator
            int right = values.top(); values.pop();
            int left = values.top(); values.pop();

            switch (ch) {
                case '+': values.push(left + right); break;
                case '-': values.push(left - right); break;
                case '*': values.push(left * right); break;
                case '/': values.push(left / right); break;
            }
        }
    }

    return values.top();
}

int main() {
    std::string infix;
    std::cout << "Enter an infix expression: ";
    std::cin >> infix;

    std::string postfix = infixToPostfix(infix);
    std::cout << "Postfix expression: " << postfix << std::endl;

    int result = evaluatePostfix(postfix);
    std::cout << "Evaluation result: " << result << std::endl;

    return 0;
}

/*
Enter an infix expression: (2+3)*(4-5)/2
Postfix expression: 23+45-*2/
Evaluation result: -2
*/
