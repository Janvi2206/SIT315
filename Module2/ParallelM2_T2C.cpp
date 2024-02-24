#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <pthread.h>

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

// Struct to pass data to thread function
struct ThreadData {
    vector<int>* arr;
    int low;
    int high;
};

// Thread function to perform quicksort on a subarray
void* threadQuicksort(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    quicksort(*(data->arr), data->low, data->high);
    pthread_exit(NULL);
}

int main() {
    srand(time(0));
    vector<int> arr;
    int n = 100000; 

    for (int i = 0; i < n; i++) {
        arr.push_back(rand() % 1000); 
    }

    auto start = high_resolution_clock::now(); 

    int numThreads = 4;

    // Create threads and sort subarrays concurrently
    pthread_t threads[numThreads];
    ThreadData threadData[numThreads];
    int partitionSize = n / numThreads;
    for (int i = 0; i < numThreads; i++) {
        int low = i * partitionSize;
        int high = (i == numThreads - 1) ? n - 1 : (i + 1) * partitionSize - 1;
        threadData[i] = {&arr, low, high};
        pthread_create(&threads[i], NULL, threadQuicksort, (void*)&threadData[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Merge sorted subarrays if necessary (not implemented here)

    auto stop = high_resolution_clock::now(); // Stop measuring execution time
    auto duration = duration_cast<milliseconds>(stop - start);

    cout << "Execution time: " << duration.count() << " milliseconds" << endl;

    return 0;
}
