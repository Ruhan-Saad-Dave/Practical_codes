print("Name = Ruhan Saad Dave")
print("Roll no = TECOA23101")

class Graph:
    def __init__(self):
        # Using a dictionary to store the adjacency list
        self.graph = {}

    def add_edge(self, u, v):
        # Since it's an undirected graph, add edges in both directions
        if u not in self.graph:
            self.graph[u] = []
        if v not in self.graph:
            self.graph[v] = []
        self.graph[u].append(v)
        self.graph[v].append(u)

    # --- Depth First Search (Recursive) ---
    def dfs_recursive(self, node, visited):
        # Mark the current node as visited
        visited.add(node)
        print(node, end=" ")

        # Recur for all adjacent vertices not yet visited
        for neighbor in self.graph.get(node, []):
            if neighbor not in visited:
                self.dfs_recursive(neighbor, visited)

    def dfs(self, start_node):
        print(f"DFS Traversal starting from node {start_node}:")
        visited = set()
        self.dfs_recursive(start_node, visited)
        print("\n")

    # --- Breadth First Search (Iterative using Queue) ---
    def bfs(self, start_node):
        print(f"BFS Traversal starting from node {start_node}:")
        visited = set()
        queue = [start_node]
        visited.add(start_node)

        while queue:
            # Take the front item of the queue
            current = queue.pop(0)
            print(current, end=" ")

            # Add all unvisited adjacent nodes to the back of the queue
            for neighbor in self.graph.get(current, []):
                if neighbor not in visited:
                    visited.add(neighbor)
                    queue.append(neighbor)
        print("\n")

# --- Driver Code ---
if __name__ == "__main__":
    g = Graph()
    
    # Building the example graph from Figure 1 in the manual
    # Vertices: 0, 1, 2, 3, 4
    g.add_edge(0, 1)
    g.add_edge(0, 2)
    g.add_edge(0, 3)
    g.add_edge(1, 2)
    g.add_edge(2, 4)

    # Perform Traversals
    g.dfs(0)
    g.bfs(0)