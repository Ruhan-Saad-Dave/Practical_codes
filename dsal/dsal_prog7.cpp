#include <iostream>
#include <vector>
#include <queue>
#include <climits>
using namespace std;

// Structure to represent an edge in the graph
struct Edge {
    int destination;
    int weight;
    
    Edge(int dest, int w) : destination(dest), weight(w) {}
};

// Structure to represent a vertex in the priority queue
struct Vertex {
    int id;
    int key;
    
    Vertex(int id, int key) : id(id), key(key) {}
    
    // Overload operator for min-priority queue
    bool operator>(const Vertex& other) const {
        return key > other.key;
    }
};

class Graph {
private:
    int numVertices;
    vector<vector<Edge>> adjacencyList;
    
public:
    Graph(int vertices) : numVertices(vertices) {
        adjacencyList.resize(vertices);
    }
    
    // Add an undirected edge
    void addEdge(int src, int dest, int weight) {
        adjacencyList[src].push_back(Edge(dest, weight));
        adjacencyList[dest].push_back(Edge(src, weight)); // For undirected graph
    }
    
    // Prim's algorithm to find MST
    void primMST() {
        // Vector to store constructed MST
        vector<int> parent(numVertices, -1);
        
        // Key values used to pick minimum weight edge
        vector<int> key(numVertices, INT_MAX);
        
        // To keep track of vertices included in MST
        vector<bool> inMST(numVertices, false);
        
        // Min priority queue
        priority_queue<Vertex, vector<Vertex>, greater<Vertex>> pq;
        
        // Start with vertex 0
        int startVertex = 0;
        key[startVertex] = 0;
        pq.push(Vertex(startVertex, key[startVertex]));
        
        // Loop until all vertices are included in MST
        while (!pq.empty()) {
            // Extract minimum key vertex
            int u = pq.top().id;
            pq.pop();
            
            // If already in MST, skip
            if (inMST[u])
                continue;
            
            inMST[u] = true; // Include vertex in MST
            
            // Traverse all adjacent vertices of u
            for (const Edge& edge : adjacencyList[u]) {
                int v = edge.destination;
                int weight = edge.weight;
                
                // If v is not in MST and weight of (u,v) is less than key of v
                if (!inMST[v] && weight < key[v]) {
                    // Update key of v
                    key[v] = weight;
                    parent[v] = u;
                    pq.push(Vertex(v, key[v]));
                }
            }
        }
        
        // Print the constructed MST
        cout << "Minimum Spanning Tree (MST) connections:\n";
        int totalCost = 0;
        for (int i = 1; i < numVertices; i++) {
            cout << "Office " << parent[i] << " - Office " << i << " (Cost: " << key[i] << ")\n";
            totalCost += key[i];
        }
        cout << "Total cost of phone line leasing: " << totalCost << endl;
    }
};

int main() {
    int numOffices, numConnections;
    cout << "Enter the number of offices: ";
    cin >> numOffices;
    
    Graph businessOffices(numOffices);
    
    cout << "Enter the number of possible connections: ";
    cin >> numConnections;
    
    cout << "Enter the connections (office1 office2 cost):\n";
    for (int i = 0; i < numConnections; i++) {
        int office1, office2, cost;
        cin >> office1 >> office2 >> cost;
        businessOffices.addEdge(office1, office2, cost);
    }
    
    // Find and print MST
    businessOffices.primMST();
    
    return 0;
}

/*
Enter the number of offices: 5
Enter the number of possible connections: 7
Enter the connections (office1 office2 cost):
0 1 2
0 3 6
1 2 3
1 3 8
1 4 5
2 4 7
3 4 9
*/
