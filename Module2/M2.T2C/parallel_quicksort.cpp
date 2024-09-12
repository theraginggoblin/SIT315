//#include <bits/stdc++.h>
#include <cstdlib>
#include <iostream>
#include <time.h>
#include <chrono>
#include <thread>

using namespace std::chrono;
using namespace std;

// swaps elements within an array
void array_swap(int v1[], int index1, int index2) {
    int temp = v1[index1];
    v1[index1] = v1[index2];
    v1[index2] = temp;
}

// taken from other tasks except this time random numbers up to 999
void randomVector(int vector[], int size) {
    for (int i = 0; i < size; i++) {
        // get random number modulo 100 and set at location i in array
        vector[i] = rand() % 1000;
    }
}

int reorder(int v1[], int partitionLow, int partitionHigh) {
    // taking high pivot as simple to implement
    int pivotIndex = v1[partitionHigh];
    int i = partitionLow - 1;

    // move smaller elements to left of pivot
    for (int j = partitionLow; j <= partitionHigh - 1; j++) {
        if (v1[j] < pivotIndex) {
            i++;
            array_swap(v1, i, j);
        }
    }
    
    // move pivot element to right as it's highest
    array_swap(v1, i + 1, partitionHigh);
    return i + 1;
}

void quickSort(int v1[], int partitionLow, int partitionHigh) {
    if (partitionLow < partitionHigh) {
        int pivotIndex = reorder(v1, partitionLow, partitionHigh);
        quickSort(v1, partitionLow, pivotIndex - 1);
        quickSort(v1, pivotIndex + 1, partitionHigh);
    }
}

// parallelised quicksort. creates threads when recursing unless at max depth
void parallelQuicksort(int v1[], int partitionLow, int partitionHigh, int depth, int threadsMaxDepth) {
    depth += 1;

    if (partitionLow < partitionHigh) {
        int pivotIndex = reorder(v1, partitionLow, partitionHigh);

        // if not above max depth create 2 threads and perform parallel quicksort in each
        if (depth <= threadsMaxDepth) {
            thread thread1 = thread(parallelQuicksort, v1, partitionLow, pivotIndex - 1, depth, threadsMaxDepth);
            thread thread2 = thread(parallelQuicksort, v1, pivotIndex + 1, partitionHigh, depth, threadsMaxDepth);
            if (thread1.joinable()) {
                thread1.join();
            }
            if (thread2.joinable()) {
                thread2.join();
            }
        }
        // no need to continue calling this parallel quicksort and hitting its condition checks if not required so call normal quicksort in that case
        else {
            quickSort(v1, partitionLow, pivotIndex - 1);
            quickSort(v1, pivotIndex + 1, partitionHigh);
        }
    }
}

int main() {
    int size = 2000000;
    // get number of threads equal to 2 ^ threadsMaxDepth.
    // acts as a way to limit the number of threads created
    // my pc has 16 logical cores. have found 2^8 = 256 threads to give best performance
    int threadsMaxDepth = 10;
    int depth = 0;
    int *v1;
    v1 = (int *) malloc(size * sizeof(int *));
    randomVector(v1, size);
    // get the current time using this c++ implementation's clock with the smallest tick period / finest resolution
    auto start = high_resolution_clock::now();
    parallelQuicksort(v1, 0, size - 1, depth, threadsMaxDepth);
    auto stop = high_resolution_clock::now();
    // time taken to run = stop time minus start time, then cast to microseconds
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by threaded function: " << duration.count() << " microseconds" << endl;
    cout << "Aka time taken by threaded function: " << duration.count() / 1000 << " milliseconds" << endl;

    //uncomment to verify output
    // for (int i = 0; i < size; i++) {
    //     cout << v1[i] << " ";
    // }
    return 0;
}