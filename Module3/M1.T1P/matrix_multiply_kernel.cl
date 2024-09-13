// this kernel function takes in vectors 1 and 2 which have values, then adds them and puts result in v3. which element this kernel works on is determined by global id
__kernel void square_magnitude(const int matrixSize, __global int* arrayA, __global int* arrayB, __global int* arrayC) {
    // Thread identifiers
    const int globalIndex_row = get_global_id(0);
    const int globalIndex_column = get_global_id(1);
 
    //uncomment to see the index each PE works on
    printf("Kernel process index :(%d)\n ", globalIndex);

    for (int offset = 0; offset < matrixSize; offset++) {
        arrayC[globalIndex_row][globalIndex_column] += arrayA[row][offset] * arrayB[offset][column];
    }
}
