#include <iostream>
#include <cstring>

using namespace std;

class info {
public:
    char name[30], add[30], year[5]; // Only name, address, and year
    static int cnt;

    info();
    void accept();
    void display();
    static int count();
};

int info::cnt = 0;

info::info() {
    strcpy(name, "Default");
    strcpy(add, "default");
    strcpy(year, "2024");
}

void info::accept() {
    cout << "Enter the name: ";
    cin.ignore();  // To ignore any leftover newline character
    cin.getline(name, 30);  // Allows spaces in the name input
    cout << "Enter the address: ";
    cin.getline(add, 30);  // Allows spaces in the address input
    cout << "Enter the year: ";
    cin >> year;  // Year is now correctly handled as a string
}

void info::display() {
    cout << "\n\nName: " << this->name;
    cout << "\nAddress: " << this->add;
    cout << "\nYear: " << this->year;
}

int info::count() {
    cnt = cnt + 1;
    return cnt;
}

int main() {
    int ch, i, n = 0, flag = 0;
    info o[50];
    char name[30];

    do {
        cout << "\n\n=======================";
        cout << "\n MENU" << "\n=====================";
        cout << "\n1. Accept"
             << "\n2. Display"
             << "\n3. Search Record"
             << "\n4. Exit"
             << "\nEnter your choice: ";
        cin >> ch;

        switch (ch) {
            case 1:
                o[n].accept();
                n = info::count();
                break;

            case 2:
                for (int i = 0; i < n; i++) {
                    o[i].display();
                }
                break;

            case 3:
                cout << "Enter the name to be searched: ";
                cin.ignore();  // Clear the input buffer
                cin.getline(name, 30);

                flag = 0;
                for (i = 0; i < n; i++) {
                    if (strcmp(name, o[i].name) == 0) {
                        flag = 1;
                        o[i].display();
                        break;
                    }
                }

                if (flag == 0)
                    cout << "Sorry, record not found.";
                break;

            case 4:
                break;

            default:
                cout << "\nInvalid choice. Please try again.";
                break;
        }
    } while (ch != 4);

    return 0;
}
