#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <string>
#include <limits>

using namespace std;

class Graph {
private:
    // Adjacency Matrix representation
    vector<vector<int>> adjMatrix;
    // Adjacency List representation
    unordered_map<string, vector<pair<string, int>>> adjList;
    // Store city names
    vector<string> cities;
    // Map city names to indices for matrix representation
    unordered_map<string, int> cityToIndex;

public:
    // Add a city to the graph
    void addCity(const string& city) {
        // Check if city already exists
        if (cityToIndex.find(city) != cityToIndex.end()) {
            return;
        }
        
        // Add to the list of cities
        cities.push_back(city);
        int index = cities.size() - 1;
        cityToIndex[city] = index;
        
        // Update adjacency matrix
        if (index == 0) {
            // First city, initialize the matrix
            adjMatrix.push_back(vector<int>(1, 0));
        } else {
            // Expand the matrix
            for (auto& row : adjMatrix) {
                row.push_back(0);
            }
            adjMatrix.push_back(vector<int>(adjMatrix.size() + 1, 0));
        }
        
        // Initialize empty adjacency list for the new city
        if (adjList.find(city) == adjList.end()) {
            adjList[city] = vector<pair<string, int>>();
        }
    }
    
    // Add a flight between two cities with a cost
    void addFlight(const string& from, const string& to, int cost) {
        // Make sure both cities exist
        addCity(from);
        addCity(to);
        
        // Update adjacency matrix
        adjMatrix[cityToIndex[from]][cityToIndex[to]] = cost;
        
        // Update adjacency list
        adjList[from].push_back(make_pair(to, cost));
    }
    
    // Print adjacency matrix
    void printAdjacencyMatrix() {
        cout << "\nAdjacency Matrix Representation:\n";
        cout << "   ";
        for (const auto& city : cities) {
            cout << city.substr(0, 3) << " ";
        }
        cout << endl;
        
        for (size_t i = 0; i < cities.size(); i++) {
            cout << cities[i].substr(0, 3) << " ";
            for (size_t j = 0; j < cities.size(); j++) {
                if (adjMatrix[i][j] == 0) {
                    cout << "0   ";
                } else {
                    cout << adjMatrix[i][j] << (adjMatrix[i][j] < 10 ? "   " : "  ");
                }
            }
            cout << endl;
        }
    }
    
    // Print adjacency list
    void printAdjacencyList() {
        cout << "\nAdjacency List Representation:\n";
        for (const auto& city : cities) {
            cout << city << " -> ";
            for (const auto& neighbor : adjList[city]) {
                cout << "(" << neighbor.first << ", " << neighbor.second << ") ";
            }
            cout << endl;
        }
    }
    
    // Check if the graph is connected using BFS and adjacency list
    bool isConnectedList() {
        if (cities.empty()) {
            return true; // Empty graph is trivially connected
        }
        
        // Keep track of visited cities
        unordered_map<string, bool> visited;
        for (const auto& city : cities) {
            visited[city] = false;
        }
        
        // Start BFS from the first city
        queue<string> q;
        string start = cities[0];
        visited[start] = true;
        q.push(start);
        
        while (!q.empty()) {
            string current = q.front();
            q.pop();
            
            // Visit all neighbors
            for (const auto& neighbor : adjList[current]) {
                string dest = neighbor.first;
                if (!visited[dest]) {
                    visited[dest] = true;
                    q.push(dest);
                }
            }
        }
        
        // Check if all cities were visited
        for (const auto& pair : visited) {
            if (!pair.second) {
                return false;
            }
        }
        
        return true;
    }
    
    // Check if the graph is connected using BFS and adjacency matrix
    bool isConnectedMatrix() {
        if (cities.empty()) {
            return true; // Empty graph is trivially connected
        }
        
        // Keep track of visited cities
        vector<bool> visited(cities.size(), false);
        
        // Start BFS from the first city
        queue<int> q;
        visited[0] = true;
        q.push(0);
        
        while (!q.empty()) {
            int current = q.front();
            q.pop();
            
            // Visit all neighbors
            for (size_t i = 0; i < cities.size(); i++) {
                if (adjMatrix[current][i] > 0 && !visited[i]) {
                    visited[i] = true;
                    q.push(i);
                }
            }
        }
        
        // Check if all cities were visited
        for (bool v : visited) {
            if (!v) {
                return false;
            }
        }
        
        return true;
    }
    
    // Analysis of time and space complexity
    void analyzeComplexity() {
        int numCities = cities.size();
        int numFlights = 0;
        
        // Count total number of flights
        for (const auto& cityPair : adjList) {
            numFlights += cityPair.second.size();
        }
        
        cout << "\nComplexity Analysis:\n";
        cout << "Number of cities (vertices): " << numCities << endl;
        cout << "Number of flights (edges): " << numFlights << endl;
        
        cout << "\nAdjacency Matrix:";
        cout << "\n- Space Complexity: O(V²) = O(" << numCities << "²) = O(" << numCities * numCities << ")";
        cout << "\n- Time Complexity:";
        cout << "\n  * Adding a vertex: O(V) = O(" << numCities << ")";
        cout << "\n  * Adding an edge: O(1)";
        cout << "\n  * Checking if two vertices are connected: O(1)";
        cout << "\n  * Finding all neighbors of a vertex: O(V) = O(" << numCities << ")";
        cout << "\n  * Checking if graph is connected: O(V²) = O(" << numCities * numCities << ")";
        
        cout << "\n\nAdjacency List:";
        cout << "\n- Space Complexity: O(V + E) = O(" << numCities << " + " << numFlights << ") = O(" << numCities + numFlights << ")";
        cout << "\n- Time Complexity:";
        cout << "\n  * Adding a vertex: O(1)";
        cout << "\n  * Adding an edge: O(1)";
        cout << "\n  * Checking if two vertices are connected: O(degree(v)) in worst case";
        cout << "\n  * Finding all neighbors of a vertex: O(degree(v))";
        cout << "\n  * Checking if graph is connected: O(V + E) = O(" << numCities << " + " << numFlights << ")";
        
        cout << "\n\nJustification for Storage Representation:";
        if (numFlights < numCities * numCities / 2) {
            cout << "\nThe graph is sparse (|E| << |V|²), so Adjacency List is more space-efficient.";
        } else {
            cout << "\nThe graph is dense (|E| ~ |V|²), so Adjacency Matrix might be more appropriate.";
        }
        
        cout << "\nFor operations like checking connectivity and traversal, Adjacency List is generally more efficient for sparse graphs.";
        cout << "\nFor quick edge lookup between specific vertices, Adjacency Matrix is more efficient.";
    }
};

int main() {
    Graph flightNetwork;
    
    // Sample data - add cities and flights
    flightNetwork.addCity("Mumbai");
    flightNetwork.addCity("Delhi");
    flightNetwork.addCity("Bangalore");
    flightNetwork.addCity("Chennai");
    flightNetwork.addCity("Kolkata");
    
    // Add flights with costs (could be time in minutes or fuel in liters)
    flightNetwork.addFlight("Mumbai", "Delhi", 130);      // 130 minutes or liters of fuel
    flightNetwork.addFlight("Delhi", "Mumbai", 130);
    flightNetwork.addFlight("Mumbai", "Bangalore", 100);
    flightNetwork.addFlight("Bangalore", "Mumbai", 100);
    flightNetwork.addFlight("Bangalore", "Chennai", 45);
    flightNetwork.addFlight("Chennai", "Bangalore", 45);
    flightNetwork.addFlight("Delhi", "Kolkata", 120);
    flightNetwork.addFlight("Kolkata", "Delhi", 120);
    flightNetwork.addFlight("Chennai", "Kolkata", 110);
    flightNetwork.addFlight("Kolkata", "Chennai", 110);
    
    // Print both representations
    flightNetwork.printAdjacencyMatrix();
    flightNetwork.printAdjacencyList();
    
    // Check if the graph is connected using both methods
    bool connectedList = flightNetwork.isConnectedList();
    bool connectedMatrix = flightNetwork.isConnectedMatrix();
    
    cout << "\nIs the flight network connected (using adjacency list)? " 
         << (connectedList ? "Yes" : "No") << endl;
    cout << "Is the flight network connected (using adjacency matrix)? " 
         << (connectedMatrix ? "Yes" : "No") << endl;
    
    // Analyze complexity
    flightNetwork.analyzeComplexity();
    
    // Add some user interaction
    cout << "\n\nWould you like to add more flights? (y/n): ";
    char choice;
    cin >> choice;
    
    if (choice == 'y' || choice == 'Y') {
        string from, to;
        int cost;
        
        cout << "Enter source city: ";
        cin >> from;
        cout << "Enter destination city: ";
        cin >> to;
        cout << "Enter flight cost (time or fuel): ";
        cin >> cost;
        
        flightNetwork.addFlight(from, to, cost);
        
        // Print updated representations
        flightNetwork.printAdjacencyMatrix();
        flightNetwork.printAdjacencyList();
        
        // Check connectivity again
        connectedList = flightNetwork.isConnectedList();
        cout << "\nIs the updated flight network connected? " 
             << (connectedList ? "Yes" : "No") << endl;
    }
    
    return 0;
}
