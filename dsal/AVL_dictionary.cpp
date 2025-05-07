#include <iostream>
#include <string>
#include <algorithm> // For std::max

struct Node {
    std::string keyword;
    std::string meaning;
    Node *left, *right;
    int height;
};

int comparison_count_global = 0;

int height(Node *N) { return N ? N->height : 0; }
int max(int a, int b) { return (a > b) ? a : b; }

Node* newNode(std::string keyword, std::string meaning) {
    Node* node = new Node();
    node->keyword = keyword;
    node->meaning = meaning;
    node->left = node->right = nullptr;
    node->height = 1; // New node is a leaf
    return node;
}

Node *rightRotate(Node *y) {
    Node *x = y->left, *T2 = x->right;
    x->right = y; y->left = T2;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

Node *leftRotate(Node *x) {
    Node *y = x->right, *T2 = y->left;
    y->left = x; x->right = T2;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

int getBalance(Node *N) { return N ? height(N->left) - height(N->right) : 0; }

Node* insert(Node* node, std::string keyword, std::string meaning) {
    if (!node) return newNode(keyword, meaning);

    if (keyword < node->keyword) node->left = insert(node->left, keyword, meaning);
    else if (keyword > node->keyword) node->right = insert(node->right, keyword, meaning);
    else { std::cout << "Keyword exists. Use update.\n"; return node; }

    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);

    // Balancing cases
    if (balance > 1 && keyword < node->left->keyword) return rightRotate(node); // LL
    if (balance < -1 && keyword > node->right->keyword) return leftRotate(node); // RR
    if (balance > 1 && keyword > node->left->keyword) { node->left = leftRotate(node->left); return rightRotate(node); } // LR
    if (balance < -1 && keyword < node->right->keyword) { node->right = rightRotate(node->right); return leftRotate(node); } // RL

    return node;
}

Node * minValueNode(Node* node) {
    Node* current = node;
    while (current && current->left != nullptr) current = current->left;
    return current;
}

Node* deleteNode(Node* root, std::string keyword) {
    if (!root) return root;

    if (keyword < root->keyword) root->left = deleteNode(root->left, keyword);
    else if (keyword > root->keyword) root->right = deleteNode(root->right, keyword);
    else { // Node to delete found
        if (!root->left || !root->right) { // Node with one or no child
            Node *temp = root->left ? root->left : root->right;
            if (!temp) { temp = root; root = nullptr; } // No child
            else *root = *temp; // One child
            delete temp;
        } else { // Node with two children
            Node* temp = minValueNode(root->right);
            root->keyword = temp->keyword;
            root->meaning = temp->meaning;
            root->right = deleteNode(root->right, temp->keyword);
        }
    }

    if (!root) return root; // If tree became empty

    root->height = 1 + max(height(root->left), height(root->right));
    int balance = getBalance(root);

    // Balancing cases
    if (balance > 1 && getBalance(root->left) >= 0) return rightRotate(root); // LL
    if (balance > 1 && getBalance(root->left) < 0) { root->left = leftRotate(root->left); return rightRotate(root); } // LR
    if (balance < -1 && getBalance(root->right) <= 0) return leftRotate(root); // RR
    if (balance < -1 && getBalance(root->right) > 0) { root->right = rightRotate(root->right); return leftRotate(root); } // RL

    return root;
}

std::string search(Node* root, std::string keyword) {
    comparison_count_global = 0;
    Node* current = root;
    while (current) {
        comparison_count_global++;
        if (keyword == current->keyword) return current->meaning;
        current = (keyword < current->keyword) ? current->left : current->right;
    }
    return "Not found.";
}

bool updateMeaning(Node* root, std::string keyword, std::string newMeaning) {
    Node* current = root;
    while (current) {
        if (keyword == current->keyword) { current->meaning = newMeaning; return true; }
        current = (keyword < current->keyword) ? current->left : current->right;
    }
    return false;
}

void displayAscending(Node *root) {
    if (!root) return;
    displayAscending(root->left);
    std::cout << root->keyword << ": " << root->meaning << std::endl;
    displayAscending(root->right);
}

void displayDescending(Node *root) {
    if (!root) return;
    displayDescending(root->right);
    std::cout << root->keyword << ": " << root->meaning << std::endl;
    displayDescending(root->left);
}

int getMaxComparisons(Node* root) { return root ? height(root) : 0; }

int main() {
    Node *root = nullptr;
    int choice;
    std::string keyword, meaning;

    std::cout << "AVL Dictionary. Search O(log N).\n";

    do {
        std::cout << "\n1.Add 2.Del 3.Upd 4.Asc 5.Desc 6.Src 7.MaxComp 8.Exit\nChoice: ";
        std::cin >> choice;
        std::cin.ignore(); 

        switch (choice) {
            case 1: std::cout << "Kwd: "; getline(std::cin, keyword); std::cout << "Mng: "; getline(std::cin, meaning); root = insert(root, keyword, meaning); std::cout << "Added.\n"; break;
            case 2: std::cout << "Kwd to del: "; getline(std::cin, keyword); if (search(root, keyword) != "Not found.") { root = deleteNode(root, keyword); std::cout << "Deleted.\n"; } else { std::cout << "Not found.\n"; } break;
            case 3: std::cout << "Kwd to upd: "; getline(std::cin, keyword); std::cout << "New Mng: "; getline(std::cin, meaning); if (updateMeaning(root, keyword, meaning)) std::cout << "Updated.\n"; else std::cout << "Not found.\n"; break;
            case 4: if (!root) std::cout << "Empty.\n"; else { std::cout << "--Asc--\n"; displayAscending(root); } break;
            case 5: if (!root) std::cout << "Empty.\n"; else { std::cout << "--Desc--\n"; displayDescending(root); } break;
            case 6: std::cout << "Kwd to src: "; getline(std::cin, keyword); meaning = search(root, keyword); std::cout << "Mng: " << meaning << (meaning != "Not found." ? " (Comps: " + std::to_string(comparison_count_global) + ")" : "") << std::endl; break;
            case 7: std::cout << "Max comps (height): " << getMaxComparisons(root) << ". O(log N).\n"; break;
            case 8: std::cout << "Exiting.\n"; break;
            default: std::cout << "Invalid.\n";
        }
    } while (choice != 8);
    return 0;
}

