#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <pthread.h>
#include <chrono>

struct ThreadData {
    int** A;
    int** B;
    int** C;
    int start;
    int end;
    int size;
};

void generateRandomMatrix(int** matrix, int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            matrix[i][j] = rand() % 10; // Generate random values between 0 and 9
        }
    }
}

void* multiplyRows(void* arg) {
    ThreadData* data = reinterpret_cast<ThreadData*>(arg);

    for (int i = data->start; i < data->end; ++i) {
        for (int j = 0; j < data->size; ++j) {
            data->C[i][j] = 0;
            for (int k = 0; k < data->size; ++k) {
                data->C[i][j] += data->A[i][k] * data->B[k][j];
            }
        }
    }

    pthread_exit(NULL);
}

void writeMatrixToFile(int** matrix, int size, const char* filename) {
    std::ofstream outputFile(filename);
    if (!outputFile) {
        std::cerr << "Error: Unable to open file " << filename << " for writing." << std::endl;
        return;
    }

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            outputFile << matrix[i][j] << " ";
        }
        outputFile << std::endl;
    }

    outputFile.close();
}

int main() {
    const int N = 1000; // Change N to adjust matrix size
    const int numThreads = 2; // Number of threads to use

    int** A = new int*[N];
    int** B = new int*[N];
    int** C = new int*[N];

    for (int i = 0; i < N; ++i) {
        A[i] = new int[N];
        B[i] = new int[N];
        C[i] = new int[N];
    }

    // Seed the random number generator
    srand(time(nullptr));

    // Generate random matrices
    generateRandomMatrix(A, N);
    generateRandomMatrix(B, N);

    // Initialize thread data
    pthread_t threads[numThreads];
    ThreadData threadData[numThreads];

    // Calculate workload per thread
    int workloadPerThread = N / numThreads;
    int remainder = N % numThreads;
    int start = 0;
    int end = 0;

    // Start measuring execution time
    auto start_time = std::chrono::steady_clock::now();

    // Create threads
    for (int i = 0; i < numThreads; ++i) {
        start = end;
        end = start + workloadPerThread + (i < remainder ? 1 : 0);

        threadData[i].A = A;
        threadData[i].B = B;
        threadData[i].C = C;
        threadData[i].start = start;
        threadData[i].end = end;
        threadData[i].size = N;

        pthread_create(&threads[i], NULL, multiplyRows, &threadData[i]);
    }

    // Join threads
    for (int i = 0; i < numThreads; ++i) {
        pthread_join(threads[i], NULL);
    }

    // End measuring execution time
    auto end_time = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Execution time (excluding initialization and file writing): " << duration.count() << " milliseconds." << std::endl;

    // Write result to file
    writeMatrixToFile(C, N, "output.txt");

    // Clean up memory
    for (int i = 0; i < N; ++i) {
        delete[] A[i];
        delete[] B[i];
        delete[] C[i];
    }
    delete[] A;
    delete[] B;
    delete[] C;

    return 0;
}
