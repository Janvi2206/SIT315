#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <omp.h>

void generateRandomMatrix(int** matrix, int size) {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            matrix[i][j] = rand() % 10; // Generate random values between 0 and 9
        }
    }
}

void multiplyMatrices(int** A, int** B, int** C, int size) {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < size; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    const int N = 1000; // Change N to adjust matrix size
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

    // Perform matrix multiplication
    auto start = std::chrono::steady_clock::now();
    multiplyMatrices(A, B, C, N);
    auto end = std::chrono::steady_clock::now();

    // Clean up memory
    for (int i = 0; i < N; ++i) {
        delete[] A[i];
        delete[] B[i];
        delete[] C[i];
    }
    delete[] A;
    delete[] B;
    delete[] C;

    // Calculate and print execution time
    std::chrono::duration<double> elapsed_seconds = end - start;
    double milliseconds = elapsed_seconds.count() * 1000;
    std::cout << "Execution time (excluding initialization and file writing): " << milliseconds << " milliseconds." << std::endl;

    return 0;
}
