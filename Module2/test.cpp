#include <iostream>
#include <vector>
#include <chrono> // For measuring time

using namespace std;
using namespace std::chrono;

// Function to multiply two matrices
vector<vector<int>> multiplyMatrices(const vector<vector<int>>& mat1, const vector<vector<int>>& mat2) {
    int m1 = mat1.size();
    int n1 = mat1[0].size();
    int m2 = mat2.size();
    int n2 = mat2[0].size();

    if (n1 != m2) {
        cerr << "Invalid dimensions for matrix multiplication";
        exit(1);
    }

    vector<vector<int>> result(m1, vector<int>(n2, 0));

    for (int i = 0; i < m1; ++i) {
        for (int j = 0; j < n2; ++j) {
            for (int k = 0; k < n1; ++k) {
                result[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }

    return result;
}

int main() {
    // Take user input for matrix1
    int m1, n1;
    cout << "Enter the number of rows and columns for matrix1: ";
    cin >> m1 >> n1;
    vector<vector<int>> matrix1(m1, vector<int>(n1));
    cout << "Enter the elements of matrix1:" << endl;
    for (int i = 0; i < m1; ++i) {
        for (int j = 0; j < n1; ++j) {
            cin >> matrix1[i][j];
        }
    }

    // Take user input for matrix2
    int m2, n2;
    cout << "Enter the number of rows and columns for matrix2: ";
    cin >> m2 >> n2;
    vector<vector<int>> matrix2(m2, vector<int>(n2));
    cout << "Enter the elements of matrix2:" << endl;
    for (int i = 0; i < m2; ++i) {
        for (int j = 0; j < n2; ++j) {
            cin >> matrix2[i][j];
        }
    }

    // Measure time
    auto start = high_resolution_clock::now();

    // Multiply matrices
    vector<vector<int>> result = multiplyMatrices(matrix1, matrix2);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    // Output multiplication result
    cout << "Result Matrix: \n";
    for (const auto& row : result) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << "\n";
    }

    // Output execution time
    cout << "Time taken: " << duration.count() << " microseconds" << endl;

    return 0;
}
