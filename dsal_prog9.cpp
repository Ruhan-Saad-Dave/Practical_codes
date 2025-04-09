#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>
using namespace std;

class DictionaryNode {
public:
    string keyword, meaning;
    DictionaryNode *left, *right;
    int height;
    
    DictionaryNode(string key, string mean) : keyword(key), meaning(mean), left(nullptr), right(nullptr), height(1) {}
};

class Dictionary {
private:
    DictionaryNode* root;
    int comparisons;

    int getHeight(DictionaryNode* node) { return node ? node->height : 0; }
    
    int getBalanceFactor(DictionaryNode* node) { return node ? getHeight(node->left) - getHeight(node->right) : 0; }
    
    void updateHeight(DictionaryNode* node) {
        if (node) node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    }
    
    DictionaryNode* rotate(DictionaryNode* node, bool isRight) {
        DictionaryNode *newRoot = isRight ? node->left : node->right;
        DictionaryNode *subtree = isRight ? newRoot->right : newRoot->left;
        
        if (isRight) {
            newRoot->right = node;
            node->left = subtree;
        } else {
            newRoot->left = node;
            node->right = subtree;
        }
        
        updateHeight(node);
        updateHeight(newRoot);
        return newRoot;
    }
    
    DictionaryNode* insertNode(DictionaryNode* node, string key, string mean) {
        if (!node) return new DictionaryNode(key, mean);
        
        comparisons++;
        if (key < node->keyword)
            node->left = insertNode(node->left, key, mean);
        else if (key > node->keyword)
            node->right = insertNode(node->right, key, mean);
        else {
            node->meaning = mean;
            return node;
        }
        
        updateHeight(node);
        int balance = getBalanceFactor(node);
        
        // Left Heavy
        if (balance > 1) {
            if (key > node->left->keyword)
                node->left = rotate(node->left, false);
            return rotate(node, true);
        }
        
        // Right Heavy
        if (balance < -1) {
            if (key < node->right->keyword)
                node->right = rotate(node->right, true);
            return rotate(node, false);
        }
        
        return node;
    }
    
    DictionaryNode* minValueNode(DictionaryNode* node) {
        while (node && node->left) node = node->left;
        return node;
    }
    
    DictionaryNode* deleteNode(DictionaryNode* root, string key) {
        if (!root) return nullptr;
        
        comparisons++;
        if (key < root->keyword)
            root->left = deleteNode(root->left, key);
        else if (key > root->keyword)
            root->right = deleteNode(root->right, key);
        else {
            if (!root->left || !root->right) {
                DictionaryNode* temp = root->left ? root->left : root->right;
                if (!temp) {
                    temp = root;
                    root = nullptr;
                } else {
                    *root = *temp;
                }
                delete temp;
            } else {
                DictionaryNode* temp = minValueNode(root->right);
                root->keyword = temp->keyword;
                root->meaning = temp->meaning;
                root->right = deleteNode(root->right, temp->keyword);
            }
        }
        
        if (!root) return nullptr;
        
        updateHeight(root);
        int balance = getBalanceFactor(root);
        
        // Balance the tree if needed
        if (balance > 1) {
            if (getBalanceFactor(root->left) < 0)
                root->left = rotate(root->left, false);
            return rotate(root, true);
        }
        
        if (balance < -1) {
            if (getBalanceFactor(root->right) > 0)
                root->right = rotate(root->right, true);
            return rotate(root, false);
        }
        
        return root;
    }
    
    DictionaryNode* search(DictionaryNode* root, string key) {
        comparisons = 0;
        
        while (root && root->keyword != key) {
            comparisons++;
            root = (key < root->keyword) ? root->left : root->right;
        }
        comparisons++;
        return root;
    }
    
    void traverse(DictionaryNode* root, bool ascending) {
        if (!root) return;
        
        if (ascending) {
            traverse(root->left, ascending);
            cout << root->keyword << ": " << root->meaning << endl;
            traverse(root->right, ascending);
        } else {
            traverse(root->right, ascending);
            cout << root->keyword << ": " << root->meaning << endl;
            traverse(root->left, ascending);
        }
    }
    
    int countNodes(DictionaryNode* root) {
        return root ? 1 + countNodes(root->left) + countNodes(root->right) : 0;
    }
    
    void destroyTree(DictionaryNode* node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

public:
    Dictionary() : root(nullptr), comparisons(0) {}
    ~Dictionary() { destroyTree(root); }
    
    void insert(string key, string mean) {
        comparisons = 0;
        root = insertNode(root, key, mean);
        cout << "Keyword '" << key << "' inserted/updated with " << comparisons << " comparisons." << endl;
    }
    
    void remove(string key) {
        DictionaryNode* node = search(root, key);
        if (!node) {
            cout << "Keyword '" << key << "' not found." << endl;
        } else {
            root = deleteNode(root, key);
            cout << "Keyword '" << key << "' deleted with " << comparisons << " comparisons." << endl;
        }
    }
    
    void update(string key, string newMeaning) {
        DictionaryNode* node = search(root, key);
        if (!node) {
            cout << "Keyword '" << key << "' not found." << endl;
        } else {
            node->meaning = newMeaning;
            cout << "Meaning updated with " << comparisons << " comparisons." << endl;
        }
    }
    
    void display(bool ascending = true) {
        if (!root) {
            cout << "Dictionary is empty." << endl;
            return;
        }
        cout << "\nDictionary in " << (ascending ? "ascending" : "descending") << " order:" << endl;
        traverse(root, ascending);
    }
    
    void searchKeyword(string key) {
        DictionaryNode* result = search(root, key);
        if (!result) {
            cout << "Keyword '" << key << "' not found." << endl;
        } else {
            cout << "Found: " << result->keyword << ": " << result->meaning << endl;
            cout << "Comparisons: " << comparisons << endl;
        }
    }
    
    void analyzeComplexity() {
        int height = getHeight(root);
        int nodeCount = countNodes(root);
        cout << "\nAVL Tree Analysis:" << endl;
        cout << "Height: " << height << ", Nodes: " << nodeCount << endl;
        cout << "Max comparisons: " << height << ", Theoretical max height: " 
             << ceil(1.44 * log2(nodeCount + 2) - 0.328) << endl;
    }
};

int main() {
    Dictionary dict;
    int choice;
    string keyword, meaning;
    
    cout << "\n====== AVL Tree Dictionary ======" << endl;
    
    do {
        cout << "\nMenu: 1-Add 2-Delete 3-Update 4-Search 5-Display(asc) "
             << "6-Display(desc) 7-Analyze 8-Exit\nChoice: ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                cout << "Keyword: ";
                cin.ignore();
                getline(cin, keyword);
                cout << "Meaning: ";
                getline(cin, meaning);
                dict.insert(keyword, meaning);
                break;
                
            case 2:
                cout << "Keyword to delete: ";
                cin.ignore();
                getline(cin, keyword);
                dict.remove(keyword);
                break;
                
            case 3:
                cout << "Keyword to update: ";
                cin.ignore();
                getline(cin, keyword);
                cout << "New meaning: ";
                getline(cin, meaning);
                dict.update(keyword, meaning);
                break;
                
            case 4:
                cout << "Keyword to search: ";
                cin.ignore();
                getline(cin, keyword);
                dict.searchKeyword(keyword);
                break;
                
            case 5:
                dict.display(true);
                break;
                
            case 6:
                dict.display(false);
                break;
                
            case 7:
                dict.analyzeComplexity();
                break;
                
            case 8:
                cout << "Exiting. Goodbye!" << endl;
                break;
                
            default:
                cout << "Invalid choice." << endl;
        }
    } while (choice != 8);
    
    return 0;
}
