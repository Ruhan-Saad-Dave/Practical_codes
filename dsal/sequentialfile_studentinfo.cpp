#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <limits>

using namespace std;

// Structure to store student information
struct Student {
    int rollNumber;
    char name[50];
    char division[10];
    char address[100];
};

// Function prototypes
void addStudent();
void displayAllStudents();
void searchStudent();
void deleteStudent();
void displayMenu();

const string FILENAME = "students.dat";

int main() {
    int choice;
    
    do {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;
        
        // Clear input buffer
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch(choice) {
            case 1:
                addStudent();
                break;
            case 2:
                displayAllStudents();
                break;
            case 3:
                searchStudent();
                break;
            case 4:
                deleteStudent();
                break;
            case 5:
                cout << "Exiting program. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
        
        cout << "\nPress Enter to continue...";
        cin.get();
        
    } while(choice != 5);
    
    return 0;
}

void displayMenu() {
    cout << "\n========== STUDENT INFORMATION SYSTEM ==========" << endl;
    cout << "1. Add Student" << endl;
    cout << "2. Display All Students" << endl;
    cout << "3. Search Student" << endl;
    cout << "4. Delete Student" << endl;
    cout << "5. Exit" << endl;
    cout << "===============================================" << endl;
}

void addStudent() {
    Student student;
    ofstream outFile;
    
    // Open file in append mode
    outFile.open(FILENAME, ios::binary | ios::app);
    
    if (!outFile) {
        cout << "Error opening file for writing!" << endl;
        return;
    }
    
    cout << "\n----- Add Student Information -----" << endl;
    
    cout << "Enter Roll Number: ";
    cin >> student.rollNumber;
    cin.ignore();
    
    cout << "Enter Name: ";
    cin.getline(student.name, 50);
    
    cout << "Enter Division: ";
    cin.getline(student.division, 10);
    
    cout << "Enter Address: ";
    cin.getline(student.address, 100);
    
    // Write student record to file
    outFile.write(reinterpret_cast<char*>(&student), sizeof(Student));
    
    outFile.close();
    cout << "Student record added successfully!" << endl;
}

void displayAllStudents() {
    Student student;
    ifstream inFile;
    
    inFile.open(FILENAME, ios::binary);
    
    if (!inFile) {
        cout << "Error opening file or file doesn't exist!" << endl;
        return;
    }
    
    cout << "\n----- All Students Information -----" << endl;
    cout << "\nRoll\tName\t\tDivision\tAddress" << endl;
    cout << "-----------------------------------------------" << endl;
    
    while (inFile.read(reinterpret_cast<char*>(&student), sizeof(Student))) {
        cout << student.rollNumber << "\t"
             << student.name << "\t\t"
             << student.division << "\t\t"
             << student.address << endl;
    }
    
    inFile.close();
}

void searchStudent() {
    Student student;
    ifstream inFile;
    int rollToSearch;
    bool found = false;
    
    inFile.open(FILENAME, ios::binary);
    
    if (!inFile) {
        cout << "Error opening file or file doesn't exist!" << endl;
        return;
    }
    
    cout << "\nEnter Roll Number to search: ";
    cin >> rollToSearch;
    
    while (inFile.read(reinterpret_cast<char*>(&student), sizeof(Student))) {
        if (student.rollNumber == rollToSearch) {
            cout << "\n----- Student Found! -----" << endl;
            cout << "Roll Number: " << student.rollNumber << endl;
            cout << "Name: " << student.name << endl;
            cout << "Division: " << student.division << endl;
            cout << "Address: " << student.address << endl;
            found = true;
            break;
        }
    }
    
    if (!found) {
        cout << "Student with Roll Number " << rollToSearch << " not found!" << endl;
    }
    
    inFile.close();
}

void deleteStudent() {
    Student student;
    ifstream inFile;
    ofstream outFile;
    int rollToDelete;
    bool found = false;
    
    inFile.open(FILENAME, ios::binary);
    
    if (!inFile) {
        cout << "Error opening file or file doesn't exist!" << endl;
        return;
    }
    
    cout << "\nEnter Roll Number to delete: ";
    cin >> rollToDelete;
    
    // Open a temporary file to write all records except the one to be deleted
    outFile.open("temp.dat", ios::binary);
    
    while (inFile.read(reinterpret_cast<char*>(&student), sizeof(Student))) {
        if (student.rollNumber != rollToDelete) {
            outFile.write(reinterpret_cast<char*>(&student), sizeof(Student));
        } else {
            found = true;
        }
    }
    
    inFile.close();
    outFile.close();
    
    if (found) {
        // Delete the original file
        remove(FILENAME.c_str());
        // Rename the temporary file to the original filename
        rename("temp.dat", FILENAME.c_str());
        cout << "Student with Roll Number " << rollToDelete << " deleted successfully!" << endl;
    } else {
        remove("temp.dat");
        cout << "Student with Roll Number " << rollToDelete << " not found!" << endl;
    }
}
