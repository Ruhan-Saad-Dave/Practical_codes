"""
Write a python program to compute the following computation on matrix:
1. addition of 2 matrix
2. subtraction of 2 matrix
3. multiplication of 2 matrix (dot product)
4. transpose of matrix
"""

def create_matrix():
    print("\n")
    rows = int(input("Enter number of rows:"))
    columns = int(input("Enter number of columns:"))
    result = [[_ for _ in range(columns)] for _ in range(rows)]
    for row in range(rows):
        for column in range(columns):
            value = int(input(f"Enter value at [{row}][{column}]:"))
            result[row][column] = value
    return result

def dimension_match(mat_a, mat_b):
    if len(mat_a) == len(mat_b) and len(mat_a[0]) == len(mat_b[0]):
        return True
    else:
        return False

def addition(mat_a, mat_b):
    if not dimension_match(mat_a, mat_b):
        print("Dimensions of both matrix doesn't match")
    else:
        result = [[_ for _ in range(len(mat_a[0]))] for _ in range(len(mat_a))]
        for row in range(len(mat_a)):
            for column in range(len(mat_a[0])):
                result[row][column] = mat_a[row][column] + mat_b[row][column]
        print("Addition of 2 matrix is:\n", result)

def subtraction(mat_a, mat_b):
    if not dimension_match(mat_a, mat_b):
        print("Dimensions of both matrix doesn't match")
    else:
        result = [[_ for _ in range(len(mat_a[0]))] for _ in range(len(mat_a))]
        for row in range(len(mat_a)):
            for column in range(len(mat_a[0])):
                result[row][column] = mat_a[row][column] - mat_b[row][column]
        print("Subtraction of 2 matrix is:\n", result)

def multiplication(mat_a, mat_b):
    if not dimension_match(mat_a, mat_b):
        print("Dimensions of both matrix doesn't match")
    else:
        result = [[_ for _ in range(len(mat_a[0]))] for _ in range(len(mat_a))]
        for row in range(len(mat_a)):
            for column in range(len(mat_a[0])):
                result[row][column] = mat_a[row][column] * mat_b[row][column]
        print("Multiplication of 2 matrix is:\n", result)

def transpose(mat_a):
    rows = len(mat_a)
    columns = len(mat_a[0])
    result = [[_ for _ in range(rows)] for _ in range(columns)]
    for row in range(rows):
        for column in range(columns):
            result[column][row] = mat_a[row][column]
    print("Transpose of matrix is:\n", result)

mat_a = create_matrix()
mat_b = create_matrix()
print("\nMatrix A:\n", mat_a)
print("Matrix B:\n", mat_b)
print("\n")

#addition
addition(mat_a, mat_b)
#subtraction
subtraction(mat_a, mat_b)
#multiplication
multiplication(mat_a, mat_b)
#transpose
transpose(mat_a)
