#include<iostream>

using namespace std;

class Publication {
    protected:
        char title[100];
        float price;
    
    public:
        void getdata(){
            cout << "\nEnter title of publication: " ;
            cin.ignore();
            cin.getline(title, 100);
            cout << "\nEnter price of publication: ";
            cin >> price;
        }

        void putdata(){
            cout << "\nTitle: " << title;
            cout << "\nPrice: " << price;
        }
};

class Book :public Publication{
    protected:
        int pages;

    public:
        void getdata(){
            Publication::getdata();
            cout << "\nEnter total number of pages: ";
            cin >> pages;
        }

        void putdata(){
            Publication::putdata();
            cout << "\nNo. of pages: " << pages;
        }
};

class Tape:public Publication{
    protected:
        double playTime;
    
    public:
        void getdata(){
            Publication::getdata();
            cout << "\nEnter play time: ";
            cin >> playTime;
        }

        void putdata(){
            Publication::putdata();
            cout << "\nPlay Time: " << playTime;
        }
};

int main(){

    Book book;
    Tape tape;
    int choice;

    cout << "\nBook";
    book.getdata();

    cout << "\nTape";
    tape.getdata();

    do{
        cout << "\n\nMENU";
        cout <<"\n1. Book";
        cout << "\n2. Tape";
        cout <<"\n3. Exit" << endl;
        cout <<"\n\nEnter choice: ";
        cin >> choice;

        switch(choice){
            case 1:
                book.putdata();
                break;
            
            case 2:
                tape.putdata();
                break;

            case 3:
                break;

            default:
                cout << "\nWrong Choice";
        }
    }while(choice!=3);
}
