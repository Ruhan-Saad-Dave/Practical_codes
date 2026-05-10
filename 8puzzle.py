import heapq

# Goal state and flat index for quick lookup
GOAL = (1, 2, 3, 4, 5, 6, 7, 8, 0)
GOAL_POS = {tile_value: (flat_index // 3, flat_index % 3) for flat_index, tile_value in enumerate(GOAL)}

# Direction the TILE moves (opposite of blank's movement)
MOVE_NAME = {(-1,0): "Down", (1,0): "Up", (0,-1): "Right", (0,1): "Left"}

def manhattan(state):
    """Sum of Manhattan distances of each tile from its goal position."""
    total_distance = 0
    for flat_index, tile_value in enumerate(state):
        if tile_value != 0:
            current_row, current_col = flat_index // 3, flat_index % 3
            goal_row, goal_col = GOAL_POS[tile_value]
            total_distance += abs(current_row - goal_row) + abs(current_col - goal_col)
    return total_distance

def neighbors(state):
    """Generate (successor_state, tile_moved, direction) by sliding a tile into the blank."""
    state_list = list(state)
    blank_index = state_list.index(0)
    blank_row, blank_col = blank_index // 3, blank_index % 3
    for row_delta, col_delta in [(-1,0),(1,0),(0,-1),(0,1)]:
        new_row, new_col = blank_row + row_delta, blank_col + col_delta
        if 0 <= new_row < 3 and 0 <= new_col < 3:
            new_state = state_list[:]
            tile_to_move = new_state[new_row*3 + new_col]
            new_state[blank_index], new_state[new_row*3 + new_col] = new_state[new_row*3 + new_col], new_state[blank_index]
            yield tuple(new_state), tile_to_move, MOVE_NAME[(row_delta, col_delta)]

def a_star(start):
    """A* search: returns list of (state, tile, direction) from start to goal, or None."""
    initial_h = manhattan(start)
    # heap: (f_cost, g_cost, state)
    heap = [(initial_h, 0, start)]
    came_from = {start: (None, None, None)}  # state -> (parent, tile, direction)
    g_score = {start: 0}

    while heap:
        _, g_cost, state = heapq.heappop(heap)

        if state == GOAL:
            # Reconstruct path
            path, current_state = [], state
            while current_state is not None:
                parent_state, tile, direction = came_from[current_state]
                path.append((current_state, tile, direction))
                current_state = parent_state
            return path[::-1]

        if g_cost > g_score.get(state, float('inf')):
            continue  # stale entry

        for next_state, tile, direction in neighbors(state):
            new_g_cost = g_cost + 1
            if new_g_cost < g_score.get(next_state, float('inf')):
                g_score[next_state] = new_g_cost
                came_from[next_state] = (state, tile, direction)
                heapq.heappush(heap, (new_g_cost + manhattan(next_state), new_g_cost, next_state))

    return None  # unsolvable

def print_state(state):
    for row_start in range(0, 9, 3):
        print(['_' if tile_value == 0 else tile_value for tile_value in state[row_start:row_start+3]])  # 0 shown as _
    print()

# --- Demo ---
start = (1, 2, 3, 4, 0, 6, 7, 5, 8)  # 2 moves from goal

print("Start:")
print_state(start)

path = a_star(start)
if path:
    print(f"Solved in {len(path)-1} moves!\n")
    for step, (state, tile, direction) in enumerate(path):
        label = "Start" if step == 0 else f"Step {step}: Tile {tile} slides {direction}"
        print(label)
        print_state(state)
else:
    print("No solution exists.")
