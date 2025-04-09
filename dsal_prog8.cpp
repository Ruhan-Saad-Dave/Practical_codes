#include <iostream>
#include <vector>
#include <climits>
using namespace std;

// Node structure for the Binary Search Tree
struct Node {
    int key;
    Node* left;
    Node* right;
    
    Node(int k) : key(k), left(nullptr), right(nullptr) {}
};

class OptimalBST {
private:
    vector<int> keys;               // The sorted keys
    vector<double> probabilities;   // Probabilities of successful searches
    vector<double> dummyProbs;      // Probabilities of unsuccessful searches
    vector<vector<double>> cost;    // Cost table for DP
    vector<vector<int>> root;       // Root table for tree construction
    
public:
    OptimalBST(const vector<int>& k, const vector<double>& p, const vector<double>& q) 
        : keys(k), probabilities(p), dummyProbs(q) {
        int n = keys.size();
        cost.resize(n + 1, vector<double>(n + 1, 0));
        root.resize(n + 1, vector<int>(n + 1, 0));
    }
    
    // Compute the optimal BST using dynamic programming
    void computeOptimalBST() {
        int n = keys.size();
        
        // Initialize cost for single key subtrees
        for (int i = 0; i < n; i++) {
            cost[i][i+1] = dummyProbs[i];
            root[i][i+1] = i+1;
        }
        cost[n][n] = dummyProbs[n];
        
        // Consider chains of increasing length
        for (int len = 2; len <= n; len++) {
            for (int i = 0; i <= n - len; i++) {
                int j = i + len;
                cost[i][j] = INT_MAX;
                
                double sum = dummyProbs[j];
                for (int k = i + 1; k <= j; k++) {
                    sum += probabilities[k-1];
                }
                
                // Try each key as root and find minimum cost
                for (int r = i + 1; r <= j; r++) {
                    double t = cost[i][r-1] + cost[r][j] + sum;
                    if (t < cost[i][j]) {
                        cost[i][j] = t;
                        root[i][j] = r;
                    }
                }
            }
        }
    }
    
    // Construct the optimal BST recursively
    Node* constructOptimalBST(int i, int j) {
        if (i == j) {
            return nullptr;
        }
        
        int r = root[i][j];
        if (r == 0) {
            return nullptr;
        }
        
        Node* node = new Node(keys[r-1]);
        node->left = constructOptimalBST(i, r-1);
        node->right = constructOptimalBST(r, j);
        
        return node;
    }
    
    // Build and return the optimal BST
    Node* buildOptimalBST() {
        computeOptimalBST();
        return constructOptimalBST(0, keys.size());
    }
    
    // Display the cost and root tables
    void displayTables() {
        int n = keys.size();
        cout << "Cost Table:" << endl;
        for (int i = 0; i <= n; i++) {
            for (int j = i; j <= n; j++) {
                if (j > i) {
                    cout << "cost[" << i << "][" << j << "] = " << cost[i][j] << "\t";
                }
            }
            cout << endl;
        }
        
        cout << "\nRoot Table:" << endl;
        for (int i = 0; i < n; i++) {
            for (int j = i+1; j <= n; j++) {
                cout << "root[" << i << "][" << j << "] = " << root[i][j] << "\t";
            }
            cout << endl;
        }
    }
    
    // Helper functions for tree operations
    
    // Search for a key in the BST
    bool search(Node* root, int key) {
        if (root == nullptr) {
            return false;
        }
        
        if (key == root->key) {
            return true;
        } else if (key < root->key) {
            return search(root->left, key);
        } else {
            return search(root->right, key);
        }
    }
    
    // Insert a key into BST (with rebalancing to maintain optimality)
    Node* insert(Node* root, int key, vector<int>& newKeys, vector<double>& newProbs, vector<double>& newDummy) {
        // Add the new key to the collections
        for (size_t i = 0; i < newKeys.size(); i++) {
            if (key < newKeys[i]) {
                newKeys.insert(newKeys.begin() + i, key);
                // Assign a default probability - in practice, this should be determined properly
                newProbs.insert(newProbs.begin() + i, 0.05); 
                // Adjust dummy probabilities as needed
                newDummy.insert(newDummy.begin() + i, 0.01);
                break;
            }
            if (i == newKeys.size() - 1) {
                newKeys.push_back(key);
                newProbs.push_back(0.05);
                newDummy.push_back(0.01);
                break;
            }
        }
        
        // Rebuild the optimal BST
        OptimalBST newTree(newKeys, newProbs, newDummy);
        return newTree.buildOptimalBST();
    }
    
    // Find the node with minimum value in a BST
    Node* findMin(Node* root) {
        if (root == nullptr) {
            return nullptr;
        }
        while (root->left != nullptr) {
            root = root->left;
        }
        return root;
    }
    
    // Delete a key from BST (with rebalancing to maintain optimality)
    Node* deleteNode(Node* root, int key, vector<int>& newKeys, vector<double>& newProbs, vector<double>& newDummy) {
        // Remove the key from collections
        for (size_t i = 0; i < newKeys.size(); i++) {
            if (newKeys[i] == key) {
                newKeys.erase(newKeys.begin() + i);
                newProbs.erase(newProbs.begin() + i);
                // Adjust dummy probabilities as needed
                if (i < newDummy.size() - 1) {
                    newDummy[i] += newDummy[i+1]/2;
                    newDummy.erase(newDummy.begin() + i + 1);
                }
                break;
            }
        }
        
        // Rebuild the optimal BST
        if (newKeys.empty()) {
            return nullptr;
        }
        OptimalBST newTree(newKeys, newProbs, newDummy);
        return newTree.buildOptimalBST();
    }
    
    // Print the BST in-order
    void inOrderTraversal(Node* root) {
        if (root != nullptr) {
            inOrderTraversal(root->left);
            cout << root->key << " ";
            inOrderTraversal(root->right);
        }
    }
    
    // Print the BST structure
    void printBST(Node* root, int space = 0, int count = 10) {
        if (root == nullptr) {
            return;
        }
        
        space += count;
        printBST(root->right, space, count);
        
        cout << endl;
        for (int i = count; i < space; i++) {
            cout << " ";
        }
        cout << root->key << endl;
        
        printBST(root->left, space, count);
    }
    
    // Get the expected search cost
    double getExpectedCost() {
        return cost[0][keys.size()];
    }
};

// Main function to demonstrate the OBST implementation
int main() {
    // Example usage with sample data
    vector<int> keys = {10, 20, 30, 40, 50};
    vector<double> probabilities = {0.15, 0.10, 0.05, 0.10, 0.20};
    vector<double> dummyProbs = {0.05, 0.10, 0.05, 0.05, 0.05, 0.10};
    
    OptimalBST obst(keys, probabilities, dummyProbs);
    
    cout << "Computing Optimal Binary Search Tree..." << endl;
    Node* root = obst.buildOptimalBST();
    
    cout << "Tables used in computation:" << endl;
    obst.displayTables();
    
    cout << "\nOptimal BST structure:" << endl;
    obst.printBST(root);
    
    cout << "\nIn-order traversal of the Optimal BST: ";
    obst.inOrderTraversal(root);
    cout << endl;
    
    cout << "\nExpected search cost: " << obst.getExpectedCost() << endl;
    
    // Demonstrate search operation
    int searchKey = 30;
    cout << "\nSearching for key " << searchKey << ": ";
    if (obst.search(root, searchKey)) {
        cout << "Found!" << endl;
    } else {
        cout << "Not found!" << endl;
    }
    
    // Demonstrate insert operation
    cout << "\nInserting key 25..." << endl;
    vector<int> newKeys = keys;
    vector<double> newProbs = probabilities;
    vector<double> newDummy = dummyProbs;
    Node* newRoot = obst.insert(root, 25, newKeys, newProbs, newDummy);
    
    cout << "Updated BST structure:" << endl;
    obst.printBST(newRoot);
    
    // Demonstrate delete operation
    cout << "\nDeleting key 30..." << endl;
    vector<int> delKeys = newKeys;
    vector<double> delProbs = newProbs;
    vector<double> delDummy = newDummy;
    Node* finalRoot = obst.deleteNode(newRoot, 30, delKeys, delProbs, delDummy);
    
    cout << "Final BST structure:" << endl;
    obst.printBST(finalRoot);
    
    cout << "\nIn-order traversal of the final BST: ";
    obst.inOrderTraversal(finalRoot);
    cout << endl;
    
    return 0;
}
