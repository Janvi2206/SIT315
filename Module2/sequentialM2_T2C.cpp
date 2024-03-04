#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Function to partition the array and return the pivot index
int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high]; // Choosing the last element as the pivot
    int i = low - 1; // Index of smaller element

    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            // Swap arr[i] and arr[j]
            swap(arr[i], arr[j]);
        }
    }
    // Swap arr[i + 1] and arr[high] (or pivot)
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quicksort(vector<int>& arr, int low, int high) {
    if (low < high) {
        // pi is partitioning index, arr[pi] is now at right place
        int pi = partition(arr, low, high);

        // Separately sort elements before partition and after partition
        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}

int main() {
    srand(time(0));
    vector<int> arr;
    int n = 100000; // Number of elements in the array

    for (int i = 0; i < n; i++) {
        arr.push_back(rand() % 1000); 
    }

    auto start = high_resolution_clock::now(); 
    quicksort(arr, 0, n - 1);

    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<milliseconds>(stop - start);
    
    cout << "Execution time: " << duration.count() << " milliseconds" << endl;
    return 0;
}
