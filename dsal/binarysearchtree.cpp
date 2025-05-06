#include <iostream>
#include <climits>
using namespace std;

// Node structure for BST
struct Node {
    int data;
    Node* left;
    Node* right;
    
    Node(int val) {
        data = val;
        left = right = nullptr;
    }
};

// Binary Search Tree class
class BST {
private:
    Node* root;
    
    // Private helper methods
    Node* insertHelper(Node* node, int val) {
        if (node == nullptr)
            return new Node(val);
        
        if (val < node->data)
            node->left = insertHelper(node->left, val);
        else if (val > node->data)
            node->right = insertHelper(node->right, val);
        
        return node;
    }
    
    bool searchHelper(Node* node, int val) {
        if (node == nullptr)
            return false;
        
        if (node->data == val)
            return true;
        
        if (val < node->data)
            return searchHelper(node->left, val);
        else
            return searchHelper(node->right, val);
    }
    
    void inOrderHelper(Node* node) {
        if (node == nullptr)
            return;
        
        inOrderHelper(node->left);
        cout << node->data << " ";
        inOrderHelper(node->right);
    }
    
    int findHeightHelper(Node* node) {
        if (node == nullptr)
            return -1;
        
        int leftHeight = findHeightHelper(node->left);
        int rightHeight = findHeightHelper(node->right);
        
        return max(leftHeight, rightHeight) + 1;
    }
    
    int findMinValueHelper(Node* node) {
        if (node == nullptr)
            return INT_MAX;
        
        if (node->left == nullptr)
            return node->data;
        
        return findMinValueHelper(node->left);
    }
    
    Node* swapPointersHelper(Node* node) {
        if (node == nullptr)
            return nullptr;
        
        // Swap left and right pointers
        Node* temp = node->left;
        node->left = node->right;
        node->right = temp;
        
        // Recursively swap for children
        swapPointersHelper(node->left);
        swapPointersHelper(node->right);
        
        return node;
    }
    
public:
    BST() {
        root = nullptr;
    }
    
    // Insert a value into the BST
    void insert(int val) {
        root = insertHelper(root, val);
        cout << val << " inserted successfully." << endl;
    }
    
    // Search for a value in the BST
    bool search(int val) {
        return searchHelper(root, val);
    }
    
    // Display the BST using inorder traversal
    void inOrder() {
        cout << "In-order traversal: ";
        inOrderHelper(root);
        cout << endl;
    }
    
    // Find the height of the tree (longest path)
    int findHeight() {
        return findHeightHelper(root);
    }
    
    // Find the number of nodes in the longest path
    int longestPathNodes() {
        return findHeight() + 1;
    }
    
    // Find the minimum value in the BST
    int findMinValue() {
        if (root == nullptr)
            return INT_MAX;
        return findMinValueHelper(root);
    }
    
    // Swap left and right pointers at every node
    void swapPointers() {
        root = swapPointersHelper(root);
    }
};

int main() {
    BST tree;
    int choice, val;
    
    while (true) {
        cout << "\n--- Binary Search Tree Operations ---" << endl;
        cout << "1. Insert a value" << endl;
        cout << "2. Search a value" << endl;
        cout << "3. Display in-order traversal" << endl;
        cout << "4. Find number of nodes in longest path" << endl;
        cout << "5. Find minimum value in the tree" << endl;
        cout << "6. Swap left and right pointers at every node" << endl;
        cout << "7. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                cout << "Enter value to insert: ";
                cin >> val;
                tree.insert(val);
                break;
                
            case 2:
                cout << "Enter value to search: ";
                cin >> val;
                if (tree.search(val))
                    cout << val << " found in the tree." << endl;
                else
                    cout << val << " not found in the tree." << endl;
                break;
                
            case 3:
                tree.inOrder();
                break;
                
            case 4:
                cout << "Number of nodes in the longest path: " << tree.longestPathNodes() << endl;
                break;
                
            case 5:
                if (tree.findMinValue() != INT_MAX)
                    cout << "Minimum value in the tree: " << tree.findMinValue() << endl;
                else
                    cout << "Tree is empty." << endl;
                break;
                
            case 6:
                tree.swapPointers();
                cout << "Tree pointers swapped. The tree is now mirrored." << endl;
                break;
                
            case 7:
                cout << "Exiting program." << endl;
                return 0;
                
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }
    
    return 0;
}
