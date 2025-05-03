#include <iostream>
#include <stack>
#include <string>
#include <cctype>

using namespace std;

// Node structure for the binary tree
struct Node {
    char data;
    Node* left;
    Node* right;
    
    Node(char val) : data(val), left(nullptr), right(nullptr) {}
};

// Function to check if the character is an operator
bool isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

// Function to get precedence of operators
int precedence(char c) {
    if (c == '*' || c == '/')
        return 2;
    if (c == '+' || c == '-')
        return 1;
    return 0;
}

// Function to construct expression tree from infix expression
Node* constructTree(string infix) {
    stack<Node*> stN; // Stack for nodes
    stack<char> stC;  // Stack for characters
    
    for (int i = 0; i < infix.length(); i++) {
        // Skip spaces
        if (infix[i] == ' ')
            continue;
            
        // If operand, create a node and push to stack
        if (isalnum(infix[i])) {
            Node* t = new Node(infix[i]);
            stN.push(t);
        }
        // If '(', push it to character stack
        else if (infix[i] == '(') {
            stC.push(infix[i]);
        }
        // If ')', pop and create nodes until '(' is found
        else if (infix[i] == ')') {
            while (!stC.empty() && stC.top() != '(') {
                Node* t = new Node(stC.top());
                stC.pop();
                
                t->right = stN.top(); stN.pop();
                t->left = stN.top(); stN.pop();
                
                stN.push(t);
            }
            if (!stC.empty()) stC.pop(); // Remove '('
        }
        // If operator
        else if (isOperator(infix[i])) {
            while (!stC.empty() && stC.top() != '(' && 
                  precedence(infix[i]) <= precedence(stC.top())) {
                Node* t = new Node(stC.top());
                stC.pop();
                
                t->right = stN.top(); stN.pop();
                t->left = stN.top(); stN.pop();
                
                stN.push(t);
            }
            stC.push(infix[i]);
        }
    }
    
    // Process remaining operators in the stack
    while (!stC.empty()) {
        Node* t = new Node(stC.top());
        stC.pop();
        
        t->right = stN.top(); stN.pop();
        t->left = stN.top(); stN.pop();
        
        stN.push(t);
    }
    
    return stN.top();
}

// Non-recursive postorder traversal
void postorderTraversal(Node* root) {
    if (root == nullptr) return;
    
    stack<Node*> s1, s2;
    s1.push(root);
    
    // First stack to second stack
    while (!s1.empty()) {
        Node* temp = s1.top();
        s1.pop();
        s2.push(temp);
        
        if (temp->left)
            s1.push(temp->left);
        if (temp->right)
            s1.push(temp->right);
    }
    
    // Print from second stack
    cout << "Postorder Traversal: ";
    while (!s2.empty()) {
        cout << s2.top()->data << " ";
        s2.pop();
    }
    cout << endl;
}

// Function to free memory
void deleteTree(Node* root) {
    if (root == nullptr) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    string expression;
    cout << "Enter infix expression (e.g., a-b*c-d/e+f): ";
    getline(cin, expression);
    
    // Add parentheses to handle operator precedence correctly
    string modifiedExp = "(" + expression + ")";
    
    Node* root = constructTree(modifiedExp);
    
    cout << "Expression tree constructed successfully!" << endl;
    postorderTraversal(root);
    
    // Clean up
    deleteTree(root);
    
    return 0;
}
