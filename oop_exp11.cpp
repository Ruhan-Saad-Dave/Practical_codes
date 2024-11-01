/*
Write C++ program using STL for sorting and searching user defined records such as personal records (Name, DOB, Telephone number etc) using vector container
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

// Record class definition
class Record {
public:
    string name;
    string dob;         // Date of Birth (format: YYYY-MM-DD)
    string phoneNumber;

    // Constructor
    Record(string name, string dob, string phoneNumber) 
        : name(name), dob(dob), phoneNumber(phoneNumber) {}

    // Display function to print a record
    void display() const {
        cout << "Name: " << name 
             << ", DOB: " << dob 
             << ", Phone: " << phoneNumber << endl;
    }

    // Overloading < operator to allow sorting by name
    bool operator<(const Record& other) const {
        return name < other.name;
    }
};

// Function to display all records
void displayRecords(const vector<Record>& records) {
    for (const auto& record : records) {
        record.display();
    }
}

// Function to search for a record by name
void searchRecord(const vector<Record>& records, const string& name) {
    auto it = find_if(records.begin(), records.end(), 
                      [&](const Record& r) { return r.name == name; });
    if (it != records.end()) {
        cout << "Record found:\n";
        it->display();
    } else {
        cout << "Record not found.\n";
    }
}

int main() {
    vector<Record> records;
    int choice;

    while (true) {
        cout << "\nMenu:\n";
        cout << "1. Add record\n";
        cout << "2. Display all records\n";
        cout << "3. Sort records by name\n";
        cout << "4. Search record by name\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            string name, dob, phone;
            cout << "Enter Name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter Date of Birth (YYYY-MM-DD): ";
            getline(cin, dob);
            cout << "Enter Phone Number: ";
            getline(cin, phone);
            records.push_back(Record(name, dob, phone));
            cout << "Record added successfully.\n";
        } 
        else if (choice == 2) {
            cout << "Displaying all records:\n";
            displayRecords(records);
        } 
        else if (choice == 3) {
            sort(records.begin(), records.end());
            cout << "Records sorted by name.\n";
        } 
        else if (choice == 4) {
            string name;
            cout << "Enter Name to search: ";
            cin.ignore();
            getline(cin, name);
            searchRecord(records, name);
        } 
        else if (choice == 5) {
            cout << "Exiting program.\n";
            break;
        } 
        else {
            cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}

/*
Menu:
1. Add record
2. Display all records
3. Sort records by name
4. Search record by name
5. Exit
Enter your choice: 1
Enter Name: Alice
Enter Date of Birth (YYYY-MM-DD): 2000-04-15
Enter Phone Number: 123-456-7890
Record added successfully.

Menu:
1. Add record
2. Display all records
3. Sort records by name
4. Search record by name
5. Exit
Enter your choice: 1
Enter Name: Bob
Enter Date of Birth (YYYY-MM-DD): 1999-09-23
Enter Phone Number: 234-567-8901
Record added successfully.

Menu:
1. Add record
2. Display all records
3. Sort records by name
4. Search record by name
5. Exit
Enter your choice: 1
Enter Name: Charlie
Enter Date of Birth (YYYY-MM-DD): 2001-01-01
Enter Phone Number: 345-678-9012
Record added successfully.

Menu:
1. Add record
2. Display all records
3. Sort records by name
4. Search record by name
5. Exit
Enter your choice: 2
Displaying all records:
Name: Alice, DOB: 2000-04-15, Phone: 123-456-7890
Name: Bob, DOB: 1999-09-23, Phone: 234-567-8901
Name: Charlie, DOB: 2001-01-01, Phone: 345-678-9012

Menu:
1. Add record
2. Display all records
3. Sort records by name
4. Search record by name
5. Exit
Enter your choice: 3
Records sorted by name.

Menu:
1. Add record
2. Display all records
3. Sort records by name
4. Search record by name
5. Exit
Enter your choice: 2
Displaying all records:
Name: Alice, DOB: 2000-04-15, Phone: 123-456-7890
Name: Bob, DOB: 1999-09-23, Phone: 234-567-8901
Name: Charlie, DOB: 2001-01-01, Phone: 345-678-9012

Menu:
1. Add record
2. Display all records
3. Sort records by name
4. Search record by name
5. Exit
Enter your choice: 4
Enter Name to search: Bob
Record found:
Name: Bob, DOB: 1999-09-23, Phone: 234-567-8901

Menu:
1. Add record
2. Display all records
3. Sort records by name
4. Search record by name
5. Exit
Enter your choice: 4
Enter Name to search: David
Record not found.

Menu:
1. Add record
2. Display all records
3. Sort records by name
4. Search record by name
5. Exit
Enter your choice: 5
Exiting program.
*/
