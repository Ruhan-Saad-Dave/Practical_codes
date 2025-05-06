class Record:
    def __init__(self, name, number):
        self.name = name
        self.number = number
    
    def get_name(self):
        return self.name
    
    def get_number(self):
        return self.number

    def __str__(self):
        return f"{self.name} ({self.number})"  # Fixed string formatting

class DoubleHash:
    def __init__(self):
        self.size = int(input("Enter the size of hash table: "))
        self.table = [None for i in range(self.size)]
        self.elementCount = 0
        self.comparisons = 0
    
    def isFull(self):
        return self.elementCount == self.size
    
    def h1(self, element):
        return element % self.size
    
    def h2(self, element):
        return 5 - (element % 5)
    
    def doublehashing(self, record):
        limit = self.size
        i = 1
        while i <= limit:
            newPosition = (self.h1(record.get_number()) + i * self.h2(record.get_number())) % self.size 
            if self.table[newPosition] == None:
                return True, newPosition
            i += 1
        return False, None
    
    def insert(self, record):
        if self.isFull():
            print("Hash Table Full")
            return False
        
        position = self.h1(record.get_number())
        
        if self.table[position] == None:
            self.table[position] = record
            print(f"Phone number of {record.get_name()} is at position {position}")
            self.elementCount += 1
            return True
        else:
            print(f"Collision has occurred for {record.get_name()}'s phone number at position {position}, finding new position")
            posFound, position = self.doublehashing(record)
            if posFound:
                self.table[position] = record
                self.elementCount += 1
                print(f"Phone number of {record.get_name()} is at position {position}")
                return True
            return False

    def search(self, record):
        position = self.h1(record.get_number())
        self.comparisons += 1
        
        if self.table[position] is not None:
            if self.table[position].get_name() == record.get_name():
                print(f"Phone number found at position {position} and total comparisons are 1")
                return position
            
            limit = self.size
            i = 1
            while i <= limit:
                newPosition = (self.h1(record.get_number()) + i * self.h2(record.get_number())) % self.size
                self.comparisons += 1
                
                if self.table[newPosition] is None:
                    break
                    
                if self.table[newPosition].get_name() == record.get_name():
                    print(f"Phone number found at position {newPosition} and total comparisons are {i + 1}")
                    return newPosition
                    
                i += 1
        
        print("Record not Found")
        return False
    
    def display(self):
        print("\nPhone Book Contents:")
        for i in range(self.size):
            print(f"Hash Value: {i}\t\t{self.table[i]}")
        print(f"The number of phonebook records in the Table are: {self.elementCount}")

if __name__ == "__main__":
    phonebook = DoubleHash()
    
    record1 = Record("Ayush", 23104)
    record2 = Record("Yash", 23102)
    record3 = Record("Atul", 23115)
    record4 = Record("Raj", 23109)
    
    phonebook.insert(record1)
    phonebook.insert(record2)
    phonebook.insert(record3)
    phonebook.search(record4)
    phonebook.display()
