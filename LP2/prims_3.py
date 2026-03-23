import sys

class PrimMST:
    def __init__(self, vertices):
        self.V = vertices
        # The edge information is stored in this 2D Adjacency Matrix
        # graph[u][v] = weight of the edge between u and v
        self.graph = [[0 for _ in range(vertices)] for _ in range(vertices)]

    def add_edge(self, u, v, weight):
        # Defines the edge by setting the weight in the matrix
        # Since it's an undirected graph, we set both directions
        self.graph[u][v] = weight
        self.graph[v][u] = weight

    def min_key(self, key, mst_set):
        # Helper to find the vertex with the minimum weight edge 
        # connecting to the current MST
        min_val = sys.maxsize
        min_index = -1
        for v in range(self.V):
            if key[v] < min_val and not mst_set[v]:
                min_val = key[v]
                min_index = v
        return min_index

    def prim_algo(self):
        # 1. Initialize keys as INFINITE and mstSet as empty [cite: 673, 674]
        key = [sys.maxsize] * self.V
        parent = [None] * self.V
        mst_set = [False] * self.V

        # 2. Start with the first vertex [cite: 675]
        key[0] = 0
        parent[0] = -1 

        for _ in range(self.V):
            # 3. Pick vertex 'u' with the minimum key value [cite: 677]
            u = self.min_key(key, mst_set)
            mst_set[u] = True

            # 4. Update key values of adjacent vertices 'v' [cite: 679]
            for v in range(self.V):
                # If there is an edge (graph[u][v] > 0), 
                # v is not in MST, and edge weight is smaller than current key
                if 0 < self.graph[u][v] < key[v] and not mst_set[v]:
                    key[v] = self.graph[u][v]
                    parent[v] = u

        self.print_mst(parent)

    def print_mst(self, parent):
        print("Edge \tWeight")
        total_cost = 0
        for i in range(1, self.V):
            weight = self.graph[i][parent[i]]
            print(f"{parent[i]} - {i} \t{weight}")
            total_cost += weight
        print(f"Total MST Cost: {total_cost}")

# --- Driver Code ---
if __name__ == "__main__":
    # Example from manual: 9 vertices [cite: 535]
    g = PrimMST(9)
    
    # Explicitly defining edges as (source, destination, weight) [cite: 548]
    edges = [
        (0, 1, 4), (0, 7, 8), (1, 2, 8), (1, 7, 11), (2, 3, 7),
        (2, 8, 2), (2, 5, 4), (3, 4, 9), (3, 5, 14), (4, 5, 10),
        (5, 6, 2), (6, 7, 1), (6, 8, 6), (7, 8, 7)
    ]
    
    for u, v, w in edges:
        g.add_edge(u, v, w)
        
    g.prim_algo()