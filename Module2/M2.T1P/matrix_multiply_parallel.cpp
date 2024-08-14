// Parallised matrix multiply

#include <iostream>
#include <cstdlib>
#include <time.h>
#include <chrono>
#include <fstream>
#include <thread>

using namespace std::chrono;
using namespace std;

const long matrixSize = 1000;

// going global as hitting max stack size - because not using heap
long arrayA[matrixSize][matrixSize];
long arrayB[matrixSize][matrixSize];
long arrayC[matrixSize][matrixSize];


void randomMatrix(long matrix[][matrixSize]) {
    for (int row = 0; row < matrixSize; row++) {
        for (int column = 0; column < matrixSize; column++) {
        // get random number modulo 100 and set at location row-column in matrix
            matrix[row][column] = rand() % 100;
        }
    }
}

// create an output file with the result matrix. column values are separated by commas and rows by newlines
void createOutputFile(string fileName, long matrix[][matrixSize]) {
    ofstream outputFileStream(fileName);
    if (outputFileStream.is_open()) {
        int row, column;
    
        for (row = 0; row < matrixSize; row++) {
            for (column = 0; column < matrixSize; column++) {
                outputFileStream << matrix[row][column];
            
                if (column != matrixSize - 1) {
                    outputFileStream << ",";
                }
            }
            if (row != matrixSize - 1) {
                outputFileStream << "\n";
            }
        }
        outputFileStream.close();
    }
}

// performs each of the calculations for a row to multiply a matrix
void multiplyMatrixRow(long matrixA[][matrixSize], long matrixB[][matrixSize], long matrixC[][matrixSize], int row) {
    int column;
    int offset;

    for (column = 0; column < matrixSize; column++) {
        for (offset = 0; offset < matrixSize; offset++) {
            arrayC[row][column] += arrayA[row][offset] * arrayB[offset][column];
        }
    }
}

// multiply N * N matrices. matrixA is multipled by matrixB with result put in matrixC
void parallelMatrixMultiply(long matrixA[][matrixSize], long matrixB[][matrixSize], long matrixC[][matrixSize]) {
    int row;
    // array to store 1 thread per row
    thread threads[matrixSize];

    // create each thread per row
    for (row = 0; row < matrixSize; row++) {
        threads[row] = thread(multiplyMatrixRow, matrixA, matrixB, matrixC, row);
    }

    // join the threads
    for (int i = 0; i < matrixSize; i++) {
        if (threads[i].joinable()) {
            threads[i].join();
        }
    }
}

int main() {
    srand(time(0));

    randomMatrix(arrayA);
    randomMatrix(arrayB);

    // get the current time using this c++ implementation's clock with the smallest tick period / finest resolution
    auto start = high_resolution_clock::now();

    int row;
    int column;
    int offset;
    
    parallelMatrixMultiply(arrayA, arrayB, arrayC);

    auto stop = high_resolution_clock::now();
    // time taken to run = stop time minus start time, then cast to microseconds
    auto duration = duration_cast<microseconds>(stop - start);

    int milliseconds = duration.count() / 1000;

    cout << "Time taken by function: " << duration.count() << " microseconds" << endl;
    cout << "Aka time taken by function: " << milliseconds << " milliseconds" << endl;

    createOutputFile("output_matrixC.txt", arrayC);

    return 0;
}
