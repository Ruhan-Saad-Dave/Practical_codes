# Function to perform InsertionShellSort in descending order
def insertion_shell_sort(arr):
    n = len(arr)
    gap = n // 2  # Start with a large gap, then reduce the gap
    while gap > 0:
        for i in range(gap, n):
            temp = arr[i]
            j = i
            # Sort sublist for this gap, using insertion sort
            while j >= gap and arr[j - gap] < temp:
                arr[j] = arr[j - gap]
                j -= gap
            arr[j] = temp
        gap //= 2  # Reduce the gap for the next element
    return arr

# Main program
percentages = []
n = int(input("Enter the number of students: "))

# Input the secondary percentages for each student
print("Enter secondary percentages of students:")
for i in range(n):
    percent = float(input(f"Percentage of student {i + 1}: "))
    percentages.append(percent)

# Sort the percentages in descending order using InsertionShellSort
sorted_percentages = insertion_shell_sort(percentages)

# Display the top 5 scores
print("\nTop 5 scores:")
for i in range(min(5, len(sorted_percentages))):
    print(f"{i + 1}: {sorted_percentages[i]}")
