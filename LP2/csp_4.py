print("Name = Ruhan Saad Dave")
print("Roll no = TECOA23101")

def solve_n_queens(n):
    # Initialize board and tracking arrays for constraints
    board = [[0] * n for _ in range(n)]
    rows = [False] * n
    slash = [False] * (2 * n - 1)      # For top-right to bottom-left diagonals (r + c)
    backslash = [False] * (2 * n - 1)  # For top-left to bottom-right diagonals (r - c + n - 1)

    def backtrack(r):
        if r == n: return True  # All queens placed [cite: 913]
        
        for c in range(n):
            # Check if current position is under attack [cite: 889, 909]
            if not (rows[c] or slash[r + c] or backslash[r - c + n - 1]):
                # Place queen and mark constraints as occupied [cite: 891, 917]
                board[r][c] = 1
                rows[c] = slash[r + c] = backslash[r - c + n - 1] = True
                
                if backtrack(r + 1): return True
                
                # Backtrack: Reset values if no solution found [cite: 891, 919]
                board[r][c] = 0
                rows[c] = slash[r + c] = backslash[r - c + n - 1] = False
        return False

    if backtrack(0):
        for row in board: print(row) # Output as a binary matrix [cite: 898]
    else:
        print("No solution exists")

n = int(input("Enter the number of queens (N): "))
solve_n_queens(n)