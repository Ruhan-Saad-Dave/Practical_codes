#include<bits/stdc++.h>
#include<fstream>

using namespace std;
class Employee{
	public:
		char name[100];
		int id;
		double salary;
		void accept(){
			cout << "Enter Name:";
			cin.ignore();
			cin.getline(name, 100);
			cout << "Enter id:";
			cin >> id;
			cout << "Enter Salary:";
			cin >> salary;
		}
		void display(){
			cout << "Name is:" << name << endl;
			cout << "ID is:" << id << endl;
			cout << "Salary is:" << salary << endl;
		}
};

int main(){
	Employee e[5];
	int i, n;
	fstream f;
	f.open("emp_data.txt", ios::out);
	cout << "How many record do you want to enter: ";
	cin >> n;
	cout << "enter information" << endl;
	for(i=0;i<n;i++){
		e[i].accept();
		f.write((char*) &e[i], sizeof(e[i]));
	}
	f.close();
		
	f.open("emp_data.txt", ios::in);
	cout << "---Info as follows---" << endl;
	for(i=0; i<n; i++){
		f.read((char*) &e[i], sizeof(e[i]));
		e[i].display();
	}
	f.close();
	return 0;
}


// #include <fstream>
// #include <iostream>
// #include <string.h>

// using namespace std;

// int main(){

//     int roll;
//     string name;
//     float marks;

//     ofstream fout ("C:\\d\\example.txt");
//     cout << "Enter name: ";
//     cin >> name;
//     fout << name << " ";
    
//     cout << "Enter roll no.: ";
//     cin >> roll;
//     fout << roll << " ";

//     cout << "Enter marks: ";
//     cin >> marks;
//     fout << marks << " ";
//     fout.close();

//     roll = 0;
//     name = "default";
//     marks = 0.0;

//     ifstream fin ("C:\\d\\example.txt");
//     fin >> name;
//     fin.ignore();
//     fin >> roll ;
//     fin.ignore();
//     fin >> marks;

//     cout << "Roll No.: " << roll << endl;
//     cout << "Name: " << name << endl;
//     cout << "Marks: " << marks << endl;
// }






// #include<iostream>
// #include<fstream>
// #include<string>

// using namespace std;

// class Employee {
//     public:
//         string name;
//         int id;
//         int salary;

//         void accept(){
//             cout << "Enter Name: ";
//             cin >> name;
//             cout << "Enter id: ";
//             cin >> id;
//             cout << "Enter salary: ";
//             cin >> salary;
//         }

//         void display(){
//             cout << "Name: " << name << endl;
//             cout << "ID: " << id << endl;
//             cout << "Salary: " << salary << endl;
//         }

//         void writeI(){
//             fstream fout ("C:\\d\\example.txt", ios::out | ios::app);
//             fout << name << " " << id << " " << salary << endl;
//             fout.close();
//         }

//         void readI(){
//             ifstream fin ("C:\\d\\example.txt");
//             Employee emp;
//             while(fin >> emp.name >> emp.id >> emp.salary){
//                 emp.display();
//             }
//             fin.close();
//         }
// };

// int main(){

//     Employee e[5];
//     for(int i=0;i<3;i++){
//         e[i].accept();
//         e[i].writeI();
//     }

//     for(int i=0;i<3;i++){
//         e[i].name = "default";
//         e[i].id = 0;
//         e[i].salary = 0;
//     }

//     e->readI();

// }
