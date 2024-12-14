# Function to perform QuickSort in ascending order
def quicksort(arr, low, high):
    if low < high:
        # Partition the array
        pi = partition(arr, low, high)
        # Recursively sort the partitions
        quicksort(arr, low, pi - 1)
        quicksort(arr, pi + 1, high)

# Partition function for QuickSort
def partition(arr, low, high):
    pivot = arr[high]
    i = low - 1
    for j in range(low, high):
        if arr[j] < pivot:
            i += 1
            arr[i], arr[j] = arr[j], arr[i]
    arr[i + 1], arr[high] = arr[high], arr[i + 1]
    return i + 1

# Main program
percentages = []
n = int(input("Enter the number of students: "))

# Input the first-year percentages for each student
print("Enter first-year percentages of students:")
for i in range(n):
    percent = float(input(f"Percentage of student {i + 1}: "))
    percentages.append(percent)

# Sort the percentages in ascending order using QuickSort
quicksort(percentages, 0, len(percentages) - 1)

# Display the top 5 scores (first 5 elements in sorted order)
print("\nTop 5 scores:")
for i in range(min(5, len(percentages))):
    print(f"{i + 1}: {percentages[i]}")
