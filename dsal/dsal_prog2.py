import random

class Node:
    def __init__(self, value, level):
        self.value = value
        # Array to hold references to node at different levels
        self.forward = [None] * (level + 1)

class SkipList:
    def __init__(self, max_level, p=0.5):
        # Maximum level for this skip list
        self.max_level = max_level
        # P is the fraction of nodes with level i references also having level i+1 references
        self.p = p
        # Create header node and initialize key to -infinity
        self.header = Node(float('-inf'), max_level)
        # Current level of skip list
        self.level = 0

    def random_level(self):
        """
        Randomly generates level for a new node
        Returns a level value between 0 and max_level
        """
        level = 0
        while random.random() < self.p and level < self.max_level:
            level += 1
        return level

    def insert(self, value):
        """
        Insert a new element with given value in the skip list
        """
        # Create update array and initialize it
        update = [None] * (self.max_level + 1)
        current = self.header

        # Start from highest level of skip list
        # Move the current reference forward while key is greater
        for i in range(self.level, -1, -1):
            while current.forward[i] and current.forward[i].value < value:
                current = current.forward[i]
            update[i] = current

        # Generate a random level for node
        random_level = self.random_level()

        # If random level is greater than list's current level
        # initialize update value with reference to header
        if random_level > self.level:
            for i in range(self.level + 1, random_level + 1):
                update[i] = self.header
            self.level = random_level

        # create new node with random level generated
        new_node = Node(value, random_level)

        # insert node by rearranging references
        for i in range(random_level + 1):
            new_node.forward[i] = update[i].forward[i]
            update[i].forward[i] = new_node
        
        print(f"\nInserted {value} with level {random_level}")

    def find_closest(self, search_value):
        """
        Find the element in the skip list that is closest to the search_value
        Returns the closest value found
        """
        current = self.header
        closest_value = None
        min_diff = float('inf')

        print(f"\nSearching for value closest to {search_value}...")
        print("Search path:")
        
        # Start from highest level of skip list
        for i in range(self.level, -1, -1):
            print(f"Level {i}: ", end="")
            while current.forward[i] and current.forward[i].value <= search_value:
                print(f"{current.forward[i].value} -> ", end="")
                current = current.forward[i]
            print("end")

        # Now current points to the largest value <= search_value
        # Check current and next nodes to find the closest value
        if current != self.header:
            diff = abs(current.value - search_value)
            if diff < min_diff:
                min_diff = diff
                closest_value = current.value

        # Check next value if it exists
        if current.forward[0]:
            diff = abs(current.forward[0].value - search_value)
            if diff < min_diff:
                closest_value = current.forward[0].value

        return closest_value

    def display(self):
        """
        Display skip list level by level
        """
        print("\n=== Skip List Structure ===")
        print(f"Maximum Level: {self.max_level}")
        print(f"Current Level: {self.level}")
        print("\nVisualization (level by level):")
        print("Note: Each level shows shortcuts that can be taken during search")
        
        for level in range(self.level, -1, -1):
            print(f"\nLevel {level}: ", end="")
            node = self.header.forward[level]
            if not node:
                print("(empty)")
            else:
                while node:
                    print(f"{node.value} ", end="")
                    if node.forward[level]:
                        print("-> ", end="")
                    node = node.forward[level]

def main():
    # Create a skip list with maximum level 4
    print("Creating Skip List with maximum level 4...")
    skip_list = SkipList(max_level=4)
    
    # Insert some elements
    print("\n=== Inserting Elements ===")
    elements = [3, 6, 7, 9, 12, 19, 17, 26, 21, 25]
    print(f"Elements to insert: {elements}")
    
    for element in elements:
        skip_list.insert(element)
    
    # Display the skip list
    skip_list.display()
    
    # Find closest values to some search values
    print("\n=== Finding Closest Values ===")
    search_values = [8, 20, 15]
    
    for value in search_values:
        closest = skip_list.find_closest(value)
        print(f"\nResult: Closest value to {value} is {closest}")
        print(f"Difference: {abs(closest - value)}")

if __name__ == "__main__":
    main()
