/*
Department of COmputer Engineering has student's club named 'Pinnacle Club'. Students of second, third and final year of department can be granted membership on request. Similarly one may cancle the membership of club. First node is reserved for president of club and last node is reserved for secretary of club. Write C++ program to maintain club member's information using singly linked list. Store student PRN and Name. Write functions to:
a) Add and delete the members as well as president or even secretary.
b) Compute total number of members of club.
c) Display members.
d) Two linked lists exists for two divisions. Concatenate 2 lists
*/

#include <iostream>
#include <string>

struct ClubMember {
    int PRN;                     // Student PRN
    std::string name;            // Student Name
    ClubMember* next;            // Pointer to the next member

    ClubMember(int prn, const std::string& name) : PRN(prn), name(name), next(nullptr) {}
};

class PinnacleClub {
private:
    ClubMember* head;            // Pointer to the president of the club
    ClubMember* tail;            // Pointer to the secretary of the club

public:
    PinnacleClub() : head(nullptr), tail(nullptr) {}

    // Add a member to the club (President, regular member, or Secretary)
    void addMember(int prn, const std::string& name, bool isPresident = false, bool isSecretary = false) {
        ClubMember* newMember = new ClubMember(prn, name);

        if (isPresident) {
            newMember->next = head;
            head = newMember;
            if (tail == nullptr) tail = head; // If list was empty, head and tail are the same
        } else if (isSecretary) {
            if (head == nullptr) {
                head = tail = newMember;
            } else {
                tail->next = newMember;
                tail = newMember;
            }
        } else {
            if (head == nullptr || head == tail) { // Only president or secretary exists
                if (head == nullptr) head = newMember; // Empty list, add as first member
                else { 
                    head->next = newMember; // Add after president
                    if (tail == head) tail = newMember; // No secretary, so set as tail
                }
            } else {
                ClubMember* temp = head;
                while (temp->next != tail) temp = temp->next; // Traverse to second last node
                newMember->next = tail;
                temp->next = newMember;
            }
        }
    }

    // Remove a member from the club (President, regular member, or Secretary)
    void deleteMember(int prn) {
        if (head == nullptr) {
            std::cout << "Club is empty, cannot delete any members.\n";
            return;
        }

        if (head->PRN == prn) { // Deleting president
            ClubMember* temp = head;
            head = head->next;
            delete temp;
            std::cout << "President deleted successfully.\n";
            return;
        }

        ClubMember* temp = head;
        while (temp->next != nullptr && temp->next->PRN != prn) {
            temp = temp->next;
        }

        if (temp->next == nullptr) {
            std::cout << "Member with PRN " << prn << " not found.\n";
        } else {
            ClubMember* deleteNode = temp->next;
            temp->next = temp->next->next;
            if (deleteNode == tail) tail = temp; // Update tail if secretary is deleted
            delete deleteNode;
            std::cout << "Member deleted successfully.\n";
        }
    }

    // Compute total number of members
    int countMembers() const {
        int count = 0;
        ClubMember* temp = head;
        while (temp != nullptr) {
            count++;
            temp = temp->next;
        }
        return count;
    }

    // Display all members in the club
    void displayMembers() const {
        if (head == nullptr) {
            std::cout << "No members in the club.\n";
            return;
        }

        ClubMember* temp = head;
        std::cout << "Club Members:\n";
        std::cout << "President: " << head->name << " (PRN: " << head->PRN << ")\n";

        temp = head->next;
        while (temp != nullptr && temp != tail) {
            std::cout << "Member: " << temp->name << " (PRN: " << temp->PRN << ")\n";
            temp = temp->next;
        }

        if (tail != nullptr && tail != head) {
            std::cout << "Secretary: " << tail->name << " (PRN: " << tail->PRN << ")\n";
        }
    }

    // Concatenate another list to this list
    void concatenate(PinnacleClub& otherClub) {
        if (head == nullptr) {
            head = otherClub.head;
            tail = otherClub.tail;
        } else if (otherClub.head != nullptr) {
            tail->next = otherClub.head;
            tail = otherClub.tail;
        }
        otherClub.head = otherClub.tail = nullptr; // Clear the second list
    }

    // Destructor to free memory
    ~PinnacleClub() {
        while (head != nullptr) {
            ClubMember* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

// Main program
int main() {
    PinnacleClub divisionA;
    PinnacleClub divisionB;

    // Adding members to Division A
    divisionA.addMember(101, "Alice", true);   // President
    divisionA.addMember(102, "Bob");
    divisionA.addMember(103, "Charlie");
    divisionA.addMember(104, "Daisy", false, true); // Secretary

    // Display Division A
    std::cout << "Division A:\n";
    divisionA.displayMembers();
    std::cout << "Total members in Division A: " << divisionA.countMembers() << "\n";

    // Adding members to Division B
    divisionB.addMember(201, "Eve", true);     // President
    divisionB.addMember(202, "Frank");
    divisionB.addMember(203, "Grace", false, true); // Secretary

    // Display Division B
    std::cout << "\nDivision B:\n";
    divisionB.displayMembers();
    std::cout << "Total members in Division B: " << divisionB.countMembers() << "\n";

    // Concatenate Division B into Division A
    divisionA.concatenate(divisionB);

    // Display combined list
    std::cout << "\nAfter concatenating Division B into Division A:\n";
    divisionA.displayMembers();
    std::cout << "Total members in combined list: " << divisionA.countMembers() << "\n";

    return 0;
}

/*
Division A:
Club Members:
President: Alice (PRN: 101)
Member: Charlie (PRN: 103)
Member: Bob (PRN: 102)
Secretary: Daisy (PRN: 104)
Total members in Division A: 4

Division B:
Club Members:
President: Eve (PRN: 201)
Member: Frank (PRN: 202)
Secretary: Grace (PRN: 203)
Total members in Division B: 3

After concatenating Division B into Division A:
Club Members:
President: Alice (PRN: 101)
Member: Charlie (PRN: 103)
Member: Bob (PRN: 102)
Member: Daisy (PRN: 104)
Member: Eve (PRN: 201)
Member: Frank (PRN: 202)
Secretary: Grace (PRN: 203)
Total members in combined list: 7
*/
