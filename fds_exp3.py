"""
Write a python program to compute the following computation on matrix:
1. addition of 2 matrix
2. subtraction of 2 matrix
3. multiplication of 2 matrix (dot product)
4. transpose of matrix
"""

# Function to add two matrices
def add_matrices(matrix_a, matrix_b):
    result = []
    for i in range(len(matrix_a)):
        row = []
        for j in range(len(matrix_a[0])):
            row.append(matrix_a[i][j] + matrix_b[i][j])
        result.append(row)
    return result

# Function to subtract two matrices
def subtract_matrices(matrix_a, matrix_b):
    result = []
    for i in range(len(matrix_a)):
        row = []
        for j in range(len(matrix_a[0])):
            row.append(matrix_a[i][j] - matrix_b[i][j])
        result.append(row)
    return result

# Function to multiply two matrices (dot product)
def multiply_matrices(matrix_a, matrix_b):
    # Initialize result matrix with zeros
    result = []
    for i in range(len(matrix_a)):
        row = []
        for j in range(len(matrix_b[0])):
            sum = 0
            for k in range(len(matrix_b)):
                sum += matrix_a[i][k] * matrix_b[k][j]
            row.append(sum)
        result.append(row)
    return result

def matrix_multiplication(matrix1, matrix2):
    try:
        # Get the dimensions of the matrices
        rows_matrix1, cols_matrix1 = len(matrix1), len(matrix1[0])
        rows_matrix2, cols_matrix2 = len(matrix2), len(matrix2[0])

        # Check if the matrices can be multiplied
        if cols_matrix1 != rows_matrix2:
            return "Error: Matrices are not compatible for multiplication."

        # Initialize the result matrix with zeros
        result = [[0 for _ in range(cols_matrix2)] for _ in range(rows_matrix1)]

        # Perform matrix multiplication
        for i in range(rows_matrix1):
            for j in range(cols_matrix2):
                for k in range(cols_matrix1):
                    result[i][j] += matrix1[i][k] * matrix2[k][j]

        return result
    except IndexError:
        return "Error: Invalid matrix format. Ensure each row has the same number of elements."

# Function to transpose a matrix
def transpose_matrix(matrix):
    result = []
    for j in range(len(matrix[0])):  # Iterate over columns
        row = []
        for i in range(len(matrix)):  # Iterate over rows
            row.append(matrix[i][j])
        result.append(row)
    return result

# Input matrices
print("Enter the number of rows and columns for the matrices: ")
rows = int(input("Rows: "))
cols = int(input("Columns: "))

# Create two matrices with user input
print("Enter values for Matrix A:")
matrix_a = []
for i in range(rows):
    row = []
    for j in range(cols):
        value = int(input(f"Enter element for Matrix A at position ({i+1}, {j+1}): "))
        row.append(value)
    matrix_a.append(row)

print("Enter values for Matrix B:")
matrix_b = []
for i in range(rows):
    row = []
    for j in range(cols):
        value = int(input(f"Enter element for Matrix B at position ({i+1}, {j+1}): "))
        row.append(value)
    matrix_b.append(row)

# Perform operations
print("\nMatrix A + Matrix B:")
result_add = add_matrices(matrix_a, matrix_b)
for row in result_add:
    print(row)

print("\nMatrix A - Matrix B:")
result_subtract = subtract_matrices(matrix_a, matrix_b)
for row in result_subtract:
    print(row)

print("\nMatrix A * Matrix B (Dot product):")
result_multiply = multiply_matrices(matrix_a, matrix_b)
for row in result_multiply:
    print(row)

print("\nTranspose of Matrix A:")
result_transpose = transpose_matrix(matrix_a)
for row in result_transpose:
    print(row)
