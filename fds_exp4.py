# Function for Linear Search
def linear_search(arr, x):
    for i in range(len(arr)):
        if arr[i] == x:
            return True
    return False

# Function for Sentinel Search
def sentinel_search(arr, x):
    n = len(arr)
    last = arr[-1]
    arr[-1] = x
    i = 0
    while arr[i] != x:
        i += 1
    arr[-1] = last
    if (i < n - 1) or (arr[-1] == x):
        return True
    return False

# Function for Binary Search (only works on sorted array)
def binary_search(arr, x):
    arr.sort()  # Sort array before binary search
    low, high = 0, len(arr) - 1
    while low <= high:
        mid = (low + high) // 2
        if arr[mid] == x:
            return True
        elif arr[mid] < x:
            low = mid + 1
        else:
            high = mid - 1
    return False

# Function for Fibonacci Search (only works on sorted array)
def fibonacci_search(arr, x):
    arr.sort()  # Sort array before Fibonacci search
    n = len(arr)
    fib2 = 0  # (m-2)'th Fibonacci number
    fib1 = 1  # (m-1)'th Fibonacci number
    fibM = fib2 + fib1  # m'th Fibonacci number

    # Find the smallest Fibonacci number greater than or equal to n
    while fibM < n:
        fib2 = fib1
        fib1 = fibM
        fibM = fib2 + fib1

    # Marks the eliminated range from front
    offset = -1

    while fibM > 1:
        i = min(offset + fib2, n - 1)
        if arr[i] < x:
            fibM = fib1
            fib1 = fib2
            fib2 = fibM - fib1
            offset = i
        elif arr[i] > x:
            fibM = fib2
            fib1 -= fib2
            fib2 = fibM - fib1
        else:
            return True

    if fib1 and arr[offset + 1] == x:
        return True

    return False

# Main program
roll_numbers = []
n = int(input("Enter the number of students who attended the training program: "))
print("Enter roll numbers of students who attended the training program:")
for i in range(n):
    roll = int(input(f"Roll number of student {i + 1}: "))
    roll_numbers.append(roll)

# Enter the roll number to search
x = int(input("Enter the roll number to search: "))

# Perform searches
print("\nSearching using different methods:")
print("Linear Search:", "Found" if linear_search(roll_numbers, x) else "Not Found")
print("Sentinel Search:", "Found" if sentinel_search(roll_numbers, x) else "Not Found")
print("Binary Search:", "Found" if binary_search(roll_numbers, x) else "Not Found")
print("Fibonacci Search:", "Found" if fibonacci_search(roll_numbers, x) else "Not Found")
