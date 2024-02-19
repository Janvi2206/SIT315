#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <chrono>

// Function to generate a random matrix of given size
void generateRandomMatrix(int** matrix, int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            matrix[i][j] = rand() % 10; // Generate random values between 0 and 9
        }
    }
}

// Function to perform matrix multiplication of A and B and store the result in C
void multiplyMatrices(int** A, int** B, int** C, int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < size; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Function to write matrix to a file (not used in this version)
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
    int** A = new int*[N]; // Allocate memory for matrix A
    int** B = new int*[N]; // Allocate memory for matrix B
    int** C = new int*[N]; // Allocate memory for matrix C

    // Allocate memory for each row in A, B, and C
    for (int i = 0; i < N; ++i) {
        A[i] = new int[N];
        B[i] = new int[N];
        C[i] = new int[N];
    }

    // Seed the random number generator
    srand(time(nullptr));

    // Generate random matrices A and B
    generateRandomMatrix(A, N);
    generateRandomMatrix(B, N);

    // Perform matrix multiplication
    auto start = std::chrono::steady_clock::now();
    multiplyMatrices(A, B, C, N);
    auto end = std::chrono::steady_clock::now();

    // Write result to file (not used in this version)
    // writeMatrixToFile(C, N, "output.txt");

    // Clean up memory
    for (int i = 0; i < N; ++i) {
        delete[] A[i];
        delete[] B[i];
        delete[] C[i];
    }
    delete[] A;
    delete[] B;
    delete[] C;

    // Calculate and print execution time in milliseconds
    std::chrono::duration<double, std::milli> elapsed_milliseconds = end - start;
    std::cout << "Execution time (excluding initialization and file writing): " << elapsed_milliseconds.count() << " milliseconds." << std::endl;

    return 0;
}