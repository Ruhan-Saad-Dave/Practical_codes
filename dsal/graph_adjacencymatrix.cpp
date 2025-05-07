#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <limits>

using namespace std;

class Graph {
private:
    int V; // Number of vertices
    vector<string> cityNames; // City names
    
    // Adjacency list representation
    vector<vector<pair<int, int>>> adjList; // {destination, cost}
    
    // Adjacency matrix representation
    vector<vector<int>> adjMatrix; // -1 if no edge, otherwise cost

public:
    Graph(int vertices) {
        V = vertices;
        cityNames.resize(V);
        
        // Initialize adjacency list
        adjList.resize(V);
        
        // Initialize adjacency matrix with -1 (no edge)
        adjMatrix.resize(V, vector<int>(V, -1));
    }
    
    void setCityName(int v, const string& name) {
        if (v >= 0 && v < V)
            cityNames[v] = name;
    }
    
    void addEdge(int src, int dest, int cost) {
        if (src >= 0 && src < V && dest >= 0 && dest < V) {
            // Add to adjacency list
            adjList[src].push_back({dest, cost});
            
            // Add to adjacency matrix
            adjMatrix[src][dest] = cost;
        }
    }
    
    bool isConnectedBFS() {
        if (V == 0) return true;
        
        vector<bool> visited(V, false);
        queue<int> q;
        
        // Start BFS from vertex 0
        q.push(0);
        visited[0] = true;
        
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            
            // Visit all adjacent vertices
            for (auto& neighbor : adjList[u]) {
                int v = neighbor.first;
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
        
        // Check if all vertices are visited
        for (bool v : visited) {
            if (!v) return false;
        }
        return true;
    }
    
    void printAdjacencyList() {
        cout << "Adjacency List Representation:\n";
        for (int i = 0; i < V; i++) {
            cout << cityNames[i] << " -> ";
            for (auto& edge : adjList[i]) {
                cout << cityNames[edge.first] << "(" << edge.second << ") ";
            }
            cout << endl;
        }
    }
    
    void printAdjacencyMatrix() {
        cout << "Adjacency Matrix Representation:\n";
        cout << "    ";
        for (int i = 0; i < V; i++) {
            cout << cityNames[i] << " ";
        }
        cout << endl;
        
        for (int i = 0; i < V; i++) {
            cout << cityNames[i] << " ";
            for (int j = 0; j < V; j++) {
                if (adjMatrix[i][j] == -1)
                    cout << "X ";
                else
                    cout << adjMatrix[i][j] << " ";
            }
            cout << endl;
        }
    }
};

int main() {
    int n, m;
    cout << "Enter the number of cities: ";
    cin >> n;
    
    Graph flightNetwork(n);
    
    // Input city names
    cout << "Enter the names of " << n << " cities:\n";
    for (int i = 0; i < n; i++) {
        string cityName;
        cin >> cityName;
        flightNetwork.setCityName(i, cityName);
    }
    
    cout << "Enter the number of flight routes: ";
    cin >> m;
    
    cout << "Enter the flight details (source city index, destination city index, cost):\n";
    cout << "Note: Use 0-based indices for cities (0 to " << n-1 << ")\n";
    for (int i = 0; i < m; i++) {
        int src, dest, cost;
        cin >> src >> dest >> cost;
        flightNetwork.addEdge(src, dest, cost);
    }
    
    // Print both representations
    flightNetwork.printAdjacencyList();
    cout << endl;
    flightNetwork.printAdjacencyMatrix();
    
    // Check if the graph is connected
    bool isConnected = flightNetwork.isConnectedBFS();
    cout << "\nThe flight network is " << (isConnected ? "connected" : "not connected") << endl;
    
    return 0;
}
