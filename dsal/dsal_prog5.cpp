#include <iostream>
#include <functional>
#include <queue>
using namespace std;

// Node structure for threaded binary tree
struct Node {
    int data;
    Node *left, *right;
    bool rightThread; // True if right points to successor
    
    Node(int val) : data(val), left(nullptr), right(nullptr), rightThread(false) {}
};

// Function to find inorder successor using rightThread
Node* inorderSuccessor(Node* ptr) {
    if (ptr->rightThread)
        return ptr->right;
    
    ptr = ptr->right;
    while (ptr->left)
        ptr = ptr->left;
    return ptr;
}

// Convert binary tree to threaded binary tree
void createThreadedTree(Node* root, Node*& prev) {
    if (!root) return;
    
    createThreadedTree(root->left, prev);
    
    if (!root->right) {
        root->right = prev;
        root->rightThread = true;
    }
    
    prev = root;
    
    if (!root->rightThread)
        createThreadedTree(root->right, prev);
}

// Function to traverse threaded binary tree
void inorderTraversal(Node* root) {
    if (!root) return;
    
    // Find leftmost node
    Node* current = root;
    while (current->left)
        current = current->left;
    
    // Traverse the tree
    while (current) {
        cout << current->data << " ";
        
        // Find the inorder successor
        if (current->rightThread)
            current = current->right;
        else {
            current = current->right;
            while (current && current->left)
                current = current->left;
        }
    }
}

// Create a binary tree from user input
Node* createUserBinaryTree() {
    cout << "Enter the number of nodes in the binary tree: ";
    int n;
    cin >> n;
    
    if (n <= 0) return nullptr;
    
    cout << "Enter the value of root node: ";
    int rootValue;
    cin >> rootValue;
    Node* root = new Node(rootValue);
    
    // Use level order insertion (queue-based)
    queue<Node*> q;
    q.push(root);
    
    for (int i = 1; i < n; i++) {
        Node* current = q.front();
        q.pop();
        
        int leftValue, rightValue;
        cout << "Enter left child value for node " << current->data 
             << " (-1 for no child): ";
        cin >> leftValue;
        
        if (leftValue != -1) {
            current->left = new Node(leftValue);
            q.push(current->left);
        }
        
        if (i < n) {
            cout << "Enter right child value for node " << current->data 
                 << " (-1 for no child): ";
            cin >> rightValue;
            
            if (rightValue != -1) {
                current->right = new Node(rightValue);
                q.push(current->right);
            }
            
            i++; // Count this as another node
        }
        
        // Break if we've created all nodes
        if (i >= n) break;
    }
    
    return root;
}

// Regular inorder traversal using recursion
void inorderRecursive(Node* node) {
    if (!node) return;
    inorderRecursive(node->left);
    cout << node->data << " ";
    inorderRecursive(node->right);
}

// Main function
int main() {
    cout << "=== Threaded Binary Tree Converter ===" << endl;
    
    // Get user input for binary tree
    Node* root = createUserBinaryTree();
    
    if (!root) {
        cout << "No tree created. Exiting." << endl;
        return 0;
    }
    
    cout << "\nOriginal binary tree (inorder): ";
    inorderRecursive(root);
    cout << endl;
    
    // Convert to threaded binary tree
    Node* prev = nullptr;
    createThreadedTree(root, prev);
    
    cout << "Threaded binary tree (inorder): ";
    inorderTraversal(root);
    cout << endl;
    
    return 0;
}
