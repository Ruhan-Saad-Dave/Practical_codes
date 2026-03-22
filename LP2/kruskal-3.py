# Laboratory Practice II - Assignment A3
# Implementation of Kruskal's Algorithm

class KruskalMST:
    def __init__(self, vertices):
        self.V = vertices
        self.graph = []

    def add_edge(self, u, v, w):
        self.graph.append([u, v, w])

    # Find set of an element i (uses path compression)
    def find(self, parent, i):
        if parent[i] == i:
            return i
        return self.find(parent, parent[i])

    # Does union of two sets of x and y (uses union by rank)
    def union(self, parent, rank, x, y):
        xroot = self.find(parent, x)
        yroot = self.find(parent, y)
        if rank[xroot] < rank[yroot]:
            parent[xroot] = yroot
        elif rank[xroot] > rank[yroot]:
            parent[yroot] = xroot
        else:
            parent[yroot] = xroot
            rank[xroot] += 1

    def kruskal_algo(self):
        result = []  # Stores the resulting MST
        i, e = 0, 0  # i for sorted edges, e for result[]

        # Step 1: Sort all edges in non-decreasing order of their weight [cite: 506]
        self.graph = sorted(self.graph, key=lambda item: item[2])
        parent = []
        rank = []

        for node in range(self.V):
            parent.append(node)
            rank.append(0)

        # Number of edges to be taken is equal to V-1 [cite: 502, 535]
        while e < self.V - 1:
            # Step 2: Pick the smallest edge [cite: 507]
            u, v, w = self.graph[i]
            i = i + 1
            x = self.find(parent, u)
            y = self.find(parent, v)

            # If including this edge doesn't cause cycle, include it [cite: 508]
            if x != y:
                e = e + 1
                result.append([u, v, w])
                self.union(parent, rank, x, y)

        print("Edges in the MST (Kruskal's):")
        total_cost = 0
        for u, v, weight in result:
            total_cost += weight
            print(f"{u} -- {v} == {weight}")
        print(f"Minimum Spanning Tree Cost: {total_cost}\n")

# --- Driver for Kruskal ---
gk = KruskalMST(9) # 9 vertices as per manual [cite: 535]
edges = [
    (0, 1, 4), (0, 7, 8), (1, 2, 8), (1, 7, 11), (2, 3, 7),
    (2, 8, 2), (2, 5, 4), (3, 4, 9), (3, 5, 14), (4, 5, 10),
    (5, 6, 2), (6, 7, 1), (6, 8, 6), (7, 8, 7)
]
for u, v, w in edges:
    gk.add_edge(u, v, w)
gk.kruskal_algo()