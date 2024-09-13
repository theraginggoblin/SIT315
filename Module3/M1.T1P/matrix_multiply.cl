// this kernel runs the inner most for loop of traditional matrix multiply. arrayA is a partition/chunk of first array to be multiplied. arrayB is entire 2nd matrix to be multiplied. arrayC is result array
__kernel void matrix_multiply(const int matrixSize, const __global long* arrayA, const __global long* arrayB, const __global long* arrayC) {
    // Thread identifiers
    const int globalIndexRow = get_global_id(0);
    const int globalIndexColumn = get_global_id(1);
 
    //uncomment to see the index each PE works on
    //printf("Kernel process index row  :(%d)  index column : (%d)\n ", globalIndexRow, globalIndexColumn);
    //printf("%d %d %d", arrayC, arrayA, arrayB);
    for (int offset = 0; offset < matrixSize; offset++) {
        arrayC[globalIndexRow][globalIndexColumn] += arrayA[globalIndexRow][offset] * arrayB[offset][globalIndexColumn];
    }
}
