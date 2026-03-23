class AStarGraph:
    def __init__(self):
        # Adjacency list: {node: [(neighbor, edge_cost), ...]}
        self.adj_list = {
            'A': [('B', 3), ('C', 1)],
            'B': [('D', 3), ('E', 1)],
            'C': [('D', 2), ('B', 4)],
            'D': [('F', 3), ('E', 7)],
            'E': [('G', 1)],
            'F': [('G', 1)],
            'G': []
        }
        
        # Heuristic values h(n) provided in the manual [cite: 425, 426, 431-437]
        self.heuristics = {
            'A': 7,
            'B': 5,
            'C': 6,
            'D': 4,
            'E': 1,
            'F': 1,
            'G': 0
        }

    def get_neighbors(self, v):
        return self.adj_list[v]

    def a_star_algorithm(self, start_node, stop_node):
        # open_list is a set of nodes which have been visited, but who's 
        # neighbors haven't all been inspected, starting with the start node
        # closed_list is a set of nodes which have been visited
        # and who's neighbors have been inspected
        open_list = set([start_node])
        closed_list = set([])

        # g contains current distances from start_node to all other nodes
        # the default value (if it's not found in the map) is +infinity
        g = {start_node: 0}

        # parents contains an adjacency map of all nodes
        parents = {start_node: start_node}

        while len(open_list) > 0:
            n = None

            # Find a node with the lowest value of f() - evaluation function [cite: 371, 373]
            for v in open_list:
                if n == None or g[v] + self.heuristics[v] < g[n] + self.heuristics[n]:
                    n = v

            if n == None:
                print('Path does not exist!')
                return None

            # If the current node is the stop_node, then we begin reconstructing the path
            if n == stop_node:
                reconst_path = []

                while parents[n] != n:
                    reconst_path.append(n)
                    n = parents[n]

                reconst_path.append(start_node)
                reconst_path.reverse()

                print(f'Path found: {" -> ".join(reconst_path)}')
                print(f'Total cost: {g[stop_node]}')
                return reconst_path

            # For all neighbors of the current node
            for (m, weight) in self.get_neighbors(n):
                # If the current node isn't in both open_list and closed_list
                # add it to open_list and note n as it's parent
                if m not in open_list and m not in closed_list:
                    open_list.add(m)
                    parents[m] = n
                    g[m] = g[n] + weight

                # Otherwise, check if it's quicker to first visit n, then m
                # and if it is, update parent data and g data
                # and if the node was in the closed_list, move it to open_list
                else:
                    if g[m] > g[n] + weight:
                        g[m] = g[n] + weight
                        parents[m] = n

                        if m in closed_list:
                            closed_list.remove(m)
                            open_list.add(m)

            # Node n is fully inspected, move it from open_list to closed_list [cite: 372, 377]
            open_list.remove(n)
            closed_list.add(n)

        print('Path does not exist!')
        return None

# --- Driver Code ---
if __name__ == "__main__":
    graph = AStarGraph()
    graph.a_star_algorithm('A', 'G')