#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

// Employee structure
struct Employee {
    int id;
    string name;
    string designation;
    float salary;

    // Default constructor
    Employee() : id(0), name(""), designation(""), salary(0.0) {}

    // Parameterized constructor
    Employee(int _id, string _name, string _desig, float _sal) : 
        id(_id), name(_name), designation(_desig), salary(_sal) {}
};

// Index entry structure
struct IndexEntry {
    int id;       // Primary key
    int position; // Position in the data file

    // Constructor
    IndexEntry(int _id, int _pos) : id(_id), position(_pos) {}
};

class EmployeeManager {
private:
    string dataFile;
    string indexFile;
    vector<IndexEntry> indexTable;

    // Load index from file
    void loadIndex() {
        indexTable.clear();
        ifstream fin(indexFile.c_str(), ios::in);
        if (!fin) {
            return; // Index file doesn't exist yet
        }

        int id, pos;
        while (fin >> id >> pos) {
            indexTable.push_back(IndexEntry(id, pos));
        }
        fin.close();

        // Sort index by ID
        sort(indexTable.begin(), indexTable.end(), 
            [](const IndexEntry& a, const IndexEntry& b) {
                return a.id < b.id;
            });
    }

    // Save index to file
    void saveIndex() {
        ofstream fout(indexFile.c_str(), ios::out);
        for (const auto& entry : indexTable) {
            fout << entry.id << " " << entry.position << endl;
        }
        fout.close();
    }

    // Binary search for ID in index
    int searchIndex(int id) {
        int low = 0;
        int high = indexTable.size() - 1;
        
        while (low <= high) {
            int mid = low + (high - low) / 2;
            
            if (indexTable[mid].id == id) {
                return mid; // Found
            }
            
            if (indexTable[mid].id < id) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        
        return -1; // Not found
    }

public:
    // Constructor
    EmployeeManager(const string& dataFileName = "employee.dat", 
                    const string& indexFileName = "employee.idx") {
        dataFile = dataFileName;
        indexFile = indexFileName;
        loadIndex();
    }

    // Add employee record
    bool addEmployee(const Employee& emp) {
        // Check if employee already exists
        if (searchIndex(emp.id) != -1) {
            cout << "Error: Employee with ID " << emp.id << " already exists!" << endl;
            return false;
        }

        // Open data file in append mode
        ofstream fout(dataFile.c_str(), ios::app | ios::binary);
        if (!fout) {
            cout << "Error opening data file for writing!" << endl;
            return false;
        }

        // Get current position in the file
        int position = fout.tellp();

        // Write employee record
        fout.write(reinterpret_cast<const char*>(&emp.id), sizeof(emp.id));
        
        // Write name with fixed length
        int nameLength = emp.name.length();
        fout.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
        fout.write(emp.name.c_str(), nameLength);
        
        // Write designation with fixed length
        int desigLength = emp.designation.length();
        fout.write(reinterpret_cast<const char*>(&desigLength), sizeof(desigLength));
        fout.write(emp.designation.c_str(), desigLength);
        
        // Write salary
        fout.write(reinterpret_cast<const char*>(&emp.salary), sizeof(emp.salary));
        
        fout.close();

        // Add entry to index table
        indexTable.push_back(IndexEntry(emp.id, position));
        
        // Sort index table
        sort(indexTable.begin(), indexTable.end(), 
            [](const IndexEntry& a, const IndexEntry& b) {
                return a.id < b.id;
            });
        
        // Save index to file
        saveIndex();
        
        return true;
    }

    // Delete employee by ID
    bool deleteEmployee(int id) {
        // Search for employee in index
        int idx = searchIndex(id);
        if (idx == -1) {
            cout << "Error: Employee with ID " << id << " not found!" << endl;
            return false;
        }

        // Remove from index table
        indexTable.erase(indexTable.begin() + idx);

        // Save updated index
        saveIndex();

        // Note: We're not physically removing the record from the data file,
        // just removing its entry from the index which makes it inaccessible.
        cout << "Employee with ID " << id << " deleted successfully!" << endl;
        return true;
    }

    // Search for employee by ID
    bool searchEmployee(int id, Employee& emp) {
        // Search for employee in index
        int idx = searchIndex(id);
        if (idx == -1) {
            return false;
        }

        // Open data file
        ifstream fin(dataFile.c_str(), ios::in | ios::binary);
        if (!fin) {
            cout << "Error opening data file for reading!" << endl;
            return false;
        }

        // Seek to the position in data file
        fin.seekg(indexTable[idx].position);

        // Read employee data
        fin.read(reinterpret_cast<char*>(&emp.id), sizeof(emp.id));
        
        // Read name
        int nameLength;
        fin.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
        char* nameBuffer = new char[nameLength + 1];
        fin.read(nameBuffer, nameLength);
        nameBuffer[nameLength] = '\0';
        emp.name = nameBuffer;
        delete[] nameBuffer;
        
        // Read designation
        int desigLength;
        fin.read(reinterpret_cast<char*>(&desigLength), sizeof(desigLength));
        char* desigBuffer = new char[desigLength + 1];
        fin.read(desigBuffer, desigLength);
        desigBuffer[desigLength] = '\0';
        emp.designation = desigBuffer;
        delete[] desigBuffer;
        
        // Read salary
        fin.read(reinterpret_cast<char*>(&emp.salary), sizeof(emp.salary));
        
        fin.close();
        return true;
    }

    // Display all employees
    void displayAllEmployees() {
        if (indexTable.empty()) {
            cout << "No employees found in the database!" << endl;
            return;
        }

        cout << "\n--- Employee List ---" << endl;
        cout << left << setw(5) << "ID" << setw(20) << "Name" 
             << setw(20) << "Designation" << setw(10) << "Salary" << endl;
        cout << string(55, '-') << endl;

        Employee emp;
        for (size_t i = 0; i < indexTable.size(); i++) {
            if (searchEmployee(indexTable[i].id, emp)) {
                cout << left << setw(5) << emp.id << setw(20) << emp.name 
                     << setw(20) << emp.designation << setw(10) << fixed << setprecision(2) << emp.salary << endl;
            }
        }
        cout << string(55, '-') << endl;
    }
};

// Main function
int main() {
    EmployeeManager manager;
    int choice;
    Employee emp;
    int searchId;

    do {
        cout << "\n--- Employee Management System ---" << endl;
        cout << "1. Add Employee" << endl;
        cout << "2. Delete Employee" << endl;
        cout << "3. Search Employee" << endl;
        cout << "4. Display All Employees" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: // Add Employee
                cout << "\nEnter Employee Details:" << endl;
                cout << "ID: ";
                cin >> emp.id;
                cin.ignore();
                cout << "Name: ";
                getline(cin, emp.name);
                cout << "Designation: ";
                getline(cin, emp.designation);
                cout << "Salary: ";
                cin >> emp.salary;
                
                if (manager.addEmployee(emp)) {
                    cout << "Employee added successfully!" << endl;
                }
                break;
                
            case 2: // Delete Employee
                cout << "\nEnter Employee ID to delete: ";
                cin >> searchId;
                manager.deleteEmployee(searchId);
                break;
                
            case 3: // Search Employee
                cout << "\nEnter Employee ID to search: ";
                cin >> searchId;
                if (manager.searchEmployee(searchId, emp)) {
                    cout << "\n--- Employee Details ---" << endl;
                    cout << "ID: " << emp.id << endl;
                    cout << "Name: " << emp.name << endl;
                    cout << "Designation: " << emp.designation << endl;
                    cout << "Salary: " << fixed << setprecision(2) << emp.salary << endl;
                } else {
                    cout << "Employee with ID " << searchId << " not found!" << endl;
                }
                break;
                
            case 4: // Display All Employees
                manager.displayAllEmployees();
                break;
                
            case 0: // Exit
                cout << "Exiting program. Goodbye!" << endl;
                break;
                
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    } while (choice != 0);

    return 0;
}