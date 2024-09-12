//#include <bits/stdc++.h>
#include <cstdlib>
#include <iostream>
#include <time.h>
#include <chrono>

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

int main() {
    int size = 2000000;
    int *v1;
    v1 = (int *) malloc(size * sizeof(int *));
    randomVector(v1, size);
    // get the current time using this c++ implementation's clock with the smallest tick period / finest resolution
    auto start = high_resolution_clock::now();
    quickSort(v1, 0, size - 1);
    auto stop = high_resolution_clock::now();
    // time taken to run = stop time minus start time, then cast to microseconds
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by function: " << duration.count() << " microseconds" << endl;
    cout << "Aka time taken by function: " << duration.count() / 1000 << " milliseconds" << endl;

    //uncomment to verify output
    // for (int i = 0; i < size; i++) {
    //     cout << v1[i] << " ";
    // }
    delete v1;
    return 0;
}