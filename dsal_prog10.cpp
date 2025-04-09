#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Double-Ended Priority Queue implementation
class DoubleEndedPriorityQueue {
private:
    vector<int> maxHeap; // For finding maximum element
    vector<int> minHeap; // For finding minimum element

    // Helper function to maintain max heap property
    void maxHeapify(int i) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        
        if (left < maxHeap.size() && maxHeap[left] > maxHeap[largest])
            largest = left;
        
        if (right < maxHeap.size() && maxHeap[right] > maxHeap[largest])
            largest = right;
        
        if (largest != i) {
            swap(maxHeap[i], maxHeap[largest]);
            maxHeapify(largest);
        }
    }
    
    // Helper function to maintain min heap property
    void minHeapify(int i) {
        int smallest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        
        if (left < minHeap.size() && minHeap[left] < minHeap[smallest])
            smallest = left;
        
        if (right < minHeap.size() && minHeap[right] < minHeap[smallest])
            smallest = right;
        
        if (smallest != i) {
            swap(minHeap[i], minHeap[smallest]);
            minHeapify(smallest);
        }
    }
    
    // Build max heap
    void buildMaxHeap() {
        for (int i = maxHeap.size() / 2 - 1; i >= 0; i--)
            maxHeapify(i);
    }
    
    // Build min heap
    void buildMinHeap() {
        for (int i = minHeap.size() / 2 - 1; i >= 0; i--)
            minHeapify(i);
    }

public:
    // Insert a new element into both heaps
    void insert(int mark) {
        maxHeap.push_back(mark);
        minHeap.push_back(mark);
        
        // Maintain heap properties
        buildMaxHeap();
        buildMinHeap();
    }
    
    // Get maximum element
    int getMax() {
        if (maxHeap.empty())
            return -1; // Indicate empty heap
        return maxHeap[0];
    }
    
    // Get minimum element
    int getMin() {
        if (minHeap.empty())
            return -1; // Indicate empty heap
        return minHeap[0];
    }
    
    // Insert batch of elements
    void insertBatch(vector<int>& marks) {
        for (int mark : marks) {
            maxHeap.push_back(mark);
            minHeap.push_back(mark);
        }
        
        // Build heaps once after all insertions
        buildMaxHeap();
        buildMinHeap();
    }
    
    // Check if heap is empty
    bool isEmpty() {
        return maxHeap.empty();
    }
};

// Alternative implementation using built-in priority_queue
class OptimizedDEPQ {
private:
    vector<int> data;

public:
    void insert(int mark) {
        data.push_back(mark);
    }
    
    int getMax() {
        if (data.empty()) return -1;
        return *max_element(data.begin(), data.end());
    }
    
    int getMin() {
        if (data.empty()) return -1;
        return *min_element(data.begin(), data.end());
    }
    
    void insertBatch(vector<int>& marks) {
        data.insert(data.end(), marks.begin(), marks.end());
    }
    
    bool isEmpty() {
        return data.empty();
    }
};

int main() {
    DoubleEndedPriorityQueue depq;
    vector<int> marks;
    int n, mark;
    
    cout << "========== Student Marks Analysis ==========" << endl;
    cout << "Enter the number of students: ";
    cin >> n;
    
    if (n <= 0) {
        cout << "Invalid input. Number of students must be positive." << endl;
        return 1;
    }
    
    cout << "Enter the marks of " << n << " students:" << endl;
    for (int i = 0; i < n; i++) {
        cout << "Student " << (i+1) << ": ";
        cin >> mark;
        marks.push_back(mark);
    }
    
    // Insert all marks into the heap
    depq.insertBatch(marks);
    
    // Display results
    cout << "\n========== Results ==========" << endl;
    cout << "Maximum marks: " << depq.getMax() << endl;
    cout << "Minimum marks: " << depq.getMin() << endl;
    
    cout << "\n========== Algorithm Analysis ==========" << endl;
    cout << "1. Time Complexity:" << endl;
    cout << "   - Building both heaps: O(n)" << endl;
    cout << "   - Finding maximum: O(1)" << endl;
    cout << "   - Finding minimum: O(1)" << endl;
    cout << "   - Overall: O(n) for initialization, O(1) for queries" << endl;
    cout << "2. Space Complexity: O(n)" << endl;
    
    return 0;
}
